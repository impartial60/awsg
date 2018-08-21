#include <QtGui>
#include "mainWindowG.h"
#include "ui_mainwindow.h"

extern float convert360angle2PlusMinus180(float dltAz360);


QPen penSoprKnot(QColor::fromRgbF(1, 0.25, 1, 1), 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
QPen penZavKnot(QColor::fromRgbF(0.75, 0.5, 0.5, 1), 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);

QPen penSoprTrassa(QColor::fromRgbF(1, 0.25, 1, 0.1), 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
QPen penZavTrassa(QColor::fromRgbF(0.75, 0.5, 0.5, 0.1), 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);

float korrelOkno [10] = {1, 0.98, 0.93, 0.86, 0.75, 0.63, 0.5, 0.367, 0.25, 0.25};

int azLeftInt = 0;
int startAzInt = 0;
int otstyp_sverhyEB = 114;//134;
int otstyp_slevaEB = 20;
int sectorB_numDegrees = 120;//180;
float factorB_indicatorEB = 1.0 / sectorB_numDegrees * MAXNUMTOCHEKFORPAINT_EB;
int sectorE_numDegrees = 10;
float factorE_indicatorEB = 1.0 / sectorE_numDegrees * HGT_H_EB;

int maxNumTochekR =  MAXNUMTOCHEKFORPAINT_EB;  // временно

QColor gridColors[] = {
    QColor(0, 0, 0, 0),    QColor(16, 32, 24, 76),    QColor(24, 48, 36, 80),   QColor(28, 52, 40, 96), QColor(32, 56, 44, 112),
    QColor(32, 64, 48, 128), QColor(40, 72, 54, 132), QColor(48, 80, 64, 144),  QColor(56, 88, 72, 152), QColor(64, 112, 80, 160),

    QColor(64, 128, 96, 176), QColor(64, 128, 96, 192), QColor(64, 128, 96, 208),  QColor(64, 128, 96, 224), QColor(64, 128, 96, 240),
    QColor(64, 128, 96, 255), QColor(72, 136, 102, 255), QColor(80, 144, 104, 255), QColor(88, 152, 112, 255), QColor(96, 160, 120, 255),

    QColor(112, 176, 132, 255), QColor(144, 240, 208, 255), QColor(192, 255, 224, 255), QColor(255, 255, 255, 255)
};



void MainWindow::MouseMoveIndicatorEB(int xx, int yy)
{    
#if 1    //по новому   (c  М Д О - 1)
    target_IndicatorEB_MouseMove_Number = -1;
    float minNeviazka = 1000.0;
    int target_IndicatorEB_MouseMove_Number_Temp = -1;

    for(int j = 0; j < MAX_MDO1_PRIMARY_PLOTS_COUNT; j++)
    {
        if(console2User.primaryPlots[j].packageWeight == 0)
            break;

        //if(targetObsolTime[j] < 10.0) // ВРЕМЯ УСТАРЕВАНИЯ ФОРМУЛЯРА В СЕКУНДАХ
        {
            int nextTargetY = otstyp_sverhyEB + HGT_H_EB / 2 + factorE_indicatorEB * (console2User.srvDrvInfo.currentDirectrisaElevationAngle - console2User.primaryPlots[j].elevAngleF);
            int nextTargetX = startAzInt + factorB_indicatorEB * (console2User.primaryPlots[j].azimuth360 - azLeftInt);

            float nevX = nextTargetX - xx;
            float nevY = nextTargetY - yy;
            float nevAbs = sqrt(nevX * nevX + nevY * nevY);

            if(nevAbs < minNeviazka)
            {
                minNeviazka = nevAbs;
                target_IndicatorEB_MouseMove_Number_Temp = j;
            }
            //qDebug()<<"                               MouseMoveIndicatorEB: j:"<<j<<"nevAbs:"<<nevAbs<<"minNeviazka:"<<minNeviazka;
        }

        if(minNeviazka < 10.0)  //  пиксели
        {
            target_IndicatorEB_MouseMove_Number = target_IndicatorEB_MouseMove_Number_Temp;
        }
        else
            target_IndicatorEB_MouseMove_Number = -1;

        //ui->labelMouseMoveIndicatorEB->setText(QString("MouseMoveIndicatorEB: x: %1° y: %2° %3").arg(xx).arg(yy).arg(target_IndicatorEB_MouseMove_Number));
        //ui->labelMouseMoveIndicatorEB->setText(QString("MouseMoveIndicatorEB: nextTargetX: %1° xx: %2° ").arg(nextTargetX).arg(xx));
        //ui->labelAltitude->setText();
    }
#endif

    //  в режиме тренажа отождествление с номером списка имитированных целей
    if(target_IndicatorEB_MouseMove_Number > -1 && (console2User.apyInfo.rejimRabotiZRK == RejimRabotiZRK::TR_A ||
                                                    console2User.apyInfo.rejimRabotiZRK == RejimRabotiZRK::TR_K))
    {
        int simTrgt_IndEB_MouseMove_Number_Temp = - 1;
        float simMinNeviazka = 1000.0;

        int ys1 = otstyp_sverhyEB + HGT_H_EB / 2 + factorE_indicatorEB * (console2User.srvDrvInfo.currentDirectrisaElevationAngle - console2User.primaryPlots[target_IndicatorEB_MouseMove_Number].elevAngleF);
        int xs1 = startAzInt + factorB_indicatorEB * (console2User.primaryPlots[target_IndicatorEB_MouseMove_Number].azimuth360 - azLeftInt);

//        float azRad1 = console2User.primaryPlots[target_IndicatorEB_MouseMove_Number].azimuth360 / Rad2Grad;
//        float xs1 =  console2User.primaryPlots[target_IndicatorEB_MouseMove_Number].range_km * sin(azRad1);
//        float ys1 =  console2User.primaryPlots[target_IndicatorEB_MouseMove_Number].range_km * cos(azRad1);

        for(int i = 0; i < console2User.apyInfo.totalSimTargetNum; i++)
        {
            int ys2 = otstyp_sverhyEB + HGT_H_EB / 2 + factorE_indicatorEB * (console2User.srvDrvInfo.currentDirectrisaElevationAngle - console2User.apyInfo.nextTocka[i].elevationAngle);
            int xs2 = startAzInt + factorB_indicatorEB * (console2User.apyInfo.nextTocka[i].azimuth360 - azLeftInt);

            float nevX = xs2 - xs1;
            float nevY = ys2 - ys1;

            float nevAbs = sqrt(nevX * nevX + nevY * nevY);

            if(nevAbs < simMinNeviazka)
            {
                simMinNeviazka = nevAbs;
                simTrgt_IndEB_MouseMove_Number_Temp = i;
            }
        }

        if(simMinNeviazka < 6.0)
        {
            simTrgt_IndEB_MouseMove_Number = simTrgt_IndEB_MouseMove_Number_Temp;; // цель отождествлена
        }
        else
        {
            simTrgt_IndEB_MouseMove_Number = -1;                               // цель не отождествлена
        }
    }

//    yy = otstyp_sverhyEB + HGT_H_EB / 2 + factorE_indicatorEB * (console2User.srvDrvInfo.currentDirectrisaElevationAngle - mouseCursorElevAngle);
//    xx = startAzInt + factorB_indicatorEB * (mouseCursorAzimuth - azLeftInt);

    mouseCursorElevAngle = console2User.srvDrvInfo.currentDirectrisaElevationAngle - (yy - otstyp_sverhyEB - HGT_H_EB / 2 ) / factorE_indicatorEB;
    mouseCursorAzimuth = (xx - startAzInt) / factorB_indicatorEB + azLeftInt;

    //qDebug()<<"MouseMoveIndicatorEB: mouseCursorAzimuth:"<<mouseCursorAzimuth<<"mouseCursorElevAngle:"<<mouseCursorElevAngle;


#if 0   //по старому   (c   АРМ руководителя тренировки)
    float minNeviazka = 1000.0;

    int target_IndicatorEB_MouseMove_Number_Temp = -1;

    for(int j = console2User.apyInfo.totalSimTargetNum - 1; j >= 0; j--)
    {
        if(targetObsolTime[j] < 10.0) // ВРЕМЯ УСТАРЕВАНИЯ ФОРМУЛЯРА В СЕКУНДАХ
        {
            int nextTargetY = otstyp_sverhyEB + HGT_H_EB / 2 + factorE_indicatorEB * (console2User.srvDrvInfo.currentDirectrisaElevationAngle - console2User.apyInfo.nextTocka[j].elevationAngle);            
            int nextTargetX = startAzInt + factorB_indicatorEB * (console2User.apyInfo.nextTocka[j].azimuth360 - azLeftInt);

            float nevX = nextTargetX - xx;
            float nevY = nextTargetY - yy;
            float nevAbs = sqrt(nevX * nevX + nevY * nevY);

            if(nevAbs < minNeviazka)
            {
                minNeviazka = nevAbs;
                target_IndicatorEB_MouseMove_Number_Temp = j;
            }
            //qDebug()<<"                               MouseMoveIndicatorEB: j:"<<j<<"nevAbs:"<<nevAbs<<"minNeviazka:"<<minNeviazka;
        }

        if(minNeviazka < 10.0)  //  пиксели
        {
            target_IndicatorEB_MouseMove_Number = target_IndicatorEB_MouseMove_Number_Temp;
        }
        else
            target_IndicatorEB_MouseMove_Number = -1;

        //ui->labelMouseMoveIndicatorEB->setText(QString("MouseMoveIndicatorEB: x: %1° y: %2° %3").arg(xx).arg(yy).arg(target_IndicatorEB_MouseMove_Number));
        //ui->labelMouseMoveIndicatorEB->setText(QString("MouseMoveIndicatorEB: nextTargetX: %1° xx: %2° ").arg(nextTargetX).arg(xx));
        //ui->labelAltitude->setText();
    }
#endif
}

float cntrNetVibranihCelej = 0;

void MainWindow::MouseClickIndicatorEB()
{
    float dAz;

    if(target_IndicatorEB_MouseMove_Number > - 1)   // команда захватить на сопровождение "отождествлённую" цель
    {
        ui->pushButtonZhvt->setChecked(true);
        shMemU2C.guidanceOfficerCmds.rejimPoiskaSNR = RejimPoiskaSNR::POISK_PEREKLYCHEN_V_ZAHVAT;

        if(console2User.apyInfo.rejimRabotiZRK == RejimRabotiZRK::TR_A || console2User.apyInfo.rejimRabotiZRK == RejimRabotiZRK::TR_K)
        {
            shMemU2C.guidanceOfficerCmds.targetIndexCY = simTrgt_IndEB_MouseMove_Number;

            if(simTrgt_IndEB_MouseMove_Number > - 1 && simTrgt_IndEB_MouseMove_Number < 256)
                dAz = console2User.apyInfo.nextTocka[simTrgt_IndEB_MouseMove_Number].azimuth360 - console2User.srvDrvInfo.directrisaAzimuth;
            else
                qDebug()<<"***   MouseClickSlot: simTrgt_IndEB_MouseMove_Number:"<<simTrgt_IndEB_MouseMove_Number;
        }
        else
        {
            shMemU2C.guidanceOfficerCmds.targetIndexCY = target_IndicatorEB_MouseMove_Number;

            dAz = console2User.primaryPlots[target_IndicatorEB_MouseMove_Number].azimuth360 - console2User.srvDrvInfo.directrisaAzimuth;
        }

        cntrNetVibranihCelej = 0;
    }
    else    // команда изменить только директрису УНВ
    {
        cntrNetVibranihCelej = 2;

        dAz = mouseCursorAzimuth - console2User.srvDrvInfo.directrisaAzimuth;



        ui->indicatorEB->setCursor(Qt::WaitCursor);
    }

    if(dAz < -180.0)
        dAz += 360.0;

    if(dAz > 180.0)
        dAz -= 360.0;

    shMemU2C.guidanceOfficerCmds.servoDriveDelta[AZIMUTH] = convert360angle2PlusMinus180(dAz);
    shMemU2C.guidanceOfficerCmds.servoDriveExecute[AZIMUTH] = true;

    shMemU2C.guidanceOfficerCmds.servoDriveDelta[ELEVANG] =  mouseCursorElevAngle - console2User.srvDrvInfo.currentDirectrisaElevationAngle;
    shMemU2C.guidanceOfficerCmds.servoDriveExecute[ELEVANG] = true;

    //qDebug()<<"MouseClickIndicatorEB";

    if(target_IndicatorEB_MouseMove_Number > -1)
    {
        ui->pushButtonZhvt->setChecked(true);//ui->pushButtonServoDrive_Off->setChecked(true);
        shMemU2C.guidanceOfficerCmds.rejimPoiskaSNR = RejimPoiskaSNR::POISK_PEREKLYCHEN_V_ZAHVAT;//ui->pushButtonCY->setChecked(true);

        cntrNetVibranihCelej = 0;

        shMemU2C.guidanceOfficerCmds.targetIndexCY = target_IndicatorEB_MouseMove_Number;        
    }
    else
    {
        cntrNetVibranihCelej = 2;
    }
}

void MainWindow::drawFormularEB(QPainter *painter, int j)
{
    int deltaX = 12;
    int deltaY = 6;

    //по старому   (c   АРМ руководителя тренировки)
//    int targetScrY = otstyp_sverhyEB + HGT_H_EB / 2 + factorE_indicatorEB * (console2User.srvDrvInfo.currentDirectrisaElevationAngle - console2User.apyInfo.nextTocka[j].elevationAngle);
//    int targetScrX = startAzInt + factorB_indicatorEB * (console2User.apyInfo.nextTocka[j].azimuth360 - azLeftInt);
    //по новому   (c  М Д О - 1)
    int targetScrY = otstyp_sverhyEB + HGT_H_EB / 2 + factorE_indicatorEB * (console2User.srvDrvInfo.currentDirectrisaElevationAngle - console2User.primaryPlots[j].elevAngleF);
    int targetScrX = startAzInt + factorB_indicatorEB * (console2User.primaryPlots[j].azimuth360 - azLeftInt);

    painter->drawLine(targetScrX - deltaX, targetScrY - deltaY, targetScrX + deltaX, targetScrY - deltaY);
    painter->drawLine(targetScrX - deltaX, targetScrY + deltaY, targetScrX + deltaX, targetScrY + deltaY);
    painter->drawLine(targetScrX - deltaX, targetScrY - deltaY, targetScrX - deltaX, targetScrY + deltaY);
    painter->drawLine(targetScrX + deltaX, targetScrY - deltaY, targetScrX + deltaX, targetScrY + deltaY);

    int xText = targetScrX + 12;
    int yText = targetScrY;

    QFont font("Arial");//" "Helvetica" );
    font.setBold(false);
    font.setPointSize(10);
    font.setStyleStrategy(QFont::NoAntialias);

    drawTextHalo(painter, painter->pen(), font, xText, yText + 20, strI[j]);
    drawTextHalo(painter, painter->pen(), font, xText, yText + 35, strD[j]);
    //drawTextHalo(painter, pen, font, xText, yText, strE);    
}


void MainWindow::drawGridsEB(QPainter *painter)
{
    int echoBrightnessInd = ui->sliderGridBrightness->value();

    painter->setPen(QPen(gridColors[echoBrightnessInd], 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    QFont fontE("Arial");
    fontE.setPointSize(8);// 10 );
    fontE.setBold(true);
    //fontE.setStyleStrategy(QFont::NoAntialias);
    painter->setFont( fontE );

    QPen penDot0(gridColors[echoBrightnessInd], 1);

    QVector<qreal> dashes0;
    qreal space = 2;    
    dashes0 << 1 << space << space << space;//dashes0 << 1 << space << 3 << space << 9 << space  << 27 << space << 9 << space;
    penDot0.setDashPattern(dashes0);

    QPen penDot5(gridColors[echoBrightnessInd], 1);
    QVector<qreal> dashes5;
    dashes5 << 1 << 2;
    penDot5.setDashPattern(dashes5);

    painter->setPen(QPen(gridColors[echoBrightnessInd], 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

    float eAngle = console2User.srvDrvInfo.currentDirectrisaElevationAngle; //ui->dblSpnBxSimulationUNV_EAngle->value();
    int azTopInt = (int)(eAngle + sectorE_numDegrees / 2 + 1.0 + 0.5); // +0.5 для округления в большую сторону
    int startEInt = otstyp_sverhyEB + HGT_H_EB / 2 + factorE_indicatorEB * (eAngle - azTopInt);

    for(int j = 0; j < 13; j ++)
    {
        int y1 = startEInt + j * factorE_indicatorEB;

        if(y1 > otstyp_sverhyEB + HGT_H_EB + 22)
               continue;

       QString str = QString::number(sectorE_numDegrees / 2 + 1 + eAngle - j, 'f', 0) + QString::fromUtf8("°");

       QSize sz = painter->fontMetrics().size( Qt::TextSingleLine, str);

       painter->setPen(QPen(gridColors[echoBrightnessInd], 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

       painter->drawText(otstyp_slevaEB - 1 - sz.width(), y1 + sz.height() / 4, str);

       painter->drawLine(otstyp_slevaEB, y1, otstyp_slevaEB + maxNumTochekR, y1);
    }

    for(int j = -1; j <= sectorB_numDegrees; j ++)
    {
        int x1 = startAzInt + j * factorB_indicatorEB;

        if(x1 < otstyp_slevaEB - 1 || x1 > otstyp_slevaEB + MAXNUMTOCHEKFORPAINT_EB + 3)
           continue;

        int next5BValue = (azLeftInt + j + 360) % 360;


        if(next5BValue % 10 == 0)
        {
           QString str = QString::number(next5BValue, 'f', 0) + QString::fromUtf8("°");

           QSize sz = painter->fontMetrics().size( Qt::TextSingleLine, str);

           painter->setPen(QPen(gridColors[echoBrightnessInd], 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

           painter->drawText(x1 - sz.width() / 2, HGT_H_EB + otstyp_sverhyEB + sz.height() + 22, str);

           painter->drawLine(x1, otstyp_sverhyEB - 30, x1, HGT_H_EB + otstyp_sverhyEB + 22);
        }
    }

    //  Ц Е Н Т Р   Д И Р Е К Т Р И С Ы
    painter->setBrush(Qt::NoBrush);
    QPoint centerEB(otstyp_slevaEB + MAXNUMTOCHEKFORPAINT_EB / 2, HGT_H_EB / 2 + otstyp_sverhyEB);

    painter->setPen(QPen(QColor(0, 160, 255, 192), 1));//, Qt::DotLine, Qt::RoundCap, Qt::RoundJoin));
    painter->drawPoint(centerEB.x() - 4, centerEB.y());
    painter->drawPoint(centerEB.x() - 3, centerEB.y());
    painter->drawPoint(centerEB.x() + 3, centerEB.y());
    painter->drawPoint(centerEB.x() + 4, centerEB.y());

    painter->drawPoint(centerEB.x(), centerEB.y() - 4);
    painter->drawPoint(centerEB.x(), centerEB.y() - 3);
    painter->drawPoint(centerEB.x(), centerEB.y() + 4);
    painter->drawPoint(centerEB.x(), centerEB.y() + 3);

    //  Ф О Р М У Л Я Р Ы   Ц Е Л Е Й   по новому   (c  М Д О - 1)
    int rgValue = 128;

    painter->setPen(QPen(QColor(rgValue, rgValue, 0), 1, Qt::DotLine, Qt::RoundCap, Qt::RoundJoin));

    for(int j = 0; j < MAX_MDO1_PRIMARY_PLOTS_COUNT; j++)
    {
        if(console2User.primaryPlots[j].packageWeight == 0)
            break;

        if(j == target_IndicatorEB_MouseMove_Number)
            continue;

        strD[j] = QString::number(console2User.primaryPlots[j].range_km, 'f', 2) + QString::fromUtf8("км");
        strI[j] = QString::number(j + 1);

        drawFormularEB(painter, j);
    }

    if(target_IndicatorEB_MouseMove_Number > - 1)
    {
        rgValue = 255;
        painter->setPen(QPen(QColor(rgValue, rgValue, 0), 1, Qt::DotLine, Qt::RoundCap, Qt::RoundJoin));
        drawFormularEB(painter, target_IndicatorEB_MouseMove_Number);
    }

//    qDebug()<<"primaryPlots: r,az,elAng:"<<console2User.primaryPlots[0].range_km<<console2User.primaryPlots[0].azimuth360<<console2User.primaryPlots[0].elevAngleF
//                                         <<console2User.primaryPlots[1].range_km<<console2User.primaryPlots[1].azimuth360<<console2User.primaryPlots[1].elevAngleF;

    #if 0
    //  Ф О Р М У Л Я Р Ы   Ц Е Л Е Й   по старому   (c   АРМ руководителя тренировки)
    if(shMemU2C.guidanceOfficerCmds.rejimPoiskaSNR == RejimPoiskaSNR::POISK_BSP || shMemU2C.guidanceOfficerCmds.rejimPoiskaSNR == RejimPoiskaSNR::POISK_MSP)
    {
        for(int j =  console2User.apyInfo.totalSimTargetNum - 1; j >= 0; j --)
        {            
            if(fabs(console2User.srvDrvInfo.currentAzimuth - console2User.apyInfo.nextTocka[j].azimuth360) < 0.1 &&
                    console2User.apyInfo.nextTocka[j].elevationAngle > console2User.srvDrvInfo.currentDirectrisaElevationAngle - 5 &&  //      цель в пределах сектора сканирования по углу
                    console2User.apyInfo.nextTocka[j].elevationAngle < console2User.srvDrvInfo.currentDirectrisaElevationAngle + 5)    //      цель в пределах сектора сканирования по углу
            {
                targetObsolTime[j] = 0;

                //detectedTargetElevAngle[j] = console2User.apyInfo.nextTocka[j].elevationAngle;
                //detectedTargetAzimuth[j] = console2User.apyInfo.nextTocka[j].azimuth360;

                strD[j] = QString::number(console2User.apyInfo.nextTocka[j].groundRange_km, 'f', 2) + QString::fromUtf8("км");
                strB[j] = QString::number(console2User.apyInfo.nextTocka[j].seaLevelAltitude * 1000, 'f', 0) + QString::fromUtf8("м");    // .radarRelativeAltitude
                strI[j] = QString::number(console2User.apyInfo.nextTocka[j].targetNo);
                //QString strE = QString::number(arm->trgt[i]->Angle, 'f', 2) + QString::fromUtf8("°");
                //QString strPower = QString::number(arm->trgt[i]->Power, 'f', 2) + QString::fromUtf8("Pwr");
            }
            //else  //должно устаревать всегда -> переносим в таймер
              //  targetObsolTime[j] += 0.001 * drawGraphicsTimer->interval();
        }

        int rgValue = 128;
        // Ф О Р М У Л Я Р Ы   В С Е Х   Ц Е Л Е Й   К Р О М Е   В Ы Б Р А Н Н О Й
        for(int j = console2User.apyInfo.totalSimTargetNum - 1; j >= 0; j --)
        {
            if(target_IndicatorEB_MouseMove_Number != j && targetObsolTime[j] < 10.0) // ВРЕМЯ УСТАРЕВАНИЯ ФОРМУЛЯРА В СЕКУНДАХ
            {
                rgValue = 128 - targetObsolTime[j] * 10.0;

                if(target_IndicatorEB_MouseMove_Number > -1)
                    rgValue *= 0.67;

                painter->setPen(QPen(QColor(rgValue, rgValue, 0), 1, Qt::DotLine, Qt::RoundCap, Qt::RoundJoin));

                drawFormularEB(painter, j);
            }
        }
        // Ф О Р М У Л Я Р   В Ы Б Р А Н Н О Й   Ц Е Л И   Р И С У Е Т С Я   П О В Е Р Х   В С Е Х   О С Т А Л Ь Н Ы Х
        if(target_IndicatorEB_MouseMove_Number > - 1 && targetObsolTime[target_IndicatorEB_MouseMove_Number] < 10.0) // ВРЕМЯ УСТАРЕВАНИЯ ФОРМУЛЯРА В СЕКУНДАХ
        {
            rgValue = 255;
            painter->setPen(QPen(QColor(rgValue, rgValue, 0), 1, Qt::DotLine, Qt::RoundCap, Qt::RoundJoin));
            drawFormularEB(painter, target_IndicatorEB_MouseMove_Number);
        }
    }
    #endif
}

int oldAzScanAddr = -1;


void MainWindow::drawIndicatorEB()
{
    QPainter painter(&indicatorE);
    painter.setBrush(QBrush(Qt::black));
    painter.drawRect(0, 0, indicatorE.width(), indicatorE.height());

    //drawGridsEB(&painter);

    float azNazn = console2User.srvDrvInfo.currentAzimuth;
    if(console2User.apyInfo.rejimPoiskaSNR == RejimPoiskaSNR::POISK_BSP || console2User.apyInfo.rejimPoiskaSNR == RejimPoiskaSNR::POISK_MSP)
    {
        azNazn = console2User.srvDrvInfo.directrisaAzimuth;
    }

    azLeftInt = (int)(azNazn - sectorB_numDegrees / 2);
    startAzInt = otstyp_slevaEB + MAXNUMTOCHEKFORPAINT_EB / 2 - factorB_indicatorEB * (azNazn - azLeftInt);

    if( (shMemU2C.guidanceOfficerCmds.rejimPoiskaSNR == RejimPoiskaSNR::POISK_BSP ||
         shMemU2C.guidanceOfficerCmds.rejimPoiskaSNR == RejimPoiskaSNR::POISK_MSP ||
         shMemU2C.guidanceOfficerCmds.rejimPoiskaSNR == RejimPoiskaSNR::POISK_VIKLUCHEN ) &&
        (console2User.apyInfo.rejimPoiskaSNR != RejimPoiskaSNR::POISK_BSP && console2User.apyInfo.rejimPoiskaSNR != RejimPoiskaSNR::POISK_MSP))
    {
        memset(yarkostEB, 0, MAXNUMTOCHEKFORPAINT_EB * HGT_H_EB * sizeof(double));
        memset(imgDataEB, 0, MAXNUMTOCHEKFORPAINT_EB * HGT_H_EB * 3);
    }

    //ui->mainIndicator->setPixmap(indicatorE);

    //if(false)
    if(ui->pushButtonEcho->isChecked())
    {
        float postLightFactor = 0.97;

        for(int j = 0; j < HGT_H_EB; j ++)                 // ПОСЛЕСВЕЧЕНИЕ
        {
            for(int i = 0; i < MAXNUMTOCHEKFORPAINT_EB; i ++)
            {
                *(yarkostEB + j * MAXNUMTOCHEKFORPAINT_EB + i) *= postLightFactor;
                *(imgDataEB + (j * MAXNUMTOCHEKFORPAINT_EB + i) * 3 + 1) =  (unsigned char)(*(yarkostEB + j * MAXNUMTOCHEKFORPAINT_EB + i));
            }
        }

        if(shMemU2C.guidanceOfficerCmds.rejimPoiskaSNR == RejimPoiskaSNR::POISK_BSP || shMemU2C.guidanceOfficerCmds.rejimPoiskaSNR == RejimPoiskaSNR::POISK_MSP)
        {
            int azScanAddr = MAXNUMTOCHEKFORPAINT_EB / 2 - factorB_indicatorEB * (convert360angle2PlusMinus180(console2User.srvDrvInfo.directrisaAzimuth - console2User.srvDrvInfo.currentAzimuth));

            //qDebug()<<"drawIndicatorEB: directrissa:"<<shMemU2C.userCmds.directrissaAzimuth<<"srvDrvInfo.currentAzimuth:"<<srvDrvInfo.currentAzimuth<<"azScanAddr:"<<azScanAddr;

            if(azScanAddr < 0)
                azScanAddr = 0;
            if(azScanAddr >= MAXNUMTOCHEKFORPAINT_EB)
                azScanAddr = MAXNUMTOCHEKFORPAINT_EB - 1;


            for(int e = 0; e < HGT_H_EB; e ++)  //  ПРОРИСОВКА ПСЕВДОРАЗВЁРТОК
            {
                float znach = 32.0;

                *(yarkostEB + e * MAXNUMTOCHEKFORPAINT_EB + azScanAddr) = znach;
                *(imgDataEB + (e * MAXNUMTOCHEKFORPAINT_EB + azScanAddr) * 3 + 1) = (unsigned char)(*(yarkostEB + e * MAXNUMTOCHEKFORPAINT_EB + azScanAddr));
            }


            // МДО-1:   ПРОРИСОВКА ВЕРТИКАЛЬНЫХ СЕЧЕНИЙ ДУЖЕК ЦЕЛЕЙ

            for(int j = 0; j < MAX_MDO1_TARGETS_COUNT; j++)
            {
                //if(console2User.apyInfo.nextTocka[j].elevationAngle < console2User.srvDrvInfo.currentDirectrisaElevationAngle - 5 || console2User.apyInfo.nextTocka[j].elevationAngle > console2User.srvDrvInfo.currentDirectrisaElevationAngle + 5)
                  //  continue;   //      цель за пределами сектора сканирования по углу

                //int trgtUsred;// = HGT_H_EB / 2 - factorE_indicatorEB * (console2User.joinedTrgtData[j].sredEnF[0]..apyInfo.nextTocka[j].elevationAngle - console2User.srvDrvInfo.currentDirectrisaElevationAngle);

                if(console2User.joinedTrgtData[j].range == 0)
                    break;

                int trgtUsred = HGT_H_EB / 2 - factorE_indicatorEB * (console2User.joinedTrgtData[j].elevAngleF - console2User.srvDrvInfo.currentDirectrisaElevationAngle);

                int eMin = trgtUsred - 1;
                if(eMin < 0)
                    eMin = 0;
                int eMax = trgtUsred + 2;
                if(eMax > HGT_H_EB)
                    eMax = HGT_H_EB;

                float znach = 255 * console2User.joinedTrgtData[j].sredEnF;

                //qDebug()<<"drawIndicatorEB: trgtUsred:"<<trgtUsred<<"znach:"<<znach;

                for(int e = eMin; e < eMax; e ++)
                {
                    *(yarkostEB + e * MAXNUMTOCHEKFORPAINT_EB + azScanAddr) = znach;
                    *(imgDataEB + (e * MAXNUMTOCHEKFORPAINT_EB + azScanAddr) * 3 + 1) = (unsigned char)(*(yarkostEB + e * MAXNUMTOCHEKFORPAINT_EB + azScanAddr));
                }

            }
            oldAzScanAddr = azScanAddr;

        }

        QPixmap pixmapEcho = QPixmap::fromImage(QImage((unsigned char *) imgDataEB, MAXNUMTOCHEKFORPAINT_EB, HGT_H_EB, QImage::Format_RGB888));//QImage::Format_RGB32));//
        painter.drawPixmap(otstyp_slevaEB, otstyp_sverhyEB, pixmapEcho);
    }//if(ui->CheckBoxEcho->isChecked())

    //qDebug()<<"drawIndicatorEB: shMemC2U.komKom.showSimulationTrack:"<<shMemC2U.komKom.showSimulationTrack;

//    qDebug()<<"drawIndicatorEB: monoTrack[][].elevationAngle:"<<monoTrack[0][trackPointsCount - 1].elevationAngle<<monoTrack[1][trackPointsCount - 1].elevationAngle
//            <<"   nextTocka:"<<console2User.apyInfo.nextTocka[0].elevationAngle<<console2User.apyInfo.nextTocka[1].elevationAngle
//            <<"   joinedTrgtData[].elevAngleF:"<<console2User.joinedTrgtData[0].elevAngleF<<console2User.joinedTrgtData[1].elevAngleF<<console2User.joinedTrgtData[2].elevAngleF<<console2User.joinedTrgtData[3].elevAngleF;

    if(console2User.apyInfo.showSimulationTrack)  //  О Т Р И С О В К А   И С Т И Н Н Ы Х   Т Р А С С
    {
        for(int j = 0; j < console2User.apyInfo.totalSimTargetNum; j++)
        {
            for(int i = 0; i < trackPointsCount; i++)
            {
                int y1 = otstyp_sverhyEB + HGT_H_EB / 2 + factorE_indicatorEB * (console2User.srvDrvInfo.currentDirectrisaElevationAngle - monoTrack[j][i].elevationAngle);

                int x1 = startAzInt + factorB_indicatorEB * (monoTrack[j][i].azimuth360 - azLeftInt);

                if(i == trackPointsCount - 1)
                    painter.fillRect(x1 - 1, y1 - 1, 3, 3, QColor::fromRgbF(redFi2, 0.8, bluFi2, 0.5));
                else
                    painter.fillRect(x1 - 1, y1 - 1, 2, 2, QColor::fromRgbF(redFi2 / 2, 0.4, bluFi2 / 2, 0.5));
            }
        }
    }

    if(console2User.apyInfo.showSimulationTarget)  //  П Р О Р И С О В К А   Т О Л Ь К О   Т Е К У Щ И Х   К О О Р Д И Н А Т   Ц Е Л Е Й
    {
        for(int j = 0; j < console2User.apyInfo.totalSimTargetNum; j++)
        {
            int y1 = otstyp_sverhyEB + HGT_H_EB / 2 + factorE_indicatorEB * (console2User.srvDrvInfo.currentDirectrisaElevationAngle - monoTrack[j][trackPointsCount - 1].elevationAngle);

            int x1 = startAzInt + factorB_indicatorEB * (monoTrack[j][trackPointsCount - 1].azimuth360 - azLeftInt);

            painter.fillRect(x1 - 1, y1 - 1, 3, 3, QColor::fromRgbF(redFi2, 0.8, bluFi2, 0.5));
        }
    }


    // *** GRIDS & PLOTS *** GRIDS & PLOTS *** GRIDS & PLOTS *** GRIDS & PLOTS *** GRIDS & PLOTS ***
    drawGridsEB(&painter);

    if(cntrNetVibranihCelej > 0.25)  // ОФИЦЕР НАВЕДЕНИЯ НЕ ПОПАЛ ТРЕКБОЛОМ В ФОРМУЛЯР ЦЕЛИ
    {
        QFont fnt = painter.font();
        fnt.setPointSize(10);
        painter.setFont(fnt);
        //painter.setFont(painter.font().setPointSizeF(20);

        cntrNetVibranihCelej -= 0.001 * drawGraphicsTimer->interval();        

        painter.setPen(QColor::fromRgbF(1.0, 0.25, 0.25, cntrNetVibranihCelej / 2.0));

        painter.drawText(10, 30, QString::fromUtf8("Цель не выбрана"));

        if(cntrNetVibranihCelej < 1.0)
            ui->indicatorEB->setCursor(Qt::ArrowCursor);
    }

    ui->indicatorEB->setPixmap(indicatorE);
}

//void MainWindow::MouseDoubleClickSlot_indicatorEB(int scrX, int scrY)
//{
//    qDebug()<<"MouseDoubleClickSlot_indicatorEB";
//    float azTemp;
//    if(shMemU2C.guidanceOfficerCmds.unvScanIsOn)
//        azTemp = shMemU2C.guidanceOfficerCmds.directrissaAzimuth;
//    else
//        azTemp = shMemC2U.srvDrvInfo.currentAzimuth;

//    float trgtB = (scrX - otstyp_slevaEB - MAXNUMTOCHEKFORPAINT_EB / 2) / factorB_indicatorEB + azTemp;
//    if(trgtB < 0)
//        trgtB += 360.0;

//    float trgtE = (otstyp_sverhyEB + HGT_H_EB / 2 - scrY) / factorE_indicatorEB + shMemC2U.srvDrvInfo.currentDirectrisaElevationAngle;

//    if(shMemU2C.guidanceOfficerCmds.unvScanIsOn)
//    {
//        shMemU2C.guidanceOfficerCmds.directrissaAzimuth = trgtB;
//        //shMemU2C.guidanceOfficerCmds.simulatorTargetIndexCY =
//        ui->pushButtonServoDrive_Off->setChecked(true);
//        ui->pushButtonZhvt->setChecked(true);
//    }
//    else
//    {
//        shMemU2C.guidanceOfficerCmds.deltaAzim = trgtB - shMemC2U.srvDrvInfo.currentAzimuth;
//        if(shMemU2C.guidanceOfficerCmds.deltaAzim > 180.0)
//            shMemU2C.guidanceOfficerCmds.deltaAzim -= 360.0;

//        shMemU2C.guidanceOfficerCmds.executeNewAzManual = true;
//    }

//    shMemU2C.guidanceOfficerCmds.deltaElevationAngle = trgtE - shMemC2U.srvDrvInfo.currentDirectrisaElevationAngle;
//    shMemU2C.guidanceOfficerCmds.executeNewElevationAngleManual = true;


//    ui->labelPorog->setText(QString("β: %1° scrX: %2° ε: %3°  scrY: %4").arg(trgtB).arg(scrX).arg(trgtE).arg(scrY));
//}

