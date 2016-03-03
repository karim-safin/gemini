/* KPCC
 * GridPainter is a widget able to draw Grids
 * Author: Safin Karim, Alexandra Balyuk
 * Date: 2015.09.05
 */

#ifndef GRIDPAINTER
#define GRIDPAINTER

#include <QBrush>
#include <QMouseEvent>
#include <QOpenGLWidget>
#include <QPaintEvent>
#include <QPen>
#include <QColor>
#include <QVector>
#include <QWheelEvent>
#include <QWidget>

#include "grid.h"

enum MOUSE_MODE
{
    DRAWING,
    MOVING,
    ERASING
};

class GridPainter : public QOpenGLWidget
{
    Q_OBJECT
private:
    Grid grid;                // grid with cells

    QVector<Grid> painting;   // patterns that we paint with
    int currentPaintingIndex; // exact pattern used for drawing
    QVector<Grid> erasing;    // patterns that we erase with
    int currentErasingIndex;  // exact pattern used for drawing

    bool stopped;             // true if the field is updating continuously
    
    QColor cellColor;         // color of a living cell
    QColor spaceColor;        // color of a dead cel;
    QColor gridColor;         // color of a grid
    
    QBrush cellBrush;         // brush used to paint cells(plain, spaceColor)

    QPen gridPen;             // pen for drawing grid (has gridColor)

    double mouseScrollSensitivity; // field increases in size (mouseScrollSensitivity) times after each scroll

    QPoint topLeftDrawingPosition;
    QPoint bottomRightDrawingPosition;
    // The entire field is being drawn between this two points

    // Does its best to fit the entire field in the screen
    void autoFitDrawingPoints();

    // If the grid chaged its size after updating, then two drawing points
    // change their positions so that the cell remains the same size
    void preventResizing(int prevGridWidth, int currentGridWidth, int prevFieldWidth);

    QPoint mousePosition; // position of a mouse; we need to store it to draw/move

    bool mousePressed;

    MOUSE_MODE mode; // how we process mouse events

public:
    GridPainter(QWidget *parent);
    void setCellColor(QColor cc);
    void setSpaceColor(QColor sc);
    void setGridColor(QColor gc);

    int getGenerationCount();
    long getPopulation();

    // creates a square grid, whose side is bigger that width and height
    void initEmptyGrid(int width, int height);

    void parsePlainText(const QString &fileName);
    void parseRLE(const QString &fileName);
    void setMouseMode(MOUSE_MODE m);
    void initRandom(int width, int height);
    bool isStopped();

    // These two parse text files and put that in corresponding Grids
    void setDrawingPattern(int index, const QString& fileName);
    void setErasingPattern(int index, const QString& fileName);
    // sets a pattern, no matter the mode. Used by UserInterface::keyPressEvent
    void setCurrentPattern(int index);
    void setCurrentDrawingPattern(int index);
    void setCurrentErasingPattern(int index);

public slots:
    void animate();
    void stopPressed();
    void clear();
    void rotateClockwise();
    void rotateAntiClockwise();
    void nextGeneration();

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
#ifndef QT_NO_WHEELEVENT
    void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;
#endif
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
};

#endif // GRIDPAINTER
