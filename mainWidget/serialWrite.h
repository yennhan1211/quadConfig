#ifndef SERIALREADWRITE_H
#define SERIALREADWRITE_H
#include <QThread>
#include <QSerialPort>
#include <addrdefine.h>
#include <QDebug>
#include <QMutex>

class SerialWrite : public QThread
{
    Q_OBJECT
public:
    explicit SerialWrite(QObject *parent = 0);
    virtual ~SerialWrite();
    void setPort(QSerialPort *);
    void setMutex(QMutex *);
    void initDefaultVal();
public slots:
    void SLOT_requestData(int addr,int len);
    void SLOT_requestWriteData(int*,int,int);
signals:
     void SIGNAL_dataByteUpdate(int*,int,int);

private:
    QSerialPort *comPort;
    QMutex *m_PortLock;
    unsigned char cka ,ckb ,state ,header, id , length;
    unsigned char counter ,r_cka ,r_ckb ;
    int addr ;
    QByteArray buffer;
    void cks(unsigned char data);
    void writeByteData(int addr,int numOfField,int rwStt);
    void writeByteDataWithCheckSum(int *data,int len,int addr);
protected:
    enum Tabs{
         TabRx = 0,
         TabConfig = 1,
         TabGain = 2,
         TabAuto = 3,
         TabSensor = 4,
         TabGimbal = 5,
         TabFirmware = 6
     };
};

#endif // SERIALREADWRITE_H
