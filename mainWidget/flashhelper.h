#ifndef FLASHHELPER_H
#define FLASHHELPER_H

#include <QObject>
#include <QSerialPort>
#include <QTextStream>
#include <QThread>
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
        WriteDone = 5,
        WriteError = -5,
        FlashOk = 9,
        FlashError = -9,
        StartReconnect = 1000
    };

    explicit FlashHelper(QSerialPort *port = 0, QObject *parent = 0);
    void setFlashFilePath(const QString &flashFilePath);
    void setPort(QSerialPort *port);

public slots:
    void updateFlash();
    void emitFlashStatus(int status);

signals:
    void _flashStatusChanged(int);
    void _flashProgressChanged(int);

private:
    void write4Byte(QByteArray,char,char);

    QSerialPort *m_port;
    QTextStream m_stream;
    QString m_flashFilePath;
    int m_currentFlashPercent;
};

#endif // FLASHHELPER_H
