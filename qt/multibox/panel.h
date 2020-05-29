#ifndef PANEL_H
#define PANEL_H

#include <iostream>

#include "tensorflow/core/framework/tensor.h"

#include <QtWidgets/QMainWindow>
#include <QtCore/QObject>
#include <QtWidgets/QWidget>
#include <QtWidgets/QLabel>
#include <QtWidgets/QTextEdit>

#include "label.h"
#include "sessionrunner.h"

using namespace std;
using namespace tensorflow;

class Panel : public QWidget
{
    Q_OBJECT

public:
    Panel(QMainWindow *parent);

    QMainWindow *mainWindow;
    Label *image_label;
    SessionRunner *runner;

    string image_path = "tensorflow/examples/multibox_detector/data/surfers.jpg";

    Tensor original_image_tensor;

    void message(QString &arg);

public slots:
    void display();
    void showOutputs();
};


#endif // PANEL_H

