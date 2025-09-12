#include "esphome/core/component.h"
#include "esphome/core/log.h"
#include "esphome/core/application.h"
#include "esphome/components/web_server/web_server.h"
#include <ArduinoJson.h>

namespace esphome {
namespace custom_web_server {

class WebServerListEntities : public Component {
 public:
  float get_setup_priority() const override { return setup_priority::AFTER_WIFI; }  // After web_server in IDF

  void setup() override {
    ESP_LOGD("WebServerListEntities", "Setting up /entities endpoint for ESP-IDF");

    // Access shared web_server instance (works in IDF)
    auto* ws = App.get_component<web_server::WebServer>();
    if (!ws) {
      ESP_LOGE("WebServerListEntities", "Built-in web_server not found; cannot register routes");
      return;
    }

    // Register /entities route (IDF HTTP handler)
    ws->get_server()->on("/entities", HTTP_GET, [this](AsyncWebServerRequest* req) {
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

        // Map entity type (IDF protobuf access)
        switch (entity.which()) {
          case api::ListEntities::LIST_ENTITIES_SENSOR: ent["type"] = "sensor"; break;
          case api::ListEntities::LIST_ENTITIES_BINARY_SENSOR: ent["type"] = "binary_sensor"; break;
          case api::ListEntities::LIST_ENTITIES_SWITCH: ent["type"] = "switch"; break;
          case api::ListEntities::LIST_ENTITIES_NUMBER: ent["type"] = "number"; break;
          case api::ListEntities::LIST_ENTITIES_LIGHT: ent["type"] = "light"; break;
          case api::ListEntities::LIST_ENTITIES_CLIMATE: ent["type"] = "climate"; break;
          case api::ListEntities::LIST_ENTITIES_TEXT_SENSOR: ent["type"] = "text_sensor"; break;
          case api::ListEntities::LIST_ENTITIES_FAN: ent["type"] = "fan"; break;
          case api::ListEntities::LIST_ENTITIES_COVER: ent["type"] = "cover"; break;
          case api::ListEntities::LIST_ENTITIES_SELECT: ent["type"] = "select"; break;
          default: ent["type"] = "unknown"; break;
        }
      }

      String json;
      serializeJson(doc, json);
      req->send(200, "application/json", json);
      ESP_LOGD("WebServerListEntities", "Sent /entities response with %d entities", entities.size());
    });

    ESP_LOGI("WebServerListEntities", "/entities route registered on shared web server (ESP-IDF)");
  }
};

}  // namespace custom_web_server
}  // namespace esphome