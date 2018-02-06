#ifndef RADIUS_H
#define RADIUS_H

#include <QObject>
#include <QDebug>
#include <QVector>
#include <QtCharts>
#include "QTime"
#include "QTimer"
#include "QtMath"

#include "sensor.h"

/* Включает:
 *      пять датчиков давления
 *      значение радиуса */
/* Нумерация датчиков:
 * 0 - центральный (полное)
 * 1 - разность левого и правого
 * 2 - значение левого и правого
 * 3 - верхний
 * 4 - нижний */


class Radius : public QObject
{
    Q_OBJECT

    QScatterSeries *actualPoint;
public:
    Radius();

    //Заполнение списка сенсоров из донора начиная с позиции first
    void setSensors(int first, QVector <Sensor*> *donor){
        for (int i = 0; i < 5; i++)
            sensors->replace(i, donor->at(first + i));
        // Подключаем графики к 0 и 1 датчикам
        sensors->at(0)->makeChart();
        sensors->at(1)->makeChart();
    }
    void setOutput(QTextStream *_output){
        output = _output;
    }
    //Выдаёт указатель на список ссылок на датчики
    QVector <Sensor *> *getListSensors(){
        return sensors;
    }
    //Установка значений радиуса закрепления (мм)
    void setRadius(qreal _radiusMm){
        radius = _radiusMm;
    }
    qreal getRadius(){
        return radius;
    }
    //Указание положения
    void setFiBetta(qreal* _fi, qreal* _betta){
        fi = _fi;
        betta = _betta;
    }
    //Выдача главного виджета
    QHBoxLayout *getWidget(){
        return mainLayout;
    }

public slots:
    void moveBettaActual(double betta){
        qreal fi = actualPoint->at(0).y();
        actualPoint->replace(0, betta, fi);
    }
    void moveFiActual(double fi){
        qreal betta = actualPoint->at(0).x();
        actualPoint->replace(0, betta, fi);
    }

private:
    QVector <Sensor *> *sensors;    //Список сенсоров насадка
    Sensor* angle;                  //Фиктивный сенсор угла
    qreal radius;                   //Радиус закрепления (мм)

    QScatterSeries *angleSeries;
    QScatterSeries *actualPosition;
    QTextStream *output;

    //Указатели на текущее положение
    qreal* fi;
    qreal *betta;

    //Графики
    QChartView *totalPressure;
    QChartView *staticPressure;
    QChartView *angleChart;

    //Набор для графического представления
    QHBoxLayout *mainLayout;
    QVBoxLayout *actualData;
    QFormLayout *dataForm;
    QPushButton *button;

    //Средние значения в датчиках
    QLabel *totalP;
    QLabel *dPH;
    QLabel *pH;
    QLabel *pUp;
    QLabel *pDown;

    void actualPointSetting();
private slots:

    /* При нажатии кнопки Взять */
    void getPoint();

    /* Открытие дополнительного окна с единственным графиком */
    void chartOnNewWindow(int num){
        auto view = new QChartView;
        Sensor *sensor = sensors->at(num);
        sensor->makeChart();
        view->setChart(sensor->getChart());
        view->show();
    }
};

#endif // RADIUS_H
