#ifndef HISTOGRAMOBJECT_H
#define HISTOGRAMOBJECT_H

#include <QObject>
#include <QImage>
#include <QBarSeries>
#include <QValueAxis>
#include <QTime>
#include <QPair>


QT_CHARTS_USE_NAMESPACE


class HistogramObject : public QObject
{
    Q_OBJECT
public:
    explicit HistogramObject(QObject *parent = 0);

signals:
    void emitHistogram(QList<qreal>);
//    void emitDisplay(QBarSeries*, QValueAxis*, QValueAxis*);
//    void emitDisplay(QList<QBarSet*>, QValueAxis*, QValueAxis*);
    void emitDisplay(QList<qreal>, QPair<qreal,qreal>, QPair<qreal,qreal>,float,float,QList<QPair<qreal,qreal>>);

public slots:
    void onRecieveImage(QImage image, int channels);
    void onRecieveValues(QVector<qreal> list);
    void onBinWidth(int width);

protected:
    void updateHistogram();

private:
    QVector<qreal> values;
    QBarSeries *series = nullptr;
    int mBinWidth = 256;
    qreal mXMin = 0;
    qreal mXMax = 0;
    QTime timer;
    int decimalPlaces = 10000;
};

#endif // HISTOGRAMOBJECT_H
