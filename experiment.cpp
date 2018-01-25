#include "experiment.h"

Experiment::Experiment(QObject *parent) : QObject(parent)
{
}

// Добавить новое значение на датчике
void Experiment::addData(qint8 numerSensor, quint16 value)
{
    if (numerSensor > allSensors->length()) qDebug() << "numerSensor > allSensors->length()";
    allSensors->at(numerSensor)->addData(value);

    qDebug() << numerSensor << value;
}

bool Experiment::setFile( QFile *value){
    try {
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
