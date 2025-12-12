// cursor.h
#pragma once
#include <stdint.h>
void cursor_init(int start_x, int start_y);
void cursor_move(int dx, int dy);
void cursor_set(int x, int y);
void cursor_draw(void);
int cursor_x(void);
int cursor_y(void);
