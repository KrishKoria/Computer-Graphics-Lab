#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QGroupBox>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setWindowTitle("Shape Fill Algorithms - Animated");
    resize(800, 600);

    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    // Create drawing area
    drawingArea = new DrawingArea(this);

    // Create control panel
    QGroupBox *controlsGroup = new QGroupBox("Controls");
    QGridLayout *controlsLayout = new QGridLayout(controlsGroup);

    QLabel *algoLabel = new QLabel("Algorithm:");
    algorithmComboBox = new QComboBox();
    algorithmComboBox->addItem("Flood Fill");
    algorithmComboBox->addItem("Boundary Fill");
    algorithmComboBox->addItem("Scan Line Fill");

    QLabel *shapeLabel = new QLabel("Shape:");
    shapeComboBox = new QComboBox();
    shapeComboBox->addItem("Rectangle");
    shapeComboBox->addItem("Circle");

    colorButton = new QPushButton("Select Color");
    fillButton = new QPushButton("Fill Shape");
    clearButton = new QPushButton("Clear");

    speedLabel = new QLabel("Animation Speed:");
    speedSlider = new QSlider(Qt::Horizontal);
    speedSlider->setMinimum(1);
    speedSlider->setMaximum(10);
    speedSlider->setValue(5);
    speedSlider->setTickInterval(1);
    speedSlider->setTickPosition(QSlider::TicksBelow);

    // Add widgets to layout
    controlsLayout->addWidget(algoLabel, 0, 0);
    controlsLayout->addWidget(algorithmComboBox, 0, 1);
    controlsLayout->addWidget(shapeLabel, 0, 2);
    controlsLayout->addWidget(shapeComboBox, 0, 3);
    controlsLayout->addWidget(colorButton, 0, 4);

    controlsLayout->addWidget(speedLabel, 1, 0);
    controlsLayout->addWidget(speedSlider, 1, 1, 1, 2);
    controlsLayout->addWidget(fillButton, 1, 3);
    controlsLayout->addWidget(clearButton, 1, 4);

    mainLayout->addWidget(drawingArea, 1);
    mainLayout->addWidget(controlsGroup, 0);

    // Connect signals and slots
    connect(algorithmComboBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(onAlgorithmChanged(int)));
    connect(shapeComboBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(onShapeChanged(int)));
    connect(colorButton, SIGNAL(clicked()),
            this, SLOT(onColorButtonClicked()));
    connect(clearButton, SIGNAL(clicked()),
            this, SLOT(onClearButtonClicked()));
    connect(fillButton, SIGNAL(clicked()),
            this, SLOT(onFillButtonClicked()));
    connect(speedSlider, SIGNAL(valueChanged(int)),
            this, SLOT(onSpeedChanged(int)));

    // Set default color and speed
    fillColor = Qt::red;
    drawingArea->setAnimationSpeed(speedSlider->value());
}

MainWindow::~MainWindow()
{
}

void MainWindow::onAlgorithmChanged(int index)
{
    drawingArea->setAlgorithm(static_cast<FillAlgorithm>(index));
}

void MainWindow::onShapeChanged(int index)
{
    drawingArea->setShape(static_cast<Shape>(index));
    drawingArea->clearAndDrawShape();
}

void MainWindow::onColorButtonClicked()
{
    QColor color = QColorDialog::getColor(fillColor, this, "Select Fill Color");
    if (color.isValid()) {
        fillColor = color;
        drawingArea->setFillColor(fillColor);
    }
}

void MainWindow::onClearButtonClicked()
{
    drawingArea->clearAndDrawShape();
}

void MainWindow::onFillButtonClicked()
{
    drawingArea->fillShape();
}

void MainWindow::onSpeedChanged(int value)
{
    drawingArea->setAnimationSpeed(value);
}
