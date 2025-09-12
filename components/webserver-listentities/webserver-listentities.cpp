#include "esphome/core/component.h"
#include "esphome/core/log.h"
#include "esphome/core/application.h"
#include "esphome/components/web_server_base/web_server_base.h"
#include "esphome/components/web_server_idf/web_server_idf.h"  // IDF web server types
#include <ArduinoJson.h>

namespace esphome {
namespace custom_web_server {

// Custom handler class for ESP-IDF web server
class ListEntitiesHandler : public esphome::web_server_idf::AsyncWebHandler {
 public:
  bool canHandle(esphome::web_server_idf::AsyncWebServerRequest* request) override {
    const auto url = request->url();
    const auto method = request->method();
    const bool match = (url == "/entities" || url == "/entities/") &&
                       (method == esphome::web_server_idf::HTTP_GET);
    ESP_LOGD("ListEntitiesHandler", "canHandle url=%s method=%d match=%d", url.c_str(), (int) method, match);
    return match;
  }

  void handleRequest(esphome::web_server_idf::AsyncWebServerRequest* request) override {
    ESP_LOGD("ListEntitiesHandler", "handleRequest /entities");
    ArduinoJson::JsonDocument doc;
    auto root = doc.to<ArduinoJson::JsonObject>();
    auto entities = root["entities"].to<ArduinoJson::JsonArray>();

    // TODO: populate entities; sending an empty array is fine for now

    std::string json;
    ArduinoJson::serializeJson(doc, json);
    request->send(200, "application/json", json.c_str());
  }
};

class WebServerListEntities : public Component {
 public:
  float get_setup_priority() const override { return setup_priority::AFTER_WIFI; }  // after web_server init

  void setup() override {
    ESP_LOGD("WebServerListEntities", "Registering /entities endpoint (ESP-IDF)");
    auto* ws = esphome::web_server_base::global_web_server_base;
    if (!ws) {
      ESP_LOGE("WebServerListEntities", "Web server not available; cannot register /entities");
      return;
    }
    ws->add_handler(new ListEntitiesHandler());
    ESP_LOGI("WebServerListEntities", "Registered /entities endpoint");
  }
};

}  // namespace custom_web_server
}  // namespace esphome