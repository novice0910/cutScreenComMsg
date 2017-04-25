#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include <QFileDialog>
#include <QElapsedTimer>
#include <QMessageBox>
#include "data.h"
#include "ExcelBase.h"
#include "feedbackserial.h"
#include "readserialdata.h"

#define TC_FREE(x)  {delete x; x=NULL;}

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    void construct();
    void init();
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
public slots:
    void slotGetOpenSerialResult(bool result);
    void slotWriteCurrentSheet(const QList<QList<QVariant> > &cells);
signals:
    void signalOpenSerial(const QString &,quint32);
    void signalCloseSerial();
    void signalSendTestBuf(char *buf,int length);
private slots:
    void on_btnOpenSerial_clicked();
    void on_btnSave_clicked();
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    bool m_serialIsOpen;
    bool m_canWriteCell;
    QScopedPointer<ExcelBase> m_xls;
    feedbackSerial *feedBack;
    QThread *thrFeedBack;
    readSerialData *readSerial;
    QThread *thrReadSerial;

};

#endif // MAINWINDOW_H
