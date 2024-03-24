#include "samplingthread.h"
#include "signaldata.h"
#include <qwt_math.h>
#include <math.h>

#if QT_VERSION < 0x040600
#define qFastSin(x) ::sin(x)
#endif

SamplingThread::SamplingThread(QObject* parent) :
    QwtSamplingThread(parent),
    d_frequency( 5.0 ),
    d_amplitude( 20.0 ),
    d_displacement( 0.0 )
{
}

void SamplingThread::setFrequency( double frequency )
{
    d_frequency = frequency;
}

double SamplingThread::frequency() const
{
    return d_frequency;
}

void SamplingThread::setAmplitude( double amplitude )
{
    d_amplitude = amplitude;
}

double SamplingThread::amplitude() const
{
    return d_amplitude;
}

void SamplingThread::setDisplacement( double discplacement )
{
    d_displacement = discplacement;
}

double SamplingThread::displacement() const
{
    return d_displacement;
}

void SamplingThread::sample( double elapsed )
{
    if ( d_frequency > 0.0 )
    {
        const QPointF s( elapsed, value( elapsed ) );   // (x, y) value -> sin(x)
        SignalData::instance().append( s );
    }
}

double SamplingThread::value( double timeStamp ) const
{
    const double period = 1.0 / d_frequency;

    const double x = ::fmod( timeStamp, period );
    const double v = d_amplitude * qFastSin( x / period * 2 * M_PI ) + d_displacement; // add the displacement

    return v;
}
