#include "widget.h"
#include "ui_widget.h"
#include <QSerialPortInfo>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    QStringList serialNamePort;
    serialPort =new QSerialPort(this);
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


void Widget::on_openPB_clicked()
{

}

