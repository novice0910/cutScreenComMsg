#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_serialIsOpen = false;
    m_canWriteCell = false;
    construct();
    init();
}

MainWindow::~MainWindow()
{
    delete ui;
    TC_FREE(feedBack);
    TC_FREE(thrFeedBack);
    TC_FREE(readSerial);
    TC_FREE(thrReadSerial);
}

void MainWindow::construct()
{
    QStringList baud ;
    baud << "1200" << "2400" << "4800" << "9600"
         << "19200" << "38400" << "115200";
    ui->cBoxBaud->addItems(baud);
    ui->cBoxBaud->setCurrentText("9600");

    QStringList port;
    port  <<"com1" << "com2" << "com3" << "com4"
         << "com5" <<"com6" << "com7" << "com8" << "com9"<< "com10";
    ui->cBoxPort->addItems(port);
    ui->cBoxPort->setCurrentText("com0");
}

void MainWindow::init()
{
    feedBack = new feedbackSerial;
    thrFeedBack = new QThread;
    readSerial = new readSerialData;
    thrReadSerial = new QThread;

    //读串口
    readSerial->moveToThread(thrReadSerial);
    connect(thrReadSerial,SIGNAL(finished()),readSerial,SLOT(deleteLater()));
    connect(thrReadSerial,SIGNAL(started()),readSerial,SLOT(init()));
    connect(this,SIGNAL(signalOpenSerial(const QString &,quint32)),readSerial,SLOT(slotOpenSerial(const QString &,quint32)));
    connect(readSerial,SIGNAL(signalOpenSerialResult(bool)),this,SLOT(slotGetOpenSerialResult(bool)));
    connect(this,SIGNAL(signalCloseSerial()),readSerial,SLOT(slotCloseSerial()));
    connect(this,SIGNAL(signalSendTestBuf(char*,int)),readSerial,SLOT(slotGetTestBuf(char*,int)));
    thrReadSerial->start();

    //解析及反馈数据
    feedBack->moveToThread(thrFeedBack);
    connect(thrFeedBack,SIGNAL(finished()),feedBack,SLOT(deleteLater()));
    connect(thrFeedBack,SIGNAL(started()),feedBack,SLOT(init()));
    connect(feedBack,SIGNAL(signalWriteCurrentSheet(QList<QList<QVariant> >)),this,SLOT(slotWriteCurrentSheet(QList<QList<QVariant> >)));
    thrFeedBack->start();

}

void MainWindow::on_btnOpenSerial_clicked()
{
    if(m_serialIsOpen)//from open to close
    {
        emit signalCloseSerial();
        m_serialIsOpen = false;
        ui->btnOpenSerial->setText("open");
        ui->cBoxBaud->setEnabled(true);
        ui->cBoxPort->setEnabled(true);
    }
    else //from close to open
    {
        QString port = ui->cBoxPort->currentText();
        quint32 baud = ui->cBoxBaud->currentText().toInt();
        emit signalOpenSerial(port,baud);
    }
}

void MainWindow::slotGetOpenSerialResult(bool result)
{
    if(result)
    {
        m_serialIsOpen = true;
        ui->btnOpenSerial->setText("close");
        ui->cBoxBaud->setEnabled(false);
        ui->cBoxPort->setEnabled(false);
    }
    else
    {
        QMessageBox::warning(this,tr("waring"),tr("打开串口失败！！"));
        return;
    }
}

void MainWindow::on_btnSave_clicked()
{
    if(m_serialIsOpen)// serial has opened
    {
        if(m_canWriteCell == false)//from can not save to can save
        {
            m_canWriteCell = true;
            ui->btnOpenSerial->setEnabled(false);
            ui->btnSave->setText("stop save");
            QString xlsFile;
            xlsFile = RUN_PATH + "/1.xls";
            QElapsedTimer timer;
            timer.start();
            if(m_xls.isNull())
                m_xls.reset(new ExcelBase);
            m_xls->create(xlsFile);
            m_xls->setCurrentSheet(1);
//            qDebug()<<"create cost:"<<timer.elapsed()<<"ms";timer.restart();
//            QList< QList<QVariant> > m_datas;
//            for(int i=0;i<1000;++i)
//            {
//                QList<QVariant> rows;
//                for(int j=0;j<100;++j)
//                {
//                    rows.append(i*j);
//                }
//                m_datas.append(rows);
//            }
//            timer.restart();
//            m_xls->writeCurrentSheet(m_datas);
//            qDebug()<<"write cost:"<<timer.elapsed()<<"ms";timer.restart();
//            m_xls->save();
        }
        else
        {
            m_canWriteCell = false;
            ui->btnOpenSerial->setEnabled(true);
            ui->btnSave->setText("save");

            m_xls->save();
            m_xls->close();
        }
    }
    else
    {
        QMessageBox::information(this,"waring",tr("请先打开串口！"));
    }

}

void MainWindow::slotWriteCurrentSheet(const QList<QList<QVariant> > &cells)
{
    if(m_canWriteCell)
    {
        m_xls->writeCurrentSheet(cells);
    }
}

void MainWindow::on_pushButton_clicked()
{
    char buf[8] ={0x5A,0xA5,0x05,0x81,0x1E,0x01,0xE9,0x88};
    QList<QList <QVariant> >cell;
    QList <QVariant> row;//this row infomation
    for(int i=0;i<8;++i)
    {
        INTTOBCD(buf[i]);
        row.append(buf[i]);
    }
    cell.append(row);
    m_xls->appendCurrentSheetFromNextRow(cell);
}
