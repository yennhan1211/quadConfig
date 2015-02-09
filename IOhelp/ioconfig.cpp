#include "ioconfig.h"
#include <QFile>
#include <QJsonDocument>
#include <QDebug>
ioConfig::ioConfig(QObject *parent) :
    QObject(parent)
{
    mTxMode = 0;
    mSkin = 0;
    startPosX = 0;
    startPosY = 0;
    mComPortName ="";
}

ioConfig::~ioConfig()
{

}

void ioConfig::setPortInfo(QSerialPortInfo &portinfo)
{
    mComPortName = portinfo.portName();
}

void ioConfig::setTxInfo(int val)
{
    mTxMode = val;
}

void ioConfig::setSkin(int val)
{
    mSkin = val;
}

int ioConfig::getSkin()
{
    return mSkin;
}

int ioConfig::getTxMode()
{
    return mTxMode;
}

void ioConfig::getStartPos(int _x,int _y)
{
    startPosX = _x;
    startPosY = _y;
}

int ioConfig::posX()
{
    return startPosX;
}
int ioConfig::posY()
{
    return startPosY;
}
void ioConfig::writeConfigToFile(const QString filePath)
{
    QFile mFile(filePath);
    if(mFile.open(QIODevice::WriteOnly))
    {
        QJsonObject tmpJson;
        writeConfig(tmpJson);
        QJsonDocument saveDoc(tmpJson);
        mFile.write(saveDoc.toJson());

    }
}

void ioConfig::loadConfigFromFile(const QString filePath)
{
    QFile mFile(filePath);
    if(mFile.open(QIODevice::ReadOnly))
    {
        qDebug() << "open config file ok";
        QByteArray saveDAta = mFile.readAll();
        QJsonDocument loadDoc(QJsonDocument::fromJson(saveDAta));
        readConfig(loadDoc.object());
    }
}

void ioConfig::writeConfig( QJsonObject &json)const
{

    json["PortName"] = mComPortName;
    json["TXMode"] = mTxMode;
    json["Skin"]   = mSkin;
    json["posX"]   = startPosX;
    json["posY"]   = startPosY;
    qDebug() << "write" << mComPortName << mTxMode << mSkin << startPosX << startPosY;
}

void ioConfig::readConfig(const QJsonObject &json)
{
    mComPortName = json["PortName"].toString();
    mTxMode = json["TXMode"].toInt();
    mSkin = json["Skin"].toInt();
    startPosX = json["posX"].toInt();
    startPosY = json["posY"].toInt();
    qDebug() << mComPortName << mTxMode << mSkin << startPosX << startPosY;
}
