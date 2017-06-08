#include "sensor.h"
quint8 Sensor::count = 0;
Sensor::Sensor()
{
    id = Sensor::count++;
    chart = new QChart();
    //view = new QChartView(chart);
    lineSeries = new QLineSeries();
    chart->addSeries(lineSeries);
    chart->legend()->hide();
    chart->createDefaultAxes();
    chart->axisX()->setRange(0,2*60);
    chart->axisY()->setRange(0,0xFFF);
}

void Sensor::setView(QChartView *view){
    view->setChart(chart);
}
