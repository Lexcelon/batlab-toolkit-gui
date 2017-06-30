#include "HistogramObject.h"
#include <QVector>
#include <QBarSet>
#include <QDebug>

HistogramObject::HistogramObject(QObject *parent) : QObject(parent)
{
    series = new QBarSeries();
}

void HistogramObject::onRecieveImage(QImage image, int channels)
{
    values = QVector<qreal>(256, 0);
    for (int rows = 0; rows < image.height(); ++rows) {
        for (int cols = 0; cols < image.width(); ++cols) {
            values[qGray(image.pixel(rows,cols))]++;
        }
    }

    updateHistogram();
}

void HistogramObject::onRecieveValues(QVector<qreal> list)
{
    values = list;

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
    QVector<qreal> toSend(values.size(),0);
    for (int i = 0; i < values.size(); ++i) {
        toSend[(i / mBinWidth) * mBinWidth] += values[i];
    }

    series->clear();
    QBarSet *barSet = new QBarSet("Histogram");
    barSet->append(toSend.toList());

    qreal mXMin = 0;
    qreal mXMax = toSend.size();
    qreal mYMax = std::numeric_limits<int>::min();
    qreal mYMin = std::numeric_limits<int>::max();

    for (int i = 0; i < toSend.size(); ++i) {
        if (toSend[i] > mYMax) {
            mYMax = toSend[i];
        }
        if (toSend[i] < mYMin) {
            mYMin = toSend[i];
        }
    }
    QValueAxis *axisX = new QValueAxis;
    axisX->setRange(mXMin, mXMax);
    axisX->setTitleText(QString());
    axisX->setLabelFormat("%d");
    axisX->setTickCount(10);
    QValueAxis *axisY = new QValueAxis;
    axisY->setRange(mYMin, mYMax);
    axisY->setTitleText(QString());

    series->append(barSet);
    series->setBarWidth(mBinWidth);
    emit emitDisplay(series, axisX, axisY);
}
