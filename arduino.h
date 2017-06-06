#ifndef ARDUINO_H
#define ARDUINO_H

#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>

class Arduino : public QSerialPort
{
public:
    Arduino();

    /* Поиск порта с ардуинкой и подключение к нему*/
    bool findArduino();

    /* Поверернуть бэтта на количество шагов */
    bool bettaRevolutionOnSteps(int stepsNumber);

    /* Повернуть фи на количество шагов */
    bool fiRevolutionOnSteps(int stepsNumber);



};

#endif // ARDUINO_H
