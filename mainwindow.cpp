#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /* Назначение графиков разности давлений */
    ui->deltaPHight->setChart(experiment.getRHight()->at(1)->getChart());
    ui->deltaPMiddle->setChart(experiment.getRMiddle()->at(1)->getChart());
    ui->deltaPLow->setChart(experiment.getRLow()->at(1)->getChart());

    /* Назначение графиков полных давлений */
    ui->pHight->setChart(experiment.getRHight()->at(0)->getChart());
    ui->pMiddle->setChart(experiment.getRMiddle()->at(0)->getChart());
    ui->pLow->setChart(experiment.getRLow()->at(0)->getChart());

    /* Назначение графиков углов */
    experiment.getAngleHight()->getChart()->axisX()->setRange(-24, 24);
    experiment.getAngleHight()->getChart()->axisY()->setRange(-180, 180);
    experiment.getAngleHight()->noScroll(); //Не прокручивать графики
}

MainWindow::~MainWindow()
{
    if (arduino.isOpen()) arduino.close();
    delete ui;
}

/* При нажатии на поиск ардуинки вызывает функцию поиска findArduino
 * и при успехе конектит readyRead ардуины к слоту readData
 * меняет назначение кнопки на отключение от ардуины */
void MainWindow::on_findArduino_clicked()
{
    if(arduino.isOpen()) {
        disconnect(&arduino, &Arduino::readyRead, this, &readData);
        arduino.close();
        ui->label->setText("Найти ардуину");
        return;
    }
    if (!arduino.findArduino()) return;
    ui->label->setText("Arduino on " + arduino.portName());
    ui->findArduino->setText("Close arduino");
    connect(&arduino, &Arduino::readyRead, this, &readData);
}

/* Вызывается при появлении данных от ардуинки, читает сообщение
 * проверяет, пишет куда надо */
void MainWindow::readData()
{
        quint8 numerSensor;
        quint16 value;
        quint16 fi;
        quint16 betta;
        /* При совпадении crc запись в лог и серии датчика */
        if(arduino.read(numerSensor, value, fi, betta))
        {
            experiment.addData(numerSensor, value);
            //fiCurrent = fi*stpFi - 180;
            //bettaCurrent = betta*stpBetta - 24;
        }

        //ui->valueFi->setText(QString::number(fiCurrent) + "°");
        //ui->valueBetta->setText(QString::number(bettaCurrent) + "°");
}

/* Выставление шага изменения угла и округление
 * введенного значения */
void MainWindow::on_fiStep_editingFinished()
{
    double step = ui->fiStep->value();
    int nStep = step/Arduino::stpFi;
    step = Arduino::stpFi*nStep;
    ui->fiStep->setValue(step);
    ui->fi->setSingleStep(step);
}
void MainWindow::on_bettaStep_editingFinished()
{
    double step = ui->bettaStep->value();
    int nStep = step/Arduino::stpBetta;
    step = Arduino::stpBetta*nStep;
    ui->bettaStep->setValue(step);
    ui->betta->setSingleStep(step);
}
void MainWindow::on_betta_editingFinished()
{
    double step = ui->betta->value();
    int nStep = step/Arduino::stpBetta;
    step = Arduino::stpBetta*nStep;
    ui->betta->setValue(step);
}
void MainWindow::on_fi_editingFinished()
{
    double step = ui->fi->value();
    int nStep = step/Arduino::stpFi;
    step = Arduino::stpFi*nStep;
    ui->fi->setValue(step);
}

/* Запись данных по кнопке для распихивания по кнопкам */
void MainWindow::getData(QVector <Sensor*> *radius){
/*
    if (!arduino.isOpen()) return;

    auto angle = radius->at(5);
    radius->at(5)->getSeries()->append(bettaCurrent, fiCurrent);

    for (qint8 i = 0; i < 5; ++i)
        experimentStream << radius->at(i)->getSeries()->points().back().y() << "; ";
    experimentStream << angle->getSeries()->points().back().x(); //betta
    experimentStream << angle->getSeries()->points().back().y(); //fi

    experimentStream << endl;
    experiment.flush();
*/
}

/* Взятие данных с верхнего радиуса */
void MainWindow::on_pushHight_clicked()
{
//    experimentStream << "Hight; ";
//    getData(rHight);
}

void MainWindow::on_pushMiddle_clicked()
{
//    experimentStream << "Middle; ";
//    getData(rMiddle);
}

void MainWindow::on_pushLow_clicked()
{
//    experimentStream << "Low; ";
//    getData(rLow);
}

void MainWindow::on_pushButton_4_clicked()
{
    quint16 f = (ui->fi->value()+180)/Arduino::stpFi;
    quint16 b = (ui->betta->value()+24)/Arduino::stpBetta;

    arduino.revolution(f, b);
}

void MainWindow::on_newExperiment_clicked()
{/*

    QString nameExperiment = QFileDialog::getSaveFileName(this,
                                                          tr("New experiment file"),
                                                          QDate::currentDate().toString("yyyy.MM.dd") + QTime::currentTime().toString("_hh.mm"),
                                                          tr("Text (*.txt)"));
    experiment.setFileName(nameExperiment);
    if (nameExperiment.isEmpty()) return;
    if (!experiment.open(QIODevice::WriteOnly)){
        QMessageBox::critical(this, "Holly shit!", "This file doesn't want be open");
        return;
    }
    experimentStream.setDevice(&experiment);
    experimentStream << "#" << QDate::currentDate().toString("dd.MM.yyyy")
                     << QTime::currentTime().toString(" hh:mm") << endl;
    experimentStream << "#radius; central; difference between left and right; value right and left;"
                        " up; down; betta; fi;" << endl;

    ui->newExperiment->setText(experiment.fileName());
*/}

void MainWindow::on_oldExperiment_clicked()
{/*
    if(experiment.isOpen()) experiment.close();
    QString nameExperiment = QFileDialog::getOpenFileName(this, tr("Open experiment file"), "", tr("Text file (*.txt)"));
    if (nameExperiment.isEmpty()) return;

    experiment.setFileName(nameExperiment);

    if (!experiment.open(QIODevice::Append)){
        QMessageBox::critical(this, "Holly shit!", "This file doesn't want be open");
        return;
    }
    experimentStream.setDevice(&experiment);
    experimentStream << "#" << QDate::currentDate().toString("dd.MM.yyyy")
                     << QTime::currentTime().toString(" hh:mm") << endl;

*/
}
