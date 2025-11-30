#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"
#include "esp_log.h"

#define RELAY_GPIO      18
#define SOIL_ADC_CH     ADC1_CHANNEL_3    // GPIO4

static const char* TAG = "GARDEN";

void relay_task(void* pvParameters)
{
    gpio_reset_pin(RELAY_GPIO);
    gpio_set_direction(RELAY_GPIO, GPIO_MODE_OUTPUT);

    while (1) {
      
        gpio_set_level(RELAY_GPIO, 0);
        ESP_LOGI(TAG, "[RELAY] Valve CLOSED (watering stopped)");
        vTaskDelay(pdMS_TO_TICKS(5000));

        gpio_set_level(RELAY_GPIO, 1);
        ESP_LOGI(TAG, "[RELAY] Valve OPEN (watering...)");
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}

void soil_task(void* pvParameters)
{
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(SOIL_ADC_CH, ADC_ATTEN_DB_11);

    while (1) {
        int raw = adc1_get_raw(SOIL_ADC_CH);
        float volt = raw * 3.3f / 4095.0f;

        ESP_LOGI(TAG, "[SOIL] Raw=%d  Voltage=%.3fV  %s", raw, volt,
                 raw > 3200 ? "DRY!!!" :
                 raw > 2200 ? "A bit dry" :
                 raw > 1200 ? "Good" : "Wet");

        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

void app_main(void)
{
    ESP_LOGI(TAG, "===================================");
    ESP_LOGI(TAG, " ESP32-S3 Garden Node Started");
    ESP_LOGI(TAG, " Soil sensor on GPIO4, Relay on GPIO18 (NC mode)");
    ESP_LOGI(TAG, "===================================");

    xTaskCreate(soil_task, "soil", 2048, NULL, 5, NULL);
    xTaskCreate(relay_task, "relay", 2048, NULL, 5, NULL);
}