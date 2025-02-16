#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QPainter>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    resize(800, 600);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.scale(1.5, 1.5);
    int x1 = 50, y1 = 50;
    int x2 = 400, y2 = 300;

    drawBresenhamLine(painter, x1, y1, x2, y2);
}

void MainWindow::drawBresenhamLine(QPainter &painter, int x1, int y1, int x2, int y2) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    painter.setPen(QPen(Qt::green, 3));
    painter.drawPoint(x1, y1);

    while (x1 != x2 || y1 != y2) {
        painter.setPen(Qt::blue);
        painter.drawPoint(x1, y1);

        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }

    painter.setPen(QPen(Qt::red, 3));
    painter.drawPoint(x2, y2);
}


