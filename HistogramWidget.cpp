#include "HistogramWidget.h"

#include <QBarSet>
HistogramWidget::HistogramWidget() : QWidget()
{
    m_chart = new QChart;
    m_chart->legend()->hide();
    QChartView *chartView = new QChartView(m_chart);
    chartView->setMinimumSize(800, 600);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(chartView);
    setLayout(mainLayout);
}

void HistogramWidget::onTitle(QString title)
{
    mTitle = title;

    if (m_chart) {
        m_chart->setTitle(mTitle);
    }
}

void HistogramWidget::onXLabel(QString xLabel)
{
    mXLabel = xLabel;
    QValueAxis *axisX = static_cast<QValueAxis*>(m_chart->axisX());
    if (axisX) {
        axisX->setTitleText(mXLabel);
    }
}

void HistogramWidget::onYLabel(QString yLabel)
{
    mYLabel = yLabel;
    QValueAxis *axisY = static_cast<QValueAxis*>(m_chart->axisY());
    if (axisY) {
        axisY->setTitleText(mYLabel);
    }
}

void HistogramWidget::onXRange(qreal xMin, qreal xMax)
{
    mXMin = xMin;
    mXMax = xMax;

    QValueAxis *axisX = static_cast<QValueAxis*>(m_chart->axisX());
    if (axisX) {
        axisX->setRange(mXMin, mXMax);
        axisX->setLabelFormat("%d");
    }
}

void HistogramWidget::onYRange(qreal yMin, qreal yMax)
{
    mYMin = yMin;
    mYMax = yMax;

    QValueAxis *axisY = static_cast<QValueAxis*>(m_chart->axisY());
    if (axisY) {
        axisY->setRange(mYMin, mYMax);
    }
}

void HistogramWidget::onReceiveValues(QBarSeries *series, QValueAxis *xAxis, QValueAxis *yAxis)
{
    m_chart->removeAllSeries();
    m_chart->addSeries(series);
    m_chart->setAxisX(xAxis, series);
    m_chart->setAxisY(yAxis, series);
}
