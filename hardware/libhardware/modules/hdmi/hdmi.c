#define LOG_TAG "HdmiStub"

#include <errno.h>
#include <malloc.h>
#include <pthread.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include <hardware/hardware.h>
#include <hardware/hdmi.h>
#include <fcntl.h>
#include <errno.h>
#include <cutils/log.h>
#include <cutils/atomic.h>

#define DEVICE_NAME_HDMI "/sys/devices/platform/display-subsystem/drm/card0/card0-HDMI-A-1/status"
#define MODULE_NAME "Hdmi"
#define MODULE_AUTHOR "jeffson.wu@iircade.com"

static int hdmi_device_open(const struct hw_module_t* module, const char* name, struct hw_device_t** device);
static int hdmi_device_close(struct hw_device_t* device);

static int hdmi_set_val(struct hdmi_device_t* dev, int val);
static int hdmi_get_val(struct hdmi_device_t* dev, int* val);

static int hdmi_device_open(const struct hw_module_t* module, const char* name, struct hw_device_t** device) {
	struct hdmi_device_t* dev;dev = (struct hdmi_device_t*)malloc(sizeof(struct hdmi_device_t));
	
	if (strcmp(name, HDMI_HARDWARE_DEVICE_ID) != 0)
        {
		ALOGE("Hdmi Stub: device ID is incorrect");
		return -EINVAL;
	}

	if(!dev) {
		ALOGE("Hdmi Stub: failed to alloc space");
		return -EFAULT;
	}

	memset(dev, 0, sizeof(struct hdmi_device_t));
	dev->common.tag = HARDWARE_DEVICE_TAG;
	dev->common.version = 0;
	dev->common.module = (hw_module_t*)module;
	dev->common.close = hdmi_device_close;
	dev->set_val = hdmi_set_val;
	dev->get_val = hdmi_get_val;

	if((dev->fd = fopen(DEVICE_NAME_HDMI, "w+")) == NULL) {
		ALOGE("Hdmi Stub: failed to open /dev/hdmi -- %s.", strerror(errno));free(dev);
		return -EFAULT;
	}

	
	*device = &(dev->common);
	ALOGI("Hdmi Stub: open /dev/hdmi successfully.");

	return 0;
}

static int hdmi_device_close(struct hw_device_t* device) {
	struct hdmi_device_t* hdmi_device = (struct hdmi_device_t*)device;

	if(hdmi_device) {
		fclose(hdmi_device->fd);
		free(hdmi_device);
	}
	
	return 0;
}

static int hdmi_set_val(struct hdmi_device_t* dev, int val) {
	if(val != HDMI_ON && val != HDMI_OFF) {
		ALOGE("Hdmi Stub: error val");	
		return -EFAULT;
	}

	ALOGI("Hdmi Stub: set value %d to device.", val);

	if(val == HDMI_ON) {
		fseek(dev->fd, 0, SEEK_SET);
		fwrite("on", 2, 1, dev->fd);
		fflush(dev->fd);
	}
	
	else {
		fseek(dev->fd, 0, SEEK_SET);
		fwrite("off", 3, 1, dev->fd);
		fflush(dev->fd);	
	}

	return 0;
}

static int hdmi_get_val(struct hdmi_device_t* dev, int* val) {
	char buffer[3];

	if(!val) {
		ALOGE("Hdmi Stub: error val pointer");
		return -EFAULT;
	}
	
	fseek(dev->fd, 0, SEEK_SET);
	fread(buffer, 3, 1, dev->fd);
	if(buffer[0] == 'c')
		*val = HDMI_ON;
	else
		*val = HDMI_OFF;

	ALOGI("Hdmi Stub: get value %d from device", *val);

	return 0;
}

static struct hw_module_methods_t hdmi_module_methods = {
	.open = hdmi_device_open,
};

struct hdmi_module_t HAL_MODULE_INFO_SYM = {
	.common = {
		.tag = HARDWARE_MODULE_TAG,
		.version_major = 1,
		.version_minor = 0,
		.id = HDMI_HARDWARE_MODULE_ID,
		.name = MODULE_NAME,
		.author = MODULE_AUTHOR,
		.methods = &hdmi_module_methods,
	},
};
