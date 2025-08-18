#ifndef LCD_H_
#define LCD_H_

#include "stm32f1xx_hal.h"

#define RS_PIN LCD_RS_Pin
#define EN_PIN LCD_EN_Pin
#define D4_PIN LCD_D4_Pin
#define D5_PIN LCD_D5_Pin
#define D6_PIN LCD_D6_Pin
#define D7_PIN LCD_D7_Pin
#define LCD_PORT GPIOA

void LCD_Init(void);
void LCD_Command(uint8_t cmd);
void LCD_Char(uint8_t data);
void LCD_String(const char *str);
void LCD_Clear(void);
void LCD_SetCursor(uint8_t row, uint8_t column);

#endif /* LCD_H_ */
