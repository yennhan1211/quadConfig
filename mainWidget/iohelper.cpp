#include "iohelper.h"
#include <QFile>
#include <QButtonGroup>
#include <QSlider>
#include <QCheckBox>
#include <QComboBox>
#include <QLineEdit>

#include <QDebug>

iohelper::iohelper( QObject *parent) :
  QThread(parent)
{
    moveToThread(this);
    qRegisterMetaType<m_hash>("m_hash&");
}

iohelper::~iohelper()
{

}

  void iohelper::SLOT_saveConfigToFile(const QString &filePath, m_hash &widgetHash)
{
    QFile file(filePath);
        if (file.open(QIODevice::WriteOnly)) {

            QByteArray bytes;
            foreach (int key, widgetHash.keys()) {

                char first = (char)((key >> 8) & 0xFF);
                char second = (char)(key & 0xFF);
                bytes.append(first).append(second);
                int value = 0;
                QObject *obj = widgetHash.value(key);
                if(obj != NULL){
                QString strtmp(obj->metaObject()->className());
                if(strtmp == "QButtonGroup"){
                   QButtonGroup *qbg = qobject_cast<QButtonGroup*> (obj);
                   value = qbg->checkedId();
                }
                else if(strtmp == "QSlider"){
                    QSlider *qsld = qobject_cast<QSlider*> (obj);
                    if(qsld->minimum() < 0)
                    value = qsld->value()+100;
                    else value = qsld->value();

                }else if(strtmp == "QCheckBox"){
                    QCheckBox *qcb = qobject_cast<QCheckBox*> (obj);
                    if(qcb->isChecked())value = 1;else value = 0;
                } else if(strtmp == "QComboBox"){
                    QComboBox *qcbb = qobject_cast<QComboBox*> (obj);
                    value = qcbb->currentIndex()+1;
                }
                else if(strtmp == "QLineEdit"){
                    QLineEdit *qled = qobject_cast<QLineEdit*> (obj);
                    value = qled->text().toInt();
                }
              }
                first = (char)((value >> 8) & 0xFF);
                second = (char)(value & 0xFF);
                bytes.append(first).append(second);
    }

            file.write(bytes);
            file.flush();
            file.close();

   }
}

void iohelper::SLOT_writeValueToFc(const QString&,m_hash &widgetHash)
{   qDebug() << "thread write ->" << QThread::currentThreadId();
    int total_bytes =0;

    QHash<int,int> tmp_hash;

    foreach (int key, widgetHash.keys()) {
        int value = 0;
        QObject *obj = widgetHash.value(key);
        if(obj != NULL){
        QString strtmp(obj->metaObject()->className());
        if(strtmp == "QButtonGroup"){
           QButtonGroup *qbg = qobject_cast<QButtonGroup*> (obj);
           value = qbg->checkedId();
           total_bytes += 2;
        }
        else if(strtmp == "QSlider"){

            QSlider *qsld = qobject_cast<QSlider*> (obj);
            if(qsld->minimum() < 0)
            value = qsld->value()+100;
            else value = qsld->value();
             total_bytes += 2;
        }else if(strtmp == "QCheckBox"){
            QCheckBox *qcb = qobject_cast<QCheckBox*> (obj);
            if(qcb->isChecked())value = 1;else value = 0;
             total_bytes += 2;
        } else if(strtmp == "QComboBox"){
            QComboBox *qcbb = qobject_cast<QComboBox*> (obj);
            value = qcbb->currentIndex()+1;
             total_bytes += 2;
        }
        else if(strtmp == "QLineEdit"){
            QLineEdit *qled = qobject_cast<QLineEdit*> (obj);
            value = qled->text().toInt();
            total_bytes += 2;

        }

        tmp_hash.insert(key,value);
        qDebug() << "emit" << key << value;
      }
   }
    int tmp_byte_write = 0;
    foreach (int addr, tmp_hash.keys()) {
         int *tmpint = new int[1];
        tmp_byte_write += 2;
        tmpint[0] = tmp_hash.value(addr);
        int tmp = tmp_byte_write*100 / total_bytes;
        SIGNAL_emitPercentWrite(tmp);
        SIGNAL_emitWrite(tmpint,addr,1);
        qDebug() << "print->" << addr << tmpint[0] ;

        QThread::currentThread()->msleep(100);
    }
    QThread::currentThread()->msleep(100);
    SIGNAL_emitRestartTimer();
}

void iohelper::SLOT_loadConfigFromFile(const QString &filePath, m_hash &widgetHash)
{
       QFile file(filePath);
       int_hash.clear();
        if (file.exists() && file.open(QIODevice::ReadOnly)) {
            char *buffer = new char[4];
            while (!file.atEnd()) {
                qint64 bytesRead = file.read(buffer, 4);
                if (bytesRead == 4){

                    int key = (buffer[0] << 8) | (buffer[1] & 0x00FF);
                    int value = (buffer[2] << 8) | (buffer[3] & 0x00FF);
                    qDebug() << key << value;
                    int_hash.insert(key, value);
                }
            }
            delete[] buffer;
            file.close();
            // update Ui
           foreach (int key, widgetHash.keys()) {
               int *tmpint = new int[1];
               tmpint[0] = int_hash.value(key);
               emit SIGNAL_updateUiFromFile(tmpint,1,key);
           }
        }
}

void iohelper::run()
{
    exec();
}
