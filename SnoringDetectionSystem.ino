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
  led_init();              // 初始化LED
  buzzer_init();           // 初始化蜂鸣器
  OLED_Init();             // 初始化OLED
  system_boot_animation(); // 系统开机动画
  display_predictions_page(0.98, 0.002);
  xTaskCreatePinnedToCore(buzzerTask, "buzzerTask", 2048, NULL, 1, NULL, 0); // buzzerTask在 0核心
  xTaskCreatePinnedToCore(ledTask, "ledTask", 2048, NULL, 2, NULL, 0);       // ledTask在 0核心
  buzzer_status = ON;
  red_led_status = OFF;
  yellow_led_status = OFF;
  green_led_status = ON;
  // run_classifier_init();
  // ei_printf("Inferencing settings:\n");
  // ei_printf("\tInterval: ");
  // ei_printf_float((float)EI_CLASSIFIER_INTERVAL_MS);
  // ei_printf(" ms.\n");
  // ei_printf("\tFrame size: %d\n", EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE);
  // ei_printf("\tSample length: %d ms.\n", EI_CLASSIFIER_RAW_SAMPLE_COUNT / 16);
  // ei_printf("\tNo. of classes: %d\n", sizeof(ei_classifier_inferencing_categories) / sizeof(ei_classifier_inferencing_categories[0]));
  // ei_printf("\nStarting continious inference in 2 seconds...\n");
  // ei_sleep(2000);

  // if (microphone_inference_start(EI_CLASSIFIER_SLICE_SIZE) == false)
  // {
  //   ei_printf("ERR: Could not allocate audio buffer (size %d), this could be due to the window length of your model\r\n", EI_CLASSIFIER_RAW_SAMPLE_COUNT);
  //   return;
  // }

  // ei_printf("Recording...\n");
}

/**
 * @brief      Arduino main function. Runs the inferencing loop.
 */
void loop()
{
  Serial.print("hello");
  delay(1000);
  // rtc_wdt_feed();
  // yield();
  //   bool m = microphone_inference_record();
  //   if (!m)
  //   {
  //     ei_printf("ERR: Failed to record audio...\n");
  //     return;
  //   }

  //   signal_t signal;
  //   signal.total_length = EI_CLASSIFIER_SLICE_SIZE;
  //   signal.get_data = &microphone_audio_signal_get_data;
  //   ei_impulse_result_t result = {0};

  //   EI_IMPULSE_ERROR r = run_classifier_continuous(&signal, &result, debug_nn);
  //   if (r != EI_IMPULSE_OK)
  //   {
  //     ei_printf("ERR: Failed to run classifier (%d)\n", r);
  //     return;
  //   }

  //   if (++print_results >= (EI_CLASSIFIER_SLICES_PER_MODEL_WINDOW))
  //   {
  //     // print the predictions
  //     ei_printf("Predictions ");
  //     ei_printf("(DSP: %d ms., Classification: %d ms., Anomaly: %d ms.)",
  //               result.timing.dsp, result.timing.classification, result.timing.anomaly);
  //     ei_printf(": \n");
  //     for (size_t ix = 0; ix < EI_CLASSIFIER_LABEL_COUNT; ix++)
  //     {
  //       ei_printf("    %s: ", result.classification[ix].label);
  //       ei_printf_float(result.classification[ix].value);
  //       ei_printf("\n");
  //     }
  // #if EI_CLASSIFIER_HAS_ANOMALY == 1
  //     ei_printf("    anomaly score: ");
  //     ei_printf_float(result.anomaly);
  //     ei_printf("\n");
  // #endif

  //     print_results = 0;
  //   }
}
