#include "feedbackserial.h"

extern RINGQ *queue;
feedbackSerial::feedbackSerial()
{
    queue->size = QUEUE_MAX;

    crc16 = new checkOut;
}

feedbackSerial::~feedbackSerial()
{
}

void feedbackSerial::init()
{
    manageDate();
}

void feedbackSerial::manageDate()
{
    quint8 step = 0;
    quint32 rev_delay = 0;
    while(1)
    {
        usleep(200);
        switch (step) {
        case 0:
            while(queue->tail != queue->head)
            {
                if((queue->space[queue->head%queue->size] == 0x5a)
                        && (queue->space[(queue->head +1)%queue->size]==0xa5))
                {
                    step = 1;
                    break;
                }
                else {
                    queue->head = (queue->head +1)%queue->size;
                }
            }
            break;
        case 1: //判断功能码
        {
            if((queue->space[(queue->head +3)%queue->size]==0x81)
            ||(queue->space[(queue->head +3)%queue->size]== 0x82)
            ||(queue->space[(queue->head +3)%queue->size]== 0x83)
            ||(queue->space[(queue->head +3)%queue->size]== 0x80))
            {
                step = 2;
                rev_delay = 0;
                break;
            }
            else
            {
                step = 0;
                queue->head = (queue->head + 1)%queue->size;
            }
        }
            break;
        case 2://判断长度
        {
            int bufLength = (queue->space[queue->head +2]%queue->size) + 3;//报文总长度
            if (((queue->tail - queue->head + queue->size)%queue->size) >= bufLength)//enough
            {
                char *tmpBuf = new char[bufLength];
                for(int j=0;j < bufLength;j++)
                {
                    tmpBuf[j] = queue->space[queue->head];
                    queue->head = (queue->head + 1)%queue->size;
                    if(queue->head == queue->tail)//全部读完
                    {
                        queue->tag = 0;
                    }
                }
                step = 0;
                rev_delay = 0;
                judgeFunctionCode(tmpBuf,bufLength);
            }
            else//等待一会
            {
                if(rev_delay <= 100)//100ms
                {
                    usleep(100);
                    rev_delay ++;
                    break;
                }
                else//超时
                {
                    step =0;
                    rev_delay =0;
                    queue->head = (queue->head + 1)%queue->size;
                }
            }
        }
            break;
        default:
            break;
        }
    }
}

void feedbackSerial::judgeFunctionCode(char *buf, int length)
{
    switch (buf[3]) {
    case 0x80: //kzq向寄存器写入数据
//        qDebug()<<__FILE__<<__LINE__<<"buf is 0x80";
        analyzeSetRegData(buf,length);
        break;
    case 0x81://kzq读取寄存器地址
//        qDebug()<<__FILE__<<__LINE__<<"buf is 0x81";
        analyzeReadRegData(buf,length);
        break;
    case 0x82://kzq向变量存储区写入数据
//        qDebug()<<__FILE__<<__LINE__<<"buf is 0x82";
        analyzeSetDataData(buf,length);
        break;
    case 0x83://kzq读取变量存储区数据
//        qDebug()<<__FILE__<<__LINE__<<"buf is 0x83";
        analyzeReadDataData(buf,length);
        break;
    default:
        break;
    }
}

void feedbackSerial::analyzeSetRegData(char *buf,int length)
{
    if(!feedBackCrcResult(buf,length))
        return;
    sendSignalWriteCurrentSheet(buf,length);
}

void feedbackSerial::analyzeSetDataData(char *buf,int length)
{
    if(!feedBackCrcResult(buf,length))
        return;
    sendSignalWriteCurrentSheet(buf,length);
}

void feedbackSerial::analyzeReadRegData(char *buf,int length)
{
    if(!feedBackCrcResult(buf,length))
        return;
    sendSignalWriteCurrentSheet(buf,length);
}

void feedbackSerial::analyzeReadDataData(char *buf,int length)
{
    if(!feedBackCrcResult(buf,length))
        return;
    sendSignalWriteCurrentSheet(buf,length);
}

bool feedbackSerial::feedBackCrcResult(char *buf, int length)
{
    int tmp = crc16->ANSI_CRC16(buf+3,buf[2]-2);
    if(!((((tmp>>8)&0xff) == buf[length -2])&&((tmp&0xff) == buf[length-1])))
    {

        qDebug()<<"crcErro";
        qDebug("crcErro %02x",buf[length -2]);
        qDebug("crcErro %02x",buf[length -1]);
        return false;
    }
    else
    {
        return true;
    }
}
void feedbackSerial::sendSignalWriteCurrentSheet(char *buf, int length)
{
    QList<QList <QVariant> >cell;
    QList <QVariant> row;//this row infomation
    for(int i=0;i<length;++i)
    {
        row.append(buf[i]);
    }
    cell.append(row);
    emit signalWriteCurrentSheet(cell);
}

