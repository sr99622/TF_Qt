/***********************************************************************
Copyright 2020 Stephen Rhodes
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at
    http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
************************************************************************/

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

