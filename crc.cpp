#include "crc.h"

Crc::Crc()
{

}

quint8 Crc::crc8(const quint8 *array, quint8 len)
{
    quint8 crc = 0x00;
    while (len--){
        crc ^= *array++;
        for (quint8 i = 0; i < 8; i++)
            crc = crc & 0x80 ? (crc << 1) ^ 0x07 : crc << 1;
    }

    return crc;
}
quint8 Crc::crc8(const QByteArray &array, quint8 len)
{
    //ОНо рабОТатЕт!!1111АДИН
    quint8 crc = 0x00;
    for( qint8 num = 0; num < len; ++num){
        crc ^= array[num];
        for (quint8 i = 0; i < 8; i++)
            crc = crc & 0x80 ? (crc << 1) ^ 0x07 : crc << 1;
    }

    return crc;
}
