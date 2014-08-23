#include "downloadhelper.h"
#include <QDebug>
#include <QFile>
#include <QThread>

DownloadHelper::DownloadHelper(const QString &addr, int port, QObject *parent)
    : QObject(parent)
{
    qRegisterMetaType<QAbstractSocket::SocketError>("QAbstractSocket::SocketError");

    setServerAddress(addr, port);
}

DownloadHelper::~DownloadHelper()
{
    closeConnection();
}

void DownloadHelper::setServerAddress(const QString &addr, int port)
{
    m_serverAddr = addr;
    m_serverPort = port;
}

void DownloadHelper::startDownload()
{
    connect(&m_socket, SIGNAL(hostFound()), this, SLOT(server_hostFound()));
    connect(&m_socket, SIGNAL(connected()), this, SLOT(client_connected()));
    connect(&m_socket, SIGNAL(disconnected()), this, SLOT(client_disconected()));
    connect(&m_socket, SIGNAL(readyRead()), this, SLOT(dataAvailable()));
    connect(&m_socket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(socket_error(QAbstractSocket::SocketError)));

    qDebug() << "start connect to server";
    m_socket.connectToHost(m_serverAddr, m_serverPort);

    if (! m_loop.isRunning()) {
        m_loop.exec();
    }
}

void DownloadHelper::closeConnection()
{
    qDebug() << "close connection";
    m_socket.close();
}

void DownloadHelper::server_hostFound()
{
    qDebug() << "host found";
}

void DownloadHelper::dataAvailable()
{
    while (!m_socket.atEnd()) {
        QByteArray bytes = m_socket.read(BUFFER_LENGTH);
        m_buffer.append(bytes);
        qDebug() << "buffer length" << m_buffer.length();
        emit _progressChanged(qRound((m_buffer.length()/25140.0d) * 100));
    }
}
// 0xaa 0x55 0xb5 0x62 0xXX
void DownloadHelper::client_connected()
{
    qDebug() << "client connected to " << m_serverAddr
             << "at port" << m_serverPort;
}

void DownloadHelper::client_disconected()
{
    qDebug() << "client disconnected to " << m_serverAddr
             << "at port" << m_serverPort;

    if (m_buffer.length() > 0) {
        emit _firmwareDownloaded(m_buffer);
    }
    m_loop.exit(0);
}

void DownloadHelper::socket_error(const QAbstractSocket::SocketError &)
{
    qDebug() << "socket error" << m_socket.errorString();
}
