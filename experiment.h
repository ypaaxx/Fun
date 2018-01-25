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

    //Установка списка датчиков извне
    void setAllSensors(QVector <Sensor*> *_allSensors){
        allSensors = _allSensors;
    }

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
    Radius(){
        //Инициализация псевдодатчика положения
        angle = new Sensor;
        angle->makeAngle();

        //Инициализация и заполнение списка сенсоров и заодно иникаторов их средних значений
        sensors = new QVector <Sensor *> (5);
        auto linesMeanSeries = new QVector <QLineEdit*> (5);
        for(int i=0; i<5; i++){
            sensors->replace(i, new Sensor);
            linesMeanSeries->replace(i, new QLineEdit);
            connect(sensors->at(i), SIGNAL(newMean(QString)),
                    linesMeanSeries->at(i), SLOT(setText(QString)));
        }

        //Алиасы индикаторов
        totalP = linesMeanSeries->at(0);
        dPH = linesMeanSeries->at(1);
        pH = linesMeanSeries->at(2);
        pUp = linesMeanSeries->at(3);
        pDown = linesMeanSeries->at(4);

        //Датчики с полным и статическим давлением с графиками
        sensors->at(0)->makeChart();
        sensors->at(1)->makeChart();

        // Указатели на необходимые графики
        totalPressure = new QChartView(sensors->at(0)->getChart());
        staticPressure = new QChartView(sensors->at(1)->getChart());
        angleChart = new QChartView(angle->getChart());

        /* Графическое содержание */
        mainLayout = new QHBoxLayout;
        actualData = new QVBoxLayout;

        dataForm = new QFormLayout;
        button = new QPushButton("Взять");
        connect(button, SIGNAL(clicked(bool)), this, SLOT(getPoint()));

        // Формирование главного виджета радиуса
        mainLayout->addLayout(actualData);
        mainLayout->addWidget(totalPressure);
        mainLayout->addWidget(staticPressure);
        mainLayout->addWidget(angleChart);
        // Добавить кнопку записи и форму значений
        actualData->addWidget(button);
        actualData->addLayout(dataForm);

        // Наполнение формы
        dataForm->addRow("p*", totalP);
        dataForm->addRow("dp", dPH);
        dataForm->addRow("p", pH);
        dataForm->addRow("p up", pUp);
        dataForm->addRow("p dn", pDown);
    }


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

    void getPoint(){
        angle->getSeries()->append(*betta, *fi);
        qDebug() << *betta << *fi;

        //Убрать позже
        chartOnNewWindow(3);
    }

    void chartOnNewWindow(int num){
        QWidget *window = new QWidget();
             window->resize(320, 240);
             window->show();
        sensors->at(3)->makeChart();
        sensors->at(3)->setParent(0);
        sensors->at(3)->getView()->show();
    }
};

#endif // EXPERIMENT_H
