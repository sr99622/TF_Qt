#include "label.h"

using namespace std;

Label::~Label()
{

}

Label::Label()
{

}

void Label::setImage(Tensor &contents)
{
    int height = contents.dim_size(0);
    int width = contents.dim_size(1);
    int depth = contents.dim_size(2);

    enum QImage::Format format = QImage::Format_Invalid;

    switch (contents.dtype()) {
        case DataType::DT_UINT8:
            format = QImage::Format_RGB888;
        break;
    }

    if (format == QImage::Format_Invalid) {
        cout << "Label::setImage error - Unsupported image format: " << contents.dtype();
        return;
    }

    buffer = contents.tensor<uint8, 3>().data();
    setPixmap(QPixmap::fromImage(QImage(buffer, width, height, width * depth, format)));
}
