#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QPainter>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), xc(300), yc(300), r(100)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::drawPixel(int x, int y, const QColor &color)
{
    QPainter painter(this);
    painter.setPen(QPen(color, 3));
    painter.drawPoint(x, y);
}

void MainWindow::drawCircle(int xc, int yc, int r, const QColor &color)
{
    int x = 0;
    int y = r;
    int d = 1 - r;
    drawPixel(xc + x, yc + y, color); // Quadrant 1
    drawPixel(xc - x, yc + y, color); // Quadrant 2
    drawPixel(xc + x, yc - y, color); // Quadrant 4
    drawPixel(xc - x, yc - y, color); // Quadrant 3
    drawPixel(xc + y, yc + x, color); // Quadrant 1
    drawPixel(xc - y, yc + x, color); // Quadrant 2
    drawPixel(xc + y, yc - x, color); // Quadrant 4
    drawPixel(xc - y, yc - x, color); // Quadrant 3
    while (x < y)
    {
        if (d < 0) {
            d = d + 2 * x + 3;
        } else {
            d = d + 2 * (x - y) + 5;
            y--;
        }
        x++;
        drawPixel(xc + x, yc + y, color); // Quadrant 1
        drawPixel(xc - x, yc + y, color); // Quadrant 2
        drawPixel(xc + x, yc - y, color); // Quadrant 4
        drawPixel(xc - x, yc - y, color); // Quadrant 3
        drawPixel(xc + y, yc + x, color); // Quadrant 1
        drawPixel(xc - y, yc + x, color); // Quadrant 2
        drawPixel(xc + y, yc - x, color); // Quadrant 4
        drawPixel(xc - y, yc - x, color); // Quadrant 3
    }
}

void MainWindow::drawLine(int x1, int y1, int x2, int y2, const QColor &color){
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (true)
    {
        drawPixel(x1, y1, color);

        if (x1 == x2 && y1 == y2)
            break;

        int e2 = 2 * err;
        if (e2 > -dy)
        {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx)
        {
            err += dx;
            y1 += sy;
        }
    }
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    int centerX = width() / 2;
    int centerY = height() / 2;
    int radius = 50;

    drawCircle(centerX, centerY, radius, Qt::red);
    drawCircle(centerX, centerY, radius * 2, Qt::green);
    drawCircle(centerX, centerY, radius * 3, Qt::blue);

    drawLine(centerX, centerY, centerX + radius * 3, centerY, Qt::yellow);
    drawLine(centerX, centerY, centerX - radius * 3, centerY, Qt::cyan);
    drawLine(centerX, centerY, centerX, centerY + radius * 3, Qt::magenta);
    drawLine(centerX, centerY, centerX, centerY - radius * 3, Qt::red);
    drawLine(centerX, centerY, centerX + radius * 2, centerY + radius * 2, Qt::yellow);
    drawLine(centerX, centerY, centerX - radius * 2, centerY - radius * 2, Qt::cyan);
    drawLine(centerX, centerY, centerX + radius * 2, centerY - radius * 2, Qt::magenta);
    drawLine(centerX, centerY, centerX - radius * 2, centerY + radius * 2, Qt::white);
}
