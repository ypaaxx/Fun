#ifndef SENSOR_H
#define SENSOR_H

#include <QObject>
#include <QtCharts>

class Sensor : public QObject
{
    Q_OBJECT

private:
    quint8 id_;
    QString name = "Jopa";
    QChart *chart;
    QChartView *view;
    QLineSeries *p_t;

public:
    explicit Sensor(QObject *parent = 0);
    QChartView *getView(){ return view; }
    QLineSeries *getSeries() { return p_t; }

signals:

public slots:
};

#endif // SENSOR_H
