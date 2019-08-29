#include "audio.h"
#include <QWidget>

audio::audio(QWidget *parent) : QWidget (parent)
{
    //임시로 오디오 UI 확인하기 위한 윈도우 창
      this->setFixedSize(500, 500);
//      QHash<QString, QString> song;
      //보통 인덱스만 가지고 있는 queu(딕셔너리)e 하나. 그 queue가
    //point하도록 하는 부분....?
      QMap<int, QString> songList;
      //저장 형태: 제목, 가수
      songList[0] = "ICY ITZY";
      songList[1] = "Stone of Zion Naul";




}
