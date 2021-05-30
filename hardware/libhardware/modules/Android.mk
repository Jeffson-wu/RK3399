hardware_modules := \
    camera \
    gralloc \
    sensors \
    hw_output \
    gpio \
    hdmi
include $(call all-named-subdir-makefiles,$(hardware_modules))
