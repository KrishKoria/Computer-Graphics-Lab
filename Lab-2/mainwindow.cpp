#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QPainter>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    resize(800,600);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent* event){
    QPainter painter(this);
    // painter.scale(2.0, 2.0);
    drawLineDDA(painter, 500, 500, 100, 210);
}

void MainWindow::drawLineDDA(QPainter &painter, int x1, int y1, int x2, int y2){
    int dx = x2 - x1;
    int dy = y2 - y1;
    int steps = std::max(abs(dx), abs(dy));

    float x_inc = dx / float(steps);
    float y_inc = dy / float(steps);

    float x = x1;
    float y = y1;

    for(int i = 0; i < steps; i++){
        painter.drawPoint(round(x), round(y));

        x += x_inc;
        y += y_inc;
    }
}
