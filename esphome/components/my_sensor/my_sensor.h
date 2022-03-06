#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"

namespace esphome {
namespace my_sensor {

class LilygoT547Sensor : public sensor::Sensor, public PollingComponent {
 public:
  void setup() override;
  void update() override;

  void dump_config() override;

  float get_setup_priority() const override;

};

}  // namespace my_sensor
}  // namespace esphome
