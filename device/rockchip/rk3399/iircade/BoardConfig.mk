MY_PATH := device/rockchip/rk3399/iircade

PRODUCT_PACKAGE_OVERLAYS := device/rockchip/rk3399/iircade/overlay

HAVE_BOOT_ANIMATION := $(shell test -f $(MY_PATH)/logo/bootanimation.zip && echo yes)
ifeq ($(HAVE_BOOT_ANIMATION), yes)
PRODUCT_COPY_FILES += $(MY_PATH)/logo/bootanimation.zip:$(TARGET_COPY_OUT_SYSTEM)/media/bootanimation.zip
PRODUCT_COPY_FILES += $(MY_PATH)/logo/bootanimation.zip:$(TARGET_COPY_OUT_OEM)/media/shutdownanimation.zip
endif
