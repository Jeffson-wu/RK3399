#define LOG_TAG "GpioStub"

#include <errno.h>
#include <malloc.h>
#include <pthread.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include <hardware/hardware.h>
#include <hardware/gpio.h>
#include <fcntl.h>
#include <errno.h>
#include <cutils/log.h>
#include <cutils/atomic.h>

#include <sys/ioctl.h>
#define TNFS_FD -110

#define DEVICE_NAME_TMB "/sys/class/gpio/gpio35/value"
#define DEVICE_NAME_TMA "/sys/class/gpio/gpio152/value"
#define DEVICE_NAME_STATUS_LED "/sys/devices/platform/gpio-leds/leds/status_led/trigger"
#define DEVICE_NAME_AMP_ON "/sys/class/gpio/gpio153/value"
#define DEVICE_NAME_AMP_MUTE "/sys/class/gpio/gpio158/value"

#define MODULE_NAME "Gpio"
#define MODULE_AUTHOR "jeffson.wu@iircade.com"

static int gpio_device_open(const struct hw_module_t* module, const char* name, struct hw_device_t** device);
static int gpio_device_close(struct hw_device_t* device);

static int gpio_set_val(struct gpio_device_t* dev, char* val);
static int gpio_get_val(struct gpio_device_t* dev, char* val);

static int gpio_device_open(const struct hw_module_t* module, const char* name, struct hw_device_t** device) {
	struct gpio_device_t* dev;dev = (struct gpio_device_t*)malloc(sizeof(struct gpio_device_t));
	
	if (strcmp(name, GPIO_HARDWARE_DEVICE_ID_TMA) != 0
            && strcmp(name, GPIO_HARDWARE_DEVICE_ID_TMB) != 0
            && strcmp(name, GPIO_HARDWARE_DEVICE_ID_AMP_MUTE) != 0
            && strcmp(name, GPIO_HARDWARE_DEVICE_ID_AMP_ON) != 0
            && strcmp(name, GPIO_HARDWARE_DEVICE_ID_STATUS_LED) != 0)
        {
		ALOGE("Gpio Stub: device ID is incorrect");
		return -EINVAL;
	}

	if(!dev) {
		ALOGE("Gpio Stub: failed to alloc space");
		return -EFAULT;
	}

	memset(dev, 0, sizeof(struct gpio_device_t));
	dev->common.tag = HARDWARE_DEVICE_TAG;
	dev->common.version = 0;
	dev->common.module = (hw_module_t*)module;
	dev->common.close = gpio_device_close;
	dev->set_val = gpio_set_val;
	dev->get_val = gpio_get_val;

	if(strcmp(name, GPIO_HARDWARE_DEVICE_ID_TMA) == 0) {	
		if((dev->fd = fopen(DEVICE_NAME_TMA, "w+")) == NULL) {
			ALOGE("Gpio Stub: failed to open /dev/gpio -- %s.", strerror(errno));free(dev);
			return -EFAULT;
		}

		dev->id = DEV_ID_TMA;
	}

	else if(strcmp(name, GPIO_HARDWARE_DEVICE_ID_TMB) == 0) {
		if((dev->fd = fopen(DEVICE_NAME_TMB, "w+")) == NULL) {
			ALOGE("Gpio Stub: failed to open /dev/gpio -- %s.", strerror(errno));free(dev);
			return -EFAULT;
		}
		
		dev->id = DEV_ID_TMB;		
	}
	
	else if(strcmp(name, GPIO_HARDWARE_DEVICE_ID_AMP_ON) == 0) {
                if((dev->fd = fopen(DEVICE_NAME_AMP_ON, "w+")) == NULL) {
                        ALOGE("Gpio Stub: failed to open /dev/gpio -- %s.", strerror(errno));free(dev);
                        return -EFAULT;
                }
		
		dev->id = DEV_ID_AMP;
        }

        else if(strcmp(name, GPIO_HARDWARE_DEVICE_ID_AMP_MUTE) == 0) {
                if((dev->fd = fopen(DEVICE_NAME_AMP_MUTE, "w+")) == NULL) {
                        ALOGE("Gpio Stub: failed to open /dev/gpio -- %s.", strerror(errno));free(dev);
                        return -EFAULT;
                }

		dev->id = DEV_ID_MUTE;
        }

	else {
                if((dev->fd = fopen(DEVICE_NAME_STATUS_LED, "w+")) == NULL) {
                        ALOGE("Gpio Stub: failed to open /dev/gpio -- %s.", strerror(errno));free(dev);
                        return -EFAULT;
                }

		dev->id = DEV_ID_STATUS_LED;
        }

	
	*device = &(dev->common);
	ALOGI("Gpio Stub: open /dev/gpio successfully.");

	return 0;
}

static int gpio_device_close(struct hw_device_t* device) {
	struct gpio_device_t* gpio_device = (struct gpio_device_t*)device;

	if(gpio_device) {
		fclose(gpio_device->fd);
		free(gpio_device);
	}
	
	return 0;
}

static int gpio_set_val(struct gpio_device_t* dev, char* val) {	
	unsigned int vol;

        if(!val) {
                ALOGE("Gpio Stub: error val pointer");
                return -EFAULT;
        }

	ALOGI("Gpio Stub: set value %s to device.", val);

	fseek(dev->fd, 0, SEEK_SET);
	fwrite(val, sizeof(char), strlen(val), dev->fd);
    	fflush(dev->fd);
	if(dev->id ==  DEV_ID_MUTE) {
		vol = atoi(val);
		ioctl(TNFS_FD ,vol);	
	}

	return 0;
}

static int gpio_get_val(struct gpio_device_t* dev, char* val) {
	if(!val) {
		ALOGE("Gpio Stub: error val pointer");
		return -EFAULT;
	}
	
	fseek(dev->fd, 0, SEEK_SET);
	fread(val, sizeof(char), 1, dev->fd);
	ALOGI("Gpio Stub: get value %s from device", val);

	return 0;
}

static struct hw_module_methods_t gpio_module_methods = {
	.open = gpio_device_open,
};

struct gpio_module_t HAL_MODULE_INFO_SYM = {
	.common = {
		.tag = HARDWARE_MODULE_TAG,
		.version_major = 1,
		.version_minor = 0,
		.id = GPIO_HARDWARE_MODULE_ID,
		.name = MODULE_NAME,
		.author = MODULE_AUTHOR,
		.methods = &gpio_module_methods,
	},
};
