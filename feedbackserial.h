#ifndef FEEDBACKSERIAL_H
#define FEEDBACKSERIAL_H

#include <QObject>
#include <QTimer>
#include <unistd.h>
#include <QDateTime>
#include "checkout.h"
#include "readserialdata.h"

#define REG_LEN_BEFORE_DATA 5
#define DATA_LEN_BEFORE_DATA 6
#define MINI_LENGTH 6


class feedbackSerial : public QObject
{
    Q_OBJECT
public:
    feedbackSerial();
    ~feedbackSerial();
public slots:
    void init();
signals:
    void signalWriteCurrentSheet(const QList<QList<QVariant> > &cells);
private:
    checkOut * crc16;
    void judgeFunctionCode(char * buf,int length);
    void analyzeSetRegData(char *buf, int length);
    void analyzeSetDataData(char *buf, int length);
    void analyzeReadRegData(char *buf, int length);
    void analyzeReadDataData(char *buf, int length);

    void manageDate();
    void sendSignalWriteCurrentSheet(char *buf,int length);
    bool feedBackCrcResult(char *buf, int length);

};

#endif // FEEDBACKSERIAL_H
