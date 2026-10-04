#include "eink_gfx.h"

#define SCREEN_WIDTH 200
#define SCREEN_HEIGHT 200

#define EN 32
#define RU 0xC0

int FB_DrawPixel(uint8_t *fbuffer, uint16_t x, uint16_t y, EINK_Color color) {

    if (x >= SCREEN_WIDTH || y >= SCREEN_HEIGHT) return 0;
    
    uint32_t pixel = y * SCREEN_WIDTH + x; //номер пикселя на экране
    uint32_t byte_index = pixel / 4; //позиция байта в массиве 
    uint32_t pixel_in_byte = pixel % 4; //позиция пикселя в байте (в 1 байте их 4)

    uint32_t shift = 6 - pixel_in_byte * 2 ; //сдвиг цвета 

    fbuffer[byte_index] &= ~(0x3 << shift); //стереть старое значение
    fbuffer[byte_index] |= (color << shift);

	return 1;
}

int FB_FillRect(uint8_t *fbuffer,uint16_t x, uint16_t y, uint16_t w, uint16_t h, EINK_Color color) {

    if((x >= SCREEN_WIDTH) || (y >= SCREEN_HEIGHT)) return 0;
    if(w <= 0 || h <= 0) return 0;
  
    if(x + w >  SCREEN_WIDTH)  w = SCREEN_WIDTH  - x;
    if(y + h > SCREEN_HEIGHT) h = SCREEN_HEIGHT - y;
  
  
  for(int16_t i=0; i<h; i++) {
      for(int16_t j=0; j<w; j++) {
  
      FB_DrawPixel(fbuffer, x + j, y + i, color);
  
      }
    }
     return 1;
}

void FB_DrawChar(uint8_t *fbuffer,uint16_t x, uint16_t y, char c, EINK_Color color,FontDef font) {

//    const uint16_t* char_data = font.data + ((int)c-32)*font.height;
// индекс символа × высота   

const uint16_t* char_data;

	if (c < 128) {

		char_data = font.data + ((int)c-32)*font.height; // EN
	} 

	if (c >= 0xC0) {

		char_data = font.data + ((int)c-0xC0)*font.height; // RU
	}



    // Проходим по всем 8 строкам
    for (int row = 0; row < font.height; row++) {

    uint16_t row_data = char_data[row];// Проходим по всем 8 стoлбцам
            
		for (int col = 0; col < font.width; col++){
		
 		uint16_t mask = (1 << ( 15 - col)); // 

			if (row_data & mask){ //0x80 - 0b10000000
             // Рисуем пиксель, если бит = 1
             FB_DrawPixel(fbuffer, x + col, y + row, color);
			} //end if
		} //end for 2
	} //end for 1	
}


void FB_DrawString(uint8_t *fbuffer,uint16_t x, uint16_t y, 
						const char* str, EINK_Color color,
						FontDef font) {


    uint16_t cursor_x = x;

    while (*str) {

        FB_DrawChar(fbuffer, cursor_x, y, *str, color,font);
        cursor_x += font.width;  // Ширина символа

        // Проверка выхода за границу экрана
        if (cursor_x + font.width > SCREEN_WIDTH) {
            cursor_x = x;
            y += font.height;  // Переход на следующую строку
        }

     str++; 
     }  //end while
}


void FB_DrawLine(uint8_t *fbuffer,int16_t x0, int16_t y0,
                     int16_t x1, int16_t y1,
                     EINK_Color color)
{
    int16_t dx = abs(x1 - x0);
    int16_t step_x = (x0 < x1) ? 1 : -1; // шаг по х

    int16_t dy = -abs(y1 - y0);
    int16_t step_y = (y0 < y1) ? 1 : -1; // шаг по у

    int16_t err = dx + dy; // ошибка - отклонение


    while (1)
    {
        FB_DrawPixel(fbuffer, x0, y0, color);

        if (x0 == x1 && y0 == y1)
            break;

        int16_t e2 = 2 * err;

        if (e2 >= dy)
        {
            err += dy;
            x0 += step_x;
        }

        if (e2 <= dx)
        {
            err += dx;
            y0 += step_y;
        }
    }

}





void bresenhamCircle(uint8_t *fbuffer,int16_t x0, int16_t y0, int16_t radius, EINK_Color color){

	int16_t x = 0;
	int16_t y = radius;
	int16_t d = 3 - 2 * radius; // Начальное значение параметра принятия решения

	while (y >= x){
		// Используем симметрию окружности для отрисовки точек во всех октантах

		FB_DrawPixel(fbuffer, x0 + x, y0 + y,color);
		FB_DrawPixel(fbuffer, x0 + y, y0 + x,color);
		FB_DrawPixel(fbuffer, x0 - y, y0 + x,color);
		FB_DrawPixel(fbuffer, x0 - x, y0 + y,color);
		FB_DrawPixel(fbuffer, x0 - x, y0 - y,color);
		FB_DrawPixel(fbuffer, x0 - y, y0 - x,color);
		FB_DrawPixel(fbuffer, x0 + y, y0 - x,color);
		FB_DrawPixel(fbuffer, x0 + x, y0 - y,color);

		if (d < 0) {

		d += 4 * x + 6;
		} else {

		d += 4 * (x - y) + 10;
		y--;
		}

	x++;
	}
}


void bresenhamCircleFill(uint8_t *fbuffer,int16_t x0, int16_t y0, int16_t radius, EINK_Color color)
{
    int16_t x = 0;
    int16_t y = radius;
    int16_t d = 3 - 2 * radius;

    while (y >= x){
        // рисуем заливку горизонтальными линиями (8 октантов)

        FB_DrawLine(fbuffer, x0 - x, y0 + y, x0 + x, y0 + y, color);
        FB_DrawLine(fbuffer, x0 - x, y0 - y, x0 + x, y0 - y, color);

        FB_DrawLine(fbuffer, x0 - y, y0 + x, x0 + y, y0 + x, color);
        FB_DrawLine(fbuffer, x0 - y, y0 - x, x0 + y, y0 - x,  color);

        if (d < 0){

        d += 4 * x + 6;
        } else {
        
        d += 4 * (x - y) + 10;
        y--;

        }

    x++;
    }
}
