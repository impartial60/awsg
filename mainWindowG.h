#ifndef MAINWINDOWG_H
#define MAINWINDOWG_H

#include <QMainWindow>
#include <QThread>
#include <QFrame>
#include <QPushButton>
#include <QLabel>
#include <QTimer>
#include <QDebug>
#include <QSignalMapper>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <sys/types.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <math.h>
#include <memory>

#include <encoder_control.h>
#include <EventHandler_IronButtons.h>
#include <sharedMem.h>
#include <externCommon.h>
#include <commonEnum.h>
#include "scaleInfo.h"
#include "settingsG.h"
#include "qlabelm.h"

#define INCLIN_HISTORY_SIZE 2//100

//#define MAX_NUM_TARGET_MDO 4

#define MAX_NUM_MDO 6

#define MAX_NUM_DISCRETS_R 19608//16384//8192

#define MAXNUMTOCHEKFORPAINT_OSC_FISCHUK 256
#define HGT_H_OSC_FISCHUK 256

#define MAXNUMTOCHEKFORPAINT_EB 1060//1184//1216
#define HGT_H_EB 256//240

#define WDT_NAVED_RF 200
#define HGT_NAVED_RF 400

//#define MONOTRACKSIZE 16000

//enum ColorIndicatorTypes{IndicatorIspravnost, IndicatorNeIsprvnst, IndicatorVkluchenia};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    KOMANDI_OFICERA_NAVED shMemU2C;
    MServer2UserG console2User;
    RejimRabotiZRK oldRejimRabotiZRK = RejimRabotiZRK::VIKLUCHEN;

    struct sockaddr_in udpSockAddr;
    int udpClientSocketFD;
    socklen_t udpSockAddrLength;

    TargetParams monoTrack[MAXTOCKANUM][MONOTRACKSIZE];
    int monoTrackElapsedTime[MAXTOCKANUM];
    int monoTrackPointsCount[MAXTOCKANUM];
    int monoTrackTargetNoNo[MAXTOCKANUM];

    int trackPointsCount;
    int target_IndicatorEB_MouseMove_Number = -1;
    int simTrgt_IndEB_MouseMove_Number = -1;

    float mouseCursorAzimuth;
    float mouseCursorElevAngle;

    //float detectedTargetElevAngle[MAXTOCKANUM];
    //float detectedTargetAzimuth[MAXTOCKANUM];

    QString strD[MAXTOCKANUM];
    QString strB[MAXTOCKANUM];
    QString strI[MAXTOCKANUM];

    float targetObsolTime[MAXTOCKANUM];

    float redFi1;
    float bluFi1;
    float redFi2;
    float bluFi2;

    int timeFrameIncr;

    float nakalTimeInSeconds;
    float nakalMaxTicks;
    float nakalTimeInTicks;

    int transmitterStatus;

    QPixmap oscBMP;
    QPixmap singleTargetRV_BMP;
    QPixmap indicatorRV_Fischuk_BMP;

//    QPixmap indicatorRVBMP;
    QPixmap indicatorE;

//    uchar * imgDataOsc;
//    double *yarkostOsc;

//    uchar * imgDataRV;
//    double *yarkostRV;

    uchar * imgDataEB;
    double *yarkostEB;

//    QPixmap graypix;
//    QPixmap lightGreenPixmap;
//    QPixmap darkGreenPixmap;
//    QPixmap lightRedPixmap;
//    QPixmap darkRedPixmap;

    QPixmap labelPixmap[5];

//    enum LABELINDICATORCOLORS {Graypix, LightGreenPixmap, DarkGreenPixmap, LightRedPixmap, DarkRedPixmap};

    QTimer *drawGraphicsTimer;

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void userCommandCoder();
    void usmCommandCoder();//unsigned char *outDataMemoryAddress);
    void usm02TransmitterManager(u_int8_t  inputs4,  u_int8_t  inputs6);
    void mdo1CommandCoder();

    void btnsInit();
    //////////////////////void setIndicatorColor(QLabel *indicatorLabel, bool enabled, int indicatorType, unsigned char stateIndex);
    //void set6IndicatorColors(unsigned char inputCh, QLabel lbl0, QLabel lbl1, QLabel lbl2, QLabel lbl3, QLabel lbl4, QLabel lbl5);
//    void fourVoltages(unsigned char inputCh, int startQFrameInd);
//    void ABCphases(unsigned char inputCh, QFrame *frameA, QFrame *frameB, QFrame *frameC);
//    void setTemperature(unsigned char inputCh, QLabel *label);

    //void MakeCRC16Table();
    //bool crc16BoolResult(unsigned char *pcBlock, unsigned int len, unsigned short int crc16Value);
    ////////////////////QString crc16QStringResult(unsigned char *pcBlock, unsigned int len, unsigned short int crc16Value);
    ////////////////////void assignCRC16Values(unsigned char *pcBlock, unsigned int len, unsigned char *lowByte , unsigned char *highByte);

    unsigned char priznakGr();
    //void transmitterManager(unsigned char *bufStartInd);//int startInd);
    void commandCoderAndSender();
    void diagnosticParser(int n);//, unsigned char *inChs);
    void ironHandWheelOptions();
    void nextTochkaOptions();

    void drawAzimuthScales(float azCur360);
    void drawScale360(float drctrAz, float azBlue);
    void drawScale3600(float drctrAz, float azBlue);
    void drawElevationScale87(float elevationAngleCur);
    void drawElevationScale870(float elevationAngleCur);

    void drawTextHalo(QPainter *painter, QPen textPen, QFont textFont, int x, int y, QString text);

    void drawOscilloscopeFischuk(int startIndex);
    void drawIndicatorRV_Fischuk();//int startIndex);

    void drawFormularEB(QPainter *painter, int j);
    void drawGridsEB(QPainter *painter);
    void drawIndicatorEB();

    void drawGridsRF(QPainter *painter);
    void drawIndicatorRF();

    void setShadowEffects();
    void setShadowEffect(QWidget *lbl);

protected:
    void closeEvent(QCloseEvent *event);

public slots:
    void timerTickSlot();

    void MouseMoveIndicatorEB(int xx, int yy);
    void MouseClickIndicatorEB();

    void MouseMoveScales(int hovMain, int hovDelt);

    void MouseClickSlot_azimuthScale(double deltaAzimHov);
    void MouseClickSlot_elevationScale(double deltaHov);
    //void MouseDoubleClickSlot_indicatorEB(int scrX, int scrY);

    void languageChange(int lng);

private slots:
    //void on_pushButtonNakal_clicked(bool checked);

    //void on_pushButtonTransmitter_clicked(bool checked);

    void on_buttonClose_clicked();

    void on_pushButtonEchoRecordSingleShort_clicked();

    void on_pushButtonFullScreen_clicked();

    void on_iron_button_down(int key);
    void on_iron_button_up(int key);
    void on_iron_button_error();

private:
    Ui::MainWindow *ui;
    QSignalMapper *signalMapper;

    SETTINGSG *settings;
    SCALE_INFO *scaleInfo;

    std::unique_ptr<EncoderControl> handwheel_epsilon{nullptr};
    std::unique_ptr<EncoderControl> handwheel_betta{nullptr};
    std::unique_ptr<EncoderControl> handwheel_threshold{nullptr};
    std::unique_ptr<EventHandler_IronButtons> p_iron_buttons;
    int oldHandWheelPosition[3];
};

#endif // MAINWINDOWG_H
