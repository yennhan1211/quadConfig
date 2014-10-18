#ifndef IOHELPER_H
#define IOHELPER_H
#include <QObject>
#include <QHash>
#include <QThread>
typedef  QHash<int, QObject *> m_hash;// dang ki kieu du lieu moi
class iohelper : public QThread
{
Q_OBJECT
public:
   explicit iohelper(QObject *parent = 0);
   virtual ~iohelper();

   QHash <int,int> int_hash;
protected:
         void run();
private slots:
         void SLOT_saveConfigToFile(const QString&,m_hash&);
         void SLOT_writeValueToFc(const QString&,m_hash&);
         void SLOT_loadConfigFromFile(const QString&,m_hash&);
signals:
   void SIGNAL_updateUiFromFile(int*,int,int);
   void SIGNAL_emitWrite(int*,int,int);
   void SIGNAL_emitRestartTimer();
   void SIGNAL_emitErrorFile(int);
   void SIGNAL_emitPercentWrite(int);
};

#endif // IOHELPER_H
