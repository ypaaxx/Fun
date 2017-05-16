#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>

#include "QTime"
#include "QTimer"
#include "QtMath"
#include "QtCharts"

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
    void on_pushButton_clicked();
    void readData();
    void on_fiStep_editingFinished();

    void on_bettaStep_editingFinished();

    void on_betta_editingFinished();

    void on_fi_editingFinished();

private:
    Ui::MainWindow *ui;

    Arduino arduino;

    QTime startTime;
    QChart* plot_ = new QChart;

    QSplineSeries* curve_ = new QSplineSeries();
    QSplineSeries* curveMean_ = new QSplineSeries();
    QChartView *view = new QChartView(plot_);
    //QValueAxis *x_axis;
    double mean(QSplineSeries *series, int ms);
};

#endif // MAINWINDOW_H
