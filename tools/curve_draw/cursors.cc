#include <SDL.h>
#include "cursors.h"

static SDL_Cursor *create_cursor(const char **image);

SDL_Cursor *cursor_std, *cursor_cross, *cursor_x;

static const char *cur_std[] = {
  /* width height num_colors chars_per_pixel */
  "    32    32        3            1",
  /* colors */
  "X c #000000",
  ". c #ffffff",
  "  c None",
  /* pixels */
  ".                               ",
  "..                              ",
  ".X.                             ",
  ".XX.                            ",
  ".XXX.                           ",
  ".XXXX.                          ",
  ".XXXXX.                         ",
  ".XXXXXX.                        ",
  ".XXXXXXX.                       ",
  ".XXXXXXXX.                      ",
  ".XXXXX.....                     ",
  ".XX.XX.                         ",
  ".X. .XX.                        ",
  "..  .XX.                        ",
  ".    .XX.                       ",
  "     .XX.                       ",
  "      .XX.                      ",
  "      .XX.                      ",
  "       ..                       ",
  "                                ",
  "                                ",
  "                                ",
  "                                ",
  "                                ",
  "                                ",
  "                                ",
  "                                ",
  "                                ",
  "                                ",
  "                                ",
  "                                ",
  "                                ",
  "0,0"
};

static const char *cur_cross[] = {
  /* width height num_colors chars_per_pixel */
  "    32    32        3            1",
  /* colors */
  "X c #000000",
  ". c #ffffff",
  "  c None",
  /* pixels */
  "               .                ",
  "              .X.               ",
  "              .X.               ",
  "              .X.               ",
  "              .X.               ",
  "              .X.               ",
  "              .X.               ",
  "              .X.               ",
  "              .X.               ",
  "              .X.               ",
  "              .X.               ",
  "              .X.               ",
  "              .X.               ",
  "               .                ",
  " ............     ............  ",
  ".XXXXXXXXXXXX.   .XXXXXXXXXXXX. ",
  " ............     ............  ",
  "               .                ",
  "              .X.               ",
  "              .X.               ",
  "              .X.               ",
  "              .X.               ",
  "              .X.               ",
  "              .X.               ",
  "              .X.               ",
  "              .X.               ",
  "              .X.               ",
  "              .X.               ",
  "              .X.               ",
  "              .X.               ",
  "               .                ",
  "                                ",
  "15,15"
};

static const char *cur_x[] = {
  /* width height num_colors chars_per_pixel */
  "    32    32        3            1",
  /* colors */
  "X c #000000",
  ". c #ffffff",
  "  c None",
  /* pixels */
  "                                ",
  "                                ",
  "  ..                       ..   ",
  "  .X.                     .X.   ",
  "   .X.                   .X.    ",
  "    .X.                 .X.     ",
  "     .X.               .X.      ",
  "      .X.             .X.       ",
  "       .X.           .X.        ",
  "        .X.   ...   .X.         ",
  "         .X. .XXX. .X.          ",
  "          .X.X...X.X.           ",
  "           .X.   .X.            ",
  "          .X.     .X.           ",
  "         .X.       .X.          ",
  "         .X.       .X.          ",
  "         .X.       .X.          ",
  "          .X.     .X.           ",
  "           .X.   .X.            ",
  "          .X.X...X.X.           ",
  "         .X. .XXX. .X.          ",
  "        .X.   ...   .X.         ",
  "       .X.           .X.        ",
  "      .X.             .X.       ",
  "     .X.               .X.      ",
  "    .X.                 .X.     ",
  "   .X.                   .X.    ",
  "  .X.                     .X.   ",
  "  ..                       ..   ",
  "                                ",
  "                                ",
  "                                ",
  "15,15"
};



void init_cursors() {
	cursor_std = create_cursor(cur_std);
	cursor_cross = create_cursor(cur_cross);
	cursor_x = create_cursor(cur_x);
}

static SDL_Cursor *create_cursor(const char **image) {
	int i, row, col;
	Uint8 data[4*32];
	Uint8 mask[4*32];
	int hot_x, hot_y;

	i = -1;
	for(row=0; row<32; ++row) {
		for (col=0; col<32; ++col) {
			if(col % 8) {
				data[i] <<= 1;
				mask[i] <<= 1;
			} else {
				++i;
				data[i] = mask[i] = 0;
			}
			switch(image[4+row][col]) {
			case 'X':
				data[i] |= 0x01;
				mask[i] |= 0x01;
				break;
			case '.':
				mask[i] |= 0x01;
				break;
			case ' ':
				break;
			}
		}
	}
	sscanf(image[4+row], "%d,%d", &hot_x, &hot_y);
	return SDL_CreateCursor(data, mask, 32, 32, hot_x, hot_y);
}
