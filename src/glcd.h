/*
 * glcd.h
 *
 *  Created on: Nov 25, 2019
 *  Updated: 8:00 11/27/2019
 *      Author: dave
 */

#ifndef GLCD_H_
#define GLCD_H_

void nano_wait(int t);
void micro_wait(int t);
void en_pulse(void);
void gpiob_init(void);
void clear_data(void);
void GLCD_init(void);
void GLCD_on(void);
void set_line(uint8_t line);
void goto_col(uint8_t x);
void goto_row(uint8_t y);
void goto_xy(uint8_t x, uint8_t y);
void GLCD_write(uint8_t data);
void GLCD_clearLine(uint8_t ln);
void GLCD_clear(void);
void dot(uint8_t x, uint8_t y, int color);
void dot_draw(uint8_t x, uint8_t y, int color);
void GLCD_fill(void);
void GLCD_fillLine(uint8_t ln);
void GLCD_refreshLine(uint8_t ln);
void GLCD_refreshFrame(void);
void rectangle(uint8_t x, uint8_t y, uint8_t a, uint8_t b, int color);
void line(uint8_t x, uint8_t y, uint8_t a, uint8_t b, int color);
void circle(uint8_t x, uint8_t y, uint8_t r, int color);
void image(uint8_t *sprite, int w, int h, uint8_t x, uint8_t y);

#endif /* GLCD_H_ */
