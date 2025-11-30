// main/soil_sensor.c
#include "soil_sensor.h"
#include "driver/adc.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define SOIL_ADC        ADC1_CHANNEL_3
#define SAMPLE_COUNT    20                    
#define DRY_VALUE       3400                  
#define WET_VALUE       1250                  

static const char* TAG = "SOIL";
static int samples[SAMPLE_COUNT] = {0};
static int idx = 0;
static int sum = 0;
static bool initialized = false;

void soil_sensor_init(void)
{
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(SOIL_ADC, ADC_ATTEN_DB_11);
    ESP_LOGI(TAG, "Capacitive soil sensor + moving average filter initialized");
}

float soil_get_moisture_percent(void)
{
    int raw = adc1_get_raw(SOIL_ADC);

    // moving the average filter
    if (!initialized) {
        for (int i = 0; i < SAMPLE_COUNT; i++) samples[i] = raw;
        sum = raw * SAMPLE_COUNT;
        initialized = true;
    }

    sum = sum - samples[idx] + raw;
    samples[idx] = raw;
    idx = (idx + 1) % SAMPLE_COUNT;

    int avg = sum / SAMPLE_COUNT;
    if (avg < WET_VALUE) avg = WET_VALUE;
    if (avg > DRY_VALUE) avg = DRY_VALUE;

    float percent = 100.0f * (DRY_VALUE - avg) / (DRY_VALUE - WET_VALUE);
    return percent;
}


// It did only when the stable_time_ms is consecutively lower than the value
bool soil_is_stable_dry(uint32_t stable_time_ms, float threshold_percent)
{
    static uint32_t dry_start_time = 0;
    uint32_t now = xTaskGetTickCount() * portTICK_PERIOD_MS;

    if (soil_get_moisture_percent() < threshold_percent) {
        if (dry_start_time == 0) dry_start_time = now;
        if (now - dry_start_time >= stable_time_ms) {
            return true;           // 10 seconds lower than value, need to water
        }
    } else {
        dry_start_time = 0;        // zero when wet
    }
    return false;
}