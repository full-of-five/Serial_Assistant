#include "widget.h"
#include "ui_widget.h"
#include <QSerialPortInfo>
#include <QMessageBox>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    QStringList serialNamePort;
    serialPort =new QSerialPort(this);

    connect(serialPort,SIGNAL(readyRead()),this,SLOT(serialPortReadyRead_slot()));
    foreach(const QSerialPortInfo &info,QSerialPortInfo::availablePorts())
    {
        serialNamePort<<info.portName();
    }
    ui->serialCB->addItems(serialNamePort);
}

Widget::~Widget()
{
    delete ui;
}
void Widget::serialPortReadyRead_slot()
{
    QString Rbuff;
    Rbuff=QString(serialPort->readAll());
    ui->recvEdit->appendPlainText(Rbuff);
}

void Widget::on_openPB_clicked()
{
    QSerialPort::BaudRate baudRate;
    QSerialPort::DataBits dateBits;
    QSerialPort::StopBits stopBits;
    QSerialPort::Parity parityBits;
    if(ui->baudrateCB->currentText()=="4800")
    {
        baudRate=QSerialPort::Baud4800;
    }else if(ui->baudrateCB->currentText()=="9600")
    {
        baudRate=QSerialPort::Baud9600;
    }else if (ui->baudrateCB->currentText()=="115200")
    {
        baudRate=QSerialPort::Baud115200;
    }

    if(ui->dataCB->currentText()=="8")
    {
        dateBits=QSerialPort::Data8;
    }

    if(ui->stopCB->currentText()=="1")
    {
        stopBits=QSerialPort::OneStop;
    }

    if(ui->ParityCB->currentText()=="None")
    {
        parityBits=QSerialPort::NoParity;
    }

    serialPort->setPortName(ui->serialCB->currentText());
    serialPort->setBaudRate(baudRate);
    serialPort->setDataBits(dateBits);
    serialPort->setStopBits(stopBits);
    serialPort->setParity(parityBits);
    if(serialPort->open(QIODevice::ReadWrite)==true)
    {
        QMessageBox::information(this,"提示","串口打开成功");
    }
    else
    {
        QMessageBox::critical(this,"提示","串口打开失败");
    }

}


void Widget::on_cloePB_clicked()
{
    serialPort->close();
}


void Widget::on_sendPB_clicked()
{
    serialPort->write(ui->sendEdit->text().toLocal8Bit().data());
}


void Widget::on_clearPB_clicked()
{
    ui->recvEdit->clear();
}

