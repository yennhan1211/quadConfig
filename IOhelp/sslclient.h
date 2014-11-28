#ifndef SSLCLIENT_H
#define SSLCLIENT_H

#include <QThread>
#include <QSslSocket>
#include <QTcpSocket>
#include <QTimer>

class sslClient : public QThread
{
    Q_OBJECT
public:
    explicit sslClient(const QString host,int port,QObject *parent = 0);
    ~sslClient();

private:
    int mPort;
    QString mHost;
    QTcpSocket *mSocket;
    QTimer      mTimer;
signals:
    void SIGNAL_serverStatus(int);
public slots:
    void SLOT_readCMD();
    void SLOT_readData();
    void SLOT_checkCertError(int, const QString &str);
    void SLOT_sendCMD(QString);
    void SLOT_connectionClosed();
    void SLOT_error();

};

#endif // SSLCLIENT_H
