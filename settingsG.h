#ifndef SETTINGSG_H
#define SETTINGSG_H
#include <QGraphicsDropShadowEffect>
#include <QApplication>
#include <QSettings>
#include <QDebug>
#include <QDir>

#include "mainWindowG.h"
#include "ui_mainwindow.h"


class SETTINGSG
{
    public:
        QSettings *factorySettings;
        QSettings *userSettings;

        char ipAddress_dispatcher[255];
        ushort port_dispatcherG;

        //QString awsG125AbsolutPath;
        int languageIndex;

        SETTINGSG(Ui::MainWindow *ui)
        {
            QDir dir = QDir(QCoreApplication::applicationDirPath());//::applicationFilePath());
            QString awsG125AbsolutPath = dir.absolutePath() + "/";

            //qDebug()<<"user SETTINGSG awsG125AbsolutPath:"<<awsG125AbsolutPath;
            factorySettings = new QSettings(awsG125AbsolutPath + "awsG125FactorySettings.conf", QSettings::NativeFormat);

            userSettings = new QSettings(awsG125AbsolutPath + "awsG125UserSettings.conf", QSettings::NativeFormat);

            //QString settingsFullPathName = dir.absolutePath() + "/awsG125FactorySettings.conf";
            //bool fileExists = QFile::exists(settingsFullPathName);

            assignSettings(ui);
        }

        void assignSettings(Ui::MainWindow *ui)
        {
            //(void)ui;

            QString strIpAddressAndPortDispatcher = factorySettings->value("IPAddressesAndPorts/ipAddressAndPort_mainServer", "10.5.0.21:30017").toString();

            QStringList strListIpAddrPortDispatcher = strIpAddressAndPortDispatcher.split(":");

            if(strListIpAddrPortDispatcher.size() == 2)
            {
                strcpy(ipAddress_dispatcher,  strListIpAddrPortDispatcher[0].toStdString().c_str());
                bool ok;
                port_dispatcherG = strListIpAddrPortDispatcher[1].toUShort(&ok, 10);
            }


            languageIndex = userSettings->value("Application/languageIndex", 0).toInt();

            switch (languageIndex)
            {
                case 0: ui->pushButtonEnglish->setChecked(true);  break;
                case 1: ui->pushButtonRussian->setChecked(true);  break;
                case 2: ui->pushButtonPortug->setChecked(true);
            }
        }

        void saveToFile(Ui::MainWindow *ui)
        {
            qDebug()<<"saveToFile: languageIndex:"<<languageIndex;

            userSettings->setValue("Application/languageIndex", QString::number(languageIndex));

            //userSettings->sync();
        }

};

#endif // SETTINGSG_H
