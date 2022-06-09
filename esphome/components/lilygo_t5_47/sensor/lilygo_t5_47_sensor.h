#pragma once

#include <Arduino.h>

#include <driver/adc.h>
#include "esp_adc_cal.h"

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"

namespace esphome {
namespace lilygo_t5_47 {

class LilygoT547Sensor : public sensor::Sensor, public PollingComponent {
 public:
  void update() override;

  void dump_config() override;

  float get_setup_priority() const override;

  float get_battery_voltage();

  void set_always_on(bool value) { this->always_on_ = value; }
  bool get_always_on() const { return this->always_on_; }

 private:
  bool always_on_ = false;
};

}  // namespace lilygo_t5_47
}  // namespace esphome
