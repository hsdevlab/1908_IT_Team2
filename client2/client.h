#ifndef CLIENT_H
#define CLIENT_H

#include <QWidget>
#include <QtNetwork/QTcpSocket>
#include <QDebug>

class client : public QWidget
{
    Q_OBJECT

public:
    client(QWidget *parent = 0);
    ~client();

private slots:
    void readyRead();
    void connected();

    void click(int);

private:
    QTcpSocket *socket;
};

#endif // CLIENT_H
