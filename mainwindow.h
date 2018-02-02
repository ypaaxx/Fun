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
#include "radius.h"

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
    void move();
    void on_actionFind_triggered();

    void calibration(){
        QString fileName = QFileDialog::getOpenFileName(this,
                                                        tr("New experiment file"),
                                                        QDate::currentDate().toString("yyyy.MM.dd") + QTime::currentTime().toString("_hh.mm"),
                                                        tr("Text (*.txt)"));
        if (fileName.isEmpty()) return;

        if ( !experiment.setCalibrationFile(new QFile(fileName))) {
            QMessageBox::critical(this, "Holly shit!", "This file doesn't want be open");
            return;
        }

        this->setWindowTitle("Fun base - " + fileName);
    }

private:
    QVector <QLabel *> *labelVector;

    Ui::MainWindow *ui;
    Arduino arduino;
    Experiment experiment;

    Radius *upper;
    Radius *middle;
    Radius *lower;

    qreal fi;//Переменные положения
    qreal betta;
    QLabel *currentFi; //Полученные от ардуины положения
    QLabel *currentBetta;
    QDoubleSpinBox *nextFi; //Следующие положения
    QDoubleSpinBox *nextBetta;


    QLabel* isArduinoHave;
    QLabel* lastMessage;


};

#endif // MAINWINDOW_H
