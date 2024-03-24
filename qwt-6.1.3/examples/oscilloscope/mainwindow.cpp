#include "mainwindow.h"
#include "plot.h"
#include "knob.h"
#include "wheelbox.h"
#include <qwt_scale_engine.h>
#include <qlabel.h>
#include <qlayout.h>
#include <QPushButton>
#include <qcheckbox.h>
#include <qcolordialog.h>
#include <qlistwidget.h>
#include <qpixmap.h>
#include <qfiledialog.h>
#include "extra/colorpicker.h"
#include "extra/recordlist.h"

int MainWindow::itemCounter = 0;

MainWindow::MainWindow( QWidget *parent ):
    QWidget( parent )
{
    const double intervalLength = 10.0; // seconds

    d_plot = new Plot( this );
    d_plot->setIntervalLength( intervalLength );

    // a hidden plot clone for capturing a screenshot, it has a fixed size of 400, 400
    d_plotForCapture = new Plot( this );
    d_plotForCapture->setFixedSize( 400, 400 );
    d_plotForCapture->setIntervalLength( intervalLength );
    d_plotForCapture->setVisible(false);

    d_itemList = new RecordList(this); // Item list
    d_itemList->updateGeometry();

    d_amplitudeKnob = new Knob( "Amplitude", 0.0, 200.0, this );
    d_amplitudeKnob->setValue( 160.0 );

    d_frequencyKnob = new Knob( "Frequency [Hz]", 0.1, 20.0, this );
    d_frequencyKnob->setValue( 17.8 );

    d_intervalWheel = new WheelBox( "Displayed [s]", 1.0, 100.0, 1.0, this );
    d_intervalWheel->setValue( intervalLength );

    d_timerWheel = new WheelBox( "Sample Interval [ms]", 0.0, 20.0, 0.1, this );
    d_timerWheel->setValue( 10.0 );

    d_colorDialog = new ColorPicker(this);
    
    d_plot->setLegend(d_amplitudeKnob->value(), d_frequencyKnob->value());

    QVBoxLayout* vLayout1 = new QVBoxLayout();
    vLayout1->addWidget( d_intervalWheel );
    vLayout1->addWidget( d_timerWheel );
    vLayout1->addStretch( 10 );
    vLayout1->addWidget( d_amplitudeKnob );
    vLayout1->addWidget( d_frequencyKnob );

    // vertical layout contains: stop, foto actual, change color, legend and vertical displacement wheel
    // adding a vertical layout and then makes it a child of the horizontal layout
    d_stopButton = new QPushButton("Stop", this);

    d_photoButton = new QPushButton("Foto actual", this);

    d_colorButton = new QPushButton("Change Color", this);

    d_legendButton = new QCheckBox("Show Legend", this);

    d_verticalDisplWheel = new WheelBox("Mover verticalmente", -100.0, 100.0, 5.0, this);
    d_verticalDisplWheel->setValue(0.0);

    QVBoxLayout* vLayout2 = new QVBoxLayout();
    vLayout2->addWidget( d_stopButton );
    vLayout2->addWidget(d_photoButton);
    vLayout2->addWidget(d_colorButton);
    vLayout2->addStretch(10);
    vLayout2->addWidget(d_legendButton);
    vLayout2->addWidget(d_verticalDisplWheel);

    layout = new QHBoxLayout( this );
    layout->addWidget( d_plot, 10 );
    layout->addLayout( vLayout1 );
    layout->addLayout( vLayout2 );
    layout->addWidget( d_itemList );

    currentItemDataLayout = nullptr;

    connect( d_amplitudeKnob, SIGNAL( valueChanged( double ) ),
        SIGNAL( amplitudeChanged( double ) ) );
    connect( d_frequencyKnob, SIGNAL( valueChanged( double ) ),
        SIGNAL( frequencyChanged( double ) ) );
    connect( d_timerWheel, SIGNAL( valueChanged( double ) ),
        SIGNAL( signalIntervalChanged( double ) ) );
    // connecting signals to use in the main.cpp file
    connect(d_stopButton, SIGNAL( pressed() ),
        SIGNAL( stopButtonPressed() ) );
    connect(d_verticalDisplWheel, SIGNAL( valueChanged(double) ),
        SIGNAL( displacementChanged( double ) ) );

    connect( d_intervalWheel, SIGNAL( valueChanged( double ) ),
        d_plot, SLOT( setIntervalLength( double ) ) );
    // connect default signal pressed to the slot stopIntervalUpdate which stops the timer in Plot
    connect(d_stopButton, SIGNAL( pressed() ),
        d_plot, SLOT( stopIntervalUpdate() ) );

    // connecting the intervalWheel and stopButton signals also to the plot clone 
    connect(d_intervalWheel, SIGNAL( valueChanged(double) ),
        d_plotForCapture, SLOT( setIntervalLength(double) ) );
    connect(d_stopButton, SIGNAL( pressed() ),
        d_plotForCapture, SLOT( stopIntervalUpdate() ) );

    
    // first connect the color button to the dialog and then connect the dialog to 
    // the changeColorCurve() slot using the selected color
    connect(d_colorButton, SIGNAL( pressed() ),
        d_colorDialog, SLOT( showDialogWindow() ) );
    connect(d_colorDialog, SIGNAL( colorSelected( QColor ) ),
        d_plot, SLOT( changeColorCurve( QColor ) ) );
    // Also for the plot clone
    connect(d_colorDialog, SIGNAL( colorSelected(QColor) ),
        d_plotForCapture, SLOT( changeColorCurve(QColor) ) );

    // show the legend
    connect(d_amplitudeKnob, SIGNAL( valueChanged(double) ), 
        d_plot, SLOT( updateLegendAmplitude(double) ) );
    connect(d_frequencyKnob, SIGNAL(valueChanged(double)),
        d_plot, SLOT(updateLegendFrequency(double)));
    connect(d_legendButton, SIGNAL( stateChanged(int) ),
        d_plot, SLOT( showLegend(int) ) );

    connect(d_photoButton, SIGNAL( pressed() ),
        this, SLOT( handlePhotoButtonPressed() ) );
    connect(d_itemList, SIGNAL( itemPressed(QListWidgetItem* ) ),
        this, SLOT( handleItemSelected(QListWidgetItem* ) ) );
}

void MainWindow::start()
{
    d_plot->start();
    d_plotForCapture->start();
}

double MainWindow::frequency() const
{
    return d_frequencyKnob->value();
}

double MainWindow::amplitude() const
{
    return d_amplitudeKnob->value();
}

double MainWindow::signalInterval() const
{
    return d_timerWheel->value();
}

void MainWindow::handlePhotoButtonPressed()
{
    double currentAmplitude = this->amplitude();
    double currentFrequency = this->frequency();

    QListWidgetItem* item = new QListWidgetItem("item " + QString::number(++itemCounter), d_itemList);
    item->setData(Qt::UserRole, currentAmplitude);
    item->setData(Qt::UserRole+1, currentFrequency); 
    item->setData(Qt::UserRole+2, d_plotForCapture->grab());

    d_itemList->addItem(item);
}

void MainWindow::handleExportButtonPressed()
{
    QString filePath = QFileDialog::getSaveFileName(this, tr("Save File"), "screen_plot", tr("Images (*.png *.xpm *.jpg)"));
    d_imageGrabbed.save(filePath);
}

void MainWindow::handleItemSelected(QListWidgetItem* item)
{
    // remove the layout (if exists)
    if (currentItemDataLayout)
    {
        layout->removeItem(currentItemDataLayout);
      
        QLayoutItem* child;
        while ((child = currentItemDataLayout->takeAt(0)) != nullptr) {
            delete child->widget(); // delete the widget
            delete child;           // delete the layout item
        }
    }

    // create a new grid layout
    QGridLayout* newItemDataLayout = new QGridLayout();

    // extract the data from the item selected
    QString item_name = item->text();
    QString amplitude = QString::number(item->data(Qt::UserRole).toDouble());
    QString frequency = QString::number(item->data(Qt::UserRole + 1).toDouble());

    QLabel* data_text = new QLabel("Imagen: "    + item_name + ", " + 
                                   "Amplitude: " + amplitude + ", " + 
                                   "Frequency: " + frequency);

    QLabel* text_screenshot = new QLabel("Widget Vista de imagen:");

    QLabel* screenshot = new QLabel();
    d_imageGrabbed = item->data(Qt::UserRole + 2).value<QPixmap>();
    d_imageGrabbed = d_imageGrabbed.scaled(400, 400, Qt::KeepAspectRatio); // screenshot and then resize 
    screenshot->setPixmap(d_imageGrabbed);

    QPushButton* d_exportScreenButton = new QPushButton("Export Image");

    // this empty widget helps for positioning all the widgets to the top of the window
    QWidget* empty = new QWidget();
    empty->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);

    // adding all the widgets to the grid layout
    newItemDataLayout->addWidget(data_text, 0, 0);
    newItemDataLayout->addWidget(text_screenshot, 1, 0);
    newItemDataLayout->addWidget(d_exportScreenButton, 1, 1);
    newItemDataLayout->addWidget(screenshot, 2, 0, 1, 2);
    newItemDataLayout->addWidget(empty);

    // add the newly created grid layout to the main layout of the program
    layout->addLayout(newItemDataLayout);
    
    // set the newly created grid layout to the current layout 
    // this is neccesary because I want to delete the layout when I select a different item
    currentItemDataLayout = newItemDataLayout;

    // connecting the export screenshot button to the slot that handles the save image action 
    connect(d_exportScreenButton, SIGNAL( pressed() ),
            this, SLOT( handleExportButtonPressed() ) );
}
