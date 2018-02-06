#ifndef EXPERIMENT_H
#define EXPERIMENT_H

#include <QObject>
#include <QDebug>
#include <QVector>
#include <QtCharts>
#include "QTime"
#include "QTimer"
#include "QtMath"

#include "sensor.h"
#include "arduino.h"

class Experiment : public QObject
{
    Q_OBJECT
public:

    explicit Experiment(QObject *parent = 0);
    void addData(qint8 numerSensor, quint16 value);

    //Установка списка датчиков извне
    void setAllSensors(QVector <Sensor*> *_allSensors){
        allSensors = _allSensors;
    }

    void setSteam(const QTextStream &value);
    QTextStream getSteam() const;

    QFile getFile() const;
    bool setFile(QFile *value);

    bool isFileSet(){ return file->isOpen(); }

    bool setCalibrationFile(QFile *file){
        qDebug() << "Set calibration coefficient from file" << file->fileName();
        if (!file->open(QIODevice::ReadOnly)) return false;

        steam = new QTextStream (file);
        int numSensor;
        qreal A,B;

        auto line = new QString;
        QStringList arr;

        while (steam->readLineInto(line)){

            if (line->at(0) == "#") continue;
            arr = line->split(';');
            try{
                numSensor =  arr.at(0).toInt();
                A = arr.at(1).toDouble();
                B = arr.at(2).toDouble() ;
            } catch (...){
                qDebug() << "Неформат";
                return false;
            }

            allSensors->at(numSensor)->setCalibrationCoefficients(A, B);
        }
        delete steam;
        file->close();

        return true;
    }


private:

    QVector <Sensor*> *allSensors; //Список всех реальных сенсоров

    // Текущие значения Fi Betta
    qreal fiCurrent, bettaCurrent;

    //Файлы
    QFile *file;
    QFile *calibration;
    QTextStream *steam;

};


#endif // EXPERIMENT_H
