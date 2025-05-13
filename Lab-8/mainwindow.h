#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QComboBox>
#include <QPushButton>
#include <QColorDialog>
#include <QSlider>
#include <QLabel>
#include "drawingarea.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onAlgorithmChanged(int index);
    void onShapeChanged(int index);
    void onColorButtonClicked();
    void onClearButtonClicked();
    void onFillButtonClicked();
    void onSpeedChanged(int value);

private:
    DrawingArea *drawingArea;
    QComboBox *algorithmComboBox;
    QComboBox *shapeComboBox;
    QPushButton *colorButton;
    QPushButton *clearButton;
    QPushButton *fillButton;
    QSlider *speedSlider;
    QLabel *speedLabel;
    QColor fillColor;
};

#endif // MAINWINDOW_H
