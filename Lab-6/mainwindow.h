#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void paintEvent(QPaintEvent *event) override;


private:
    Ui::MainWindow *ui;
    void drawDDALine(QPainter &painter, int x1, int y1, int x2, int y2);
    void drawBresenhamLine(QPainter &painter, int x1, int y1, int x2, int y2);
    void drawMidpointCircle(QPainter &painter, int xc, int yc, int r);
    void drawCirclePoints(QPainter &painter, int xc, int yc, int x, int y);
    void drawMidpointEllipse(QPainter &painter, int xc, int yc, int rx, int ry);
    void drawEllipsePoints(QPainter &painter, int xc, int yc, int x, int y);
    void drawRectangle(QPainter &painter, int x, int y, int width, int height);
    void drawTriangle(QPainter &painter, int x1, int y1, int x2, int y2, int x3, int y3);
    void drawPolygon(QPainter &painter, const QVector<QPoint> &points);
};
#endif // MAINWINDOW_H
