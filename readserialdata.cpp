#include "readserialdata.h"

RINGQ *queue = new RINGQ;
readSerialData::readSerialData()
{
    readTimer = new QTimer;
    connect(this,SIGNAL(readyRead()),this,SLOT(slotReceiveData()));

    crc16 = new checkOut;
}

void readSerialData::init()
{
    queue->size = QUEUE_MAX ;
    queue->head = 0;
    queue->tail = 0;
    queue->tag = 0;
    memset(queue->space,0,sizeof(queue->size));
}

void readSerialData::slotOpenSerial(const QString &dev, quint32 baud)
{
    bool result = openSerial(dev,baud);
    emit signalOpenSerialResult(result);
}

void readSerialData::slotCloseSerial()
{
     close();
}

bool readSerialData::openSerial(const QString &dev,quint32 baud)
{
    setPortName(dev);
    setBaudRate(baud);
    setDataBits(QSerialPort::Data8);
    setStopBits(QSerialPort::OneStop);
    bool result = open(QIODevice::ReadWrite);
    return result;
}
void readSerialData::slotReceiveData()
{
    if(ringq_is_full(queue))
    {
        qDebug()<<"queue is full,please enlarge size"<<"queue->tag"<<queue->tag;
    }
    else
    {
        char buf[100] = {0};
        int length = readData(buf,sizeof(buf));
//        if(length>0)  qDebug()<<"receive data"<<length<<QDateTime::currentDateTime().toString("hh:MM:ss.zzz");
        for(int i =0;i<length;i++)
        {
            queue->space[queue->tail] = buf[i];
            queue->tail = (queue->tail +1)%queue->size;

            if(queue->tail == queue->head)
            {
                queue->tag = 1;//已插入满
                break;
            }
        }
    }
}

void readSerialData::receive()
{
    if(ringq_is_full(queue))
    {
        qDebug()<<"queue is full,please enlarge size"<<"queue->tag"<<queue->tag;
    }
    else
    {
        char buf[1024] = {0};
        int length = readData(buf,sizeof(buf));
//        qDebug()<<"receive data"<<length<<QDateTime::currentDateTime().toString("hh:MM:ss.zzz");
        for(int i =0;i<length;i++)
        {
            queue->space[queue->tail] = buf[i];
            queue->tail = (queue->tail +1)%queue->size;

            if(queue->tail == queue->head)
            {
                queue->tag = 1;//已插入满
                break;
            }
        }
    }
}

void readSerialData::slotGetTestBuf(char *buf, int length)
{
    writeData(buf,length);
}
