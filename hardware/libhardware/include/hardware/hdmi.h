#ifndef ANDROID_HDMI_INTERFACE_H
#define ANDROID_HDMI_INTERFACE_H

#include <stdio.h>
#include <string.h>
#include <hardware/hardware.h>

__BEGIN_DECLS

#define HDMI_HARDWARE_MODULE_ID "hdmi"
#define HDMI_HARDWARE_DEVICE_ID "hdmi"
#define HDMI_ON 1
#define HDMI_OFF 0

struct hdmi_module_t {
	struct hw_module_t common;
};

struct hdmi_device_t {
	struct hw_device_t common;
	FILE *fd;
	int (*set_val)(struct hdmi_device_t* dev, int val);
	int (*get_val)(struct hdmi_device_t* dev, int* val);
};

__END_DECLS

#endif
