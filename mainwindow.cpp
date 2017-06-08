#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    time.start();
    log.setFileName("time.txt");
    log.open(QIODevice::WriteOnly);
    logStream.setDevice(&log);
    logStream << "# time (ms); numer of sensor; value(Pa); message;" << endl;

    experiment.setFileName(QDate::currentDate().toString("experiments/yyyy.MM.dd_hh.mm.txt"));
    experiment.open(QIODevice::ReadOnly);
    experimentStream.setDevice(&experiment);

    /* По пять датчиков на каждом насадке */
    rHight = new QVector <Sensor> (5);
    rMiddle = new QVector <Sensor> (5);
    rLow = new QVector <Sensor> (5);

    /* Добавим в allSensors ссылки на сенсоры */
    allSensors << &temperature;
    for (Sensor &sensor: *rHight)
        allSensors << &sensor;
    for (Sensor &sensor: *rMiddle)
        allSensors << &sensor;
    for (Sensor &sensor: *rLow)
        allSensors << &sensor;

    //Псевдо датчики угла
    rHight->resize(6);
    rMiddle->resize(6);
    rLow->resize(6);

    angleHight = &rHight[5];
    angleMiddle = rMiddle[5];
    angleLow = rLow[5];

    /* Назначение графиков разности давлений */
    ui->deltaPHight->setChart(rHight->at(1).getChart());
    ui->deltaPMiddle->setChart(rMiddle->at(1).getChart());
    ui->deltaPLow->setChart(rLow->at(1).getChart());

    /* Назначение графиков полных давлений */
    ui->pHight->setChart(rHight->at(0).getChart());
    ui->pMiddle->setChart(rMiddle->at(0).getChart());
    ui->pLow->setChart(rLow->at(0).getChart());

    /* Назначение графиков углов */
    angleHight->getChart()->axisX()->setRange(-24, 24);
    angleHight->getChart()->axisY()->setRange(-180, 180);
    ui->fiHight->setChart(angleHight->getChart());
    ui->fiMiddle->setChart(angleMiddle->getChart());
    ui->fiLow->setChart(angleLow->getChart());


}

MainWindow::~MainWindow()
{
    arduino.close();
    log.close();
    experiment.close();

    delete ui;
}

/* При нажатии на поиск ардуинки вызывает функцию поиска findArduino
 * и при успехе конектит readyRead ардуины к слоту readData */
void MainWindow::on_findArduino_clicked()
{
    if (arduino.findArduino())
    {
        ui->findArduino->hide();
        ui->label->setText("Arduino on " + arduino.portName());
        connect(&arduino, &Arduino::readyRead, this, &readData);
    }
}

/* Вызывается при появлении данных от ардуинки, читает сообщение
 * проверяет, пишет куда надо */
void MainWindow::readData()
{
    if(arduino.isReadable()) {
        QByteArray message = arduino.readAll();
        quint8 numerSensor = message[0]; // Номер сенсора
        quint16 value = (message.at(1) << 8) + (quint8) message.at(2); // Значение от сенсора
        quint16 betta = (message.at(3) << 8) + (quint8) message.at(4); // Betta
        quint16 fi = (message.at(5) << 8) + (quint8) message.at(6); // Fi
        quint8 crc = message[7]; // Контрольная сумма
        if (crc == crc8(message, 7)){
            allSensors.at(numerSensor)->getSeries()->append(time.elapsed()/1000, value); //время в секундах

            logStream << time.elapsed() << "; ";
            logStream << numerSensor << "; ";
            logStream << value << "; ";
            logStream << fi << "; ";
            logStream << betta << "; ";
            for (auto byte: message)
                logStream << bin << byte;
            logStream << "; ";
            logStream << dec << endl;
        }
        //qDebug() << numerSensor << " " << value;
        //qDebug() << crc << " " << crc8(arr, 7);
    }
}

/* Выставление шага изменения угла и округление
 * введенного значения */
void MainWindow::on_fiStep_editingFinished()
{
    double step = ui->fiStep->value();
    int nStep = step/0.25;
    step = 0.25*nStep;
    ui->fiStep->setValue(step);
    ui->fi->setSingleStep(step);
}
void MainWindow::on_bettaStep_editingFinished()
{
    double step = ui->bettaStep->value();
    int nStep = step/0.125;
    step = 0.125*nStep;
    ui->bettaStep->setValue(step);
    ui->betta->setSingleStep(step);
}
void MainWindow::on_betta_editingFinished()
{
    double step = ui->betta->value();
    int nStep = step/0.125;
    step = 0.125*nStep;
    ui->betta->setValue(step);
}
void MainWindow::on_fi_editingFinished()
{
    double step = ui->fi->value();
    int nStep = step/0.25;
    step = 0.25*nStep;
    ui->fi->setValue(step);
}

/* Вычисление контрольной суммы */
quint8 MainWindow::crc8(QByteArray &array, quint8 len)
{
    //ОНо рабОТатЕт!!1111АДИН
    quint8 crc = 0x00;
    for( qint8 num = 0; num < len; ++num){
        crc ^= array[num];
        for (quint8 i = 0; i < 8; i++)
            crc = crc & 0x80 ? (crc << 1) ^ 0x07 : crc << 1;
    }

    return crc;
}

/* Запись данных по кнопке для распихивания по кнопкам */
void MainWindow::getData(QVector <Sensor> *radius){

    auto angle = radius->at(5);
    radius->at(5).getSeries()->append(bettaCurrent, fiCurrent);

    for (qint8 i = 0; i < 5; ++i)
            experimentStream << radius->at(i).getSeries()->points().back().y() << "; ";
    experimentStream << angle.getSeries()->points().back().x();
    experimentStream << angle.getSeries()->points().back().y();

}

/* Взятие данных с верхнего радиуса */
void MainWindow::on_pushHight_clicked()
{
    getData(rHight);
}

void MainWindow::on_pushMiddle_clicked()
{
    getData(rMiddle);
}

void MainWindow::on_pushLow_clicked()
{
    getData(rLow);
}
