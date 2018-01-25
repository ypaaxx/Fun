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

    /* Начало исправления всего */
    //Три насадка
    upper = new Radius;
    middle = new Radius;
    lower = new Radius;

    upper->setFiBetta(&fi, &betta);
    middle->setFiBetta(&fi, &betta);
    lower->setFiBetta(&fi, &betta);

    // Наполнение списка датчиков
    auto allSensors = new QVector <Sensor*>;
    *allSensors << new Sensor; // температурный датчик
    allSensors->append( *(upper->getListSensors()) );
    allSensors->append( *(middle->getListSensors()) );
    allSensors->append( *(lower->getListSensors()) );
    experiment.setAllSensors(allSensors);

    /* Формирование окна */
    auto maineLayout = new QVBoxLayout; //Главный вертикальный лайоут
    auto positionLayout = new QHBoxLayout; //Горизонтальный в котором содержится всё относительно позиций
    positionLayout->addStretch(1);  //Позиции слева
    maineLayout->addLayout(positionLayout); //Сверху позиции
    maineLayout->addLayout(upper->getWidget()); //потом верхний насадок
    maineLayout->addLayout(middle->getWidget());
    maineLayout->addLayout(lower->getWidget());
    currentFi = new QLabel; //Полученные от ардуины положения
    currentBetta = new QLabel;
    nextFi = new QDoubleSpinBox; //Следующие положения
    nextFi->setSingleStep(Arduino::stpFi);
    nextBetta = new QDoubleSpinBox;
    nextFi->setSingleStep(Arduino::stpBetta);
    auto goToNext = new QPushButton("Переместить"); // Кнопка отправки на ардуино
    connect(goToNext, SIGNAL(clicked(bool)), this, SLOT(move()));
    auto currentForm = new QFormLayout;
    currentForm->addRow("fi", currentFi);
    currentForm->addRow("betta", currentBetta);
    auto nextPosition = new QVBoxLayout;
    nextPosition->addWidget(nextFi);
    nextPosition->addWidget(nextBetta);
    //Корректеровка вписанных значений.
    connect(nextFi, SIGNAL(editingFinished()), this, SLOT(on_fi_editingFinished()));
    connect(nextBetta, SIGNAL(editingFinished()), this, SLOT(on_betta_editingFinished()));
    positionLayout->addLayout(currentForm); // Наполнение блока позиций
    positionLayout->addLayout(nextPosition);
    positionLayout->addWidget(goToNext);

    //Установка главного лайоута главным
    QWidget *window = new QWidget(this);
    window->setLayout(maineLayout);
    setCentralWidget(window);

    //А вдруг база уже подключена?
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
    if(arduino.getMessage(numerSensor, value, fi, betta)){
        experiment.addData(numerSensor, value);
        currentBetta->setText(QString::number(betta));
        currentFi->setText(QString::number(fi));
    }
    else
        return;

    lastMessage->setText(QString::number(numerSensor) + " " + QString::number(value));
}

/* Выставляют кратный шагам угол при редактировании */
void MainWindow::on_betta_editingFinished()
{
    double step = nextBetta->value();
    int nStep = step/Arduino::stpBetta;
    step = Arduino::stpBetta*nStep;
    nextBetta->setValue(step);
}
void MainWindow::on_fi_editingFinished()
{
    double step = nextFi->value();
    int nStep = step/Arduino::stpFi;
    step = Arduino::stpFi*nStep;
    nextFi->setValue(step);
}

///* Взятие данных с верхнего радиуса */
//void MainWindow::on_pushHight_clicked()
//{
//    if (!experiment.isFileSet()) {
//        QMessageBox::critical(this, "Holly shit!", "File doesn't open");
//        return;
//    }
//    experiment.getPoint(2, fi, betta);
//}

//void MainWindow::on_pushMiddle_clicked()
//{
//    if (!experiment.isFileSet()) {
//        QMessageBox::critical(this, "Holly shit!", "File doesn't open");
//        return;
//    }
//    experiment.getPoint(1, fi, betta);
//}

//void MainWindow::on_pushLow_clicked()
//{
//    if (!experiment.isFileSet()) {
//        QMessageBox::critical(this, "Holly shit!", "File doesn't open");
//        return;
//    }
//    experiment.getPoint(0, fi, betta);
//}

///* Поворот шагового двигателя */
void MainWindow::move()
{
    fi = nextFi->value();
    betta = nextBetta->value();
    arduino.revolution(fi, betta);
}


///* Открытие нового файла эксперемента */
//void MainWindow::on_actionNew_triggered()
//{
//    QString nameExperiment = QFileDialog::getSaveFileName(this,
//                                                          tr("New experiment file"),
//                                                          QDate::currentDate().toString("yyyy.MM.dd") + QTime::currentTime().toString("_hh.mm"),
//                                                          tr("Text (*.txt)"));
//    if (nameExperiment.isEmpty()) return;

//    if ( !experiment.setFile(new QFile(nameExperiment))) {
//        QMessageBox::critical(this, "Holly shit!", "This file doesn't want be open");
//        return;
//    }

//    this->setWindowTitle("Fun base - " + nameExperiment);
//}

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
