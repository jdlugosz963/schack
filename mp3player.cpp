#include "mp3player.h"

MP3List::MP3List(File *dir, ListMode mode) {
  Serial.println("MP3List::MP3List");
  this->mode = mode;
  int size = countListElements(dir);

  list = new String[size];

  Serial.println(size);

  listSize = size;
  listCurrentPosition=0;

  insertElementsToList(dir);
  debug();
}

MP3List::~MP3List() {
  delete [] list;
}

void MP3List::debug() {
  Serial.println("============");
  for(int i=0; i<listSize; i++) {
    Serial.print(mode);
    Serial.print(": ");
    Serial.print(list[i]);
    Serial.print("\n\n");
  }
  Serial.print("List size");
  Serial.print(listSize);
  Serial.print("\n");

  Serial.print("List position");
  Serial.print(listCurrentPosition);
  Serial.print("\n");

  Serial.println("============");
}

bool MP3List::canAdd(File *file) {
  String fileName = file->name();

  switch(mode) {
  case ListMode::SECTIONS:
    Serial.println("SECTIONS");
    return file->isDirectory();
    break;

  case ListMode::MUSIC_FILES:
    Serial.println("MUSIC FILES");
    return fileName.endsWith(".mp3");
    break;

  default:
    Serial.println("DEFAULT");
    return false;
    break;
  }
}

int MP3List::countListElements(File *dir) {
  dir->rewindDirectory();

  if (!dir->isDirectory()) {
    Serial.println("Not a directory");
    return 0;
  }

  File file = dir->openNextFile();
  int elementsCount = 0;
  while (file) {
    if(canAdd(&file)) 
      elementsCount++;

    file.close();
    file = dir->openNextFile();
  }
  return elementsCount;
}

void MP3List::insertElementsToList(File *dir) {
  dir->rewindDirectory();

  Serial.println("MP3List::insertElementsToList");
  if (!dir->isDirectory()) {
    Serial.println("Not a directory");
    return;
  }

  File file = dir->openNextFile();
  int i=0;
  while (file) {
    if(canAdd(&file)) {
      if (i > listSize) 
	continue;
      list[i] = file.name();
      Serial.print("Dodaje: ");
      Serial.print(file.name());
      Serial.print("\n");

      Serial.print("na index: ");
      Serial.print(i);
      Serial.print("\n");
      i++;
    }
    file.close();
    file = dir->openNextFile();
  }
}

String MP3List::getCurrent() {
  Serial.print("list size: ");
  Serial.print(listSize);
  Serial.print("\n");

  Serial.print("list current position: ");
  Serial.print(listCurrentPosition);
  Serial.print("\n");

  if(listCurrentPosition < listSize)
    return list[listCurrentPosition];
  return "";
}


String MP3List::getNext() {
  if(listCurrentPosition < listSize-1)
    listCurrentPosition++;
  else
    listCurrentPosition = 0;

  return getCurrent();
}

String MP3List::getPrevious() {
  if(listCurrentPosition > 0)
    listCurrentPosition--;
  else
    listCurrentPosition = listSize-1;

  return getCurrent();
}


MP3Player::MP3Player() {
  Serial.println("MP3Player::MP3Player");

  source = new AudioFileSourceSD();
  mp3 = new AudioGeneratorMP3();
  out = new AudioOutputI2SNoDAC();
  updateListSections();
}

MP3Player::~MP3Player() {
  delete source;
  delete mp3;
  delete out;

  delete listSections;
  delete listMusicFiles;
}

void MP3Player::playMP3(String name) {
  Serial.println("MP3Player::playMP3");
  Serial.println(name);

  if (mp3->isRunning())
    mp3->stop();

  source->open(name.c_str());
  mp3->begin(source, out);
}

void MP3Player::loop() {
  if (mp3->isRunning())
    if (!mp3->loop()) mp3->stop();
}

void MP3Player::volumeUp() {
  if(volume + volumeBy < 2.0) {
    volume += volumeBy;
    out->SetGain(volume);
  }
}

void MP3Player::volumeDown() {
  if(volume - volumeBy > 0.0) {
    volume -= volumeBy;
    out->SetGain(volume);
  }
}

String MP3Player::getFilePath(String fileName) {
  Serial.println("MP3Player::getFilePath");
  String currentSection = listSections->getCurrent();

  if(currentSection == "" || fileName == "")
    return "";
  return rootDir+"/"+currentSection+"/"+fileName;
}

void MP3Player::nextFile() {
  Serial.println("MP3Player::next");
  String current = listMusicFiles->getNext();
  playMP3(getFilePath(current));
}

void MP3Player::previousFile() {
  Serial.println("MP3Player::previous");
  String current = listMusicFiles->getPrevious();
  playMP3(getFilePath(current));
}

void MP3Player::nextSection() {
  Serial.println("MP3Player::nextS");
  listSections->getNext();
  updateListMusicFiles();
  nextFile();
}

void MP3Player::previousSection() {
  Serial.println("MP3Player::previousS");
  listSections->getPrevious();
  updateListMusicFiles();
  nextFile();
}


bool MP3Player::updateListSections() {
  Serial.println("MP3Player::updateListSections");
  File root = SD.open(rootDir);
  if (!root) {
    Serial.println("Failed to open directory");
    return false;
  }

  if (!root.isDirectory()) {
    Serial.println("Not a directory");
    return false;
  }

  delete listSections;
  listSections = new MP3List(&root, ListMode::SECTIONS);
  //  root.rewindDirectory();
  return updateListMusicFiles();
}

bool MP3Player::updateListMusicFiles() {
  Serial.println("MP3Player::updateMusicFiles");
  String currentSection = listSections->getCurrent();

  Serial.println("CURRENT SECTION: ");
  Serial.println(currentSection);
  if(currentSection == "")
    return false;

  File root = SD.open(rootDir + "/" + currentSection);

  if (!root) {
    Serial.println("Failed to open directory");
    return false;
  }
  if (!root.isDirectory()) {
    Serial.println("Not a directory");
    return false;
  }

  delete listMusicFiles;
  listMusicFiles = new MP3List(&root, ListMode::MUSIC_FILES);
  //  root.rewindDirectory();
  return true;
}

