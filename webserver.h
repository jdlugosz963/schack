//#include <ESPAsyncTCP.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>

#include "controller.h"
#include <LittleFS.h>


#ifndef WEBSERVER_H
#define WEBSERVER_H

class WebServer {
  ESP8266WebServer *server;
  Controller *controller;
  
  bool setupST();
  bool setupAP();
  void setupRoutes();
  bool wifiStatus = false;
public:
  WebServer(Controller *controller);
  ~WebServer();
  void loop();
};

#endif
