#include "experiment.h"

Experiment::Experiment(QObject *parent) : QObject(parent)
{    
    angleHight = new Sensor;
    angleMiddle = new Sensor;
    angleLow = new Sensor;

    angleHight->makeAngle();
    angleMiddle->makeAngle();
    angleLow->makeAngle();

    temperature = new Sensor();
    allSensors = new QVector <Sensor*>;
    rHight = new QVector <Sensor*> (5);
    rMiddle = new QVector <Sensor*> (5);
    rLow = new QVector <Sensor*> (5);

    radiuses = new QVector <QVector <Sensor*>*>;
    *radiuses << rLow << rMiddle << rHight ;

    *allSensors << temperature;
    for(Sensor* &sensor: *rHight){
        sensor = new Sensor();
        *allSensors << sensor;
    }
    for(Sensor* &sensor: *rMiddle){
        sensor = new Sensor();
        *allSensors << sensor;
    }
    for(Sensor* &sensor: *rLow){
        sensor = new Sensor();
        *allSensors << sensor;
    }
}

void Experiment::addData(qint8 numerSensor, quint16 value)
{
    if (numerSensor > allSensors->length()) qDebug() << "numerSensor > allSensors->length()";
    allSensors->at(numerSensor)->addData(value);

    qDebug() << numerSensor << value;
}

void Experiment::getPoint(int radius, qreal fi, qreal betta)
{    
    *steam << radius << "; ";

    if (radius == 2)
        angleHight->getSeries()->append(betta, fi);

    if (radius == 1)
        angleMiddle->getSeries()->append(betta, fi);

    if (radius == 0)
        angleLow->getSeries()->append(betta, fi);

    for(Sensor* &sensor: *radiuses->at(radius)){
        *steam << sensor->getMean() << "; ";
    }

    *steam << fi << "; ";
    *steam << betta << "; " << endl;
}

bool Experiment::setFile( QFile *value){
    try{
        value->open(QIODevice::ReadWrite);
        steam = new QTextStream (value);
    } catch (...){
        return 0;
    }

    *steam << "#" << QDate::currentDate().toString("dd.MM.yyyy")
           << QTime::currentTime().toString(" hh:mm") << endl;
    *steam << "#radius; central; difference between left and right; value right and left;"
              " up; down; betta; fi;" << endl;

    return 1;
}

qreal Experiment::getMean(quint8 numer){
    if (numer > allSensors->length()-1) return 0;
    return allSensors->at(numer)->getMean();
}

