#include "experiment.h"

Experiment::Experiment(QObject *parent) : QObject(parent)
{
    time = new QTime;
    timer = new QTimer;

    time->start();
    timer->setInterval(1000);
    timer->start();

    temperature = new Sensor();
    /* По пять датчиков на каждом насадке и фиктивный угла */
    rHight = new QVector <Sensor*> (5);
    for(Sensor* &sensor: *rHight)
        sensor = new Sensor();
    rMiddle = new QVector <Sensor*> (5);
    for(Sensor* &sensor: *rMiddle)
        sensor = new Sensor();
    rLow = new QVector <Sensor*> (5);
    for(Sensor* &sensor: *rLow)
        sensor = new Sensor();

    /* Добавим в allSensors ссылки на сенсоры */
    allSensors = new QVector <Sensor*> (16);
    *allSensors << temperature;
    for (Sensor* sensor: *rHight)
        *allSensors << sensor;
    for (Sensor* sensor: *rMiddle)
        *allSensors << sensor;
    for (Sensor* sensor: *rLow)
        *allSensors << sensor;
}

void Experiment::addData(qint8 numerSensor, quint16 value)
{
    allSensors->at(numerSensor)->getSeries()->append(time->elapsed(), value);
}

qreal Experiment::getFiCurrent() const
{
    return fiCurrent;
}

qreal Experiment::getBettaCurrent() const
{
    return bettaCurrent;
}

QVector<Sensor *> *Experiment::getRHight() const
{
    return rHight;
}

QVector<Sensor *> *Experiment::getRMiddle() const
{
    return rMiddle;
}

QVector<Sensor *> *Experiment::getRLow() const
{
    return rLow;
}

Sensor *Experiment::getAngleHight() const
{
    return angleHight;
}

Sensor *Experiment::getAngleMiddle() const
{
    return angleMiddle;
}

Sensor *Experiment::getAngleLow() const
{
    return angleLow;
}
