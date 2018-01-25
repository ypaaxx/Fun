#include "arduino.h"
#include <QDataStream>
#include <QQueue>
#include <QThread>

const qreal Arduino::stpBetta = 0.125;
const qreal Arduino::stpFi = 0.25;
const quint16 Arduino::nullFi = 720;
const quint16 Arduino::nullBetta = 192;

Arduino::Arduino()
{
    /* Настройки COM порта */
    setBaudRate(QSerialPort::Baud9600);
    setDataBits(QSerialPort::Data8);
    setStopBits(QSerialPort::OneStop);
    setParity(QSerialPort::NoParity);
    setFlowControl(QSerialPort::NoFlowControl);
}

/* Поиск порта с ардуинкой и подключение к нему*/
bool Arduino::findArduino()
{
    /* Поиск порта с ардуинкой */
    QList <QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
    if(ports.isEmpty()){
        qDebug() << "Нет COM портов ;-(";
        return 0;
    }

    for (QSerialPortInfo& port: ports){
        if ( port.hasVendorIdentifier() )
            if ((port.vendorIdentifier() == 0x2341)
                    || (port.vendorIdentifier() == 0x1A86))
                if ( port.productIdentifier() == 0x7523){
                    //qDebug() << "Product ID: " << hex << "0x" << port.productIdentifier();
                    qDebug() << "On" << port.portName();
                    setPort(port);
                    open(QIODevice::ReadWrite);
                    return 1;
                }
    }

    //Происходит если ничего не нашёл
    return 0;
}

/* Отправка новых значений углов которые необходимо выставить */
void Arduino::revolution(qreal fi, qreal betta)
{
    quint16 stepFi = fi/stpFi + nullFi;
    quint16 stepBetta = betta/stpBetta + nullBetta;

    quint8 array[4];
    array[0] = stepFi >> 8;
    array[1] = stepFi & 0xFF;
    array[2] = stepBetta >> 8;
    array[3] = stepBetta & 0xFF;

    /* Отправка сообщения типа Fi,Betta,CRC */
    qDebug() << "Revolution " << stepFi << stepBetta ;

    QDataStream steam(this);
    steam << stepFi << stepBetta << Crc::crc8(array, 4);
}

/* Считывает сообщение на последовательном порту, распихивает данные по ссылкам
 * Если Crc не совпали, возращает false */
bool Arduino::getMessage(quint8 &numerSensor, quint16 &value, qreal &fi, qreal &betta)
{
    /* Приём и расшифровка послания */
    static char message[8];
    if (bytesAvailable() < 8) return 0;
    while (bytesAvailable() > 8){
        if (!read(message, 8)) {
            qDebug() << "Ошибка чтения";
            return false;
        }
    }

    //Когда контрольные суммы не совпали, начинает искать смещение выкидывая один пришедший байт
    if (message[7] != Crc::crc8(message, 7)) {
        qDebug() << "Не совпали CRC";
        QThread::msleep(1000);
        if(bytesAvailable())
            getChar(NULL);
        return false;
    }

    numerSensor = message[0]; // Номер сенсора
    value = ( message[1] << 8) + (quint8) message[2]; // Значение от сенсора
    quint16 fiStep = ( message[3] << 8) + (quint8) message[4];
    fi = (fiStep - nullFi) * stpFi;
    quint16 bettaStep = ( message[5] << 8) + (quint8) message[6];
    betta = (bettaStep - nullBetta) * stpBetta;

    return true;
}

