#include <QtWidgets>
#include "speed.h"

speed::speed(QWidget *parent) : QWidget(parent)
{


    //임시로 시간 잘 돌아가는지 보기
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(10);
    //전체 윈도우 사이즈 조정
    //847*632 pixel (클러스터 이미지 사이즈)
    this->setFixedSize(847, 632);
    if(!pixmap.load(":/cluster_1.PNG")){
        qDebug() << "unable to load img";
        exit(-1);
    }
    //속도계 바늘 설정

}


void speed::paintEvent(QPaintEvent *event){
    //현재 시간
    QTime time = QTime::currentTime();

    QPainter painter(this);
    painter.drawPixmap(0, 0, pixmap);

    //속도계 바늘 설정
    static const QPoint speedNeedle[3] = {
        QPoint(7, 8),
        QPoint(-7, 8),
        QPoint(0, -70)
    };

    QColor speedNeedleColor(0, 127, 127, 191);

    //원주: 300pixel로 가정.
    int side = 300;
    painter.setRenderHint(QPainter::Antialiasing);
    painter.translate(width()/6 +10, height()/2);
    painter.scale(side/200.0, side/200.0);

    painter.setPen(Qt::NoPen);
    painter.setBrush(speedNeedleColor);

    painter.save();
    //rotate 내부 함수에서 돌아가는 부분 설정

    painter.rotate(-120);
    //각 270도가 되면 올라가지 않게.
    //속도 계산하는 로직 짜는데... 얘를 천천히 올라가도록 해야하고.....
    //-120도에서 120도 사이에서 왔다갔다 해야함.

    painter.rotate(6.0 * time.second() + time.msec()*0.006);

//    painter.brushOrigin()

    painter.drawConvexPolygon(speedNeedle, 3);

    painter.restore();







    //---------------------------------------------------
    //RPM 바늘
    static const QPoint rpmNeedle[3] = {
        QPoint(7, 8),
        QPoint(-7, 8),
        QPoint(0, -70)
    };

    QColor rpmNeedleColor(127, 0, 127, 191);
    //painter 좌표 위치 초기화
    painter.translate(-(width()/6 +10), -(height()/2 + 9));
    //원주: 300pixel로 가정.
    painter.setRenderHint(QPainter::Antialiasing);
    painter.translate((width()/6)*3 + 90, height()/2 + 9);

    painter.setPen(Qt::NoPen);
    painter.setBrush(rpmNeedleColor);

    painter.save();
    painter.rotate(-120);
    //속도 설정해주는 애랑 LCD 패널 안에 로직 구현하는 애만 클래스 따로 설계해서 넣어 줄 것...?
    painter.rotate(6.0 * time.second() + time.msec()*0.006);
    painter.drawConvexPolygon(rpmNeedle, 3);
    painter.restore();


    //-----------------------------------------------------
    //LCD 패널
    //음악 재생, 주행거리, 지도, 낧씨, 연료량 표시
//    painter.drawRect(20, 20, 160, 160, 50, 80);


    //-----------------------------------------------------
    //방향지시등

    //왼쪽 오른쪽 화살표

}



