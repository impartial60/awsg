#include <QtGui>
#include "mainWindowG.h"
#include "ui_mainwindow.h"

//const int scaleHeight = 34;
//const int scaleWidth = 37;
extern float convert360angle2PlusMinus180(float dltAz360);

void MainWindow::MouseMoveScales(int hovMain, int hovDelt)
{    
    scaleInfo->detailScaleHoverX = hovDelt;
    scaleInfo->mainScaleHoverX = hovMain;

    ui->labelPorog->setText(QString("e: %1° [%2°]").arg(hovMain).arg(hovDelt));
}

void MainWindow::MouseClickSlot_elevationScale(double deltaHov)
{
    shMemU2C.guidanceOfficerCmds.servoDriveDelta[ELEVANG] = convert360angle2PlusMinus180(deltaHov);
    shMemU2C.guidanceOfficerCmds.servoDriveExecute[ELEVANG] = true;

    qDebug()<<"***   MouseClickSlot_eleavationScale: shMemU2C.guidanceOfficerCmds.servoDriveDelta[ELEVANG]:"
               <<shMemU2C.guidanceOfficerCmds.servoDriveDelta[ELEVANG];//<<"shMemU2C.userCmds.deltaElevationAngle:"<<shMemU2C.userCmds.deltaElevationAngle;
}

void MainWindow::MouseClickSlot_azimuthScale(double deltaAzimHov)
{
    //ui->labelTest_2->setText(QString("deltaAzimHov: %1").arg(deltaAzimHov));

    qDebug()<<"MouseClickSlot: deltaAzimHov:"<<deltaAzimHov;

    shMemU2C.guidanceOfficerCmds.servoDriveDelta[AZIMUTH] = convert360angle2PlusMinus180(deltaAzimHov);
    shMemU2C.guidanceOfficerCmds.servoDriveExecute[AZIMUTH] = true;

    //qDebug()<<"*************************************    MouseClickSlot: shMemU2C.userCmds.executeNewAzManual:"<<shMemU2C.userCmds.executeNewAzManual;
}

