#ifndef WIDGET_H
#define WIDGET_H
#include <QString>
#include <QWidget>
#include <QSerialPort>
QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    QSerialPort *serialPort;
private slots:
    void on_openPB_clicked();

    void on_cloePB_clicked();

    void serialPortReadyRead_slot();

    void on_sendPB_clicked();

    void on_clearPB_clicked();



    void on_Hex_display_stateChanged(int arg1);

private:
    Ui::Widget *ui;
    char convertCharToHex(char ch);
    void convertStringToHex(const QString &str, QByteArray &byteData);
    QByteArray origin_data;
};
#endif // WIDGET_H
