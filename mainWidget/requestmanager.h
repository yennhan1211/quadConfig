#ifndef REQUESTMANAGER_H
#define REQUESTMANAGER_H

#include <QThread>

class requestmanager : public QThread
{
    Q_OBJECT
public:
    explicit requestmanager(QObject *parent = 0);
    void startRequestTimer();
    void stopRequestTimer();

signals:


public slots:
    void SLOT_requestReadData(int,int);
    void SLOT_dowhenConnect();
    void SLOT_dowhenDisconnect();

};

#endif // REQUESTMANAGER_H
