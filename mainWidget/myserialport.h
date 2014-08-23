#ifndef MYSERIALPORT_H
#define MYSERIALPORT_H
#define delayms(i)  QThread::currentThread()->msleep(i);

#include <QSerialPort>
#include <QSerialPortInfo>
#include <QtCore>
#include <QString>
#include <QThread>
#include <addrdefine.h>
#include <serialWrite.h>
#include <serialread.h>
#include <flashhelper.h>
#include <QTimer>
#include <QMutex>
class mainWidget;
class MySerialPort :  public QThread
{
    Q_OBJECT
public:
        explicit MySerialPort(mainWidget *ui, QObject *parent = 0);
        virtual ~MySerialPort();
        bool setDefaulValue(QSerialPort *port);
        void writeByteData(int addr,int numOfField,int rwStt,QSerialPort *port);
        void readAction(int i);     
        void clearSIGNAL();
protected:
         void run();
private:
        QSerialPort *myport;
        QSerialPortInfo *infoPort;
        SerialWrite _serialWrite;
        serialRead _serialRead;
        FlashHelper _flash;
        QTimer _timerScan;
        QMutex m_mutex;
        unsigned char cka ,ckb ,state ,header, id , length;
        unsigned char counter ,r_cka ,r_ckb ;
        int addr ;
        bool connectStatus;
        int m_flashstatus;
        QByteArray buffer;
        void cks(unsigned char data);
        void write4Byte(QByteArray,char,char);
        void initDefaultVal();
        void tryToHandleError();
        void startSerialReadWrite();
        void stopSerialReadWrite();
        bool openPort(QSerialPort *port);
public slots:
        void SLOT_readByteData(QSerialPort *port);
        void SLOT_eventWmiChange(int);
        void SLOT_handleSerialPortError(const QSerialPort::SerialPortError&);
        void checkDevice();
        void SLOT_HandleReconnect();
        void SLOT_startUpdateFlash(const QString);
signals:
        void SIGNAL_connectStatus(bool);
};

#endif // MYSERIALPORT_H



