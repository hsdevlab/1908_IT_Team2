#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPixmap>
#include <QDebug>
#include <QPushButton>

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
    QPushButton btn;

signals:
public slots:
};

#endif // WIDGET_H
