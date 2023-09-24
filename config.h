#ifndef _CONFIG_H
#define _CONFIG_H
/*
 * Universal -----------------------------------------------------------------------------------------------------------------------------------
 */
#include <Arduino.h>
#include "esp32-hal-cpu.h" //设置CPU主频
#include "soc/rtc_wdt.h"   //设置看门狗用

#define SUCCEED 1
#define FAILED 0
#define ON 1
#define OFF 0
/*
 * Utility -----------------------------------------------------------------------------------------------------------------------------------
 */
#include "soc/rtc_wdt.h"     //设置看门狗用

#define PIN_BUZZER (4) // 蜂鸣器引脚 GPIO4
#define PIN_RED_LED (27)
#define PIN_YELLOW_LED (26)
#define PIN_GREEN_LED (25)

int freq_buzzer = 2000; // 设置频率
int freq_led = 5000;
int channel_buzzer = 0; // 通道号，取值0 ~ 15
int channel_red_led = 1;
int channel_yellow_led = 2;
int channel_green_led = 3;
int resolution = 8; // 计数位数，2的8次幂=256

int buzzer_status = OFF;
int red_led_status = OFF;
int yellow_led_status = OFF;
int green_led_status = OFF;

enum
{
    RED_LED = 1,
    YELLOW_LED,
    GREEN_LED
};

/*
 * OLED -----------------------------------------------------------------------------------------------------------------------------------
 */
#include <U8g2lib.h>
#include <Wire.h>

#define OLED_SDA 21
#define OLED_SCL 22
// U8G2程序库 SSD1306控制晶片 128X64_NONAME解析度和型号 F暂存区大小可以改为1或2 F_HW_I2C控制介面 u8g2(U8G2_R0代表不旋转,U8X8_PIN_NONE代表没有重置引脚)
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /*reset=*/U8X8_PIN_NONE, /*clock=*/OLED_SCL, /*data=*/OLED_SDA);

/*
 * Edge Impulse -----------------------------------------------------------------------------------------------------------------------------------
 */
// If your target is limited in memory remove this macro to save 10K RAM
// #define EIDSP_QUANTIZE_FILTERBANK   0
//
// #include <zh-project-1_inferencing.h>
//
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
//
// #include "driver/i2s.h"
//
///** Audio buffers, pointers and selectors */
// typedef struct {
//     signed short *buffers[2];
//     unsigned char buf_select;
//     unsigned char buf_ready;
//     unsigned int buf_count;
//     unsigned int n_samples;
// } inference_t;
//
// inference_t inference;
// const uint32_t sample_buffer_size = 2048;
// signed short sampleBuffer[sample_buffer_size];
// bool debug_nn = false; // Set this to true to see e.g. features generated from the raw signal
// int print_results = -(EI_CLASSIFIER_SLICES_PER_MODEL_WINDOW);
// bool record_status = true;

#endif CONFIG_H
