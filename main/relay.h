#pragma once
#include "driver/gpio.h"
#define RELAY_GPIO 18
void relay_init(void);
void relay_open(void);
void relay_close(void);