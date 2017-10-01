#include "histogramobject.h"
#include <QVector>
#include <QBarSet>
#include <QDebug>
#include <qmath.h>

HistogramObject::HistogramObject(QObject *parent) : QObject(parent)
{
    series = new QBarSeries();
    timer.start();
    timer.restart();
}

void HistogramObject::onRecieveImage(QImage image, int channels)
{
    Q_UNUSED(channels);
    if (timer.elapsed() >= 600) {
        for (int rows = 0; rows < image.height(); ++rows) {
            for (int cols = 0; cols < image.width(); ++cols) {
                values.push_back(qGray(image.pixel(rows,cols)));
            }
        }
        mXMin = 0;
        mXMax = values.size();
        updateHistogram();
        timer.restart();
    }
}

void HistogramObject::onRecieveValues(QVector<qreal> list)
{
    mBinWidth = ceil(sqrt(list.size()));
    for (int i = list.size()-1; i >= 0; --i) {
        if (!std::isnan(list[i]) && list[i] < std::numeric_limits<float>::max()) {
            list[i] *= decimalPlaces;
        } else {
            list.removeAt(i);
        }
    }
    qSort(list);

    values = list;
    mXMin = (list.first());
    mXMax = (list.last());
    updateHistogram();
}

void HistogramObject::onBinWidth(int width)
{
    mBinWidth = width;
    if (mBinWidth < 1) {
        mBinWidth = 1;
    }
    updateHistogram();
}

void HistogramObject::updateHistogram()
{
    QVector<qreal> toSend(mBinWidth+1,0);
    QList<QPair<qreal,qreal>> lineSeriesList;

    float sum = 0.0f;
    int numZeros = 0;
    int binZero = ((0.0 - mXMin) / ((mXMax - mXMin)/mBinWidth));
    for (int i = 0; i < values.size(); ++i) {
        sum +=values[i];
        int bin = ((values[i] - mXMin) / ((mXMax - mXMin)/mBinWidth));
        toSend[bin]++;
        if (binZero >= bin) {
            numZeros++;
        }
    }

}
