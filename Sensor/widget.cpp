#include "widget.h"
#include <QPainter>
#include <QtWidgets>
#include <QPushButton>


Widget::Widget(QWidget *parent)
    : QWidget(parent)
{

    this->setFixedSize(564, 764);
    if(!pixmap.load(":/sensor.PNG")){
        qDebug() << "unable to load img";
        exit(-1);
    }
}

Widget::~Widget()
{

}


void Widget::paintEvent(QPaintEvent *event){

    QPainter painter(this);
    painter.drawPixmap(0, 0, pixmap);
    //방향 지시등
    QPushButton *LR = new QPushButton(this);
    LR->setGeometry(10, 10, 100, 100);



}
