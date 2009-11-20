#include <iostream>
#include <vector>
#include <SDL.h>
#include "vmath/vmath.h"
#include "curves.h"
#include "cursors.h"
#include "gfx.h"

using namespace std;

enum {
	TOOL_SELECT,
	TOOL_INSERT,
	TOOL_REMOVE
};

#define TOOL_COUNT	3

SDL_Cursor *tool_cursors[TOOL_COUNT];

struct EditState {
	Vector2 view_size;
	unsigned int tool;
	std::vector<Curve*> curves;
	int sel_curve;
	int sel_pt;
	int sel_x, sel_y;
	bool draw_wire;
	bool draw_pt;
};

void reset_state(EditState *s = 0);
void redraw();
Vector2 world_to_dev(const Vector2 &pt);
Vector2 dev_to_world(const Vector2 &pt);
void key_handler(SDLKey key, bool pressed);
void motion_handler(int x, int y);
void bn_handler(int x, int y, int bn, bool pressed);


int xsz = 800, ysz = 600;
SDL_Surface *sdl_fb;
bool done = false;
EditState state;
bool view_valid = false;

int main(int argc, char **argv) {
	const char *usage = "curve_draw [-s(--size) WxH] [curve(s) to load]\n";

	reset_state();

	for(int i=1; i<argc; i++) {
		if(argv[i][0] == '-') {
			if(!strcmp(argv[i], "-s") || !strcmp(argv[i], "--size")) {
				if(!isdigit(argv[++i][0])) {
					cerr << "invalid size argument: " << argv[i] << endl;
					return EXIT_FAILURE;
				}
				xsz = atoi(argv[i]);

				char *ptr = strchr(argv[i], 'x');
				if(!ptr || !isdigit(*(ptr + 1))) {
					cerr << "invalid size argument: " << argv[i] << endl;
					return EXIT_FAILURE;
				}
				ysz = atoi(ptr + 1);
				
			} else if(!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) {
				fputs(usage, stdout);
				return 0;
			} else {
				fputs(usage, stderr);
				return EXIT_FAILURE;
			}
		} else {
			Curve *curve = load_curve(argv[i]);
			if(!curve) {
				fprintf(stderr, "failed to load curve: %s\n", argv[i]);
				return EXIT_FAILURE;
			}
			state.curves.push_back(curve);
			view_valid = false;
		}
	}
	
	SDL_Init(SDL_INIT_VIDEO);
	if(!(sdl_fb = SDL_SetVideoMode(xsz, ysz, 32, SDL_SWSURFACE))) {
		cerr << "failed to set initialize graphics\n";
		return EXIT_FAILURE;
	}

	SDL_WM_SetCaption("curvedraw - press h for usage help", 0);

	init_cursors();

	tool_cursors[TOOL_SELECT] = cursor_std;
	tool_cursors[TOOL_INSERT] = cursor_cross;
	tool_cursors[TOOL_REMOVE] = cursor_x;


	SDL_Event event;
	while(!done && SDL_WaitEvent(&event)) {
		do {
			switch(event.type) {
			case SDL_QUIT:
				done = true;
				break;

			case SDL_KEYDOWN:
			case SDL_KEYUP:
				key_handler(event.key.keysym.sym, event.type == SDL_KEYDOWN);
				break;

			case SDL_MOUSEMOTION:
				motion_handler(event.motion.x, event.motion.y);
				break;

			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP:
				bn_handler(event.button.x, event.button.y, event.button.button, event.button.state == SDL_PRESSED);
				break;

			default:
				break;
			}
		} while(SDL_PollEvent(&event));

		if(!view_valid) redraw();
	}
	
	SDL_Quit();
	return 0;
}


void reset_state(EditState *s) {
	if(!s) s = &state;

	s->view_size.y = 100;
	s->view_size.x = s->view_size.y * ((float)xsz / (float)ysz);

	s->tool = TOOL_SELECT;
	s->sel_curve = -1;
	s->sel_pt = -1;
	s->sel_x = s->sel_y = -1;

	s->draw_wire = false;
	s->draw_pt = true;

	for(size_t i=0; i<s->curves.size(); i++) {
		delete s->curves[i];
	}
	s->curves.clear();
}

void redraw() {

	SDL_FillRect(sdl_fb, 0, 0);
	
	if(SDL_MUSTLOCK(sdl_fb)) {
		SDL_LockSurface(sdl_fb);
	}
	unsigned int *fb = (unsigned int*)sdl_fb->pixels;

	memset(fb + xsz * ysz / 2, 0x18, xsz * 4);

	unsigned int *ptr = fb + xsz / 2;
	for(int i=0; i<ysz; i++) {
		*ptr = 0x181818;
		ptr += xsz;
	}
	

	for(size_t i=0; i<state.curves.size(); i++) {
		Vector2 prev;

		// draw a line between control points
		if(state.draw_wire) {
			for(int j=0; j<state.curves[i]->get_point_count(); j++) {
				Vector3 *ptptr = state.curves[i]->get_control_point(j);
				Vector2 pt = world_to_dev(*ptptr);

				if(j > 0) {
					draw_line((int)prev.x, (int)prev.y, (int)pt.x, (int)pt.y, 0x404040, fb, xsz);
				}
				prev = pt;
			}
		}


		// draw the spline as a polyline
		const int draw_seg = 12 * state.curves[i]->get_segment_count();
		for(int j=0; j<=draw_seg; j++) {
			float t = (float)j / (float)draw_seg;
			Vector2 pt = (*state.curves[i])(t);

			pt = world_to_dev(pt);

			if(j > 0) {
				unsigned int col = state.sel_curve == (int)i ? 0x00ff00 : 0x0000ff;
				draw_line((int)prev.x, (int)prev.y, (int)pt.x, (int)pt.y, col, fb, xsz);
			}
			
			prev = pt;
		}

		// draw the control points
		if(state.draw_pt) {
			for(int j=0; j<state.curves[i]->get_point_count(); j++) {
				Vector3 *ptptr = state.curves[i]->get_control_point(j);
				Vector2 pt = world_to_dev(*ptptr);

				unsigned int col = 0x808080;
				if(state.sel_curve == (int)i) {
					col = state.sel_pt == j ? 0xffff00 : 0xff0000;
				}
			
				draw_point((int)pt.x, (int)pt.y, col, fb, xsz);
			}
		}
	}

	if(SDL_MUSTLOCK(sdl_fb)) {
		SDL_UnlockSurface(sdl_fb);
	}

	SDL_Flip(sdl_fb);
	view_valid = true;
}

Vector2 world_to_dev(const Vector2 &pt) {
	Vector2 norm = (Vector2(1, -1) * pt + state.view_size / 2.0) / state.view_size;
	return norm * Vector2(xsz, ysz);
}

Vector2 dev_to_world(const Vector2 &pt) {
	Vector2 norm = pt / Vector2(xsz, ysz);
	return Vector2(1, -1) * ((norm * state.view_size) - state.view_size / 2.0);
}


void key_handler(SDLKey key, bool pressed) {
	switch(key) {
	case 'q':
		done = true;
		break;

	case SDLK_ESCAPE:
		state.sel_curve = -1;
		state.sel_pt = -1;
		view_valid = false;
		break;

	case '1':
		redraw();
		break;

	case 'w':
		if(!pressed) {
			state.draw_wire = !state.draw_wire;
			view_valid = false;
		}
		break;

	case 'p':
		if(!pressed) {
			state.draw_pt = !state.draw_pt;
			view_valid = false;
		}
		break;

	case SDLK_LEFT:
		if(!pressed) {
			if(state.sel_curve <= 0) {
				state.sel_curve = state.curves.size() - 1;
			} else {
				state.sel_curve--;
			}
			state.sel_pt = -1;
			view_valid = false;
		}
		break;

	case SDLK_SPACE:
	case SDLK_RIGHT:
		if(!pressed) {	
			if(state.sel_curve == -1 || state.sel_curve >= (int)state.curves.size() - 1) {
				state.sel_curve = 0;
			} else {
				state.sel_curve++;
			}
			state.sel_pt = -1;
			view_valid = false;
		}
		break;

	case SDLK_RETURN:
		if(!pressed && state.sel_curve != -1) {
			Curve *current = state.curves[state.sel_curve];
			Curve *curve = dynamic_cast<BSplineCurve*>(current) ? (Curve*)new CatmullRomSplineCurve : (Curve*)new BSplineCurve;

			for(int i=0; i<current->get_point_count(); i++) {
				Vector3 pt = *current->get_control_point(i);
				pt.z = 0;
				curve->add_control_point(pt);
			}

			delete current;
			state.curves[state.sel_curve] = curve;
			view_valid = false;
		}
		break;

	case 'h':
		if(!pressed) {
			cout << "----- controls -----\n";
			cout << "[keyboard]\n";
			cout << "space - cycle through curves\n";
			cout << "right - cycle through curves forward\n";
			cout << "left  - cycle through curves backwards\n";
			cout << "esc   - deselect selected curve\n";
			cout << "enter - toggle curve type (bspline/catmull-rom)\n";
			cout << "q     - quit\n";
			cout << "s     - save all curves to current directory\n";
			cout << "[mouse]\n";
			cout << "left-button  - perform action (select tool also drags points around)\n";
			cout << "right-button - cycle through tools (select/insert/remove)\n";
			cout << "mousewheel   - zoom in/out\n";
			cout << endl;
		}
		break;

	case 's':
		if(!pressed) {
			char fname[128];
			for(size_t i=0; i<state.curves.size(); i++) {
				sprintf(fname, "curve%02d", (int)i);
				save_curve(fname, state.curves[i]);
			}
			cout << state.curves.size() << " curves saved.\n";
		}
		break;

	default:
		break;
	}
}

void motion_handler(int x, int y) {
	Vector2 mouse = dev_to_world(Vector2(x, y));
	
	if(state.sel_x != -1) {
		if(x != state.sel_x || y != state.sel_y) {
			Curve *curve = state.curves[state.sel_curve];
			Vector3 *ptptr = curve->get_control_point(state.sel_pt);
			*ptptr = mouse;

			state.sel_x = x;
			state.sel_y = y;
			view_valid = false;
		}
	}
}

void bn_handler(int x, int y, int bn, bool pressed) {
	Vector2 mouse = dev_to_world(Vector2(x, y));

	if(bn == 3 && pressed) {
		state.tool = (state.tool + 1) % TOOL_COUNT;
		SDL_SetCursor(tool_cursors[state.tool]);
	}
	
	if(bn == 1 && state.tool == TOOL_SELECT) {
		if(pressed) {
			
			if(state.sel_curve >= 0) {
				Curve *curve = state.curves[state.sel_curve];
				for(int i=0; i<curve->get_point_count(); i++) {
					Vector3 *ptptr = curve->get_control_point(i);
					Vector2 pt = *ptptr;
				
					if((pt - mouse).length() < state.view_size.y * 0.008) {
						state.sel_pt = i;
						state.sel_x = x;
						state.sel_y = y;
						view_valid = false;
					}
				}
			}

		} else {	// depressed
			state.sel_x = -1;
			state.sel_y = -1;
		}
	}

	if(bn == 1 && state.tool == TOOL_REMOVE) {
		static int press_x, press_y;
		
		if(pressed) {
			press_x = x;
			press_y = y;
		} else {
			if(press_x == x && press_y == y && state.sel_curve >= 0) {
				Curve *curve = state.curves[state.sel_curve];
				for(int i=0; i<curve->get_point_count(); i++) {
					Vector3 *ptptr = curve->get_control_point(i);
					Vector2 pt = *ptptr;
				
					if((pt - mouse).length() < state.view_size.y * 0.008) {
						curve->remove_control_point(i);
						if(state.sel_pt >= i) state.sel_pt--;
						if(!curve->get_point_count()) {
							delete curve;
							state.curves.erase(state.curves.begin() + state.sel_curve);
						}
						view_valid = false;
						break;
					}
				}
			}
		}
	}

	if(bn == 1 && state.tool == TOOL_INSERT) {
		static int press_x, press_y;
		
		if(pressed) {
			press_x = x;
			press_y = y;
		} else {
			if(press_x == x && press_y == y) {
				if(state.sel_curve == -1) {
					// create a new curve...
					Curve *curve = new CatmullRomSplineCurve;
					curve->add_control_point(Vector3(mouse.x, mouse.y, 0.0));

					state.sel_curve = state.curves.size();
					state.sel_pt = 0;
					state.curves.push_back(curve);
				} else {
					// add a control point to the selected curve
					Curve *curve = state.curves[state.sel_curve];
					state.sel_pt = curve->get_point_count();
					curve->add_control_point(Vector3(mouse.x, mouse.y, 0.0));
				}
				view_valid = false;
			}
		}
	}

	if(bn == 4 && pressed) {
		state.view_size *= 0.95;
		view_valid = false;
	}

	if(bn == 5 && pressed) {
		state.view_size *= 1.05;
		view_valid = false;
	}
}
