#include "readSerialScreen.h"

ReadSerialScreen::ReadSerialScreen()
{
    connect(this,SIGNAL(readyRead()),this,SLOT(slotReadMyCom()));
    memset(m_buf,0,sizeof(m_buf));
}

void ReadSerialScreen::slotOpenSerial(const QString &dev, quint32 baud)
{
    openSerial(dev,baud);
}

void ReadSerialScreen::openSerial(const QString &dev,quint32 baud)
{
    setPortName(dev);
    open(QIODevice::ReadWrite);
    setBaudRate(baud);
    setDataBits(QSerialPort::Data8);
    setStopBits(QSerialPort::OneStop);
}

void ReadSerialScreen::slotReadMyCom()
{
    int ret = readData(m_buf,sizeof(m_buf));
}

void ReadSerialScreen::judgeFuncitonCode(char *buf, int length)
{

}

void ReadSerialScreen::closeSerial()
{
    close();
}

