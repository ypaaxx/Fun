#ifndef SENSOR_H
#define SENSOR_H

#include <QtCharts>

class Sensor
{
    static quint8 count;
private:

    quint8 id;
    QChart *chart;
    QChartView *view;
    QLineSeries *lineSeries;

public:
    Sensor();

    QChartView *getView() const { return view; }
    QLineSeries *getSeries() const { return lineSeries; }
    QChart *getChart() const { return chart; }

    void setView(QChartView *view);
signals:

public slots:
};


#endif // SENSOR_H
