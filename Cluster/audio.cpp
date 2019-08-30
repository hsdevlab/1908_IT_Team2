#include "audio.h"
#include <QWidget>

audio::audio(QWidget *parent) : QWidget (parent)
{
    //임시로 오디오 UI 확인하기 위한 윈도우 창
      this->setFixedSize(500, 500);

      QMap<int, QString> songList;
      //저장 형태: 제목, 가수
      songList[0] = "ICY ITZY";
      songList[1] = "Stone of Zion Naul";




}
