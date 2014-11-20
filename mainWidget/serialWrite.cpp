#include <serialWrite.h>
#include <QDataStream>
SerialWrite::SerialWrite(QObject *parent) :
    QThread(parent)
{
    moveToThread(this);
    initDefaultVal();
}
SerialWrite::~SerialWrite()
{

}

void SerialWrite::setPort(QSerialPort *port)
{
    this->comPort = port;
}

void SerialWrite::setMutex(QMutex *mutex)
{
    this->m_PortLock = mutex;
}

void SerialWrite::initDefaultVal()
{

}

void SerialWrite::SLOT_requestData(int addr,int len)
{
    writeByteData(addr,len,_READ);
}

void SerialWrite::SLOT_requestWriteData(int *datawwirte, int addr, int len )
{
    writeByteDataWithCheckSum(datawwirte,len,addr);
}

void SerialWrite::writeByteData(int addr, int numOfField, int rwStt)
{

    QByteArray tmpSend;tmpSend.resize(4);
    tmpSend[0] = ( unsigned char)170;
    tmpSend[1] = ( unsigned char)202;
    tmpSend[2] = ((((rwStt  << 5) | numOfField)  << 2 ) | (addr >> 8));
    tmpSend[3] = (addr & 255);
    m_PortLock->lock();
    //qDebug() << "start write";
    if(addr == 112)qDebug() << "read ->" << (quint8)tmpSend[2] << (quint8)tmpSend[3];
    comPort->write(tmpSend);
    SLEEP(24)
    //qDebug() << "end write";
    m_PortLock->unlock();
}

void SerialWrite::writeByteDataWithCheckSum(int *data, int len, int addr)
{
    if(len == 0 ||  data == NULL)return;
    QByteArray tmpSend;tmpSend.resize(len * 2 + 4);
    unsigned char cksa = 0,cksb = 0;
    tmpSend[0] = ( unsigned char)170;
    tmpSend[1] = ( unsigned char)202;
    tmpSend[2] = ((((1 << 5) | len) << 2) | (addr >> 8));
    tmpSend[3] = (addr & 255);
    for(int i = 0;i < len*2 ;i =i+ 2)
    {
        tmpSend[4+i] = data[i/2] >> 8;
        tmpSend[5+i] = data[i/2] & 255;
    }

    for(int j = 0;j < len* 2 +2 ; j++)
    {
        cksa += tmpSend[2 + j];
        cksb += cksa;
    }

    tmpSend[4+len*2] = cksa;
    tmpSend[5+len*2] = cksb ;
    m_PortLock->lock();
    comPort->write(tmpSend);
    SLEEP(100)
    m_PortLock->unlock();
    qDebug() << "write total byte ->" << len;
    delete[] data;
}

void SerialWrite::run()
{
   try{
        exec();
    }
    catch   (const std::exception)
    {
        qDebug() << "sw exxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
        return ;
    }
}
