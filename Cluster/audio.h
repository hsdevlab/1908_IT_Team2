#ifndef AUDIO_H
#define AUDIO_H

#include <QWidget>
#include <QMediaPlayer>
class audio : public QWidget
{
    Q_OBJECT
public:
    explicit audio(QWidget *parent = nullptr);
    QMediaPlayer *player;

protected:
signals:
public slots:


};

#endif // AUDIO_H
