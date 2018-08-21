#include "qlabelm.h"
#include <QApplication>
#include <QDebug>
//#include <QObject>

qLabelM::qLabelM(QWidget *parent) :
    QLabel(parent)
{
    visibleMarker = false;    
    cursorHotPointX = 4;
    cursorHotPointY = 0;//27;

    controlModifierIsHoldingDown = false;
    //qDebug()<<"qLabelM::controlModifierIsHoldingDown:"<<parent->objectName()<<controlModifierIsHoldingDown;
}

void qLabelM::mouseReleaseEvent(QMouseEvent *ev)
{
    controlModifierIsHoldingDown = false;
}


void qLabelM::mouseMoveEvent(QMouseEvent *ev)
{    
    //this->x  = ev->x() - cursorHotPointX;
    //this->y  = ev->y() + cursorHotPointY;

    int scrX = ev->x() - width() / 2 - cursorHotPointX;
    int scrY = - ev->y() + height() / 2 + cursorHotPointY;

    int hovMain = scrX;
    int hovDelt = scrX * 10;

    if(objectName() == "azimuthScale3600")
    {
        hovDelt = scrX;
        hovMain = scrX / 10;
    }

    if(objectName() == "elevationScale87")
    {
        hovMain = scrY;
        hovDelt = scrY * 10;

        //qDebug()<<"qLabelM::mouseMoveEvent: elevationScale360:"<<"hovMain:"<<hovMain;
    }

    if(objectName() == "elevationScale870")
    {
        hovDelt = scrY;
        hovMain = scrY / 10;
    }


    if(objectName() == "indicatorEB")
    {
        hovMain = ev->x() - cursorHotPointX;
        hovDelt = ev->y() - 6;
    }

    if(controlModifierIsHoldingDown)
    {
        double delitel = 30.0;

        if(objectName() == "azimuthScale360" || objectName() == "elevationScale87")
        {
            delitel = 3.0;
        }

        int scrX = ev->x() - width() / 2 - cursorHotPointX;
        int scrY = ev->y() - height() / 2 - cursorHotPointY;

        double deltaAzimHvr = 1 / delitel * scrX;

        if(objectName() == "elevationScale87" || objectName() == "elevationScale870")
            deltaAzimHvr = 1 / delitel * scrY;

        speed = 0.1 * deltaAzimHvr;
    }

    visibleMarker = true;

//    if(objectName() == "mainIndicator" && ev->modifiers().testFlag(Qt::ShiftModifier)) //if(QApplication::keyboardModifiers().testFlag(Qt::ShiftModifier))
//        this->setCursor(Qt::CrossCursor);
//    else
//        this->setCursor(Qt::ArrowCursor);

    emit Mouse_Pos(hovMain, hovDelt);//objectName());//parent()->objectName());
}

void qLabelM::mousePressEvent(QMouseEvent *ev)
{
    double delitel = 3.0;

    if(objectName() == "azimuthScale3600" || objectName() == "elevationScale870")
    {
        delitel = 30.0;
    }

    int scrX = ev->x() - width() / 2 - cursorHotPointX;
    double deltaAzimHvr = 1 / delitel * scrX;

    if(objectName() == "elevationScale87" || objectName() == "elevationScale870")
    {
        scrX = - ev->y() + height() / 2 + cursorHotPointY;
        deltaAzimHvr = 1 / delitel * scrX;
    }

    //////////////////////////qDebug()<<"qLabelM::mousePressEvent:"<<ev->modifiers();

    //controlModifierIsPressed = false;
    if(Qt::ControlModifier == QApplication::keyboardModifiers())
    {
        controlModifierIsHoldingDown = true;
        speed = 0.1 * deltaAzimHvr;
    }

    if(objectName() == "mainIndicator")
        emit Mouse_Click(ev->x(), ev->y());
    else
        if(objectName() == "indicatorEB")
        {
            emit Mouse_Click();
        }
        else
        {
            emit Mouse_Click(deltaAzimHvr);//, controlModifierIsPressed);
        }
}

void qLabelM::mouseDoubleClickEvent(QMouseEvent* ev)
{
    this->doubleClickX = ev->x();
    if (ev->button() == Qt::LeftButton)
    {
        emit doubleClicked(ev->x() - cursorHotPointX , ev->y() - cursorHotPointY);
    }
}

void qLabelM::leaveEvent(QEvent *ev)
{
    if (ev->type() == QEvent::Leave)
        visibleMarker = false;

    //qDebug() << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";
}
