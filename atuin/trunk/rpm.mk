DATE = `date '+%Y%m%d%H%M'`

VERSION ?= 0.1

RPMDIRS=        rpm/BUILD rpm/RPMS/i386 rpm/RPMS/i586 rpm/RPMS/i686 rpm/RPMS/athlon \
                rpm/RPMS/noarch rpm/RPMS/k6 rpm/RPMS/ppc rpm/SOURCES rpm/SPECS \
                rpm/SRPMS rpm/tmp

tar: rpmdirs admin/* slicker.spec
	@tar --exclude=./rpm -cvzf rpm/SOURCES/slicker-$(DATE).tar.gz .

rpmdirs:
	@mkdir -p $(RPMDIRS)

rpm: rpmdirs slicker.spec tar
	@echo "building RPM..."
	@rpmbuild -ba slicker.spec --define "_topdir $(PWD)/rpm" --define "_tmppath $(PWD)/rpm/tmp" --with snapshot

%.spec: %.spec.in
	@echo -e "generating $@..."
	sed -e "s#@@@date@@@#$(DATE)#g" -e "s#@@@version@@@#$(VERSION)#g" $< > $@
