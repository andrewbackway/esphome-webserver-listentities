#pragma once

#include "esphome/core/component.h"

namespace esphome {
namespace webserver_listentities {

class WebServerListEntities : public Component {
 public:
  float get_setup_priority() const override;
  void setup() override;
};

}  // namespace custom_web_server
}  // namespace esphome