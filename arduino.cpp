#include "arduino.h"

Arduino::Arduino()
{

}

bool Arduino::findArduino()
{
    /* Поиск порта с ардуинкой */
    QList <QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
    if(ports.isEmpty())
    {
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

    setBaudRate(QSerialPort::Baud115200);
    setDataBits(QSerialPort::Data8);
    setStopBits(QSerialPort::OneStop);
    setParity(QSerialPort::NoParity);
    setFlowControl(QSerialPort::NoFlowControl);
    open(QIODevice::ReadOnly);

    return 1;

}
