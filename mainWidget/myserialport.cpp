#include "myserialport.h"
#include <mainwidget.h>


MySerialPort::MySerialPort(mainWidget *ui,QObject *parent): QThread(parent)
 ,_flash(NULL,this)
{
    moveToThread(this);
    _timerScan.moveToThread(this);
    qRegisterMetaType<QSerialPort::SerialPortError>("QSerialPort::SerialPortError");
    initDefaultVal();

    connect(ui,SIGNAL(SIGNAL_requestReadData(int,int)),&_serialWrite,
            SLOT(SLOT_requestData(int,int)));
    connect(ui,SIGNAL(SIGNAL_requestWitreData(int*,int,int)),&_serialWrite,
            SLOT(SLOT_requestWriteData(int*,int,int)));
    connect(&_serialRead,SIGNAL(SIGNAL_dataByteUpdate(int*,int,int)),ui,
            SLOT(SLOT_updateUI(int*,int,int)));
    connect(&_timerScan,SIGNAL(timeout()),this,SLOT(checkDevice()));
    connect(ui,SIGNAL(SIGNAL_handleReconnect()),this,SLOT(SLOT_HandleReconnect()));
    connect(ui,SIGNAL(SIGNAL_needUpdateFlash(QString)),this,SLOT(SLOT_startUpdateFlash(QString)));
    connect(&_flash,SIGNAL(_flashProgressChanged(int)),ui,SLOT(SLOT_updateFlashPercent(int)));
    connect(&_flash,SIGNAL(_flashStatusChanged(int)),ui,SLOT(SLOT_updateFlashStatus(int)));
    connect(&_flash,SIGNAL(_flashFinish(int)),this,SLOT(SLOT_Flashfinish(int)));
}

MySerialPort::~MySerialPort()
{
    delete myport;
    delete infoPort;
}


bool MySerialPort::setDefaulValue(QSerialPort *port)
{
    if (!port->isOpen()) {
        return false;
        qDebug() << "port is closed";
    }

    if (!port->setDataBits(QSerialPort::Data8)) {

        return false;
         qDebug() << "data bit";
    }

    if (!port->setFlowControl(QSerialPort::NoFlowControl)) {

        return false;
    }

    if (!port->setBaudRate(QSerialPort::Baud115200)) {

        return false;
        qDebug() << "flow";
        qDebug() << "baud";
    }

    if (!port->setStopBits(QSerialPort::OneStop)) {

        return false;
        qDebug() << "stop bit";
    }

    if (!port->setParity(QSerialPort::NoParity)) {

        return false;
        qDebug() << "parity bit";
    }
    return true;
}
void MySerialPort::write4Byte(QByteArray data,char mesClass,char mesID)
{
        char cka = 0,ckb = 0;
        QByteArray tmpArr;
        tmpArr.resize(11);
        tmpArr[0] = 181;
        tmpArr[1] =  98;
        tmpArr[2] = mesClass;
        tmpArr[3] = mesID;
        tmpArr[4] = 4;
        for(int i = 0;i < 4;i++)
        {
            tmpArr[5+i] = data[i];
        }
        for(int i = 0;i<7;i++)
        {
            cka += tmpArr[2+i];
            ckb += cka;
        }
        tmpArr[9]=cka;tmpArr[10]=ckb;
        myport->write(tmpArr);
}

void MySerialPort::initDefaultVal()
{
        state = 0;header = 0;id = 0;length = 0;cka = 0;
        ckb = 0;r_cka = 0; r_ckb =0;connectStatus = false;
        m_flashstatus = 0;
        buffer.resize(255);
}
void MySerialPort::writeByteData(int addr, int numOfField, int rwStt,QSerialPort *port)
{
        QByteArray tmpSend;tmpSend.resize(4);
        tmpSend[0] = ( unsigned char)170;
        tmpSend[1] = ( unsigned char)202;
        tmpSend[2] = ((((rwStt  * 32) | numOfField) * 4) | (addr / 256));
        tmpSend[3] = (addr & 255);
        port->write(tmpSend);
        port->flush();
}

void MySerialPort::readAction(int i)
{
        switch (i) {
        case READ_STRING:
            break;
        case READ_BYTE:
            connect(myport,SIGNAL(readyRead()),&_serialRead,SLOT(SLOT_readByteFromBuffer()), Qt::UniqueConnection);
            connect(myport,SIGNAL(error(QSerialPort::SerialPortError)),this,
                    SLOT(SLOT_handleSerialPortError(QSerialPort::SerialPortError)),
                     (Qt::ConnectionType) (Qt::QueuedConnection | Qt::UniqueConnection));
            break;
        case READ_NONE:
            disconnect(myport,SIGNAL(readyRead()),0,0);
            disconnect(myport,SIGNAL(error(QSerialPort::SerialPortError)),0,0);
            break;
        }
}
void MySerialPort::SLOT_handleSerialPortError(const QSerialPort::SerialPortError& error)
{
        switch (error)
        {
        case QSerialPort::NotOpenError:
        case QSerialPort::DeviceNotFoundError:
        case QSerialPort::PermissionError:
        case QSerialPort::OpenError:
        case QSerialPort::ParityError:
        case QSerialPort::FramingError:
        case QSerialPort::BreakConditionError:
        case QSerialPort::ResourceError:
        case QSerialPort::UnknownError:
            break;

        case QSerialPort::WriteError:
        case QSerialPort::ReadError:
        case QSerialPort::UnsupportedOperationError:
        case QSerialPort::TimeoutError:
        case QSerialPort::NoError:
            return;
        }
        tryToHandleError();
}
void MySerialPort::tryToHandleError()
{
    connectStatus = false;
    emit SIGNAL_connectStatus(false);
    readAction(READ_NONE);
    stopSerialReadWrite();
    _serialWrite.setPort(NULL);
    _serialRead.setPort(NULL);
    if(myport != NULL)
    {
        if(myport->isOpen()){
            myport->close();
        }
        SLEEP(100)
        delete myport;
        myport = NULL;
    }
    if(!_timerScan.isActive())_timerScan.start();
    qDebug() << "try to reconnect";
}

void MySerialPort::startSerialReadWrite()
{
    if(!_serialWrite.isRunning())_serialWrite.start(QThread::NormalPriority);
    if(!_serialRead.isRunning())_serialRead.start(QThread::NormalPriority);
}

void MySerialPort::stopSerialReadWrite()
{
    if(_serialWrite.isRunning())
    {
        QCoreApplication::removePostedEvents(&_serialWrite);
        _serialWrite.quit();
        _serialWrite.wait(1000);
    }
    if(_serialRead.isRunning())
    {
        QCoreApplication::removePostedEvents(&_serialRead);
        _serialRead.quit();
        _serialRead.wait(1000);
    }
}

bool MySerialPort::openPort(QSerialPort *port)
{
    if (port->isOpen()) {
        port->clear(QSerialPort::AllDirections);
        port->close();
        delayms(10);
    }

    if (! port->open(QIODevice::ReadWrite)) {
        qDebug() << port->error() << port->errorString();
        return false;
    }

    if (! setDefaulValue(port)) {
        port->close();
        return false;
         qDebug() << "set port value fail";
    }

    return true;
}

void MySerialPort::clearSIGNAL()
{
    QCoreApplication::removePostedEvents(&_serialWrite);
}
void MySerialPort::checkDevice()// 12c4 ea12
{
  //  qDebug() << "scan my device";
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        if ((info.vendorIdentifier() == QUAD_VENDOR_ID
             && info.productIdentifier() == QUAD_PRODUCT_ID)||(info.vendorIdentifier() == FBL_VENDOR_ID
                                                          && info.productIdentifier() == FBL_PRODUCT_ID))
        {       qDebug() << "this is my device" << info.portName();
                QSerialPort *port = new QSerialPort(info);
                if (openPort(port)) {

                        port->clear(QSerialPort::AllDirections);
                        writeByteData(HARDWARE_ADDR,1,_READ,port);
                        delayms(10)
                        port->waitForReadyRead(50);
                        SLOT_readByteData(port);
                        delayms(50);
                        if(connectStatus)
                        {   _timerScan.stop();
                            myport = port;
                            readAction(READ_BYTE);
                            _serialWrite.setPort(myport);
                            _serialWrite.setMutex(&m_mutex);
                            _serialRead.setPort(myport);
                            _serialRead.setMutex(&m_mutex);
                            qDebug() << "lock thread->" << &m_mutex;
                            startSerialReadWrite();
                            emit SIGNAL_connectStatus(true);
//                             if (m_flashstatus >= 2) {
//                                 qDebug() << "-> jum 10";
//                                 _flash.emitFlashStatus(FlashHelper::FlashOk);
//                                 m_flashstatus = 0;
//                                }
                            break;
                        }
                        else                        
                        {   port->close();
                            delete port;
                            if (m_flashstatus <= 10 && m_flashstatus > 0) {
                                if(m_flashstatus == 1)_flash.emitFlashStatus(FlashHelper::FlashError);
                                m_flashstatus--;
                            }
                        }       

                }else  qDebug() << "can not open";

        }
    }
}

void MySerialPort::SLOT_HandleReconnect()
{
    qDebug() << "handle connect thread id ->" << QThread::currentThreadId();
    tryToHandleError();
}

void MySerialPort::SLOT_startUpdateFlash(const QString filePath)
{
    int tmpFlag = 0;
    if (! connectStatus) { // neu port chua connect, flash cu bi hong, hoac chua connect nhung van nap flash
            if(_timerScan.isActive())_timerScan.stop();
                    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
                    {
                        if ((info.vendorIdentifier() == QUAD_VENDOR_ID && info.productIdentifier() == QUAD_PRODUCT_ID)||
                            (info.vendorIdentifier() == FBL_VENDOR_ID  && info.productIdentifier() == FBL_PRODUCT_ID)) {

                        QSerialPort *port = new QSerialPort(info);
                        if (openPort(port)) {
                            myport = port;
                            connect(myport,SIGNAL(readyRead()),&_flash,SLOT(SLOT_ReadByteFromBuffer()));
                            tmpFlag = 1;// tim thay port va mo duoc
                            break;
                          }
                        else{
                            delete port;
                            }
                        }
                    }
             if(!tmpFlag){
                  if(!_timerScan.isActive())_timerScan.start();
                  _flash.emitFlashStatus(FlashHelper::NotFoundDevice);
                  return;
             }
        }
        else {
            disconnect(myport, SIGNAL(readyRead()), 0, 0);
            connect(myport,SIGNAL(readyRead()),&_flash,SLOT(SLOT_ReadByteFromBuffer()));
        }

        _flash.setFlashFilePath(filePath);
        _flash.setPort(myport);
        _flash.setMutex(&m_mutex);
        _flash.updateFlashNew();
//        m_flashstatus = 10;
//        SLEEP(100)
//        // emit signal start try reconnect
//        _flash.emitFlashStatus(FlashHelper::StartReconnect);
//        // disconnect port va thu connect lai de test firmware moi
//        this->tryToHandleError();
//        qDebug() << "flashstatus ->" << m_flashstatus;
}

void MySerialPort::SLOT_Flashfinish(int status)
{
    if(status == 0 || status == 20){
    this->tryToHandleError();
    }
    else if(status == 10){
        _flash.setPort(NULL);
        _flash.setMutex(NULL);
        readAction(READ_NONE);
        readAction(READ_BYTE);
        emit SIGNAL_connectStatus(true);
    }
}

void MySerialPort::SLOT_eventWmiChange(int wmiStatus)
{
    if(wmiStatus == WMI_STATUS_PLUGGED)
    {
        if(!connectStatus)checkDevice();
    }
    else if(wmiStatus == WMI_STATUS_UNPLUGGED){}
}

void MySerialPort::run()
{
    _timerScan.start(100);
    qDebug() << "serial port thread id ->" << QThread::currentThreadId();
    exec();
}

void MySerialPort::cks(unsigned char data)
{
    cka += data;
    ckb += cka;
}
void MySerialPort::SLOT_readByteData(QSerialPort *port)
{
    try
     {

         char tmpbuffer[2048];
         qint64 num = port->bytesAvailable();
         qDebug() << "byte to read" << num;
         if(num > 0){
         if(num > 2048)num = 2048;
         port->read(tmpbuffer,num);
         QByteArray qbarrTmp(tmpbuffer,num);
         for (int i = 0; i < qbarrTmp.size(); i++)
         {
           unsigned char data   = (unsigned char)qbarrTmp[i];
             switch (state)
             {
                 case 0:
                         if (data == ( unsigned char)170) state++;
                         break;
                 case 1:
                         if (data == ( unsigned char)202) state++; else state = 0;
                         break;
                 case 2:
                         header = data; cks(header); state++; counter = 0; length = (unsigned char)((((header /4)  & 31)*2));
                         break;
                 case 3:
                         id = data; cks(id); state++; counter = 0; addr = (unsigned char)(((header & 3) * 256) | id);
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
                             if(addr == HARDWARE_VALUE)connectStatus = true;else connectStatus = false;
                             cka = 0; ckb = 0; counter = 0; state = 0;
                         }
                         else {
                             cka = 0; ckb = 0; counter = 0; state = 0;qDebug() <<"emit -> error packge" ;
                             }
                         break;
            }
         }
       }
     }
     catch(std::bad_alloc &e)
     {
         qDebug() << "exception ->" << e.what();

     }
}
