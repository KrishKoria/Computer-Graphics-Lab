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
    void drawBresenhamLine(QPainter &painter, int x1, int y1, int x2, int y2);

protected:
    void paintEvent(QPaintEvent *event) override;
};
#endif // MAINWINDOW_H
