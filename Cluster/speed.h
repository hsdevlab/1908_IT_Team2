#ifndef SPEED_H
#define SPEED_H

#include <QWidget>
#include <QtNetwork/QTcpSocket>
#include <QMediaPlayer>

class speed : public QWidget
{
    Q_OBJECT
public:
    explicit speed(QWidget *parent = nullptr);
    QPixmap pixmap;
    QPixmap cd;
    QMediaPlayer *player;

protected:
    void paintEvent(QPaintEvent *event);

private:
    QTcpSocket *socket;

signals:

public slots:
    void readyRead();
    void click(int);
};

#endif // SPEED_H
