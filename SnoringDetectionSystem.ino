/*
 * Snoring Detection System -----------------------------------------------------------------------------------------------------------------------------------
 */
#include "config.h"

/**
 * @brief      Arduino setup function
 */
void setup()
{
  Serial.begin(115200);
  led_init();                                                                // 初始化LED
  buzzer_init();                                                             // 初始化蜂鸣器
  OLED_Init();                                                               // 初始化OLED
  system_boot_animation();                                                   // 系统开机动画
  xTaskCreatePinnedToCore(buzzerTask, "buzzerTask", 2048, NULL, 1, NULL, 0); // buzzerTask在 0核心
  xTaskCreatePinnedToCore(ledTask, "ledTask", 2048, NULL, 2, NULL, 0);       // ledTask在 0核心
  run_classifier_init();                                                     // 初始化Edge Impulse
  /*打印配置参数*/
  ei_printf("Inferencing settings:\n");
  ei_printf("\tInterval: ");
  ei_printf_float((float)EI_CLASSIFIER_INTERVAL_MS);
  ei_printf(" ms.\n");
  ei_printf("\tFrame size: %d\n", EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE);
  ei_printf("\tSample length: %d ms.\n", EI_CLASSIFIER_RAW_SAMPLE_COUNT / 16);
  ei_printf("\tNo. of classes: %d\n", sizeof(ei_classifier_inferencing_categories) / sizeof(ei_classifier_inferencing_categories[0]));
  ei_printf("\nStarting continious inference in 2 seconds...\n");
  delay(2000);
  // 开始采集麦克风数据
  if (microphone_inference_start(EI_CLASSIFIER_SLICE_SIZE) == false)
  {
    ei_printf("ERR: Could not allocate audio buffer (size %d), this could be due to the window length of your model\r\n", EI_CLASSIFIER_RAW_SAMPLE_COUNT);
    return;
  }

  ei_printf("Recording...\n");
}

/**
 * @brief      Arduino main function. Runs the inferencing loop.
 */
void loop()
{
  while (TRUE)
  {

    bool m = microphone_inference_record();
    if (!m)
    {
      ei_printf("ERR: Failed to record audio...\n");
      return;
    }

    signal_t signal;
    signal.total_length = EI_CLASSIFIER_SLICE_SIZE;
    signal.get_data = &microphone_audio_signal_get_data;
    ei_impulse_result_t result = {0};

    EI_IMPULSE_ERROR r = run_classifier_continuous(&signal, &result, debug_nn);
    if (r != EI_IMPULSE_OK)
    {
      ei_printf("ERR: Failed to run classifier (%d)\n", r);
      return;
    }

    if (++print_results >= (EI_CLASSIFIER_SLICES_PER_MODEL_WINDOW))
    {

      display_predictions_page(result.classification[0].value, result.classification[1].value);

      // 在打呼噜，证明在睡觉
      if (result.classification[0].value >= 0.6)
      {
        buzzer_status = OFF;
        red_led_status = OFF;
        yellow_led_status = OFF;
        green_led_status = ON;
        snoring_status.snoring_pause_time_start = 0;
        snoring_status.snoring_pause_time_end = 0;
        snoring_status.breath_pause_flag = FALSE;
        snoring_status.sleep_status = TRUE;
        print_results = 0;
        delay(10);
        continue;
      }
      // 从未打过呼噜，证明没有睡觉
      else if (result.classification[0].value < 0.6 && snoring_status.sleep_status != TRUE)
      {
        print_results = 0;
        delay(10);
        continue;
      }
      // 睡着了，但是没打呼噜
      else if (result.classification[0].value < 0.6 && TRUE == snoring_status.sleep_status)
      {
        // 开始记录呼吸暂停时间
        if (FALSE == snoring_status.breath_pause_flag)
        {
          green_led_status = OFF;
          red_led_status = OFF;
          yellow_led_status = ON;
          snoring_status.snoring_pause_time_start = millis();
          snoring_status.breath_pause_flag = TRUE;
        }
        else if (TRUE == snoring_status.breath_pause_flag)
        {
          snoring_status.snoring_pause_time_end = millis();
          Serial.print("\r\n------------");
          Serial.print(snoring_status.snoring_pause_time_start);
          Serial.print("\r\n------------");
          Serial.print(snoring_status.snoring_pause_time_end);
          // 如果出现呼吸暂停情况(大于20秒钟)，红灯亮，蜂鸣器响
          if ((snoring_status.snoring_pause_time_end - snoring_status.snoring_pause_time_start) > 20000)
          {
            yellow_led_status = OFF;
            green_led_status = OFF;
            red_led_status = ON;
            buzzer_status = ON;
          }
        }
      }

      print_results = 0;
    }
    delay(10);
  }
}
