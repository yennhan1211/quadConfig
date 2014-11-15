#include "serialread.h"
#include <QDebug>
#include <addrdefine.h>
#include <QException>
serialRead::serialRead(QObject *parent) :
    QThread(parent)
{
    moveToThread(this);
    initDefaultVal();
}

void serialRead::initDefaultVal()
{
    state = 0;header = 0;id = 0;length = 0;cka = 0;
    ckb = 0;r_cka = 0; r_ckb =0,counter = 0;
    error_count = 0;
}

void serialRead::setPort(QSerialPort *port)
{
    this->comPort = port;
}

void serialRead::setMutex(QMutex *mutex)
{
    this->m_PortLock = mutex;
}

void serialRead::cks(char data)
{
    cka += data;
    ckb += cka;
}

void serialRead::SLOT_readByteFromBuffer()
{
//    try{
        m_PortLock->lock();
        //qDebug() << "start read";
        if(comPort == NULL || !comPort->isOpen())return;     
        qint64 num = comPort->bytesAvailable();
        if(num > 5){
                if(num > 2048)num = 2048;
        char* bufferRead = new char [num];
        if(bufferRead == NULL)return;
       // QByteArray bufferRead = comPort->read(num);
        comPort->read(bufferRead,num);
        //qDebug() << "end read";
        m_PortLock->unlock();
        for (int pos = 0; pos < num; pos++)
        {
            qDebug() << "pos" << pos << num;
            unsigned char data   = (unsigned char)bufferRead[pos];
            switch (state)
            {
                case 0:
                        if (data == ( unsigned char)170) state++;
                        break;
                case 1:
                        if (data == ( unsigned char)202) state++; else state = 0;
                        break;
                case 2:
                        header = data; cks(header); state++; counter = 0; length = (unsigned char)((((header >> 2)  & 31)*2));
                        if(length == 0 ){state = 0;}
                        break;
                case 3:
                        id = data; cks(id); state++; counter = 0; addr = (((header & 3) << 8) | id);
                        break;
                case 4:
                        buffer[counter] = data;
                        cks(data); counter++;
                        if (counter >= length) state++;
                        break;
                case 5:
                        r_cka = data; state++;
                        break;
                case 6:
                        r_ckb = data;
                        if ((cka == r_cka) && (ckb == r_ckb))//203 226 206 240 205 229
                        {
                            int *tmpint = new int[length/2];
                            if(tmpint == NULL)return;
                            int j =0;
                            for (j = 0; j < length; j=j+2)
                            {
                                tmpint[ (j/2) ] = ( unsigned char)buffer[j] << 8   | ( unsigned char) buffer[j + 1];
                            }

                            emit SIGNAL_dataByteUpdate(tmpint,length/2,addr);
                            cka = 0; ckb = 0; counter = 0; state = 0;buffer.clear();
                        }
                        else {
                            error_count++;
                            qDebug() << "error count" << error_count;
                            cka = 0; ckb = 0; counter = 0; state = 0;

                            }
                        break;
           }
        }

        delete[] bufferRead;
      }
        else{
            //qDebug() << "end read";
            m_PortLock->unlock();
           }
//    }
//    catch()
//    {

//    }
}
