#ifndef SERIALREAD_H
#define SERIALREAD_H

#include <QThread>
#include<QSerialPort>
#include <QMutex>
class serialRead : public QThread
{
    Q_OBJECT
public:
    explicit serialRead(QObject *parent = 0);
    void initDefaultVal();
    void setPort(QSerialPort *);
    void setMutex(QMutex *);
private:
    QSerialPort *comPort;
    QMutex *m_PortLock;
    unsigned char cka ,ckb ,state ,header, id , length;
    unsigned char counter ,r_cka ,r_ckb ;
    int addr ;
    QByteArray buffer;

    void cks(char data);

signals:
    void SIGNAL_dataByteUpdate(int*,int,int);
public slots:
    void SLOT_readByteFromBuffer();
};

#endif // SERIALREAD_H
