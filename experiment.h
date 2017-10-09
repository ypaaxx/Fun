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
    void getPoint(int radius, qreal fi, qreal betta);

    qreal getFiCurrent() const { return fiCurrent;}
    qreal getBettaCurrent() const {return bettaCurrent;}
    QVector<Sensor *> *getRHight() const {return rHight;}
    QVector<Sensor *> *getRMiddle() const {return rMiddle;}
    QVector<Sensor *> *getRLow() const {return rLow;}
    Sensor *getAngleHight() const {return angleHight;}
    Sensor *getAngleMiddle() const {return angleMiddle;}
    Sensor *getAngleLow() const {return angleLow;}

    void setSteam(const QTextStream &value);
    QTextStream getSteam() const;

    QFile getFile() const;
    bool setFile(QFile *value);

    qreal getMean(quint8 numer);
    bool isFileSet(){ return file->isOpen(); }
    void setNull(){
        for (Sensor* &sensor: *allSensors){
            sensor->setCalibrationCoefficients(sensor->getMean(), 1245.41);
        }
    }
    void setCallibrationCoefficient( quint8 numerSensor, qreal A, qreal B);

private:

    /* Нумерация датчиков:
     * 0 - центральный (полное)
     * 1 - разность левого и правого
     * 2 - значение левого и правого
     * 3 - верхний
     * 4 - нижний */

    QVector <Sensor*> *rHight;
    QVector <Sensor*> *rMiddle;
    QVector <Sensor*> *rLow;
    Sensor *temperature; //Температурный датчик в корпусе
    QVector <Sensor*> *allSensors; //Список всех реальных сенсоров
    QVector <QVector <Sensor*> *> *radiuses;
    Sensor* angleHight;// Фиктивные датчики угла
    Sensor* angleMiddle;
    Sensor* angleLow;

    // Текущие значения Fi Betta
    qreal fiCurrent, bettaCurrent;

    //Таймер
    QTime *time;
    QTimer *timer;

    //Файлы
    QFile *file;
    QFile *calibration;
    QTextStream *steam;

signals:

public slots:


};

#endif // EXPERIMENT_H
