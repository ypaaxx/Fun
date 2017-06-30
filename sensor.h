#ifndef SENSOR_H
#define SENSOR_H

#include <QtCharts>
#include <QTimer>

class Sensor : public QObject
{
    Q_OBJECT

    static quint8 count;
private:

    quint8 id;
    QChart *chart;
    QChartView *view;
    QLineSeries *lineSeries;
    QTimer *timer;

public:
    Sensor();

    QChartView *getView() const { return view; }
    QLineSeries *getSeries() const { return lineSeries; }
    QChart *getChart() const { return chart; }

    void setView(QChartView *view);
    void noScroll();
    QTimer *getTimer() const;

    void setTimer(QTimer *value) { timer = value; }

signals:

public slots:
    void scroll_();
};


#endif // SENSOR_H
