from esphome import config_validation as cv
from esphome.const import CONF_ID

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id("custom_web_server::WebServerListEntities"),
})

# Ensure ESPAsyncWebServer is included in the build
DEPENDENCIES = ["web_server"]
LIB_DEPS = ["esphome/ESPAsyncWebServer@2.7.0"]  # Matches ESPHome's bundled version