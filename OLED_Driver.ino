/*
 * OLED -----------------------------------------------------------------------------------------------------------------------------------
 */
#include "config.h"

// 初始化OLED及U8G2
void OLED_Init()
{
  u8g2.setI2CAddress(0x78); // 设置I2C地址
  u8g2.begin();             // 初始化显示器
  u8g2.enableUTF8Print();   // 启用UTF8字符集
  u8g2.clear();             // 清除操作
}
// 开机页面
void display_boot_page(int dotCount)
{
  u8g2.clearBuffer();
  u8g2.firstPage();
  do
  {
    u8g2.setFont(u8g2_font_9x15B_tf); // 设置字体集
    u8g2.setCursor(4, 25);            // 指定显示位置
    u8g2.print("Snoring Detect");     // 使用print来显示字符串
    u8g2.setFont(u8g2_font_unifont_t_symbols);
    if (dotCount == 0)
      u8g2.drawGlyph(40, 55, 0x25CF); // 画图
    else if (dotCount == 1)
    {
      u8g2.drawGlyph(40, 55, 0x25CF);
      u8g2.drawGlyph(58, 55, 0x25CF);
    }
    else if (dotCount == 2)
    {
      u8g2.drawGlyph(40, 55, 0x25CF);
      u8g2.drawGlyph(58, 55, 0x25CF);
      u8g2.drawGlyph(76, 55, 0x25CF);
    }
  } while (u8g2.nextPage());
}
// 主界面
void display_main_page()
{
  u8g2.clearBuffer();
  u8g2.firstPage();
  do
  {
    u8g2.setFont(u8g2_font_9x15B_tf);
    u8g2.setCursor(20, 25);
    u8g2.print("2019850069");
    u8g2.setFont(u8g2_font_wqy16_t_gb2312);
    u8g2.setCursor(40, 50);
    u8g2.print("肖秀月");
  } while (u8g2.nextPage());
}
// 预测界面
void display_predictions_page(double percent_snoring, double percent_nonsnoring)
{
  u8g2.clearBuffer();
  u8g2.firstPage();
  do
  {
    u8g2.setFont(u8g2_font_Pixellari_te);
    u8g2.setCursor(25, 15);
    u8g2.print("Predictions");
    u8g2.setFont(u8g2_font_9x15B_tf);
    u8g2.setCursor(10, 35);
    u8g2.print("snoring:");
    u8g2.print(percent_snoring);
    u8g2.setCursor(19, 55);
    u8g2.print("others:");
    u8g2.print(percent_nonsnoring);
  } while (u8g2.nextPage());
}
// 开机动画
void system_boot_animation()
{
  display_boot_page(0);
  delay(1000);
  display_boot_page(1);
  delay(1000);
  display_boot_page(2);
  delay(1000);
  display_main_page();
  delay(1000);
}
