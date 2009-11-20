#include "gfx.h"

#define RGB(r, g, b)	(((r) << 16) & 0xff0000) | (((g) << 8) & 0x00ff00) | ((b) & 0xff)

static int clip_line(int *x1, int *y1, int *x2, int *y2);


extern int xsz, ysz;
	

void draw_point(int x, int y, unsigned int col, unsigned int *ptr, int xsz) {
	const int sz = 9;
	const int pt_img[sz][sz] = {
		{0,0,0,0,1,0,0,0,0},
		{0,0,1,1,1,1,1,0,0},
		{0,1,2,2,1,1,1,3,0},
		{0,1,2,1,1,1,1,3,0},
		{1,1,1,1,1,1,1,3,3},
		{0,1,1,1,1,1,3,3,0},
		{0,1,1,1,1,3,3,3,0},
		{0,0,3,3,3,3,3,0,0},
		{0,0,0,0,3,0,0,0,0}
	};

	if(x < sz/2 || x >= xsz - sz/2 || y < sz/2 || y >= ysz - sz/2) {
		return;
	}

	int base_r = (col & 0xff0000) >> 16;
	int base_g = (col & 0x00ff00) >> 8;
	int base_b = (col & 0x0000ff);

	unsigned int dark = RGB(base_r - base_r/4, base_g - base_g/4, base_b - base_b/4);;
	
	unsigned int ctable[] = {0, col, 0xffffff, dark};
	ptr += (y - sz / 2) * xsz + (x - sz / 2);

	for(int i=0; i<sz; i++) {
		for(int j=0; j<sz; j++) {
			if(pt_img[i][j]) {
				*(ptr + j) = ctable[pt_img[i][j]];
			}
		}
		ptr += xsz;
	}
}

void draw_line(int x1, int y1, int x2, int y2, unsigned int col, unsigned int *ptr, int xsz) {
	int dx, dy, dx2, dy2;
	int x_inc, y_inc;
	int error;
	int i;

	if(!clip_line(&x1, &y1, &x2, &y2)) return;

	ptr += y1 * xsz + x1;
	dx = x2 - x1;
	dy = y2 - y1;
	
	if(dx >= 0) {
		x_inc = 1;
	} else {
		x_inc = -1;
		dx = -dx;
	}
	
	if(dy >= 0) {
		y_inc = xsz;
	} else {
		y_inc = -xsz;
		dy = -dy;
	}
	
	dx2 = dx << 1;
	dy2 = dy << 1;

	if(dx > dy) {
		error = dy2 - dx;
		for(i=0; i<=dx; i++) {
			*ptr = col;
			if(error >= 0) {
				error -= dx2;
				ptr += y_inc;
			}
			error += dy2;
			ptr += x_inc;
		}
	} else {
		error = dx2 - dy;
		for(i=0;i<=dy;i++) {
			*ptr = col;
			if(error >= 0) {
				error -= dy2;
				ptr += x_inc;
			}
			error += dx2;
			ptr += y_inc;
		}
	}
}

enum {
	CLIP_C		= 0x0000,
	CLIP_N		= 0x0008,
	CLIP_S		= 0x0004,
	CLIP_E		= 0x0002,
	CLIP_W		= 0x0001,
	CLIP_NE		= 0x000a,
	CLIP_SE		= 0x0006,
	CLIP_NW		= 0x0009,
	CLIP_SW		= 0x0005
};

#define MIN_CLIP_X	0
#define MIN_CLIP_Y	0
#define MAX_CLIP_X	(xsz - 1)
#define MAX_CLIP_Y	(ysz - 1)

static int clip_line(int *x1, int *y1, int *x2, int *y2) {
	int xc1 = *x1;
	int yc1 = *y1;
	int xc2 = *x2;
	int yc2 = *y2;
	int p1_code = 0;
	int p2_code = 0;

	// determine codes for p1 and p2
	if(*y1 < MIN_CLIP_Y) {
		p1_code |= CLIP_N;
	} else {
		if(*y1 > MAX_CLIP_Y) {
			p1_code |= CLIP_S;
		} else {
			if(*x1 < MIN_CLIP_X) {
				p1_code |= CLIP_W;
			} else {
				if(*x1 > MAX_CLIP_X) p1_code |= CLIP_E;
			}
		}
	}

	if(*y2 < MIN_CLIP_Y) {
		p2_code |= CLIP_N;
	} else {
		if(*y2 > MAX_CLIP_Y) {
			p2_code |= CLIP_S;
		} else {
			if(*x2 < MIN_CLIP_X) {
				p2_code |= CLIP_W;
			} else {
				if(*x2 > MAX_CLIP_X) p2_code |= CLIP_E;
			}
		}
	}

	if((p1_code & p2_code)) return 0; // trivial rejection
	if(p1_code == 0 && p2_code == 0) return 1; // test if totally visible

	// find clip point for p1
	switch(p1_code) {
		case CLIP_C: break;
		
		case CLIP_N:
			yc1 = MIN_CLIP_Y;
			xc1 = *x1 + 0.5 + (MIN_CLIP_Y - *y1) * (*x2 - *x1) / (*y2 - *y1);
			break;

		case CLIP_S:
			yc1 = MAX_CLIP_Y;
			xc1 = *x1 + 0.5 + (MAX_CLIP_Y - *y1) * (*x2 - *x1) / (*y2 - *y1);
			break;

		case CLIP_W:
			xc1 = MIN_CLIP_X;
			yc1 = *y1 + 0.5 + (MIN_CLIP_X - *x1) * (*y2 - *y1) / (*x2 - *x1);
			break;

		case CLIP_E:
			xc1 = MAX_CLIP_X;
			yc1 = *y1 + 0.5 + (MAX_CLIP_X - *x1) * (*y2 - *y1) / (*x2 - *x1);
			break;

		case CLIP_NE:
			yc1 = MIN_CLIP_Y;
			xc1 = *x1 + 0.5 + (MIN_CLIP_Y - *y1) * (*x2 - *x1) / (*y2 - *y1);
			if(xc1 < MIN_CLIP_X || xc1 > MAX_CLIP_X) {
				xc1 = MAX_CLIP_X;
				yc1 = *y1 + 0.5 + (MAX_CLIP_X - *x1) * (*y2 - *y1) / (*x2 - *x1);
			}
			break;

		case CLIP_SE:
			yc1 = MAX_CLIP_Y;
			xc1 = *x1 + 0.5 + (MAX_CLIP_Y - *y1) * (*x2 - *x1) / (*y2 - *y1);
			if(xc1 < MIN_CLIP_X || xc1 > MAX_CLIP_X) {
				xc1 = MAX_CLIP_X;
				yc1 = *y1 + 0.5 + (MAX_CLIP_X - *x1) * (*y2 - *y1) / (*x2 - *x1);
			}
			break;

		case CLIP_NW:
			yc1 = MIN_CLIP_Y;
			xc1 = *x1 + 0.5 + (MIN_CLIP_Y - *y1) * (*x2 - *x1) / (*y2 - *y1);
			if(xc1 < MIN_CLIP_X || xc1 > MAX_CLIP_X) {
				xc1 = MIN_CLIP_X;
				yc1 = *y1 + 0.5 + (MIN_CLIP_X - *x1) * (*y2 - *y1) / (*x2 - *x1);
			}
			break;

		case CLIP_SW:
			yc1 = MAX_CLIP_Y;
			xc1 = *x1 + 0.5 + (MAX_CLIP_Y - *y1) * (*x2 - *x1) / (*y2 - *y1);
			if(xc1 < MIN_CLIP_X || xc1 > MAX_CLIP_X) {
				xc1 = MIN_CLIP_X;
				yc1 = *y1 + 0.5 + (MIN_CLIP_X - *x1) * (*y2 - *y1) / (*x2 - *x1);
			}
			break;

		default: break;
	}

	// find clip point for p2
	switch(p2_code) {
		case CLIP_C: break;
		
		case CLIP_N:
			yc2 = MIN_CLIP_Y;
			xc2 = *x2 + 0.5 + (MIN_CLIP_Y - *y2) * (*x1 - *x2) / (*y1 - *y2);
			break;

		case CLIP_S:
			yc2 = MAX_CLIP_Y;
			xc2 = *x2 + 0.5 + (MAX_CLIP_Y - *y2) * (*x1 - *x2) / (*y1 - *y2);
			break;

		case CLIP_W:
			xc2 = MIN_CLIP_X;
			yc2 = *y2 + 0.5 + (MIN_CLIP_X - *x2) * (*y1 - *y2) / (*x1 - *x2);
			break;

		case CLIP_E:
			xc2 = MAX_CLIP_X;
			yc2 = *y1 + 0.5 + (MAX_CLIP_X - *x2) * (*y1 - *y2) / (*x1 - *x2);
			break;

		case CLIP_NE:
			yc2 = MIN_CLIP_Y;
			xc2 = *x2 + 0.5 + (MIN_CLIP_Y - *y2) * (*x1 - *x2) / (*y1 - *y2);
			if(xc2 < MIN_CLIP_X || xc2 > MAX_CLIP_X) {
				xc2 = MAX_CLIP_X;
				yc2 = *y2 + 0.5 + (MAX_CLIP_X - *x2) * (*y1 - *y2) / (*x1 - *x2);
			}
			break;

		case CLIP_SE:
			yc2 = MAX_CLIP_Y;
			xc2 = *x2 + 0.5 + (MAX_CLIP_Y - *y2) * (*x1 - *x2) / (*y1 - *y2);
			if(xc2 < MIN_CLIP_X || xc2 > MAX_CLIP_X) {
				xc2 = MAX_CLIP_X;
				yc2 = *y2 + 0.5 + (MAX_CLIP_X - *x2) * (*y1 - *y2) / (*x1 - *x2);
			}
			break;

		case CLIP_NW:
			yc2 = MIN_CLIP_Y;
			xc2 = *x2 + 0.5 + (MIN_CLIP_Y - *y2) * (*x1 - *x2) / (*y1 - *y2);
			if(xc2 < MIN_CLIP_X || xc2 > MAX_CLIP_X) {
				xc2 = MIN_CLIP_X;
				yc2 = *y2 + 0.5 + (MIN_CLIP_X - *x2) * (*y1 - *y2) / (*x1 - *x2);
			}
			break;

		case CLIP_SW:
			yc2 = MAX_CLIP_Y;
			xc2 = *x2 + 0.5 + (MAX_CLIP_Y - *y2) * (*x1 - *x2) / (*y1 - *y2);
			if(xc2 < MIN_CLIP_X || xc2 > MAX_CLIP_X) {
				xc2 = MIN_CLIP_X;
				yc2 = *y2 + 0.5 + (MIN_CLIP_X - *x2) * (*y1 - *y2) / (*x1 - *x2);
			}
			break;

		default: break;
	}

	// bounds check
	if(	(xc1 < MIN_CLIP_X) || (xc1 > MAX_CLIP_X) ||
		(yc1 < MIN_CLIP_Y) || (yc1 > MAX_CLIP_Y) ||
		(xc2 < MIN_CLIP_X) || (xc2 > MAX_CLIP_X) ||
		(yc2 < MIN_CLIP_Y) || (yc2 > MAX_CLIP_X)  ) return 0;

	*x1 = xc1;
	*y1 = yc1;
	*x2 = xc2;
	*y2 = yc2;

	return 1;
}


