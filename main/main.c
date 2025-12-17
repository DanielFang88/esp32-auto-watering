#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "soil_sensor.h"
#include "relay.h"

static const char* TAG = "GARDEN";

void watering_task(void* pvParameters)
{
    const uint32_t STABLE_DRY_TIME_MS = 10000;   // 连续10秒干才浇水
    const float DRY_THRESHOLD = 35.0f;           // 低于35%触发
    const uint32_t WATERING_TIME_MS = 8000;      // 定量浇8秒

    while (1) {
        float moisture = soil_get_moisture_percent();
        ESP_LOGI(TAG, "Filtered moisture: %.1f%%", moisture);

        if (soil_is_stable_dry(STABLE_DRY_TIME_MS, DRY_THRESHOLD)) {
            ESP_LOGW(TAG, "Stable dry for 10s → Starting 8s watering");
            relay_open();                            // 出水
            vTaskDelay(pdMS_TO_TICKS(WATERING_TIME_MS));
            relay_close();                           // 停水
            ESP_LOGI(TAG, "Watering finished. Sleep 5min to avoid repeat");
            vTaskDelay(pdMS_TO_TICKS(300000));        // 浇完5分钟内不再浇
        } else {
            vTaskDelay(pdMS_TO_TICKS(2000));
        }
    }
}

void app_main(void)
{
    ESP_LOGI(TAG, "=======================================");
    ESP_LOGI(TAG, " ESP32-S3 Smart Garden Node v1.0");
    ESP_LOGI(TAG, " Auto watering when soil < 35%% (stable 10s)");
    ESP_LOGI(TAG, "=======================================");

    soil_sensor_init();
    relay_init();

    xTaskCreate(watering_task, "watering", 4096, NULL, 5, NULL);
}
