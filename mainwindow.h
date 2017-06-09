#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QVector>
#include <QtCharts>
#include "QTime"
#include "QTimer"
#include "QtMath"

#include "sensor.h"
#include "arduino.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_findArduino_clicked();
    void readData();

    void on_fiStep_editingFinished();
    void on_bettaStep_editingFinished();
    void on_betta_editingFinished();
    void on_fi_editingFinished();

    void on_pushHight_clicked();

    void on_pushMiddle_clicked();

    void on_pushLow_clicked();

private:
    Ui::MainWindow *ui;
    Arduino arduino;

    /* Нумерация датчиков:
     * 0 - центральный (полное)
     * 1 - разность левого и правого
     * 2 - значение левого и правого
     * 3 - верхний
     * 4 - нижний */
    QVector <Sensor*> *rHight;
    QVector <Sensor*> *rMiddle;
    QVector <Sensor*> *rLow;
    Sensor temperature; //Температурный датчик в корпусе
    QVector <Sensor*> allSensors; //Список всех реальных сенсоров
    Sensor* angleHight;// Фиктивные датчики угла
    Sensor* angleMiddle;
    Sensor* angleLow;

    // Текущие значения Fi Betta
    qreal fiCurrent, bettaCurrent;

    //Таймер
    QTime time;

    //Файлы логов
    QFile log;
    QTextStream logStream;
    QFile experiment;
    QTextStream experimentStream;

    quint8 crc8(QByteArray &array, quint8 len);
    void getData(QVector<Sensor *> *radius);
};

#endif // MAINWINDOW_H
