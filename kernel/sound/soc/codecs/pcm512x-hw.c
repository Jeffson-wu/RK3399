/*
  * ALSA SoC PCM512x support
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
 #include <linux/clk.h>

 static const struct snd_soc_dapm_widget pcm512x_dapm_widgets[] = {
	SND_SOC_DAPM_DAC("DACL", NULL, SND_SOC_NOPM, 0, 0),
	SND_SOC_DAPM_DAC("DACR", NULL, SND_SOC_NOPM, 0, 0),

	SND_SOC_DAPM_OUTPUT("OUTL"),
	SND_SOC_DAPM_OUTPUT("OUTR"),
 };

 static const struct snd_soc_dapm_route pcm512x_dapm_routes[] = {
        { "DACL", NULL, "Playback" },
        { "DACR", NULL, "Playback" },

        { "OUTL", NULL, "DACL" },
        { "OUTR", NULL, "DACR" },
 };


 static struct snd_soc_dai_driver pcm512x_dai = {
        .name = "pcm512x-hifi",
        .playback = {
                .stream_name = "Playback",
                .channels_min = 2,
                .channels_max = 2,
                .rates = SNDRV_PCM_RATE_CONTINUOUS,
                .rate_min = 8000,
                .rate_max = 384000,
                .formats = SNDRV_PCM_FMTBIT_S16_LE |
                           SNDRV_PCM_FMTBIT_S24_LE |
                           SNDRV_PCM_FMTBIT_S32_LE
        },
 };

 static struct snd_soc_codec_driver soc_codec_dev_pcm512x = {
        .idle_bias_off = true,

        .dapm_widgets = pcm512x_dapm_widgets,
        .num_dapm_widgets = ARRAY_SIZE(pcm512x_dapm_widgets),
        .dapm_routes = pcm512x_dapm_routes,
        .num_dapm_routes = ARRAY_SIZE(pcm512x_dapm_routes),
 };

  
 static int pcm512x_codec_probe(struct platform_device *pdev)
 {

	struct clk* sclk;
	int ret;
/*	 printk("%s----%d\n",__FUNCTION__,__LINE__);	
	sclk = devm_clk_get(&pdev->dev, "mclk");
        if (PTR_ERR(sclk) == -EPROBE_DEFER)
                return -EPROBE_DEFER;
	printk("%s----%d\n",__FUNCTION__,__LINE__);
        if (!IS_ERR(sclk)) {
                ret = clk_prepare_enable(sclk);
		printk("%s----%d\n",__FUNCTION__,__LINE__);
                if (ret != 0) {
                        dev_err(&pdev->dev, "Failed to enable SCLK: %d\n", ret);
                        return ret;
                }
		printk("%s----%d\n",__FUNCTION__,__LINE__);
        }
	printk("%s----%d\n",__FUNCTION__,__LINE__);*/
	 return snd_soc_register_codec(&pdev->dev, &soc_codec_dev_pcm512x,
 			&pcm512x_dai, 1);
 }
 
 static int pcm512x_codec_remove(struct platform_device *pdev)
 {
 	snd_soc_unregister_codec(&pdev->dev);
 	return 0;
 }
 
 #ifdef CONFIG_OF
 static const struct of_device_id pcm512x_codec_dt_ids[] = {
 	{ .compatible = "ti,pcm512x", },
 	{ }
 };
 MODULE_DEVICE_TABLE(of, pcm512x_codec_dt_ids);
 #endif
 
 static struct platform_driver pcm512x_codec_driver = {
 	.probe		= pcm512x_codec_probe,
 	.remove		= pcm512x_codec_remove,
 	.driver		= {
 		.name	= "pcm512x-codec",
 		.of_match_table = of_match_ptr(pcm512x_codec_dt_ids),
 	},
 };
 
 module_platform_driver(pcm512x_codec_driver);
 
 MODULE_AUTHOR("Florian Larysch <fl@n621.de>");
 MODULE_DESCRIPTION("PCM512x driver");
 MODULE_LICENSE("GPL");
 MODULE_ALIAS("platform:pcm512x-codec");
