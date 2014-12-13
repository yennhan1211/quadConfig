#include "sslclient.h"
#include <QSslKey>
#include <QSslCertificate>
#include <QDebug>
#include <QFile>
#include <QHostAddress>
#include <QDir>
#include <QJsonObject>
#include <QJsonDocument>
#include <QCoreApplication>
#include <iostream>

sslClient::sslClient(const QString host,int port,QObject *parent) :
    QThread(parent),_sslSocket(0)
{

    mHost = host;
    mPort = port;
    mLogon =  false;
    mIsBreak = false;
    mBreakFileSize=0;
    mCurrentPosBreakInFile=0;
    isDataTranfer =  false;
    isDownloading = false;
    QFile f(":/ssl.key");f.open(QIODevice::ReadOnly);
    mKey  = f.readAll();
    if(!mRSA.createRsaPrivateKey(mKey.data()))qDebug() << "creat RSA fail";
    connect(this,SIGNAL(SIGNAL_emitCMD(QString)),this,
                            SLOT(SLOT_handleCMD(QString)));
    connect(&mTimer,SIGNAL(timeout()),this,SLOT(SLOT_timeOut()));
    connect(&mTimeOutResquet,SIGNAL(timeout()),this,SLOT(SLOT_RequestTimeout()));
    connect(this,SIGNAL(SIGNAL_appendTmpFile()),this,SLOT(SLOT_writeTmpFile()));
    connect(this,SIGNAL(SIGNAL_checkFile()),this,SLOT(SLOT_checkFile()));   
    connect(&mTimeOutDownload,SIGNAL(timeout()),this,SLOT(SLOT_transferTimeOut()));
    connect(this,SIGNAL(SIGNAL_emitCheckUpdate()),this,SLOT(SLOT_timeOut()));
    connect(this,SIGNAL(SIGNAL_getFirm()),this,SLOT(SLOT_getFirmWare()));
    connect(this,SIGNAL(SIGNAL_getSoft()),this,SLOT(SLOT_getSoftWare()));
    mTimeOutResquet.setInterval(TIMEOUT);
    mTimeOutResquet.setSingleShot(true);
    mTimeOutResquet.moveToThread(this);
    mTimeOutDownload.setInterval(TIMEOUT_DOWNLOAD);
    mTimeOutDownload.moveToThread(this);
    mTimer.moveToThread(this);
    moveToThread(this);
}

sslClient::~sslClient()
{

    if(_sslSocket != NULL) {
        if(_sslSocket->state() == QSslSocket::ConnectedState)_sslSocket->abort();


        delete _sslSocket;
        _sslSocket = NULL;
    }
}

void sslClient::run()
{
    qDebug() << "ssl client start" << QThread::currentThreadId();
    mTmpPath = QStandardPaths::standardLocations(QStandardPaths::DataLocation)[0];
    QDir mDir(mTmpPath);
    if(!mDir.exists())mDir.mkdir(mTmpPath);
    mInstallFilePath = mTmpPath +"/" + INSTALL_INFO;
    mDownloadFilePath = mTmpPath +"/" + DOWNLOAD_INFO;

//    if(checkDownloadInfo())
//    {
//        if(!mTimeOutDownload.isActive())mTimeOutDownload.start();
//    }
//    mTimer.setInterval(500);
//    mTimer.setSingleShot(true);
//    mTimer.start();
    exec();
}

void sslClient::writeLog(QString error)
{
    QFile mFile(LOG_PATH);
    mFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append);
    QString content ="[" + QDateTime::currentDateTime().toString()+"]"+" "  + error +"\r\n";
    mFile.write(content.toUtf8());
    mFile.close();
}

void sslClient::writeInstallInfo()
{
     QFile f(mInstallFilePath);
     if(f.open(QIODevice::WriteOnly))
     {
         QJsonObject tmpJson;
         tmpJson["Version"] = mVersion;
         tmpJson["InstallDate"] = QDateTime::currentDateTime().toString();
         QJsonDocument saveDoc(tmpJson);
         f.write(saveDoc.toJson());
     }
}

void sslClient::writeDownloadInfo()
{
    QFile f(mDownloadFilePath);
    qDebug() << "write download info";
    if(f.open(QIODevice::WriteOnly))
    {
        QJsonObject tmpJson;
        tmpJson["BreakFlag"] = mIsBreak;
        tmpJson["Path"] = mTmpFileName;
        tmpJson["Hash"] = mHashRec;
        tmpJson["Time"] =  QDateTime::currentDateTime().toString();
        tmpJson["FileSize"] = mFileSize;
        tmpJson["Pos"] = mByteWriteToTmp;
        QJsonDocument saveDoc(tmpJson);
        f.write(saveDoc.toJson());
    }
}

bool sslClient::checkDownloadInfo()
{
    QFile f(mDownloadFilePath);// duong dan file thong tin
    if(!f.exists())return false;
    if(f.open(QIODevice::ReadOnly))
    {
        qDebug() << "open download config file ok";
        QByteArray saveDAta = f.readAll();
        QJsonDocument loadDoc(QJsonDocument::fromJson(saveDAta));
        QJsonObject json = loadDoc.object();
        mIsBreak = json["BreakFlag"].toBool();
        if(!mIsBreak)return mIsBreak;
        mBreakFilePath = json["Path"].toString();// duong dan file da luu
        if(mBreakFilePath.length() == 0){mIsBreak = false;return false;}
        QFile f(mBreakFilePath);qint64 tmp64;
        if(!f.exists()){mIsBreak = false;return false;}
        else {
            f.open(QIODevice::ReadOnly);
             tmp64 = f.readAll().size();
            f.close();
        }
        mBreakFileHash = json["Hash"].toString();
        if(mBreakFileHash.length() == 0){mIsBreak = false;return false;}
        mBreakFileSize = (qint64)json["FileSize"].toDouble();
        if(mBreakFileSize == 0){mIsBreak = false;return false;}
        mCurrentPosBreakInFile = (qint64)json["Pos"].toDouble();
        if(mCurrentPosBreakInFile >= mBreakFileSize || mCurrentPosBreakInFile== 0 ){mIsBreak = false;return false;}
        if(tmp64 != mCurrentPosBreakInFile)qDebug() << "file size not match";


        return true;
    }

}

void sslClient::checkInstallInfo()
{

}

void sslClient::sendCMD(QString cmd)
{

    mCurrentRequestCMD = cmd;
    mTimeOutResquet.start();
    qDebug() << "data send" << cmd ;
     _sslSocket->write(cmd.toLocal8Bit());
}

void sslClient::SLOT_connected()
{
    qDebug() << "connected" <<  _sslSocket->localPort();
    QString loginSTR = "USER %1\r\n";
    sendCMD(loginSTR.arg(USERNAME));
}

void sslClient::SLOT_hostFound()
{
    qDebug() << "host found";
}

void sslClient::SLOT_readCMD()
{
    if(_sslSocket)mBuffer.append(_sslSocket->readAll());

            int idx ;
            while ((idx = mBuffer.indexOf("\r\n")) > 0) {
                   QByteArray ba = mBuffer.mid(0,idx);
                   qDebug() << QString(ba);
                   emit SIGNAL_emitCMD(QString(ba));
                   mBuffer.remove(0,idx + 2);
            }

}

void sslClient::SLOT_readData()
{
    if(_sslSocket){
        int num = _sslSocket->bytesAvailable();
        isDataTranfer = true;
        mByteTotalRec += num;
        mBufferData.append(_sslSocket->readAll());
        if(mFileSize > (MAX_BUFFER_SZIE)){
            fileSizeFlag = true ;
            mTotalByteEmit += mBufferData.size();
            mBufferDataTmp.append(mBufferData);
            mBufferData.clear();
            emit SIGNAL_appendTmpFile();
        }
        if(mByteTotalRec == mFileSize)
        {
            if(mTotalByteEmit < mFileSize && fileSizeFlag)
            {
                qDebug() << "emit last package" << mBufferData.size();
                mBufferDataTmp.append(mBufferData);
                mTotalByteEmit += mBufferData.size();
                mBufferData.clear();
                emit SIGNAL_appendTmpFile();
            }
            qDebug() << "rec total byte ok " << mByteTotalRec << mTotalByteEmit;
            disconnect(_sslSocket,SIGNAL(readyRead()),0,0);
            connect(_sslSocket,SIGNAL(readyRead()),this,SLOT(SLOT_readCMD()));
            if(!fileSizeFlag)emit SIGNAL_checkFile();// check firm
        }
       emit SIGNAL_percentDownload(mByteTotalRec*100 / mFileSize);
    }
}


void sslClient::SLOT_sendCMD(QString cmd)
{

}
void sslClient::SLOT_connectionClosed()
{
    qDebug() << "close main";
}
void sslClient::SLOT_SSLerror(const QList<QSslError> &errors)
{
    qDebug() << "cert error";
    if(_sslSocket) _sslSocket->ignoreSslErrors();

}
void sslClient::SLOT_timeOut()
{
     if(_sslSocket == NULL)
     {
         qDebug() << "creat new socket";
        _sslSocket = new QSslSocket();
        _sslSocket->moveToThread(this);
        connect(_sslSocket,SIGNAL(hostFound()),this,SLOT(SLOT_hostFound()));
        connect(_sslSocket,SIGNAL(connected()),this,SLOT(SLOT_connected()));
        connect(_sslSocket,SIGNAL(readyRead()),this,SLOT(SLOT_readCMD()));
        connect(_sslSocket,SIGNAL(sslErrors(QList<QSslError>)),this,
                SLOT(SLOT_SSLerror(QList<QSslError>)));
        connect(_sslSocket,SIGNAL(error(QAbstractSocket::SocketError)),this,
                SLOT(SLOT_connectionError(QAbstractSocket::SocketError)));
        mCurrentRequestCMD = CMD_CONNECTHOST;
        if( !mTimeOutResquet.isActive())mTimeOutResquet.start();
       _sslSocket->connectToHost(mHost,mPort);
       emit SIGNAL_checkStatus(sslClient::Connecting);
     }
     else if(_sslSocket->state() == QSslSocket::ConnectedState)
     {
         if( !mTimeOutResquet.isActive())mTimeOutResquet.start();
             if(mLogon){
                 qDebug() <<"try request";
                 QString strver = CMD_VER;
                 sendCMD(strver.arg("1.0.0").arg("1.0.0"));
             }
             else
             {
                 SLOT_connected();
             }
     }
}

void sslClient::SLOT_handleCMD(QString mCmd)
{
    qint64 cmdCode = 0;
    cmdCode = mCmd.mid(0,3).toInt();
    qDebug() << "Command rec " << cmdCode;
    if(mTimeOutResquet.isActive())mTimeOutResquet.stop();
    if(cmdCode == TOKEN_CONFIRM)
    {
        QByteArray mSig;
        mRSA.sign(QByteArray(USERNAME),mSig);
        _sslSocket->write("Sign ");
        _sslSocket->write(mSig.toHex());
        _sslSocket->write("\r\n");
        qDebug() << "my sig->" << mSig.toHex();
    }
    else if(cmdCode == LOGIN_SUCCESS)
    {
         mLogon = true ;
         QString strver = CMD_VER;
         sendCMD(strver.arg(SOFTWARE_VERSION).arg(FIRMWARE_VERSION));
         qDebug() << "login success";
    }
    else if(cmdCode == VER_CONFIRM)
    {
        if(mTypeCheck == SOFTWARE) {
            QString strcheck = CMD_CHECK;
            sendCMD(strcheck.arg(SOFTWARE));
        }
        else if(mTypeCheck == FIRMWARE)
        {
            QString strcheck = CMD_CHECK;
            sendCMD(strcheck.arg(FIRMWARE));
        }
         qDebug() << "version confirm";
    }
    else if(cmdCode == VER_NOT_CONFIRM)
    {
//        QString strver = CMD_VER;
//        sendCMD(strver.arg("0.0.0").arg("0.0.1"));
    }
    else if(cmdCode == NEW_VER && mCurrentRequestCMD.contains(SOFTWARE))
    {
//            SLOT_getSoftWare();
            emit SIGNAL_checkStatus(sslClient::NewSoftWareAvailable);
            qDebug() << "new software available";
    }
    else if(cmdCode == NEW_VER && mCurrentRequestCMD.contains(FIRMWARE))
    {
//            SLOT_getFirmWare();
            emit SIGNAL_checkStatus(sslClient::NewFirmWareAvailable);
            qDebug() << "new firmware available";
    }
    else if(cmdCode == LASTEST_VER && mCurrentRequestCMD.contains(SOFTWARE))
    {
            emit SIGNAL_checkStatus(sslClient::LastestSoftWare);
            qDebug() << "lastest software ";
    }
    else if(cmdCode == LASTEST_VER && mCurrentRequestCMD.contains(FIRMWARE))
    {
            emit SIGNAL_checkStatus(sslClient::LastestFirmWare);
            qDebug() << "lastest firmware ";
    }
    else if(cmdCode == GET_ACCEPT)
    {
            int _start = mCmd.indexOf('(')+1;
            int _end   = mCmd.indexOf(')');
            QString strData = mCmd.mid(_start,_end - _start);
            QStringList lstr = strData.split(",");
            Q_ASSERT(lstr.size() > 0);
            mHashRec.clear();
            mHashRec = lstr[0];
            mFileSize = lstr[1].toLongLong();
            if(!mIsBreak)
            {
                mTmpFileName.clear();
                if(mFileSize > MAX_BUFFER_SZIE)
                {
                   QString time =  QTime::currentTime().toString();
                   time = time.replace(":","");
                   mTmpFileName =mTmpPath +"/" + time +".exe";
                   qDebug() << "my Path" << mTmpFileName;
                   if(mTmpFileName.length() == 0){qDebug() <<"wrong path";}
                   m_File.setFileName(mTmpFileName);
                }
                qDebug() << "hash rec " << mHashRec << " file size " << mFileSize;
            }
            else{
                 qDebug() << mTmpFileName;
                 m_File.setFileName(mTmpFileName);
            }
    }
    else if(cmdCode == START_DOWNLOAD)
    {
            qDebug() << "start download";
//

            disconnect(_sslSocket,SIGNAL(readyRead()),0,0);
            connect(_sslSocket,SIGNAL(readyRead()),this,SLOT(SLOT_readData()));
            if(!mTimeOutDownload.isActive())mTimeOutDownload.start();
            if(mIsBreak){
                QString cmd = CMD_READY_REST;
                QString pos = QString::number(mCurrentPosBreakInFile);
                emit SIGNAL_checkStatus(sslClient::StartDownload);
                sendCMD(cmd.arg(pos));
            }
            else
            {
                mByteTotalRec = 0;
                mTotalByteEmit = 0;
                mByteWriteToTmp = 0;
                fileSizeFlag =  false;
               emit SIGNAL_checkStatus(sslClient::StartDownload);
               sendCMD(CMD_READY);
            }

            if(mTimeOutResquet.isActive())mTimeOutResquet.stop();
    }
}

void sslClient::SLOT_checkUpdate(QString typeCheck)
{
        if(isDownloading || mLogon)return;
        qDebug() << "type end" << typeCheck;
        if(typeCheck == SOFTWARE || typeCheck == FIRMWARE){
        emit SIGNAL_emitCheckUpdate();
        mTypeCheck = typeCheck;
        }
        else return;
}

void sslClient::SLOT_getUpdate()
{
    if(mTypeCheck == SOFTWARE)emit SIGNAL_getSoft();
    else if(mTypeCheck == FIRMWARE)emit SIGNAL_getFirm();
}

void sslClient::SLOT_getSoftWare()
{
    QString strcheck = CMD_GET;
    sendCMD(strcheck.arg(SOFTWARE));
}

void sslClient::SLOT_getFirmWare()
{
    QString strcheck = CMD_GET;
    sendCMD(strcheck.arg(FIRMWARE));
}

void sslClient::SLOT_Mainclose()
{
    qDebug() << "sslclient quiting" << isDownloading << isDataTranfer;
    if(isDownloading && isDataTranfer){
        qDebug() << "force close when downlading";
        mIsBreak = true;
        writeDownloadInfo();
    }
}

void sslClient::SLOT_connectionError(QAbstractSocket::SocketError soc_error)
{
    qDebug() << soc_error;
    switch (soc_error) {
    case QAbstractSocket::RemoteHostClosedError :
        writeLog("CLOSE BY REMOTE HOST");
        break;
    case QAbstractSocket::NetworkError:
        emit SIGNAL_checkStatus(sslClient::ServerNotFound);
        qDebug() << "network error";
        writeLog("NETWORK ERROR");
        break;
    case QAbstractSocket::HostNotFoundError:
        writeLog("SERVER NOT FOUND");
        qDebug() << "error server not found";
        break;
    case QAbstractSocket::SocketTimeoutError:
        writeLog("TIME OUT TO HANDLE SOCKET");
        qDebug() << "time out to handle socket";
        break;
       if(_sslSocket != NULL)
      _sslSocket->abort();
      mLogon =  false;
    }
}

void sslClient::SLOT_RequestTimeout()

{
     if(mCurrentRequestCMD == CMD_CONNECTHOST)
     {
         mLogon = false;
         _sslSocket->abort();
         delete _sslSocket;
         _sslSocket = NULL;
         emit SIGNAL_checkStatus(sslClient::NoConnection);
     }
     else emit SIGNAL_checkStatus(sslClient::RequestTimeOut);
     writeLog("Request timeout " + mCurrentRequestCMD);
     qDebug() << "Request timeout 412" << mCurrentRequestCMD;
}

void sslClient::SLOT_writeTmpFile()
{
    if(!m_File.isOpen()) {
       if( !m_File.open(QIODevice::WriteOnly | QIODevice::Append)){

           qDebug() << "can not open file" << m_File.fileName();
       }
    }
    if(!isDownloading)isDownloading = true;
    m_File.write(mBufferDataTmp);
    m_File.waitForBytesWritten(10);
    mByteWriteToTmp += mBufferDataTmp.size();
    mBufferDataTmp.clear();
    if(mByteWriteToTmp == mFileSize){
        qDebug() << "emit";
        isDownloading = false;
        isDataTranfer = false;
        emit SIGNAL_checkFile();
    }
}

void sslClient::SLOT_checkFile()
{
    QByteArray byteFrom;
    if(!fileSizeFlag)
    {
        byteFrom.append(mBufferData);
        mBufferData.clear();
    }
    else
    {
         mIsBreak = false;
         if(m_File.exists()){
            if(m_File.isOpen())m_File.close();
            m_File.setFileName(mTmpFileName);
            m_File.open(QIODevice::ReadOnly);
            qDebug() << "read file to calc hash";
            byteFrom = m_File.readAll();
            m_File.close();
         }else qDebug() << "Not found file to calc hash";
    }
    qDebug() << "file size check hash " << byteFrom.size();
    QString hashCalc = QCryptographicHash::hash(byteFrom,QCryptographicHash::Sha1).toHex();
    qDebug() << "hash calc " << hashCalc;
    qDebug() << "hash rec"  << mHashRec;
    if(hashCalc == mHashRec)
    {
        qDebug() << "hash calc match";
        if(fileSizeFlag){

          writeDownloadInfo();
          emit SIGNAL_softwareArrival(mTmpFileName);
        }
        else
        {
            emit SIGNAL_firmwareArrival(byteFrom);
        }
    }
    else
    {
        emit SIGNAL_checkStatus(sslClient::DownloadError);
        QFile::remove(mTmpFileName);
        QFile::remove(mDownloadFilePath);
        qDebug() << "hash calc not match";

    }
    if(mTimeOutDownload.isActive())mTimeOutDownload.stop();
}

void sslClient::SLOT_transferTimeOut()
{
    qDebug() << "check download timeout" << isDownloading <<  isDataTranfer << mLogon;
    if(isDownloading)
    {
        if(!isDataTranfer){
            qDebug() << "session download time out";
            mLogon = false;
            isDownloading = false;
            mIsBreak = true;
            if(m_File.isOpen())m_File.close();
            writeDownloadInfo();
            _sslSocket->abort();
            delete _sslSocket;
            _sslSocket = NULL;
            emit SIGNAL_checkStatus(sslClient::DownloadBreak);
        }
        else isDataTranfer = false;
    }
    else
    {
        if(!mLogon)
        {
            qDebug() << "try relogin";
            emit SIGNAL_emitCheckUpdate();
        }
        else
        {
             if(checkDownloadInfo()) {
                qDebug() << "resuming download";
                mTmpFileName = mBreakFilePath;
                mHashRec = mBreakFileHash;
                mFileSize = mBreakFileSize;
                mByteTotalRec = mCurrentPosBreakInFile;
                mTotalByteEmit = mCurrentPosBreakInFile;
                mByteWriteToTmp = mCurrentPosBreakInFile;
                isDownloading = true;
                SLOT_getSoftWare();
           }
             else qDebug() << "downloadinfo file not exist";
        }

    }
}
