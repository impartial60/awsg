#include <QtGui>
#include "mainWindowG.h"
#include "ui_mainwindow.h"

int otstyp_sverhyRV = 15;
int otstyp_slevaRV = 20;


void MainWindow::drawIndicatorRV_Fischuk()
{
    /*
    QPainter painter(&indicatorRV_Fischuk_BMP);
//    painter.setBrush(QBrush(Qt::black));
//    painter.drawRect(0, 0, indicatorRV_Fischuk_BMP.width(), indicatorRV_Fischuk_BMP.height());

    float postLightFactor = 0.99;//1.0;//0.9;//1.0;

    //qDebug()<<"drawIndicatorRV_Fischuk: postLightFactor:"<<postLightFactor;

    for(int j = 0; j < MAX_NUM_MDO; j ++)                 // ПОСЛЕСВЕЧЕНИЕ
    {
        for(int i = 0; i < MAX_NUM_DISCRETS_R; i ++)
        {
            *(yarkostRV +  j * MAX_NUM_DISCRETS_R + i) *= postLightFactor;
            *(imgDataRV + (j * MAX_NUM_DISCRETS_R + i) * 3 + 1) =  (unsigned char)(*(yarkostRV + j * MAX_NUM_DISCRETS_R + i));
        }
    }

    for(int u = 0; u < 1; u ++)
    {
        for(int n = 0; n < 3; n ++)
        {
            for(int i = 0; i < MAX_NUM_DISCRETS_R; i ++) // ECHO_LENGTH
            {
                int nextPixelsAddr = n * MAX_NUM_DISCRETS_R + i;

                *(yarkostRV + nextPixelsAddr) += shMemC2U.mdo1Echo.echo[n][i];

                if(*(yarkostRV + nextPixelsAddr) > 255)
                    *(yarkostRV + nextPixelsAddr) = 255;
            }
        }
    }

    QPixmap pixmapEcho = QPixmap::fromImage(QImage((unsigned char *) imgDataRV, MAX_NUM_DISCRETS_R, MAX_NUM_MDO, QImage::Format_RGB888));

    QPen pen2(QColor(48,48,48, 255), 1, Qt::SolidLine, Qt::SquareCap, Qt::RoundJoin);
    painter.setPen(pen2);
    painter.setBrush(Qt::NoBrush);

    for(int j = 0; j < 38; j ++)
    {
        painter.drawPixmap(otstyp_slevaRV, otstyp_sverhyRV + 24 * j, 512, 18, pixmapEcho, 512 * j, 0, 512, 6);
    }

    painter.drawPixmap(otstyp_slevaRV, otstyp_sverhyRV + 24 * 38, 152, 18, pixmapEcho, 512 * 38, 0, 152, 6);

    for(int j = 0; j < 38; j ++)
    {
        painter.drawRect(otstyp_slevaRV - 1, otstyp_sverhyRV + 24 * j - 1 , 513, 18);

        for(int i = 1; i < MAX_NUM_MDO; i ++)
        {
            painter.drawLine(otstyp_slevaRV - 1, otstyp_sverhyRV + 24 * j - 1 + i * 3, otstyp_slevaRV + 512, otstyp_sverhyRV + 24 * j - 1 + i * 3);
        }
    }

    painter.drawRect(otstyp_slevaRV - 1, otstyp_sverhyRV + 24 * 38 - 1 , 153, 18);

    for(int i = 1; i < MAX_NUM_MDO; i ++)
    {
        painter.drawLine(otstyp_slevaRV - 1, otstyp_sverhyRV + 24 * 38 - 1 + i * 3, otstyp_slevaRV + 152, otstyp_sverhyRV + 24 * 38 - 1 + i * 3);
    }

    ui->indicatorRV->setPixmap(indicatorRV_Fischuk_BMP);
    */
}
