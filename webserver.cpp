#include "webserver.h"


WebServer::WebServer(Controller *controller) {
  this->controller = controller;
  this->server = new ESP8266WebServer(80);

  if(!setupST()) {
    Serial.println("Cos sie zjebalo!!");
    return;
  }

  if(!LittleFS.begin()){
    Serial.println("An Error has occurred while mounting LittleFS");
    return;
  }
  setupRoutes();
}

WebServer::~WebServer() {
  delete server;
}

void WebServer::setupRoutes() {
  server->serveStatic("/", LittleFS, "/index.html", "max-age=43200");
  server->serveStatic("/script.js", LittleFS, "/script.js", "max-age=43200");
  server->serveStatic("/style.css", LittleFS, "/style.css", "max-age=43200");

  server->on("/next", [this]() {
    this->server->send(200, "text/plain", "Next ok!");
    this->controller->next();
  });

  server->on("/previous", [this]() {
    this->server->send(200, "text/plain", "Previous ok!");
    this->controller->previous();
  });

  server->on("/nexts", [this]() {
    this->server->send(200, "text/plain", "Next_S ok!");
    this->controller->nextS();
  });

  server->on("/previouss", [this]() {
    this->server->send(200, "text/plain", "Previous_S ok!");
    this->controller->previousS();
  });

  server->begin();
}

void WebServer::loop() {
  server->handleClient();
}

bool WebServer::setupAP () {
  WiFi.softAP(controller->getAPSSID(), controller->getAPPassword());
  wifiStatus = WiFi.status();
  return wifiStatus;
}

bool WebServer::setupST () {
  Serial.println("Station connecting: ");
  
  WiFi.begin("OpenWrt2");
  while (WiFi.status() != WL_CONNECTED)
    {
      delay(500);
      Serial.print(".");
    }
  Serial.println();
  
  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
  
  wifiStatus = WiFi.status();
  return wifiStatus;
}
