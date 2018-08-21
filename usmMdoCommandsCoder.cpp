#include "mainWindowG.h"
#include "ui_mainwindow.h"
#include <cssTemplates.h>
#include <QDebug>

extern void assignCRC16Values(u_int8_t *pcBlock, unsigned int len, u_int8_t *lowByte , u_int8_t *highByte);

void MainWindow::mdo1CommandCoder()
{
    for(int i = 0; i < 3; i++)
    {
        shMemU2C.mdo1Cmds[i].header.x55           = 0x55;
        shMemU2C.mdo1Cmds[i].header.xAA           = 0xAA;
        shMemU2C.mdo1Cmds[i].header.deviceAddress = 0x21;
        shMemU2C.mdo1Cmds[i].timeFrame.lowByte    = timeFrameIncr & 0x0000FF;           //  Младший байт 23 – битного номера 10мс интервала от начала рабочей сессии. Значащие биты 0…7
        shMemU2C.mdo1Cmds[i].timeFrame.middleByte = (timeFrameIncr & 0x00FF00) >> 8;    //  Средний байт 23 – битного номера 10мс интервала от начала рабочей сессии. Значащие биты 0…7
        shMemU2C.mdo1Cmds[i].timeFrame.highByte   = (timeFrameIncr & 0xFF0000) >> 16;   //  Старший байт 23 – битного номера 10мс интервала от начала рабочей сессии. Значащие биты 0…6
    }

    u_int8_t outset0 = 0x00;                                                    // Задание состояния управляющих выходов
    if(ui->pushButtonFKM127->isChecked())       outset0 = 0x01;                 // Бит 0 – признак ФКМ 0-63 эл 1-127эл

    u_int8_t mod;
    if(ui->radioButtonFKM0->isChecked())        mod = 0x00;
    if(ui->radioButtonFKM1->isChecked())        mod = 0x01;
    if(ui->radioButtonFKM2->isChecked())        mod = 0x02;
    if(ui->radioButtonFKM3->isChecked())        mod = 0x03;
    if(ui->radioButtonFKM4->isChecked())        mod = 0x04;
    if(ui->radioButtonFKM5->isChecked())        mod = 0x05;
    if(ui->radioButtonFKM6->isChecked())        mod = 0x06;
    if(ui->radioButtonFKM7->isChecked())        mod = 0x07;
    if(ui->radioButtonFKM8->isChecked())        mod = 0x08;
    if(ui->radioButtonFKM9->isChecked())        mod = 0x0A;//mod = 0x09;

    outset0 += mod * 2;                                                         // Бит 1..4 - код ФКМ

    if(ui->pushButtonAutoKomp->isChecked())     outset0 += 0x20;                // Бит 5 – автокомменсация
                                            //  outset1 += 0x40;                // Бит 6 – СДЦ
                                            //  outset1 += 0x80;                // Бит 7 – черезпериодная СДЦ

    shMemU2C.mdo1Cmds[0].outset[0] = outset0;

    shMemU2C.mdo1Cmds[0].outset[1] = ui->sliderPorog->value();                  // Бит 0..7 – Уровень порога

    assignCRC16Values(&shMemU2C.mdo1Cmds[0].command, 7, &shMemU2C.mdo1Cmds[0].CRC16[0], &shMemU2C.mdo1Cmds[0].CRC16[1]);
}

void MainWindow::usmCommandCoder()
{
    //qDebug()<<"userCommandCoder Metka 1";

    u_int8_t headerData[7];

    headerData[0] = 0x55;   // Первый байт маркера заголовка 0x55 - для всех одинаковый
    headerData[1] = 0xAA;   // Второй байт маркера заголовка 0xAA - для всех одинаковый

    headerData[3] = 0xAA;   // Код операции, команда 0x00- обмен по умолчанию

    headerData[4] = timeFrameIncr & 0x0000FF;           //  Младший байт 23 – битного номера 10мс интервала от начала рабочей сессии. Значащие биты 0…7
    headerData[5] = (timeFrameIncr & 0x00FF00) >> 8;    //  Средний байт 23 – битного номера 10мс интервала от начала рабочей сессии. Значащие биты 0…7
    headerData[6] = (timeFrameIncr & 0xFF0000) >> 16;   //  Старший байт 23 – битного номера 10мс интервала от начала рабочей сессии. Значащие биты 0…6

    ///////////////////USM_Commands outD;   //2017-11-28

    memcpy(shMemU2C.usmCmds.usm01,       headerData, 7);
    memcpy(shMemU2C.usmCmds.usm02,       headerData, 7);
    memcpy(shMemU2C.usmCmds.usm03,       headerData, 7);
    memcpy(shMemU2C.usmCmds.usm04_topo,  headerData, 7);
    memcpy(shMemU2C.usmCmds.usm05_sjo1,  headerData, 7);
    memcpy(shMemU2C.usmCmds.usm06_sjo2,  headerData, 7);
    memcpy(shMemU2C.usmCmds.usm07_svo,   headerData, 7);
    memcpy(shMemU2C.usmCmds.usm08_sid,   headerData, 7);
    memcpy(shMemU2C.usmCmds.usm09_istp,  headerData, 7);
    memcpy(shMemU2C.usmCmds.usm10_kond,  headerData, 7);
    memcpy(shMemU2C.usmCmds.sync,        headerData, 7);
    memcpy(shMemU2C.usmCmds.oklik,       headerData, 7);

    shMemU2C.usmCmds.usm01[2]=0x0C;
    shMemU2C.usmCmds.usm02[2]=0x18;
    shMemU2C.usmCmds.usm03[2]=0x06;
    shMemU2C.usmCmds.usm04_topo[2]=0x11;
    shMemU2C.usmCmds.usm05_sjo1[2]=0x12;
    shMemU2C.usmCmds.usm06_sjo2[2]=0x13;
    shMemU2C.usmCmds.usm07_svo[2]=0x14;
    shMemU2C.usmCmds.usm08_sid[2]=0x15;
    shMemU2C.usmCmds.usm09_istp[2]=0x16;
    shMemU2C.usmCmds.usm10_kond[2]=0x17;
    shMemU2C.usmCmds.sync[2]=0x03;
    shMemU2C.usmCmds.oklik[2]=0x08;    

    assignCRC16Values(shMemU2C.usmCmds.usm01 + 2, 12, shMemU2C.usmCmds.usm01 + 14, shMemU2C.usmCmds.usm01 + 15);

    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    u_int8_t  inputs4;      //INPUTS4: 	Бит 5 – XP4:24 "02-XK "+27В зад 3мин"
    u_int8_t  inputs6;      //INPUTS6: 	Бит 1 - "02-XK "+27В пит Р"
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    usm02TransmitterManager(inputs4, inputs6);


    // У С М - 0 3

    u_int8_t usm03_7 = ui->sliderUSM3->value();

    if(ui->pushButtonMSY->isChecked())
        usm03_7 += 0x40;

    shMemU2C.usmCmds.usm03[7]=usm03_7;//0b1111;

    shMemU2C.usmCmds.usm03[8]=0;
    shMemU2C.usmCmds.usm03[9]=0;

    assignCRC16Values(shMemU2C.usmCmds.usm03 + 2, 8, shMemU2C.usmCmds.usm03 + 10, shMemU2C.usmCmds.usm03 + 11);



    // С И Н Х Р О Н И З А Т О Р     С И Н Х Р О Н И З А Т О Р     С И Н Х Р О Н И З А Т О Р

    shMemU2C.usmCmds.sync[3] = 0X00;    // !!!!!!!!!!



    unsigned char sync7 = 0x00; //  OUTSET 1
    if(ui->pushButtonStrobIZP->isChecked())      // "Строб ИЗП" включить
        sync7 += 0x01;
    if(ui->pushButtonDprm10plus11->isChecked())  //  "Д прм 10 + 11" включить - по умолчанию включен
        sync7 += 0x06;
    if(ui->pushButtonDrakKan->isChecked())  //  "Д рак кан" включить
        sync7 += 0x08;
    if(ui->pushButtonStrobPS->isChecked())  //  "Строб ПС" разрешить - по умолчанию включен
        sync7 += 0x10;
    if(ui->pushButtonVneshTI->isChecked())  //  "Внеш ТИ" разрешить
        sync7 += 0x20;
    if(ui->pushButtonKS->isChecked())       //  "КС" включить
        sync7 += 0x40;
    shMemU2C.usmCmds.sync[7] = sync7;

    unsigned char rej = 0x00;   //  OUTSET 2

    if(ui->radioButtonRej1->isChecked() || ui->radioButtonRej12->isChecked() || ui->radioButtonRej14->isChecked())  //
        ui->pushButtonFKM127->setChecked(true);
    else
        ui->pushButtonFKM63->setChecked(true);

    if(ui->radioButtonRej12->isChecked())
        ui->pushButtonNPI->setChecked(true);
    else
        ui->pushButtonNPI->setChecked(false);

    if(ui->radioButtonRej1->isChecked())
        rej = 0x21;//rej = 0x01 + 0x20 ФКМ127 ;
    if(ui->radioButtonRej3->isChecked())
        rej = 0x13;//rej = 0x03 + 0x10 ФКМ63;
    if(ui->radioButtonRej12->isChecked())
        rej = 0x6C;//rej = 0x0C + 0x20 ФКМ127 + 0x40 НПИ;

    if(ui->radioButtonRej2->isChecked())
        rej = 0x12;//0x02 + 0x10 ФКМ63
    if(ui->radioButtonRej14->isChecked())
        rej = 0x2E;//0x0E + 0x20 ФКМ127
    if(ui->radioButtonRej15->isChecked())
        rej = 0x1F;//0x0F + 0x10 ФКМ63

    shMemU2C.usmCmds.sync[8] = rej;

    unsigned char mod = 0x00;   //  OUTSET 3    Вид ФКМ
    if(ui->radioButtonFKM0->isChecked())        //  Бит 0..3 – "вид ФКМ 63" в памяти СхР
        mod = 0;
    if(ui->radioButtonFKM1->isChecked())
        mod = 1;
    if(ui->radioButtonFKM2->isChecked())
        mod = 2;
    if(ui->radioButtonFKM3->isChecked())
        mod = 3;
    if(ui->radioButtonFKM4->isChecked())
        mod = 4;
    if(ui->radioButtonFKM5->isChecked())
        mod = 5;
    if(ui->radioButtonFKM6->isChecked())
        mod = 6;
    if(ui->radioButtonFKM7->isChecked())
        mod = 7;
    if(ui->radioButtonFKM8->isChecked())
        mod = 8;
    if(ui->radioButtonFKM9->isChecked())
        mod = 0x0A;//mod = 9;
    if(ui->pushButtonFKM127->isChecked())       //  Бит 4..7 – "вид ФКМ 127" в памяти СхР
        shMemU2C.usmCmds.sync[9] = mod * 16;
    else
        shMemU2C.usmCmds.sync[9] = mod;

    shMemU2C.usmCmds.sync[10] = (ui->dblSpnBxTayImpulsa->value() - 260) / 10; // OUTSET 4 Длительность импульса НПИ    Бит 0..5 – число*10нс + 260 нс

    shMemU2C.usmCmds.sync[11] = (int)ui->dblSpnBxSkvNPI->value();// - 9;//0x0A;   // OUTSET 5     Скважность НПИ      Бит 0..3 – число 9 ... 12

    shMemU2C.usmCmds.sync[12] = (int)ui->dblSpnBxTnakopl->value();   // OUTSET 6     Время накопления    Бит 0..4 – число мс

    shMemU2C.usmCmds.sync[13] = (int)ui->dblSpnBxAtten->value() - 1;   // OUTSET 7   Настройка БЗГ Бит 0..3 – "Атт" аттенюатор БЗГ

    shMemU2C.usmCmds.sync[14] = (int)ui->dblSpnBxLitera->value(); // OUTSET 8     Бит 0..5 – частота сигнала

    shMemU2C.usmCmds.sync[15] = 0x00; // OUTSET  9      Настройка БЗГ   Бит 0..3 – частота подсвета
    shMemU2C.usmCmds.sync[16] = 0x00; // OUTSET 10      Резерв
    shMemU2C.usmCmds.sync[17] = 0x00; // OUTSET 11      Резерв

    assignCRC16Values(shMemU2C.usmCmds.sync + 2, 16, shMemU2C.usmCmds.sync + 18, shMemU2C.usmCmds.sync + 19);


    // О К Л И К     О К Л И К     О К Л И К     О К Л И К     О К Л И К     О К Л И К     О К Л И К     О К Л И К     О К Л И К     О К Л И К

        //  COMMAND Команда для следующего такта синхронизации ПС.  //  Команда включения режимов работы ОКЛИК
    uint8_t okl6 = 0x01;        //  Бит 0 – 1 – “Боевой режим” излучение на антенну,  0 – “Контроль” излучение на эквивалент
                                //  Бит 1 – “Подготовка 2”  Команда включения контроля канала РК.  Бит передаётся до завершения работы контроля РК.
            okl6 += 0x04;       //  Бит 2 – “Си Окл.” Происходит подсинхронизация начала работы рабочего периода канала РК.
                                //  С текущего такта СИП во время интервала работы реж. Обзор запускается счётчик от 01 до 21, по 7 для каждой ракеты.
                                //  Бит передаётся разово для каждого рабочего периода РК.
                                //  Бит 3 – “Сход”   Команда схода ракеты
                                //  Бит 4,5 – Резерв
            okl6 += 0x40;       //  Бит 7,6 –  01 “Ракета №1” 10 “Ракета №2” 11 “Ракета №3” резерв
                                //  Номер ракеты указывает, кокой ракете передаются радиокоррекция и разовые команды. Радиокоррекция и Стр.РК передаются на БЗГ
                                //  только при приходе данных для своего номера. Наличию/истинности события соответствует «1»
    shMemU2C.usmCmds.oklik[6] = okl6;

    shMemU2C.usmCmds.oklik[7] = 0b0100;     //  OUTSET 1       Бит 3 … 0 -  “Номер литера ПС”   Литер общий для всех ракет
                                            //  0000-литер1 0001-литер2 0010-литер3 0011-литер4 0100-литер5 0101-литер6 0110-литер7 0111-литер8 1000-литер9 1001-литер 10

    uint8_t okl8 =  0x01;       //  OUTSET 2       Бит 0 – “Вкл. Реж. ПС”  Команда включения режима подсвет.
            okl8 += 0x02;       //  Бит 1 – “Вкл. реж. РК”  Команда включения радиокоррекции. Передаётся индивидуально для каждого номера ракеты.
                                //  Команда действительна только для одного рабочего периода работы канала РК.
                                //  Бит 2 – Резерв. ( СРК-170)
            okl8 += 0x08;       //  Бит 3 – “Вкл. Реж. КС” (Круговое излучение)
            okl8 += 0x10;       //  Бит 4 – “Вкл. Д0 БЗГ”  Команда разрешения включения выходного СВЧ сигнала с БЗГ и УМ.
                                //  Бит 5 – “Вкл. УМ”  Команда включения усилителя мощности.
            okl8 += 0x40;       //  Бит 6 – “Вкл. Термостатов БЗГ”  Команда включения термостатов в БЗГ.
            okl8 += 0x80;       //  Бит 7 – “Вкл. Боевого реж. БЗГ”  Команда полного включения БЗГ.
                                //  Наличию/истинности события соответствует «1»  Общий для всех ракет
    shMemU2C.usmCmds.oklik[8] = okl8;

    shMemU2C.usmCmds.oklik[9] = 0x00;    // OUTSET 3        Бит 0...7 – Резерв

    shMemU2C.usmCmds.oklik[10] = 78;     // КБ              Бит 6...0 “Код Баркера” Передача индивидуального КБ для каждой ракеты по её номеру в слове COMMAND


    shMemU2C.usmCmds.oklik[11] = 0x00;  // ΔlZ  ΔlY        Бит 6...4 “ΔlZ”  Линейная относительная координата цели 0Z, Бит 6-знаковый. Бит 4-младший.
                                        //  011   +300 м    010   +200 м    001   +100 м    000      0 м    101    -100 м   110    -200 м   111    -300 м
                                        //  Бит 2...0 “ΔlY”  Линейная относительная координата цели 0У, Бит 2-знаковый. Бит 0-младший.
                                        //  011   +300 м    010   +200 м    001   +100 м    000      0 м    101    -100 м   110    -200 м   111    -300 м
                                        //  Индивидуальный для каждой ракеты по её номеру в слове COMMAND

    shMemU2C.usmCmds.oklik[12] = 0x00;  //  ΔlX  ΔiZ
    shMemU2C.usmCmds.oklik[13] = 0x00;  //  ΔiY  ΔiX

    shMemU2C.usmCmds.oklik[14] = 0b001; //  РК             Бит 2...0 - Разовая команда, Бит 2-знаковый. Бит 0-младший.
                                        //  011-“РЗ” 010-“ТБЦ” 001-“ТЦМ” 000-“ТЦС” 101-“ПхРЛС” 110-“ЗРЗ” 111-“ШП” Индивидуальный для каждой ракеты по её номеру в слове COMMAND

    assignCRC16Values(shMemU2C.usmCmds.oklik + 2, 13, shMemU2C.usmCmds.oklik + 15, shMemU2C.usmCmds.oklik + 16);

    //  for test only  СЖО-1 2017-11-28
    //  On_=0x55 0xAA 0x12 0x00 0x00 0x00 0x00 0x01 0x3D 0x56
    //  Off=0x55 0xAA 0x12 0x00 0x00 0x00 0x00 0x00 0xFC 0x96

    shMemU2C.usmCmds.usm05_sjo1[3] = 0x00;
    shMemU2C.usmCmds.usm05_sjo1[4] = 0x00;
    shMemU2C.usmCmds.usm05_sjo1[5] = 0x00;
    shMemU2C.usmCmds.usm05_sjo1[6] = 0x00;

    if(ui->pushButtonSJO1->isChecked())
    {
        shMemU2C.usmCmds.usm05_sjo1[7] = 0x01;
        shMemU2C.usmCmds.usm05_sjo1[8] = 0x3D;
        shMemU2C.usmCmds.usm05_sjo1[9] = 0x56;
    }
    else
    {
        shMemU2C.usmCmds.usm05_sjo1[7] = 0x00;
        shMemU2C.usmCmds.usm05_sjo1[8] = 0xFC;
        shMemU2C.usmCmds.usm05_sjo1[9] = 0x96;
    }
    //  end for test only  СЖО-1 2017-11-28


//    Caption=СЖО-2
//    On_=0x55 0xAA 0x13 0x00 0x00 0x00 0x00 0x01 0x3C 0x87
//    Off=0x55 0xAA 0x13 0x00 0x00 0x00 0x00 0x00 0xFD 0x47

    shMemU2C.usmCmds.usm06_sjo2[3] = 0x00;
    shMemU2C.usmCmds.usm06_sjo2[4] = 0x00;
    shMemU2C.usmCmds.usm06_sjo2[5] = 0x00;
    shMemU2C.usmCmds.usm06_sjo2[6] = 0x00;

    if(ui->pushButtonSJO2->isChecked())
    {
        shMemU2C.usmCmds.usm06_sjo2[7] = 0x01;
        shMemU2C.usmCmds.usm06_sjo2[8] = 0x3C;
        shMemU2C.usmCmds.usm06_sjo2[9] = 0x87;
    }
    else
    {
        shMemU2C.usmCmds.usm06_sjo2[7] = 0x00;
        shMemU2C.usmCmds.usm06_sjo2[8] = 0xFD;
        shMemU2C.usmCmds.usm06_sjo2[9] = 0x47;
    }

//    Caption=С И Д
//    On_=0x55 0xAA 0x15 0x00 0x00 0x00 0x00 0x01 0x3C 0xe1
//    Off=0x55 0xAA 0x15 0x00 0x00 0x00 0x00 0x00 0xFD 0x21

    shMemU2C.usmCmds.usm08_sid[3] = 0x00;
    shMemU2C.usmCmds.usm08_sid[4] = 0x00;
    shMemU2C.usmCmds.usm08_sid[5] = 0x00;
    shMemU2C.usmCmds.usm08_sid[6] = 0x00;

    if(ui->pushButtonSID->isChecked())
    {
        shMemU2C.usmCmds.usm08_sid[7] = 0x01;
        shMemU2C.usmCmds.usm08_sid[8] = 0x3C;
        shMemU2C.usmCmds.usm08_sid[9] = 0xe1;
    }
    else
    {
        shMemU2C.usmCmds.usm08_sid[7] = 0x00;
        shMemU2C.usmCmds.usm08_sid[8] = 0xFD;
        shMemU2C.usmCmds.usm08_sid[9] = 0x21;
    }

    /////////////////////////memcpy(&shMemU2C.usmCmds, &outD, sizeof(USM_Commands));    //2017-11-28

    //memcpy(array2SendAll + oklikStartAddr, &usmOutcomingData.oklik, sizeof(usmOutcomingData.oklik));

    //assignCRC16Values(array2SendAll + oklikStartAddr + 2, 21, array2SendAll + oklikStartAddr + 23, array2SendAll + oklikStartAddr + 24);
    //assignCRC16Values(array2SendAll + oklikStartAddr + 2, 13, array2SendAll + oklikStartAddr + 15, array2SendAll + oklikStartAddr + 26);

    //memcpy(&awsgSharedMemory.awsgUser2Console.usmOutcomingData, &outData, sizeof(outData));

    //memcpy(outDataMemoryAddress, &outData, sizeof(outData));
}

void MainWindow::usm02TransmitterManager(u_int8_t  inputs4,  u_int8_t  inputs6)
{
    //qDebug()<<"USM_Data::usm02TransmitterManager nakalMaxTicks:"<<nakalMaxTicks<<"nakalTimeInTicks:"<<nakalTimeInTicks<<"nakalTimeInSeconds:"<<nakalTimeInSeconds;

    bool nakalGotov = false;

    if(((inputs4 & 0x20) == 0x20) && ((inputs6 & 0x02) == 0x02))  //INPUTS4: 	Бит 5 – XP4:24 "02-XK "+27В зад 3мин"    INPUTS6: 	Бит 1 - "02-XK "+27В пит Р"
        nakalGotov = true;

    if(nakalTimeInTicks < -100) //  временно   временно   временно
        nakalGotov = true;

    switch(transmitterStatus)
    {
    case TransmitterStatus::ISKHODNOE:
        if(ui->pushButtonNakal->isChecked())
        {
            transmitterStatus = TransmitterStatus::PEREHOD_ISKHODNOE_NAKAL;

            shMemU2C.usmCmds.usm02[7] = 0x00;    //OUTSET1
            shMemU2C.usmCmds.usm02[8] = 0x43;    //OUTSET2

            ui->pushButtonNakal->setStyleSheet(btnCSS[GREEN_COLOR_INDEX]);
        }
        else
        {
            shMemU2C.usmCmds.usm02[7] = 0x00;    //OUTSET1
            shMemU2C.usmCmds.usm02[8] = 0x03;    //OUTSET2
        }
        break;

    case TransmitterStatus::PEREHOD_ISKHODNOE_NAKAL:

        if(ui->pushButtonNakal->isChecked() == false)
        {
            transmitterStatus = TransmitterStatus::ISKHODNOE;

            shMemU2C.usmCmds.usm02[7] = 0x00;    //OUTSET1
            shMemU2C.usmCmds.usm02[8] = 0x03;    //OUTSET2

            ui->pushButtonNakal->setStyleSheet(btnCSS[GRAY_COLOR_INDEX]);

            nakalTimeInTicks = nakalMaxTicks;
            nakalTimeInSeconds = nakalTimeInTicks / 1000 * TIMER_INTERVAL;

            ui->progressBarTimeRemain->setValue(nakalTimeInTicks);
            ui->progressBarTimeRemain->setVisible(true);
            ui->label_time_remain->setText(QString::number(nakalTimeInSeconds) + "\"");
            ui->pushButtonTransmitter->setEnabled(false);
            ui->pushButtonTransmitter->setStyleSheet(btnCSS[GRAY_COLOR_INDEX]);
        }
        else
        {
            shMemU2C.usmCmds.usm02[7] = 0x00;    //OUTSET1
            shMemU2C.usmCmds.usm02[8] = 0x43;    //OUTSET2

            if(nakalGotov)
            {
                transmitterStatus = TransmitterStatus::NAKAL;

                nakalTimeInTicks = nakalMaxTicks;
                nakalTimeInSeconds = nakalTimeInTicks / 1000 * TIMER_INTERVAL;

                QString gotovStr [] = {"ready", "готов", "pronto"};

               //if(settings->languageIndex >= 0 && settings->languageIndex <= 2)
                   ui->label_time_remain->setText(gotovStr[1]);//settings->languageIndex]);

                ui->pushButtonNakal->setStyleSheet(btnCSS[GREEN_COLOR_INDEX]);

                ui->progressBarTimeRemain->setVisible(false);
                ui->pushButtonTransmitter->setEnabled(true);
            }
            else
            {
               nakalTimeInTicks -= 1;

               bool isGreen = false;
               if(((int)(fabs(nakalTimeInTicks) / 1000 * TIMER_INTERVAL * 2))% 2 == 1)
                   isGreen = true;

               if(isGreen)
               {
                  ui->pushButtonNakal->setStyleSheet(btnCSS[GRAY_COLOR_INDEX]);
               }
               else
               {
                  ui->pushButtonNakal->setStyleSheet(btnCSS[GREEN_COLOR_INDEX]);
               }

               int indicatorNakalTime = nakalTimeInTicks / 1000 * TIMER_INTERVAL;

               if(indicatorNakalTime < 1)
               {
                   ui->pushButtonTransmitter->setEnabled(true);

                   if(ui->pushButtonTransmitter->isChecked())
                   {
                       if(isGreen)
                       {
                          ui->pushButtonTransmitter->setStyleSheet(btnCSS[GRAY_COLOR_INDEX]);
                       }
                       else
                       {
                          ui->pushButtonTransmitter->setStyleSheet(btnCSS[GREEN_COLOR_INDEX]);
                       }
                   }
                   else
                   {
                       ui->pushButtonTransmitter->setStyleSheet(btnCSS[GRAY_COLOR_INDEX]);
                   }

                   indicatorNakalTime = 1;
               }

               ui->progressBarTimeRemain->setValue(nakalTimeInTicks);
               ui->progressBarTimeRemain->setVisible(true);
               ui->label_time_remain->setText(QString::number(indicatorNakalTime) + "\"");
            }
        }

        break;

        case TransmitterStatus::NAKAL:

        if(ui->pushButtonTransmitter->isChecked())
        {
            shMemU2C.usmCmds.usm02[7] = priznakGr();//OUTSET1
            shMemU2C.usmCmds.usm02[8] = 0x41;    //OUTSET2

            ui->pushButtonTransmitter->setStyleSheet(btnCSS[GREEN_COLOR_INDEX]);

            transmitterStatus = TransmitterStatus::VISOKOE_EKV;
            ui->pushButtonNakal->setEnabled(false);

            ui->frameAntEcv->setEnabled(true);
            ui->frameAntEcv->setEnabled(true);
            ui->pushButtonAnt->setEnabled(true);
            ui->pushButtonEcv->setEnabled(true);

            //qDebug()<<"transmitterManager ui->pushButtonIzluch->isChecked() transmitterStatus: "<<transmitterStatus;
        }
        else
        {
            if(ui->pushButtonNakal->isChecked() == false)
            {
                shMemU2C.usmCmds.usm02[7] = 0x00;    //OUTSET1
                shMemU2C.usmCmds.usm02[8] = 0x03;    //OUTSET2

                transmitterStatus = TransmitterStatus::ISKHODNOE;
                ui->pushButtonNakal->setStyleSheet(btnCSS[GRAY_COLOR_INDEX]);

                ui->progressBarTimeRemain->setValue(nakalTimeInTicks);// / 2);
                ui->progressBarTimeRemain->setVisible(true);
                ui->label_time_remain->setText(QString::number(nakalTimeInSeconds / 1) + "\"");

                ui->pushButtonTransmitter->setEnabled(false);
            }
            else
            {
                shMemU2C.usmCmds.usm02[7] = 0x00;    //OUTSET1
                shMemU2C.usmCmds.usm02[8] = 0x43;    //OUTSET2
            }
        }

        break;

    case TransmitterStatus::VISOKOE_EKV:

        if(ui->pushButtonTransmitter->isChecked() == false)
        {
            shMemU2C.usmCmds.usm02[7] = 0x00;    //OUTSET1
            shMemU2C.usmCmds.usm02[8] = 0x43;    //OUTSET2

            ui->frameAntEcv->setEnabled(false);
            ui->pushButtonEcv->setChecked(true);

            ui->pushButtonTransmitter->setStyleSheet(btnCSS[GRAY_COLOR_INDEX]);

            ui->pushButtonNakal->setEnabled(true);

            transmitterStatus = TransmitterStatus::NAKAL;
        }
        else
        {
            ui->frameAntEcv->setEnabled(true);

            shMemU2C.usmCmds.usm02[7] = priznakGr();//OUTSET1

            if(ui->pushButtonAnt->isChecked())
            {
                transmitterStatus = TransmitterStatus::VISOKOE_ANT;
                shMemU2C.usmCmds.usm02[8] = 0x44;    //OUTSET2
            }
            else
            {
                shMemU2C.usmCmds.usm02[8] = 0x41;    //OUTSET2
            }
        }

        break;

    case TransmitterStatus::VISOKOE_ANT:

        if(ui->pushButtonEcv->isChecked() || ui->pushButtonTransmitter->isChecked() == false)
        {
            ui->pushButtonEcv->setChecked(true);
            ui->pushButtonTransmitter->setChecked(false);
            ui->pushButtonTransmitter->setStyleSheet(btnCSS[GRAY_COLOR_INDEX]);

            ui->frameAntEcv->setEnabled(false);
            ui->pushButtonNakal->setEnabled(true);
            transmitterStatus = TransmitterStatus::NAKAL;
            shMemU2C.usmCmds.usm02[7] = 0x00;  //OUTSET1
            shMemU2C.usmCmds.usm02[8] = 0x41;  //OUTSET2
        }
        else
        {
            shMemU2C.usmCmds.usm02[7] = priznakGr();//OUTSET1
            shMemU2C.usmCmds.usm02[8] = 0x44;    //OUTSET2
        }
    }

//    unsigned short int x1 = CRC16 (bufStartInd + 2, 7);
//    *(bufStartInd + 9) = x1 % 256;
//    *(bufStartInd + 10) = x1 / 256;

//    unsigned char lB, hB;
//    lB = *(bufStartInd + 9);
//    hB = *(bufStartInd + 10);

    assignCRC16Values(shMemU2C.usmCmds.usm02 + 2, 7, shMemU2C.usmCmds.usm02 + 9, shMemU2C.usmCmds.usm02 + 10);

    //assignCRC16Values(bufStartInd + 2, 7, &lB, &hB);

//    qDebug()<<QString("CRC16: 0x%1 0x%2    assignCRC16Values: 0x%3 0x%4").arg(*(bufStartInd + 9), 2, 16, QChar('0')).arg(*(bufStartInd + 10), 2, 16, QChar('0'))
//                                            .arg(lB, 2, 16, QChar('0')).arg(hB, 2, 16, QChar('0'));
}

u_int8_t MainWindow::priznakGr()
{
    if(ui->radioButtonTransm15->isChecked())
        return 0x01;    //OUTSET1           Бит 0 – "02-X4 "Призн 1 и 5 групп"
    if(ui->radioButtonTransm26->isChecked())
        return 0x02;    //OUTSET1           Бит 1 - "02-X4 "Призн 2 и 6 групп"
    if(ui->radioButtonTransm3->isChecked())
        return 0x04;    //OUTSET1           Бит 2 – "02-X4 "Призн 3 групп"
    if(ui->radioButtonTransm4->isChecked())
        return 0x08;    //OUTSET1           Бит 3 – "02-X4" Призн 4 групп"
    if(ui->radioButtonStrobP->isChecked())
        return 0x20;    //OUTSET1           Бит 5 – "02-X5 "Строб П"
}
