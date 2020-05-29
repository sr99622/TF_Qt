#ifndef LABEL_H
#define LABEL_H

#include <iostream>

#include "tensorflow/core/framework/tensor.h"

#include <QtWidgets/QLabel>
#include <QtGui/QImage>
#include <QtGui/QPixmap>

using namespace tensorflow;

class Label : public QLabel
{
    Q_OBJECT

public: 
    Label();
    ~Label();

    unsigned char *buffer;

    void setImage(Tensor &contents);

};

#endif