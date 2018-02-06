#include "radius.h"

Radius::Radius(){

    // Настройка графика углов
    angleSeries = new QScatterSeries;
    angleSeries->setMarkerSize(5);
    angleSeries->setColor(Qt::blue);
    angleSeries->setBorderColor(Qt::blue);
    auto angleChart = new QChart;

    //Настройка актуальной точки

    actualPoint = new QScatterSeries;
    actualPoint->append(0, 0);
    actualPoint->setMarkerSize(5);
    actualPoint->setColor(Qt::red);
    actualPoint->setBorderColor(Qt::red);

    angleChart->addSeries(angleSeries);
    angleChart->addSeries(actualPoint);

    angleChart->legend()->hide();
    angleChart->createDefaultAxes();
    angleChart->axisX()->setRange(-24, 24);
    angleChart->axisY()->setRange(-180, 180);
    angleChart->setMargins(QMargins(0,0,0,0));


    //Инициализация и заполнение списка сенсоров и заодно иникаторов их средних значений
    sensors = new QVector <Sensor *> (5);
    auto labelMeanSeries = new QVector <QLabel*> (5);
    for(int i=0; i<5; i++){
        sensors->replace(i, new Sensor);
        labelMeanSeries->replace(i, new QLabel);
        connect(sensors->at(i), SIGNAL(newMean(QString)),
                labelMeanSeries->at(i), SLOT(setText(QString)));
        //        connect(linesMeanSeries->at(i), SIGNAL(returnPressed()),
        //                sensors->at(i), SLOT(viewOnWindow()));
    }

    //Алиасы индикаторов
    totalP = labelMeanSeries->at(0);
    dPH = labelMeanSeries->at(1);
    pH = labelMeanSeries->at(2);
    pUp = labelMeanSeries->at(3);
    pDown = labelMeanSeries->at(4);

    //Датчики с полным и статическим давлением с графиками
    sensors->at(0)->makeChart();
    sensors->at(1)->makeChart();

    // Указатели на необходимые графики
    totalPressure = new QChartView(sensors->at(0)->getChart());
    totalPressure->setRubberBand(QChartView::VerticalRubberBand);
    staticPressure = new QChartView(sensors->at(1)->getChart());
    staticPressure->setRubberBand(QChartView::VerticalRubberBand);

    auto angleChartView = new QChartView(angleChart);
    angleChartView->setRubberBand(QChartView::RectangleRubberBand);

    /* Графическое содержание */
    mainLayout = new QHBoxLayout;
    actualData = new QVBoxLayout;

    dataForm = new QFormLayout;
    button = new QPushButton("Взять");
    connect(button, SIGNAL(clicked(bool)), this, SLOT(getPoint()));

    // Формирование главного виджета радиуса
    mainLayout->addLayout(actualData);
    mainLayout->addWidget(totalPressure);
    mainLayout->addWidget(staticPressure);
    mainLayout->addWidget(angleChartView);

    // Добавить кнопку записи и форму значений
    actualData->addWidget(button);
    actualData->addLayout(dataForm);

    // Наполнение формы
    dataForm->addRow("p*", totalP);
    dataForm->addRow("dp", dPH);
    dataForm->addRow("p", pH);
    dataForm->addRow("p up", pUp);
    dataForm->addRow("p dn", pDown);
}

void Radius::getPoint(){
    angleSeries->append(*betta, *fi);
    *output << objectName() << " ";
    *output << radius;
    for(Sensor *sensor: *sensors)
        *output << sensor->getMean() << "; ";
    *output << endl;
    output->flush();
}
