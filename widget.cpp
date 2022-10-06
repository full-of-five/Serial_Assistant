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
    bool status =ui->Hex_display->isChecked();

    if(status == true)
    {
     //显示窗口改变
        QByteArray Rbuff_u;
        QString Rbuff;

        Rbuff_u=(serialPort->readAll());
        origin_data.append(Rbuff_u);
        //转换8位为字符串
        Rbuff = Rbuff_u.toHex();

        //处理16进制长度
        int size = Rbuff.size();

        for(int i=0;i<(size/2);)
        {
        QChar buff = Rbuff[i*2];
        ui->recvEdit->insertPlainText(buff);
        buff = Rbuff[i*2+1];
        ui->recvEdit->insertPlainText(buff);

        ui->recvEdit->insertPlainText(" ");

        ui->recvEdit->moveCursor(QTextCursor::End);
        i++;
        }

    }
    else if(status == false)
    {
     //显示窗口改变
     QString Rbuff;
     Rbuff=QString(serialPort->readAll());
     origin_data.append(Rbuff);
     ui->recvEdit->insertPlainText(Rbuff);
     ui->recvEdit->moveCursor(QTextCursor::End);

    }

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
    bool status = ui->Hex_send->isChecked();
    QString text_send = ui->sendEdit->text();
    if (status==false)
    serialPort->write(text_send.toLocal8Bit().data());
    else if(status==true)
    {
                QByteArray sendBuf;
                if (text_send.contains(" "))
               {
                   text_send.replace(QString(" "),QString(""));//我这里是把空格去掉，根据你们定的协议来
                }
                convertStringToHex(text_send, sendBuf); //把QString 转换为 hex
                serialPort->write(sendBuf);
    }
}


void Widget::on_clearPB_clicked()
{
    ui->recvEdit->clear();
}

//Qstring 转为 16进制的函数
void Widget::convertStringToHex(const QString &str, QByteArray &byteData)
{
    int hexdata,lowhexdata;
    int hexdatalen = 0;
    int len = str.length();
    byteData.resize((len)/2);
    char lstr,hstr;
    for(int i=0; i<len; )
    {
        //char lstr,
        hstr=str[i].toLatin1();
        i++;
        if(i >= len)
            break;
        lstr = str[i].toLatin1();
        hexdata = convertCharToHex(hstr);
        lowhexdata = convertCharToHex(lstr);
        if((hexdata == 16) || (lowhexdata == 16))
            break;
        else
            hexdata = hexdata*16+lowhexdata;
        i++;
        byteData[hexdatalen] = (char)hexdata;
        hexdatalen++;
    }
    byteData.resize(hexdatalen);
}

//另一个 函数 char 转为 16进制
char Widget::convertCharToHex(char ch)
{
    /*
            0x30等于十进制的48，48也是0的ASCII值，，
            1-9的ASCII值是49-57，，所以某一个值－0x30，，
            就是将字符0-9转换为0-9

            */
    if((ch >= '0') && (ch <= '9'))
        return ch-0x30;
    else if((ch >= 'A') && (ch <= 'F'))
        return ch-'A'+10;
    else if((ch >= 'a') && (ch <= 'f'))
        return ch-'a'+10;
    else return (-1);
}



void Widget::on_Hex_display_stateChanged(int arg1)
{
    bool status =ui->Hex_display->isChecked();

    if(status == true)
    {
     //显示窗口改变
        ui->recvEdit->clear();
        QString origin_data_s;
        origin_data_s = origin_data.toHex();
        int size = origin_data_s.size();

        for(int i=0;i<(size/2);)
        {
        QChar buff = origin_data_s[i*2];
        ui->recvEdit->insertPlainText(buff);
        buff = origin_data_s[i*2+1];
        ui->recvEdit->insertPlainText(buff);

        ui->recvEdit->insertPlainText(" ");

        ui->recvEdit->moveCursor(QTextCursor::End);
        i++;
        }
    }
    else if(status == false)
    {
     //显示窗口改变
        ui->recvEdit->clear();
        QString origin_data_s;
        origin_data_s = QString(origin_data);
        ui->recvEdit->insertPlainText(origin_data_s);
        ui->recvEdit->moveCursor(QTextCursor::End);
    }
}

