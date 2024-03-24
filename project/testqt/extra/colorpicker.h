#include <qcolordialog.h>

class ColorPicker : public QColorDialog
{
    Q_OBJECT

public:
    ColorPicker(QWidget* = NULL);

public Q_SLOTS:
    void showDialogWindow();
};