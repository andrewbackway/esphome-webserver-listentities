#include "webserver-listentities.h"
#include "esphome/core/log.h"
#include "esphome/core/component.h"
#include "esphome/core/application.h"  
#include "esphome/components/web_server/web_server.h"
#include "esphome/components/api/list_entities.h"
//#include "AsyncWebServer.h"
#include <ArduinoJson.h>


namespace esphome {
namespace webserver_listentities {



float WebServerListEntities::get_setup_priority() const {
  return setup_priority::AFTER_WIFI;
}

void WebServerListEntities::setup() {
  ESP_LOGD("WebServerListEntities", "Setting up /entities endpoint for ESPHome");

  auto *ws = web_server::global_web_server;
  if (!ws) {
    ESP_LOGE("WebServerListEntities", "Web server not found; cannot register routes");
    return;
  }

  ws->add_handler("/entities", HTTP_GET, [this](web_server::Request &req) {
    ArduinoJson::JsonDocument doc;
    auto entities = doc["entities"].to<ArduinoJson::JsonArray>();

    for (auto *comp : App.get_components()) {
      ArduinoJson::JsonObject ent;
      if (auto *sensor = dynamic_cast<esphome::sensor::Sensor *>(comp)) {
        ent = entities.add<ArduinoJson::JsonObject>();
        ent["object_id"] = sensor->get_object_id();
        ent["name"] = sensor->get_name();
        ent["type"] = "sensor";
      } else if (auto *bs = dynamic_cast<esphome::binary_sensor::BinarySensor *>(comp)) {
        ent = entities.add<ArduinoJson::JsonObject>();
        ent["object_id"] = bs->get_object_id();
        ent["name"] = bs->get_name();
        ent["type"] = "binary_sensor";
      } else if (auto *sw = dynamic_cast<esphome::switch_::Switch *>(comp)) {
        ent = entities.add<ArduinoJson::JsonObject>();
        ent["object_id"] = sw->get_object_id();
        ent["name"] = sw->get_name();
        ent["type"] = "switch";
      } else if (auto *num = dynamic_cast<esphome::number::Number *>(comp)) {
        ent = entities.add<ArduinoJson::JsonObject>();
        ent["object_id"] = num->get_object_id();
        ent["name"] = num->get_name();
        ent["type"] = "number";
      } else if (auto *light = dynamic_cast<esphome::light::LightState *>(comp)) {
        ent = entities.add<ArduinoJson::JsonObject>();
        ent["object_id"] = light->get_object_id();
        ent["name"] = light->get_name();
        ent["type"] = "light";
      } else if (auto *climate = dynamic_cast<esphome::climate::Climate *>(comp)) {
        ent = entities.add<ArduinoJson::JsonObject>();
        ent["object_id"] = climate->get_object_id();
        ent["name"] = climate->get_name();
        ent["type"] = "climate";
      } else if (auto *text_sensor = dynamic_cast<esphome::text_sensor::TextSensor *>(comp)) {
        ent = entities.add<ArduinoJson::JsonObject>();
        ent["object_id"] = text_sensor->get_object_id();
        ent["name"] = text_sensor->get_name();
        ent["type"] = "text_sensor";
      } else if (auto *fan = dynamic_cast<esphome::fan::Fan *>(comp)) {
        ent = entities.add<ArduinoJson::JsonObject>();
        ent["object_id"] = fan->get_object_id();
        ent["name"] = fan->get_name();
        ent["type"] = "fan";
      } else if (auto *cover = dynamic_cast<esphome::cover::Cover *>(comp)) {
        ent = entities.add<ArduinoJson::JsonObject>();
        ent["object_id"] = cover->get_object_id();
        ent["name"] = cover->get_name();
        ent["type"] = "cover";
      } else if (auto *select = dynamic_cast<esphome::select::Select *>(comp)) {
        ent = entities.add<ArduinoJson::JsonObject>();
        ent["object_id"] = select->get_object_id();
        ent["name"] = select->get_name();
        ent["type"] = "select";
      }
    }

    std::string json;
    ArduinoJson::serializeJson(doc, json);
    req.send(200, "application/json", json.c_str());
    ESP_LOGD("WebServerListEntities", "Sent /entities response with %d entities", entities.size());
  });

  ESP_LOGI("WebServerListEntities", "/entities route registered on web server");
}

}  // namespace webserver_listentities
}  // namespace esphome