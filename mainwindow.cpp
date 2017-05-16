#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QTime midnight(0,0,0);
    qsrand(midnight.secsTo(QTime::currentTime()));
    startTime = QTime::currentTime();

    ui->setupUi(this);

    QTimer *timer = new QTimer;
    timer->setInterval(1000);
    connect(timer, &QTimer::timeout, this, &readData);
    timer->start();

    //plot_->setAnimationOptions(QChart::AllAnimations);
    view->setRenderHint(QPainter::Antialiasing);
    plot_->legend()->hide();
    plot_->addSeries(curve_);
    plot_->addSeries(curveMean_);
    plot_->createDefaultAxes();
    plot_->axisY()->setRange(-1,1);
    //plot_->margins().setRight(0);
    plot_->setTitle("P 1-3");


    ui->P13_1->setViewport(view);
}

MainWindow::~MainWindow()
{
    arduino.close();
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    if (arduino.findArduino())
    {
        ui->pushButton->hide();
        ui->label->setText("Arduino on " + arduino.portName());
        connect(&arduino, &Arduino::readyRead, this, &readData);

    }
}

void MainWindow::readData()
{
//    if(arduino.isReadable()) QByteArray arr = arduino.readAll();
//    int i = arr[0];
//    qDebug() << arr << " " << i;
//    ui->textBrowser->append(arr);
    int ms = startTime.secsTo(QTime::currentTime());
    *curve_ << QPointF(ms, qSin(ms)/ms);
    if (ms >= 15) *curveMean_ << QPointF(ms, mean(curve_, 15));
    plot_->axisX()->setRange(0, ms);
}

double MainWindow::mean(QSplineSeries *series, int ms)
{
    double summ=0;
    for (int i = ms; i < series->count(); ++i)
    {
        summ += series->at(i).y();
    }
    //ui->label_2->setText("Среднее интегральное: " + QString::number(summ/(series->count()-ms)));
    return summ/((series->count()-ms));
}

/* Выставление шага изменения угла и округляет
 * введенное значение */
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
