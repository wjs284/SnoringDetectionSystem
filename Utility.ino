/*
 * Utility -----------------------------------------------------------------------------------------------------------------------------------
 */
void setup_watch_dog()
{
  rtc_wdt_protect_off(); // 看门狗写保护关闭 关闭后可以喂狗
  // rtc_wdt_protect_on();    //看门狗写保护打开 打开后不能喂狗
  // rtc_wdt_disable();       //禁用看门狗
  rtc_wdt_enable(); // 启用看门狗
  rtc_wdt_set_time(RTC_WDT_STAGE0, 20000);
}
// 蜂鸣器初始化函数
void buzzer_init()
{
  ledcSetup(channel_buzzer, 5000, resolution); // 配置PWM通道
  ledcAttachPin(PIN_BUZZER, channel_buzzer);   // 将PWM通道连接到LED引脚
}
// 蜂鸣器响一声
void buzzer_on()
{
  ledcWrite(channel_buzzer, 300);
  delay(300);
  ledcWrite(channel_buzzer, 0);
  delay(700);
}
// LED初始化函数
void led_init()
{
  ledcSetup(channel_red_led, freq_led, resolution);
  ledcAttachPin(PIN_RED_LED, channel_red_led);
  ledcSetup(channel_yellow_led, freq_led, resolution);
  ledcAttachPin(PIN_YELLOW_LED, channel_yellow_led);
  ledcSetup(channel_green_led, freq_led, resolution);
  ledcAttachPin(PIN_GREEN_LED, channel_green_led);
}
// LED闪烁函数
void led_blink(int led_chanel)
{
  for (int duty = 0; duty < 255; duty++)
  {
    ledcWrite(led_chanel, duty); // 增加亮度
    delay(3);
  }
  for (int duty = 255; duty > 0; duty--)
  {
    ledcWrite(led_chanel, duty); // 减少亮度
    delay(3);
  }
}
// 蜂鸣器任务线程实体
void buzzerTask(void *xTask1)
{
  while (1)
  {
    if (ON == buzzer_status)
      buzzer_on();
    else
      delay(100);
  }
}
//  LED任务线程实体
void ledTask(void *xTask1)
{
  while (1)
  {
    if (ON == red_led_status)
      led_blink(RED_LED);
    else if (ON == yellow_led_status)
      led_blink(YELLOW_LED);
    else if (ON == green_led_status)
      led_blink(GREEN_LED);
    else
      delay(100);
  }
}
