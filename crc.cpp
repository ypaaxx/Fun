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

char Crc::crc8(const char *array, quint8 len)
{
    char crc = 0x00;
    while (len--){
        crc ^= *array++;
        for (quint8 i = 0; i < 8; i++)
            crc = crc & 0x80 ? (crc << 1) ^ 0x07 : crc << 1;
    }

    return crc;
}
