#include "mainWindowG.h"
#include "ui_mainwindow.h"
#include <QDebug>

extern QString btnCSS[2];

int otrabotkaCY_counter = 0;

void MainWindow::userCommandCoder()
{
    otrabotkaCY_counter++;
    //if(shMemC2U.commander2Guidance.komKom.remoteControlZRK && shMemC2U.commander2Guidance.komKom.rejimRabotiZRK == RejimRabotiZRK::TR_K)
      //  shMemU2C.guidanceOfficerCmds.simulatorTargetIndexCY = 0;

    //shMemU2C.guidanceOfficerCmds.executeNewAzByHandWheel = ui->pushButtonHandWheels->isChecked();

//    if(shMemC2U.srvDrvInfo.rejimPoiskaSNR != RejimPoiskaSNR::POISK_PEREKLYCHEN_V_ZAHVAT && shMemU2C.guidanceOfficerCmds.rejimPoiskaSNR == RejimPoiskaSNR::POISK_PEREKLYCHEN_V_ZAHVAT)// //ui->pushButtonZhvt->isChecked() == true)
//        trackPointsCount = 0;

    /*
    if(shMemU2C.guidanceOfficerCmds.rejimPoiskaSNR == RejimPoiskaSNR::POISK_PEREKLYCHEN_V_ZAHVAT || shMemU2C.guidanceOfficerCmds.rejimPoiskaSNR == RejimPoiskaSNR::POISK_PEREKLYCHEN_V_SOPROVOJDENIE)//if(shMemU2C.guidanceOfficerCmds.cyStatus == CYStatus::OTRABOTKA_CY)
    {
        float deltaMax = 0.3;

        float deltaAz = shMemC2U.apyInfo.nextTocka[shMemU2C.guidanceOfficerCmds.simulatorTargetIndexCY].azimuth360 - shMemC2U.srvDrvInfo.currentAzimuth;

        if(deltaAz > 180)
            deltaAz -= 360.0;

        if(fabs(deltaAz) > deltaMax)
        {
            shMemU2C.guidanceOfficerCmds.deltaAzim = deltaAz;
            shMemU2C.guidanceOfficerCmds.executeNewAzManual = true;
        }

        float deltaElAngle = shMemC2U.apyInfo.nextTocka[shMemU2C.guidanceOfficerCmds.simulatorTargetIndexCY].elevationAngle - shMemC2U.srvDrvInfo.currentDirectrisaElevationAngle;

        if(fabs(deltaElAngle) > deltaMax)
        {
            shMemU2C.guidanceOfficerCmds.deltaElevationAngle = deltaElAngle;
            shMemU2C.guidanceOfficerCmds.executeNewElevationAngleManual = true;
        }

        if(fabs(deltaAz) <= deltaMax && fabs(deltaElAngle) <= deltaMax)
        {
            shMemU2C.guidanceOfficerCmds.rejimPoiskaSNR = RejimPoiskaSNR::POISK_PEREKLYCHEN_V_SOPROVOJDENIE;//shMemU2C.guidanceOfficerCmds.cyStatus = CYStatus::CY_OTRABOTANO;

            ui->lamp_zahvat->setStyleSheet(QString("QLabel{background-repeat: no-repeat; background-position: right;"
                                                       " background-image: url(:/b_ind2/resgui/b_ind2/buttonIndGreen023.png); padding-right: 4px;}"));

            qDebug()<<"userCommandCoder: fabs(deltaAz) <= deltaMax && fabs(deltaElAngle) <= deltaMax deltaAz:"<<deltaAz<<"deltaElAngle:"<<deltaElAngle;
        }
        else
        {
            int polPeriodaNumTicks = 250 / TIMER_INTERVAL;

            otrabotkaCY_counter++;

            int val = otrabotkaCY_counter / polPeriodaNumTicks;

            //qDebug()<<"userCommandCoder: otrabotkaCY_counter:"<<otrabotkaCY_counter<<"polPeriodaNumTicks:"<<polPeriodaNumTicks<<"otrabotkaCY_counter / polPeriodaNumTicks:"<<val;

            ////////qDebug()<<"userCommandCoder: else deltaAz:"<<deltaAz<<"deltaElAngle:"<<deltaElAngle;

            if(val % 2 == 0)
                ui->lamp_zahvat->setStyleSheet(QString("QLabel{background-repeat: no-repeat; background-position: right;"
                                                           " background-image: url(:/b_ind2/resgui/b_ind2/buttonIndGreen023.png); padding-right: 4px;}"));
            else
                ui->lamp_zahvat->setStyleSheet(QString("QLabel{background-repeat: no-repeat; background-position: right;"
                                                           " background-image: url(:/b_ind2/resgui/b_ind2/buttonIndGray023.png); padding-right: 4px;}"));
        }
    }
    */

    shMemU2C.guidanceOfficerCmds.scannerUV10IsOn = ui->pushButtonScannerUV10->isChecked();
    shMemU2C.guidanceOfficerCmds.scannerUV11IsOn = ui->pushButtonScannerUV11->isChecked();

    shMemU2C.guidanceOfficerCmds.writeDataContinues = ui->pushButtonEchoRecordContinues->isChecked();

    shMemU2C.guidanceOfficerCmds.integerValue4Test = ui->sliderEchoBrightness->value();

    if(ui->pushButtonServoDrive_Off->isChecked())
    {
        shMemU2C.guidanceOfficerCmds.rejimPoiskaSNR = POISK_VIKLUCHEN;
    }
    else
    {
//        if(shMemC2U.srvDrvInfo.rejimPoiskaSNR == POISK_VIKLUCHEN)  // при включении сканирования
//            shMemU2C.guidanceOfficerCmds.directrissaAzimuth = shMemC2U.srvDrvInfo.currentAzimuth; // запомнить директрису

        if(ui->pushButtonKrygSP->isChecked())
            shMemU2C.guidanceOfficerCmds.rejimPoiskaSNR = POISK_Kryg;
        else
            if(ui->pushButtonBSP->isChecked())
                shMemU2C.guidanceOfficerCmds.rejimPoiskaSNR = POISK_BSP;
            else
                if(ui->pushButtonMSP->isChecked())
                    shMemU2C.guidanceOfficerCmds.rejimPoiskaSNR = POISK_MSP;
    }

    if(ui->pushButtonAS->isChecked())
        shMemU2C.guidanceOfficerCmds.rejimSoprovojdeniaSNR = SOPROVOJDENIE_AS;
    else
        if(ui->pushButtonASRS->isChecked())
            shMemU2C.guidanceOfficerCmds.rejimSoprovojdeniaSNR = SOPROVOJDENIE_ASDRSF;
        else
            if(ui->pushButtonRS->isChecked())
                shMemU2C.guidanceOfficerCmds.rejimSoprovojdeniaSNR = SOPROVOJDENIE_RS;


    if(ui->azimuthScale360->controlModifierIsHoldingDown || ui->azimuthScale3600->controlModifierIsHoldingDown)
    {
        if(ui->azimuthScale360->controlModifierIsHoldingDown)
            shMemU2C.guidanceOfficerCmds.servoDriveDelta[AZIMUTH] = ui->azimuthScale360->speed;
        if(ui->azimuthScale3600->controlModifierIsHoldingDown)
            shMemU2C.guidanceOfficerCmds.servoDriveDelta[AZIMUTH] = ui->azimuthScale3600->speed;
        shMemU2C.guidanceOfficerCmds.servoDriveExecute[AZIMUTH] = true;

//        qDebug()<<"$$$    userCommandCoder: userCmds.executeNewAzManual:"<<shMemU2C.userCmds.executeNewAzManual
//               <<"ctrl360:"<<ui->azimuthScale360->controlModifierIsHoldingDown<<"ctrl3600:"<<ui->azimuthScale3600->controlModifierIsHoldingDown;
    }
//    else  закомментировано 2018-02-24
//    {
//        if((ui->sliderFi1_proizv->value() != 0 || ui->sliderFi2_proizv->value() != 0) && shMemU2C.guidanceOfficerCmds.rejimPoiskaSNR == RejimPoiskaSNR::POISK_VIKLUCHEN)//shMemU2C.guidanceOfficerCmds.cyStatus == CYStatus::CY_NE_POSTYPALO)
//        {
//            shMemU2C.guidanceOfficerCmds.executeNewAzManual = true;
//            shMemU2C.guidanceOfficerCmds.executeNewElevationAngleManual = true;

//            float sliderFactor = 0.01;

//            float fi1Proizv = ui->sliderFi1_proizv->value();
//            fi1Proizv *= sliderFactor;// * fabs(fi1Proizv);

//            float fi2Proizv = ui->sliderFi2_proizv->value();
//            fi2Proizv *= sliderFactor;// * fabs(fi2Proizv);

//            shMemU2C.guidanceOfficerCmds.deltaAzim =           fi1Proizv * SIN45COS45 + fi2Proizv * SIN45COS45;
//            shMemU2C.guidanceOfficerCmds.deltaElevationAngle = fi1Proizv * SIN45COS45 - fi2Proizv * SIN45COS45;

//            ui->labelPorog_2->setText(QString("fi1Proizv: %1°    fi2Proizv: %2°").arg(fi1Proizv).arg(fi2Proizv));
//            ui->labelPorog->setText(QString("b_proizv: %1°    e_proizv: %2°").arg(shMemU2C.guidanceOfficerCmds.deltaAzim).arg(shMemU2C.guidanceOfficerCmds.deltaElevationAngle));

////            qDebug()<<"$$$    userCommandCoder: shMemU2C.userCmds.deltaAzim:"<<shMemU2C.userCmds.executeNewAzManual
////            //               <<"ctrl360:"<<ui->azimuthScale360->controlModifierIsHoldingDown<<"ctrl3600:"<<ui->azimuthScale3600->controlModifierIsHoldingDown;
//        }
//    }



//  2018-02-24    if(shMemU2C.guidanceOfficerCmds.executeNewAzManual == false && shMemC2U.commander2Guidance.komKom.executeNewAzManualCounter > 0)   //  2018-02-03
//    {
//        shMemU2C.guidanceOfficerCmds.executeNewAzManual = true;
//        shMemU2C.guidanceOfficerCmds.deltaAzim = shMemC2U.commander2Guidance.komKom.deltaAzim;

//        qDebug()<<"userCommandCoder otrabotkaCY_counter:"<<otrabotkaCY_counter<<"shMemC2U.commander2Guidance.komKom.executeNewAzManualCounter:"<<shMemC2U.commander2Guidance.komKom.executeNewAzManualCounter
//                <<"shMemU2C.guidanceOfficerCmds.deltaAzim:"<<shMemU2C.guidanceOfficerCmds.deltaAzim;
//    }

///////////////////////    if(shMemU2C.guidanceOfficerCmds.executeNewElevationAngleManual == false && shMemC2U.komKom.executeNewElAngleManualCounter > 0)   //  2018-02-03    {
///////////////////////        shMemU2C.guidanceOfficerCmds.executeNewElevationAngleManual = true;
///////////////////////        shMemU2C.guidanceOfficerCmds.deltaElevationAngle = shMemC2U.komKom.deltaElevationAngle;    }

//    qDebug()<<"userCommandCoder: userCmds.unvScanIsOn:"<<shMemU2C.userCmds.unvScanIsOn<<"userCmds.executeNewAzManual:"<<shMemU2C.userCmds.deltaAzim<<shMemU2C.userCmds.executeNewAzManual;

}
