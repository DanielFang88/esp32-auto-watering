#include "soil_sensor.h"
#include "relay.h"
#include "esp_log.h"

static const char* TAG = "GARDEN";

void watering_task(void* pvParameters)
{
    const uint32_t STABLE_DRY_TIME_MS = 10000;   // Water only after 10 seconds of dry
    const float    DRY_THRESHOLD      = 35.0f;   // active when lower than 35%
    const uint32_t WATERING_TIME_MS   = 8000;    // adjustable

    while (1) {
        float moisture = soil_get_moisture_percent();
        ESP_LOGI(TAG, "Filtered moisture: %.1f%%", moisture);

        // Three layers 
        if (soil_is_stable_dry(STABLE_DRY_TIME_MS, DRY_THRESHOLD)) {
            ESP_LOGW(TAG, "Stable dry detected for 10s → Starting 8s watering");
            relay_open();
            vTaskDelay(pdMS_TO_TICKS(WATERING_TIME_MS));
            relay_close();
            ESP_LOGI(TAG, "Quantitative watering finished");
            vTaskDelay(pdMS_TO_TICKS(300000));  // Does not repeat in 5 minutes
        } else {
            vTaskDelay(pdMS_TO_TICKS(2000));
        }
    }
}