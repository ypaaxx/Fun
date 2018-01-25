#ifndef SENSOR_H
#define SENSOR_H

#include <QtCharts>
#include <QPolygonF>
#include <QTimer>
#include <QTime>

class Sensor : public QObject
{
    Q_OBJECT

    static QTimer *timer;
    static QTime *time;

private:
    const quint8 LENGHT_AVE = 10; //длина осреднения
    quint8 MAX_SIZE = 20;

    int numberValues = 0;// Cчётчик точек
    QList<quint16> *lastValues;
    QChart *chart;
    QChartView *view;
    QXYSeries *series;
    qreal mean = 0;

    //Таррировочные коэффициенты p(value) = A*value + B
    qreal coefficientA;
    qreal coefficientB;

public:
    Sensor();

    //Делает из сенсора не просто сенсор, а сенсор с графиками, мать его!
    void makeChart();

    // Сделает всё, дабы отображались углы как надобно
    void makeAngle();

    //Добавить новое значение. Время выставится само от статического таймера
    void addData(int value);

    //установка коэффициентов таррировки
    void setCalibrationCoefficients(qreal A, qreal B) { coefficientA = A; coefficientB = B; }

    //Давление в Па из значения по коэффициентам
    qreal pressure(int value) {return coefficientA*value + coefficientB; }

    // Кучка жалких get и set
    void setView(QChartView *view);
    QChartView *getView() const { return view; }
    QXYSeries *getSeries() const { return series; }
    qreal getMean() const { return mean; }
    QChart *getChart() {
        if(!chart) makeChart(); //Если нет графика
        return chart;
    }

signals:
    void newMean(QString);

public slots:
    void scroll_();

};


#endif // SENSOR_H
