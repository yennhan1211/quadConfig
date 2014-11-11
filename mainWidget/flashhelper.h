#ifndef FLASHHELPER_H
#define FLASHHELPER_H

#include <QObject>
#include <QSerialPort>
#include <QTextStream>
#include <QThread>
#include <QMutex>
#include <QTimer>

class FlashHelper : public QObject
{
    Q_OBJECT
public:
    enum FlashStatus
    {
        PortCannotOpen = -1,
        FileNotExist = -2,
        CannotJumpToBoot = -3,
        StartEraseFlash = 0,
        EraseFlashFinished = 10,
        EraseFlashFail = 20,
        WriteDone = 5,
        WriteError = -5,
        FlashOk = 9,
        FlashError = -9,
        StartReconnect = 1000,
        NotFoundDevice = 100,
        JumpFail = 30,
        ToltalByteWRong = 40,
    };

    explicit FlashHelper(QSerialPort *port = 0, QObject *parent = 0);
    void setFlashFilePath(const QString &flashFilePath);
    void setPort(QSerialPort *port);
    void setMutex(QMutex *);

public slots:
    void updateFlash();
    void updateFlashNew();
    void SLOT_ReadByteFromBuffer();
    void emitFlashStatus(int status);
    void SLOT_timeOut();
signals:
    void _flashStatusChanged(int);
    void _flashProgressChanged(int);
    void _flashFinish(int);

private:
    void write4Byte(QByteArray,char,char);
    void writeByte(QByteArray,char,char);
    QMutex *mPortLock;
    QSerialPort *m_port;
    QTextStream m_stream;
    QString m_flashFilePath;
    QByteArray mByteToWrite;
    QTimer mTimer;
    int mFileLen;
    int mBuffSize;
    int mBuffLen;
    int mBuffType;
    int m_currentFlashPercent;
    int timeOutToRespon;
    bool responFlag;
    unsigned char cka ,ckb ,state , mId ,tmpData;
    unsigned char rcka ,rckb ;
};

#endif // FLASHHELPER_H
