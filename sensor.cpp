#include "sensor.h"

QTimer *Sensor::timer = new QTimer;
QTime *Sensor::time = new QTime;

Sensor::Sensor()
{
    chart = NULL;
    series = NULL;
    lastValues = new QList<quint16>;

    //setCalibrationCoefficients(0, -1245.41);
}

void Sensor::makeChart()
{
    MAX_SIZE = 50;
    chart = new QChart();
    series = new QLineSeries();
    chart->addSeries(series);
    chart->legend()->hide();
    chart->createDefaultAxes();
    chart->axisX()->setRange(0, 60);
    chart->axisX()->hide();
    chart->axisY()->setGridLineVisible(false);
    //chart->axisY()->setRange(-1245.41, 1245.41);
    chart->axisY()->setRange(0, 0xFFF);
    connect(timer, SIGNAL(timeout()), this, SLOT(scroll_()));
}

void Sensor::addData(int value) {

    //Если это первый вызов и таймер ещё не запущен
    if(!timer->isActive()){
        timer->start(1000);
        time->start();
    }

    qreal now = time->elapsed()/1000.0;
    if (series)
        series->append(now, value);

    if (numberValues >= LENGHT_AVE)
        (*lastValues)[numberValues % LENGHT_AVE] = value;
    else
        lastValues->append(value);
    numberValues++;

    int localMean = 0;
    for (auto value: *lastValues)
        localMean += value;
    mean = (qreal) localMean/LENGHT_AVE;
}

void Sensor::scroll_(){

    static const quint8 timeLenght = 60;

    int now = time->elapsed()/1000;
    if (now < timeLenght) return;

    if(series->count() > MAX_SIZE) series->removePoints(0,10);
    if(!chart) return;

    chart->axisX()->setRange(now - timeLenght, now);
}

void Sensor::makeAngle()
{
    if(!chart) makeChart();
    disconnect(timer, SIGNAL(timeout()), this, SLOT(scroll_()));
    chart->axisX()->setRange(-24, 24);
    chart->axisX()->visibleChanged(true);
    chart->axisY()->setRange(-180, 180);
}

