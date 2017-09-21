#ifndef HISTOGRAMWIDGET_H
#define HISTOGRAMWIDGET_H


#include <QWidget>

#include <QChart>
#include <QBarSeries>
#include <QBarSet>

#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChart>
#include <QtWidgets/QVBoxLayout>
#include <QtCharts/QValueAxis>
#include <QXYSeries>
#include <QPair>

QT_CHARTS_BEGIN_NAMESPACE
class QLineSeries;
class QChart;
QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE


class HistogramWidget : public QWidget
{
public:
    HistogramWidget(QWidget *parent = 0);

public slots:
    void onTitle(QString title);
    void onXLabel(QString xLabel);
    void onYLabel(QString yLabel);
    void onXRange(qreal xMin,  qreal xMax);
    void onYRange(qreal yMin,  qreal yMax);
    void onReceiveValues(QList<qreal> series, QPair<qreal, qreal> xLims, QPair<qreal, qreal> yLims, float mean, float stdDev, QList<QPair<qreal, qreal> > lineSeriesList = QList<QPair<qreal,qreal>>());

    void onLineGraph(QVector<int> xaxis, QVector<float> yaxis );
protected:
    void updateView();

private:
    QChart *mChart = nullptr;
    QBarSeries *mSeries = nullptr;
    QList<QBarSet*> mSet;
    QValueAxis *axisX = nullptr;
    QValueAxis *axisY = nullptr;

    QString mTitle = QString("Title");
    QString mXLabel = QString("X Label");
    QString mYLabel = QString("Y Label");

    qreal mXMin = 0;
    qreal mXMax = 255;
    qreal mYMin = 0;
    qreal mYMax = 20000;
};

#endif // HISTOGRAMWIDGET_H
