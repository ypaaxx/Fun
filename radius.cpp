#include "radius.h"

Radius::Radius(){
    //Инициализация псевдодатчика положения
    angle = new Sensor;
    angle->makeAngle();

    //Инициализация и заполнение списка сенсоров и заодно иникаторов их средних значений
    sensors = new QVector <Sensor *> (5);
    auto linesMeanSeries = new QVector <QLineEdit*> (5);
    for(int i=0; i<5; i++){
        sensors->replace(i, new Sensor);
        linesMeanSeries->replace(i, new QLineEdit);
        connect(sensors->at(i), SIGNAL(newMean(QString)),
                linesMeanSeries->at(i), SLOT(setText(QString)));
//        connect(linesMeanSeries->at(i), SIGNAL(returnPressed()),
//                sensors->at(i), SLOT(viewOnWindow()));
    }

    //Алиасы индикаторов
    totalP = linesMeanSeries->at(0);
    dPH = linesMeanSeries->at(1);
    pH = linesMeanSeries->at(2);
    pUp = linesMeanSeries->at(3);
    pDown = linesMeanSeries->at(4);

    //Датчики с полным и статическим давлением с графиками
    sensors->at(0)->makeChart();
    sensors->at(1)->makeChart();

    // Указатели на необходимые графики
    totalPressure = new QChartView(sensors->at(0)->getChart());
    staticPressure = new QChartView(sensors->at(1)->getChart());
    angleChart = new QChartView(angle->getChart());

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
    mainLayout->addWidget(angleChart);
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
