#include <qwt_plot.h>
#include <qwt_interval.h>
#include <qwt_system_clock.h>

class QwtPlotCurve;
class QwtPlotMarker;
class QwtPlotDirectPainter;
class QColor;
class QLabel;
class QVBoxLayout;

class Plot: public QwtPlot
{
    Q_OBJECT

public:
    Plot( QWidget * = NULL );
    virtual ~Plot();

    void start();
    virtual void replot();

    virtual bool eventFilter( QObject *, QEvent * );

    void setLegend(const double&, const double&);

public Q_SLOTS:
    void setIntervalLength( double );
    void stopIntervalUpdate();
    void changeColorCurve(const QColor& );
    void updateLegendAmplitude(const double& );
    void updateLegendFrequency(const double& );
    void showLegend(const int& );

protected:
    virtual void showEvent( QShowEvent * );
    virtual void resizeEvent( QResizeEvent * );
    virtual void timerEvent( QTimerEvent * );

private:
    void updateCurve();
    void incrementInterval();

    QwtPlotMarker *d_origin;
    QwtPlotCurve *d_curve;
    int d_paintedPoints;

    QwtPlotDirectPainter *d_directPainter;

    QwtInterval d_interval;
    int d_timerId;  // store this variable as an attribute for later stopping the plot timer

    QwtSystemClock d_clock;

    QLabel* d_label_amplitude;
    QLabel* d_label_frequency;
    QVBoxLayout* d_label_layout;
};
