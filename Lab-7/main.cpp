#include <QApplication>
#include <QMainWindow>
#include <QPainter>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QFormLayout>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>

const int INSIDE = 0; // 0000
const int LEFT = 1;   // 0001
const int RIGHT = 2;  // 0010
const int BOTTOM = 4; // 0100
const int TOP = 8;    // 1000

class LineClippingWindow : public QMainWindow {
public:
    LineClippingWindow(QWidget *parent = nullptr) : QMainWindow(parent) {
        setWindowTitle("Cohen-Sutherland Line Clipping");
        resize(800, 600);

        QGroupBox *clipWindowGroup = new QGroupBox("Clipping Window");
        QFormLayout *clipLayout = new QFormLayout;

        xminEdit = new QLineEdit("100");
        yminEdit = new QLineEdit("100");
        xmaxEdit = new QLineEdit("500");
        ymaxEdit = new QLineEdit("400");

        clipLayout->addRow("xmin:", xminEdit);
        clipLayout->addRow("ymin:", yminEdit);
        clipLayout->addRow("xmax:", xmaxEdit);
        clipLayout->addRow("ymax:", ymaxEdit);
        clipWindowGroup->setLayout(clipLayout);

        QGroupBox *lineGroup = new QGroupBox("Line Segment");
        QFormLayout *lineLayout = new QFormLayout;

        x1Edit = new QLineEdit("50");
        y1Edit = new QLineEdit("50");
        x2Edit = new QLineEdit("450");
        y2Edit = new QLineEdit("450");

        lineLayout->addRow("x1:", x1Edit);
        lineLayout->addRow("y1:", y1Edit);
        lineLayout->addRow("x2:", x2Edit);
        lineLayout->addRow("y2:", y2Edit);
        lineGroup->setLayout(lineLayout);

        clipButton = new QPushButton("Clip Line");
        connect(clipButton, &QPushButton::clicked, this, &LineClippingWindow::clipLine);

        resultLabel = new QLabel("Clipping result will appear here");
        resultLabel->setAlignment(Qt::AlignCenter);

        canvas = new QLabel;
        canvas->setMinimumSize(600, 450);
        canvas->setFrameStyle(QFrame::Box);
        canvas->setAlignment(Qt::AlignCenter);

        QVBoxLayout *controlLayout = new QVBoxLayout;
        controlLayout->addWidget(clipWindowGroup);
        controlLayout->addWidget(lineGroup);
        controlLayout->addWidget(clipButton);
        controlLayout->addWidget(resultLabel);
        controlLayout->addStretch();

        QHBoxLayout *mainLayout = new QHBoxLayout;
        mainLayout->addLayout(controlLayout);
        mainLayout->addWidget(canvas);

        QWidget *centralWidget = new QWidget;
        centralWidget->setLayout(mainLayout);
        setCentralWidget(centralWidget);

        drawScene();
    }

protected:
    int computeRegionCode(double x, double y, double xmin, double ymin, double xmax, double ymax) {
        int code = INSIDE;

        if (x < xmin)
            code |= LEFT;
        else if (x > xmax)
            code |= RIGHT;
        if (y < ymin)
            code |= BOTTOM;
        else if (y > ymax)
            code |= TOP;

        return code;
    }

    bool cohenSutherlandClip(double &x1, double &y1, double &x2, double &y2,
                             double xmin, double ymin, double xmax, double ymax,
                             QString &result) {
        int code1 = computeRegionCode(x1, y1, xmin, ymin, xmax, ymax);
        int code2 = computeRegionCode(x2, y2, xmin, ymin, xmax, ymax);

        bool accept = false;

        while (true) {
            if ((code1 | code2) == 0) {
                accept = true;
                result = "Line ACCEPTED (completely inside the window)";
                break;
            }
            else if ((code1 & code2) != 0) {
                result = "Line REJECTED (completely outside the window)";
                break;
            }
            else {
                int codeOut = code1 != 0 ? code1 : code2;

                double x, y;

                if (codeOut & TOP) {
                    x = x1 + (x2 - x1) * (ymax - y1) / (y2 - y1);
                    y = ymax;
                }
                else if (codeOut & BOTTOM) {
                    x = x1 + (x2 - x1) * (ymin - y1) / (y2 - y1);
                    y = ymin;
                }
                else if (codeOut & RIGHT) {
                    y = y1 + (y2 - y1) * (xmax - x1) / (x2 - x1);
                    x = xmax;
                }
                else if (codeOut & LEFT) {
                    y = y1 + (y2 - y1) * (xmin - x1) / (x2 - x1);
                    x = xmin;
                }


                if (codeOut == code1) {
                    x1 = x;
                    y1 = y;
                    code1 = computeRegionCode(x1, y1, xmin, ymin, xmax, ymax);
                }
                else {
                    x2 = x;
                    y2 = y;
                    code2 = computeRegionCode(x2, y2, xmin, ymin, xmax, ymax);
                }

                result = "Line CLIPPED (partially inside the window)";
            }
        }

        return accept;
    }

    void drawScene() {
        bool ok1, ok2, ok3, ok4, ok5, ok6, ok7, ok8;
        double xmin = xminEdit->text().toDouble(&ok1);
        double ymin = yminEdit->text().toDouble(&ok2);
        double xmax = xmaxEdit->text().toDouble(&ok3);
        double ymax = ymaxEdit->text().toDouble(&ok4);

        double x1 = x1Edit->text().toDouble(&ok5);
        double y1 = y1Edit->text().toDouble(&ok6);
        double x2 = x2Edit->text().toDouble(&ok7);
        double y2 = y2Edit->text().toDouble(&ok8);

        if (!ok1 || !ok2 || !ok3 || !ok4 || !ok5 || !ok6 || !ok7 || !ok8) {
            QMessageBox::warning(this, "Input Error", "Please enter valid numbers for all coordinates.");
            return;
        }

        QPixmap pixmap(canvas->width(), canvas->height());
        pixmap.fill(Qt::white);

        QPainter painter(&pixmap);
        painter.setRenderHint(QPainter::Antialiasing);


        painter.setPen(QPen(Qt::blue, 2));
        painter.drawRect(xmin, ymin, xmax - xmin, ymax - ymin);


        painter.setPen(QPen(Qt::gray, 1, Qt::DashLine));
        painter.drawLine(x1, y1, x2, y2);


        double x1_clip = x1;
        double y1_clip = y1;
        double x2_clip = x2;
        double y2_clip = y2;


        QString clipResult;
        bool accepted = cohenSutherlandClip(x1_clip, y1_clip, x2_clip, y2_clip,
                                            xmin, ymin, xmax, ymax, clipResult);


        if (accepted || clipResult.contains("CLIPPED")) {
            painter.setPen(QPen(Qt::red, 2));
            painter.drawLine(x1_clip, y1_clip, x2_clip, y2_clip);
        }


        resultLabel->setText(clipResult);


        painter.setPen(Qt::black);
        painter.drawText(10, 20, "Blue: Clipping Window");
        painter.drawText(10, 40, "Gray (dashed): Original Line");
        painter.drawText(10, 60, "Red: Clipped Line");

        canvas->setPixmap(pixmap);
    }

private slots:
    void clipLine() {
        drawScene();
    }

private:
    QLineEdit *xminEdit, *yminEdit, *xmaxEdit, *ymaxEdit;
    QLineEdit *x1Edit, *y1Edit, *x2Edit, *y2Edit;
    QPushButton *clipButton;
    QLabel *resultLabel;
    QLabel *canvas;
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    LineClippingWindow window;
    window.show();
    return app.exec();
}
