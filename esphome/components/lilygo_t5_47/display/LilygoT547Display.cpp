#include "LilygoT547Display.h"

#include "esphome/core/log.h"
#include <string.h>
extern "C" {
#include "eink.h"
}

namespace esphome {
namespace lilygo_t5_47 {

static const char *const TAG = "lilygo_t5_47.display";

// Assuming there will be at least one display instance at a time
// Static initialization to save time on memory allocation in runtime,
// ans also workaround framgamted memory in ESPHome
static uint8_t fb[EINK_BUFFER_SIZE] = {0};

RTC_NOINIT_ATTR static uint32_t full_update_countdown_;

static int correct_adc_reference() {
  esp_adc_cal_characteristics_t adc_chars;
  esp_adc_cal_value_t val_type = esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 1100, &adc_chars);
  if (val_type == ESP_ADC_CAL_VAL_EFUSE_VREF) {
    return adc_chars.vref;
  }
  return 1100;
}

static inline bool convert_color(Color color) { return color.red != 0 || color.green != 0 || color.blue != 0; }

float LilygoT547Display::get_setup_priority() const { return esphome::setup_priority::LATE; }

int LilygoT547Display::get_width_internal() { return EINK_WIDTH; }

int LilygoT547Display::get_height_internal() { return EINK_HEIGHT; }

void LilygoT547Display::fill(Color color) { eink_buffer_set(fb, convert_color(color)); }

void LilygoT547Display::setup() {
  eink_init();

  if (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_UNDEFINED || full_update_countdown_ < 0 || full_update_countdown_ >= this->full_update_every_) {
    ESP_LOGD(TAG, "Resetting periodic full update countdown to 0.");
    full_update_countdown_ = 0;
  } else {
    ESP_LOGD(TAG, "Restored periodic full update countdown to %u.", full_update_countdown_);
  }
}

void LilygoT547Display::update() {
  this->do_update_();
  LilygoT547Display::flush_screen_changes();
}

void LilygoT547Display::flush_screen_changes() {
  eink_power_on();

  if (this->full_update_every_ > 0) {
    ESP_LOGD(TAG, "Periodic full update countdown %u.", full_update_countdown_);

    if (full_update_countdown_ == 0) {
      full_update_countdown_ = this->full_update_every_;
      ESP_LOGD(TAG, "Full update!");
      
      eink_flush(false);
      eink_flush(true);
    }

    full_update_countdown_--;
  }

  eink_render(fb);

  eink_power_off();
}

void LilygoT547Display::on_shutdown() { eink_deinit(); }

void HOT LilygoT547Display::draw_absolute_pixel_internal(int x, int y, Color color) {
  bool c = convert_color(color);
  eink_set_pixel(x, y, c, fb);
}

void LilygoT547Display::power_on() { eink_power_on(); }

void LilygoT547Display::power_off() { eink_power_off(); }

void LilygoT547Display::dump_config() {
  LOG_DISPLAY("", "Lilygo T5 47 Display", this);
  ESP_LOGCONFIG(TAG, "  Full Update Every: %u", this->full_update_every_);
  LOG_UPDATE_INTERVAL(this);
}

double_t LilygoT547Display::get_battery_voltage() {
  int vref = correct_adc_reference();
  int v = analogRead(36);
  return ((double_t) v / 4095.0) * 2.0 * 3.3 * (vref / 1000.0);
}

}  // namespace lilygo_t5_47
}  // namespace esphome
