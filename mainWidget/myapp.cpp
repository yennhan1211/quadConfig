#include "myapp.h"
#include "QDebug"
#include "QFile"
#include <QDateTime>

bool myApp::notify(QObject *receiver, QEvent *event)
{
        try {
          return QApplication::notify(receiver, event);
        } catch (const std::exception &e) {

             qDebug() << "exxxxxxxxxxxxxxxxx1"<< e.what() << receiver << receiver->objectName() << event->type();
        }
        catch (std::bad_alloc &e)
        {
             qDebug() << "exxxxxxxxxxxxxxxxx2";
        }
        catch (std::bad_exception&)
        {
             qDebug() << "exxxxxxxxxxxxxxxxx3";
        }

        QString strPath = "./crash_log.txt";
        QFile mFile(strPath);
        mFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append);
        QString content ="[" + QDateTime::currentDateTime().toString()+"]"+" "  + receiver->metaObject()->className() +"\r\n";
        mFile.write(content.toUtf8());
        mFile.close();
        return false;
}
