#ifndef DRAWINGAREA_H
#define DRAWINGAREA_H

#include <QWidget>
#include <QImage>
#include <QPoint>
#include <QStack>
#include <QVector>
#include <QTimer>

enum FillAlgorithm {
    FloodFill,
    BoundaryFill,
    ScanLineFill
};

enum Shape {
    Rectangle,
    Circle
};

class DrawingArea : public QWidget
{
    Q_OBJECT

public:
    explicit DrawingArea(QWidget *parent = nullptr);
    void setAlgorithm(FillAlgorithm algo);
    void setShape(Shape shape);
    void setFillColor(const QColor &color);
    void clearAndDrawShape();
    void fillShape();
    void setAnimationSpeed(int speed); // 1-10, 1 is slowest

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void processFloodFillStep();
    void processBoundaryFillStep();
    void processScanLineStep();

private:
    QImage image;
    FillAlgorithm currentAlgorithm;
    Shape currentShape;
    QColor fillColor;
    QColor boundaryColor;
    QColor backgroundColor;

    QRect rectArea;
    QPoint circleCenter;
    int circleRadius;
    int animationDelay;

    QTimer *animationTimer;
    QStack<QPoint> fillStack;
    QVector<QPoint> scanLinePoints;
    int currentScanY;
    QVector<int> currentIntersections;
    int currentFillIndex;

    void initializeImage();
    void drawRectangle();
    void drawCircle();

    // Fill algorithms
    void startFloodFill(QPoint startPoint, QColor targetColor);
    void startBoundaryFill(QPoint startPoint, QColor boundaryColor);
    void startScanLineFill(const QVector<QPoint>& vertices);

    bool isInsideCircle(const QPoint &p);
    QVector<QPoint> getCirclePoints();
    QVector<QPoint> getRectanglePoints();

    void prepareScanLine(int y);
};

#endif // DRAWINGAREA_H
