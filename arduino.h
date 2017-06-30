#ifndef ARDUINO_H
#define ARDUINO_H

#include "crc.h"

#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>

class Arduino : public QSerialPort
{


public:
    const static qreal stpBetta;
    static const qreal stpFi;
    Arduino();

    /* Поиск порта с ардуинкой и подключение к нему*/
    bool findArduino();

    /* Отправить поворот углов */
    void revolution(quint16 fiStep, quint16 bettaStep);

    /* Приём и расшифровка сообщения */
    bool read(quint8 &numerSensor, quint16 &value, quint16 &fi, quint16 &betta);


};

#endif // ARDUINO_H
