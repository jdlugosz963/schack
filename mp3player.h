#ifndef MP3_PLAYER_H
#define MP3_PLAYER_H

#include "AudioOutputI2SNoDAC.h"
#include "AudioGeneratorMP3.h"
#include "AudioFileSourceSD.h"
#include "AudioLogger.h"
#include "controller.h"
#include "SD.h"
#include <iostream>


enum ListMode {
  SECTIONS=0,
  MUSIC_FILES=1
};

class MP3List {
  String *list;
  int listSize = 0;
  int listCurrentPosition = 0;
  int countListElements(File*);
  void insertElementsToList(File*);
  bool canAdd(File*);
  void debug();
  ListMode mode;

public:
  MP3List(File*, ListMode);
  ~MP3List();
  String getCurrent();
  String getNext();
  String getPrevious();
};


class MP3Player : public MP3Controller {
  AudioFileSourceSD *source;
  AudioGeneratorMP3 *mp3;
  AudioOutputI2SNoDAC *out;

  MP3List *listMusicFiles;
  MP3List *listSections;

  String currentFolder;
  float volume=2.0;
  float const volumeBy = 0.3;

  String rootDir = "/";

  void playMP3(String);
  bool updateListSections();
  bool updateListMusicFiles();
  String getFilePath(String);


 public:
  MP3Player();
  ~MP3Player();
  void volumeUp();
  void volumeDown();
  void nextSection() override;
  void previousSection() override;
  void nextFile() override;
  void previousFile() override;
  void loop();
};

#endif
