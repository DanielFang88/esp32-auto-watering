#include "soil_sensor.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define SOIL_ADC_PIN    4                 // GPIO4
#define DRY_VALUE       3400
#define WET_VALUE       1250

static adc_oneshot_unit_handle_t adc_handle;
static const char* TAG = "SOIL";

void soil_sensor_init(void)
{
    adc_oneshot_unit_init_cfg_t init_cfg = {
        .unit_id = ADC_UNIT_1,
        .ulp_mode = false,
    };
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_cfg, &adc_handle));

    adc_oneshot_chan_cfg_t chan_cfg = {
        .atten = ADC_ATTEN_DB_12,       // 用 DB_12 代替 DB_11（官方推荐，避开警告）
        .bitwidth = ADC_BITWIDTH_12,
    };
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc_handle, ADC_CHANNEL_3, &chan_cfg));

    ESP_LOGI(TAG, "ADC oneshot driver initialized (GPIO4)");
}

float soil_get_moisture_percent(void)
{
    int raw;
    ESP_ERROR_CHECK(adc_oneshot_read(adc_handle, ADC_CHANNEL_3, &raw));

    if (raw < WET_VALUE) raw = WET_VALUE;
    if (raw > DRY_VALUE) raw = DRY_VALUE;

    return 100.0f * (DRY_VALUE - raw) / (DRY_VALUE - WET_VALUE);
}

bool soil_is_stable_dry(uint32_t stable_time_ms, float threshold_percent)
{
    static uint32_t dry_start_time = 0;
    uint32_t now = xTaskGetTickCount() * portTICK_PERIOD_MS;  // 现在头文件包含了，能用了

    if (soil_get_moisture_percent() < threshold_percent) {
        if (dry_start_time == 0) dry_start_time = now;
        if (now - dry_start_time >= stable_time_ms) return true;
    } else {
        dry_start_time = 0;
    }
    return false;
}
