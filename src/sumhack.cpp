#include <iostream>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include "3dengfx/3dengfx.hpp"
#include "dsys/demosys.hpp"
#include "common/err_msg.h"
#include "parts/parts.hpp"
#include "events.hpp"
#include "sdlvf/sdlvf.h"

using namespace std;

bool init();
void update_gfx();
void clean_up();

dsys::Part *part;
ntimer timer;

std::vector<dsys::Part*> parts;

bool music = true;

bool render = false;
int render_fps = 25;
const char *render_path = "frames";

unsigned long avg_frame_time, frame_count;

const char *help_str = "command line options\n------------------\n"
	"-r\trender the demo as a sequence of targa files\n"
	"-f <fps>\tset the rendering framerate for -r above\n"
	"-p <path>\tset the directory in which to render the sequnce for -r.\n"
	"-m\ttoggle music.\n"
	"-h\tthis help screen\n\n";

int main(int argc, char **argv) {
	for(int i=1; i<argc; i++) {
		if(argv[i][0] == '-' && argv[i][2] == 0) {
			switch(argv[i][1]) {
			case 'r':
				render = true;
				break;

			case 'f':
				if(isdigit(argv[++i][0])) {
					render_fps = atoi(argv[i]);
				} else {
					cerr << "invalid argument -f " << argv[i] << endl;
					return EXIT_FAILURE;
				}
				break;

			case 'p':
				render_path = argv[++i];
				break;

			case 'm':
				music = !music;
				break;

			default:
				cout << help_str;
				return argv[i][1] == 'h' ? 0 : EXIT_FAILURE;
			}
		} else {
			cout << help_str;
			return EXIT_FAILURE;
		}
	}

	if(!init()) {
		return EXIT_FAILURE;
	}

	return fxwt::main_loop();
}

bool init() {
	GraphicsInitParameters *gip;
	if(!(gip = load_graphics_context_config("3dengfx.conf"))) {
		return false;
	}

	if(!create_graphics_context(*gip)) {
		return false;
	}

	if(gip->fullscreen) {
		fxwt::show_cursor(false);
	}

	fxwt::set_window_title("The Lab Demos :: Summer Hack (rebuild)");
	fxwt::set_display_handler(update_gfx);
	fxwt::set_idle_handler(update_gfx);
	fxwt::set_keyboard_handler(key_handler);
	fxwt::set_motion_handler(motion_handler);
	fxwt::set_button_handler(bn_handler);
	atexit(clean_up);

	//dsys::init();

	// show loading screen
	glClear(GL_COLOR_BUFFER_BIT);
	dsys::overlay(get_texture("data/img/loading.jpg"), Vector2(0, 0), Vector2(1, 1), 1.0);
	flip();

	// check if we have all the textures we need
	FILE *fp = fopen("data/tex_list", "r");
	if(!fp) {
		error("couldn't find texture list, run this only from its directory");
		return false;
	}

	char tex_name[512];
	while(fgets(tex_name, 512, fp)) {
		char *last_char = tex_name + strlen(tex_name) - 1;
		if(*last_char == '\n') {
			*last_char = 0;
		}
		if(!get_texture(tex_name)) {
			error("failed to load texture: \"%s\"", tex_name);
			return false;
		}
	}
	fclose(fp);
	
	set_scene_data_path("data/img");

	INIT_PARTS();

	for(size_t i=0; i<parts.size(); i++) {
		add_part(parts[i]);
	}

	if(!render) {
		dsys::start_demo();
	} else {
		dsys::render_demo(render_fps, render_path);
	}

	if(music) {
		sdlvf_init("data/music/red_herring.ogg");
	}
	
	timer_reset(&timer);
	timer_start(&timer);

	return true;
}

void clean_up() {
	unsigned long time = timer_getmsec(&timer);

	if(music) {
		sdlvf_done();
	}

	fxwt::show_cursor(true);

	for(size_t i=0; i<parts.size(); i++) {
		delete parts[i];
	}
	destroy_graphics_context();

	if(frame_count) {
		avg_frame_time /= frame_count;
		printf("average frame time: %lu msec, average fps: %.2f\n", avg_frame_time, 1000.0 / avg_frame_time);
	}

	time /= 1000;
	cout << "ran for ";
	if(time < 60) {
		cout << time << " seconds\n";
	} else {
		cout << time / 60 << ":" << time - (time / 60) * 60 << "\n";
	}
}

void update_gfx() {
	unsigned long frame_start = timer_getmsec(&timer);
	
	if(music) {
		int sdlvf_err;
		if((sdlvf_err = sdlvf_check()) != SDLVF_PLAYING) {
			fprintf(stderr, "quit: %s", sdlvf_strerror(sdlvf_err));
			exit(0);
		}
	}
	
	if(dsys::update_graphics() == -1) {
		exit(0);
	}

	avg_frame_time += timer_getmsec(&timer) - frame_start;
	frame_count++;
}
