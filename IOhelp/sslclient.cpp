#include "sslclient.h"
#include <QSslKey>
#include <QSslCertificate>
#include <QDebug>
#include <QFile>
#include <QHostAddress>
sslClient::sslClient(const QString host,int port,QObject *parent) :
    QThread(parent)
{

    mHost = host;
    mPort = port;
    _sslSocket = new QSslSocket();
    connect(_sslSocket,SIGNAL(hostFound()),this,SLOT(SLOT_hostFound()));
    connect(_sslSocket,SIGNAL(connected()),this,SLOT(SLOT_connected()));
    connect(_sslSocket,SIGNAL(encrypted()),this,SLOT(SLOT_readData()));
    connect(_sslSocket,SIGNAL(sslErrors(QList<QSslError>)),this,
            SLOT(SLOT_SSLerror(QList<QSslError>)));
    connect(this,SIGNAL(SIGNAL_emitCMD(QString)),this,
                            SLOT(SLOT_handleCMD(QString)));
    moveToThread(this);
}

sslClient::~sslClient()
{
    if(_sslSocket != NULL) {
        delete _sslSocket;
        _sslSocket = NULL;
    }
    if(this->isRunning())this->quit();
}

void sslClient::run()
{
    if(_sslSocket)_sslSocket->connectToHostEncrypted(mHost,mPort);
    exec();
}

void sslClient::SLOT_connected()
{
    qDebug() << "connected" <<  _sslSocket->localPort();
}

void sslClient::SLOT_hostFound()
{
    qDebug() << "host found";
}

void sslClient::SLOT_readCMD()
{
    if(_sslSocket)mBuffer.append(_sslSocket->readAll());
    if(mBuffer.indexOf('\n') > 0)
    {
        QList<QByteArray> listByteCmd = mBuffer.split('\n');
        Q_ASSERT(listByteCmd.count() > 0);
            for(int i = 0;i < listByteCmd.count() - 1;i++)
            {
                QString cmd(listByteCmd[i]);
                emit SIGNAL_emitCMD(cmd);
            }
            while (mBuffer.indexOf('\n') > 0) {
                   //int idx = mBuffer.indexOf();
                   //mBuffer.remove(0,idx);
            }
    }
}

void sslClient::SLOT_readData()
{
    if(_sslSocket)mBufferData.append(_sslSocket->readAll());
}

void sslClient::SLOT_checkCertError(int, const QString &str)
{

}

void sslClient::SLOT_sendCMD(QString cmd)
{

}

void sslClient::SLOT_connectionClosed()
{

}

void sslClient::SLOT_SSLerror(const QList<QSslError> &errors)
{
    qDebug() << "cert error";
    if(_sslSocket) _sslSocket->ignoreSslErrors();

}

void sslClient::SLOT_timeOut()
{

}

void sslClient::SLOT_handleCMD(QString mCmd)
{
    qDebug() << "command rec " << mCmd;
}

void sslClient::SLOT_checkUpdate()
{

}
