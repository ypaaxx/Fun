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

private:
    QVector <Sensor *> *sensors;    //Список сенсоров насадка
    Sensor* angle;                  //Фиктивный сенсор угла
    qreal radius;                   //Радиус закрепления (мм)

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
    QLineEdit *totalP;
    QLineEdit *dPH;
    QLineEdit *pH;
    QLineEdit *pUp;
    QLineEdit *pDown;

private slots:

    /* При нажатии кнопки Взять */
    void getPoint(){
        angle->getSeries()->append(*betta, *fi);
    }

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
