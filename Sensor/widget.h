#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPixmap>
#include <QDebug>
#include <QPushButton>
#include <QtNetwork/QTcpSocket>

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    QPixmap pixmap;

protected:
    void paintEvent(QPaintEvent *event);


private:
//    QPushButton btn;
    QTcpSocket *socket;

signals:
private slots:
    void readyRead();
    void connected();
    void click(int);
};

#endif // WIDGET_H
