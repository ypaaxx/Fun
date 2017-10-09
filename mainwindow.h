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
#include "experiment.h"

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
    void readData();

    void on_betta_editingFinished();
    void on_fi_editingFinished();

    void on_pushHight_clicked();

    void on_pushMiddle_clicked();

    void on_pushLow_clicked();

    void on_pushButton_4_clicked();

    void on_actionNew_triggered();

    void on_actionFind_triggered();

    void on_pushButton_clicked();

private:
    QVector <QLabel *> *labelVector;

    Ui::MainWindow *ui;
    Arduino arduino;
    Experiment experiment;
    qreal fi;
    qreal betta;

    QLabel* isArduinoHave;
    QLabel* lastMessage;

};

#endif // MAINWINDOW_H
