#include "mainwidget.h"
#include "ui_mainwidget.h"
#include <mystring.h>
#include <QFileDialog>
#include <QToolTip>

mainWidget::mainWidget(QWidget *parent) :
    CustomFrame(parent),
    ui(new Ui::mainWidget)
{
    ui->setupUi(this);

    CustomFrame::setWindowSize(1000,600);
    CustomFrame::setWindowTitleSize(1000,30);
    CustomFrame::setBackGroundImage(APP_BACKGROUND);
    CustomFrame::setWindowTitle(APP_TITLE);
    m_serialPort = new MySerialPort(this);

    initRxview();
    initSensorview();
    initTooltips();
    loadImage();
    initWidget();
    initWidgetToHash();   
    initConnectionsSIGNALtoSLOT();
    initWidgetValue();
    intDefaultValue();  
    m_serialPort->start();
    initTimer();

    qDebug() << "main thread" << QThread::currentThreadId();
}

mainWidget::~mainWidget()
{
        m_requestReadTimer.stop();
    if(m_serialPort->isRunning())
    {
        m_serialPort->quit();
        m_serialPort->wait(1000);
        m_serialPort->deleteLater();
    }

    if(m_btnConfigFrameGroups != NULL) m_btnConfigFrameGroups->deleteLater();
    if(m_btnFaisafeActionGroups != NULL)  m_btnFaisafeActionGroups->deleteLater();
    if(m_btnGimbalOnOffGroups != NULL) m_btnGimbalOnOffGroups->deleteLater();
    if(m_btnQuadSizeGroups != NULL)  m_btnQuadSizeGroups->deleteLater();
    if(m_btnRxTypeGroups != NULL) m_btnRxTypeGroups->deleteLater();
    if(m_btnSetCenterGroups != NULL) m_btnSetCenterGroups->deleteLater();
    if(m_btnTranmisterModeGroups != NULL) m_btnTranmisterModeGroups->deleteLater();
    if(rxViewLeft != NULL) rxViewLeft->deleteLater();
    if(rxViewLeft != NULL) rxViewLeft->deleteLater();

    if(m_BindToolTips != NULL) m_BindToolTips->deleteLater();
    if(m_IntroToolTips != NULL)  m_IntroToolTips->deleteLater();
    if(m_messbox_writetocf != NULL) m_messbox_writetocf->deleteLater();
    if(m_IO_config != NULL){
       if(m_IO_config->isRunning()) {m_IO_config->quit();m_IO_config->wait(100);}
        m_IO_config->deleteLater();
    }
    if(m_WriteConfigDialog != NULL)  m_WriteConfigDialog->deleteLater();
    if(m_waitDialog != NULL)  m_waitDialog->deleteLater();
    if(m_info != NULL)  m_info->deleteLater();
    if(m_counterlabel != NULL)  m_counterlabel->deleteLater();

//    if(m_rollSensor != NULL)  m_rollSensor->deleteLater();
//    if(m_pitchSensor != NULL)  m_pitchSensor->deleteLater();
//    if(m_yawSensor != NULL)  m_yawSensor->deleteLater();
//    if(m_objFocus != NULL)  m_objFocus->deleteLater();
//    if(m_objTriger != NULL)  m_objTriger->deleteLater();

    delete ui;
}

void mainWidget::initConnectionsSIGNALtoSLOT()
{
    connect(m_serialPort,SIGNAL(SIGNAL_connectStatus(bool)),
            this,SLOT(SLOT_updateConnectStatus(bool)), Qt::QueuedConnection);

    connect(ui->btnQuadX,SIGNAL(clicked()),this,SLOT(SLOT_moveImageChecked()));
    connect(ui->btnQuadPlus,SIGNAL(clicked()),this,SLOT(SLOT_moveImageChecked()));
    connect(ui->btnHexPlus,SIGNAL(clicked()),this,SLOT(SLOT_moveImageChecked()));
    connect(ui->btnHexX,SIGNAL(clicked()),this,SLOT(SLOT_moveImageChecked()));
    connect(ui->btnHexV,SIGNAL(clicked()),this,SLOT(SLOT_moveImageChecked()));
    connect(ui->btnHexY,SIGNAL(clicked()),this,SLOT(SLOT_moveImageChecked()));
    connect(ui->btnOctoPlus,SIGNAL(clicked()),this,SLOT(SLOT_moveImageChecked()));
    connect(ui->btnOctoQuad,SIGNAL(clicked()),this,SLOT(SLOT_moveImageChecked()));
    connect(ui->btnOctoX,SIGNAL(clicked()),this,SLOT(SLOT_moveImageChecked()));

    connect(ui->sldAileron,SIGNAL(valueChanged(int)),this,SLOT(SLOT_displayLabelFromObj(int)));
    connect(ui->sldElevon,SIGNAL(valueChanged(int)),this,SLOT(SLOT_displayLabelFromObj(int)));
    connect(ui->sldRudder,SIGNAL(valueChanged(int)),this,SLOT(SLOT_displayLabelFromObj(int)));
    connect(ui->sldThrotle,SIGNAL(valueChanged(int)),this,SLOT(SLOT_displayLabelFromObj(int)));

    connect(ui->sldAileron,SIGNAL(valueChanged(int)),this,SLOT(SLOT_rxViewValueChange(int)));
    connect(ui->sldElevon,SIGNAL(valueChanged(int)),this,SLOT(SLOT_rxViewValueChange(int)));
    connect(ui->sldRudder,SIGNAL(valueChanged(int)),this,SLOT(SLOT_rxViewValueChange(int)));
    connect(ui->sldThrotle,SIGNAL(valueChanged(int)),this,SLOT(SLOT_rxViewValueChange(int)));

    connect(ui->sldGear,SIGNAL(valueChanged(int)),this,SLOT(SLOT_displayLabelFromObj(int)));
    connect(ui->sldAux1,SIGNAL(valueChanged(int)),this,SLOT(SLOT_displayLabelFromObj(int)));
    connect(ui->sldGainRoll,SIGNAL(valueChanged(int)),this,SLOT(SLOT_displayLabelFromObj(int)));
    connect(ui->sldGainPitch,SIGNAL(valueChanged(int)),this,SLOT(SLOT_displayLabelFromObj(int)));
    connect(ui->sldGainYaw,SIGNAL(valueChanged(int)),this,SLOT(SLOT_displayLabelFromObj(int)));
    connect(ui->sldGainVer,SIGNAL(valueChanged(int)),this,SLOT(SLOT_displayLabelFromObj(int)));
    connect(ui->sldGainGPS,SIGNAL(valueChanged(int)),this,SLOT(SLOT_displayLabelFromObj(int)));
    connect(ui->sldGainVer,SIGNAL(valueChanged(int)),this,SLOT(SLOT_displayLabelFromObj(int)));
    connect(ui->sldGainGPS,SIGNAL(valueChanged(int)),this,SLOT(SLOT_displayLabelFromObj(int)));
    connect(ui->sldGainGimbalRoll,SIGNAL(valueChanged(int)),this,SLOT(SLOT_displayLabelFromObj(int)));
    connect(ui->sldGainPitchGimbal,SIGNAL(valueChanged(int)),this,SLOT(SLOT_displayLabelFromObj(int)));
    connect(ui->sldAgility,SIGNAL(valueChanged(int)),this,SLOT(SLOT_displayLabelFromObj(int)));
    connect(ui->sldYawRate,SIGNAL(valueChanged(int)),this,SLOT(SLOT_displayLabelFromObj(int)));
    connect(ui->sldTrimGimbalPitch,SIGNAL(valueChanged(int)),this,SLOT(SLOT_displayLabelFromObj(int)));
    connect(ui->sldTrimGimbalRoll,SIGNAL(valueChanged(int)),this,SLOT(SLOT_displayLabelFromObj(int)));

    connect(ui->sldGohmeSpeed,SIGNAL(valueChanged(int)),this,SLOT(SLOT_displayLabelFromObj(int)));

    connect(m_rollSensor,SIGNAL(SIGNAL_rollvalueChange(int)),ui->lbAngleRollVal,SLOT(setNum(int)));
    connect(m_rollSensor,SIGNAL(SIGNAL_pitchvalueChange(int)),ui->lbAnglePitchVal,SLOT(setNum(int)));
    connect(m_yawSensor,SIGNAL(SIGNAL_rollvalueChange(int)),ui->lbAngleYawVal,SLOT(setNum(int)));

    connect(ui->btnCalibAcc,SIGNAL(clicked()),this,SLOT(SLOT_getDataWriteFromButton()));

    connect(ui->sldGainRoll,SIGNAL(sliderReleased()),this,SLOT(SLOT_getDataWriteFromSliderRelease()));
    connect(ui->sldGainRoll,SIGNAL(actionTriggered(int)),this,SLOT(SLOT_actionTrigerSlider(int)));
    connect(ui->sldGainRoll,SIGNAL(valueChanged(int)),this,SLOT(SLOT_getDataWriteFromSliderTriger()));
    connect(ui->sldGainRoll,SIGNAL(sliderPressed()),this,SLOT(SLOT_stopUpdateUi()));

    connect(ui->sldGainPitch,SIGNAL(sliderReleased()),this,SLOT(SLOT_getDataWriteFromSliderRelease()));
    connect(ui->sldGainPitch,SIGNAL(actionTriggered(int)),this,SLOT(SLOT_actionTrigerSlider(int)));
    connect(ui->sldGainPitch,SIGNAL(valueChanged(int)),this,SLOT(SLOT_getDataWriteFromSliderTriger()));
    connect(ui->sldGainPitch,SIGNAL(sliderPressed()),this,SLOT(SLOT_stopUpdateUi()));

    connect(ui->sldGainYaw,SIGNAL(sliderReleased()),this,SLOT(SLOT_getDataWriteFromSliderRelease()));
    connect(ui->sldGainYaw,SIGNAL(actionTriggered(int)),this,SLOT(SLOT_actionTrigerSlider(int)));
    connect(ui->sldGainYaw,SIGNAL(valueChanged(int)),this,SLOT(SLOT_getDataWriteFromSliderTriger()));
    connect(ui->sldGainYaw,SIGNAL(sliderPressed()),this,SLOT(SLOT_stopUpdateUi()));

    connect(ui->sldGainVer,SIGNAL(sliderReleased()),this,SLOT(SLOT_getDataWriteFromSliderRelease()));
    connect(ui->sldGainVer,SIGNAL(actionTriggered(int)),this,SLOT(SLOT_actionTrigerSlider(int)));
    connect(ui->sldGainVer,SIGNAL(valueChanged(int)),this,SLOT(SLOT_getDataWriteFromSliderTriger()));
    connect(ui->sldGainVer,SIGNAL(sliderPressed()),this,SLOT(SLOT_stopUpdateUi()));

    connect(ui->sldGainGPS,SIGNAL(sliderReleased()),this,SLOT(SLOT_getDataWriteFromSliderRelease()));
    connect(ui->sldGainGPS,SIGNAL(actionTriggered(int)),this,SLOT(SLOT_actionTrigerSlider(int)));
    connect(ui->sldGainGPS,SIGNAL(valueChanged(int)),this,SLOT(SLOT_getDataWriteFromSliderTriger()));
    connect(ui->sldGainGPS,SIGNAL(sliderPressed()),this,SLOT(SLOT_stopUpdateUi()));

    connect(ui->sldAgility,SIGNAL(sliderReleased()),this,SLOT(SLOT_getDataWriteFromSliderRelease()));
    connect(ui->sldAgility,SIGNAL(actionTriggered(int)),this,SLOT(SLOT_actionTrigerSlider(int)));
    connect(ui->sldAgility,SIGNAL(valueChanged(int)),this,SLOT(SLOT_getDataWriteFromSliderTriger()));
    connect(ui->sldAgility,SIGNAL(sliderPressed()),this,SLOT(SLOT_stopUpdateUi()));

    connect(ui->sldYawRate,SIGNAL(sliderReleased()),this,SLOT(SLOT_getDataWriteFromSliderRelease()));
    connect(ui->sldYawRate,SIGNAL(actionTriggered(int)),this,SLOT(SLOT_actionTrigerSlider(int)));
    connect(ui->sldYawRate,SIGNAL(valueChanged(int)),this,SLOT(SLOT_getDataWriteFromSliderTriger()));
    connect(ui->sldYawRate,SIGNAL(sliderPressed()),this,SLOT(SLOT_stopUpdateUi()));

    connect(ui->sldGohmeSpeed,SIGNAL(sliderReleased()),this,SLOT(SLOT_getDataWriteFromSliderRelease()));
    connect(ui->sldGohmeSpeed,SIGNAL(actionTriggered(int)),this,SLOT(SLOT_actionTrigerSlider(int)));
    connect(ui->sldGohmeSpeed,SIGNAL(valueChanged(int)),this,SLOT(SLOT_getDataWriteFromSliderTriger()));
    connect(ui->sldGohmeSpeed,SIGNAL(sliderPressed()),this,SLOT(SLOT_stopUpdateUi()));

    connect(ui->sldGainGimbalRoll,SIGNAL(sliderReleased()),this,SLOT(SLOT_getDataWriteFromSliderRelease()));
    connect(ui->sldGainGimbalRoll,SIGNAL(actionTriggered(int)),this,SLOT(SLOT_actionTrigerSlider(int)));
    connect(ui->sldGainGimbalRoll,SIGNAL(valueChanged(int)),this,SLOT(SLOT_getDataWriteFromSliderTriger()));
    connect(ui->sldGainGimbalRoll,SIGNAL(sliderPressed()),this,SLOT(SLOT_stopUpdateUi()));

    connect(ui->sldGainPitchGimbal,SIGNAL(sliderReleased()),this,SLOT(SLOT_getDataWriteFromSliderRelease()));
    connect(ui->sldGainPitchGimbal,SIGNAL(actionTriggered(int)),this,SLOT(SLOT_actionTrigerSlider(int)));
    connect(ui->sldGainPitchGimbal,SIGNAL(valueChanged(int)),this,SLOT(SLOT_getDataWriteFromSliderTriger()));
    connect(ui->sldGainPitchGimbal,SIGNAL(sliderPressed()),this,SLOT(SLOT_stopUpdateUi()));

    connect(ui->sldTrimGimbalPitch,SIGNAL(sliderReleased()),this,SLOT(SLOT_getDataWriteFromSliderRelease()));
    connect(ui->sldTrimGimbalPitch,SIGNAL(actionTriggered(int)),this,SLOT(SLOT_actionTrigerSlider(int)));
    connect(ui->sldTrimGimbalPitch,SIGNAL(valueChanged(int)),this,SLOT(SLOT_getDataWriteFromSliderTriger()));
    connect(ui->sldTrimGimbalPitch,SIGNAL(sliderPressed()),this,SLOT(SLOT_stopUpdateUi()));

    connect(ui->sldTrimGimbalRoll,SIGNAL(sliderReleased()),this,SLOT(SLOT_getDataWriteFromSliderRelease()));
    connect(ui->sldTrimGimbalRoll,SIGNAL(actionTriggered(int)),this,SLOT(SLOT_actionTrigerSlider(int)));
    connect(ui->sldTrimGimbalRoll,SIGNAL(valueChanged(int)),this,SLOT(SLOT_getDataWriteFromSliderTriger()));
    connect(ui->sldTrimGimbalRoll,SIGNAL(sliderPressed()),this,SLOT(SLOT_stopUpdateUi()));


    connect(ui->sldIdlemotorspeed,SIGNAL(sliderReleased()),this,SLOT(SLOT_getDataWriteFromSliderRelease()));
    connect(ui->sldIdlemotorspeed,SIGNAL(actionTriggered(int)),this,SLOT(SLOT_actionTrigerSlider(int)));
    connect(ui->sldIdlemotorspeed,SIGNAL(valueChanged(int)),this,SLOT(SLOT_getDataWriteFromSliderTriger()));
    connect(ui->sldIdlemotorspeed,SIGNAL(sliderPressed()),this,SLOT(SLOT_stopUpdateUi()));


    connect(ui->btnSetdefault,SIGNAL(clicked()),this,SLOT(SLOT_getDataWriteFromButton()));
    connect(ui->btnSetdefault,SIGNAL(pressed()),this,SLOT(SLOT_stopUpdateUi()));

    connect(m_btnConfigFrameGroups,SIGNAL(buttonToggled(int,bool)),this,SLOT(SLOT_moveImageCheckedFromButtonId(int,bool)));
    connect(m_btnRxTypeGroups,SIGNAL(buttonClicked(int)),this,SLOT(SLOT_getDataWriteFromButtonGroups(int)));
    connect(ui->rbDSMx,SIGNAL(toggled(bool)),this,SLOT(SLOT_btnBindAnimation(bool)));
    connect(ui->rbDSM2,SIGNAL(toggled(bool)),this,SLOT(SLOT_btnBindAnimation(bool)));
    connect(ui->rbTradition,SIGNAL(toggled(bool)),this,SLOT(SLOT_btnBindAnimation(bool)));
    connect(ui->rbSbus,SIGNAL(toggled(bool)),this,SLOT(SLOT_btnBindAnimation(bool)));
    connect(ui->rbPPM,SIGNAL(toggled(bool)),this,SLOT(SLOT_btnBindAnimation(bool)));
    connect(ui->btnBind,SIGNAL(clicked()),this,SLOT(SLOT_getDataWriteFromButton()));

    connect(m_btnConfigFrameGroups,SIGNAL(buttonClicked(int)),this,SLOT(SLOT_getDataWriteFromButtonGroups(int)));
    connect(m_btnQuadSizeGroups,SIGNAL(buttonClicked(int)),this,SLOT(SLOT_getDataWriteFromButtonGroups(int)));
    connect(m_btnGimbalOnOffGroups,SIGNAL(buttonClicked(int)),this,SLOT(SLOT_getDataWriteFromButtonGroups(int)));
    connect(m_btnSetCenterGroups,SIGNAL(buttonClicked(int)),this,SLOT(SLOT_getDataWriteFromButtonGroups(int)));
    connect(m_btnFaisafeActionGroups,SIGNAL(buttonClicked(int)),this,SLOT(SLOT_getDataWriteFromButtonGroups(int)));

    connect(ui->cbFlymode1,SIGNAL(activated(int)),this,SLOT(SLOT_getDataWriteFromCombobox(int)));
    connect(ui->cbFlymode2,SIGNAL(activated(int)),this,SLOT(SLOT_getDataWriteFromCombobox(int)));
    connect(ui->cbFlymode3,SIGNAL(activated(int)),this,SLOT(SLOT_getDataWriteFromCombobox(int)));
    connect(ui->cbFlymode4,SIGNAL(activated(int)),this,SLOT(SLOT_getDataWriteFromCombobox(int)));
    connect(ui->cbFlymode5,SIGNAL(activated(int)),this,SLOT(SLOT_getDataWriteFromCombobox(int)));

    connect(ui->btnSelectFile,SIGNAL(clicked()),this,SLOT(SLOT_btnFlashUpdate_button_clicked()));

    connect(ui->chbGimbalPitchRev,SIGNAL(clicked(bool)),this,SLOT(SLOT_getDataWriteFromCheckBox(bool)));
    connect(ui->chbGimbalRollRev,SIGNAL(clicked(bool)),this,SLOT(SLOT_getDataWriteFromCheckBox(bool)));
    connect(ui->chbGimbalTxRev,SIGNAL(clicked(bool)),this,SLOT(SLOT_getDataWriteFromCheckBox(bool)));


    connect(ui->btnVoltAlarm1,SIGNAL(clicked()),this,SLOT(SLOT_changeProtectBackground()));
    connect(ui->btnVoltAlarm2,SIGNAL(clicked()),this,SLOT(SLOT_changeProtectBackground()));
    connect(ui->btnVoltAlarm3,SIGNAL(clicked()),this,SLOT(SLOT_changeProtectBackground()));

    connect(ui->btnStackedWidgetsConfig,SIGNAL(clicked()),this,SLOT(SLOT_changeStackedConfig()));
    connect(ui->btnChangeRXView,SIGNAL(clicked()),this,SLOT(SLOT_changeStackedRxView()));

    connect(ui->sldPitchMode,SIGNAL(valueChanged(int)),this,SLOT(SLOT_modeSelectChange(int)));
    connect(ui->sldPitchMode,SIGNAL(valueChanged(int)),this,SLOT(SLOT_modeSelectChange(int)));

    connect(m_btnTranmisterModeGroups,SIGNAL(buttonToggled(int,bool)),this,SLOT(SLOT_txModeChange(int,bool)));

    connect(rxViewLeft,SIGNAL(_sensorValueChanged(int,int)),this,SLOT(SLOT_displayLabelFromRxView(int,int)));
    connect(rxViewRight,SIGNAL(_sensorValueChanged(int,int)),this,SLOT(SLOT_displayLabelFromRxView(int,int)));

    connect(ui->le_gps_x,SIGNAL(textEdited(QString)),this,SLOT(SLOT_getetObjFocus()));
    connect(ui->le_gps_y,SIGNAL(textEdited(QString)),this,SLOT(SLOT_getetObjFocus()));
    connect(ui->le_gps_z,SIGNAL(textEdited(QString)),this,SLOT(SLOT_getetObjFocus()));
    connect(ui->le_imu_x,SIGNAL(textEdited(QString)),this,SLOT(SLOT_getetObjFocus()));
    connect(ui->le_imu_y,SIGNAL(textEdited(QString)),this,SLOT(SLOT_getetObjFocus()));
    connect(ui->le_imu_z,SIGNAL(textEdited(QString)),this,SLOT(SLOT_getetObjFocus()));
    connect(ui->lePerAlarm,SIGNAL(textEdited(QString)),this,SLOT(SLOT_getetObjFocus()));
    connect(ui->lePerAlarm_2,SIGNAL(textEdited(QString)),this,SLOT(SLOT_getetObjFocus()));
    connect(ui->lePerAlarm_3,SIGNAL(textEdited(QString)),this,SLOT(SLOT_getetObjFocus()));

    connect(ui->le_gps_x,SIGNAL(editingFinished()),this,SLOT(SLOT_getDataWritelineEditFinish()));
    connect(ui->le_gps_y,SIGNAL(editingFinished()),this,SLOT(SLOT_getDataWritelineEditFinish()));
    connect(ui->le_gps_z,SIGNAL(editingFinished()),this,SLOT(SLOT_getDataWritelineEditFinish()));
    connect(ui->le_imu_x,SIGNAL(editingFinished()),this,SLOT(SLOT_getDataWritelineEditFinish()));
    connect(ui->le_imu_y,SIGNAL(editingFinished()),this,SLOT(SLOT_getDataWritelineEditFinish()));
    connect(ui->le_imu_z,SIGNAL(editingFinished()),this,SLOT(SLOT_getDataWritelineEditFinish()));
    connect(ui->lePerAlarm,SIGNAL(editingFinished()),this,SLOT(SLOT_getDataWritelineEditFinish()));
    connect(ui->lePerAlarm_2,SIGNAL(editingFinished()),this,SLOT(SLOT_getDataWritelineEditFinish()));
    connect(ui->lePerAlarm_3,SIGNAL(editingFinished()),this,SLOT(SLOT_getDataWritelineEditFinish()));


    connect(m_IO_config,SIGNAL(SIGNAL_updateUiFromFile(int*,int,int)),this,SLOT(SLOT_updateUI(int*,int,int)));
    connect(ui->btnSave,SIGNAL(clicked()),this,SLOT(SLOT_saveConfigToFile()));
    connect(ui->btnOpen,SIGNAL(clicked()),this,SLOT(SLOT_openConfigFile()));
    connect(m_messbox_writetocf,SIGNAL(SIGNAL_stateChange(int)),this,SLOT(SLOT_writecftoFc(int)));
    connect(m_IO_config,SIGNAL(SIGNAL_emitWrite(int*,int,int)),this,SLOT(SLOT_emitWriteData(int*,int,int)));
    connect(m_IO_config,SIGNAL(SIGNAL_emitRestartTimer()),this,SLOT(SLOT_restartTimer()));
    connect(this,SIGNAL(SIGNAL_emitLoadFile(QString,QHash<int,QObject*>)),m_IO_config
            ,SLOT(SLOT_loadConfigFromFile(QString,QHash<int,QObject*>)));
    connect(this,SIGNAL(SIGNAL_emitSaveFile(QString,QHash<int,QObject*>)),m_IO_config
            ,SLOT(SLOT_saveConfigToFile(QString,QHash<int,QObject*>)));
    connect(this,SIGNAL(SIGNAL_emitWriteFile(QString,QHash<int,QObject*>)),m_IO_config
            ,SLOT(SLOT_writeValueToFc(QString,QHash<int,QObject*>)));
    connect(m_IO_config,SIGNAL(SIGNAL_emitPercentWrite(int)),m_WriteConfigDialog,SLOT(setProgressBarValue(int)));
    connect(ui->btnInfo,SIGNAL(clicked()),m_info,SLOT(SLOT_showForm()));
}

void mainWidget::initWidgetToHash()
{
// Rx tab

    m_WidgetsIdHash.insert(RX_ALE_ADDR, ui->sldAileron);
    m_WidgetsIdHash.insert(RX_ELE_ADDR, ui->sldElevon);
    m_WidgetsIdHash.insert(RX_RUDDER_ADDR, ui->sldRudder);
    m_WidgetsIdHash.insert(RX_THROTLE_ADDR, ui->sldThrotle);

    m_WidgetsIdHash.insert(RX_GEAR_ADDR, ui->sldGear);
    m_WidgetsIdHash.insert(RX_AUX1_ADDR, ui->sldAux1);
    m_WidgetsIdHash.insert(RX_PITCH_ADDR, ui->sldPitchMode);

    m_WidgetsIdHash.insert(RXTYPE_ADDR,m_btnRxTypeGroups);
    m_WidgetsIdHash.insert(TX_MODE_ADDR,m_btnTranmisterModeGroups);
    m_WidgetsIdHash.insert(FRAME_TYPE_ADDR,m_btnConfigFrameGroups);
    m_WidgetsIdHash.insert(FRAME_SIZE_ADDR,m_btnQuadSizeGroups);

    m_WidgetsIdHash.insert(IMU_MOUNT_X,ui->le_imu_x);
    m_WidgetsIdHash.insert(IMU_MOUNT_Y,ui->le_imu_y);
    m_WidgetsIdHash.insert(IMU_MOUNT_Z,ui->le_imu_z);
    m_WidgetsIdHash.insert(GPS_MOUNT_X,ui->le_gps_x);
    m_WidgetsIdHash.insert(GPS_MOUNT_Y,ui->le_gps_y);
    m_WidgetsIdHash.insert(GPS_MOUNT_Z,ui->le_gps_z);


 // motor idle speed
    m_WidgetsIdHash.insert(IDLE_MOTORSPEED_ADDR,ui->sldIdlemotorspeed);



    m_WidgetsIdHash.insert(FLYMODE1_ADDR,ui->cbFlymode1);
    m_WidgetsIdHash.insert(FLYMODE2_ADDR,ui->cbFlymode2);
    m_WidgetsIdHash.insert(FLYMODE3_ADDR,ui->cbFlymode3);
    m_WidgetsIdHash.insert(FLYMODE4_ADDR,ui->cbFlymode4);
    m_WidgetsIdHash.insert(FLYMODE5_ADDR,ui->cbFlymode5);

    m_WidgetsIdHash.insert(RXCENTER_ADDR,m_btnSetCenterGroups);

// gain tab
    m_WidgetsIdHash.insert(GAIN_ROLL_ADDR,ui->sldGainRoll);
    m_WidgetsIdHash.insert(GAIN_PTCH_ADDR,ui->sldGainPitch);
    m_WidgetsIdHash.insert(GAIN_YAW_ADDR,ui->sldGainYaw);
    m_WidgetsIdHash.insert(GAIN_VER_ADDR,ui->sldGainVer);
    m_WidgetsIdHash.insert(GAIN_GPS_ADDR,ui->sldGainGPS);
    m_WidgetsIdHash.insert(AGILITY_ADDR,ui->sldAgility);
    m_WidgetsIdHash.insert(YAW_RATE_ADDR,ui->sldYawRate);

// auto tab
    m_WidgetsIdHash.insert(FAILSAFE_ACTION_ADDR,m_btnFaisafeActionGroups);
    m_WidgetsIdHash.insert(GOHOME_SPEED_ADDR,ui->sldGohmeSpeed);
    m_WidgetsIdHash.insert(VBAT_ADDR,ui->lbVbat);
    m_WidgetsIdHash.insert(NUMBER_OF_CELL_ADDR,ui->lbNumofCell);
    m_WidgetsIdHash.insert(PERCENT_BAT_ADDR,ui->progressPercenPin);
    m_WidgetsIdHash.insert(VALARM_1_ADDR,ui->lePerAlarm);
    m_WidgetsIdHash.insert(VALARM_2_ADDR,ui->lePerAlarm_2);
    m_WidgetsIdHash.insert(VALARM_3_ADDR,ui->lePerAlarm_3);

// sensor tab
    m_WidgetsIdHash.insert(SENSOR_ROLL_ANGLE_ADDR,m_rollSensor);
    m_WidgetsIdHash.insert(SENSOR_PITCH_ANGLE_ADDR,m_rollSensor);
    m_WidgetsIdHash.insert(SENSOR_YAW_ANGLE_ADDR,m_yawSensor);
    m_WidgetsIdHash.insert(CALIB_ACC_STATUS_ADDR,ui->lbCalibAccStt);
    m_WidgetsIdHash.insert(CALIB_MAG_STATUS_ADDR,ui->lbCalibMagStt);

// gimbal tab
    m_WidgetsIdHash.insert(GIMBAL_ONOFF_ADDR,m_btnGimbalOnOffGroups);
    m_WidgetsIdHash.insert(GIMBAL_GAIN_PITCH,ui->sldGainPitchGimbal);
    m_WidgetsIdHash.insert(GIMBAL_GAIN_ROLL,ui->sldGainGimbalRoll);
    m_WidgetsIdHash.insert(GIMBAL_REV_ROLL,ui->chbGimbalRollRev);
    m_WidgetsIdHash.insert(GIMBAL_REV_PITCH,ui->chbGimbalPitchRev);
    m_WidgetsIdHash.insert(GIMBAL_REV_CONTROL_PITCH,ui->chbGimbalTxRev);
    m_WidgetsIdHash.insert(GIMBAL_CENTER_PITCH,ui->sldTrimGimbalPitch);
    m_WidgetsIdHash.insert(GIMBAL_CENTER_ROLL,ui->sldTrimGimbalRoll);

//firmware tab
    m_WidgetsIdHash.insert(HARDWARE_ADDR,ui->lbHWVer);
    m_WidgetsIdHash.insert(SOFTWARE_ADDR,ui->lbSWVer);
    m_WidgetsIdHash.insert(SERIAL_NUM_ADDR,ui->lbSerialNum);
    //mWidgetsIdHash.insert()

    m_ObjTolabel.insert(ui->sldAileron,ui->lbA);
    m_ObjTolabel.insert(ui->sldElevon,ui->lbE);
    m_ObjTolabel.insert(ui->sldThrotle,ui->lbT);
    m_ObjTolabel.insert(ui->sldRudder,ui->lbR);
    m_ObjTolabel.insert(ui->sldGear,ui->lbG);
    m_ObjTolabel.insert(ui->sldAux1,ui->lbAux1);

    m_ObjTolabel.insert(ui->sldGainRoll,ui->lbRollVal);
    m_ObjTolabel.insert(ui->sldGainPitch,ui->lbPitchVal);
    m_ObjTolabel.insert(ui->sldGainYaw,ui->lbYawVal);
    m_ObjTolabel.insert(ui->sldGainVer,ui->lbVerVal);
    m_ObjTolabel.insert(ui->sldGainGPS,ui->lbGPSVal);

    m_ObjTolabel.insert(ui->sldGohmeSpeed,ui->lbGohomeSpeed);

    m_ObjTolabel.insert(ui->sldGainGimbalRoll,ui->lbGimbalRollVal);
    m_ObjTolabel.insert(ui->sldGainPitchGimbal,ui->lbGimbalPitchVal);
    m_ObjTolabel.insert(ui->sldAgility,ui->lbAgilityVal);
    m_ObjTolabel.insert(ui->sldYawRate,ui->lbYawRateVal);

    //m_ObjTolabel.insert(m_pitchSensor,ui->lbAnglePitchVal);
    m_ObjTolabel.insert(m_rollSensor,ui->lbAngleRollVal);
    m_ObjTolabel.insert(m_yawSensor,ui->lbAngleYawVal);

    m_ObjTolabel.insert(ui->sldGainPitchGimbal,ui->lbGimbalPitchVal);
    m_ObjTolabel.insert(ui->sldGainGimbalRoll,ui->lbGimbalRollVal);
    m_ObjTolabel.insert(ui->sldTrimGimbalPitch,ui->lbGimbalPitchTrimVal);
    m_ObjTolabel.insert(ui->sldTrimGimbalRoll,ui->lbGimbalRolTrimVal);

    m_calibAccStatus.insert(0,"");
    m_calibAccStatus.insert(1,IN_CALIB_ACC);
    m_calibAccStatus.insert(2,AFTER_CALIB_UP);
    m_calibAccStatus.insert(3,CALIB_ACC_SUCCESS);
    m_calibAccStatus.insert(9,CALIB_ACC_FAIL);

    m_calibMagStatus.insert(0,"");
    m_calibMagStatus.insert(1,IN_CALIB_MAG);
    m_calibMagStatus.insert(2,CALIB_MAG_SUCCESS);
    m_calibMagStatus.insert(9,CALIB_MAG_FAIL);



    ////////////////
    m_widgettoWrite.insert(GAIN_ROLL_ADDR,ui->sldGainRoll);
    m_widgettoWrite.insert(GAIN_PTCH_ADDR,ui->sldGainPitch);
    m_widgettoWrite.insert(GAIN_YAW_ADDR,ui->sldGainYaw);
    m_widgettoWrite.insert(GAIN_VER_ADDR,ui->sldGainVer);
    m_widgettoWrite.insert(GAIN_GPS_ADDR,ui->sldGainGPS);
    m_widgettoWrite.insert(AGILITY_ADDR,ui->sldAgility);
    m_widgettoWrite.insert(YAW_RATE_ADDR,ui->sldYawRate);

    m_widgettoWrite.insert(RXTYPE_ADDR,m_btnRxTypeGroups);
    m_widgettoWrite.insert(TX_MODE_ADDR,m_btnTranmisterModeGroups);
    m_widgettoWrite.insert(FRAME_TYPE_ADDR,m_btnConfigFrameGroups);
    m_widgettoWrite.insert(FRAME_SIZE_ADDR,m_btnQuadSizeGroups);

    m_widgettoWrite.insert(IDLE_MOTORSPEED_ADDR,ui->sldIdlemotorspeed);

    m_widgettoWrite.insert(IMU_MOUNT_X,ui->le_imu_x);
    m_widgettoWrite.insert(IMU_MOUNT_Y,ui->le_imu_y);
    m_widgettoWrite.insert(IMU_MOUNT_Z,ui->le_imu_z);
    m_widgettoWrite.insert(GPS_MOUNT_X,ui->le_gps_x);
    m_widgettoWrite.insert(GPS_MOUNT_Y,ui->le_gps_y);
    m_widgettoWrite.insert(GPS_MOUNT_Z,ui->le_gps_z);

    m_widgettoWrite.insert(FAILSAFE_ACTION_ADDR,m_btnFaisafeActionGroups);
    m_widgettoWrite.insert(GOHOME_SPEED_ADDR,ui->sldGohmeSpeed);
    m_widgettoWrite.insert(VALARM_1_ADDR,ui->lePerAlarm);
    m_widgettoWrite.insert(VALARM_2_ADDR,ui->lePerAlarm_2);
    m_widgettoWrite.insert(VALARM_3_ADDR,ui->lePerAlarm_3);

    m_widgettoWrite.insert(FLYMODE1_ADDR,ui->cbFlymode1);
    m_widgettoWrite.insert(FLYMODE2_ADDR,ui->cbFlymode2);
    m_widgettoWrite.insert(FLYMODE3_ADDR,ui->cbFlymode3);
    m_widgettoWrite.insert(FLYMODE4_ADDR,ui->cbFlymode4);
    m_widgettoWrite.insert(FLYMODE5_ADDR,ui->cbFlymode5);


    m_widgettoWrite.insert(GIMBAL_ONOFF_ADDR,m_btnGimbalOnOffGroups);
    m_widgettoWrite.insert(GIMBAL_GAIN_PITCH,ui->sldGainPitchGimbal);
    m_widgettoWrite.insert(GIMBAL_GAIN_ROLL,ui->sldGainGimbalRoll);
    m_widgettoWrite.insert(GIMBAL_REV_ROLL,ui->chbGimbalRollRev);
    m_widgettoWrite.insert(GIMBAL_REV_PITCH,ui->chbGimbalPitchRev);
    m_widgettoWrite.insert(GIMBAL_REV_CONTROL_PITCH,ui->chbGimbalTxRev);
    m_widgettoWrite.insert(GIMBAL_CENTER_PITCH,ui->sldTrimGimbalPitch);
    m_widgettoWrite.insert(GIMBAL_CENTER_ROLL,ui->sldTrimGimbalRoll);


}
void mainWidget::initWidget()
{
    ui->maintabWidget->setCurrentIndex(0);

    m_btnRxTypeGroups = new QButtonGroup();
    m_btnRxTypeGroups->setExclusive(true);
    m_btnRxTypeGroups->addButton(ui->rbTradition);
    m_btnRxTypeGroups->addButton(ui->rbPPM);
    m_btnRxTypeGroups->addButton(ui->rbSbus);
    m_btnRxTypeGroups->addButton(ui->rbDSM2);
    m_btnRxTypeGroups->addButton(ui->rbDSMx);
    m_btnRxTypeGroups->addButton(ui->rbXbus);

    m_btnRxTypeGroups->setId(ui->rbTradition,RX_TRADITION_VAL);
    m_btnRxTypeGroups->setId(ui->rbPPM,RX_PPM_VAL);
    m_btnRxTypeGroups->setId(ui->rbSbus,RX_SBUS_VAL);
    m_btnRxTypeGroups->setId(ui->rbDSM2,RX_DSM2_VAL);
    m_btnRxTypeGroups->setId(ui->rbDSMx,RX_DSMX_VAL);
    m_btnRxTypeGroups->setId(ui->rbXbus,RX_JR_XBUS_VAL);

    m_btnConfigFrameGroups = new QButtonGroup();
    m_btnConfigFrameGroups->setExclusive(true);
    m_btnConfigFrameGroups->addButton(ui->btnQuadX);
    m_btnConfigFrameGroups->addButton(ui->btnQuadPlus);
    m_btnConfigFrameGroups->addButton(ui->btnHexX);
    m_btnConfigFrameGroups->addButton(ui->btnHexY);
    m_btnConfigFrameGroups->addButton(ui->btnHexPlus);
    m_btnConfigFrameGroups->addButton(ui->btnHexV);
    m_btnConfigFrameGroups->addButton(ui->btnOctoX);
    m_btnConfigFrameGroups->addButton(ui->btnOctoPlus);
    m_btnConfigFrameGroups->addButton(ui->btnOctoQuad);

    m_btnConfigFrameGroups->setId(ui->btnQuadX,FRAME_TYPE_QUADX);
    m_btnConfigFrameGroups->setId(ui->btnQuadPlus,FRAME_TYPE_QUADPLUS);
    m_btnConfigFrameGroups->setId(ui->btnHexX,FRAME_TYPE_HEXX);
    m_btnConfigFrameGroups->setId(ui->btnHexY,FRAME_TYPE_HEXY);
    m_btnConfigFrameGroups->setId(ui->btnHexV,FRAME_TYPE_HEXV);
    m_btnConfigFrameGroups->setId(ui->btnHexPlus,FRAME_TYPE_HEXPLUS);
    m_btnConfigFrameGroups->setId(ui->btnOctoX,FRAME_TYPE_OCTOX);
    m_btnConfigFrameGroups->setId(ui->btnOctoPlus,FRAME_TYPE_OCTOPLUS);
    m_btnConfigFrameGroups->setId(ui->btnOctoQuad,FRAME_TYPE_OCTOQUAD);

    m_btnQuadSizeGroups = new QButtonGroup();
    m_btnQuadSizeGroups->setExclusive(true);
    m_btnQuadSizeGroups->addButton(ui->rbSmall);
    m_btnQuadSizeGroups->addButton(ui->rbMedium);
    m_btnQuadSizeGroups->addButton(ui->rbLarge);
    m_btnQuadSizeGroups->addButton(ui->rbXLarge);
    m_btnQuadSizeGroups->addButton(ui->rbXXLarge);

    m_btnQuadSizeGroups->setId(ui->rbSmall,FRAME_SIZE_SMALL);
    m_btnQuadSizeGroups->setId(ui->rbMedium,FRAME_SIZE_MEDIUM);
    m_btnQuadSizeGroups->setId(ui->rbLarge,FRAME_SIZE_LARGE);
    m_btnQuadSizeGroups->setId(ui->rbXLarge,FRAME_SIZE_XLARGE);
    m_btnQuadSizeGroups->setId(ui->rbXXLarge,FRAME_SIZE_XXLARGE);

    m_btnFaisafeActionGroups = new QButtonGroup();
    m_btnFaisafeActionGroups->setExclusive(true);
    m_btnFaisafeActionGroups->addButton(ui->rbGohome);
    m_btnFaisafeActionGroups->addButton(ui->rbLanding);
    m_btnFaisafeActionGroups->addButton(ui->rbLandAfterHold);
    m_btnFaisafeActionGroups->addButton(ui->rbGohomeAfterHold);

    m_btnFaisafeActionGroups->setId(ui->rbGohome,FAILSAFE_ACTION_GOHOME);
    m_btnFaisafeActionGroups->setId(ui->rbLanding,FAILSAFE_ACTION_LANDING);
    m_btnFaisafeActionGroups->setId(ui->rbLandAfterHold,FAILSAFE_ACTION_LANDING_HOLD);
    m_btnFaisafeActionGroups->setId(ui->rbGohomeAfterHold,FAILSAFE_ACTION_GOHOME_HOLD);

    m_btnSetCenterGroups = new QButtonGroup();
    m_btnSetCenterGroups->setExclusive(true);
    m_btnSetCenterGroups->addButton(ui->btnSetcenter);
    m_btnSetCenterGroups->setId(ui->btnSetcenter,RXCENTER_VAL);

    m_btnGimbalOnOffGroups = new QButtonGroup();
    m_btnGimbalOnOffGroups->setExclusive(true);
    m_btnGimbalOnOffGroups->addButton(ui->rbONGimbal);
    m_btnGimbalOnOffGroups->addButton(ui->rbOFFGimbal);
    m_btnGimbalOnOffGroups->setId(ui->rbONGimbal,GIMBAL_ON_VAL);
    m_btnGimbalOnOffGroups->setId(ui->rbOFFGimbal,GIMBAL_OFF_VAL);

    m_btnTranmisterModeGroups = new QButtonGroup();
    m_btnTranmisterModeGroups->setExclusive(true);
    m_btnTranmisterModeGroups->addButton(ui->rbMode1);
    m_btnTranmisterModeGroups->addButton(ui->rbMode2);
    m_btnTranmisterModeGroups->addButton(ui->rbMode3);
    m_btnTranmisterModeGroups->addButton(ui->rbMode4);
    m_btnTranmisterModeGroups->setId(ui->rbMode1,TX_MODE_1);
    m_btnTranmisterModeGroups->setId(ui->rbMode2,TX_MODE_2);
    m_btnTranmisterModeGroups->setId(ui->rbMode3,TX_MODE_3);
    m_btnTranmisterModeGroups->setId(ui->rbMode4,TX_MODE_4);

    m_IO_config = new iohelper();
    m_IO_config->start();

    m_WriteConfigDialog = new ProgressDialog(this);
    m_WriteConfigDialog->changeState(1);
    m_WriteConfigDialog->hideDialog();

    m_waitDialog = new ProgressDialog(this);
    m_waitDialog->setContent("Writeing ...");
    m_waitDialog->hideDialog();

    m_info = new vtinfoform(this);

    m_counterlabel = new counterLabel(900,5,ui->tabConfig);
    m_counterlabel->setTotalCounter(3);
}

void mainWidget::initWidgetValue()
{
    for(int i = 0;i < 5;i++)
    {
        QComboBox *qcb = qobject_cast<QComboBox*> ( m_WidgetsIdHash[FLYMODE1_ADDR + i]);
        qcb->addItem(FLYMODE1_TEXT,FLYMODE1_VAL);
        qcb->addItem(FLYMODE2_TEXT,FLYMODE2_VAL);
        qcb->addItem(FLYMODE3_TEXT,FLYMODE3_VAL);
        qcb->addItem(FLYMODE4_TEXT,FLYMODE4_VAL);
        qcb->addItem(FLYMODE5_TEXT,FLYMODE5_VAL);
        qcb->addItem(FLYMODE6_TEXT,FLYMODE6_VAL);
    }
    for(int j = RX_ALE_ADDR ;j <= RX_AUX1_ADDR; j++){

        QSlider *qpb = qobject_cast<QSlider*> (m_WidgetsIdHash[j]);
        qpb->setEnabled(false);
    }
    ui->progressPercenPin->stackUnder(ui->lbAutoBg);
    ui->txtBDesCalib->stackUnder(ui->btnCalibAcc);

    ui->stackedWidgetConfig->setCurrentIndex(0);
    m_btnTranmisterModeGroups->button(TX_MODE_2)->setChecked(true);
    ui->btnVoltAlarm1->click();


    ui->lePerAlarm->setValidator(new QIntValidator(10,70));
    ui->lePerAlarm_2->setValidator(new QIntValidator(10,70));
    ui->lePerAlarm_3->setValidator(new QIntValidator(10,70));
    ui->le_gps_x->setValidator(new QIntValidator(-50,50));
    ui->le_gps_y->setValidator(new QIntValidator(-50,50));
    ui->le_gps_z->setValidator(new QIntValidator(-50,50));
    ui->le_imu_x->setValidator(new QIntValidator(-50,50));
    ui->le_imu_y->setValidator(new QIntValidator(-50,50));
    ui->le_imu_z->setValidator(new QIntValidator(-50,50));
    ui->frameTranmister->hide();
    ui->rbXbus->hide();

    SLOT_txModeChange(2,true);
    ui->stackedWidgetRxView->setCurrentIndex(0);
    SLOT_changeStackedRxView();


}

void mainWidget::intDefaultValue()
{
    m_counterToWrite = 0;
    m_addrWrite = 0;
    m_lenWrite = 0;
    m_UpdateUi = true;
    m_connect = false;
    m_dataCounter = 0;
    m_BindAnimationFlag = true;
    m_ReadConfigFromFileFlag = false;
    m_Boardresponse = true;
    m_WriteFail = 0;
    lv1 = 0;lv2 = 0;lv3 =0;
    line_edit_value_last = 0;

    ui->rbTradition->setChecked(true);
    m_btnConfigFrameGroups->button(FRAME_TYPE_QUADX)->setChecked(true);
    m_btnQuadSizeGroups->button(FRAME_SIZE_SMALL)->setChecked(true);
    m_btnFaisafeActionGroups->button(FAILSAFE_ACTION_GOHOME)->setChecked(true);
    m_btnGimbalOnOffGroups->button(GIMBAL_OFF_VAL)->setChecked(true);
}

void mainWidget::initTimer()
{
    m_requestReadTimer.setInterval(40);
}

void mainWidget::loadImage()
{
   imgConnected = QPixmap("images/connected.png");
   imgDisconnected = QPixmap("images/disconnected.png");
   imgDesQuadX = QPixmap("images/config/quadx200.png");
   imgDesQuadPlus = QPixmap("images/config/quad+200.png");
   imgDesHexX = QPixmap("images/config/hexx200.png");
   imgDesHexPlus = QPixmap("images/config/hex+200.png");
   imgDesHexY = QPixmap("images/config/hexy6200.png");
   imgDesHexV = QPixmap("images/config/hexv6200.png");
   imgDesOctoX = QPixmap("images/config/octox200.png");
   imgDesOctoPlus = QPixmap("images/config/octo+200.png");
   imgDesOctoQuad = QPixmap("images/config/octox8200.png");
   imgGoHome = QPixmap("images/auto/gohome.png");
   imgLanding = QPixmap("images/auto/landing.png");
   imgGoHomeAfterHold = QPixmap("images/auto/holdgohome.png");
   imgLandAfterHold = QPixmap("images/auto/holdlanding.png");
   imgProtectLv1 = QPixmap("images/auto/lv1.png");
   imgProtectLv2 = QPixmap("images/auto/lv2.png");
   imgProtectLv3 = QPixmap("images/auto/lv3.png");

   m_Des.insert(ui->btnQuadX,QUADROTOR_X_DES_URL);
   m_Des.insert(ui->btnQuadPlus,QUADROTOR_PLUS_DES_URL);
   m_Des.insert(ui->btnHexX,HEXAROTOR_X_DES_URL);
   m_Des.insert(ui->btnHexV,HEXAROTOR_V_DES_URL);
   m_Des.insert(ui->btnHexY,HEXAROTOR_Y_DES_URL);
   m_Des.insert(ui->btnHexPlus,HEXAROTOR_PLUS_DES_URL);
   m_Des.insert(ui->btnOctoPlus,OCTOROTOR_PLUS_DES_URL);
   m_Des.insert(ui->btnOctoX,OCTOROTOR_X_DES_URL);
   m_Des.insert(ui->btnOctoQuad,OCTOROTOR_QUAD_DES_URL);

   m_pixmapHash.insert(ui->btnQuadX,&imgDesQuadX);
   m_pixmapHash.insert(ui->btnQuadPlus,&imgDesQuadPlus);
   m_pixmapHash.insert(ui->btnHexX,&imgDesHexX);
   m_pixmapHash.insert(ui->btnHexV,&imgDesHexV);
   m_pixmapHash.insert(ui->btnHexY,&imgDesHexY);
   m_pixmapHash.insert(ui->btnHexPlus,&imgDesHexPlus);
   m_pixmapHash.insert(ui->btnOctoPlus,&imgDesOctoPlus);
   m_pixmapHash.insert(ui->btnOctoX,&imgDesOctoX);
   m_pixmapHash.insert(ui->btnOctoQuad,&imgDesOctoQuad);
   m_pixmapHash.insert(ui->rbGohome,&imgGoHome);
   m_pixmapHash.insert(ui->rbLandAfterHold,&imgLandAfterHold);
   m_pixmapHash.insert(ui->rbGohomeAfterHold,&imgGoHomeAfterHold);
   m_pixmapHash.insert(ui->rbLanding,&imgLanding);
   m_pixmapHash.insert(ui->btnVoltAlarm1,&imgProtectLv1);
   m_pixmapHash.insert(ui->btnVoltAlarm2,&imgProtectLv2);
   m_pixmapHash.insert(ui->btnVoltAlarm3,&imgProtectLv3);
   foreach (QObject* qwg, m_pixmapHash.keys()) {
       QWidget *qpb = qobject_cast<QWidget*> (qwg);
       qpb->installEventFilter(this);
   }
   ui->lbConectionStatus->setPixmap(imgDisconnected);
}

void mainWidget::doWhenConnect()
{
     qDebug() << "do when connect";
    ui->lbConectionStatus->setPixmap(imgConnected);
    connect(&m_requestReadTimer,SIGNAL(timeout()),this,SLOT(SLOT_emitSIGNALtoRead()));

    if(m_ReadConfigFromFileFlag == false){
    readAllValue();
    m_requestReadTimer.start();
    }
    m_connect = true;

}
void mainWidget::doWhenDisconnect()
{
    qDebug() << "do when disconnect";
    ui->lbConectionStatus->setPixmap(imgDisconnected);
    disconnect(&m_requestReadTimer,SIGNAL(timeout()),0,0);
    m_requestReadTimer.stop();
    m_connect = false;

}

void mainWidget::updateLabelFlashStatus(QString str)
{
    ui->lbFWStatus->setText(str);
}



void mainWidget::SLOT_getDataWriteFromButtonGroups(int bttid)
{
    if(m_connect){
        foreach (const int k, m_WidgetsIdHash.keys())
        {
            if (m_WidgetsIdHash[k] == sender()) {
                m_addrWrite = k;
                break;
            }
        }
        m_dataWrite = new int[1];
        m_dataWrite[0] = bttid;
        m_lenWrite = 1;
        m_counterToWrite = TIMEOUT_TO_WRITE;
    }
}

void mainWidget::SLOT_getDataWriteFromButton()
{
    if(m_connect){
    QPushButton *qpb = qobject_cast<QPushButton*> (sender());
    if(qpb == ui->btnSetdefault){
        m_dataWrite = new int[7];
        for(int i =0;i < 7;i++){
            m_dataWrite[i] = 100;
        }
        m_addrWrite = GAIN_ROLL_ADDR;
        m_lenWrite = 7;
        m_counterToWrite = TIMEOUT_TO_WRITE;
        m_waitDialog->showDialog();
      }
    else if(qpb == ui->btnBind)
        {
            m_dataWrite = new int[1];
            if(ui->rbDSM2->isChecked())m_dataWrite[0] = 3;else if(ui->rbDSMx->isChecked())m_dataWrite[0] = 9;
            m_addrWrite = BIND_ADDR;
            m_lenWrite = 1;
            m_counterToWrite = TIMEOUT_TO_WRITE;
            QPoint tmp(-110,ui->btnBind->height()+3);
            m_BindToolTips->move(ui->btnBind->mapTo(this,tmp));
            m_BindToolTips->showMe();
        }
    else if(qpb == ui->btnCalibAcc){
            m_dataWrite = new int[1];
            m_addrWrite = CALIB_ACC_STATUS_ADDR;
            m_dataWrite[0] =1;
            m_lenWrite = 1;
            m_counterToWrite = TIMEOUT_TO_WRITE;
     }
    }
    if(m_UpdateUi == false)m_UpdateUi = true;
}

void mainWidget::SLOT_getDataWriteFromCombobox(int index)
{
        Q_UNUSED(index);
    if(m_connect){
        foreach (const int k, m_WidgetsIdHash.keys())
        {
            if (m_WidgetsIdHash[k] == sender()) {
                m_addrWrite = k;
                break;
            }
        }
        QComboBox *qpb = qobject_cast<QComboBox*> (sender());
        m_dataWrite = new int[1];
        m_dataWrite[0] = qpb->currentIndex()+1;
        m_lenWrite = 1;
        m_counterToWrite = TIMEOUT_TO_WRITE;
    }
        if(m_UpdateUi == false)m_UpdateUi = true;
}

void mainWidget::SLOT_getDataWriteFromSliderRelease()
{
    qDebug() << "slider release";
    if(m_connect){
        foreach (const int k, m_WidgetsIdHash.keys())
        {
            if (m_WidgetsIdHash[k] == sender()) {
                m_addrWrite = k;
                break;
            }
        }
        m_dataWrite = new int[1];
        QSlider *qpb = qobject_cast<QSlider*> (sender());
        int val = qpb->value();
        if (qpb->minimum() < 0) {
            val = val + 100;
        }
        m_dataWrite[0] = val;
        m_lenWrite = 1;
        m_counterToWrite = TIMEOUT_TO_WRITE;
    }
    if(m_UpdateUi == false)m_UpdateUi = true;
}

void mainWidget::SLOT_getDataWriteFromSliderTriger()
{
    if(m_objTriger == sender() && m_connect){
        SLOT_getDataWriteFromSliderRelease();
        qDebug() <<  sender();
        m_objTriger = NULL;
    }
}

void mainWidget::SLOT_getDataWriteFromCheckBox(bool b)
{
    QCheckBox *qpb = qobject_cast<QCheckBox*> (sender());
     if(b)qpb->setText("Rev");else qpb->setText("Norm");
     if(m_connect){
         foreach (const int k, m_WidgetsIdHash.keys())
         {
             if (m_WidgetsIdHash[k] == sender()) {
                 m_addrWrite = k;
                 break;
             }
         }
         m_dataWrite = new int[1];
         if(b)m_dataWrite[0] = 1;else m_dataWrite[0] =0;
         m_lenWrite = 1;
         m_counterToWrite = TIMEOUT_TO_WRITE;
     }
     if(m_UpdateUi == false)m_UpdateUi = true;
}

void mainWidget::SLOT_displayLabelFromObj(int val)
{
     QObject *qpb = qobject_cast<QObject*> (sender());
     if(qpb != NULL){
        if(qpb == ui->sldGohmeSpeed){
            double tmpd = (double)val /10;
            QString tmps = QString::number(tmpd)+" m/s";
            m_ObjTolabel[qpb]->setText(tmps);
        }else if(qpb == ui->sldAileron || qpb == ui->sldElevon || qpb == ui->sldRudder ){
            int tmpd = val - 1520;
            QString tmps = QString::number(tmpd);
            m_ObjTolabel[qpb]->setText(tmps);
        }
        else
        m_ObjTolabel[qpb]->setText(QString::number(val));
       }
}

void mainWidget::SLOT_actionTrigerSlider(int action)
{
    if((action>= 1 && action <= 4 ) || action == 7)
    {
            m_objTriger = sender();

    }
}

void mainWidget::SLOT_stopUpdateUi()
{
    m_serialPort->clearSIGNAL();
    if(m_UpdateUi)m_UpdateUi=false;
}

void mainWidget::SLOT_btnBindAnimation(bool b)
{
    if(b){
        if((sender() == ui->rbTradition || sender() == ui->rbPPM || sender() == ui->rbSbus) && m_BindAnimationFlag == false){
            QPropertyAnimation *anim = new QPropertyAnimation(ui->btnBind, "pos");
            anim->setKeyValueAt(0, QPointF(ui->btnBind->x(), ui->btnBind->y() ));
            anim->setKeyValueAt(1, QPointF(ui->rbDSMx->x()+30, ui->rbDSMx->y() -120));
            anim->setDuration(350);
            anim->start(QAbstractAnimation::DeleteWhenStopped);
            m_BindAnimationFlag = true;

        }
        else if((sender() == ui->rbDSM2 || sender() == ui->rbDSMx) && m_BindAnimationFlag == true){
            QPropertyAnimation *anim = new QPropertyAnimation(ui->btnBind, "pos");
            anim->setKeyValueAt(0, QPointF(ui->btnBind->x(), ui->btnBind->y()));
            anim->setKeyValueAt(1,QPointF(ui->rbDSMx->x()+30, ui->rbDSMx->y() +30));
            anim->setDuration(350);
            anim->start(QAbstractAnimation::DeleteWhenStopped);
            m_BindAnimationFlag = false;
        }
    }
}

void mainWidget::SLOT_btnFlashUpdate_button_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this,
         tr("Open Firmware File"), QCoreApplication::applicationDirPath(),
         tr("Firmware Files (*.vtk)"));

     if (! filePath.isNull() && ! filePath.isEmpty()) {
//         updateLabelFlashStatus(FLASH_START);
//         ui->btns->setEnabled(false);
         if (m_connect) {
             this->doWhenDisconnect();
         }
         emit SIGNAL_needUpdateFlash(filePath);
     }
}

void mainWidget::SLOT_updateFlashPercent(int percent)
{
    ui->prgbFlash->setValue(percent);
}

void mainWidget::SLOT_updateFlashStatus(int status)
{
    switch (status)
        {
        case FlashHelper::PortCannotOpen:
            updateLabelFlashStatus(FLASH_PORT_NOT_OPEN);
            break;
        case FlashHelper::FileNotExist:
            updateLabelFlashStatus(FLASH_FILE_ERROR);
            break;
        case FlashHelper::CannotJumpToBoot:
            updateLabelFlashStatus(FLASH_CANNOT_JUMP);
            break;
        case FlashHelper::StartEraseFlash:
            updateLabelFlashStatus(FLASH_ERASING);
            return;
        case FlashHelper::EraseFlashFinished:
            updateLabelFlashStatus(FLASH_ERASING_FINISHED);
            return;
        case FlashHelper::WriteError:
            updateLabelFlashStatus(FLASH_WRITE_ERROR);
            break;
        case FlashHelper::WriteDone:
            updateLabelFlashStatus(FLASH_WRITE_DONE);
            return;
        case FlashHelper::StartReconnect:
            updateLabelFlashStatus(FLASH_RECONNECT);
            return;
        case FlashHelper::FlashError:
            updateLabelFlashStatus(FLASH_ERROR);
            break;
        case FlashHelper::FlashOk:
            updateLabelFlashStatus(FLASH_OK);
            break;
    }
}

void mainWidget::SLOT_changeProtectBackground()
{
    QPushButton *qpb = qobject_cast<QPushButton*> (sender());
    if(qpb == ui->btnVoltAlarm1){
        ui->lePerAlarm->show();
        ui->lePerAlarm_2->hide();
        ui->lePerAlarm_3->hide();
        ui->lePerAlarm_2->stackUnder(ui->lePerAlarm);
        ui->lePerAlarm_3->stackUnder(ui->lePerAlarm);
    }else if(qpb == ui->btnVoltAlarm2){
        ui->lePerAlarm_2->show();
        ui->lePerAlarm->hide();
        ui->lePerAlarm_3->hide();
        ui->lePerAlarm->stackUnder(ui->lePerAlarm_2);
        ui->lePerAlarm_3->stackUnder(ui->lePerAlarm_2);
    }else if(qpb == ui->btnVoltAlarm3){
        ui->lePerAlarm_2->hide();
        ui->lePerAlarm->hide();
        ui->lePerAlarm_3->show();
        ui->lePerAlarm_2->stackUnder(ui->lePerAlarm_3);
        ui->lePerAlarm->stackUnder(ui->lePerAlarm_3);
    }
    ui->lbprotectbg->setPixmap(*(m_pixmapHash[qpb]));
}

void mainWidget::SLOT_changeStackedConfig()
{
   if(ui->stackedWidgetConfig->currentIndex() ==  ui->stackedWidgetConfig->count()-1){
       ui->stackedWidgetConfig->setCurrentIndex(0);
   }
   else {
       ui->stackedWidgetConfig->setCurrentIndex(ui->stackedWidgetConfig->currentIndex()+1);
    }
   QPropertyAnimation *anim = new QPropertyAnimation(ui->stackedWidgetConfig, "pos");
   anim->setKeyValueAt(0, QPointF(ui->stackedWidgetConfig->x() - 20, ui->stackedWidgetConfig->y()));
   anim->setKeyValueAt(0.8, QPointF(ui->stackedWidgetConfig->x() + 5, ui->stackedWidgetConfig->y()));
   anim->setKeyValueAt(1, QPointF(ui->stackedWidgetConfig->x(), ui->stackedWidgetConfig->y()));
   anim->setDuration(300);
   anim->start(QAbstractAnimation::DeleteWhenStopped);
   qDebug() << ui->stackedWidgetConfig->count();
   m_counterlabel->setCurrentCounter(ui->stackedWidgetConfig->currentIndex()+1);
}

void mainWidget::SLOT_changeStackedRxView()
{

    if(ui->stackedWidgetRxView->currentIndex() == 0) {
        QList<QWidget*> list;
        list.append(ui->frameTranmister);
        list.append(ui->stackedWidgetRxView);
        ui->frameTranmister->show();
        ui->stackedWidgetRxView->setCurrentIndex(1);

        QParallelAnimationGroup *group = new QParallelAnimationGroup;
        foreach (QWidget* widget, list) {

            QPropertyAnimation *anim = new QPropertyAnimation(widget, "pos");
            anim->setKeyValueAt(0, QPointF(widget->x() + 20, widget->y()));
            anim->setKeyValueAt(0.8, QPointF(widget->x() - 5, widget->y()));
            anim->setKeyValueAt(1, QPointF(widget->x(), widget->y()));
            anim->setDuration(300);
            group->addAnimation(anim);
        }
        group->start(QAbstractAnimation::DeleteWhenStopped);
    }
    else {
        ui->frameTranmister->hide();
        ui->stackedWidgetRxView->setCurrentIndex(0);
        QPropertyAnimation *anim = new QPropertyAnimation(ui->stackedWidgetRxView, "pos");
        anim->setKeyValueAt(0, QPointF(ui->stackedWidgetRxView->x() +20, ui->stackedWidgetRxView->y()));
        anim->setKeyValueAt(0.8, QPointF(ui->stackedWidgetRxView->x() - 5, ui->stackedWidgetRxView->y()));
        anim->setKeyValueAt(1, QPointF(ui->stackedWidgetRxView->x(), ui->stackedWidgetRxView->y()));
        anim->setDuration(300);
        anim->start(QAbstractAnimation::DeleteWhenStopped);
    }


}

void mainWidget::SLOT_rxViewValueChange(int value)
{
    int tmp = 0;
    if(m_RxViewIdHash.count() == 0)return;

    foreach (const int k, m_WidgetsIdHash.keys())
    {
        if (m_WidgetsIdHash[k] == sender()) {
            tmp = k;
            break;
        }
    }
    QVariant qvtmp;
    RxMonitorView *qpb = qobject_cast<RxMonitorView*> (m_RxViewIdHash[tmp]);
    short int tmp1 = (short int)tmp;
    short int tmp2  = (short int)value;
    int tmp3 = tmp1 << 16 | tmp2;
    qvtmp = tmp3;
    VtekInterface *inf = VtekInterface::createVtekObject(qpb);
    if (inf != NULL) {
        inf->setValue(qvtmp);
        delete inf;
    }
}

void mainWidget::SLOT_modeSelectChange(int value)
{
    int tmp = 0;

    if(value <1200 ){tmp = ui->cbFlymode1->x()- ui->lbModeSelectBg->x();}
    else if(value >= 1200 && value < 1400){tmp = ui->cbFlymode2->x()- ui->lbModeSelectBg->x();}
    else if(value >= 1400 && value < 1600){tmp = ui->cbFlymode3->x()- ui->lbModeSelectBg->x();}
    else if(value >= 1600 && value < 1800){tmp = ui->cbFlymode4->x()- ui->lbModeSelectBg->x();}
    else if(value >= 1800 && value < 2000){tmp = ui->cbFlymode5->x()- ui->lbModeSelectBg->x();}
    QPropertyAnimation *anim = new QPropertyAnimation(ui->lbModeSelectBg, "pos");
    anim->setKeyValueAt(0, QPointF(ui->lbModeSelectBg->x(), ui->lbModeSelectBg->y()));
    anim->setKeyValueAt(1, QPointF(ui->lbModeSelectBg->x() + tmp, ui->lbModeSelectBg->y()));
    anim->setDuration(250);
    anim->start(QAbstractAnimation::DeleteWhenStopped);

}

void mainWidget::SLOT_txModeChange(int control,bool b)
{
    if(m_RxViewIdHash.count() != 0)m_RxViewIdHash.clear();
    if(!b)return;
    switch (control) {
    case 1:
        rxViewRight->setLocation(RX_LOCATION_RIGHT);
        rxViewLeft->setLocation(RX_LOCATION_LEFT);
        m_RxViewIdHash.insert(RX_ALE_ADDR,rxViewRight );
        m_RxViewIdHash.insert(RX_THROTLE_ADDR, rxViewRight);
        m_RxViewIdHash.insert(RX_RUDDER_ADDR, rxViewLeft);
        m_RxViewIdHash.insert( RX_ELE_ADDR,rxViewLeft );
        break;
    case 2:
        rxViewRight->setLocation(RX_LOCATION_RIGHT);
        rxViewLeft->setLocation(RX_LOCATION_LEFT);
        m_RxViewIdHash.insert(RX_ALE_ADDR,rxViewRight );
        m_RxViewIdHash.insert(RX_ELE_ADDR, rxViewRight);
        m_RxViewIdHash.insert(RX_RUDDER_ADDR, rxViewLeft);
        m_RxViewIdHash.insert(RX_THROTLE_ADDR,rxViewLeft );
        break;
    case 3:
        rxViewRight->setLocation(RX_LOCATION_LEFT);
        rxViewLeft->setLocation(RX_LOCATION_RIGHT);
        m_RxViewIdHash.insert(RX_RUDDER_ADDR,rxViewRight );
        m_RxViewIdHash.insert(RX_THROTLE_ADDR,rxViewRight );
        m_RxViewIdHash.insert(RX_ALE_ADDR, rxViewLeft);
        m_RxViewIdHash.insert(RX_ELE_ADDR,rxViewLeft );
        break;
    case 4:
        rxViewRight->setLocation(RX_LOCATION_LEFT);
        rxViewLeft->setLocation(RX_LOCATION_RIGHT);
        m_RxViewIdHash.insert(RX_RUDDER_ADDR, rxViewRight);
        m_RxViewIdHash.insert(RX_ELE_ADDR, rxViewRight);
        m_RxViewIdHash.insert(RX_ALE_ADDR,rxViewLeft );
        m_RxViewIdHash.insert(RX_THROTLE_ADDR, rxViewLeft);
        break;
    }
    if(m_connect){
    ui->sldAileron->setValue(ui->sldAileron->value()+1);
    ui->sldElevon->setValue(ui->sldElevon->value()+1);
    ui->sldThrotle->setValue(ui->sldThrotle->value()+1);
    ui->sldRudder->setValue(ui->sldRudder->value()+1);
    qDebug() << "jum tx mode";
    }
}

void mainWidget::SLOT_getDataWritelineEditFinish()
{
    if(m_connect){
        QLineEdit *qpb = qobject_cast<QLineEdit*> (sender());
        if(line_edit_value_last != qpb->text().toInt()){
        m_dataWrite = new int[1];
        const QIntValidator *bvt = qobject_cast<const QIntValidator* > (qpb->validator());
        if(bvt->bottom() == -50)
        m_dataWrite[0] =   qpb->text().toInt() + 50;
        else {
            if(qpb == ui->lePerAlarm && qpb->text().toInt() < ui->lePerAlarm_2->text().toInt()){
                m_dataWrite[0] =   ui->lePerAlarm_2->text().toInt() ;
                ui->lePerAlarm ->setText( ui->lePerAlarm_2->text());
                //qDebug() << "-> 1";
            }else if(qpb == ui->lePerAlarm_2 && qpb->text().toInt() < ui->lePerAlarm_3->text().toInt()){
                    m_dataWrite[0] =   ui->lePerAlarm_3->text().toInt() ;
                    ui->lePerAlarm_2 ->setText( ui->lePerAlarm_3->text());
                     //qDebug() << "-> 2";
            }else if(qpb == ui->lePerAlarm_2 && qpb->text().toInt() > ui->lePerAlarm->text().toInt()){
                m_dataWrite[0] =   ui->lePerAlarm->text().toInt() ;
                ui->lePerAlarm_2 ->setText( ui->lePerAlarm->text());
                 //qDebug() << "-> 3";
            }else if(qpb == ui->lePerAlarm_3 && qpb->text().toInt() > ui->lePerAlarm_2->text().toInt()){
                m_dataWrite[0] =   ui->lePerAlarm_2->text().toInt() ;
                ui->lePerAlarm_3 ->setText( ui->lePerAlarm_2->text());
                // qDebug() << "-> 4";
                    }
            else  m_dataWrite[0] =   qpb->text().toInt() ;
         }
        qDebug() << "start write"<<m_dataWrite[0] << m_addrWrite << line_edit_value_last <<
                    qpb->text().toInt();

        line_edit_value_last = qpb->text().toInt();
        m_lenWrite = 1;
        foreach (const int k, m_WidgetsIdHash.keys())
        {
            if (m_WidgetsIdHash[k] == sender()) {
                m_addrWrite = k;
                break;
            }
        }

        m_counterToWrite = TIMEOUT_TO_WRITE;

        }
    }
    if(m_UpdateUi == false)m_UpdateUi = true;
    if(m_objFocus ==  sender()){m_objFocus = NULL;hideTooltips();}
}
void mainWidget::SLOT_displayLabelFromRxView(int x, int y)
{
    int _x = x - 1520;int _y = y - 1520;

    if(sender() == rxViewLeft){
        if(m_btnTranmisterModeGroups->checkedId() == 2 || m_btnTranmisterModeGroups->checkedId() == 4)_y = y;
        ui->lbLeftX->setText(QString::number(_x));
        ui->lbLeftY->setText(QString::number(_y));
    }
    else if(sender() == rxViewRight){
        if(m_btnTranmisterModeGroups->checkedId() == 1 || m_btnTranmisterModeGroups->checkedId() == 3)_y = y;
        ui->lbRightX->setText(QString::number(_x));
        ui->lbRightY->setText(QString::number(_y));
    }
}

void mainWidget::SLOT_getetObjFocus()
{
    if(sender() != m_objFocus){
    showTooltips();
    m_objFocus = sender();
    line_edit_value_last = 0;
    qDebug() << m_objFocus ;
    }
}

void mainWidget::SLOT_saveConfigToFile()
{
   if(m_connect){
    QString filePath = QFileDialog::getSaveFileName(this,
                      tr("Save config File"), QCoreApplication::applicationDirPath(),
                      tr("Config Files (*.aplo)"));
    if(filePath.length() > 0)
        emit SIGNAL_emitSaveFile(filePath,m_widgettoWrite);
   }
}

void mainWidget::SLOT_openConfigFile()
{
    QString filePath = QFileDialog::getOpenFileName(this,
                      tr("Open config File"), QCoreApplication::applicationDirPath(),
                      tr("Config Files (*.aplo)"));

     if(filePath.length() > 0){

         if(m_connect && m_ReadConfigFromFileFlag == false){
            if(m_requestReadTimer.isActive())m_requestReadTimer.stop();
            m_serialPort->clearSIGNAL();
         }
    emit SIGNAL_emitLoadFile(filePath,m_widgettoWrite);   
    m_messbox_writetocf->setMessage("Write all config to FC ?");
    m_messbox_writetocf->showMe();
    m_ReadConfigFromFileFlag = true;
     }
}

void mainWidget::SLOT_writecftoFc(int st)
{
    if(st == 0){
        m_messbox_writetocf->hideMe();
        SLOT_restartTimer();
    }
    else if(st == 1){

        if(m_connect){
        m_WriteConfigDialog->showDialog();
        m_messbox_writetocf->hideMe(); 
        emit SIGNAL_emitWriteFile("",m_widgettoWrite);
        }
    }
    m_ReadConfigFromFileFlag = false;
}

void mainWidget::SLOT_emitWriteData(int *l_dataWrite, int l_addrWrite, int l_lenWrite)
{
    qDebug() << "->" << l_dataWrite[0] << l_addrWrite;
     emit SIGNAL_requestWitreData(l_dataWrite,l_addrWrite,l_lenWrite);
}

void mainWidget::SLOT_restartTimer()
{
    readAllValue();
    if(!m_requestReadTimer.isActive() && m_connect )m_requestReadTimer.start();
    m_WriteConfigDialog->hideDialog();
}

bool mainWidget::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type() == QEvent::HoverEnter)
    {   
         QString strtmp(obj->metaObject()->className());
         if(strtmp =="QPushButton" || strtmp =="QRadioButton" ){
            SLOT_changeDescriptionWhenEnter(obj);
         }
    }
    else if(event->type() == QEvent::HoverLeave)
    {
        QString strtmp(obj->metaObject()->className());
        if(strtmp =="QPushButton" || strtmp =="QRadioButton" ){
           SLOT_changeDescriptionWhenLeave(obj);
        }
    }
    return false;
}
void mainWidget::SLOT_moveImageChecked()
{
    QPushButton *qpb = qobject_cast<QPushButton*> (sender());
    if (qpb != NULL) {
        QPoint point(qpb->pos().x() ,
                     qpb->pos().y());
        ui->lbImageChecked->move(point);
        ui->lbImageChecked->stackUnder(qpb);
        ui->lbImageChecked->show();
    }
}

void mainWidget::SLOT_moveImageCheckedFromButtonId(int bttId, bool bttCheck)
{
    if(bttCheck)
    {
        QAbstractButton  *qpb = m_btnConfigFrameGroups->button(bttId);
        QPoint point(qpb->pos().x() ,
                     qpb->pos().y());
        ui->lbImageChecked->move(point);
        ui->lbImageChecked->stackUnder(qpb);
        ui->lbImageChecked->show();
    }
}

void mainWidget::SLOT_changeDescriptionWhenEnter(QObject *obj)
{
    QString strtmp(obj->metaObject()->className());
    if(strtmp =="QPushButton" ){
       ui->lbDescep->setPixmap(*(m_pixmapHash[obj]));
       ui->txtFrameDes->setSource(QUrl(m_Des[obj]));
    }
    else if(strtmp =="QRadioButton")ui->lbFailsafeDes->setPixmap(*(m_pixmapHash[obj]));

}

void mainWidget::SLOT_changeDescriptionWhenLeave(QObject *obj)
{
    QString strtmp(obj->metaObject()->className());
    if(strtmp =="QPushButton" ){
       ui->lbDescep->setPixmap(*(m_pixmapHash[m_btnConfigFrameGroups->checkedButton()]));
       ui->txtFrameDes->setSource(QUrl(m_Des[m_btnConfigFrameGroups->checkedButton()]));
    }
    else if(strtmp =="QRadioButton")
        ui->lbFailsafeDes->setPixmap(*(m_pixmapHash[m_btnFaisafeActionGroups->checkedButton()]));
}

void mainWidget::animationWhenTabChanged(int tabIndex)
{
    QList<QWidget*> list;
    switch(tabIndex)
    {
    case TabRx: {
        list.append(ui->rbTradition);
        list.append(ui->rbPPM);
        list.append(ui->rbSbus);
        list.append(ui->rbDSM2);
        list.append(ui->rbDSMx);
        list.append(ui->stackedWidgetRxView);
        list.append(ui->btnChangeRXView);
        list.append(ui->sldAux1);
        list.append(ui->sldPitchMode);
        list.append(ui->sldGear);
        list.append(ui->lbAux1);
        list.append(ui->lbG);
        list.append(ui->lb_g);
        list.append(ui->lb_x);
        list.append(ui->lb_p);
        list.append(ui->lbmonitor);
        list.append(ui->lbrxtype);
        list.append(ui->lbFlightmode);
        list.append(ui->cbFlymode1);
        list.append(ui->cbFlymode2);
        list.append(ui->cbFlymode3);
        list.append(ui->cbFlymode4);
        list.append(ui->cbFlymode5);
        list.append(ui->btnSetcenter);
        list.append(ui->btnBind);
        list.append(ui->rbXbus);
        list.append(ui->frameTranmister);
        list.append(ui->lbModeSelectBg);
        list.append(ui->lbA_11);
        list.append(ui->lbA_12);
        break;
    }
    case TabConfig: {
        list.append(ui->lbDesBackGround);
        list.append(ui->lbDescep);
        list.append(ui->btnStackedWidgetsConfig);
        list.append(ui->stackedWidgetConfig);
        list.append(ui->txtFrameDes);
        break;
    }
    case TabGain: {
        list.append(ui->stackedWidgetGainTab);
        break;
    }
    case TabAuto: {
        list.append(ui->lbGohomeSpeed);
        list.append(ui->lbAutoBg);
        list.append(ui->rbGohome);
        list.append(ui->rbGohomeAfterHold);
        list.append(ui->rbLandAfterHold);
        list.append(ui->rbLanding);
        list.append(ui->sldGohmeSpeed);
        list.append(ui->lbFailsafeDes);
        list.append(ui->progressPercenPin);
        list.append(ui->lbVbat);
        list.append(ui->btnVoltAlarm1);
        list.append(ui->btnVoltAlarm2);
        list.append(ui->btnVoltAlarm3);
        list.append(ui->lbNumofCell);
        list.append(ui->lbprotectbg);
        list.append(ui->lePerAlarm);
        list.append(ui->lePerAlarm_2);
        list.append(ui->lePerAlarm_3);
        list.append(ui->lbA_per);
        break;
    }
    case TabSensor:
        list.append(ui->btnCalibAcc);
        list.append(ui->lbAnglePitchVal);
        list.append(ui->lbAngleRollVal);
        list.append(ui->lbAngleYawVal);
        list.append(ui->lbCalibAccStt);
        list.append(ui->lbCalibMagStt);
        list.append(ui->lbRollSensorBg);
        list.append(ui->lbYawSensorBg);
        list.append(ui->txtBDesCalib);
        list.append(m_rollSensor);
        list.append(m_yawSensor);
        break;
    case TabGimbal: {
        list.append(ui->chbGimbalPitchRev);
        list.append(ui->chbGimbalRollRev);
        list.append(ui->chbGimbalTxRev);
        list.append(ui->lbGimbalBg);
        list.append(ui->lbGimbalPitchVal);
        list.append(ui->lbGimbalRoll);
        list.append(ui->lbGimbalPitch);
        list.append(ui->lbGimbalRollVal);
        list.append(ui->lbGimbalPitch_2);
        list.append(ui->lbGimbalRoll_2);
        list.append(ui->lbGimbalPitchTrimVal);
        list.append(ui->lbGimbalRolTrimVal);
        list.append(ui->rbOFFGimbal);
        list.append(ui->rbONGimbal);
        list.append(ui->sldGainGimbalRoll);
        list.append(ui->sldGainPitchGimbal);
        list.append(ui->sldTrimGimbalPitch);
        list.append(ui->sldTrimGimbalRoll);
        break;
    }
    case TabFirmware: {
        list.append(ui->btnSelectFile);
        list.append(ui->btnUpdate);
        list.append(ui->lbFWbg);
        list.append(ui->lbFWStatus);
        list.append(ui->lbSerialNum);
        list.append(ui->lbSWVer);
        list.append(ui->lbHWVer);
        list.append(ui->lbLastTestFW);
        list.append(ui->prgbFlash);
        break;
      }
    }

    QParallelAnimationGroup *group = new QParallelAnimationGroup;
    foreach (QWidget* widget, list) {

        QPropertyAnimation *anim = new QPropertyAnimation(widget, "pos");
        anim->setKeyValueAt(0, QPointF(widget->x() + 20, widget->y()));
        anim->setKeyValueAt(0.8, QPointF(widget->x() - 5, widget->y()));
        anim->setKeyValueAt(1, QPointF(widget->x(), widget->y()));
        anim->setDuration(300);
        group->addAnimation(anim);
    }
    group->start(QAbstractAnimation::DeleteWhenStopped);
}

void mainWidget::on_maintabWidget_currentChanged(int index)
{
     m_serialPort->clearSIGNAL();
    switch (index) {
    case TabConfig:
        SLOT_changeDescriptionWhenLeave(ui->btnQuadX);
        break;
    case TabAuto:
        SLOT_changeDescriptionWhenLeave(ui->rbLanding);
        break;
    }
    animationWhenTabChanged(index);

}

void mainWidget::SLOT_updateConnectStatus(bool status)
{
    if(status)doWhenConnect();
    else doWhenDisconnect();
}

void mainWidget::SLOT_updateUI(int *data,int len,int addr)
{
    m_dataCounter = 0;

    if(m_UpdateUi){

        for(int i =0;i < len ;i ++)
        {
          QObject *obj = m_WidgetsIdHash[addr + i];

          if (obj != NULL && obj != m_objFocus){
          QVariant qvtmp;
          if(obj == ui->lbCalibAccStt){qvtmp = m_calibAccStatus[data[i]];}
          else if(obj == ui->lbCalibMagStt)qvtmp = m_calibMagStatus[data[i]];
          else if(obj == ui->lbVbat){
              double tmpd = (double)data[i]/10;
              QString tmps = QString::number(tmpd)+" v";
              qvtmp = tmps;
          }
          else if(obj == ui->lbNumofCell){
              QString tmps = QString::number(data[i])+" Cells";
              qvtmp = tmps;
          }
          else if(obj == ui->lbHWVer || obj == ui->lbSWVer){
              if(data[i] > 255){data[i] &= 255;data[i] *= 10;}
              QString tmps1 = QString::number(data[i]/100)+"."+QString::number((data[i]/10)%10)+"."+QString::number(data[i]%10);
              qvtmp = tmps1;
          }
          else if((addr+i) == 501 || (addr+i) == 502 || (addr+i) == 503){
              short int tmp1 = (short int)(addr+i);
              short int tmp2  = (short int)data[i];
              if(tmp2 < 0)tmp2 = 360 + tmp2;
              int tmp3 = tmp1 << 16 | tmp2;
              qvtmp = tmp3;
          }
          else qvtmp = data[i];
          VtekInterface *inf = VtekInterface::createVtekObject(obj);
          if (inf != NULL) {
              inf->setValue(qvtmp);
              delete inf;
            }
          }

       }
         m_waitDialog->hideDialog();
    }
  if(data != NULL)  delete[] data;
}

void mainWidget::SLOT_emitSIGNALtoRead()
{
    if(m_counterToWrite == 0){

        if(m_dataCounter  < 10)
        {
        qDebug() << "response";
        } else {
             emit SIGNAL_handleReconnect();m_dataCounter = 0;
             qDebug() << " no response";
             return;
        }
        switch (ui->maintabWidget->currentIndex()) {
        case TabRx:
            if(m_counterToRead == 0){emit SIGNAL_requestReadData(FLYMODE1_ADDR,6);m_dataCounter+=5;}
            else emit SIGNAL_requestReadData(RX_ALE_ADDR,7);
            break;
        case TabConfig:
            if(m_counterToRead == 0){emit SIGNAL_requestReadData(FRAME_TYPE_ADDR,9);m_dataCounter +=5;
            }
            break;
        case TabGain:
             if(m_counterToRead == 0){ emit SIGNAL_requestReadData(GAIN_ROLL_ADDR,7);m_dataCounter +=5;
             }
            break;
        case TabAuto:
             if(m_counterToRead == 0){
                 emit SIGNAL_requestReadData(GOHOME_SPEED_ADDR,3);
                 SLEEP(25)
                 emit SIGNAL_requestReadData(VALARM_1_ADDR,3);
                 m_dataCounter +=5;

             }else emit SIGNAL_requestReadData(VBAT_ADDR,4);
            break;
        case TabSensor:
             if(m_counterToRead == 0){ emit SIGNAL_requestReadData(CALIB_ACC_STATUS_ADDR,2);m_dataCounter+=5;}
           else  emit SIGNAL_requestReadData(SENSOR_ROLL_ANGLE_ADDR,3);
        case TabGimbal:
             if(m_counterToRead == 0){ emit SIGNAL_requestReadData(GIMBAL_ONOFF_ADDR,9);m_dataCounter+=5;}
            break;
        case TabFirmware:
             if(m_counterToRead == 0){ emit SIGNAL_requestReadData(HARDWARE_ADDR,3);m_dataCounter+=5;
             }
            break;
            }

        if(++m_counterToRead >= 20)m_counterToRead =0;
        //if(m_dataCounter >= 20){emit SIGNAL_handleReconnect();m_dataCounter = 0;}

    }
    else if(m_counterToWrite == 10)
            {
              m_serialPort->clearSIGNAL();

              emit SIGNAL_requestWitreData(m_dataWrite,m_addrWrite,m_lenWrite);
            }
    if(m_counterToWrite > 0) m_counterToWrite--;
}
void mainWidget::readAllValue()
{
    emit SIGNAL_requestReadData(FLYMODE1_ADDR,6);
    SLEEP(40)
    emit SIGNAL_requestReadData(RX_ALE_ADDR,7);
    SLEEP(40)
    emit SIGNAL_requestReadData(FRAME_TYPE_ADDR,9);
    SLEEP(40)
    emit SIGNAL_requestReadData(GAIN_ROLL_ADDR,7);
    SLEEP(40)
    emit SIGNAL_requestReadData(GOHOME_SPEED_ADDR,3);
    SLEEP(40)
    emit SIGNAL_requestReadData(VALARM_1_ADDR,3);
    SLEEP(40)
    emit SIGNAL_requestReadData(VBAT_ADDR,4);
    SLEEP(40)
    emit SIGNAL_requestReadData(CALIB_ACC_STATUS_ADDR,2);
    SLEEP(40)
    emit SIGNAL_requestReadData(SENSOR_ROLL_ANGLE_ADDR,3);
    SLEEP(40)
    emit SIGNAL_requestReadData(GIMBAL_ONOFF_ADDR,9);
    SLEEP(40)
    emit SIGNAL_requestReadData(HARDWARE_ADDR,3);
    SLEEP(40)
}

void mainWidget::showTooltips()
{
    QLineEdit *qle = qobject_cast<QLineEdit*> (sender());

    QPoint tmp(-130,qle->height()+3);

    m_IntroToolTips->move(qle->mapTo(this, tmp));
    m_IntroToolTips->showMe();
}

void mainWidget::hideTooltips()
{
    m_IntroToolTips->hideMe();
}

void mainWidget::initTooltips()
{
    m_BindToolTips = new LineEditTooltip(this);
    m_BindToolTips->setText(BIND_MESS);
    m_BindToolTips->setFixedSize(160,45);
    QPoint tmp(-110,ui->btnBind->height()+3);
    m_BindToolTips->move(ui->btnBind->mapTo(this,tmp));
    m_BindToolTips->setTimeAutoHide(3);

    m_IntroToolTips = new LineEditTooltip(this);
    m_IntroToolTips->setText(LINEEDIT_MESS);
    m_IntroToolTips->setFixedSize(160,45);

    ui->btnSetcenter->setToolTip(DES_SETCENTER_RX);
    ui->btnBind->setToolTip(DES_BIND_RX);
    ui->rbONGimbal->setToolTip(DES_GIMBAL_ONOFF);
    ui->btnChangeRXView->setToolTip(DES_CHANGE_RX_MONITOR_VIEW);
    ui->btnSave->setToolTip(DES_SAVEFILE);
    ui->btnOpen->setToolTip(DES_LOADFILE);
    ui->btnSetdefault->setToolTip(DES_DEFAULTGAIN);
    m_messbox_writetocf =  new vtmessagebox(this);
    m_messbox_writetocf->setMessage("Write all config to FC ?");
    m_messbox_writetocf->move(430,605);
}

void mainWidget::initRxview()
{
    rxViewLeft = new RxMonitorView(RX_MONITOR_WIDTH, RX_MONITOR_HEIGHT,
                                   RX_SENSOR_WIDTH, RX_SENSOR_HEIGHT, ui->stackedWidgetRxView->widget(1));
    rxViewLeft->setRangeValue(920,2120);
    rxViewLeft->setLocation(RX_LOCATION_LEFT);
    rxViewLeft->move(40, 15);

    rxViewRight = new RxMonitorView(RX_MONITOR_WIDTH, RX_MONITOR_HEIGHT,
                                    RX_SENSOR_WIDTH, RX_SENSOR_HEIGHT, ui->stackedWidgetRxView->widget(1));
    rxViewRight->setRangeValue(920,2120);
    rxViewRight->setLocation(RX_LOCATION_RIGHT);
    rxViewRight->move(295, 15);
}

void mainWidget::initSensorview()
{
    m_rollSensor = new DrawImages(ui->tabSensor);
    m_rollSensor->setGeometry(330,140,298,288);
    m_rollSensor->setDrawBackGround(true);
    m_rollSensor->setBackGroundImage(ROLL_PITCH_BACKGROUND_IMAGE_URL);
    m_rollSensor->setRollPixmapUrl(ROLL_SENSOR_URL);
    m_rollSensor->setPitchPixmapUrl(PITCH_SENSOR_URL);
    m_rollSensor->show();

    m_yawSensor = new DrawImages(ui->tabSensor);
    m_yawSensor->setGeometry(668,140,298,288);
    m_yawSensor->setDrawBackGround(false);
    m_yawSensor->setBackGroundImage(YAW_BACKGROUND_IMAGE_URL);
    m_yawSensor->setRollPixmapUrl(YAW_SENSOR_URL);
    m_yawSensor->setPitchPixmapUrl("");
    m_yawSensor->show();
}

