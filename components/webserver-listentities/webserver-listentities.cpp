#include "esphome/core/component.h"
#include "esphome/core/log.h"
#include "esphome/core/application.h"
#include "esphome/components/web_server_base/web_server_base.h"
#include <ArduinoJson.h>

namespace esphome {
namespace custom_web_server {

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

    // Register /entities route (IDF HTTP handler)
    ws->add_handler("/entities", HTTP_GET, HTTP_GET, [](esphome::web_server_base::AsyncWebServerRequest* request) {
      /*
      ArduinoJson::JsonDocument doc(4096);  // v8 buffer
      auto entities = doc["entities"].as<ArduinoJson::JsonArray>();

      api::ListEntitiesIterator it;
      it.begin();
      while (it.has_next()) {
        it.next();
        const auto& entity = it.current();
        auto ent = entities.add<ArduinoJson::JsonObject>();
        ent["key"] = entity.key();
        ent["object_id"] = entity.object_id();
        ent["name"] = entity.name();

      }

      String json;
      serializeJson(doc, json);
      req->send(200, "application/json", json);
      */
      ESP_LOGD("WebServerListEntities", "Sent /entities response");
    });

    ESP_LOGI("WebServerListEntities", "/entities route registered on shared web server (ESP-IDF)");
  }
};

}  // namespace custom_web_server
}  // namespace esphome