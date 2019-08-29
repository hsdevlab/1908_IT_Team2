#include <QtWidgets>
#include "speed.h"


speed::speed(QWidget *parent) : QWidget(parent)
{


    //임시로 시간 잘 돌아가는지 보기
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(10);
    //전체 윈도우 사이즈 조정
    //755*565 pixel (클러스터 이미지 사이즈)
    this->setFixedSize(755, 565);
    if(!pixmap.load(":/cluster_cd.PNG")){
        qDebug() << "unable to load img";
        exit(-1);
    }

    //속도계 바늘 설정

    //소켓 및 통신 설정, 서버에 메시지 전송
    socket = new QTcpSocket(this);
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    socket->connectToHost("192.168.100.49", 7777);
    QString SendStr = QString("cluster");
    socket->write(SendStr.toUtf8(), SendStr.length()+1);

//C는 null로 체크, QT는 개행으로 check



    //음악 재생기
    QLabel *label = new QLabel(this);
    label->setGeometry(350, 230, 100, 20);
    label->setStyleSheet("QLabel {color: white;}");
    label->setText("Artist: ");


    QLabel *title = new QLabel(this);
    title->setGeometry(350, 250, 100, 20);
    title->setStyleSheet("QLabel {color: white;}");
    title->setText("Title: ");


    QMap<int, QString> songList;
    //저장 형태: 제목, 가수
    songList[0] = "ICY ITZY";
    songList[1] = "Stone of Zion Naul";

    //    player = new QMediaPlayer;

    //실제 읽어오는 노래
    QLabel *song_singer = new QLabel(this);
    song_singer->setGeometry(390, 230, 100, 20);
    song_singer->setStyleSheet("QLabel {color: white;}");
//    song_singer->setText("ss");
    //만약ECU에서 오는 값이 4 0이면 이전 곡
    //4 1이면 다음 곡
    //4 2면 PLAY
    //    player->setMedia(QUrl::fromLocalFile("./mp3/ICY.mp3"));


    QLabel *song_title = new QLabel(this);
    song_title->setGeometry(390, 250, 100, 20);
    song_title->setStyleSheet("QLabel {color: white;}");
//    song_title->setText("dd");

    //    player = new QMediaPlayer;
//    player->setMedia(QUrl::fromLocalFile("./mp3/ICY.mp3"));
//    player->setVolume(50);
//    player->play();
//    player->stop();




}


void speed::paintEvent(QPaintEvent *event){
    //현재 시간
    QTime time = QTime::currentTime();

    QPainter painter(this);
    painter.drawPixmap(0, 0, pixmap);

    painter.save();

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

//    painter.save();
    //rotate 내부 함수에서 돌아가는 부분 설정

    painter.rotate(-120);
    //각 270도가 되면 올라가지 않게.
    //속도 계산하는 로직 짜는데... 얘를 천천히 올라가도록 해야하고.....
    //-120도에서 120도 사이에서 왔다갔다 해야함.

//    int speed;
//    for(speed = -120; speed<120; speed++){
//        painter.rotate(speed);


//    }
    painter.rotate(6.0 * time.second() + time.msec()*0.006);

//    painter.brushOrigin()

    painter.drawConvexPolygon(speedNeedle, 3);

    painter.restore();
    painter.save();


    //---------------------------------------------------
    //RPM 바늘
    static const QPoint rpmNeedle[3] = {
        QPoint(7, 8),
        QPoint(-7, 8),
        QPoint(0, -70)
    };

    QColor rpmNeedleColor(127, 0, 127, 191);

    painter.setRenderHint(QPainter::Antialiasing);
    painter.translate(width()*5/6 - 10, height()/2);
    painter.scale(side/200.0, side/200.0);

    painter.setPen(Qt::NoPen);
    painter.setBrush(rpmNeedleColor);

    painter.rotate(-120);
    //속도 설정해주는 애랑 LCD 패널 안에 로직 구현하는 애만 클래스 따로 설계해서 넣어 줄 것...?
    painter.rotate(6.0 * time.second() + time.msec()*0.006);
    painter.drawConvexPolygon(rpmNeedle, 3);
//    painter.restore();


    //-----------------------------------------------------
    //LCD 패널
    //음악 재생, 주행거리, 지도, 낧씨, 연료량 표시
//    painter.drawRect(20, 20, 160, 160, 50, 80);
    //음악 재생기 UI
    //시작 위치 초기화

//    painter.translate(-((width()/6)*3 + 90), -(height()/2 + 9));
//    painter.rotate(-(6.0 * time.second() + time.msec()*0.006));
    //    painter.translate(0, 0);
    painter.restore();
//    painter.setPen(Qt::white);


    //-----------------------------------------------------
    //방향지시등

    //왼쪽 오른쪽 화살표


}


void speed::readyRead()
{
    while(socket->canReadLine())
    {
        QString line = QString::fromUtf8(socket->readLine()).trimmed();
        qDebug() << line << "\n";
    }
}



void speed::click(int id){

    QPushButton* button = (QPushButton*)((QButtonGroup*)sender())->button(id);
    qDebug() << id;
    QString SendStr;
    switch(id){

//        case 61:
//            socket->connectToHost("192.168.100.49", 7777);
//            SendStr = QString("cluster");
//            socket->write(SendStr.toUtf8(), SendStr.length()+1);
//            break;
    }


}




