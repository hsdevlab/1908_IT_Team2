#include <QtWidgets>
#include "speed.h"
#include <QString>
#include <QFileInfo>
#include <QUrl>

speed::speed(QWidget *parent) : QWidget(parent)
{


    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(10);
    //전체 윈도우 사이즈 조정
    //755*565 pixel (클러스터 이미지 사이즈)
    this->setFixedSize(756, 563);
    if(!pixmap.load(":/cluster.PNG")){
        qDebug() << "unable to load img";
        exit(-1);
    }
    if(!lArrow.load(":/lArrow.PNG")){
        qDebug() << "unable to load img";
        exit(-1);
    }
    if(!rArrow.load(":/rArrow.PNG")){
        qDebug() << "unable to load img";
        exit(-1);
    }

    //시계
    lcd = new QLCDNumber(8, this);
    lcd->setGeometry(230, 150, 100, 50);
    lcd->setFrameStyle(0);
    connect(timer, SIGNAL(timeout()), SLOT(displayClock()));    timer->start(1000);
    timer->start(1000);

    //연료량
    fuelLabel = new QLabel(this);
    fuelLabel->setGeometry(300, 325, 200, 13);
    fuelLabel->setStyleSheet("QLabel {color: white;}");
    //QString("%1").arg(fuel);
//    fuelLabel->setText("hi");
//    fuelLabel->setText(QString("%1").arg(fuel));

    //주행거리

    disLabel = new QLabel(this);
    disLabel->setGeometry(420, 325, 200, 13);
    disLabel->setStyleSheet("QLabel {color: white;}");
    //QString("%1").arg(fuel);
//    fuelLabel->setText("hi");
//    disLabel->setText(QString("%1").arg(distance));

    //기어

    QLabel *gearP = new QLabel(this);
    gearP->setGeometry(360, 165, 13, 13);
    gearP->setText("P  ");
//    gearP->setStyleSheet("QLabel {color: white;}");

    QLabel *gearN = new QLabel(this);
    gearN->setGeometry(373, 165, 200, 13);
    gearN->setText("N  ");

    QLabel *gearR = new QLabel(this);
    gearR->setGeometry(390, 165, 200, 13);
    gearR->setText("R  ");

    QLabel *gearD = new QLabel(this);
    gearD->setGeometry(403, 165, 200, 13);
    gearD->setText("D  ");


    if(gear == 0){
        gearP->setStyleSheet("QLabel {color: white;}");
        gearN->setStyleSheet("QLabel {color: black;}");
        gearR->setStyleSheet("QLabel {color: black;}");
        gearD->setStyleSheet("QLabel {color: black;}");
    }
    else if(gear ==1){
        gearP->setStyleSheet("QLabel {color: black;}");
        gearN->setStyleSheet("QLabel {color: white;}");
        gearR->setStyleSheet("QLabel {color: black;}");
        gearD->setStyleSheet("QLabel {color: black;}");
    }
    else if(gear ==2){
        gearP->setStyleSheet("QLabel {color: black;}");
        gearN->setStyleSheet("QLabel {color: black;}");
        gearR->setStyleSheet("QLabel {color: white;}");
        gearD->setStyleSheet("QLabel {color: black;}");
    }
    else if(gear ==3){
        gearP->setStyleSheet("QLabel {color: black;}");
        gearN->setStyleSheet("QLabel {color: black;}");
        gearR->setStyleSheet("QLabel {color: black;}");
        gearD->setStyleSheet("QLabel {color: white;}");}
    else{}


    //소켓 및 통신 설정, 서버에 메시지 전송
    socket = new QTcpSocket(this);
    socket->connectToHost("192.168.100.38", 7777);
    QString SendStr = QString("cluster");
    socket->write(SendStr.toUtf8(), SendStr.length()+1);
    connect(socket, SIGNAL(readyRead()), this, SLOT(getData()));



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
    songList[0] = "DNA";
    songList[1] = "12";
    songList[3] = "Engine";
    //    player = new QMediaPlayer;
    QMap<int, QString> artistList;
    //저장 형태: 제목, 가수
    artistList[0] = "BTS";
    artistList[1] = "_0";
    artistList[3] = "_1";

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


     player = new QMediaPlayer(this);
     playerList= new QMediaPlaylist();
     //playerList->addMedia(QUrl::fromLocalFile(QFileInfo())));
     playerList->addMedia(QUrl::fromLocalFile(QFileInfo("C:/Users/MOBIS/Desktop/DNA.mp3").absoluteFilePath()));
     playerList->addMedia(QUrl::fromLocalFile(QFileInfo("C:/Users/MOBIS/Desktop/12.mp3").absoluteFilePath()));
     playerList->addMedia(QUrl::fromLocalFile(QFileInfo("C:/Users/MOBIS/Desktop/start_engine_1.wav").absoluteFilePath()));
     //"C:\\prj/1908_IT_Team2/Cluster/mp3/Stone_Of_Zion.mp3"



     playerVideo = new QVideoWidget(this);

     player->setVolume(50);
     player->setPlaybackRate(1);
     player->setVideoOutput(playerVideo);

     playerList->setCurrentIndex(0);
     player->setPlaylist(playerList);

     int musicIndex = 0;

     if(music == 0){
         player->stop();
     }

     //play
     else if(music ==1){
         playerList->setCurrentIndex(musicIndex);

         song_title->setText(songList[musicIndex]);
         song_singer->setText(artistList[musicIndex]);

         qDebug() << QFileInfo("C:/Users/MOBIS/Desktop/DNA.mp3").fileName();
         player->setVolume(100);
         player->play();
     }
     //next
     else if (music ==2){
         musicIndex++;
         song_title->setText(songList[musicIndex]);
         song_singer->setText(artistList[musicIndex]);
         playerList->setCurrentIndex(musicIndex);
         player->play();
     }
//     prev
     else if(music ==3){
         musicIndex--;
         song_title->setText(songList[musicIndex]);
         song_singer->setText(artistList[musicIndex]);
         playerList->setCurrentIndex(musicIndex);
         player->play();
     }
     else{}





}


void speed::displayClock(){
    //

//    label->setText(QString("%1 %2").arg(QDate::currentDate().toString()).arg(QTime::currentTime().toString()));
////    label->setText(QTime::currentTime().toString());
    lcd->display(QString("%1").arg(QTime::currentTime().toString()));

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

    painter.rotate(-120);
    painter.rotate(speed_ECU);
//    painter.rotate(6.0 * time.second() + time.msec()*0.006);

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
    painter.rotate(RPM);

    //    painter.rotate(6.0 * time.second() + time.msec()*0.006);
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
    //방향지시등
    painter.restore();
    painter.save();
//    light = 3;
    if(light == 0){

    }
    else if(light ==1 ){

        painter.restore();
        painter.save();
        painter.drawPixmap(200, 120, lArrow);
    }
    else if(light ==2 ){
        painter.restore();
        painter.save();
        painter.drawPixmap(400, 110, rArrow);
    }
    else if(light ==3 ){
        painter.restore();
        painter.save();
        painter.drawPixmap(400, 110, rArrow);
        painter.restore();
        painter.save();
        painter.drawPixmap(200, 120, lArrow);

    }
    else{}


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


void speed::getData(){

    QTcpSocket *ecu = (QTcpSocket*)sender();
    QPainter painter(this);

    while(ecu->canReadLine())
    {

        QString line = QString::fromUtf8(ecu->readLine()).trimmed();


        QStringList msgs = line.split(" ");
        //0 속도
        if(msgs[0].toInt() == 0){
            speed_ECU = msgs[1].toInt();
        }
        //1 주행거리
        else if(msgs[0].toInt() == 1){
            distance = msgs[1].toInt();
            disLabel->setText(QString("%1").arg(distance));

            qDebug() <<"dis" << distance << "\n";

        }
        //2 연료량
        else if(msgs[0].toInt() == 2){
            fuel = msgs[1].toInt();
            fuelLabel->setText(QString("%1").arg(fuel));
            qDebug() <<"fu" << fuel << "\n";


        }
        //3 RPM
        else if(msgs[0].toInt() == 3){
            RPM = msgs[1].toInt();
            qDebug() <<"rpm" <<RPM << "\n";


        }
        //4 기어
        else if(msgs[0].toInt() == 4){
            gear = msgs[1].toInt();


        }
        //5 깜박이
        else if(msgs[0].toInt() == 5){
            light = msgs[1].toInt();
//            if(light == 0){

//            }
//            else if(light ==1 ){

//                painter.restore();
//                painter.save();
//                painter.drawPixmap(200, 120, lArrow);
//            }
//            else if(light ==2 ){
//                painter.restore();
//                painter.save();
//                painter.drawPixmap(400, 110, rArrow);
//            }
//            else if(light ==3 ){
//                painter.restore();
//                painter.save();
//                painter.drawPixmap(400, 110, rArrow);
//                painter.restore();
//                painter.save();
//                painter.drawPixmap(200, 120, lArrow);

//            }
//            else{}

        }
        //6 음악
        else if(msgs[0].toInt() == 6){
            music = msgs[1].toInt();


        }
        else {

        }
    }


}




