# ESPHome WebServer List Entities

An ESPHome external component that adds the missing `/components` HTTP endpoint to your device's web server, providing a JSON API to list all entities and their metadata.

## Overview

This component extends the built-in ESPHome web server by adding a new endpoint at `/components` that returns a JSON response containing information about all entities configured on your device. This is particularly useful for:

- Building custom dashboards or integrations
- Debugging entity configurations
- Discovering available entities programmatically
- Creating dynamic UI applications that need to know about device capabilities

## Features

- 🌐 Adds `/components` GET endpoint to existing ESPHome web server
- 📊 Returns comprehensive entity metadata (key, object_id, name, type)
- 🔧 Supports all major ESPHome entity types
- ⚡ ESP-IDF compatible
- 📱 JSON response format for easy integration
- 🛡️ Leverages existing web server security and configuration

## Supported Entity Types

- `sensor` - Sensors that provide numeric values
- `binary_sensor` - On/off sensors
- `switch` - Controllable switches
- `number` - Numeric input controls
- `light` - Lighting controls
- `climate` - Climate control (heating/cooling)
- `text_sensor` - Text-based sensors
- `fan` - Fan controls
- `cover` - Covers/blinds/shutters
- `select` - Selection controls
- `unknown` - Fallback for unrecognized types

## Installation

Add this component to your ESPHome configuration as an external component:

```yaml
external_components:
  - source:
      type: git
      url: https://github.com/andrewbackway/esphome-webserver-listcomponents
      components: ["webserver_listcomponents"]

# Enable the built-in web server (required)
web_server:
  port: 80

# Enable the list entities component
webserver_listcomponents:
```

## Prerequisites

- ESPHome with web server component enabled
- ESP32 or ESP8266 device
- Network connectivity (Wi-Fi configured)

## API Documentation

### GET /entities

Returns a JSON object containing an array of all entities configured on the device.

**Request:**
```http
GET /entities HTTP/1.1
Host: your-device-ip
```

**Response:**
```json
{
  "entities": [
    {
      "key": "sensor-temperature-123456",
      "object_id": "temperature", 
      "name": "Living Room Temperature",
      "type": "sensor"
    },
    {
      "key": "switch-relay-789012",
      "object_id": "relay",
      "name": "Main Relay",
      "type": "switch"
    },
    {
      "key": "binary_sensor-motion-345678",
      "object_id": "motion",
      "name": "Motion Detector",
      "type": "binary_sensor"
    }
  ]
}
```

**Response Fields:**
- `key` - Unique identifier for the entity
- `object_id` - The entity's object ID as defined in configuration
- `name` - Human-readable display name
- `type` - Entity type (sensor, switch, etc.)

**Status Codes:**
- `200` - Success
- `404` - Endpoint not found (component not configured or web server disabled)
- `500` - Internal server error

## Example Usage

### Python Integration
```python
import requests
import json

# Get entities from your ESPHome device
response = requests.get('http://192.168.1.100/components')
data = response.json()

for entity in data['entities']:
    print(f"Found {entity['type']}: {entity['name']} ({entity['object_id']})")
```

### JavaScript/Fetch
```javascript
fetch('http://192.168.1.100/components')
  .then(response => response.json())
  .then(data => {
    data.entities.forEach(entity => {
      console.log(`${entity.type}: ${entity.name}`);
    });
  });
```

### cURL
```bash
curl http://192.168.1.100/components | jq '.entities[] | {name, type}'
```

## Complete Configuration Example

```yaml
# ESPHome configuration example
esphome:
  name: my-device
  platform: ESP32
  board: esp32dev

wifi:
  ssid: "Your-WiFi"
  password: "your-password"

# Enable logging
logger:

# Enable Home Assistant API
api:

# Enable OTA updates
ota:

# Enable web server
web_server:
  port: 80
  auth:
    username: admin
    password: password

# Add the list entities component
external_components:
  - source:
      type: git
      url: https://github.com/andrewbackway/esphome-webserver-listcomponents
      components: ["webserver_listcomponents"]

webserver_listcomponents:

# Example entities that will appear in /entities
sensor:
  - platform: dht
    pin: GPIO4
    temperature:
      name: "Temperature"
    humidity:
      name: "Humidity"

switch:
  - platform: gpio
    pin: GPIO2
    name: "LED"

binary_sensor:
  - platform: gpio
    pin: GPIO0
    name: "Button"
```

## Technical Details

### Implementation Notes

- **ESP-IDF Compatible**: Designed specifically to work with ESP-IDF framework
- **Memory Efficient**: Uses a 4KB JSON buffer optimized for typical entity counts
- **Thread Safe**: Integrates safely with ESPHome's component lifecycle
- **Setup Priority**: Runs after Wi-Fi to ensure web server availability

### Dependencies

This component relies on:
- `esphome/components/web_server` - For the HTTP server infrastructure
- `AsyncWebServer` - For HTTP request handling
- `ArduinoJson` - For JSON serialization

### Logging

The component logs to the `WebServerListComponents` tag with different levels:
- `ESP_LOGD` - Debug information about setup and responses
- `ESP_LOGI` - Information about successful route registration  
- `ESP_LOGE` - Errors during setup or operation

## Troubleshooting

### Common Issues

**"Built-in web_server not found"**
- Ensure `web_server:` is configured in your YAML
- Verify the web server component is loading successfully

**404 on /entities endpoint**
- Check that `webserver-listcomponents:` is included in your configuration
- Verify the external component was loaded successfully
- Check device logs for setup errors

**Empty entities array**
- This is normal if no entities are configured on the device
- Add some sensors, switches, or other entities to see results

### Debug Logging

Enable debug logging to see detailed component operation:

```yaml
logger:
  level: DEBUG
  logs:
    WebServerListEntities: DEBUG
```

## Contributing

Issues and pull requests are welcome! Please feel free to contribute improvements, bug fixes, or additional features.

## License

This project follows the same license terms as ESPHome.