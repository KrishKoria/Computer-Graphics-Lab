#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QPainter>
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
    painter.setRenderHint(QPainter::Antialiasing, true);
    // Draw a red-filled rectangle
    painter.setBrush(Qt::red);
    painter.drawRect(50, 50, 200, 150);

    // Draw a yellow-filled ellipse
    painter.setBrush(Qt::yellow);
    painter.drawEllipse(300, 50, 150, 100);

    // Draw a green-filled polygon (triangle)
    painter.setBrush(Qt::green);
    static const QPointF triangle[3] = {
        QPointF(600, 50),
        QPointF(600, 200),
        QPointF(400, 200)
    };
    painter.drawPolygon(triangle, 3);

    // Draw a blue-filled polygon (pentagon)
    painter.setBrush(Qt::blue);
    static const QPointF pentagon[5] = {
        QPointF(200, 300),
        QPointF(250, 350),
        QPointF(225, 400),
        QPointF(175, 400),
        QPointF(150, 350)
    };
    painter.drawPolygon(pentagon, 5);

    // Draw a magenta-filled rounded rectangle
    painter.setBrush(Qt::magenta);
    painter.drawRoundedRect(350, 300, 200, 100, 20, 20);

    // Draw a cyan-filled arc
    painter.setBrush(Qt::NoBrush);
    painter.setPen(Qt::cyan);
    QRectF rectangle(600, 300, 100, 100);
    int startAngle = 30 * 16;
    int spanAngle = 120 * 16;
    painter.drawArc(rectangle, startAngle, spanAngle);
}
