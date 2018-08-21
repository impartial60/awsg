#include <QtGui>
#include "mainWindowG.h"
#include "ui_mainwindow.h"

int otstyp_sverhyPckg = 30;
int otstyp_slevaPckg = 10;

extern QColor gridColors[];

void MainWindow::drawOscilloscopeFischuk(int startIndex)
{
    /*
    QPainter painter(&oscBMP);
    painter.setBrush(QBrush(Qt::black));
    painter.drawRect(0, 0, oscBMP.width(), oscBMP.height());

    float postLightFactor = 0.99;//1.0;

    for(int j = 0; j < HGT_H_OSC_FISCHUK; j ++)                 // ПОСЛЕСВЕЧЕНИЕ
    {
        for(int i = 0; i < MAXNUMTOCHEKFORPAINT_OSC_FISCHUK; i ++)
        {
            *(yarkostOsc + j * MAXNUMTOCHEKFORPAINT_OSC_FISCHUK + i) *= postLightFactor;
            *(imgDataOsc + (j * MAXNUMTOCHEKFORPAINT_OSC_FISCHUK + i) * 3 + 1) =  (unsigned char)(*(yarkostOsc + j * MAXNUMTOCHEKFORPAINT_OSC_FISCHUK + i));
        }
    }

    double maxValue = 255;//sliderValues[slideValue_0based];

    //int startIndex = 299;

    for(int i = 0; i < MAXNUMTOCHEKFORPAINT_OSC_FISCHUK; i ++)
    {
        double znach = 1;

        znach = shMemC2U.mdo1Echo.echo[0][i];//array2Read[startIndex + i];

        int index_Y = (int)((1.0 - znach / maxValue) * HGT_H_OSC_FISCHUK);

        if(index_Y < 0)
            index_Y = 0;
        else
            if(index_Y >= HGT_H_OSC_FISCHUK)
                index_Y = HGT_H_OSC_FISCHUK - 1;

        *(yarkostOsc + index_Y * MAXNUMTOCHEKFORPAINT_OSC_FISCHUK + i) = 255.0;
        *(imgDataOsc + (index_Y * MAXNUMTOCHEKFORPAINT_OSC_FISCHUK + i) * 3 + 1) = 255;

        /////////////////////////////////////////////////

//        znach = array2Read[startIndex + 266 + i];

//        index_Y = (int)((1.0 - znach / maxValue) * HGT_H_OSC_FISCHUK);

//        if(index_Y < 0)
//            index_Y = 0;
//        else
//            if(index_Y >= HGT_H_OSC_FISCHUK)
//                index_Y = HGT_H_OSC_FISCHUK - 1;

//        *(yarkostOsc + index_Y * MAXNUMTOCHEKFORPAINT_OSC_FISCHUK + i) = 255.0;
//        *(imgDataOsc + (index_Y * MAXNUMTOCHEKFORPAINT_OSC_FISCHUK + i) * 3 + 1) = 255;

//        znach = array2Read[startIndex + 266 * 2 + i];

//        index_Y = (int)((1.0 - znach / maxValue) * HGT_H_OSC_FISCHUK);

//        if(index_Y < 0)
//            index_Y = 0;
//        else
//            if(index_Y >= HGT_H_OSC_FISCHUK)
//                index_Y = HGT_H_OSC_FISCHUK - 1;

//        *(yarkostOsc + index_Y * MAXNUMTOCHEKFORPAINT_OSC_FISCHUK + i) = 255.0;
//        *(imgDataOsc + (index_Y * MAXNUMTOCHEKFORPAINT_OSC_FISCHUK + i) * 3 + 1) = 255;

    }  // for NUMTOCHEKFORPAINT

    QPixmap pixmapEcho = QPixmap::fromImage(QImage((unsigned char *) imgDataOsc, MAXNUMTOCHEKFORPAINT_OSC_FISCHUK, HGT_H_OSC_FISCHUK, QImage::Format_RGB888));
    //painter.drawPixmap(otstyp_slevaOsc, otstyp_sverhyOsc, pixmapEcho);
    painter.drawPixmap(otstyp_slevaOsc, otstyp_sverhyOsc, pixmapEcho.width() * 2, pixmapEcho.height(), pixmapEcho);

    ui->oscilloscope->setPixmap(oscBMP);
    */
}

void MainWindow::drawTextHalo(QPainter *painter, QPen textPen, QFont textFont, int x, int y, QString text)
{
    painter->setFont(textFont);
    //QFontMetrics fMetrics = painter.fontMetrics();
    //QSize sz = fMetrics.size(Qt::TextSingleLine, text);

    //QPen blackPen((Qt::black);
    //painter->setPen(blackPen);
    painter->setPen(QColor(0,0,0,255));

    int d = 1;
    painter->drawText(x - d, y - d, text);
    painter->drawText(x + d, y - d, text);
    painter->drawText(x - d, y + d, text);
    painter->drawText(x + d, y + d, text);

    painter->drawText(x    , y - d, text);
    painter->drawText(x    , y - d, text);
    painter->drawText(x - d, y    , text);
    painter->drawText(x + d, y    , text);

    painter->setPen(textPen);
    painter->drawText(x,y,text);
}
