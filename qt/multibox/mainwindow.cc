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

#include "mainwindow.h"

#include <QtWidgets/QSplitter>
#include <QtWidgets/QBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Qt for Tensorflow");
    panel = new Panel(this);
    messages = new QTextEdit();
    messages->setMinimumWidth(600);

    QSplitter *splitter = new QSplitter();
    splitter->addWidget(panel);
    splitter->addWidget(messages);

    QWidget *mainPanel = new QWidget();
    QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget(splitter);
    mainPanel->setLayout(layout);

    setCentralWidget(mainPanel);
}

MainWindow::~MainWindow()
{
}

