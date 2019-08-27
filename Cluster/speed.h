#ifndef SPEED_H
#define SPEED_H

#include <QWidget>

class speed : public QWidget
{
    Q_OBJECT
public:
    explicit speed(QWidget *parent = nullptr);
    QPixmap pixmap;

protected:
    void paintEvent(QPaintEvent *event);

signals:

public slots:
};

#endif // SPEED_H
