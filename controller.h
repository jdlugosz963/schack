#ifndef CONTROLLER_H
#define CONTROLLER_H

class MP3Controller {
public:
  virtual void nextFile() = 0;
  virtual void previousFile() = 0;
  virtual void nextSection() = 0;
  virtual void previousSection() = 0;
};

class Controller {
  String root = "/";
  MP3Controller *mp3Controller;
public:
  Controller(MP3Controller*);

  void next();
  void nextS();
  void previous();
  void previousS();

  void setRoot(String);
  String getRoot();

  String getAPSSID() { return "ESP8266"; }
  String getAPPassword() { return "test1234"; }
};

#endif
