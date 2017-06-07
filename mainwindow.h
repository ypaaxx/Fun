#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QVector>
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

private:
    Ui::MainWindow *ui;
    Arduino arduino;

    /* Нумерация датчиков:
     * 0 - центральный (полное)
     * 1 - разность левого и правого
     * 2 - значение левого и правого
     * 3 - верхний
     * 4 - нижний */
    QVector <Sensor> *rHight;
    QVector <Sensor> *rMiddle;
    QVector <Sensor> *rLow;
    Sensor temperature; //Температурный датчик в корпусе
    QVector <Sensor*> allSensors; //Список всех сенсоров

    //Таймер
    QTime time;

    //Файлы логов
    QFile log;
    QTextStream logStream;

    quint8 crc8(QByteArray &array, quint8 len);
};

#endif // MAINWINDOW_H
