#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include "customframe.h"
#include <qdebug.h>
#include "myserialport.h"
#include <QThread>
#include <QTimer>
#include <QHash>
#include <vtekinterface.h>
#include <QPushButton>
#include <QMouseEvent>
#include <QSlider>
#include <QParallelAnimationGroup>
#include <drawimages.h>
#include <lineedittooltip.h>
#include <rxmonitorview.h>
#include <iohelper.h>
#include <vtmessagebox.h>
#include <progressdialog.h>
#include <vtinfoform.h>
#include <counterlabel.h>
namespace Ui {
class mainWidget;
}

class mainWidget : public CustomFrame
{
    Q_OBJECT
public:
    explicit mainWidget(QWidget *parent = 0);
    ~mainWidget();
    void initConnectionsSIGNALtoSLOT();
    void initWidgetToHash();
    void initWidget();
    void initWidgetValue();
    void intDefaultValue();
    void initTimer();
    void loadImage();
    void doWhenConnect();
    void doWhenDisconnect();
    void updateLabelFlashStatus(QString);
    void readAllValue();
    void showTooltips();
    void hideTooltips();
    void initTooltips();
    void initRxview();
    void initSensorview();
protected:
   enum Tabs{
        TabRx = 0,
        TabConfig = 1,
        TabGain = 3,
        TabAuto = 2,
        TabSensor = 4,
        TabGimbal = 5,
        TabFirmware = 6
    };
   bool eventFilter(QObject *obj, QEvent *event);
private:
    Ui::mainWidget *ui;
        RxMonitorView *rxViewLeft;
        RxMonitorView *rxViewRight;
        MySerialPort *m_serialPort;
        QButtonGroup *m_btnRxTypeGroups;
        QButtonGroup *m_btnConfigFrameGroups;
        QButtonGroup *m_btnFaisafeActionGroups;
        QButtonGroup *m_btnQuadSizeGroups;
        QButtonGroup *m_btnSetCenterGroups;
        QButtonGroup *m_btnGimbalOnOffGroups;
        QButtonGroup *m_btnTranmisterModeGroups;
        QHash<int, QObject*> m_WidgetsIdHash;
        QHash<int, QObject*>m_widgettoWrite;
        QHash<int, QObject*> m_RxViewIdHash;
        QTimer m_requestReadTimer;
        QTimer m_timeOutToWrite;
        QPixmap imgConnected,imgDisconnected,
                imgDesQuadX,imgDesQuadPlus,
                imgDesHexX,imgDesHexPlus,
                imgDesHexY,imgDesHexV,
                imgDesOctoX,imgDesOctoPlus,
                imgDesOctoQuad,imgGoHome,
                imgLanding,imgLandAfterHold,
                imgGoHomeAfterHold,imgProtectLv1,
                imgProtectLv2,imgProtectLv3;
        QHash<QObject*,QString>m_Des;
        QHash<QObject*,QPixmap*>m_pixmapHash;
        QHash<QObject*,QLabel*> m_ObjTolabel;
        QHash<int,QString> m_calibAccStatus;
        QHash<int,QString> m_calibMagStatus;
        QObject *m_objTriger;
        QObject *m_objFocus;
        LineEditTooltip *m_BindToolTips;
        LineEditTooltip *m_IntroToolTips;
        vtmessagebox *m_messbox_writetocf;
        DrawImages *m_rollSensor;
        DrawImages *m_yawSensor;
        iohelper *m_IO_config;
        ProgressDialog *m_WriteConfigDialog;
        ProgressDialog *m_waitDialog;
        vtinfoform *m_info;
        counterLabel *m_counterlabel;
        bool m_UpdateUi;
        bool m_connect;
        bool m_BindAnimationFlag;
        bool m_ReadConfigFromFileFlag;
        bool m_Boardresponse;
        int  m_WriteFail;
        int m_counterToWrite,m_addrWrite,m_lenWrite;
        int *m_dataWrite;
        int m_counterToRead;
        int m_dataCounter;
        int lv1,lv2,lv3;
        int line_edit_value_last;
private slots:
        void animationWhenTabChanged(int tabIndex);
        void on_maintabWidget_currentChanged(int index);
public slots:
        void SLOT_updateConnectStatus(bool);
        void SLOT_updateUI(int *data,int,int);
        void SLOT_emitSIGNALtoRead();
        void SLOT_moveImageChecked();
        void SLOT_changeDescriptionWhenEnter(QObject *obj);
        void SLOT_changeDescriptionWhenLeave(QObject *obj);
        void SLOT_moveImageCheckedFromButtonId(int,bool);
        void SLOT_getDataWriteFromButtonGroups(int);
        void SLOT_getDataWriteFromButton();
        void SLOT_getDataWriteFromCombobox(int);
        void SLOT_getDataWriteFromSliderRelease();
        void SLOT_getDataWriteFromSliderTriger();
        void SLOT_getDataWriteFromCheckBox(bool);
        void SLOT_displayLabelFromObj(int);        
        void SLOT_actionTrigerSlider(int);
        void SLOT_stopUpdateUi();
        void SLOT_btnBindAnimation(bool);
        void SLOT_btnFlashUpdate_button_clicked();
        void SLOT_updateFlashPercent(int);
        void SLOT_updateFlashStatus(int);
        void SLOT_changeProtectBackground();
        void SLOT_changeStackedConfig();
        void SLOT_changeStackedRxView();
        void SLOT_rxViewValueChange(int);
        void SLOT_modeSelectChange(int);
        void SLOT_txModeChange(int,bool);
        void SLOT_getDataWritelineEditFinish();
        void SLOT_displayLabelFromRxView(int,int);
        void SLOT_getetObjFocus();
        void SLOT_saveConfigToFile();
        void SLOT_openConfigFile();
        void SLOT_writecftoFc(int);
        void SLOT_emitWriteData(int*,int,int);
        void SLOT_restartTimer();


signals:
        void SIGNAL_requestWitreData(int*,int,int);
        void SIGNAL_requestReadData(int addr,int len);
        void SIGNAL_handleReconnect();
        void SIGNAL_needUpdateFlash(QString);
        void SIGNAL_emitSaveFile(QString,QHash<int, QObject*>);
        void SIGNAL_emitLoadFile(QString,QHash<int, QObject*>);
        void SIGNAL_emitWriteFile(QString,QHash<int, QObject*>);
};

#endif // MAINWIDGET_H
