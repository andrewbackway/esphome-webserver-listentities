from esphome import config_validation as cv
from esphome.const import CONF_ID

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id("custom_web_server::WebServerListEntities"),
})

# Dependencies to include API and web_server headers
DEPENDENCIES = ["api", "web_server"]

# ESPAsyncWebServer lib dep (from previous fix)
LIB_DEPS = ["esphome/ESPAsyncWebServer@2.7.0"]