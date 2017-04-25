/*********************
 2017.1.5 novice0910
 ********************/
/*q 为指针类型
 *一.是附加一个标志位tag
      当head赶上tail，队列空，则令tag=0,
      当tail赶上head，队列满，则令tag=1,

  二.限制tail赶上head，即队尾结点与队首结点之间至少留有一个元素的空间。
      队列空：   head==tail
      队列满：   (tail+1)% MAXN ==head
*/

#ifndef READSERIALDATA_H
#define READSERIALDATA_H

#include <QTimer>
#include <QtSerialPort/QSerialPort>
#include "data.h"
#include <QDebug>
#include <QSettings>
#include <QCoreApplication>
#include "checkout.h"
#include <QDateTime>

#define QUEUE_MAX 4096
#define ringq_is_empty(q) ( (q->head == q->tail) && (q->tag == 0))
#define ringq_is_full(q) ( (q->head == q->tail) && (q->tag == 1))

typedef struct ringq{
    int head; /* 头部，出队列方向*/
    int tail; /* 尾部，入队列方向*/
    int tag ;
    int size ; /* 队列总尺寸 */
    int space[QUEUE_MAX]; /* 队列空间 */
}RINGQ;


class readSerialData :public QSerialPort
{
    Q_OBJECT
public:
    readSerialData();
    bool openSerial(const QString &dev,quint32 baud);
signals:
    void signalOpenSerialResult(bool result);
//    void signalCloseSerialResult(bool result);
public slots:
    void init();
    void slotOpenSerial(const QString &dev, quint32 baud);
    void slotCloseSerial();
    void slotGetTestBuf(char* buf,int length);
private:
    QTimer *readTimer;
    checkOut * crc16;
    void receive();
private slots:
    void slotReceiveData();
};

#endif // READSERIALDATA_H
