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

