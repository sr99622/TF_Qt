/***********************************************************************
Copyright [2020] [Stephen Rhodes]

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
