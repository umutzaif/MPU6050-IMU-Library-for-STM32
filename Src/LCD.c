#include "lcd.h"
#include "main.h"

void LCD_PulseEnable(void) {
    HAL_GPIO_WritePin(LCD_PORT, EN_PIN, GPIO_PIN_SET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(LCD_PORT, EN_PIN, GPIO_PIN_RESET);
    HAL_Delay(1);
}

void LCD_SendNibble(uint8_t data) {
    HAL_GPIO_WritePin(LCD_PORT, D4_PIN, (data & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LCD_PORT, D5_PIN, (data & 0x02) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LCD_PORT, D6_PIN, (data & 0x04) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LCD_PORT, D7_PIN, (data & 0x08) ? GPIO_PIN_SET : GPIO_PIN_RESET);

    LCD_PulseEnable();
}

void LCD_Command(uint8_t cmd) {
    HAL_GPIO_WritePin(LCD_PORT, RS_PIN, GPIO_PIN_RESET); // Command mode

    // Üst nibble (4 MSB)
    LCD_SendNibble(cmd >> 4);
    // Alt nibble (4 LSB)
    LCD_SendNibble(cmd & 0x0F);

    // Komutlar için ek bekleme
    if(cmd == 0x01 || cmd == 0x02) {
        HAL_Delay(2);
    }
}

void LCD_Char(uint8_t data) {
    HAL_GPIO_WritePin(LCD_PORT, RS_PIN, GPIO_PIN_SET); // Data mode

    // Üst nibble (4 MSB)
    LCD_SendNibble(data >> 4);
    // Alt nibble (4 LSB)
    LCD_SendNibble(data & 0x0F);
}

void LCD_String(const char *str) {
    while(*str) {
        LCD_Char(*str++);
    }
}

void LCD_Clear(void) {
    LCD_Command(0x01);
    HAL_Delay(2);
}

void LCD_SetCursor(uint8_t row, uint8_t column) {
    uint8_t address;

    // 16x2 LCD için satır adresleri
    if(row == 0) {
        address = 0x80 + column;
    } else {
        address = 0xC0 + column;
    }

    LCD_Command(address);
}

void LCD_Init(void) {
    HAL_Delay(50); // LCD güç stabilizasyonu için bekle
    
    // 4-bit mod başlatma
    LCD_SendNibble(0x03);
    HAL_Delay(5);
    LCD_SendNibble(0x03);
    HAL_Delay(1);
    LCD_SendNibble(0x03);
    HAL_Delay(1);
    LCD_SendNibble(0x02); // 4-bit moda geçiş

    // 4-bit mod, 2 satır, 5x8 font
    LCD_Command(0x28);
    HAL_Delay(1);

    // Display on, cursor off, blink off
    LCD_Command(0x0C);
    HAL_Delay(1);
    
    // Clear display
    LCD_Clear();
    
    // Entry mode set - imleç sağa hareket, display shift yok
    LCD_Command(0x06);
    HAL_Delay(1);
}
