#include <QtGui>
#include "mainWindowG.h"
#include "ui_mainwindow.h"

//tcpdump -i enp2s0 -vvv -X             просмотр пакетов в бинарном виде

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    p_iron_buttons{new EventHandler_IronButtons}
{
    //qDebug()<<"awsG125User: MainWindow::MainWindow";// sizeof(USER_COMMANDS):"<<sizeof(USER_COMMANDS)<<"sizeof(SNR_INFO):"<<sizeof(SNR_INFO)
           //<<"sizeof(USM_incomingData):"<<sizeof(USM_incomingData)<<"sizeof(MDO_incomingData):"<<sizeof(MDO_incomingData);

//    qDebug()<<"awsg125User: MainWindow::MainWindow sizeof(MDO1_commands):"<<sizeof(MDO1_commands)<<"sizeof(MDO1_incomingData):"<<sizeof(MDO1_incomingData)
//            <<"sizeof(MDO1_echoData):"<<sizeof(MDO1_echoData);

//    float factor = 3.1415926 / 60; //float factor = Rad2Grad / 60;

//    for(int i = 1; i < 361; i++)
//    {
//        //float val = 1.0 / Rad2Grad * i;
//        float val = factor * i;
//        float arsin = fabs(sin(val) / (val));
//        qDebug()<<"awsg125User: i:"<<i<<"arsin:"<<arsin;
//    }

    redFi2 = 0.5;
    bluFi2 = 0.8;
    redFi1 = redFi2;//0.8;
    bluFi1 = bluFi2;//0.0;

    transmitterStatus = TransmitterStatus::ISKHODNOE;
    //usmData.MakeCRC16Table();   //0xB4C8
    timeFrameIncr = 0;
    nakalTimeInSeconds = NAKAL_MAX_TIME_SECONDS;
    nakalMaxTicks = (1000.0 / TIMER_INTERVAL) * NAKAL_MAX_TIME_SECONDS;
    nakalTimeInTicks = nakalMaxTicks;

    ui->setupUi(this);

    handwheel_epsilon.reset(new EncoderControl(51));
    handwheel_betta.reset(new EncoderControl(52));
    handwheel_threshold.reset(new EncoderControl(50));

    oldHandWheelPosition[AZIMUTH] = handwheel_betta->getPosition();
    oldHandWheelPosition[ELEVANG] = handwheel_epsilon->getPosition();
    oldHandWheelPosition[THRESHOLD] = handwheel_threshold->getPosition();

    if( p_iron_buttons->is_valid() )
    {
        connect(p_iron_buttons.get(),&EventHandler_IronButtons::onKeyDown,     this,&MainWindow::on_iron_button_down);
        connect(p_iron_buttons.get(),&EventHandler_IronButtons::onKeyUp,       this,&MainWindow::on_iron_button_up);
        connect(p_iron_buttons.get(),&EventHandler_IronButtons::onKeyPressFail,this,&MainWindow::on_iron_button_error);
    }

    ui->azimuthScale360 ->controlModifierIsHoldingDown = false;
    ui->azimuthScale3600->controlModifierIsHoldingDown = false;

//    qDebug()<<"MainWindow::MainWindow: userCmds.executeNewAzManual:"<<shMemU2C.guidanceOfficerCmds.executeNewAzManual
//           <<"ctrl360:"<<ui->azimuthScale360->controlModifierIsHoldingDown<<"ctrl3600:"<<ui->azimuthScale3600->controlModifierIsHoldingDown;

    //monoTrack = (TargetParams *)calloc(MONOTRACKSIZE, sizeof(TargetParams));
    //monoTrack = new TargetParams[MAXTOCKANUM * MONOTRACKSIZE];

    //usmData.afterStartOperation(ui);
    ui->label_time_remain->setText(QString::number(nakalTimeInSeconds) + "\"");
    ui->progressBarTimeRemain->setMaximum(nakalMaxTicks);
    ui->progressBarTimeRemain->setValue(nakalMaxTicks);

    scaleInfo = new SCALE_INFO();
    setShadowEffects();

    QPixmap pixmap(ui->indicatorEB->width(), ui->indicatorEB->height() - 10);
    indicatorE = pixmap;

    settings = new SETTINGSG(ui);

    console2User.komKom.targetIndexCY = -1;

    shMemU2C.guidanceOfficerCmds.rejimPoiskaSNR = RejimPoiskaSNR::POISK_VIKLUCHEN;
    shMemU2C.guidanceOfficerCmds.rejimSoprovojdeniaSNR = RejimSoprovojdeniaSNR::SOPROVOJDENIE_AS;

    for(int i = 0; i < 2; i ++)     //  2018_07_20  for(int i = 0; i < 3; i ++)
    {
        shMemU2C.guidanceOfficerCmds.servoDriveExecute[i] = false;
        shMemU2C.guidanceOfficerCmds.servoDriveDelta[i] = 0.0;
    }

    trackPointsCount = 0;

    //btnsInit();

//    graypix.load(":/InfoInd/resgui/infoind/ind_gray7.png");
//    lightGreenPixmap.load(":/InfoInd/resgui/infoind/ind_green72.png");
//    darkGreenPixmap.load(":/InfoInd/resgui/infoind/ind_darkGreen01.png");
//    lightRedPixmap.load(":/InfoInd/resgui/infoind/ind_red7.png");
//    darkRedPixmap.load(":/InfoInd/resgui/infoind/ind_darkRed01.png");

    labelPixmap[LABELINDICATORCOLORS::Graypix].load(":/InfoInd/resgui/infoind/ind_gray7.png");
    labelPixmap[LABELINDICATORCOLORS::LightGreenPixmap].load(":/InfoInd/resgui/infoind/ind_green72.png");
    labelPixmap[LABELINDICATORCOLORS::DarkGreenPixmap].load(":/InfoInd/resgui/infoind/ind_darkGreen01.png");
    labelPixmap[LABELINDICATORCOLORS::LightRedPixmap].load(":/InfoInd/resgui/infoind/ind_red7.png");
    labelPixmap[LABELINDICATORCOLORS::DarkRedPixmap].load(":/InfoInd/resgui/infoind/ind_darkRed01.png");

    QPixmap pixmap3(ui->oscilloscope->width(), ui->oscilloscope->height());
    oscBMP = pixmap3;    

    QPixmap pixmap4(ui->indicatorRV->width(), ui->indicatorRV->height());
    indicatorRV_Fischuk_BMP = pixmap4;

    QPixmap pixmap41(ui->indicatorRV->width(), ui->indicatorRV->height());
    singleTargetRV_BMP = pixmap41;

//    yarkostOsc = (double *)calloc(MAXNUMTOCHEKFORPAINT_OSC_FISCHUK * HGT_H_OSC_FISCHUK, sizeof(double));
//    imgDataOsc = (unsigned char*)calloc(MAXNUMTOCHEKFORPAINT_OSC_FISCHUK * HGT_H_OSC_FISCHUK * 3, sizeof(unsigned char));

    yarkostEB = (double *)calloc(MAXNUMTOCHEKFORPAINT_EB * HGT_H_EB, sizeof(double));
    imgDataEB = (unsigned char*)calloc(MAXNUMTOCHEKFORPAINT_EB * HGT_H_EB * 3, sizeof(unsigned char));

//    yarkostRV = (double *)calloc(MAX_NUM_DISCRETS_R * MAX_NUM_MDO, sizeof(double));
//    imgDataRV = (unsigned char*)calloc(MAX_NUM_DISCRETS_R * MAX_NUM_MDO * 3, sizeof(unsigned char));

    //////////////inclinXY = (float *)calloc(INCLIN_HISTORY_SIZE * 2, sizeof(float ));

    //interchange2ConsoleStart();

    connect(ui->azimuthScale360,  SIGNAL(Mouse_Pos(int, int)), this, SLOT(MouseMoveScales(int, int)));
    connect(ui->azimuthScale3600, SIGNAL(Mouse_Pos(int, int)), this, SLOT(MouseMoveScales(int, int)));
    connect(ui->elevationScale87, SIGNAL(Mouse_Pos(int, int)), this, SLOT(MouseMoveScales(int, int)));
    connect(ui->elevationScale870, SIGNAL(Mouse_Pos(int, int)), this, SLOT(MouseMoveScales(int, int)));

    connect(ui->indicatorEB, SIGNAL(Mouse_Pos(int, int)), this, SLOT(MouseMoveIndicatorEB(int, int)));
    connect(ui->indicatorEB, SIGNAL(Mouse_Click()), this, SLOT(MouseClickIndicatorEB()));


    connect(ui->azimuthScale360, SIGNAL(Mouse_Click(double)), this, SLOT(MouseClickSlot_azimuthScale(double)));
    connect(ui->azimuthScale3600, SIGNAL(Mouse_Click(double)), this, SLOT(MouseClickSlot_azimuthScale(double)));
    connect(ui->elevationScale87, SIGNAL(Mouse_Click(double)), this, SLOT(MouseClickSlot_elevationScale(double)));
    connect(ui->elevationScale870, SIGNAL(Mouse_Click(double)), this, SLOT(MouseClickSlot_elevationScale(double)));

    //connect(ui->indicatorEB, SIGNAL(doubleClicked(int, int)), this, SLOT(MouseDoubleClickSlot_indicatorEB(int, int)));

    signalMapper = new QSignalMapper(this);

    connect(ui->pushButtonEnglish, SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui->pushButtonPortug, SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui->pushButtonRussian, SIGNAL(clicked()), signalMapper, SLOT(map()));

    signalMapper->setMapping(ui->pushButtonEnglish, 0);
    signalMapper->setMapping(ui->pushButtonPortug, 2);
    signalMapper->setMapping(ui->pushButtonRussian, 1);

    connect(signalMapper, SIGNAL(mapped(const int &)), this, SLOT(languageChange(const int &)));

    languageChange(settings->languageIndex);

    drawScale360(0, 0);
    drawScale3600(0, 0);
    drawElevationScale87(0);
    drawElevationScale870(0);

    // ИНИЦИАЛИЗАЦИЯ КЛИЕНТСКОГО UDP-СОКЕТА
    qDebug()<<"MainWindow::MainWindow: settings->ipAddressServerUNV:"<<settings->ipAddress_dispatcher
           <<"settings->port_awsMainConsoleApp:"<<settings->port_dispatcherG;

    udpSockAddrLength = sizeof(udpSockAddr);
    if ((udpClientSocketFD=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        qDebug()<<"udpSrv: socket failed";  //connectStatus = false;//die("socket");
    }

    fcntl(udpClientSocketFD, F_SETFL, O_NONBLOCK);

    udpSockAddr.sin_family = AF_INET;
    udpSockAddr.sin_port = htons(settings->port_dispatcherG);
    udpSockAddr.sin_addr.s_addr = inet_addr(settings->ipAddress_dispatcher);

    drawGraphicsTimer =  new QTimer(this);
    connect(drawGraphicsTimer, SIGNAL(timeout()), this, SLOT(timerTickSlot()));

    /////////    for IRON BUTTONS & HANDWHEELS    /////////
    //encoder63OldPosition = listner63->newPosition;

    drawGraphicsTimer->start(TIMER_INTERVAL);    
}

MainWindow::~MainWindow()
{
    delete ui;
}

//int cntr = 120;

void MainWindow::timerTickSlot()
{
    ironHandWheelOptions();

    //qDebug()<<"timerTickSlot:"<<cntr++;
    //qDebug()<<"timerTickSlot: QApplication::mouseButtons():"<<QApplication::mouseButtons();

    //if(Qt::ControlModifier == QApplication::keyboardModifiers())

//    if(ui->azimuthScale360->controlModifierIsHoldingDown || ui->azimuthScale3600->controlModifierIsHoldingDown)
//    {
//        ///////////////qDebug()<<"timerTickSlot: speed:"<<ui->azimuthScale360->speed;
//    }

    userCommandCoder();
    usmCommandCoder();//local_memory + SERVICE_TOTAL_LENGTH);
    mdo1CommandCoder();
    timeFrameIncr ++;

//    qDebug()<<QString("MainWindow::timerTickSlot: 0x%1 0x%2 0x%3").arg(local_memory[SERVICE_TOTAL_LENGTH + 16], 2, 16, QChar('0'))
//              .arg(local_memory[SERVICE_TOTAL_LENGTH + 17], 2, 16, QChar('0')).arg(local_memory[SERVICE_TOTAL_LENGTH + 18], 2, 16, QChar('0'));

    // С И Н Х Р О Н И З А Ц И Я   П О И С К А   С   К О М А Н Д И Р О М

    ///////////////////////////////////////////////////////int n = interchange2Console();  //qDebug()<<"timerTickSlot n:"<<n;

    ssize_t totalReadBytes = 0;

//    qDebug()<<"timerTickSlot:"<<shMemU2C.guidanceOfficerCmds.servoDriveExecute[0]<<shMemU2C.guidanceOfficerCmds.servoDriveExecute[1]
//            <<shMemU2C.guidanceOfficerCmds.servoDriveDelta[0]<<shMemU2C.guidanceOfficerCmds.servoDriveDelta[1];

    //if(connectStatus)
    {
        sendto(udpClientSocketFD, &shMemU2C, sizeof(KOMANDI_OFICERA_NAVED), 0, (struct sockaddr *) &udpSockAddr, udpSockAddrLength);

        ssize_t nextReadBytes;
        int readCirclCounter = 0;
        char readByteArray[sizeof(MServer2UserG)];

        while (totalReadBytes < abs(sizeof(MServer2UserG)) && readCirclCounter++ < 10)
        {
            nextReadBytes = recvfrom(udpClientSocketFD, readByteArray + totalReadBytes, sizeof(MServer2UserG) - totalReadBytes, 0, (struct sockaddr *) &udpSockAddr, &udpSockAddrLength);

            if(nextReadBytes < 0)
                nextReadBytes = 0;

            totalReadBytes += nextReadBytes;
        }

        if(totalReadBytes == sizeof(MServer2UserG))
            memcpy(&console2User, readByteArray, sizeof(MServer2UserG));

//        qDebug()<<"udpSocketReadWriteData totalReadBytes:"<<totalReadBytes<<"currentAzimuth:"<<shMemC2U.commander2Launcher.srvDrvInfo.currentAzimuth
//                  <<"missileStatus:"<<shMemC2U.launcherInfo[1].missileInfo[0].missileStatus;

        shMemU2C.guidanceOfficerCmds.servoDriveExecute[AZIMUTH] = false;
        shMemU2C.guidanceOfficerCmds.servoDriveExecute[ELEVANG] = false;
    }

    diagnosticParser(totalReadBytes);

    nextTochkaOptions();

    //qDebug()<<"***   MainWindow::timerTickSlot shMemU2C.guidanceOfficerCmds.rejimPoiskaSNR:"<<shMemU2C.guidanceOfficerCmds.rejimPoiskaSNR;

    for(int j =  console2User.apyInfo.totalSimTargetNum - 1; j >= 0; j --)
    {
        targetObsolTime[j] += 0.001 * drawGraphicsTimer->interval();        //должно устаревать всегда -> переносим в таймер
    }

    //if(shMemC2U.srvDrvInfo.rejimPoiskaSNR == RejimPoiskaSNR::ZAHVAT || shMemC2U.srvDrvInfo.rejimPoiskaSNR == RejimPoiskaSNR::SOPROVOJDENIE)//if(ui->pushButtonZhvt->isChecked())
    if(shMemU2C.guidanceOfficerCmds.rejimPoiskaSNR == RejimPoiskaSNR::POISK_PEREKLYCHEN_V_ZAHVAT || shMemU2C.guidanceOfficerCmds.rejimPoiskaSNR == RejimPoiskaSNR::POISK_PEREKLYCHEN_V_SOPROVOJDENIE)
        drawIndicatorRF();
    else
        drawIndicatorEB();

//    qDebug()<<"timerTickSlot           shMemC2U.komKom.poiskState"<<shMemC2U.komKom.poiskState//shMemC2U.commander2Guidance.komKom.poiskState
//           <<"shMemC2U.guidanceOfficerCmds.rejimPoiskaSNR:"<<shMemU2C.guidanceOfficerCmds.rejimPoiskaSNR<<"shMemC2U.srvDrvInfo.rejimPoiskaSNR:"<<shMemC2U.srvDrvInfo.rejimPoiskaSNR;

    if(console2User.komKom.poiskState > 200 && console2User.komKom.poiskState <= 290)
    {
        ui->pushButtonBSP->setChecked(true);
//        qDebug()<<"ui->pushButtonBSP->setChecked(true);"<<"ui->pushButtonBSP->isChecked()"<<ui->pushButtonBSP->isChecked();
    }

    if(console2User.komKom.poiskState > 300 && console2User.komKom.poiskState <= 390)
    {
        ui->pushButtonMSP->setChecked(true);
//        qDebug()<<"ui->pushButtonMSP->setChecked(true);"<<"ui->pushButtonMSP->isChecked()"<<ui->pushButtonMSP->isChecked();
    }

    if(console2User.komKom.poiskState > 500 && console2User.komKom.poiskState <= 590)
    {
        ui->pushButtonServoDrive_Off->setChecked(true);
//        qDebug()<<"ui->pushButtonServoDrive_Off->setChecked(true);"<<"ui->pushButtonServoDrive_Off->isChecked()"<<ui->pushButtonServoDrive_Off->isChecked();
    }

    if(console2User.komKom.targetIndexCY > - 1)
    {       
        target_IndicatorEB_MouseMove_Number = console2User.komKom.targetIndexCY;
        MouseClickIndicatorEB();
    }

    //if(shMemU2C.userCmds.unvScanIsOn)
    drawAzimuthScales(console2User.srvDrvInfo.currentAzimuth);
    ///////////////////drawElevationScale87(console2User.srvDrvInfo.currentDirectrisaElevationAngle);
    ///////////////////drawElevationScale870(console2User.srvDrvInfo.currentDirectrisaElevationAngle);

    // Р Е Ж И М Ы   Р А Б О Т Ы   З Р К

    bool lampIsOn[4];

    for(int i = 0; i < 4; i++)
        lampIsOn[i] = false;

    switch((int)console2User.apyInfo.rejimRabotiZRK)
    {
        case RejimRabotiZRK::BR: lampIsOn[0] = true; break;
        case RejimRabotiZRK::FK: lampIsOn[1] = true; break;
        case RejimRabotiZRK::TR_A: case RejimRabotiZRK::TR_K: lampIsOn[2] = true; break;
        case RejimRabotiZRK::RR: lampIsOn[3] = true;
    }

    ui->lampRejimRabotiBR->setEnabled(lampIsOn[0]);
    ui->lampRejimRabotiFK->setEnabled(lampIsOn[1]);
    ui->lampRejimRabotiTren->setEnabled(lampIsOn[2]);
    ui->lampRejimRabotiReglRab->setEnabled(lampIsOn[3]);

    // Д А Т А   И   В Р Е М Я
    QDateTime now =  QDateTime::currentDateTime();
    if(now.time().msec() < 500)
        ui->qLabelTime->setText(now.toString("hh:mm:ss"));
    else
        ui->qLabelTime->setText(now.toString("hh mm ss"));
    ui->qLabelDate->setText(now.toString("yyyy.MM.dd"));
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    shMemU2C.stopConsolePrgrm = 0xFF;

    settings->saveToFile(ui);

    usleep(300 * 1000);

//    qDebug()<<3121212;
//    QMessageBox msgBox;
//    msgBox.setText("The document has been modified.");
//    msgBox.setInformativeText(QString::number(arm->settings->seaLevel));
//    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
//    msgBox.setDefaultButton(QMessageBox::Save);
//    int ret = msgBox.exec();
}


void MainWindow::on_buttonClose_clicked()
{
    shMemU2C.stopConsolePrgrm = 0xFF;
    usleep(300 * 1000);

    close();
}

void MainWindow::setShadowEffect(QWidget *lbl)
{
    QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect();
       effect->setBlurRadius(1);
       effect->setXOffset(-1);
       effect->setYOffset(-1);
       effect->setColor(qRgba(0,0,0,255));

    lbl->setGraphicsEffect(effect);
}

void MainWindow::setShadowEffects()
{
    setShadowEffect(ui->labelWindowTitle);
    setShadowEffect(ui->groupBoxTransmitter);
    setShadowEffect(ui->groupBoxEcho);
    setShadowEffect(ui->groupBoxScanSector);
    setShadowEffect(ui->groupBoxZahvat);
    setShadowEffect(ui->groupBoxSynhro);
    setShadowEffect(ui->groupBoxYSM03);
    setShadowEffect(ui->groupBoxRejimRabotiZRK);

    setShadowEffect(ui->groupBoxDateTime);
    setShadowEffect(ui->groupBoxLanguage);
    setShadowEffect(ui->groupBoxMDO1);
    //setShadowEffect(ui->labelPlusMinusAzScale);
}

void MainWindow::on_pushButtonEchoRecordSingleShort_clicked()
{
    QImage *pckgImg = new QImage(64, 32, QImage::Format_RGB32);//new QImage(1280, 540, QImage::Format_RGB32);//QImage *pckgImg = new QImage(1020, 286, QImage::Format_RGB32);
    QPainter *painter = new QPainter(pckgImg);

    painter->fillRect(QRect(0, 0, pckgImg->width(), pckgImg->height()), QBrush(Qt::black, Qt::SolidPattern));

    painter->setPen(QColor::fromRgba(qRgba(0, 64, 255, 255)));
    //painter->drawRect(otstyp_slevaOsc + j * (256 + 28) - 5, otstyp_sverhyOsc + u * (256 + 8) - 4, 256 + 8, 256 + 4);
    //painter->drawRect(10, 10, 380, 280); // R-β    левый верх

    QFont fnt("FreeSerif");//Arial");
    fnt.setBold(true);
    fnt.setItalic(true);
    fnt.setPointSize(16);
    painter->setFont(fnt);
    //painter->drawText(250, pckgImg->height() - 20, "b");
    painter->drawText(3, pckgImg->height() - 10, QString::fromUtf8("R  β  ε"));

    QString pctFileName = "rbe.png";
    pckgImg->save(pctFileName,"PNG");
    delete painter;
    delete pckgImg;


    ///////////////shMemU2C.guidanceOfficerCmds.writeDataSingleShort = true;
}

void MainWindow::on_pushButtonFullScreen_clicked()
{
    this->setWindowState(this->windowState() ^ Qt::WindowFullScreen);
    this->setWindowState(this->windowState() ^ Qt::WindowMaximized);
}

void MainWindow::on_iron_button_down(int key)
{
    qWarning() << "on_iron_button_down " << key;

    switch(key)
    {
        case  2: ui->pushButtonBSP->setChecked(true); break;
        case  3: ui->pushButtonMSP->setChecked(true); break;
        case  4: ui->pushButtonServoDrive_Off->setChecked(true); break;

        case 10: ui->pushButtonAS->setChecked(true); break;
        case 11: ui->pushButtonASRS->setChecked(true); break;
        //case 13: ui->pushButtonRS->setChecked(true);
    }

}

void MainWindow::on_iron_button_up(int key)
{
    qWarning() << "on_iron_button_up   " << key;
}

void MainWindow::on_iron_button_error()
{
    qWarning() << "::Too many keys pressed...";
}

void MainWindow::languageChange(int lng)
{
    settings->languageIndex = lng;

    if(lng < 0 || lng > 2)
    {
        //qDebug()<<"languageChange lng:"<<lng;
        return;
    }

    setCaption(this, WindowWidget, lng, "АРМ офицера наведения",
                                        "Guidance Operator AWS",
                                        "");

    setCaption(ui->labelWindowTitle, LabelWidget, lng,  "Автоматизированное рабочее место офицера наведения",
                                                        "Guidance Operator Automated Workstation",
                                                        "");

    // В Е Р Х Н Я Я   П А Н Е Л Ь      В Е Р Х Н Я Я   П А Н Е Л Ь      В Е Р Х Н Я Я   П А Н Е Л Ь

    setCaption(ui->groupBoxCY, GroupBoxWidget, lng, "Целеуказание", "Target Designation", "Designação de alvos");
    setCaption(ui->pushButtonVneshneeCY, PushButtonWidget, lng, "Внешнее", "External", "Exterior");
    setCaption(ui->pushButtonMestnoeCY, PushButtonWidget, lng, "Местное", "Internal", "Interna");

    setCaption(ui->groupBoxRejimRabotiZRK, GroupBoxWidget, lng, "Реж.раб.ЗРК", "ADMS Mode", "");

    setCaption(ui->groupBoxDateTime, GroupBoxWidget, lng, "Дата, время", "Date, time", "   ");


    setCaption(ui->groupBoxScanSector, GroupBoxWidget, lng, "Режим поиска", "Scan Mode", "");
    setCaption(ui->pushButtonKrygSP, PushButtonWidget, lng, "   Круг. ", " Round", "");
    setCaption(ui->pushButtonBSP, PushButtonWidget, lng, "  БСП ", "   LSS  ", "");
    setCaption(ui->pushButtonMSP, PushButtonWidget, lng, " МСП ", "   SSS  ", "");
    setCaption(ui->pushButtonServoDrive_Off, PushButtonWidget, lng, "Выкл.", "   Off  ", "");


    setCaption(ui->groupBoxEcho, GroupBoxWidget, lng, "Яркость", "Brightness", "Luminosidade");
    setCaption(ui->pushButtonEcho, PushButtonWidget, lng, "  Эхо  ", " Echo ", "    ");


    //qDebug()<<"languageChange: "<<this->windowState();

//    if(this->windowState() == Qt::WindowFullScreen || this->windowState() == Qt::WindowNoState)
//    {
//        setCaption(ui->pushButtonFullScreen, PushButtonWidget, lng, "Оконный режим", "Window mode", "Modo de janela");
//    }
//    else
//    {
//        setCaption(ui->pushButtonFullScreen, PushButtonWidget, lng, "Полный экран", "Full screen", "Modo de tela cheia");
//    }
}

