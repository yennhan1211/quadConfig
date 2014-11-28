#include "sslclient.h"

sslClient::sslClient(const QString host,int port,QObject *parent) :
    QThread(parent)
{
    mHost = host;
    mPort = port;
    QSslSocket *_sslSocket = new QSslSocket(this);
    _sslSocket->setLocalCertificate(":/ssl.crt",QSsl::Pem);

}

sslClient::~sslClient()
{

}

void sslClient::SLOT_readCMD()
{

}

void sslClient::SLOT_readData()
{

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

void sslClient::SLOT_error()
{

}
