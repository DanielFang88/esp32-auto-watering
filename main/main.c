#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "esp_log.h"
#include "relay.h"
#include "soil_sensor.h"

static const char* TAG = "GARDEN";

void watering_task(void* pvParameters)
{
    const uint32_t WATERING_DURATION_MS = 8000;   // 水浇 8 秒
    const uint32_t CHECK_INTERVAL_MS    = 60000;  // 每分钟检查一次

    while (1) {
        float moisture = soil_get_moisture_percent();

        ESP_LOGI(TAG, "Current soil moisture: %.1f%%", moisture);

        if (moisture < 35.0f) {                        // 低于 35% 就浇水
            ESP_LOGW(TAG, "Soil too dry! Starting watering...");
            relay_open();                             // NC 方案：HIGH = 出水
            vTaskDelay(pdMS_TO_TICKS(WATERING_DURATION_MS));
            relay_close();                            // LOW = 停水
            ESP_LOGI(TAG, "Watering finished");
        } else {
            ESP_LOGI(TAG, "Soil moisture OK, sleeping...");
        }

        vTaskDelay(pdMS_TO_TICKS(CHECK_INTERVAL_MS));
    }
}

void app_main(void)
{
    ESP_LOGI(TAG, "=======================================");
    ESP_LOGI(TAG, " ESP32-S3 Smart Garden Node v1.0");
    ESP_LOGI(TAG, " Auto watering when soil < 35%%");
    ESP_LOGI(TAG, "=======================================");

    soil_sensor_init();
    relay_init();

    xTaskCreate(watering_task, "watering", 4096, NULL, 5, NULL);
}
