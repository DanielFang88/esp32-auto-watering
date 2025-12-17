#pragma once
#include <stdbool.h>
#include <stdint.h>     // 加这一行！解决 uint32_t

void soil_sensor_init(void);
float soil_get_moisture_percent(void);
bool soil_is_stable_dry(uint32_t stable_time_ms, float threshold_percent);
