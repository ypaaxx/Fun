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
    void setFile(QFile experiment);

    qreal getFiCurrent() const;

    qreal getBettaCurrent() const;

    QVector<Sensor *> *getRHight() const;

    QVector<Sensor *> *getRMiddle() const;

    QVector<Sensor *> *getRLow() const;

    Sensor *getAngleHight() const;

    Sensor *getAngleMiddle() const;

    Sensor *getAngleLow() const;

private:
    const qreal stpBetta = 0.125;
    const qreal stpFi = 0.25;

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
    Sensor* angleHight;// Фиктивные датчики угла
    Sensor* angleMiddle;
    Sensor* angleLow;

    // Текущие значения Fi Betta
    qreal fiCurrent, bettaCurrent;

    //Таймер
    QTime *time;
    QTimer *timer;

    //Файлы логов
    QFile log;
    QTextStream logStream;
    QFile experiment;
    QTextStream experimentStream;

    quint8 crc8(QByteArray &array, quint8 len);
    void getData(QVector<Sensor *> *radius);

signals:

public slots:


};

#endif // EXPERIMENT_H
