
#include "webserver-listentities.h"
#include "esphome/core/log.h"
#include "esphome/core/component.h"
#include "esphome/core/application.h"
#include "esphome/components/web_server/web_server.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/switch/switch.h"
#include "esphome/components/number/number.h"
#include "esphome/components/light/light_state.h"
#include "esphome/components/climate/climate.h"
#include "esphome/components/text_sensor/text_sensor.h"
#include "esphome/components/fan/fan.h"
#include "esphome/components/cover/cover.h"
#include "esphome/components/select/select.h"
#include <ArduinoJson.h>


namespace esphome {
namespace webserver_listentities {




float WebServerListEntities::get_setup_priority() const {
  return setup_priority::AFTER_WIFI;
}

void WebServerListEntities::setup() {
  ESP_LOGD("WebServerListEntities", "Setting up /entities endpoint for ESPHome");

  // Get the web server instance (singleton pattern in ESPHome)
  auto *ws = web_server::WebServer::get_instance();
  if (!ws) {
    ESP_LOGE("WebServerListEntities", "Web server not found; cannot register routes");
    return;
  }

  ws->get_server()->on("/entities", HTTP_GET, [this](AsyncWebServerRequest *req) {
    ArduinoJson::JsonDocument doc;
    auto entities = doc["entities"].to<ArduinoJson::JsonArray>();

    // Sensors
    for (auto *sensor : sensor::Sensor::get_sensors()) {
      auto ent = entities.add<ArduinoJson::JsonObject>();
      ent["object_id"] = sensor->get_object_id();
      ent["name"] = sensor->get_name();
      ent["type"] = "sensor";
    }
    // Binary Sensors
    for (auto *bs : binary_sensor::BinarySensor::get_binary_sensors()) {
      auto ent = entities.add<ArduinoJson::JsonObject>();
      ent["object_id"] = bs->get_object_id();
      ent["name"] = bs->get_name();
      ent["type"] = "binary_sensor";
    }
    // Switches
    for (auto *sw : switch_::Switch::get_switches()) {
      auto ent = entities.add<ArduinoJson::JsonObject>();
      ent["object_id"] = sw->get_object_id();
      ent["name"] = sw->get_name();
      ent["type"] = "switch";
    }
    // Numbers
    for (auto *num : number::Number::get_numbers()) {
      auto ent = entities.add<ArduinoJson::JsonObject>();
      ent["object_id"] = num->get_object_id();
      ent["name"] = num->get_name();
      ent["type"] = "number";
    }
    // Lights
    for (auto *light : light::LightState::get_lights()) {
      auto ent = entities.add<ArduinoJson::JsonObject>();
      ent["object_id"] = light->get_object_id();
      ent["name"] = light->get_name();
      ent["type"] = "light";
    }
    // Climate
    for (auto *climate : climate::Climate::get_climates()) {
      auto ent = entities.add<ArduinoJson::JsonObject>();
      ent["object_id"] = climate->get_object_id();
      ent["name"] = climate->get_name();
      ent["type"] = "climate";
    }
    // Text Sensors
    for (auto *text_sensor : text_sensor::TextSensor::get_text_sensors()) {
      auto ent = entities.add<ArduinoJson::JsonObject>();
      ent["object_id"] = text_sensor->get_object_id();
      ent["name"] = text_sensor->get_name();
      ent["type"] = "text_sensor";
    }
    // Fans
    for (auto *fan : fan::Fan::get_fans()) {
      auto ent = entities.add<ArduinoJson::JsonObject>();
      ent["object_id"] = fan->get_object_id();
      ent["name"] = fan->get_name();
      ent["type"] = "fan";
    }
    // Covers
    for (auto *cover : cover::Cover::get_covers()) {
      auto ent = entities.add<ArduinoJson::JsonObject>();
      ent["object_id"] = cover->get_object_id();
      ent["name"] = cover->get_name();
      ent["type"] = "cover";
    }
    // Selects
    for (auto *select : select::Select::get_selects()) {
      auto ent = entities.add<ArduinoJson::JsonObject>();
      ent["object_id"] = select->get_object_id();
      ent["name"] = select->get_name();
      ent["type"] = "select";
    }

    std::string json;
    ArduinoJson::serializeJson(doc, json);
    req->send(200, "application/json", json.c_str());
    ESP_LOGD("WebServerListEntities", "Sent /entities response with %d entities", entities.size());
  });

  ESP_LOGI("WebServerListEntities", "/entities route registered on web server");
}

}  // namespace webserver_listentities
}  // namespace esphome