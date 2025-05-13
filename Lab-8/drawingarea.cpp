#include "drawingarea.h"
#include <QPainter>
#include <QDebug>
#include <algorithm>
#include <QApplication>

DrawingArea::DrawingArea(QWidget *parent) : QWidget(parent),
    currentAlgorithm(FloodFill),
    currentShape(Rectangle),
    fillColor(Qt::red),
    boundaryColor(Qt::black),
    backgroundColor(Qt::white),
    animationDelay(50) // 50ms default delay between steps
{
    setMinimumSize(400, 300);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // Set up animation timer
    animationTimer = new QTimer(this);

    initializeImage();
}

void DrawingArea::initializeImage()
{
    image = QImage(size(), QImage::Format_RGB32);
    image.fill(backgroundColor);

    // Define rectangle and circle dimensions based on widget size
    int margin = 50;
    rectArea = QRect(margin, margin, width() - 2 * margin, height() - 2 * margin);

    circleCenter = QPoint(width() / 2, height() / 2);
    circleRadius = qMin(width(), height()) / 3;

    clearAndDrawShape();
}

void DrawingArea::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.drawImage(0, 0, image);
}

void DrawingArea::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    initializeImage();
}

void DrawingArea::setAlgorithm(FillAlgorithm algo)
{
    currentAlgorithm = algo;
}

void DrawingArea::setShape(Shape shape)
{
    currentShape = shape;
}

void DrawingArea::setFillColor(const QColor &color)
{
    fillColor = color;
}

void DrawingArea::setAnimationSpeed(int speed)
{
    // Convert speed (1-10) to delay (500ms to 10ms)
    animationDelay = 510 - (speed * 50);
    if (animationDelay < 10) animationDelay = 10;
    if (animationDelay > 500) animationDelay = 500;
}

void DrawingArea::clearAndDrawShape()
{
    // Stop any ongoing animation
    animationTimer->stop();
    disconnect(animationTimer, nullptr, this, nullptr);

    image.fill(backgroundColor);

    if (currentShape == Rectangle) {
        drawRectangle();
    } else {
        drawCircle();
    }

    update();
}

void DrawingArea::drawRectangle()
{
    QPainter painter(&image);
    painter.setPen(QPen(boundaryColor, 2));
    painter.drawRect(rectArea);
}

void DrawingArea::drawCircle()
{
    QPainter painter(&image);
    painter.setPen(QPen(boundaryColor, 2));
    painter.drawEllipse(circleCenter, circleRadius, circleRadius);
}

void DrawingArea::fillShape()
{
    // Stop any ongoing animation
    animationTimer->stop();
    disconnect(animationTimer, nullptr, this, nullptr);

    QPoint startPoint;

    if (currentShape == Rectangle) {
        startPoint = rectArea.center();
    } else {
        startPoint = circleCenter;
    }

    switch (currentAlgorithm) {
    case FloodFill:
        startFloodFill(startPoint, backgroundColor);
        break;
    case BoundaryFill:
        startBoundaryFill(startPoint, boundaryColor);
        break;
    case ScanLineFill: {
        QVector<QPoint> vertices;
        if (currentShape == Rectangle) {
            vertices = getRectanglePoints();
        } else {
            vertices = getCirclePoints();
        }
        startScanLineFill(vertices);
        break;
    }
    }
}

// Start animated flood fill
void DrawingArea::startFloodFill(QPoint startPoint, QColor targetColor)
{
    if (!image.rect().contains(startPoint)) {
        return;
    }

    QColor currentColor = image.pixelColor(startPoint);
    if (currentColor == fillColor || currentColor != targetColor) {
        return;
    }

    // Clear and prepare the stack
    fillStack = QStack<QPoint>();
    fillStack.push(startPoint);

    // Connect timer to process one step at a time
    connect(animationTimer, &QTimer::timeout, this, &DrawingArea::processFloodFillStep);
    animationTimer->start(animationDelay);
}

void DrawingArea::processFloodFillStep()
{
    int pixelsPerStep = 10; // Process multiple pixels per timer tick for reasonable speed
    int processed = 0;

    while (!fillStack.isEmpty() && processed < pixelsPerStep) {
        QPoint p = fillStack.pop();

        // Check if point is within image boundaries and has target color
        if (!image.rect().contains(p) || image.pixelColor(p) != backgroundColor) {
            continue;
        }

        // Fill the current pixel
        image.setPixelColor(p.x(), p.y(), fillColor);
        processed++;

        // Push the four adjacent pixels
        fillStack.push(QPoint(p.x() + 1, p.y()));
        fillStack.push(QPoint(p.x() - 1, p.y()));
        fillStack.push(QPoint(p.x(), p.y() + 1));
        fillStack.push(QPoint(p.x(), p.y() - 1));
    }

    update(); // Update the display

    // Stop timer when fill is complete
    if (fillStack.isEmpty()) {
        animationTimer->stop();
    }
}

// Start animated boundary fill
void DrawingArea::startBoundaryFill(QPoint startPoint, QColor boundaryColor)
{
    if (!image.rect().contains(startPoint)) {
        return;
    }

    QColor currentColor = image.pixelColor(startPoint);
    if (currentColor == fillColor || currentColor == boundaryColor) {
        return;
    }

    // Clear and prepare the stack
    fillStack = QStack<QPoint>();
    fillStack.push(startPoint);

    // Connect timer to process one step at a time
    connect(animationTimer, &QTimer::timeout, this, &DrawingArea::processBoundaryFillStep);
    animationTimer->start(animationDelay);
}

void DrawingArea::processBoundaryFillStep()
{
    int pixelsPerStep = 10; // Process multiple pixels per timer tick
    int processed = 0;

    while (!fillStack.isEmpty() && processed < pixelsPerStep) {
        QPoint p = fillStack.pop();

        // Check if point is within image boundaries
        if (!image.rect().contains(p)) {
            continue;
        }

        QColor pixelColor = image.pixelColor(p);

        // Continue if pixel is boundary or already filled
        if (pixelColor == boundaryColor || pixelColor == fillColor) {
            continue;
        }

        // Fill the current pixel
        image.setPixelColor(p.x(), p.y(), fillColor);
        processed++;

        // Push the four adjacent pixels
        fillStack.push(QPoint(p.x() + 1, p.y()));
        fillStack.push(QPoint(p.x() - 1, p.y()));
        fillStack.push(QPoint(p.x(), p.y() + 1));
        fillStack.push(QPoint(p.x(), p.y() - 1));
    }

    update(); // Update the display

    // Stop timer when fill is complete
    if (fillStack.isEmpty()) {
        animationTimer->stop();
    }
}

// Start animated scan line fill
void DrawingArea::startScanLineFill(const QVector<QPoint>& vertices)
{
    if (vertices.size() < 3) {
        return;
    }

    // Store vertices
    scanLinePoints = vertices;

    // Find min and max y coordinates
    int yMin = vertices[0].y();
    int yMax = vertices[0].y();

    for (const QPoint& point : vertices) {
        yMin = qMin(yMin, point.y());
        yMax = qMax(yMax, point.y());
    }

    // Initialize scan line variables
    currentScanY = yMin;
    currentFillIndex = 0;
    prepareScanLine(currentScanY);

    // Connect timer to process one step at a time
    connect(animationTimer, &QTimer::timeout, this, &DrawingArea::processScanLineStep);
    animationTimer->start(animationDelay);
}

void DrawingArea::prepareScanLine(int y)
{
    currentIntersections.clear();

    // Find intersections with all edges
    for (int i = 0; i < scanLinePoints.size(); i++) {
        int j = (i + 1) % scanLinePoints.size(); // Next vertex

        int y1 = scanLinePoints[i].y();
        int y2 = scanLinePoints[j].y();

        // Check if the edge crosses the scan line
        if ((y1 <= y && y2 > y) || (y2 <= y && y1 > y)) {
            int x1 = scanLinePoints[i].x();
            int x2 = scanLinePoints[j].x();

            // Calculate x-coordinate of intersection
            if (y1 != y2) { // Avoid division by zero
                int x = x1 + (y - y1) * (x2 - x1) / (y2 - y1);
                currentIntersections.append(x);
            }
        }
    }

    // Sort intersections
    std::sort(currentIntersections.begin(), currentIntersections.end());
    currentFillIndex = 0;
}

void DrawingArea::processScanLineStep()
{
    // If we're done with current scan line, move to next
    if (currentFillIndex >= currentIntersections.size() - 1) {
        currentScanY++;

        // Check if we're done with all scan lines
        if (currentScanY > image.height()) {
            animationTimer->stop();
            return;
        }

        prepareScanLine(currentScanY);
        return;
    }

    // Fill pixels between the current pair of intersections
    int x1 = currentIntersections[currentFillIndex];
    int x2 = currentIntersections[currentFillIndex + 1];

    for (int x = x1; x <= x2; x++) {
        if (image.rect().contains(x, currentScanY)) {
            image.setPixelColor(x, currentScanY, fillColor);
        }
    }

    // Move to next pair of intersections
    currentFillIndex += 2;

    update(); // Update the display
}

bool DrawingArea::isInsideCircle(const QPoint &p)
{
    int dx = p.x() - circleCenter.x();
    int dy = p.y() - circleCenter.y();
    return (dx * dx + dy * dy) <= (circleRadius * circleRadius);
}

QVector<QPoint> DrawingArea::getRectanglePoints()
{
    QVector<QPoint> points;
    points.append(QPoint(rectArea.left(), rectArea.top()));
    points.append(QPoint(rectArea.right(), rectArea.top()));
    points.append(QPoint(rectArea.right(), rectArea.bottom()));
    points.append(QPoint(rectArea.left(), rectArea.bottom()));
    return points;
}

QVector<QPoint> DrawingArea::getCirclePoints()
{
    QVector<QPoint> points;
    const int numPoints = 36; // Number of points to approximate the circle

    for (int i = 0; i < numPoints; i++) {
        double angle = 2 * M_PI * i / numPoints;
        int x = circleCenter.x() + static_cast<int>(circleRadius * cos(angle));
        int y = circleCenter.y() + static_cast<int>(circleRadius * sin(angle));
        points.append(QPoint(x, y));
    }

    return points;
}
