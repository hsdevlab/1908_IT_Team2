#ifndef SPEED_H
#define SPEED_H

#include <QWidget>
#include <QtNetwork/QTcpSocket>
#include <QMediaPlayer>
#include <QLCDNumber>
#include <QLabel>
#include <QMediaPlaylist>
#include <QVideoWidget>

class speed : public QWidget
{
    Q_OBJECT

public:
    explicit speed(QWidget *parent = nullptr);
    QPixmap pixmap;
    QPixmap cd;
    QPixmap lArrow;
    QPixmap rArrow;
    QMediaPlayer *player;
    QMediaPlaylist *playerList;
    QVideoWidget *playerVideo;
    QLCDNumber *lcd;
    QLabel *fuelLabel;
    QLabel *disLabel;
    int musicFlag;
//    QPainter painter(speed);
    int speed_ECU;
    int distance;
    int fuel;
    int RPM;
    int gear;
    int light;
    int music;



protected:
    void paintEvent(QPaintEvent *event);

private:
    QTcpSocket *socket;

signals:
    void getData(QString msg);
    void message_siganal(QString msg);

public slots:
    void readyRead();
    void getData();
    void click(int);
    void displayClock();
};

#endif // SPEED_H
