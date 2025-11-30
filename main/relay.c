#include "relay.h"
#include "driver/gpio.h"
#include "esp_log.h"

#define RELAY_GPIO 18
static const char* TAG = "RELAY";

void relay_init(void)
{
    gpio_reset_pin(RELAY_GPIO);
    gpio_set_direction(RELAY_GPIO, GPIO_MODE_OUTPUT);
    gpio_set_level(RELAY_GPIO, 0);        // 默认停水（LOW = 吸合 = NC断开）
    ESP_LOGI(TAG, "Relay initialized (NC safe mode)");
}

void relay_open(void)  { gpio_set_level(RELAY_GPIO, 1); ESP_LOGI(TAG, "Valve OPEN"); }
void relay_close(void) { gpio_set_level(RELAY_GPIO, 0); ESP_LOGI(TAG, "Valve CLOSED"); }
