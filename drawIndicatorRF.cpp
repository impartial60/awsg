#include <QtGui>
#include "mainWindowG.h"
#include "ui_mainwindow.h"

int sectorF_numDegrees = 15;
int sectorR_numKm = 110;

QRect fi1Rect(243, 30, WDT_NAVED_RF, HGT_NAVED_RF);
QRect fi2Rect(612, 30, WDT_NAVED_RF, HGT_NAVED_RF);

float factorR = 1.0 / sectorR_numKm * HGT_NAVED_RF;
float factorF = 1.0 / sectorF_numDegrees * WDT_NAVED_RF;

extern float convert360angle2PlusMinus180(float dltAz360);

void drawScaleRGridsRF(QPainter *painter, QRect fiRect)
{
    for(int j = 0; j <= sectorR_numKm; j ++)
    {
        int y1 = fiRect.top() + fiRect.height() - factorR * j;

        if(j % 5 == 0)
        {
           if(j % 10 == 0)
           {
               QString str = QString::number(j, 'f', 0) + QString::fromUtf8(" км");
               if(j == 0)
                   str = "0";

               QSize sz = painter->fontMetrics().size( Qt::TextSingleLine, str);
               painter->drawText(fiRect.left() - 6 - sz.width(), y1 + sz.height() / 4, str);
           }
           painter->drawLine(fiRect.left() - 5, y1, fiRect.left(), y1);
        }
        else
        {
            //painter->drawLine(fiRect.left() - 3, y1, fiRect.left(), y1);
        }
    }
}


void MainWindow::drawGridsRF(QPainter *painter)
{
    QFont fontE("Arial");
    fontE.setPointSize(7);// 10 );
    fontE.setBold(true);
    //fontE.setStyleStrategy(QFont::NoAntialias);
    painter->setFont( fontE );

    //fi1
    painter->setPen(QPen(QColor::fromRgbF(redFi1 / 2, 0.4, bluFi1 / 2, 0.9), 1));
    painter->drawRect(fi1Rect);

    int l1 = fi1Rect.left() + fi1Rect.width() / 2 - fi1Rect.width() / sectorF_numDegrees * 0.75;
    int r1 = fi1Rect.left() + fi1Rect.width() / 2 + fi1Rect.width() / sectorF_numDegrees * 0.75;
    painter->setPen(QPen(QColor::fromRgbF(redFi1 / 2, 0.4, bluFi1 / 2, 0.6), 1));
    painter->drawLine(l1, fi1Rect.top(), l1, fi1Rect.top() + fi1Rect.height());
    painter->drawLine(r1, fi1Rect.top(), r1, fi1Rect.top() + fi1Rect.height());
    drawScaleRGridsRF(painter, fi1Rect); //  шкала дальности (вертикальная)

    painter->setPen(QPen(QColor::fromRgbF(redFi1 / 2, 0.4, bluFi1 / 2, 0.4), 1));
    painter->drawLine(fi1Rect.left() + fi1Rect.width() / 2, fi1Rect.top(), fi1Rect.left() + fi1Rect.width() / 2, fi1Rect.top() + fi1Rect.height());

    //fi2
    painter->setPen(QPen(QColor::fromRgbF(redFi2 / 2, 0.4, bluFi2 / 2, 0.9), 1));
    painter->drawRect(fi2Rect);

    int l2 = fi2Rect.left() + fi2Rect.width() / 2 - fi2Rect.width() / sectorF_numDegrees * 0.75;
    int r2 = fi2Rect.left() + fi2Rect.width() / 2 + fi2Rect.width() / sectorF_numDegrees * 0.75;
    painter->setPen(QPen(QColor::fromRgbF(redFi2 / 2, 0.4, bluFi2 / 2, 0.6), 1));
    painter->drawLine(l2, fi1Rect.top(), l2, fi1Rect.top() + fi1Rect.height());
    painter->drawLine(r2, fi1Rect.top(), r2, fi1Rect.top() + fi1Rect.height());
    drawScaleRGridsRF(painter, fi2Rect); //  шкала дальности (вертикальная)

    painter->setPen(QPen(QColor::fromRgbF(redFi2 / 2, 0.4, bluFi2 / 2, 0.4), 1));
    painter->drawLine(fi2Rect.left() + fi2Rect.width() / 2, fi2Rect.top(), fi2Rect.left() + fi2Rect.width() / 2, fi2Rect.top() + fi2Rect.height());

    //int echoBrightnessInd = ui->sliderGridBrightness->value();

    //painter->setPen(QPen(gridColors[echoBrightnessInd], 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

    //painter->setPen(QPen(QColor::fromRgbF(0, 0.4, 0.4, 0.9), 1));
}

bool trig = true;

void MainWindow::drawIndicatorRF()
{
    //qDebug()<<"factorF:"<<factorF;

    QPainter painter(&indicatorE);
    painter.setBrush(QBrush(Qt::black));
    painter.drawRect(0, 0, indicatorE.width(), indicatorE.height());

    painter.setBrush(QBrush(Qt::NoBrush));

    float factorArSin = 4.0;    // множитель для арочного синуса, учитывающий ширину ДН УВ11 (1.0 град), для пересчета в пиксели

    //int ys = fi1Rect.top()  + factorR * ( - monoTrack[shMemC2U.srvDrvInfo.simulatorTargetIndexCY][trackPointsCount - 1].slantRange_km + sectorR_numKm); //  положение сигнала по дальности для обоих индикаторов Ф1 и Ф2 одинаково
    int ys = fi1Rect.top()  + factorR * ( - console2User.apyInfo.nextTocka[console2User.apyInfo.targetIndexCY].slantRange_km + sectorR_numKm); //  положение сигнала по дальности для обоих индикаторов Ф1 и Ф2 одинаково

//    qDebug()<<"drawIndicatorRF ys:"<<ys<<"shMemC2U.srvDrvInfo.simulatorTargetIndexCY:"<<shMemC2U.srvDrvInfo.simulatorTargetIndexCY
//            <<"nextTocka:"<<shMemC2U.apyInfo.nextTocka[shMemC2U.srvDrvInfo.simulatorTargetIndexCY].slantRange_km
//            <<"monoTrack:"<<monoTrack[shMemC2U.srvDrvInfo.simulatorTargetIndexCY][trackPointsCount - 1].slantRange_km<<"trackPointsCount:"<<trackPointsCount;

//    float valUV10 = sqrt(monoTrack[shMemC2U.srvDrvInfo.simulatorTargetIndexCY][trackPointsCount - 1].uv11Angles.deltaF[0] *          // абсолютное угловое рассогласование
//                         monoTrack[shMemC2U.srvDrvInfo.simulatorTargetIndexCY][trackPointsCount - 1].uv11Angles.deltaF[0] +          // между истинным направлением на цель и максимумом ДН УВ10
//                         monoTrack[shMemC2U.srvDrvInfo.simulatorTargetIndexCY][trackPointsCount - 1].uv11Angles.deltaF[1] *
//                         monoTrack[shMemC2U.srvDrvInfo.simulatorTargetIndexCY][trackPointsCount - 1].uv11Angles.deltaF[1]) * 2.75;   // 2.75 - множитель для ширины ДН УВ10 1.5 град

    float valUV10 = sqrt(console2User.apyInfo.nextTocka[console2User.apyInfo.targetIndexCY].uv11Angles.deltaF[0] *          // абсолютное угловое рассогласование
                         console2User.apyInfo.nextTocka[console2User.apyInfo.targetIndexCY].uv11Angles.deltaF[0] +          // между истинным направлением на цель и максимумом ДН УВ10
                         console2User.apyInfo.nextTocka[console2User.apyInfo.targetIndexCY].uv11Angles.deltaF[1] *
                         console2User.apyInfo.nextTocka[console2User.apyInfo.targetIndexCY].uv11Angles.deltaF[1]) * 2.75;   // 2.75 - множитель для ширины ДН УВ10 1.5 град

    float factorUV10 = 1.0; // множитель ослабления, учитывающий абсолютное угловое рассогласование между истинным направлением на цель и максимумом ДН УВ10
    if(valUV10 != 0)
    {
        factorUV10 = fabs(sin(valUV10) / (valUV10));
    }

    //  М О Д Е Л Ь   Э Х О - С И Г Н А Л А   К А Н А Л А   Ф 1

    //int xs_Fi1 = fi1Rect.width() / 2 + factorF * monoTrack[shMemC2U.srvDrvInfo.simulatorTargetIndexCY][trackPointsCount - 1].uv11Angles.deltaF[0];   //
    int xs_Fi1 = fi1Rect.width() / 2 + factorF * console2User.apyInfo.nextTocka[console2User.apyInfo.targetIndexCY].uv11Angles.deltaF[0];   //

    for(int i = 0; i < WDT_NAVED_RF; i++)
    {
        float valUV11 = (fabs(i - xs_Fi1)) / factorF * factorArSin;

        float signalPwr = factorUV10;
        if(valUV11 != 0)
            signalPwr *= fabs(sin(valUV11) / (valUV11));

        painter.setPen(QPen(QColor::fromRgbF(0, signalPwr, 0), 1));
        painter.drawPoint(fi1Rect.left() + i, ys);

        painter.setPen(QPen(QColor::fromRgbF(0, 0.8 * signalPwr, 0), 1));
        painter.drawPoint(fi1Rect.left() + i, ys - 1);
        painter.drawPoint(fi1Rect.left() + i, ys + 1);
    }

    //  М О Д Е Л Ь   Э Х О - С И Г Н А Л А   К А Н А Л А   Ф 2

    int xs_Fi2 = fi2Rect.width() / 2 + factorF * console2User.apyInfo.nextTocka[console2User.apyInfo.targetIndexCY].uv11Angles.deltaF[1];

    for(int i = 0; i < WDT_NAVED_RF; i++)
    {
        float valUV11 = (fabs(i - xs_Fi2)) / factorF * factorArSin;

        float signalPwr = factorUV10;
        if(valUV11 != 0)
            signalPwr *= fabs(sin(valUV11) / (valUV11));

        painter.setPen(QPen(QColor::fromRgbF(0, signalPwr, 0), 1));
        painter.drawPoint(fi2Rect.left() + i, ys);

        painter.setPen(QPen(QColor::fromRgbF(0, 0.8 * signalPwr, 0), 1));
        painter.drawPoint(fi2Rect.left() + i, ys - 1);
        painter.drawPoint(fi2Rect.left() + i, ys + 1);
    }


    if(console2User.apyInfo.showSimulationTrack)    //  О Т Р И С О В К А   И С Т И Н Н Ы Х   Т Р А С С
    {
        for(int i = 0; i < trackPointsCount; i++)   //  Ф1
        {
            int y1 = fi1Rect.top()  + factorR * ( - monoTrack[console2User.apyInfo.targetIndexCY][i].slantRange_km + sectorR_numKm);
            int x1 = fi1Rect.left() + fi1Rect.width() / 2 + factorF * monoTrack[console2User.apyInfo.targetIndexCY][i].uv11Angles.deltaF[0];

            ///////////////////////if(i == shMemC2U.srvDrvInfo.trackPointsNumber - 1)             qDebug()<<"drawIndicatorEB: i:"<<i<<"x1:"<<x1<<"y1:"<<y1;
            if(i == trackPointsCount - 1)
                painter.fillRect(x1 - 1, y1 - 1, 3, 3, QColor::fromRgbF(redFi1, 0.8, bluFi1, 0.9));//penSoprTrassa.color());
            else
                painter.fillRect(x1 - 1, y1 - 1, 2, 2, QColor::fromRgbF(redFi1 / 2, 0.4, bluFi1 / 2, 0.9));
        }

        for(int i = 0; i < trackPointsCount; i++)   //  Ф2
        {
            int y1 = fi2Rect.top()  + factorR * ( - monoTrack[console2User.apyInfo.targetIndexCY][i].slantRange_km + sectorR_numKm);
            int x1 = fi2Rect.left() + fi1Rect.width() / 2 + factorF * monoTrack[console2User.apyInfo.targetIndexCY][i].uv11Angles.deltaF[1];

            ///////////////////////if(i == shMemC2U.srvDrvInfo.trackPointsNumber - 1)             qDebug()<<"drawIndicatorEB: i:"<<i<<"x1:"<<x1<<"y1:"<<y1;
            if(i == trackPointsCount - 1)
                painter.fillRect(x1 - 1, y1 - 1, 3, 3, QColor::fromRgbF(redFi2, 0.8, bluFi2, 0.9));//penSoprTrassa.color());
            else
                painter.fillRect(x1 - 1, y1 - 1, 2, 2, QColor::fromRgbF(redFi2 / 2, 0.4, bluFi2 / 2, 0.9));
        }
    }

    if(console2User.apyInfo.showSimulationTarget)    //  П Р О Р И С О В К А   Т О Л Ь К О   Т Е К У Щ И Х   К О О Р Д И Н А Т   Ц Е Л Е Й
    {
        for(int i = 0; i < trackPointsCount; i++)   //  Ф1
        {
            int y1 = fi1Rect.top()  + factorR * ( - console2User.apyInfo.nextTocka[console2User.apyInfo.targetIndexCY].slantRange_km + sectorR_numKm);
            int x1 = fi1Rect.left() + fi1Rect.width() / 2 + factorF * console2User.apyInfo.nextTocka[console2User.apyInfo.targetIndexCY].uv11Angles.deltaF[0];

            if(i == trackPointsCount - 1)
                painter.fillRect(x1 - 1, y1 - 1, 3, 3, QColor::fromRgbF(redFi1, 0.8, bluFi1, 0.9));//penSoprTrassa.color());
            else
                painter.fillRect(x1 - 1, y1 - 1, 2, 2, QColor::fromRgbF(redFi1 / 2, 0.4, bluFi1 / 2, 0.9));
        }

        for(int i = 0; i < trackPointsCount; i++)   //  Ф2
        {
            int y1 = fi2Rect.top()  + factorR * ( - console2User.apyInfo.nextTocka[console2User.apyInfo.targetIndexCY].slantRange_km + sectorR_numKm);
            int x1 = fi2Rect.left() + fi1Rect.width() / 2 + factorF * console2User.apyInfo.nextTocka[console2User.apyInfo.targetIndexCY].uv11Angles.deltaF[1];

            if(i == trackPointsCount - 1)
                painter.fillRect(x1 - 1, y1 - 1, 3, 3, QColor::fromRgbF(redFi2, 0.8, bluFi2, 0.9));//penSoprTrassa.color());
            else
                painter.fillRect(x1 - 1, y1 - 1, 2, 2, QColor::fromRgbF(redFi2 / 2, 0.4, bluFi2 / 2, 0.9));
        }
    }

    drawGridsRF(&painter);

    // Ф О Р М У Л Я Р   Ц Е Л И
    QFont fontE("Arial");
    fontE.setPointSize(9);
    fontE.setBold(true);
    //fontE.setStyleStrategy(QFont::NoAntialias);
    painter.setFont( fontE );

    painter.setPen(QPen(QColor::fromRgbF(0, 0.6, 0), 1));

    int otstYtxt  = 40;
    int dyTxt = 18;
    int textPosX  = ui->indicatorEB->width() - 250;

    painter.drawText(textPosX, otstYtxt, QString("β: %1°    Д: %2 км")
                      .arg(QString::number(console2User.apyInfo.nextTockaRS[0].azimuth360, 'f', 1))
                      .arg(QString::number(console2User.apyInfo.nextTockaRS[0].slantRange_km, 'f', 1)));

    painter.drawText(textPosX, otstYtxt + dyTxt, QString("V: %1 м/c    H: %2 км    ε: %3°").arg(QString::number(console2User.apyInfo.nextTockaRS[0].groundSpeed_ms, 'f', 2))
                       .arg(QString::number(console2User.apyInfo.nextTockaRS[0].seaLevelAltitude, 'f', 1))
                       .arg(QString::number(console2User.apyInfo.nextTockaRS[0].elevationAngle, 'f', 2)));

    painter.drawText(textPosX, otstYtxt + 2 * dyTxt, QString("Курс.пар.тек.: %1 км   макс.: %2 км")
           .arg(QString::number(fabs(console2User.apyInfo.nextTockaRS[0].kyrsovojParameter_km), 'f', 1))
           .arg(QString::number(console2User.apyInfo.nextTockaRS[0].maxKyrsovojParameter_km, 'f', 1)));

    ui->indicatorEB->setPixmap(indicatorE);
}
