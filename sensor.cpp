#include "sensor.h"
quint8 Sensor::count = 0;
Sensor::Sensor()
{
    id = Sensor::count++;
    chart = new QChart();
    //view = new QChartView(chart);
    lineSeries = new QLineSeries();
}

void Sensor::setView(QChartView *view){
    view->setChart(chart);
}
