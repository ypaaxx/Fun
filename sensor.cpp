#include "sensor.h"
quint8 Sensor::count = 0;
Sensor::Sensor()
{
    id = Sensor::count++;
    chart = new QChart();
    lineSeries = new QLineSeries();
    chart->addSeries(lineSeries);
    chart->legend()->hide();
    chart->createDefaultAxes();
    chart->axisX()->setRange(0, 60);
    chart->axisY()->setRange(0, 0xFFF);

    connect(lineSeries, SIGNAL(pointAdded(int)), this, SLOT(scroll_()) );

}

void Sensor::setView(QChartView *view){
    view->setChart(chart);
}

void Sensor::scroll_(){
    qreal x = lineSeries->pointsVector().end()->x();
    //if (x > 60)
    //chart->axisX()->
    chart->axisX()->setRange(x -60 +1, x +1);
}

void Sensor::noScroll(){
    disconnect(lineSeries, SIGNAL(pointAdded(int)), this, SLOT( scroll_() ) );
}
