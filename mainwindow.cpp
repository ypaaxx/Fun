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

    ui->deltaPHight->setChart(rHight->at(0).getChart());


}

MainWindow::~MainWindow()
{
    arduino.close();
    log.close();
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
        QByteArray arr = arduino.readAll();
        quint8 numerSensor = arr[0]; // Номер сенсора
        quint16 value = (arr.at(1) << 8) + (quint8) arr.at(2); // Значение от сенсора
        quint8 crc = arr[7]; // Контрольная сумма
        if (crc == crc8(arr, 7)){
            allSensors.at(numerSensor)->getSeries()->append(time.elapsed()/1000, value);
            logStream << time.elapsed() << "; ";
            logStream << numerSensor << "; ";
            logStream << value << "; ";
            for (auto byte: arr)
                logStream << hex << byte;
            logStream << "; ";
            logStream << dec << endl;
        }
        qDebug() << numerSensor << " " << value;
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


