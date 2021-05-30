#
# Copyright 2014 The Android Open-Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

PRIV_DEVICE_PATH := device/rockchip/rk3399/iircade
TARGET_DEVICE_DIR := $(PRIV_DEVICE_PATH)

BUILD_WITH_CUSTOM_PRODUCT := device/rockchip/rk3399/iircade/product.mk.custom

TARGET_BOARD_PLATFORM_PRODUCT := tablet

BUILD_WITH_GOOGLE_GMS_EXPRESS := false

BUILD_WITHOUT_VENDOR_APPS := RkApkinstaller RkExplorer

BOARD_HAVE_BLUETOOTH := true
BOARD_HAVE_BLUETOOTH_RTK := true
BOARD_NFC_SUPPORT := false
BOARD_COMPASS_SENSOR_SUPPORT := false
BOARD_GYROSCOPE_SENSOR_SUPPORT := false
BOARD_PROXIMITY_SENSOR_SUPPORT := false
BOARD_LIGHT_SENSOR_SUPPORT := false
BOARD_CONNECTIVITY_VENDOR := false
BOARD_BLUETOOTH_SUPPORT := true
BOARD_CAMERA_SUPPORT := false
BOARD_GRAVITY_SENSOR_SUPPORT := false
BOARD_HAS_GPS := false
BOARD_HS_ETHERNET := false
BOARD_LIGHT_SENSOR_SUPPORT := false
BOARD_USE_AFBC_LAYER := false
BOARD_USE_ANDROIDNN := false

PRODUCT_HAVE_GPS := false
PRODUCT_HAVE_RKAPPS := false
PRODUCT_HAVE_RFTESTTOOL := false
PRODUCT_HAVE_RKEBOOK := false
PRODUCT_HAVE_DATACLONE := false
PRODUCT_HAVE_PLUGINSVC := false

#CAMERA_SUPPORT_AUTOFOCUS := false

#$(call inherit-product, $(SRC_TARGET_DIR)/product/full_base.mk)
include device/rockchip/rk3399/BoardConfig.mk
include device/rockchip/rk3399/iircade/BoardConfig.mk
# Inherit from those products. Most specific first.
$(call inherit-product, device/rockchip/rk3399/device.mk)
$(call inherit-product, device/rockchip/common/device.mk)
$(call inherit-product, hardware/realtek/rtkbt/rtkbt.mk)
$(call inherit-product, device/rockchip/common/rtkbt/rtkbt.mk)

PRODUCT_CHARACTERISTICS := tablet

PRODUCT_NAME := iircade
PRODUCT_DEVICE := iircade
PRODUCT_BRAND := iiRcade
PRODUCT_MODEL := iiRcade
PRODUCT_MANUFACTURER := iiRcade

PRODUCT_AAPT_PREF_CONFIG := hdpi

# Get the long list of APNs
PRODUCT_COPY_FILES += vendor/rockchip/common/phone/etc/apns-full-conf.xml:system/etc/apns-conf.xml
PRODUCT_COPY_FILES += vendor/rockchip/common/phone/etc/spn-conf.xml:system/etc/spn-conf.xml
PRODUCT_PROPERTY_OVERRIDES += \
    ro.rk.hdmisetting=true
