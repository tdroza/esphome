#include "my_sensor.h"
#include "esphome/core/log.h"

namespace esphome {
namespace my_sensor {

static const char *const TAG = "my_sensor.sensor";

void MySensor::setup() {
  ESP_LOGI(TAG, "Initialized");
}

void MySensor::update() {
  float new_value = 1.0;
  this->publish_state(new_value);

  ESP_LOGI(TAG, "Updated");
}

float MySensor::get_setup_priority() const { return esphome::setup_priority::LATE; }

void MySensor::dump_config() {
  LOG_SENSOR("", "My Sensor", this);
  LOG_UPDATE_INTERVAL(this);
}

}  // namespace my_sensor
}  // namespace esphome
