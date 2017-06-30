#ifndef HISTOGRAMWIDGET_H
#define HISTOGRAMWIDGET_H

#include <QWidget>

#include <QChart>
#include <QBarSeries>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChart>
#include <QtWidgets/QVBoxLayout>
#include <QtCharts/QValueAxis>
#include <QXYSeries>

QT_CHARTS_BEGIN_NAMESPACE
class QLineSeries;
class QChart;
QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE


class HistogramWidget : public QWidget
{
public:
    HistogramWidget();

public slots:
    void onTitle(QString title);
    void onXLabel(QString xLabel);
    void onYLabel(QString yLabel);
    void onXRange(qreal xMin,  qreal xMax);
    void onYRange(qreal yMin,  qreal yMax);
    void onReceiveValues(QBarSeries *series, QValueAxis *xAxis, QValueAxis *yAxis);

protected:
    void updateView();

private:
    QChart *m_chart;

    QString mTitle = QString("Title");
    QString mXLabel = QString("X Label");
    QString mYLabel = QString("Y Label");

    qreal mXMin = 0;
    qreal mXMax = 10;
    qreal mYMin = 0;
    qreal mYMax = 10;
};

#endif // HISTOGRAMWIDGET_H
