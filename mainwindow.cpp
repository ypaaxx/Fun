#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setWindowTitle("FunBase");
    window()->setWindowIcon(QIcon("Logo.png"));

    isArduinoHave = new QLabel(this);
    statusBar()->addWidget(isArduinoHave);
    
    lastMessage = new QLabel(this);
    statusBar()->addWidget(lastMessage);

    /* Начало исправления всего */
    //Три насадка
    upper = new Radius;
    middle = new Radius;
    lower = new Radius;

    upper->setObjectName("upper");
    middle->setObjectName("middle");
    lower->setObjectName("lower");

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
    /* Настройка выбора позиции */
    nextFi = new QDoubleSpinBox; //Следующие положения
    nextFi->setSingleStep(Arduino::stpFi);
    nextFi->setRange(-180, 180);
    nextBetta = new QDoubleSpinBox;
    nextBetta->setDecimals(3);
    nextBetta->setSingleStep(Arduino::stpBetta);
    nextBetta->setRange(-24,24);

    connect(nextBetta, SIGNAL(valueChanged(double)), upper, SLOT(moveBettaActual(double)));
    connect(nextFi, SIGNAL(valueChanged(double)), upper, SLOT(moveFiActual(double)));
    connect(nextBetta, SIGNAL(valueChanged(double)), middle, SLOT(moveBettaActual(double)));
    connect(nextFi, SIGNAL(valueChanged(double)), middle, SLOT(moveFiActual(double)));
    connect(nextBetta, SIGNAL(valueChanged(double)), lower, SLOT(moveBettaActual(double)));
    connect(nextFi, SIGNAL(valueChanged(double)), lower, SLOT(moveFiActual(double)));

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

    //Установка таррировочных кривых
    calibration("calibration.txt");

    // Не смей включать без записи в фаил
    newFile();
}

MainWindow::~MainWindow()
{
    if (arduino.isOpen()) arduino.close();
}

/* Вызывается при появлении данных от ардуинки, читает сообщение
 * проверяет, пишет куда надо */
void MainWindow::readData()
{
    quint8 numerSensor;
    quint16 value;

    /* При совпадении crc запись в лог и серии датчика */
    if (arduino.getMessage(numerSensor, value, fi, betta)){
        experiment.addData(numerSensor, value);
        currentBetta->setText(QString::number(betta));
        currentFi->setText(QString::number(fi));
        if ((nextBetta->value() == betta)
                &&(nextFi->value() == fi))
            //Добавить изменение цвета при совпадении
            ;
    }
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

/* Поворот шагового двигателя */
void MainWindow::move()
{
    fi = nextFi->value();
    betta = nextBetta->value();
    arduino.revolution(fi, betta);
}

/* Открытие нового файла эксперемента */
void MainWindow::newFile()
{
    QString nameExperiment = QFileDialog::getSaveFileName(this,
                                                          tr("New experiment file"),
                                                          QDate::currentDate().toString("yyyy.MM.dd") + QTime::currentTime().toString("_hh.mm"),
                                                          tr("Text (*.txt)"));
    if (nameExperiment.isEmpty()) return;

    auto file = new QFile(nameExperiment);
    if ( !experiment.setFile(file)) {
        QMessageBox::critical(this, "Holly shit!", "This file doesn't want be open");
        delete file;
        return;
    }
    auto output = new QTextStream(file);
    upper->setOutput(output);
    lower->setOutput(output);
    middle->setOutput(output);

    this->setWindowTitle("FunBase - " + nameExperiment);
}

/* Установка файла с таррировочными коэффициентами */
void MainWindow::calibration(QString fileName){
    //Если не указано имя, открывается диалоговый выбора файла
    if (fileName == NULL)
        fileName = QFileDialog::getOpenFileName(this,
                                                tr("New experiment file"),
                                                QDate::currentDate().toString("yyyy.MM.dd") + QTime::currentTime().toString("_hh.mm"),
                                                tr("Text (*.txt)"));
    auto file = new QFile(fileName);
    //Если метод установки выернул каку, всё заново с диалоговым окном
    if ( !experiment.setCalibrationFile(file)) {
        calibration();
        delete file;
    }
    return;
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
