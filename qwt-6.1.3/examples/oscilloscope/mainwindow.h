#include <qwidget.h>

class Plot;
class Knob;
class WheelBox;
class QPushButton;
class QCheckBox;
class ColorPicker;
class QLabel;
class RecordList;
class QHBoxLayout;
class QGridLayout;
class QListWidgetItem;
class QPixmap;

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow( QWidget * = NULL );

    void start();

    double amplitude() const;
    double frequency() const;
    double signalInterval() const;

Q_SIGNALS:
    void amplitudeChanged( double );
    void frequencyChanged( double );
    void signalIntervalChanged( double );
    void displacementChanged( double );
    void stopButtonPressed();
    void colorButtonPressed();

public Q_SLOTS:
    void handlePhotoButtonPressed();
    void handleExportButtonPressed();
    void handleItemSelected(QListWidgetItem* );

private:
    QHBoxLayout* layout;
    QGridLayout* currentItemDataLayout;

    Knob *d_frequencyKnob;
    Knob *d_amplitudeKnob;
    WheelBox *d_timerWheel;
    WheelBox *d_intervalWheel;

    QPushButton *d_stopButton;
    QPushButton* d_photoButton;
    QPushButton* d_colorButton;
    QCheckBox* d_legendButton;
    WheelBox* d_verticalDisplWheel;
    RecordList* d_itemList;

    ColorPicker* d_colorDialog;

    Plot *d_plot;
    Plot* d_plotForCapture;
    // Assuming there will be only one image to capture, the image grabbed
    // is the only one to be saved to hard disk
    QPixmap d_imageGrabbed; 

    static int itemCounter;
};
