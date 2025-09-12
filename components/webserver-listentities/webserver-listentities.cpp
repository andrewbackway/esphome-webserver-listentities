#include "esphome/core/component.h"
#include "esphome/core/log.h"
#include "esphome/core/application.h"
#include "esphome/components/web_server_base/web_server_base.h"
#include <ArduinoJson.h>
#include <memory>

namespace esphome {
namespace custom_web_server {

  
// Your custom handler class
class ListEntitiesHandler : public AsyncWebHandler {
 public:
  // The canHandle method checks if this handler should process the request.
  // It checks the URL and the HTTP method.
  bool canHandle(AsyncWebServerRequest* request) {
    // Check if the URL is "/list_entities" and the method is GET.
    if (request->url() == "/entities" && request->method() == HTTP_GET) {
      return true;
    }
    return false;
  }

  // The handleRequest method contains the logic for the request.
  void handleRequest(AsyncWebServerRequest* request) override {
    // Your custom logic to list entities goes here.
    request->send(200, "text/plain", "List of entities goes here.");
  }
};

class WebServerListEntities : public Component {
 public:
  float get_setup_priority() const override { return setup_priority::AFTER_WIFI; }  // After web_server in IDF

  void setup() override {
    ESP_LOGD("WebServerListEntities", "Setting up /entities endpoint for ESP-IDF");

    // Access shared web_server instance (works in IDF)
    auto* ws = esphome::web_server_base::global_web_server_base;
    if (!ws) {
      ESP_LOGE("WebServerListEntities", "Built-in web_server not found; cannot register routes");
      return;
    }

    if (ws != nullptr) {
      // Create a new instance of your custom handler and add it.
      // Use std::unique_ptr to manage the handler's memory.
      ws->add_handler(std::unique_ptr<ListEntitiesHandler>(new ListEntitiesHandler()));
    }

    ESP_LOGI("WebServerListEntities", "/entities route registered on shared web server (ESP-IDF)");
  }
};


}  // namespace custom_web_server
}  // namespace esphome