#pragma once
#include <stdbool.h>

void soil_sensor_init(void);
float soil_get_moisture_percent(void);
bool soil_is_stable_dry(uint32_t stable_time_ms, float threshold_percent);