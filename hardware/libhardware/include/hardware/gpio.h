#ifndef ANDROID_GPIO_INTERFACE_H
#define ANDROID_GPIO_INTERFACE_H

#include <stdio.h>
#include <string.h>
#include <hardware/hardware.h>

__BEGIN_DECLS

#define GPIO_HARDWARE_MODULE_ID "gpio"
#define GPIO_HARDWARE_DEVICE_ID_TMA "TMA"
#define GPIO_HARDWARE_DEVICE_ID_TMB "TMB"
#define GPIO_HARDWARE_DEVICE_ID_AMP_ON "AMP"
#define GPIO_HARDWARE_DEVICE_ID_AMP_MUTE "MUTE"
#define GPIO_HARDWARE_DEVICE_ID_STATUS_LED "status-led"

enum {
	DEV_ID_TMA,
	DEV_ID_TMB,
	DEV_ID_AMP,
	DEV_ID_MUTE,
	DEV_ID_STATUS_LED
};


struct gpio_module_t {
	struct hw_module_t common;
};

struct gpio_device_t {
	struct hw_device_t common;
	FILE *fd;
	int id;
	int (*set_val)(struct gpio_device_t* dev, char* val);
	int (*get_val)(struct gpio_device_t* dev, char* val);
};

__END_DECLS

#endif
