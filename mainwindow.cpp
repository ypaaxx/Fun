#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    rHight.resize(5);
    for(auto sensor: rHight)
        rHight << new Sensor();


}

MainWindow::~MainWindow()
{
    arduino.close();
    delete ui;
}

void MainWindow::on_findArduino_clicked()
{
    if (arduino.findArduino())
    {
        ui->findArduino->hide();
        ui->label->setText("Arduino on " + arduino.portName());
        connect(&arduino, &Arduino::readyRead, this, &readData);
    }
}

void MainWindow::readData()
{
    if(arduino.isReadable()) {
        QByteArray arr = arduino.readAll();
        quint8 i = arr[0];
        quint16 value = (arr.at(1) << 8) + (quint8) arr.at(2);
        //sensors.at(i)->getSeries()->append(i, value);
        quint8 crc8 = arr[7];
        qDebug() << i << " " << value << endl;
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
