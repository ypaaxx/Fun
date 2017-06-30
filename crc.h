#ifndef CRC_H
#define CRC_H

#include <QByteArray>
#include <QtGlobal>

class Crc
{
public:
    Crc();
    static quint8 crc8(const QByteArray &array, quint8 len);
    static quint8 crc8(const quint8* array, quint8 len);
};

#endif // CRC_H
