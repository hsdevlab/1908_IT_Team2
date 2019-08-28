#include "client.h"

#include <QPushButton>
#include <QButtonGroup>
#include <QCheckBox> // 1 Modify

client::client(QWidget *parent)
    : QWidget(parent)
{
    this->setGeometry(400,400,600,400);
    socket = new QTcpSocket(this);

    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(socket, SIGNAL(connected()), this, SLOT(connected()));

    QPushButton* btn_Connect = new QPushButton("POWER ON", this);

    btn_Connect->setGeometry(10, 10, 150, 50);
    QButtonGroup *buttonGroup = new QButtonGroup(this);
    connect(buttonGroup, SIGNAL(buttonClicked(int)), SLOT(click(int)));
    buttonGroup->addButton(btn_Connect, 0);
}

client::~client()
{

}

void client::readyRead()
{
    while(socket->canReadLine())
    {
        QString line = QString::fromUtf8(socket->readLine()).trimmed();
        qDebug() << line << "\n";
    }
}


void client::connected()
{
    QString SendStr;
    SendStr = QString("cluster");
//    SendStr = QString("Connect QT");
    socket->write(SendStr.toUtf8(), SendStr.length() + 1);
}

void client::click(int id)
{
    QPushButton* button = (QPushButton*)((QButtonGroup*)sender())->button(id);

    QString SendStr;
    switch(id)
    {
        case 0: // Connect Server
            socket->connectToHost("192.168.100.38", 7777);
            break;
    };
}
