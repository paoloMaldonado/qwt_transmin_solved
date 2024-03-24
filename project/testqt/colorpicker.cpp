#include "extra/colorpicker.h"

ColorPicker::ColorPicker(QWidget* parent) :
    QColorDialog(parent)
{}

void ColorPicker::showDialogWindow()
{
    QColor color = QColorDialog::getColor(Qt::red, this, "Choose a color");

    if(color.isValid())
        emit QColorDialog::colorSelected(color);
}

