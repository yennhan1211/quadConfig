#ifndef SSLCLIENT_H
#define SSLCLIENT_H

#include <QThread>
#include <QSslSocket>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QTimer>
#include <QFile>
#include <QStandardPaths>
#include <cmddef.h>
#include <rsacrypto.h>
#include <QProcess>
#include "iohelp_global.h"


class  IOHELPSHARED_EXPORT sslClient : public QThread
{
    Q_OBJECT


public:
    enum mStatus
    {
        NewSoftWareAvailable = 1,
        NewFirmWareAvailable = 2,
        LastestSoftWare      = 3,
        LastestFirmWare      = 4,
        DownloadBreak         = 5,
        NoConnection         = 6,
        RequestTimeOut       = 7,
        StartDownload        = 8,
        Connecting           = 9,
        ServerNotFound       = 10,
        DownloadError        = 11
    };
    explicit sslClient(const QString host,int port,QObject *parent = 0);
    ~sslClient();
    void setEnableBreak(bool);
    void run();
private:
    QByteArray  mBuffer;
    QByteArray  mBufferData;
    QByteArray  mBufferDataTmp;
    QByteArray  mKey;
    QByteArray  mEncryptToken;
    int         mPort;
    QString     mHost;
    QSslSocket  *_sslSocket;
    QTimer      mTimer;
    QTimer      mTimeOutResquet;
    QTimer      mTimeOutDownload;
    QTimer      mSpeedCalc;
    qint64      mByteTotalRec;
    qint64      mFileSize;
    qint64      mTotalByteEmit;
    qint64      mByteWriteToTmp;
    qint64      mByteRecTmp;
    RsaCrypto   mRSA;
    QString     mCurrentRequestCMD;
    QString     mHashRec;
    QFile       m_File;
    QString     mTmpPath;
    QString     mTmpFileName;
    QString     mTypeCheck;
    bool        mLogon;
    bool        fileSizeFlag;
    bool        isDownloading;
    bool        isDataTranfer;
    bool        enableBreak;
// install infomation
    QString     mInstallFilePath;
    QString     mInstallFileInfoPath;
    QString     mVersion;
    QString     mVersionRec;
// breaktion download info
    QString     mDownloadFilePath;// duong dan file thong tin
    QString     mBreakFilePath;// duong dan file da luu
    QString     mBreakFileHash;
    QString     mTimeBreak;
    bool        mIsBreak;
    qint64      mBreakFileSize;
    qint64      mCurrentPosBreakInFile;

    void writeLog(QString error);
    void writeInstallInfo();
    void writeDownloadInfo();
    bool checkDownloadInfo();
    bool checkInstallInfo();
    void sendCMD(QString cmd);
signals:
    void SIGNAL_appendTmpFile();
    void SIGNAL_checkFile();
    void SIGNAL_emitCMD(const QString);
    void SIGNAL_emitCheckUpdate();
    void SIGNAL_getSoft();
    void SIGNAL_getFirm();
//Public signal
    void SIGNAL_checkStatus(int);
    void SIGNAL_percentDownload(int);
    void SIGNAL_firmwareArrival(const QByteArray);
    void SIGNAL_softwareArrival(const QString);
private slots:
    void SLOT_SpeedCalc();
    void SLOT_connected();
    void SLOT_hostFound();
    void SLOT_readCMD();
    void SLOT_readData();
    void SLOT_sendCMD(QString);
    void SLOT_connectionClosed();
    void SLOT_SSLerror(const QList<QSslError> &errors);
    void SLOT_timeOut();
    void SLOT_handleCMD(QString);
    void SLOT_connectionError(QAbstractSocket::SocketError);
    void SLOT_RequestTimeout();
    void SLOT_writeTmpFile();
    void SLOT_checkFile();
    void SLOT_transferTimeOut();
public slots:
    void SLOT_checkUpdate(QString);
    void SLOT_getUpdate();
    void SLOT_getSoftWare();
    void SLOT_getFirmWare();
    void SLOT_Mainclose();

};

#endif // SSLCLIENT_H
