#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/delay.h>
#include <linux/gpio.h>
#include <linux/kernel.h>
#include <asm/io.h>

#if 0//DHYOON
struct sn65dsi84_cmd {
	uint8_t reg;
	uint8_t value;
} init_data[] = {
	{ 0x09, 0x00 },
	{ 0x0D, 0x00 },
	{ 0x0A, 0x03 },
	{ 0x0B, 0x28 },
	{ 0x0D, 0x01 },
	{ 0x10, 0x26 },
	{ 0x11, 0x00 },
	{ 0x12, 0x26 },
	{ 0x13, 0x00 },
	{ 0x18, 0x6c },
	{ 0x19, 0x00 },
	{ 0x1A, 0x03 },
	{ 0x1B, 0x00 },
	{ 0x20, 0x00 },
	{ 0x21, 0x05 },
	{ 0x22, 0x00 },
	{ 0x23, 0x00 },
	{ 0x24, 0x00 },
	{ 0x25, 0x00 },
	{ 0x26, 0x00 },
	{ 0x27, 0x00 },
	{ 0x28, 0xbb },
	{ 0x29, 0x01 },
	{ 0x2A, 0x00 },
	{ 0x2B, 0x00 },
	{ 0x2C, 0x38 },
	{ 0x2D, 0x00 },
	{ 0x2E, 0x00 },
	{ 0x2F, 0x00 },
	{ 0x30, 0x03 },
	{ 0x31, 0x00 },
	{ 0x32, 0x00 },
	{ 0x33, 0x00 },
	{ 0x34, 0x7c },
	{ 0x35, 0x00 },
	{ 0x36, 0x00 },
	{ 0x37, 0x00 },
	{ 0x38, 0x00 },
	{ 0x39, 0x00 },
	{ 0x3A, 0x00 },

	// End Marker
	{ 0xFF, 0xFF },
};

static int sn65dsi84_init(struct i2c_client *client)
{
	int ret;
	int i;

	for (i = 0; init_data[i].reg != 0xFF; i++) {
		ret = i2c_smbus_write_byte_data(client, init_data[i].reg,
				init_data[i].value);
		if (ret < 0)
			goto error;
	}

	// PLL_EN
	ret = i2c_smbus_write_byte_data(client, 0x0D, 0x01);
	if (ret < 0)
		goto error;

    msleep(5);

	// SOFT_RESET
	ret = i2c_smbus_write_byte_data(client, 0x09, 0x01);
	if (ret < 0)
		goto error;

	dev_info(&client->dev, "%s: Bridge initialized\n", __func__);
	return 0;

error:
	dev_err(&client->dev, "%s: Unable to init bridge (%d)\n",
			__func__, ret);
	return ret;
}

static int sn65dsi84_probe(struct i2c_client *client,
		const struct i2c_device_id *id)
{
	int ret;

	ret = sn65dsi84_init(client);
	if (ret < 0) {
		dev_err(&client->dev, "%s: Unable to init bridge(%d)\n",
				__func__, ret);
		return ret;
	}

	return 0;
}

static const struct i2c_device_id sn65dsi84_id[] = {
	{ "sn65dsi84", 0 },
	{ }
};
MODULE_DEVICE_TABLE(i2c, iircade_id);

static struct i2c_driver sn65dsi84_driver = {
	.driver = {
		.name = "sn65dsi84",
	},
	.probe = sn65dsi84_probe,
	.id_table = sn65dsi84_id,
};

static struct i2c_board_info iircade_device = {
	I2C_BOARD_INFO("sn65dsi84", 0x2c),
};
#endif

static int __init iircade_init(void)
{
	struct i2c_adapter *adapter;
	unsigned char __iomem *base;
	int ret;
#if 1 // DHYOON
	/* M2L_EN */
	ret = gpio_request_one(54, /* GPIO1 C6 */
			GPIOF_OUT_INIT_LOW | GPIOF_EXPORT_DIR_CHANGEABLE,
			"M2L_EN");
	if (ret < 0) {
		pr_err("%s: fail to enable bridge adapter\n", __func__);
		return ret;
	}
    gpio_set_value(54, 0);

	/* SN65DSI84 */
#if 0// move to drivers/gpu/drm/rockchip/dw-mipi-dsi.c
	adapter = i2c_get_adapter(4);
	if (!adapter) {
		pr_err("%s: fail to get bridge adapter\n", __func__);
		return -ENODEV;
	}

	if (i2c_new_device(adapter, &iircade_device) == NULL) {
		i2c_put_adapter(adapter);
		pr_err("%s: fail to create a bridge adapter\n", __func__);
		return -ENODEV;
	}

	i2c_add_driver(&sn65dsi84_driver);
	i2c_del_driver(&sn65dsi84_driver);
#endif
#endif

	/* SPK_SD */
	ret = gpio_request_one(153, /* GPIO4 D1 */
			GPIOF_OUT_INIT_LOW | GPIOF_EXPORT_DIR_CHANGEABLE,
			"SPK_SD");
	if (ret < 0) {
		pr_err("%s: fail to enable TPA3116D2 amp\n", __func__);
		return ret;
	}

	/* SPK_MUTE */
	ret = gpio_request_one(158, /* GPIO4 D6 */
			GPIOF_OUT_INIT_HIGH | GPIOF_EXPORT_DIR_CHANGEABLE,
			"SPK_MUTE");
	if (ret < 0) {
		pr_err("%s: fail to enable TPA3116D2 amp\n", __func__);
		return ret;
	}

	/* WIFI_PWR */
	ret = gpio_request_one(149, /* GPIO4 C5 */
			GPIOF_OUT_INIT_HIGH | GPIOF_EXPORT_DIR_CHANGEABLE,
			"WIFI_PWR");
	if (ret < 0) {
		pr_err("%s: fail to enable WiFi\n", __func__);
		return ret;
	}

	/* VCC5V0_OTG0_EN */
//	ret = gpio_request_one(35, /* GPIO1 A3 */
//			GPIOF_OUT_INIT_LOW | GPIOF_EXPORT_DIR_CHANGEABLE,
//			"OTG0_EN");
//	if (ret < 0) {
//		pr_err("%s: fail to enable OTG enable pin\n", __func__);
//		return ret;
//	}
#if 1
      ret = gpio_request_one(152, /* GPIO4 D0 */
                      GPIOF_DIR_IN  | GPIOF_INIT_HIGH | GPIOF_EXPORT_DIR_CHANGEABLE,
                      "test-tma");
      if (ret < 0) {
              pr_err("%s: fail to enable TMA pin\n", __func__);
              return ret;
      }


      ret = gpio_request_one(35, /* GPIO1 A3 */
                    GPIOF_DIR_IN  | GPIOF_INIT_HIGH | GPIOF_EXPORT_DIR_CHANGEABLE,
                      "test-tmb");
      if (ret < 0) {
              pr_err("%s: fail to enable TMB pin\n", __func__);
              return ret;
      }
#endif
	/* VCC5V0_HOTE1_EN */
	ret = gpio_request_one(36, /* GPIO1 A4 */
			GPIOF_OUT_INIT_HIGH | GPIOF_EXPORT_DIR_CHANGEABLE,
			"HOTE1_EN");
	if (ret < 0) {
		pr_err("%s: fail to enable Host enable pin\n", __func__);
		return ret;
	}

	/* enable schmitt trigger for joystick/button */
	base = ioremap(0xFF77E000, 4096);
	if (base != NULL) {
		*((unsigned int *)(base + 0xc0)) = 0xffffffff;
		*((unsigned int *)(base + 0xc4)) = 0xffffffff;
		*((unsigned int *)(base + 0xc8)) = 0xffffffff;
		*((unsigned int *)(base + 0xcc)) = 0xffffffff;
		*((unsigned int *)(base + 0xe4)) = 0xffffffff;

		iounmap((void *)base);
	}

	pr_info("iiRcade Extras loaded\n");
	return 0;
}

static void __exit iircade_exit(void)
{
	pr_info("iiRcade Extras unloaded\n");
	return;
}

module_init(iircade_init);
module_exit(iircade_exit);

MODULE_AUTHOR("nobody");
MODULE_LICENSE("GPL");
