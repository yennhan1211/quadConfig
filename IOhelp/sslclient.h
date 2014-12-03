#ifndef SSLCLIENT_H
#define SSLCLIENT_H

#include <QThread>
#include <QSslSocket>
#include <QTcpSocket>
#include <QTimer>
#include "iohelp_global.h"
class IOHELPSHARED_EXPORT sslClient : public QThread
{
    Q_OBJECT
public:
    explicit sslClient(const QString host,int port,QObject *parent = 0);
    ~sslClient();
    void run();
private:
    QByteArray  mBuffer;
    QByteArray  mBufferData;
    int         mPort;
    QString     mHost;
    QSslSocket  *_sslSocket;
    QTimer      mTimer;
signals:
    void SIGNAL_serverStatus(int);
    void SIGNAL_emitCMD(const QString);
public slots:

    void SLOT_connected();
    void SLOT_hostFound();
    void SLOT_readCMD();
    void SLOT_readData();
    void SLOT_checkCertError(int, const QString &str);
    void SLOT_sendCMD(QString);
    void SLOT_connectionClosed();
    void SLOT_SSLerror(const QList<QSslError> &errors);
    void SLOT_timeOut();
    void SLOT_handleCMD(QString);
    void SLOT_checkUpdate();
};

#endif // SSLCLIENT_H
