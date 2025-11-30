#include "soil_sensor.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"

#define SOIL_ADC        ADC1_CHANNEL_3    // GPIO4
#define DRY_VALUE       3400              // 空气中实测值
#define WET_VALUE       1250              // 浸水实测值

void soil_sensor_init(void)
{
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(SOIL_ADC, ADC_ATTEN_DB_11db);
}

float soil_get_moisture_percent(void)
{
    int raw = adc1_get_raw(SOIL_ADC);
    if (raw < WET_VALUE) raw = WET_VALUE;
    if (raw > DRY_VALUE) raw = DRY_VALUE;

    return 100.0f * (DRY_VALUE - raw) / (DRY_VALUE - WET_VALUE);
}
