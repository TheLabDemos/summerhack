#include "3dengfx/3dengfx.hpp"

using namespace engfx_state;

static float zoom_factor = 1.0;

void key_handler(int key) {
	TargetCamera *cam = dynamic_cast<TargetCamera*>(const_cast<Camera*>(view_mat_camera));
	
	switch(key) {
	case 'a':
		cam->zoom(0.8);
		zoom_factor *= 0.8;
		break;

	case 'z':
		cam->zoom(1.2);
		zoom_factor *= 1.2;
		break;

	case '`':
		screen_capture();
		break;

	case 27:
		exit(0);

	default:
		break;
	}
}

static int prev_x = -1;
static int prev_y = -1;

void motion_handler(int x, int y) {
	if(prev_x != -1) {
		TargetCamera *cam = dynamic_cast<TargetCamera*>(const_cast<Camera*>(view_mat_camera));
		float dx = (float)(x - prev_x) * 0.01;
		float dy = (float)(y - prev_y) * 1.0;
		
		cam->rotate(Vector3(0, -dx, 0));
		cam->translate(Vector3(0, -dy * zoom_factor, 0));
		
		prev_x = x;
		prev_y = y;
	}
}

void bn_handler(int bn, int pressed, int x, int y) {
	TargetCamera *cam = dynamic_cast<TargetCamera*>(const_cast<Camera*>(view_mat_camera));

	switch(bn) {
	case fxwt::BN_RIGHT:
		if(pressed) {
			prev_x = x;
			prev_y = y;
		} else {
			prev_x = prev_y = -1;
		}
		break;

	case fxwt::BN_WHEELUP:
		cam->zoom(0.9);
		zoom_factor *= 0.9;
		break;

	case fxwt::BN_WHEELDOWN:
		cam->zoom(1.1);
		zoom_factor *= 1.1;
		break;

	default:
		break;
	}
}
