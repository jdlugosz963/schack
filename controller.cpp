#include "Arduino.h"
#include "controller.h"


Controller::Controller(MP3Controller *mp3Controller) {
  this->mp3Controller = mp3Controller;
}

void Controller::next() {
  mp3Controller->nextFile();
}

void Controller::nextS() {
  mp3Controller->nextSection();
}

void Controller::previousS() {
  mp3Controller->previousSection();
}

void Controller::previous() {
  mp3Controller->previousFile();
}

void Controller::setRoot(String newRoot) { root = newRoot; } 
String Controller::getRoot() { return root; } 
