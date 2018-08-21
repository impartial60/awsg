#include "mainWindowG.h"
#include "ui_mainwindow.h"

int cntr2 = 0;
int cntr3 = 0;
int cntr5 = 0;
int cntr6 = 0;

int cntrWrongLength = 0;
int correctLength = 0;

int propyskDataCntr = 0;

QString rsltMDO1;

extern int setIndicatorColor(bool enabled, int indicatorType, unsigned char stateIndex);
extern QString crc16QStringResult(unsigned char *pcBlock, unsigned int len, unsigned short int crc16Value);


void MainWindow::diagnosticParser(int n)
{
    QString rsltStr = "<!DOCTYPE html><html><head> <style> p {line-height: 20%; white-space: pre-wrap;}  p.ex { margin-top: 18px; }</style></head><body>";

    cntr5++;

    if(n != sizeof(USM_Info))
        cntrWrongLength ++;
    else
        correctLength ++;

    rsltStr += QString("<p>цикл обмена: %1</p><p>сообщений с прав.длиной %2 [%3\%]</p><p>сообщений с непр.длиной %4 [%5\%]</p>")
            .arg(cntr5).arg(correctLength).arg(QString::number(100.0 * correctLength / cntr5, 'f', 3)).arg(cntrWrongLength).arg(QString::number(100.0 * cntrWrongLength / cntr5, 'f', 3));

    //rsltStr += QString("<p>totalSentBytes: %1   totalReadBytes: %2</p>").arg(console2User.srvDrvInfo.totalSentBytes).arg(console2User.srvDrvInfo.totalReadBytes);
    rsltStr += "</body></html>";
    ui->labelParser->setText(rsltStr);

//    if(n <= 0)
//    {
//        //////////////////ui->frameMain->setEnabled(false);
//        return;
//    }
    //ui->frameMain->setEnabled(true);

    bool synhroEnabled = true;

    if(console2User.usmInfo.sync[3] == 0x2D && console2User.usmInfo.sync[4] == 0x2D && console2User.usmInfo.sync[5] == 0x2D)
        synhroEnabled = false;

    ui->groupBoxSynhro->setEnabled(synhroEnabled); // ui->frameSinhro->setEnabled(synhroEnabled);

    if(synhroEnabled)
    {        

    //    rsltStr += QString("<p>Синхронизатор    0x%1 0x%2 0x%3 </p>").arg(array2Read[startAddr + 0], 2, 16, QChar('0')).toUpper() //  Первый байт маркера заголовка   0x55
    //                                                  .arg(array2Read[startAddr + 1], 2, 16, QChar('0')).toUpper() //  Второй байт маркера заголовка   0xAA
    //                                                  .arg(array2Read[startAddr + 2], 2, 16, QChar('0')).toUpper();//  Адрес отвечающего устройства    0x3

        ui->label_nprG1->setPixmap(labelPixmap[setIndicatorColor(synhroEnabled, IndicatorIspravnost,  ((console2User.usmInfo.sync[7] & 0x01) + 0x01) % 2)]);
        ui->label_nprG2->setPixmap(labelPixmap[setIndicatorColor(synhroEnabled, IndicatorIspravnost,  ((((console2User.usmInfo.sync[7]) & 0x02) >> 1) + 0x01) % 2)]);
        ui->label_nprOG->setPixmap(labelPixmap[setIndicatorColor(synhroEnabled, IndicatorIspravnost,  ((((console2User.usmInfo.sync[7]) & 0x04) >> 2) + 0x01) % 2)]);
        ui->label_nprChM->setPixmap(labelPixmap[setIndicatorColor(synhroEnabled, IndicatorIspravnost, ((((console2User.usmInfo.sync[7]) & 0x08) >> 3) + 0x01) % 2)]);
        ui->label_nprLChM->setPixmap(labelPixmap[setIndicatorColor(synhroEnabled, IndicatorIspravnost,((((console2User.usmInfo.sync[7]) & 0x10) >> 4) + 0x01) % 2)]);
    }

    //rsltStr += QString("<p>Оклик</p>");

//    rsltStr += "</body></html>";

//    ui->labelParser->setText(rsltStr);      //qDebug()<<rsltStr;

    ///////////////////////////////////////drawOscilloscopeFischuk(0);

    ///////////////////////////////////////drawIndicatorRV_Fischuk();

    // М Д О - 1     М Д О - 1     М Д О - 1     М Д О - 1     М Д О - 1     М Д О - 1     М Д О - 1     М Д О - 1     М Д О - 1     М Д О - 1

//    QString str2 = QString("   0x%1 0x%2 0x%3").arg(array2Read[startAddr + 0], 2, 16, QChar('0')).toUpper()
//                                            .arg(array2Read[startAddr + 1], 2, 16, QChar('0')).toUpper()
//                                            .arg(array2Read[startAddr + 2], 2, 16, QChar('0')).toUpper();

//    qDebug()<<"diagnosticParser: n:"<<n<<str2;

    //qDebug()<<"diagnosticParser: "<<console2User.mdo1Info[0].header.x55;

    //bool correctData = false;
    if(console2User.mdo1Info[0].header.x55 == 0x55 && console2User.mdo1Info[0].header.xAA == 0xAA && console2User.mdo1Info[0].header.deviceAddress == 0x21)
        propyskDataCntr = 0;
    else
        propyskDataCntr++;

    //if(propyskDataCntr < 3)
      //  correctData = true;

    if(propyskDataCntr == 0)
    {
        rsltMDO1 = "<!DOCTYPE html><html><head> <style> p {line-height: 20%; white-space: pre-wrap;}  p.ex { margin-top: 18px; }</style></head><body>";

        for(int j = 0; j < 3; j++)
        {
            rsltMDO1 += QString("<p>\t  МДО-1_%1   ").arg(QString::number(j + 1));

            rsltMDO1 += QString("0x%1 0x%2 0x%3     ").arg(console2User.mdo1Info[j].header.x55,             2, 16, QChar('0')).toUpper()  //  Первый байт маркера заголовка   0x55
                                                      .arg(console2User.mdo1Info[j].header.xAA,             2, 16, QChar('0')).toUpper()  //  Второй байт маркера заголовка   0xAA
                                                      .arg(console2User.mdo1Info[j].header.deviceAddress,   2, 16, QChar('0')).toUpper(); //  Адрес отвечающего устройства    0x21

            rsltMDO1 += QString("0x%1 0x%2 0x%3     ").arg(console2User.mdo1Info[j].timeFrame.lowByte,    2, 16, QChar('0')).toUpper()    //  Мл.байт 23–битного номера 10мс интервала от начала раб.сессии
                                                      .arg(console2User.mdo1Info[j].timeFrame.middleByte, 2, 16, QChar('0')).toUpper()    //  Ср.байт 23–битного номера 10мс интервала от начала раб.сессии
                                                      .arg(console2User.mdo1Info[j].timeFrame.highByte,   2, 16, QChar('0')).toUpper();   //  Ст.байт 23–битного номера 10мс интервала от начала раб.сессии

            rsltMDO1 += "     CRC: " +  crc16QStringResult(&console2User.mdo1Info[j].header.deviceAddress, 180, console2User.mdo1Info[j].CRC16[0] + 256 * console2User.mdo1Info[j].CRC16[1]) + "</p>";

        //    qDebug()<<"diagnosticParser: array2Read:"<<array2Read[startAddr + 266 + 8]<<array2Read[startAddr + 266 + 9]<<array2Read[startAddr + 266 + 10]<<array2Read[startAddr + 266 + 11]<<"    "
        //              <<array2Read[startAddr + 532 + 8]<<array2Read[startAddr + 532 + 9]<<array2Read[startAddr + 532 + 10]<<array2Read[startAddr + 532 + 11];

//            for(int i = 0; i < 16; i++)
//            {
//                rsltMDO1 += QString("<p>N<span style=\"vertical-align:sub;\">Ц</span>:0x%1  ").arg(console2User.mdo1Info[j].targetData[i].Nceli, 2, 16, QChar('0'));
//                rsltMDO1 += QString("Д<span style=\"vertical-align:sub;\">Ц</span>: 0x%1 0x%2  ").arg(console2User.mdo1Info[j].targetData[i].range[0], 2, 16, QChar('0')).arg(console2User.mdo1Info[j].targetData[i].range[1], 2, 16, QChar('0'));
//                rsltMDO1 += QString("f<span style=\"vertical-align:sub;\">Д</span>: 0x%1  ").arg(console2User.mdo1Info[j].targetData[i].doppler, 2, 16, QChar('0'));
//                rsltMDO1 += QString("ε<span style=\"vertical-align:sub;\">УВ-10</span>: 0x%1 0x%2  ").arg(console2User.mdo1Info[j].targetData[i].elevationYV10[0], 2, 16, QChar('0')).arg(console2User.mdo1Info[j].targetData[i].elevationYV10[1], 2, 16, QChar('0'));
//                rsltMDO1 += QString("ε<span style=\"vertical-align:sub;\">Ф1</span>: 0x%1 0x%2  ").arg(console2User.mdo1Info[j].targetData[i].elevationF1[0], 2, 16, QChar('0')).arg(console2User.mdo1Info[j].targetData[i].elevationF1[1], 2, 16, QChar('0'));
//                rsltMDO1 += QString("ε<span style=\"vertical-align:sub;\">Ф2</span>: 0x%1 0x%2  ").arg(console2User.mdo1Info[j].targetData[i].elevationF2[0], 2, 16, QChar('0')).arg(console2User.mdo1Info[j].targetData[i].elevationF2[1], 2, 16, QChar('0'));
//                rsltMDO1 += QString("К<span style=\"vertical-align:sub;\">АРУ</span>: 0x%1</p>").arg(console2User.mdo1Info[j].targetData[i].ARY, 2, 16, QChar('0'));
//            }
            for(int i = 0; i < 16; i++)
            {
                rsltMDO1 += QString("<p>N<span style=\"vertical-align:sub;\">Ц</span>:0x%1  ").arg(console2User.mdo1Info[j].rawTrgtData[i].Nceli, 2, 16, QChar('0'));
                rsltMDO1 += QString("Д<span style=\"vertical-align:sub;\">Ц</span>: 0x%1  ").arg(console2User.mdo1Info[j].rawTrgtData[i].range, 2, 16, QChar('0'));
                rsltMDO1 += QString("ε<span style=\"vertical-align:sub;\">УВ-10</span>: 0x%1  ").arg(console2User.mdo1Info[j].rawTrgtData[i].scanerYV10ElevAngle, 2, 16, QChar('0'));
                rsltMDO1 += QString("К<span style=\"vertical-align:sub;\">АРУ</span>: 0x%1</p>").arg(console2User.mdo1Info[j].rawTrgtData[i].ARY, 2, 16, QChar('0'));
            }
        }
        rsltMDO1 += "</body></html>";

        ui->labelTargetCharacteristicsG->setText(rsltMDO1);
        ui->labelTargetCharacteristicsG->setEnabled(true);
    }
    else
    {
        ui->labelTargetCharacteristicsG->setEnabled(false);
    }

//    if(correctData)//n == INPUT_TOTAL_LENGTH)
//    {
//    }
//    else
//    {
//      //  ui->labelTargetCharacteristics->setText("");
//        ui->labelTargetCharacteristics->setEnabled(false);
//    }
}


void MainWindow::ironHandWheelOptions()
{
    int newHandWheelPosition[3];

    newHandWheelPosition[AZIMUTH] = handwheel_betta->getPosition();
    newHandWheelPosition[ELEVANG] = handwheel_epsilon->getPosition();
    newHandWheelPosition[THRESHOLD] = handwheel_threshold->getPosition();

    //  регулятор порога
    int deltaPos = newHandWheelPosition[THRESHOLD] - oldHandWheelPosition[THRESHOLD];
    if(fabs(deltaPos) > 0 && fabs(deltaPos) < 1000000 && shMemU2C.guidanceOfficerCmds.servoDriveExecute[THRESHOLD] == false)
    {
        ui->sliderPorog->setValue(ui->sliderPorog->value() + 0.0001 * deltaPos);
    }

    for(int i = 0; i < 2; i++)  //  штурвалы азимута и угла места
    {
        deltaPos = newHandWheelPosition[i] - oldHandWheelPosition[i];

        if(fabs(deltaPos) > 0 && fabs(deltaPos) < 1000000 && shMemU2C.guidanceOfficerCmds.servoDriveExecute[i] == false)
        {
            shMemU2C.guidanceOfficerCmds.servoDriveDelta[i] = 0.001 * deltaPos;
            shMemU2C.guidanceOfficerCmds.servoDriveExecute[i] = true;
        }

        //  2018_06_25  ui->labelHandWheel->setText(QString("d: %1  p: %2 (%3)").arg(handwheel_betta->getDelta()).arg(newHandWheelPosition[AZIMUTH]).arg(deltaPos));//->getPosition());

        oldHandWheelPosition[i] = newHandWheelPosition[i];
    }
}

int klmTransactionId_old = 0;

void MainWindow::nextTochkaOptions()
{    
    if(oldRejimRabotiZRK != console2User.apyInfo.rejimRabotiZRK)    // при перезапуске тренировки траектория начинается с нуля
    {
        trackPointsCount = 0;
    }

    if(console2User.apyInfo.klmTransactionId > klmTransactionId_old)
    {
        klmTransactionId_old = console2User.apyInfo.klmTransactionId;

        if(trackPointsCount >= MONOTRACKSIZE)
            trackPointsCount = 0;

//        if(trackPointsCount > 1 && fabs(monoTrack[0][trackPointsCount - 2].groundRange_km - monoTrack[0][trackPointsCount - 1].groundRange_km) > 1.0)     // обнаружитель перезапуска траектории
//            trackPointsCount = 0;

        for(int j = 0; j < console2User.apyInfo.totalSimTargetNum; j++)
        {
            memcpy(&monoTrack[j][trackPointsCount], &console2User.apyInfo.nextTocka[j], sizeof(TargetParams));
        }

        trackPointsCount++;

        //memcpy(&monoTrack[trackPointsCount++], &shMemC2U.apyInfo.nextTocka, sizeof(TargetParams));

//       qDebug()<<"nextTochkaOptions: shMemC2U.srvDrvInfo.trackPointsNumber:"<<shMemC2U.srvDrvInfo.trackPointsNumber
//              <<"az:"<<monoTrack[shMemC2U.srvDrvInfo.trackPointsNumber].azimuth180<<"e:"<<monoTrack[shMemC2U.srvDrvInfo.trackPointsNumber].elevationAngle;
    }

    /*

    QString resStr = "<!DOCTYPE html><html><head> <style> p {line-height: 20%; white-space: pre-wrap;}  p.ex { margin-top: 18px; }</style></head><body>";
    //resStr += QString("<p>β</p>");

    //resStr += QString("<p>β<span style=\"vertical-align:sub;\">ц</span>:%1</p>").arg(shMemC2U.apyInfo.nextTocka.azimuth180, 3, 10, QChar('0'));

    resStr += QString("<p>β<span style=\"vertical-align:sub;\">ц</span>: ") + QString::number(shMemC2U.apyInfo.nextTocka.azimuth360, 'f', 2) + QString::fromUtf8("°  ");
    //resStr += QString("<p>β<span style=\"vertical-align:sub;\">унв</span>: ") + QString::number(shMemC2U.srvDrvInfo.currentAzimuth, 'f', 2) + QString::fromUtf8("°</p>");

    resStr += QString("ε<span style=\"vertical-align:sub;\">ц</span>: ") + QString::number(shMemC2U.apyInfo.nextTocka.elevationAngle, 'f', 2) + QString::fromUtf8("°</p>");
    resStr += QString("<p>H<span style=\"vertical-align:sub;\">ц</span>: ") + QString::number(shMemC2U.apyInfo.nextTocka.seaLevelAltitude, 'f', 3) + QString::fromUtf8(" km   ");
    resStr += QString("V<span style=\"vertical-align:sub;\">ц</span>: ") + QString::number(shMemC2U.apyInfo.nextTocka.groundSpeed, 'f', 1) + QString::fromUtf8(" м/с</p>");

    //resStr += QString("<p>x: ") + QString::number(shMemC2U.apyInfo.nextTocka.groundX, 'f', 3) + " km   ";//QString("<span style=\"vertical-align:sub;\"> km</span>");
    //resStr += QString("y: ") + QString::number(shMemC2U.apyInfo.nextTocka.groundY, 'f', 3) + " km</p>";
    resStr += QString("<p>Курс: ") + QString::number(shMemC2U.apyInfo.nextTocka.kyrs, 'f', 3) + QString::fromUtf8("°");
    resStr += QString("   Д<span style=\"vertical-align:sub;\">гор</span>: ") + QString::number(shMemC2U.apyInfo.nextTocka.groundRange_km, 'f', 3) + QString::fromUtf8(" km</p>");
    resStr += QString("<p>Д<span style=\"vertical-align:sub;\">накл</span>: ") + QString::number(shMemC2U.apyInfo.nextTocka.slantRange_km, 'f', 3) + QString::fromUtf8(" km   ");
    resStr += QString("Д<span style=\"vertical-align:sub;\">делта</span>: ") + QString::number(shMemC2U.apyInfo.nextTocka.slantRange_km - shMemC2U.apyInfo.nextTocka.groundRange_km, 'f', 3) + QString::fromUtf8(" km</p>");

    resStr += QString("<p> </p><p>β<span style=\"vertical-align:sub;\">УНВ</span>: ") + QString::number(shMemC2U.srvDrvInfo.currentAzimuth, 'f', 1) + QString::fromUtf8("°   ");
    resStr += QString("ε<span style=\"vertical-align:sub;\">УНВ</span>: ") + QString::number(shMemC2U.srvDrvInfo.currentDirectrisaElevationAngle, 'f', 1) + QString::fromUtf8("°</p>");
    resStr += QString("<p>trackPointsNumber: ") + QString::number(shMemC2U.srvDrvInfo.trackPointsNumber) + "</p>";

    resStr += QString("<p>Id<span style=\"vertical-align:sub;\">Арт</span>: ") + QString::number(shMemC2U.srvDrvInfo.artSimData.transactionId);
    resStr += QString("  N<span style=\"vertical-align:sub;\">Арт</span>: ") + QString::number(shMemC2U.srvDrvInfo.artSimData.numTarget) + "</p>";
    resStr += QString("<p>R<span style=\"vertical-align:sub;\">Арт</span>: ") + QString::number(shMemC2U.srvDrvInfo.artSimData.distance) + " км";
    resStr += QString("   H<span style=\"vertical-align:sub;\">Арт</span>: ") + QString::number(1000.0 * shMemC2U.srvDrvInfo.artSimData.height) + " м</p>";
    resStr += QString("<p>β<span style=\"vertical-align:sub;\">Арт</span>: ") + QString::number(shMemC2U.srvDrvInfo.artSimData.azimuth) + "°</p>";
    resStr += QString("<p>V<span style=\"vertical-align:sub;\">Арт</span>: ") + QString::number(shMemC2U.srvDrvInfo.artSimData.speed) + " м/с</p>";
    resStr += QString("<p>K<span style=\"vertical-align:sub;\">Арт</span>: ") + QString::number(shMemC2U.srvDrvInfo.artSimData.kyrs) + "°</p>";
    //resStr += QString("<p>Отработать ЦУ<span style=\"vertical-align:sub;\">Арт</span>: ") + QString::number(shMemC2U.srvDrvInfo.artSimData.otrabCY) + "</p>";

    //resStr += QString("<p>Время<span style=\"vertical-align:sub;\">командира</span>: ") + shMemC2U.srvDrvInfo.bond2AWSG_27.sendStrTime.toString("hh:mm:ss.zzz") + "</p>";

    resStr += QString("<p>l<span style=\"vertical-align:sub;\">LPH</span>: ") + QString::number(shMemC2U.apyInfo.nextTocka.soprovojdCoord.l) + "</p>";
    resStr += QString("<p>p<span style=\"vertical-align:sub;\">LPH</span>: ") + QString::number(shMemC2U.apyInfo.nextTocka.soprovojdCoord.p) + "</p>";
    resStr += QString("<p>h<span style=\"vertical-align:sub;\">LPH</span>: ") + QString::number(shMemC2U.apyInfo.nextTocka.soprovojdCoord.h) + "</p>";

    QString bitsStr = "";

    for(int i = 0; i < 42; i++)
    {
        //shMemC2U.srvDrvInfo.new_state_G.Buttons.KeyMap

                //bit = (number >> x) & 1U;
                int bit = (shMemC2U.srvDrvInfo.new_state_G.Buttons.KeyMap >> i) & 1UL;

//                if(bit == 0)
//                    bitsStr += "0 ";
//                else
                bitsStr += QString::number(bit);// + " ";
    }

    //IronBtns:

    resStr += QString("<p>") + bitsStr + "</p>";

    resStr += "</body></html>";

    ui->labelNextTochka->setText(resStr);
    */

}


