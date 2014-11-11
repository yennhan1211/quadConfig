#include "flashhelper.h"
#include <addrdefine.h>

#include <QDebug>
#include <QFile>
#include <QTextStream>

FlashHelper::FlashHelper(QSerialPort *port, QObject *parent) :
    QObject(parent)
{
    m_currentFlashPercent = 0;
    cka = 0;ckb = 0;state =0; mId = 0;tmpData = 0;
    rcka = 0;rckb = 0;
    mFileLen = 0;
    mBuffSize = 0;
    mBuffLen = 0;
    mBuffType = 0;
    responFlag = false;
    timeOutToRespon = 0;
    mTimer.moveToThread(qobject_cast<QThread*>(parent));
    mTimer.setInterval(100);
    connect(&mTimer,SIGNAL(timeout()),this,SLOT(SLOT_timeOut()));
    this->m_port = port;
}

void FlashHelper::setFlashFilePath(const QString &flashFilePath)
{
    this->m_flashFilePath = flashFilePath;
}

void FlashHelper::setPort(QSerialPort *port)
{
    this->m_port = port;
    m_stream.setDevice(m_port);
}

void FlashHelper::setMutex(QMutex *mutex)
{
    this->mPortLock = mutex;
}

void FlashHelper::updateFlash()
{
    QFile file(m_flashFilePath);

    if (file.exists())
    {
        m_stream << "*goto3007\r";
        m_stream.flush();
        int timeout = 0;
        QString tmpStr;
        while (!tmpStr.contains("booted") && timeout < 20) {
            m_stream << "$boot?\r";
            m_stream.flush();
            m_port->waitForReadyRead(50);
            while (!m_stream.atEnd()) {
                tmpStr.append(m_stream.readAll());
                SLEEP(20);
            }
            timeout++;
            qDebug() << "timeout" << timeout;
        }

        if (tmpStr.contains("booted"))
        {
            qDebug() << "booted";
            file.open(QIODevice::ReadWrite);
            QByteArray flashData = file.readAll();
            file.close();

            QByteArray fileLength;
            fileLength.resize(4);
            long len = flashData.length();

            fileLength[0] = (len/4 >> 24);
            fileLength[1] = (len/4 >> 16) & 0xff;
            fileLength[2] = (len/4 >> 8) & 0xff;
            fileLength[3] = (len/4 & 0xff);

            write4Byte(fileLength, 0x0E, 0x0E);

            m_stream << "$erase?\r";
            m_stream.flush();
            emit _flashStatusChanged(StartEraseFlash);
            SLEEP(5000);
            emit _flashStatusChanged(EraseFlashFinished);

            QByteArray bytes;
            bytes.resize(4);
            m_currentFlashPercent = 0;

            for (int i = 0; i < len; i += 4) {
                for (int j = 0; j < 4; j++) {
                    bytes[j] = flashData[i+j];
                }
                write4Byte(bytes, 0x01, 0x02);

                int percent = qRound((double)i / len * 100);
                if (percent - m_currentFlashPercent == 1) {
                    emit _flashProgressChanged(percent);
                    m_currentFlashPercent = percent;
                }
            }
            write4Byte(fileLength, 0xEE, 0xEE);

            m_port->waitForReadyRead(1000);
            tmpStr.clear();
            tmpStr.append(m_stream.readAll());

            if (!tmpStr.contains("flash_ok")) {
                emit _flashStatusChanged(WriteError);
            }
            else {
                qDebug() << "write done";
                emit _flashStatusChanged(WriteDone);
            }
        }
        else emit _flashStatusChanged(CannotJumpToBoot);
    }
    else emit _flashStatusChanged(FileNotExist);
}

void FlashHelper::updateFlashNew()
{
    QFile file(m_flashFilePath);
    if (file.exists())
    {
        file.open(QIODevice::ReadWrite);
        mByteToWrite = file.readAll();
        file.close();
        mFileLen = mByteToWrite.length();
        QByteArray tmpBuffLen;tmpBuffLen.resize(4);
        tmpBuffLen[0] = (char)(mFileLen >> 24);
        tmpBuffLen[1] = (char)((mFileLen >> 16) & 0xff);
        tmpBuffLen[2] = (char)((mFileLen >> 8) & 0xff);
        tmpBuffLen[3] = (char)(mFileLen & 0xff);

        m_port->write("*goto3007\r");
        m_port->waitForBytesWritten(50);
        m_port->write("*boot2\r");
        m_port->waitForBytesWritten(50);
        qDebug() << "erase";
        SLEEP(100)
        writeByte(tmpBuffLen,0x01,0x01);
        mTimer.start();
    }
    else
        emit _flashStatusChanged(FileNotExist);
}

void FlashHelper::SLOT_ReadByteFromBuffer()
{

        mPortLock->lock();
        if(!m_port || !m_port->isOpen())return;
        qint64 mNumOfByte = m_port->bytesAvailable();
        //qDebug() << "jum in SLOT_ReadByteFromBuffer" << mNumOfByte;
        if(mNumOfByte > 4){
            QByteArray mBuff = m_port->read(mNumOfByte);
            mPortLock->unlock();
            for(int pos = 0; pos < mBuff.size();pos++)
            {
                unsigned char mData =(unsigned char)mBuff[pos];

                switch (state) {
                case 0: if (mData == 0xB5) state++; break;
                case 1: if (mData == 0x62) state++; else state = 0; break;
                case 2: mId = mData; cka += mData; ckb += cka; state++; break;
                case 3: tmpData = mData; cka += mData; ckb += cka; state++; break;
                case 4: rcka = mData; state++; break;
                case 5: rckb = mData;                  
                    if (cka == rcka && ckb == rckb)
                    {
                        int iFlashStt = 0;
                        int tmpp = 0;
                        QByteArray tmpBuff;
                        responFlag = true;
                        switch (mId) {
                            case 0x01:
                                switch(tmpData)
                                {
                                case 0x01: iFlashStt = EraseFlashFail;
                                    qDebug() << "erase fail";
                                    break;
                                case 0x02: iFlashStt = EraseFlashFinished ;
                                     qDebug() << "erase ok";
                                    break;
                                case 0x03:
                                    qDebug() << "package" << tmpData;
                                    //emit _flashFinish(0);
                                    break;
                                case 0x04:
                                            m_port->write("*flash\r");
                                            qDebug() << "flash ok";
                                            iFlashStt = FlashOk;
                                            if(mTimer.isActive())mTimer.stop();
                                            emit _flashFinish(10);
                                            break;
                                case 0x05: iFlashStt = JumpFail;break;
                                case 6: iFlashStt = ToltalByteWRong;break;
                                case 7: break;
                                }
                                break;
                        case 0x02:
                                if ((mFileLen - tmpData * mBuffSize) > mBuffSize) mBuffLen = mBuffSize;
                                else mBuffLen = mFileLen - tmpData * mBuffSize;                            
                                if (mBuffLen == mBuffSize)
                                    tmpp = ((int)tmpData+1) * 100 * mBuffSize / mFileLen;
                                else tmpp = (((int)tmpData ) * mBuffSize + mBuffLen) * 100 / mFileLen;
                                 emit _flashProgressChanged(tmpp);
                                 qDebug() << "Percent flash " << tmpp;
                                tmpBuff.resize(mBuffLen);
                                for(int i = 0; i < mBuffLen;i++)
                                {
                                    tmpBuff[i] = mByteToWrite[tmpData*mBuffSize + i];
                                }
                                writeByte(tmpBuff,0x02,tmpData);
                                 break;
                        case 0xaa:
                                if(tmpData == 0x01)mBuffSize = 1024;
                                else if(tmpData == 0x02)mBuffSize = 2048;
                                break;
                        case 0x04:
                                qDebug() << "Current package error at" << tmpData;
                                break;
                        case 0x03:
                                int percent = tmpData * 100 * mBuffSize / mFileLen;
                                emit _flashProgressChanged(percent);
                                qDebug() << "Percent erase " << percent;
                                break;
                        }
                       emit emitFlashStatus(iFlashStt);
                    }
                    state = 0; rcka = 0; rckb = 0; cka = 0; ckb = 0;
                    break;
                }
            }
        }
        else {
            mPortLock->unlock();
        }
}

void FlashHelper::emitFlashStatus(int status)
{
    emit _flashStatusChanged(status);
}

void FlashHelper::SLOT_timeOut()
{
    if(responFlag){
        timeOutToRespon = 0;
        responFlag = false;
    }
    else timeOutToRespon++;
    if(timeOutToRespon >= 20){

        responFlag = false;
        if(mTimer.isActive())mTimer.stop();
        emit _flashFinish(timeOutToRespon);
        timeOutToRespon = 0;
    }
}

void FlashHelper::write4Byte(QByteArray data, char mesClass, char mesID)
{
    char cka = 0, ckb = 0;
    QByteArray tmpArr;
    tmpArr.resize(11);
    tmpArr[0] = 181;
    tmpArr[1] =  98;
    tmpArr[2] = mesClass;
    tmpArr[3] = mesID;
    tmpArr[4] = 4;
    for(int i = 0; i < 4; i++) {
        tmpArr[5+i] = data[i];
    }
    for(int i = 0; i < 7; i++) {
        cka += tmpArr[2+i];
        ckb += cka;
    }
    tmpArr[9] = cka;
    tmpArr[10] = ckb;
    m_port->write(tmpArr);
    m_port->waitForBytesWritten(10);
}

void FlashHelper::writeByte(QByteArray mData, char mClass, char mId)
{

    char cka = 0, ckb = 0;
    QByteArray tmpBuff;tmpBuff.resize(mData.length() + 8);
    tmpBuff[0] = 0xb5;
    tmpBuff[1] = 0x62;
    tmpBuff[2] = mClass; cka += mClass; ckb += cka;
    tmpBuff[3] = mId; cka += mId; ckb += cka;
    tmpBuff[4] = (char)((mData.length() >> 8) & 0xff); cka += tmpBuff[4]; ckb += cka;
    tmpBuff[5] = (char)(mData.length() & 0xff); cka += tmpBuff[5]; ckb += cka;
    for (int i = 0; i < mData.length(); i++)
    {
        tmpBuff[6 + i] = mData[i];
        cka += mData[i];
        ckb += cka;
    }
    tmpBuff[mData.length() + 6] = cka;
    tmpBuff[mData.length() + 7] = ckb;
    if(m_port)
    m_port->write(tmpBuff);
}
