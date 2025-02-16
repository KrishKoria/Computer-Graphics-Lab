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

private:
    Ui::MainWindow *ui;
    void drawEllipse(QPainter &painter, int xc, int yc, int rx, int ry);
    void drawCircle(QPainter &painter, int xc, int yc, int radius);

protected:
    void paintEvent(QPaintEvent *event) override;
};
#endif // MAINWINDOW_H
