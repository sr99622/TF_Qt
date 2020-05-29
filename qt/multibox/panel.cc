#include "panel.h"
#include "mainwindow.h"

#include <QtWidgets/QPushButton>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QTextEdit>
#include <QtCore/QTextStream>
#include <QtCore/QThreadPool>

using namespace std;
using namespace tensorflow;
using namespace tensorflow::ops;

Panel::Panel(QMainWindow *parent)
{
    mainWindow = parent;

    image_label = new Label;
    image_label->setMinimumWidth(480);
    image_label->setMinimumHeight(228);
    QPushButton *display = new QPushButton("display");
    display->setMaximumWidth(100);
    QGridLayout *layout = new QGridLayout;
    layout->addWidget(image_label,   0, 0, 1, 1);
    layout->addWidget(display,       1, 0, 1, 1, Qt::AlignCenter);
    setLayout(layout);

    connect(display, SIGNAL(clicked()), this, SLOT(display()));

    runner = new SessionRunner();
    connect(runner, SIGNAL(done()), this, SLOT(showOutputs()));
}

void Panel::showOutputs() 
{
    int num_detections = 5;
    TTypes<float>::Flat decoded_outputs_flat = runner->outputs[0].flat<float>();
    TTypes<float>::Flat decoded_scores_flat = runner->outputs[1].flat<float>();

    QString str;
    for (int i = 0; i < num_detections; i++) {

        QTextStream(&str) << "Detection " << i << ": "
                          << "L:" << decoded_outputs_flat(i * 4 + 0) << " "
                          << "T:" << decoded_outputs_flat(i * 4 + 1) << " "
                          << "R:" << decoded_outputs_flat(i * 4 + 2) << " "
                          << "B:" << decoded_outputs_flat(i * 4 + 3) << " " 
                          << "score: " << decoded_scores_flat(i) << endl;        

    }
    message(str);

    image_label->setImage(runner->outputs[2].SubSlice(0));
}

void Panel::display()
{   
    message(QString("Display button clicked"));

    pair<string, Tensor> p0 = {"j_filename", ::Tensor(image_path)};
    pair<string, Tensor> p1 = {"x_how_many_labels", ::Tensor(5)};
    runner->inputs = {p0, p1};

    runner->output_names = {"x_minimum", "x_sigmoid", "x_uint_image"};

    QThreadPool::globalInstance()->tryStart(runner);
}

void Panel::message(QString &arg)
{
    MW->messages->append(arg);
}

