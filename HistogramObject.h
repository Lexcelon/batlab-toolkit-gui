#ifndef HISTOGRAMOBJECT_H
#define HISTOGRAMOBJECT_H

#include <QObject>
#include <QImage>
#include <QBarSeries>
#include <QValueAxis>

QT_CHARTS_USE_NAMESPACE


class HistogramObject : public QObject
{
    Q_OBJECT
public:
    explicit HistogramObject(QObject *parent = 0);

signals:
    void emitHistogram(QList<qreal>);
    void emitDisplay(QBarSeries*, QValueAxis*, QValueAxis*);

public slots:
    void onRecieveImage(QImage image, int channels);
    void onRecieveValues(QVector<qreal> list);
    void onBinWidth(int width);

protected:
    void updateHistogram();

private:
    QVector<qreal> values;
    QBarSeries *series = nullptr;
    int mBinWidth = 1;
};

#endif // HISTOGRAMOBJECT_H
