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
     * 0 - центральный
     * 1 - верхний
     * остальные против часовой */
    QVector <Sensor> rHight, rMiddle, rLow;

    Sensor temperature;

};

#endif // MAINWINDOW_H
