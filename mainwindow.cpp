#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Fun base");

    isArduinoHave = new QLabel(this);
    statusBar()->addWidget(isArduinoHave);
    
    lastMessage = new QLabel(this);
    statusBar()->addWidget(lastMessage);

    labelVector = new QVector <QLabel*>;
    *labelVector << ui->sensor_0;
    *labelVector << ui->sensor_1;
    *labelVector << ui->sensor_2;
    *labelVector << ui->sensor_3;
    *labelVector << ui->sensor_4;
    *labelVector << ui->sensor_5;
    *labelVector << ui->sensor_6;
    *labelVector << ui->sensor_7;
    *labelVector << ui->sensor_8;
    *labelVector << ui->sensor_9;
    *labelVector << ui->sensor_10;
    *labelVector << ui->sensor_11;
    *labelVector << ui->sensor_12;
    *labelVector << ui->sensor_13;
    *labelVector << ui->sensor_14;
    *labelVector << ui->sensor_15;

    /* Назначение графиков разности давлений */
    ui->deltaPHight->setChart(experiment.getRHight()->at(1)->getChart());
    ui->deltaPMiddle->setChart(experiment.getRMiddle()->at(1)->getChart());
    ui->deltaPLow->setChart(experiment.getRLow()->at(1)->getChart());

    /* Назначение графиков полных давлений */
    ui->pHight->setChart(experiment.getRHight()->at(0)->getChart());
    ui->pMiddle->setChart(experiment.getRMiddle()->at(0)->getChart());
    ui->pLow->setChart(experiment.getRLow()->at(0)->getChart());

    /* Назначение графиков углов */
    ui->fiHight->setChart(experiment.getAngleHight()->getChart());
    ui->fiMiddle->setChart(experiment.getAngleMiddle()->getChart());
    ui->fiLow->setChart(experiment.getAngleLow()->getChart());

    //А вдруг она уже подключена?
    on_actionFind_triggered();
}

MainWindow::~MainWindow()
{
    if (arduino.isOpen()) arduino.close();
    delete ui;
}


/* Вызывается при появлении данных от ардуинки, читает сообщение
 * проверяет, пишет куда надо */
void MainWindow::readData()
{
    quint8 numerSensor;
    quint16 value;

    /* При совпадении crc запись в лог и серии датчика */
    if(arduino.getMessage(numerSensor, value, fi, betta))
        experiment.addData(numerSensor, value);
    else
        return;

    qreal mean = experiment.getMean(numerSensor);

    labelVector->at(numerSensor)->setText(QString::number(mean));
    //if (qFabs(mean-value)/mean > 0.05)  labelVector->at(numerSensor)->setFont(QFont::Bold);
    lastMessage->setText(QString::number(numerSensor) + " " + QString::number(value));

    ui->currentFi->setText(QString::number(fi) + "°");
    ui->currentBetta->setText(QString::number(betta) + "°");


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

/* Взятие данных с верхнего радиуса */
void MainWindow::on_pushHight_clicked()
{
    if (!experiment.isFileSet()) {
        QMessageBox::critical(this, "Holly shit!", "File doesn't open");
        return;
    }
    experiment.getPoint(2, fi, betta);
}

void MainWindow::on_pushMiddle_clicked()
{
    if (!experiment.isFileSet()) {
        QMessageBox::critical(this, "Holly shit!", "File doesn't open");
        return;
    }
    experiment.getPoint(1, fi, betta);
}

void MainWindow::on_pushLow_clicked()
{
    if (!experiment.isFileSet()) {
        QMessageBox::critical(this, "Holly shit!", "File doesn't open");
        return;
    }
    experiment.getPoint(0, fi, betta);
}

/* Поворот шагового двигателя */
void MainWindow::on_pushButton_4_clicked()
{
    arduino.revolution(ui->fi->value(), ui->betta->value());
}


/* Открытие нового файла эксперемента */
void MainWindow::on_actionNew_triggered()
{
    QString nameExperiment = QFileDialog::getSaveFileName(this,
                                                          tr("New experiment file"),
                                                          QDate::currentDate().toString("yyyy.MM.dd") + QTime::currentTime().toString("_hh.mm"),
                                                          tr("Text (*.txt)"));
    if (nameExperiment.isEmpty()) return;

    if ( !experiment.setFile(new QFile(nameExperiment))) {
        QMessageBox::critical(this, "Holly shit!", "This file doesn't want be open");
        return;
    }

    this->setWindowTitle("Fun base - " + nameExperiment);
}

/* При нажатии на поиск ардуинки вызывает функцию поиска findArduino
 * и при успехе конектит readyRead ардуины к слоту readData
 * меняет назначение кнопки на отключение от ардуины */
void MainWindow::on_actionFind_triggered()
{
    if(arduino.isOpen()) {
        disconnect(&arduino, &Arduino::readyRead, this, &MainWindow::readData);
        arduino.close();
        isArduinoHave->setText("no base");

        return;
    }
    if (!arduino.findArduino()) {
        isArduinoHave->setText("no base");

        return;
    }
    isArduinoHave->setText("Base on " + arduino.portName());
    connect(&arduino, &Arduino::readyRead, this, &MainWindow::readData);

    if(arduino.bytesAvailable()) readData();
}

void MainWindow::on_pushButton_clicked()
{
    experiment.setNull();
}
