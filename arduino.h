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
    static const quint16 nullFi;
    static const quint16 nullBetta;
    Arduino();

    /* Поиск порта с ардуинкой и подключение к нему*/
    bool findArduino();

    /* Приём и расшифровка сообщения */
    bool getMessage(quint8 &numerSensor, quint16 &value, qreal &fi, qreal &betta);

public slots:
    /* Отправить поворот углов */
    void revolution(qreal fi, qreal betta);

private:
    void findMessage(char _message[] = 0);
};

#endif // ARDUINO_H
