#include "edgewidgethelpers.h"

#include <kapplication.h>
#include <kdebug.h>

// ------- Transform helper-matrices -------------
/*
  Transform-matrices explained:
  A transform-matrix describes the geometrical state of an edge,
  according to some certain transform rules.
  The rows are in the order: TopEdge, RightEdge, BottomEdge, LeftEdge
  The first column shows whether this state is vertical
  The second column shows whether this state has a negative X-axis
  The third column shows whether this state has a negative Y-axis
 
  All this information is used in the applyMatrix() to transform geometry
  on EdgeChanges.
*/
char mirrorMatrix[4][3] = { {0, 0, 0},
                            {1, 1, 0},
                            {0, 0, 1},
                            {1, 0, 0} };

char simpleMatrix[4][3] = { {0, 0, 0},
                            {1, 0, 0},
                            {0, 0, 0},
                            {1, 0, 0} };

enum {Transpose = 0, NegativeX = 1, NegativeY = 2};

char *getRowForEdge(EdgeWidget::ScreenEdge edge, char matrix[4][3])
{
    switch (edge) {
    case EdgeWidget::TopEdge:
        return matrix[0];
    case EdgeWidget::RightEdge:
        return matrix[1];
    case EdgeWidget::BottomEdge:
        return matrix[2];
    case EdgeWidget::LeftEdge:
        return matrix[3];
    default:
        Q_ASSERT(0);
        return 0;
    }
}
// ------- End transform-helper matrices ----------

EdgeWidgetTransformHelper::EdgeWidgetTransformHelper(const QWidget *parent, EdgeWidget::ScreenEdge oldEdge, EdgeWidget::ScreenEdge newEdge, TransformMode mode)
{
    _parent = parent;
    _oldEdge = oldEdge;
    _newEdge = newEdge;

    switch (mode) {
    case TransformMirror:
        memcpy(_matrix, mirrorMatrix, sizeof(_matrix));
        break;
    case TransformSimple:
    default:
        memcpy(_matrix, mirrorMatrix, sizeof(_matrix));
    }
}

EdgeWidgetTransformHelper::~EdgeWidgetTransformHelper()
{}

QWidget &EdgeWidgetTransformHelper::transform(QWidget &widget)
{
    QRect geom = widget.geometry();

    // Transform the size and stuff in between to avoid problems when we try to resize the geometry.
    QSize tmpSize = widget.minimumSize();
    transform(tmpSize);
    widget.setMinimumSize(tmpSize);

    tmpSize = widget.maximumSize();
    transform(tmpSize);
    widget.setMaximumSize(tmpSize);

    // The widget should now accept the new size, since minimumSize and maximumSize have been update.
    transform(geom);
    widget.setGeometry(geom);

    return widget;
}

QRect &EdgeWidgetTransformHelper::transform( QRect &rect )
{
    // This function is simple, just find two opposite corners, apply the appropriate matrice to each point,
    // set the new corner coordinates and normalize the rectangle.

    QPoint topLeft = rect.topLeft();
    QPoint bottomRight = rect.bottomRight();

    transform(topLeft);
    transform(bottomRight);

    rect.setTopLeft(topLeft);
    rect.setBottomRight(bottomRight);

    rect = rect.normalize();

    return rect;
}

QSize &EdgeWidgetTransformHelper::transform( QSize &size )
{
    char *oldState = getRowForEdge(_oldEdge, _matrix);
    char *newState = getRowForEdge(_newEdge, _matrix);

    if (oldState[Transpose] != newState[Transpose])
        size.transpose();

    return size;
}

QPoint &EdgeWidgetTransformHelper::transform( QPoint &point )
{
    return applyMatrix(point);
}

QPoint &EdgeWidgetTransformHelper::applyMatrix(QPoint &point)
{
    // This function is a bit tricky.

    // First we get out all variables that matters
    char *oldState = getRowForEdge(_oldEdge, _matrix);
    char *newState = getRowForEdge(_newEdge, _matrix);
    int newWidth = _parent->width();
    int newHeight = _parent->height();
    int oldWidth = newWidth;   // We're gonna fix this in a couple of lines.
    int oldHeight = newHeight; // -||-
    int newX = point.x();
    int newY = point.y();

    // Actually, we don't know what the old size were, before we look at the oldState compared to the new state
    // and possibly transpose the size.
    if (oldState[Transpose] != newState[Transpose]) {
        oldWidth = newHeight;
        oldHeight = newWidth;
    }

    // Adjust X and Y according to the used coordinate-system used in the old state.
    if (oldState[NegativeX])
        newX = oldWidth - newX;
    if (oldState[NegativeY])
        newY = oldHeight - newY;

    // Ok, now if the size were transposed during the change, we definately want to transpose the coordinates as well.
    if (oldState[Transpose] != newState[Transpose]) {
        int temp;
        temp = newX;
        newX = newY;
        newY = temp;
    }

    // And we transform the coordinates according to the new coordinate-system.
    if (newState[NegativeX])
        newX = newWidth - newX;
    if (newState[NegativeY])
        newY = newHeight - newY;

    // We're done, update the info to the point.
    point.setX(newX);
    point.setY(newY);

    return point;
}

EdgeWidget::ScreenEdge oppositeEdge(EdgeWidget::ScreenEdge e)
{
    switch (e) {
    case EdgeWidget::TopEdge:
        return EdgeWidget::BottomEdge;
    case EdgeWidget::BottomEdge:
        return EdgeWidget::TopEdge;
    case EdgeWidget::LeftEdge:
        return EdgeWidget::RightEdge;
    case EdgeWidget::RightEdge:
        return EdgeWidget::LeftEdge;
    default:
        return EdgeWidget::SameEdge;
    }
}

EdgeWidgetBoxLayout::EdgeWidgetBoxLayout( QWidget* parent, EdgeWidget *edgeWidget, Orientation orientation, const char* name )
        : QBoxLayout( parent, QBoxLayout::LeftToRight, 0, 0, name )
{
    _edgeWidget = edgeWidget;
    _orientation = orientation;
    setDirection(realDirection());
}

QBoxLayout::Direction EdgeWidgetBoxLayout::realDirection() const
{
    bool isHorizontal;
    bool isPerpendicular = _orientation == Perpendicular;

    if (_edgeWidget)
        isHorizontal = _edgeWidget->orientation() == Horizontal;
    else
        isHorizontal = true;

    if ((isPerpendicular || isHorizontal) && (isPerpendicular != isHorizontal))
        return QBoxLayout::LeftToRight;
    else
        return QBoxLayout::TopToBottom;
}

void EdgeWidgetBoxLayout::invalidate()
{
    setDirection(realDirection());
    QBoxLayout::invalidate();
}

EdgeWidgetLayoutBox::EdgeWidgetLayoutBox(QWidget* parent, EdgeWidget *edgeWidget, EdgeWidgetBoxLayout::Orientation orientation, const char* name )
        : QFrame( parent, name )
{
    _layout = new EdgeWidgetBoxLayout(this, edgeWidget, orientation, name);
    _layout->setAutoAdd(true);
}

void EdgeWidgetLayoutBox::relayout()
{
    _layout->invalidate();
    updateGeometry();
}

#include "edgewidgethelpers.moc"





