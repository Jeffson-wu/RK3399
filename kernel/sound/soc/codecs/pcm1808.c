/*
  * ALSA SoC PCM1808 support
  *
  * Author:      Florian Larysch <fl@n621.de>
  *
  * This program is free software; you can redistribute it and/or modify
  * it under the terms of the GNU General Public License version 2 as
  * published by the Free Software Foundation.
  */
 
 #include <linux/module.h>
 #include <linux/moduleparam.h>
 #include <linux/slab.h>
 #include <sound/soc.h>
 #include <sound/pcm.h>
 #include <sound/initval.h>
 #include <linux/of.h>

 static const struct snd_soc_dapm_widget pcm1808_dapm_widgets[] = {
 SND_SOC_DAPM_INPUT("VINL"),
 SND_SOC_DAPM_INPUT("VINR"),
 SND_SOC_DAPM_ADC("ADC", NULL, SND_SOC_NOPM, 0, 0)
 };
 
 static const struct snd_soc_dapm_route pcm1808_dapm_routes[] = {
 	{ "Capture", NULL, "ADC" },
 	{ "ADC", NULL, "VINL" },
 	{ "ADC", NULL, "VINR" },
 };
 
 static struct snd_soc_codec_driver soc_codec_dev_pcm1808 = {
 	.dapm_widgets = pcm1808_dapm_widgets,
 	.num_dapm_widgets = ARRAY_SIZE(pcm1808_dapm_widgets),
 	.dapm_routes = pcm1808_dapm_routes,
 	.num_dapm_routes = ARRAY_SIZE(pcm1808_dapm_routes),
 };
 
 static struct snd_soc_dai_driver pcm1808_dai = {
 	.name		= "pcm1808-hifi",
 	.capture        = {
 		.stream_name	= "Capture",
 		.channels_min	= 2,
 		.channels_max	= 2,
 		.rates		= SNDRV_PCM_RATE_8000_96000,
 		.formats	= SNDRV_PCM_FMTBIT_S24_LE, 
 	},
 };
 
 static int pcm1808_codec_probe(struct platform_device *pdev)
 {
	printk(KERN_ERR "Cannot map BCSR registers\n");
 	return snd_soc_register_codec(&pdev->dev, &soc_codec_dev_pcm1808,
 			&pcm1808_dai, 1);
 }
 
 static int pcm1808_codec_remove(struct platform_device *pdev)
 {
 	snd_soc_unregister_codec(&pdev->dev);
 	return 0;
 }
 
 #ifdef CONFIG_OF
 static const struct of_device_id pcm1808_codec_dt_ids[] = {
 	{ .compatible = "ti,pcm1808", },
 	{ }
 };
 MODULE_DEVICE_TABLE(of, pcm1808_codec_dt_ids);
 #endif
 
 static struct platform_driver pcm1808_codec_driver = {
 	.probe		= pcm1808_codec_probe,
 	.remove		= pcm1808_codec_remove,
 	.driver		= {
 		.name	= "pcm1808-codec",
 		.of_match_table = of_match_ptr(pcm1808_codec_dt_ids),
 	},
 };
 
 module_platform_driver(pcm1808_codec_driver);
 
 MODULE_AUTHOR("Florian Larysch <fl@n621.de>");
 MODULE_DESCRIPTION("PCM1808 driver");
 MODULE_LICENSE("GPL");
 MODULE_ALIAS("platform:pcm1808-codec");
