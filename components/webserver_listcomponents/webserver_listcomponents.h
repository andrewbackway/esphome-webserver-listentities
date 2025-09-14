#pragma once

#include "esphome/core/component.h"

namespace esphome {
namespace webserver_listcomponents {

class WebServerListComponents : public Component {
 public:
  float get_setup_priority() const override;
  void setup() override;
  void dump_config() override; 
};

}  // namespace webserver_listcomponents
}  // namespace esphome