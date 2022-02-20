#pragma once
#include <Arduino.h>
#include <driver/adc.h>

#include "esp_adc_cal.h"
#include "esphome/components/display/display_buffer.h"
#include "esphome/components/display/display_color_utils.h"
#include "esphome/core/component.h"
#include "esphome/core/hal.h"

namespace esphome {
namespace lilygo_t5_47 {

class LilygoT547Display : public PollingComponent, public display::DisplayBuffer {
 public:
  float get_setup_priority() const override;

  void setup() override;
  void update() override;
  void dump_config() override;

  int get_width_internal() override;
  int get_height_internal() override;

  void fill(Color color) override;
  
  void on_shutdown() override;

  void set_full_update_every(uint32_t value) { this->full_update_every_ = value; }
  uint32_t get_full_update_every() { return this->full_update_every_; }

  void flush_screen_changes();
  void power_on();
  void power_off();

  double_t get_battery_voltage();

 protected:
  void HOT draw_absolute_pixel_internal(int x, int y, Color color) override;

 private:
  uint32_t full_update_every_;
  uint32_t full_update_countdown_{0};
};

}  // namespace lilygo_t5_47
}  // namespace esphome
