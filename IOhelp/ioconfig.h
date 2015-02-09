#ifndef IOCONFIG_H
#define IOCONFIG_H
#include "iohelp_global.h"
#include <QObject>
#include <QSerialPortInfo>
#include <QJsonObject>
#include <QWidget>
class IOHELPSHARED_EXPORT ioConfig : public QObject
{
    Q_OBJECT
public:
    explicit ioConfig(QObject *parent = 0);
    virtual ~ioConfig();
    void setPortInfo(QSerialPortInfo &portinfo);
    void setTxInfo(int);
    void setSkin(int);
    int getSkin();
    int getTxMode();
    void getStartPos(int _x,int _y);
    int  posX();
    int posY();
    void writeConfigToFile(QString const filePath);
    void loadConfigFromFile(QString const filePath);
private:
    QString mComPortName;
    int mTxMode;
    int mSkin;
    int startPosX ,startPosY;

    void writeConfig(QJsonObject  &json)const;
    void readConfig(const QJsonObject &json);
signals:

public slots:

};

#endif // IOCONFIG_H
