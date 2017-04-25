#ifndef CHECKOUT_H
#define CHECKOUT_H

#include <QString>
#include <QDateTime>
#include <QDebug>
class checkOut
{
public:
    checkOut();
    bool dataTimeCheck(QString dataTime);
    bool dataCheck(QString data);
    bool timeCheck(QString time);
    bool isLeapYear(int year);

    int ANSI_CRC16(char *buf,int length);
};

#endif // CHECKOUT_H
