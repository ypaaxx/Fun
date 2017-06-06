#include "sensor.h"

Sensor::Sensor(QObject *parent) : QObject(parent)
{
    chart = new QChart();
    view = new QChartView(chart);
    p_t = new QLineSeries();

    p_t->append(1,1);
    p_t->append(1.5,2);

    chart->addSeries(p_t);
    chart->createDefaultAxes();
    chart->axisX()->setRange(0,5);
    chart->axisY()->setRange(0, 5);
}
