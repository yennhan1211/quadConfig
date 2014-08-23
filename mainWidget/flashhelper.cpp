#include "flashhelper.h"
#include <addrdefine.h>

#include <QDebug>
#include <QFile>
#include <QTextStream>

FlashHelper::FlashHelper(QSerialPort *port, QObject *parent) :
    QObject(parent)
{
    m_currentFlashPercent = 0;
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

void FlashHelper::emitFlashStatus(int status)
{
    emit _flashStatusChanged(status);
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
