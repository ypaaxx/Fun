#include "arduino.h"
#include <QDataStream>

const qreal Arduino::stpBetta = 0.125;
const qreal Arduino::stpFi = 0.25;

Arduino::Arduino()
{

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
            if( (port.vendorIdentifier() == 0x2341)
                    || (port.vendorIdentifier() == 0x1A86) )
                qDebug() << "Vendor ID: " << hex << "0x" <<ports.at(0).vendorIdentifier();
        qDebug() << "On " << port.portName();
        setPort(port);
    }

    /* Настройки COM порта */
    setBaudRate(QSerialPort::Baud115200);
    setDataBits(QSerialPort::Data8);
    setStopBits(QSerialPort::OneStop);
    setParity(QSerialPort::NoParity);
    setFlowControl(QSerialPort::NoFlowControl);
    open(QIODevice::ReadWrite);

    return 1;
}

void Arduino::revolution(quint16 fiStep, quint16 bettaStep)
{
    QByteArray array;
    array.append(fiStep >> 8);
    array.append(fiStep & 0xFF);
    array.append(bettaStep >> 8);
    array.append(bettaStep & 0xFF);

    /* Отправка сообщения типа Fi,Betta,CRC */
    qDebug() << "Revolution " << fiStep << bettaStep ;

    QDataStream steam(this);
    steam << fiStep << bettaStep << Crc::crc8(array, 4);
}

bool Arduino::read(quint8 &numerSensor, quint16 &value, quint16 &fi, quint16 &betta)
{
    /* Приём и расшифровка послания */
    QByteArray message = readAll();

    if (message[7] != Crc::crc8(message, 7)) return false;

    numerSensor = message[0]; // Номер сенсора
    value = ( message.at(1) << 8) + (quint8) message.at(2); // Значение от сенсора
    fi = ((quint16) message.at(3) << 8) + (quint8) message.at(4);
    //
    betta = ( message.at(5) << 8) + (quint8) message.at(6);
    //

    return true;
}
