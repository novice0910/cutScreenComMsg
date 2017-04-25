#ifndef READSERIALSCREEN_H
#define READSERIALSCREAN_H

#include <QtSerialPort/QSerialPort>

class ReadSerialScreen :public QSerialPort
{
public:
    ReadSerialScreen();
    void openSerial(const QString &dev, quint32 baud);
    void closeSerial();
    void judgeFuncitonCode(char *buf,int length);
public slots:
    void slotOpenSerial(const QString &dev,quint32 baud);
private slots:
     void slotReadMyCom();
private:
     char m_buf[1024];
};

#endif // READSERIALSCREAN_H
