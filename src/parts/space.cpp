#include "space.hpp"

static void make_skycube(Scene *scene);
static void make_particles(Scene *scene);
static void draw_design(float t);

static const float des_sz = 0.1;
static const int des_count = (int)(1.0 / des_sz);
static const float des_offs = des_sz;

#define ANIM

SpacePart::SpacePart() : ScenePart("space", new Scene) {
#ifdef ANIM
	TargetCamera *cam = new TargetCamera(Vector3(0, 15, -5));
#else
	TargetCamera *cam = new TargetCamera(Vector3(0, 30, -250));
#endif
	cam->set_fov(DEG_TO_RAD(50));
	scene->add_camera(cam);

	PointLight *lt = new PointLight(Vector3(10, 100, -100));
	scene->add_light(lt);

	scene->set_ambient_light(0.1);

	make_skycube(scene);
	make_particles(scene);

	MotionController cam_x, cam_y, cam_z;

	cam_x = MotionController(CTRL_COS, TIME_FREE);
	cam_x.set_sin_func(0.7, 250);
	cam_x.set_control_axis(CTRL_X);

	cam_z = MotionController(CTRL_SIN, TIME_FREE);
	cam_z.set_sin_func(0.9, 120);
	cam_z.set_control_axis(CTRL_Z);

	cam_y = MotionController(CTRL_SIN, TIME_FREE);
	cam_y.set_sin_func(0.5, 40);
	cam_y.set_control_axis(CTRL_Y);

#ifdef ANIM
	cam->add_controller(cam_x, CTRL_TRANSLATION);
	cam->add_controller(cam_z, CTRL_TRANSLATION);
	cam->add_controller(cam_y, CTRL_TRANSLATION);
#endif

	scene->set_background(0.5);

	//scene->render_sequence(0, 15000, 25);
	//exit(0);
}

SpacePart::~SpacePart() {
}

void SpacePart::draw_part() {
	float t = (float)time / 1000.0;

	ScenePart::draw_part();
	draw_design(t);
}

static void make_skycube(Scene *scene) {
	const float size = 1000;
	Object *face[6];
	Texture *tex[6];

	face[0] = new ObjPlane(Vector3(0, -1, 0), Vector2(size, size), 0);
	face[0]->translate(Vector3(0, size / 2, 0));
	tex[0] = get_texture("data/img/nebula_py.jpg");
	
	face[1] = new ObjPlane(Vector3(0, 1, 0), Vector2(size, size), 0);
	face[1]->translate(Vector3(0, -size / 2, 0));
	tex[1] = get_texture("data/img/nebula_ny.jpg");

	face[2] = new ObjPlane(Vector3(0, 0, -1), Vector2(size, size), 0);
	face[2]->translate(Vector3(0, 0, size / 2));
	tex[2] = get_texture("data/img/nebula_pz.jpg");
	
	face[3] = new ObjPlane(Vector3(0, 0, 1), Vector2(size, size), 0);
	face[3]->translate(Vector3(0, 0, -size / 2));
	tex[3] = get_texture("data/img/nebula_nz.jpg");

	face[4] = new ObjPlane(Vector3(-1, 0, 0), Vector2(size, size), 0);
	face[4]->translate(Vector3(size / 2, 0, 0));
	tex[4] = get_texture("data/img/nebula_px.jpg");
	
	face[5] = new ObjPlane(Vector3(1, 0, 0), Vector2(size, size), 0);
	face[5]->translate(Vector3(-size / 2, 0, 0));
	tex[5] = get_texture("data/img/nebula_nx.jpg");

	for(int i=0; i<6; i++) {
		Material *mat = face[i]->get_material_ptr();
		mat->emissive_color = 1.0;
		add_texture(tex[i]);
		mat->set_texture(tex[i], TEXTYPE_DIFFUSE);
		face[i]->set_texture_addressing(TEXADDR_CLAMP);
		scene->add_object(face[i]);
	}
}

static void make_particles(Scene *scene) {
	MotionController mc(CTRL_LIN, TIME_FREE);
	mc.set_slope(1);
	mc.set_control_axis(CTRL_Y);
	
	ParticleSystem *galaxy = new ParticleSystem("data/galaxy.psys");
	galaxy->add_controller(mc, CTRL_ROTATION);
	scene->add_particle_sys(galaxy);

	galaxy = new ParticleSystem("data/galaxy.psys");
	mc.set_origin(half_pi);
	galaxy->add_controller(mc, CTRL_ROTATION);
	scene->add_particle_sys(galaxy);

	ParticleSystem *stars = new ParticleSystem("data/stars.psys");
	scene->add_particle_sys(stars);
}

#define MIN(a, b)	((a) < (b) ? (a) : (b))

static void draw_design(float t) {
	static const float des_fill = 3.0;
	static const float yoffs = des_sz * 0.8;// + des_sz / 8.0;
	static const Vector2 szvec(des_sz / 2, des_sz / 2);
	static const float phase_offs = half_pi;
	static const float x = des_sz;
	static const float bar_len = 0.95;

	static const float leave_start = 19.0;
	static const float leave_dur = 4.0;
	
	float fill_t;
	if(t < leave_start) {
		fill_t = MIN(1.0, t / des_fill);
	} else {
		fill_t = 1.0 - (t - leave_start) / leave_dur;
	}

	set_zbuffering(false);
	set_lighting(false);
	set_alpha_blending(true);
	set_blend_func(BLEND_ONE_MINUS_DST_COLOR, BLEND_ZERO);

	for(int i=0; i<des_count; i++) {
		if((float)i / (float)des_count > fill_t) break;
		Vector2 pos(x + des_sz * (float)i, yoffs);
	
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslatef(pos.x, pos.y, 0);
		glRotatef(100.0 * cos(t * 3.0 + (float)i * phase_offs), 0, 0, 1);
		glTranslatef(-pos.x, -pos.y, 0);
		
		float sz = sin(t * 4.0 + (float)i * phase_offs) * 0.25 + 0.5;
		draw_scr_quad(pos - szvec * sz, pos + szvec * sz, 1.0, false);
		draw_scr_quad(pos - szvec * sz * 0.7, pos + szvec * sz * 0.7, 1.0, false);

		pos = Vector2(1.0, 1.0) - pos;
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslatef(pos.x, pos.y, 0);
		glRotatef(100.0 * cos(t * 3.0 + (float)i * phase_offs), 0, 0, 1);
		glTranslatef(-pos.x, -pos.y, 0);
		
		draw_scr_quad(pos - szvec * sz, pos + szvec * sz, 1.0, false);
		draw_scr_quad(pos - szvec * sz * 0.7, pos + szvec * sz * 0.7, 1.0, false);
	}

	float bar_rest = 1.0 - bar_len;
	float bar_start = bar_rest / 2.0;
	float bar_end = bar_start + bar_len * fill_t;
	draw_scr_quad(Vector2(bar_start, yoffs - 0.002), Vector2(bar_end, yoffs + 0.002));
	draw_scr_quad(Vector2(1.0 - bar_start, 1.0 - yoffs + 0.002), Vector2(1.0 - bar_end, 1.0 - yoffs - 0.002));
	
	set_alpha_blending(false);
	set_lighting(true);
	set_zbuffering(true);
}
