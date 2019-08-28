#include "widget.h"
#include <QPainter>
#include <QtWidgets>
#include <QPushButton>
#include <QButtonGroup>
#include <QDebug>


Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    //기본 윈도우 설정 및 이미지 로드, 버튼 추가
    this->setFixedSize(564, 764);
    if(!pixmap.load(":/sensor.PNG")){
        qDebug() << "unable to load img";
        exit(-1);
    }


    //소켓 및 통신 설정
    socket = new QTcpSocket(this);
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(socket, SIGNAL(connected()), this, SLOT(connected()));


    //방향지시등 버튼 그룹
    QButtonGroup *dirLight = new QButtonGroup(this);
    QPushButton *leftLight = new QPushButton("L", this);
    QPushButton *rightLight = new QPushButton("R", this);
    leftLight->setGeometry(30, 130, 30, 30);
    rightLight->setGeometry(60, 60, 30, 30);
    dirLight->addButton(leftLight, 11);
    dirLight->addButton(rightLight, 12);
    connect(dirLight, SIGNAL(buttonClicked(int)), SLOT(click(int)));





    //브레이크 0~3 버튼 그룹
    QButtonGroup *breakGroup = new QButtonGroup(this);
    QPushButton *break0 = new QPushButton("0", this);
    QPushButton *break1 = new QPushButton("1", this);
    QPushButton *break2 = new QPushButton("2", this);
    QPushButton *break3 = new QPushButton("3", this);
    break0->setGeometry(200, 550 , 40, 40);
    break1->setGeometry(240, 550 , 40, 40);
    break2->setGeometry(280, 550 , 40, 40);
    break3->setGeometry(320, 550 , 40, 40);
    breakGroup->addButton(break0, 21);
    breakGroup->addButton(break1, 22);
    breakGroup->addButton(break2, 23);
    breakGroup->addButton(break3, 24);
    connect(breakGroup, SIGNAL(buttonClicked(int)), SLOT(click(int)));

    //액셀 0~3 버튼 그룹
    QButtonGroup *accGroup = new QButtonGroup(this);
    QPushButton *acc0 = new QPushButton("0", this);
    QPushButton *acc1 = new QPushButton("1", this);
    QPushButton *acc2 = new QPushButton("2", this);
    QPushButton *acc3 = new QPushButton("3", this);
    acc0->setGeometry(520, 550 , 40, 40);
    acc1->setGeometry(520, 590 , 40, 40);
    acc2->setGeometry(520, 630 , 40, 40);
    acc3->setGeometry(520, 670 , 40, 40);
    accGroup->addButton(acc0, 31);
    accGroup->addButton(acc1, 32);
    accGroup->addButton(acc2, 33);
    accGroup->addButton(acc3, 34);
    connect(accGroup, SIGNAL(buttonClicked(int)), SLOT(click(int)));

    //기어 버튼 그룹
    QButtonGroup *gearGroup = new QButtonGroup(this);
    QPushButton *gearP = new QPushButton("P", this);
    QPushButton *gearN = new QPushButton("N", this);
    QPushButton *gearR = new QPushButton("R", this);
    QPushButton *gearD = new QPushButton("D", this);
    gearP->setGeometry(400, 300 , 40, 40);
    gearN->setGeometry(400, 340 , 40, 40);
    gearR->setGeometry(400, 380 , 40, 40);
    gearD->setGeometry(400, 420 , 40, 40);
    gearGroup->addButton(gearP, 41);
    gearGroup->addButton(gearN, 42);
    gearGroup->addButton(gearR, 43);
    gearGroup->addButton(gearD, 44);
    connect(gearGroup, SIGNAL(buttonClicked(int)), SLOT(click(int)));

    //음악 재생 그룹
    QButtonGroup *musicGroup = new QButtonGroup(this);
    QPushButton *musicL = new QPushButton("◀", this);
    QPushButton *musicR = new QPushButton("▶", this);
    QPushButton *musicPlay = new QPushButton("Play", this);
    musicL->setGeometry(130, 170 , 40, 40);
    musicR->setGeometry(330, 170 , 40, 40);
    musicPlay->setGeometry(230, 170 , 40, 40);
    musicGroup->addButton(musicL, 51);
    musicGroup->addButton(musicR, 52);
    musicGroup->addButton(musicPlay, 53);
    connect(musicGroup, SIGNAL(buttonClicked(int)), SLOT(click(int)));


    //비상등
    QButtonGroup *alertGroup = new QButtonGroup(this);
    QPushButton *alert = new QPushButton("ALERT", this);
    alertGroup->addButton(alert, 61);
    connect(alertGroup, SIGNAL(buttonClicked(int)), SLOT(click(int)));




}

Widget::~Widget()
{

}


void Widget::paintEvent(QPaintEvent *event){

    QPainter painter(this);
    painter.drawPixmap(0, 0, pixmap);

}

void Widget::readyRead()
{
    while(socket->canReadLine())
    {
        QString line = QString::fromUtf8(socket->readLine()).trimmed();
        qDebug() << line << "\n";
    }
}


void Widget::connected()
{
    QString SendStr;
    SendStr = QString("cluster");
//    SendStr = QString("Connect QT");
    socket->write(SendStr.toUtf8(), SendStr.length() + 1);
}


void Widget::click(int id)
{
    socket->connectToHost("192.168.100.49", 7777);
    QPushButton* button = (QPushButton*)((QButtonGroup*)sender())->button(id);
    qDebug() << id;
    QString SendStr;

    switch(id)
    {


         //방향지시등
        case 11: // left lights
//            QString SendStr;
//            socket->connectToHost("10.0.2.15", 7777);
            SendStr =QString("[Direction] 0");
            socket->write(SendStr.toUtf8(), SendStr.length() + 1);
            break;
        case 12: // right light
            SendStr =QString("[Direction] 1");
            socket->write(SendStr.toUtf8(), SendStr.length() + 1);
            break;

            //21-24 브레이크
        case 21:
            SendStr =QString("[Break] 0");
            socket->write(SendStr.toUtf8(), SendStr.length() + 1);
            break;
        case 22:
            SendStr =QString("[Break] 1");
            socket->write(SendStr.toUtf8(), SendStr.length() + 1);
            break;
        case 23:
            SendStr =QString("[Break] 2");
            socket->write(SendStr.toUtf8(), SendStr.length() + 1);
            break;
        case 24:
            SendStr =QString("[Break] 3");
            socket->write(SendStr.toUtf8(), SendStr.length() + 1);
            break;

        //31-34 엑셀
        case 31:
            SendStr =QString("[Accel] 0");
            socket->write(SendStr.toUtf8(), SendStr.length() + 1);
            break;
        case 32:
            SendStr =QString("[Accel] 1");
            socket->write(SendStr.toUtf8(), SendStr.length() + 1);
            break;
        case 33:
            SendStr =QString("[Accel] 2");
            socket->write(SendStr.toUtf8(), SendStr.length() + 1);
            break;
        case 34:
            SendStr =QString("[Accel] 3");
            socket->write(SendStr.toUtf8(), SendStr.length() + 1);
            break;

        //41-44 기어
        case 41:
            SendStr =QString("[Gear] P");
            socket->write(SendStr.toUtf8(), SendStr.length() + 1);
            break;
        case 42:
            SendStr =QString("[Gear] N");
            socket->write(SendStr.toUtf8(), SendStr.length() + 1);
            break;
        case 43:
            SendStr =QString("[Gear] R");
            socket->write(SendStr.toUtf8(), SendStr.length() + 1);
            break;
        case 44:
            SendStr =QString("[Gear] D");
            socket->write(SendStr.toUtf8(), SendStr.length() + 1);
            break;

        //51-53 음악
        case 51:
            SendStr =QString("[Music] 0");
            socket->write(SendStr.toUtf8(), SendStr.length() + 1);
            break;
        case 52:
            SendStr =QString("[Music] 1");
            socket->write(SendStr.toUtf8(), SendStr.length() + 1);
            break;
       case 53:
            SendStr =QString("[Music] 2");
            socket->write(SendStr.toUtf8(), SendStr.length() + 1);
            break;
        //61 비상등
        case 61:
            SendStr =QString("[Alert] 0");
            socket->write(SendStr.toUtf8(), SendStr.length() + 1);
            break;

    };
}

