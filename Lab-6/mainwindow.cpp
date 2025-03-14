#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QTransform>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    QTransform transform;
    transform.translate(300, 300);
    transform.rotate(0);
    painter.setTransform(transform);

    painter.setPen(Qt::red);
    drawRectangle(painter, -50, -50, 100, 100);

    painter.setPen(Qt::green);
    drawTriangle(painter, 0, 0, 50, 100, 100, 0);


    painter.setPen(Qt::blue);
    drawPolygon(painter, {QPoint(0, 0), QPoint(50, 50), QPoint(100, 0), QPoint(75, -50), QPoint(25, -50)});


    painter.setPen(Qt::yellow);
    drawMidpointCircle(painter, 0, 0, 50);


    painter.setPen(Qt::cyan);
    drawMidpointEllipse(painter, 150, 0, 60, 40);
}

void MainWindow::drawDDALine(QPainter &painter, int x1, int y1, int x2, int y2)
{
    int dx = x2 - x1;
    int dy = y2 - y1;
    int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);
    float xIncrement = dx / (float)steps;
    float yIncrement = dy / (float)steps;
    float x = x1;
    float y = y1;
    for (int i = 0; i <= steps; i++) {
        painter.drawPoint(round(x), round(y));
        x += xIncrement;
        y += yIncrement;
    }
}

void MainWindow::drawBresenhamLine(QPainter &painter, int x1, int y1, int x2, int y2)
{
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = x1 < x2 ? 1 : -1;
    int sy = y1 < y2 ? 1 : -1;
    int err = dx - dy;
    while (true) {
        painter.drawPoint(x1, y1);
        if (x1 == x2 && y1 == y2) break;
        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x1 += sx; }
        if (e2 < dx) { err += dx; y1 += sy; }
    }
}

void MainWindow::drawMidpointCircle(QPainter &painter, int xc, int yc, int r)
{
    int x = 0;
    int y = r;
    int d = 1 - r;
    drawCirclePoints(painter, xc, yc, x, y);
    while (x < y) {
        if (d < 0) {
            d += 2 * x + 3;
        } else {
            d += 2 * (x - y) + 5;
            y--;
        }
        x++;
        drawCirclePoints(painter, xc, yc, x, y);
    }
}

void MainWindow::drawCirclePoints(QPainter &painter, int xc, int yc, int x, int y)
{
    painter.drawPoint(xc + x, yc + y);
    painter.drawPoint(xc - x, yc + y);
    painter.drawPoint(xc + x, yc - y);
    painter.drawPoint(xc - x, yc - y);
    painter.drawPoint(xc + y, yc + x);
    painter.drawPoint(xc - y, yc + x);
    painter.drawPoint(xc + y, yc - x);
    painter.drawPoint(xc - y, yc - x);
}

void MainWindow::drawMidpointEllipse(QPainter &painter, int xc, int yc, int rx, int ry)
{
    int x = 0;
    int y = ry;
    int rx2 = rx * rx;
    int ry2 = ry * ry;
    int tworx2 = 2 * rx2;
    int twory2 = 2 * ry2;
    int p = round(ry2 - (rx2 * ry) + (0.25 * rx2));
    int px = 0;
    int py = tworx2 * y;

    // Region 1
    while (px < py) {
        drawEllipsePoints(painter, xc, yc, x, y);
        x++;
        px += twory2;
        if (p < 0) {
            p += ry2 + px;
        } else {
            y--;
            py -= tworx2;
            p += ry2 + px - py;
        }
    }

    // Region 2
    p = round(ry2 * (x + 0.5) * (x + 0.5) + rx2 * (y - 1) * (y - 1) - rx2 * ry2);
    while (y > 0) {
        drawEllipsePoints(painter, xc, yc, x, y);
        y--;
        py -= tworx2;
        if (p > 0) {
            p += rx2 - py;
        } else {
            x++;
            px += twory2;
            p += rx2 - py + px;
        }
    }
}

void MainWindow::drawEllipsePoints(QPainter &painter, int xc, int yc, int x, int y)
{
    painter.drawPoint(xc + x, yc + y);
    painter.drawPoint(xc - x, yc + y);
    painter.drawPoint(xc + x, yc - y);
    painter.drawPoint(xc - x, yc - y);
}

void MainWindow::drawRectangle(QPainter &painter, int x, int y, int width, int height)
{
    drawBresenhamLine(painter, x, y, x + width, y);
    drawBresenhamLine(painter, x + width, y, x + width, y + height);
    drawBresenhamLine(painter, x + width, y + height, x, y + height);
    drawBresenhamLine(painter, x, y + height, x, y);
}

void MainWindow::drawTriangle(QPainter &painter, int x1, int y1, int x2, int y2, int x3, int y3)
{
    drawDDALine(painter, x1, y1, x2, y2);
    drawDDALine(painter, x2, y2, x3, y3);
    drawDDALine(painter, x3, y3, x1, y1);
}

void MainWindow::drawPolygon(QPainter &painter, const QVector<QPoint> &points)
{
    for (int i = 0; i < points.size(); ++i) {
        drawBresenhamLine(painter, points[i].x(), points[i].y(), points[(i + 1) % points.size()].x(), points[(i + 1) % points.size()].y());
    }
}
