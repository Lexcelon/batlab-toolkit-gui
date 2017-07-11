#include "HistogramWidget.h"

#include <QBarSet>
HistogramWidget::HistogramWidget(QWidget *parent) : QWidget(parent)
{
    mChart = new QChart;
    mChart->legend()->hide();

    for (int i =0; i < 256; ++i) {
        mSet.append(new QBarSet("",0));
        mSet[i]->append(0);
    }

    mSeries = new QBarSeries();
    mSeries->append(mSet);
    mChart->addSeries(mSeries);

    axisX = new QValueAxis(nullptr);
    axisX->setRange(mXMin, mXMax);
    axisX->setTitleText(QString());
    axisX->setLabelFormat("%g");
    axisX->setTickCount(10);
    axisY = new QValueAxis(nullptr);
    axisY->setRange(mYMin, mYMax);
    axisY->setTitleText(QString());
    mChart->setAxisX(axisX, mSeries);
    mChart->setAxisY(axisY, mSeries);

    QChartView *shartView = new QChartView(mChart);
    shartView->setMinimumSize(800, 600);
    shartView->setRenderHint(QPainter::Antialiasing);
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(shartView);
    setLayout(mainLayout);
}

void HistogramWidget::onTitle(QString title)
{
    mTitle = title;

    if (mChart) {
        mChart->setTitle(mTitle);
    }
}

void HistogramWidget::onXLabel(QString xLabel)
{
    mXLabel = xLabel;
    QValueAxis *axisX = static_cast<QValueAxis*>(mChart->axisX());
    if (axisX) {
        axisX->setTitleText(mXLabel);
    }
}

void HistogramWidget::onYLabel(QString yLabel)
{
    mYLabel = yLabel;
    QValueAxis *axisY = static_cast<QValueAxis*>(mChart->axisY());
    if (axisY) {
        axisY->setTitleText(mYLabel);
    }
}

void HistogramWidget::onXRange(qreal xMin, qreal xMax)
{
    mXMin = xMin;
    mXMax = xMax;

    QValueAxis *axisX = static_cast<QValueAxis*>(mChart->axisX());
    if (axisX) {
        axisX->setRange(mXMin, mXMax);
        axisX->setLabelFormat("%g");
    }
}

void HistogramWidget::onYRange(qreal yMin, qreal yMax)
{
    mYMin = yMin;
    mYMax = yMax;

    QValueAxis *axisY = static_cast<QValueAxis*>(mChart->axisY());
    if (axisY) {
        axisY->setRange(mYMin, mYMax);
    }
}

QColor GetColour(double v,double vmin,double vmax)
{
   QColor c = QColor(255,255,255); // white
   double dv;

   if (v < vmin) {
      v = vmin;
   }
   if (v > vmax) {
      v = vmax;
   }
   dv = vmax - vmin;


   if (v < (vmin + 0.25 * dv)) {
      c.setRed(0);
      c.setGreen(255*4 * (v - vmin) / dv);
   } else if (v < (vmin + 0.5 * dv)) {
      c.setRed(0);
      c.setBlue(255*(1 + 4 * (vmin + 0.25 * dv - v) / dv));
   } else if (v < (vmin + 0.75 * dv)) {
       c.setRed(255* 4 * (v - vmin - 0.5 * dv) / dv);
      c.setBlue(0);
   } else {
      c.setGreen((1 + 4 * (vmin + 0.75 * dv - v) / dv)*255);
      c.setBlue(0);
   }

   return(c);
}

void HistogramWidget::onReceiveValues(QList<qreal> series, QPair<qreal,qreal> xLims, QPair<qreal,qreal> yLims, float mean, float stdDev, QList<QPair<qreal,qreal>> lineSeriesList)
{
    QList<QBarSet*> sets;

    mChart->removeAllSeries();
    if (series.size() > 0) {
        QBarSeries *barSeries = new QBarSeries;
        for (int i =0; i < series.size(); ++i) {
            QBarSet *set = new QBarSet("",0);
            set->append(series[i]);
            set->setColor(GetColour(i,0,series.size()));
            sets.append(set);
        }

        barSeries->append(sets);
        barSeries->setBarWidth(1);
//        mChart->removeSeries(mSeries);
        mChart->addSeries(barSeries);
        mSeries = barSeries;
    }

    if (lineSeriesList.size() > 0) {
        QLineSeries *lineSeries = new QLineSeries();
        for (int i = 0; i < lineSeriesList.size(); ++i) {
            lineSeries->append(lineSeriesList[i].first, lineSeriesList[i].second);
        }

        mChart->addSeries(lineSeries);
        mChart->setAxisX(axisX, lineSeries);
        mChart->setAxisY(axisY, lineSeries);
    }

    mChart->setTitle(QString("Mean: %1 / Standard Deviation %2").arg(mean).arg(stdDev));
    onXRange(xLims.first,xLims.second);
    onYRange(yLims.first,yLims.second);
}

void HistogramWidget::onLineGraph(QVector<int> xaxis, QVector<float> yaxis)
{
    QLineSeries *series = new QLineSeries;
    float max = std::numeric_limits<float>::min();
    float min = std::numeric_limits<float>::max();
    for (int i =0; i < xaxis.size() && i < yaxis.size(); ++i) {
        series->append(xaxis[i],yaxis[i]);
        if (yaxis[i] > max) {
            max = yaxis[i];
        }
        if (yaxis[i] < min) {
            min = yaxis[i];
        }
    }

    mChart->removeAllSeries();
    mChart->addSeries(series);
    mChart->setAxisX(axisX,series);
    mChart->setAxisY(axisY,series);
    onYRange(min,max);
    onXRange(xaxis.first(),xaxis.last());
}
