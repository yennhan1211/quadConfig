#ifndef DOWNLOADHELPER_H
#define DOWNLOADHELPER_H

#include <QByteArray>
#include <QTcpSocket>
#include <QEventLoop>
#include "iohelp_global.h"

#define BUFFER_LENGTH 1024

class IOHELPSHARED_EXPORT DownloadHelper : public QObject
{
    Q_OBJECT
public:
    enum State {
        ConnectingState,
        GetFileLenState,
        GetShaCksState,
        GetFileDateState,
        ClosingState
    };

    explicit DownloadHelper(const QString&, int, QObject *parent = 0);
    ~DownloadHelper();
    void setServerAddress(const QString&, int);


signals:
    void _firmwareDownloaded(const QByteArray&);
    void _progressChanged(int);

public slots:
    void startDownload();
    void closeConnection();

private slots:
    void server_hostFound();
    void dataAvailable();
    void client_connected();
    void client_disconected();
    void socket_error(const QAbstractSocket::SocketError&);

private:
    int m_serverPort;
    QString m_serverAddr;
    QTcpSocket m_socket;

    QByteArray m_buffer;

    QEventLoop m_loop;

};

#endif // DOWNLOADHELPER_H
