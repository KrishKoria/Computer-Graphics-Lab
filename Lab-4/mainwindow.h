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
    void drawCircle(int xc, int yc, int r, const QColor &color);
    void drawLine(int x1, int y1, int x2, int y2, const QColor &color);
private:
    Ui::MainWindow *ui;
    void drawPixel(int x, int y, const QColor &color);
    int xc, yc, r;

protected:
    void paintEvent(QPaintEvent* event) override;
};
#endif // MAINWINDOW_H
