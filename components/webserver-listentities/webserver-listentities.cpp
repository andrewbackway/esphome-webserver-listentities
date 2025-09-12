#include "webserver-listentities.h"
#include "esphome/core/log.h"
#include "esphome/components/web_server_base/web_server_base.h"
#include "esphome/components/web_server_idf/web_server_idf.h"
#include <ArduinoJson.h>

namespace esphome {
namespace webserver_listentities {

static const char *const TAG = "webserver_listentities";

// ESP-IDF web server handler (snake_case virtuals)
class ListEntitiesHandler : public esphome::web_server_idf::AsyncWebHandler {
 public:
  bool can_handle(esphome::web_server_idf::AsyncWebServerRequest *request) {
    const auto url = request->url();
    const bool match = (url == "/entities" || url == "/entities/");
    ESP_LOGD(TAG, "can_handle url=%s match=%d", url.c_str(), match);
    return match;
  }

  void handle_request(esphome::web_server_idf::AsyncWebServerRequest *request) {
    ESP_LOGD(TAG, "handle_request /entities");

    ArduinoJson::JsonDocument doc;  // ArduinoJson v8
    auto root = doc.to<ArduinoJson::JsonObject>();
    auto entities = root["entities"].to<ArduinoJson::JsonArray>();

    // Populate entities if desired. Empty array is valid.

    std::string json;
    ArduinoJson::serializeJson(doc, json);
    request->send(200, "application/json", json.c_str());
  }
};

float WebServerListEntities::get_setup_priority() const {
  // After WiFi; WebServerBase attaches handlers when ready.
  return setup_priority::AFTER_WIFI;
}

void WebServerListEntities::setup() {
  ESP_LOGI(TAG, "Registering /entities endpoint (ESP-IDF)");
  auto *ws = esphome::web_server_base::global_web_server_base;
  if (!ws) {
    ESP_LOGE(TAG, "Web server not available; cannot register /entities");
    return;
  }
  ws->add_handler(new ListEntitiesHandler());
  ESP_LOGI(TAG, "Registered /entities endpoint");
}

}  // namespace webserver_listentities
}  // namespace esphome