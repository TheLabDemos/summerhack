#include "pipes.hpp"

static void make_skycube(Scene *scene);
static void make_title(Scene *scene);
static void make_vol(Scene *scene);
static void render_vol(unsigned long time);
static void render_presents(float t);

#define PSYS_COUNT	4
static ParticleSystem *ps[PSYS_COUNT];
static ParticleSysParams *psys_params[PSYS_COUNT];
static bool psys_removed = false;

static const float inner_sz = 30.0;
static const float outer_sz = 70.0;
static const int sph_count = 50;
static Object *inner_sph, *vol_sph[sph_count];

static Texture *presents, *summer_hack;

#define TITLE
#define VOL
#define ANIM

PipesPart::PipesPart() : ScenePart("pipes", new Scene) {
#ifdef ANIM
	TargetCamera *cam = new TargetCamera(Vector3(0, 20, -150));
#else
	TargetCamera *cam = new TargetCamera(Vector3(0, 10, -150));
#endif
	cam->set_fov(DEG_TO_RAD(50));
	scene->add_camera(cam);

	PointLight *lt = new PointLight(Vector3(10, 100, -100));
	scene->add_light(lt);

	scene->set_ambient_light(0.1);

	make_skycube(scene);
#ifdef TITLE
	make_title(scene);
#endif
#ifdef VOL
	make_vol(scene);
#endif

	presents = get_texture("data/img/presents.png");
	summer_hack = get_texture("data/img/title.png");

	MotionController cam_x, cam_y, cam_z;

	cam_x = MotionController(CTRL_COS, TIME_FREE);
	cam_x.set_sin_func(0.5, 40);
	cam_x.set_control_axis(CTRL_X);

	cam_z = MotionController(CTRL_SIN, TIME_FREE);
	cam_z.set_sin_func(1.5, 100);
	cam_z.set_control_axis(CTRL_Z);

	cam_y = MotionController(CTRL_SIN, TIME_FREE);
	cam_y.set_sin_func(0.35, 20);
	cam_y.set_control_axis(CTRL_Y);

#ifdef ANIM
	cam->add_controller(cam_x, CTRL_TRANSLATION);
	//cam->add_controller(cam_z, CTRL_TRANSLATION);
	cam->add_controller(cam_y, CTRL_TRANSLATION);
#endif

	scene->set_background(0.5);

	// make sure the volume and title textures are resident from the beginning
	dsys::overlay(get_texture("data/img/cracks.jpg"), Vector2(0, 0), Vector2(0.1, 0.1), 0.0);
	dsys::overlay(get_texture("data/img/volcano.jpg"), Vector2(0, 0), Vector2(0.1, 0.1), 0.0);
	dsys::overlay(get_texture("data/img/title.png"), Vector2(0, 0), Vector2(0.1, 0.1), 0.0);
	dsys::overlay(get_texture("data/img/presents.png"), Vector2(0, 0), Vector2(0.1, 0.1), 0.0);

	//scene->render_sequence(0, 15000, 25);
	//exit(0);
}

PipesPart::~PipesPart() {
	//if(psys_removed) {
		for(int i=0; i<PSYS_COUNT; i++) {
			delete ps[i];
		}
	//}
}

static const float pexpl_time = half_pi * 8.0;
static const float pexpl_full_time = pexpl_time + half_pi * 3.4;
static const float pstop_time = pexpl_time + half_pi * 6.0;

void PipesPart::draw_part() {
	float t = (float)time / 1000.0f;
	
	ScenePart::draw_part();
#ifdef VOL
	render_vol(time);
#endif

#ifdef TITLE
	if(t < pstop_time) {
		float p = t / 4.0;
		if(t > pexpl_time) {
			static unsigned long prev_drop;
			bool drop = (time - prev_drop > 33.3333) ? true : false;
			
			//Fuzzy val(0.0, 1.75 * (1.0 - sin(fmod(p, half_pi) + half_pi)));
			Fuzzy val(0.0, 1.75 * sin(fmod(p, half_pi)));
			for(int i=0; i<PSYS_COUNT; i++) {
				psys_params[i]->shoot_dir = FuzzyVec3(val, val, val);

				if(t > pexpl_full_time && drop) {
					psys_params[i]->birth_rate.num *= 0.8;
				}
			}
		}

		p = t / 15.0;
		for(int i=0; i<PSYS_COUNT; i++) {
			float val = p > 0.5 ? 0.5 : p;
			float range = val * 2.0;
			psys_params[i]->spawn_offset_curve_area = Fuzzy(val, range);

			ps[i]->update();
			ps[i]->draw();
		}
	}/* else {
		if(!psys_removed) {
			//for(int i=0; i<PSYS_COUNT; i++) {
			//	scene->remove_particle_sys(ps[i]);
			//}
			psys_removed = true;
		}
	}*/
#endif	// TITLE

	render_presents(t);
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
		face[i]->apply_xform();
		face[i]->set_rotation(Vector3(0, quarter_pi, 0));
	
		Material *mat = face[i]->get_material_ptr();
		mat->emissive_color = 1.0;
		mat->set_texture(tex[i], TEXTYPE_DIFFUSE);
		face[i]->set_texture_addressing(TEXADDR_CLAMP);
		scene->add_object(face[i]);
	}
}


void make_title(Scene *scene) {

	for(int i=0; i<PSYS_COUNT; i++) {
		char curve_fname[64];
		sprintf(curve_fname, "data/curve%02d", i);
		
		Curve *curve = load_curve(curve_fname);
		curve->set_arc_parametrization(true);
		ps[i] = new ParticleSystem("data/thelab.psys");
		psys_params[i] = ps[i]->get_params();
		psys_params[i]->spawn_offset_curve = curve;
		if(i == 2) {
			psys_params[i]->birth_rate.num /= 1.5;
		}
		if(i == 1) {
			psys_params[i]->birth_rate.num /= 6.0;
		}
		if(i == 3) {
			psys_params[i]->birth_rate.num *= 1.3;
		}
		//scene->add_particle_sys(ps[i]);
	}
}

static float vol_alpha[sph_count];

static void make_vol(Scene *scene) {
	Material *mat;

	MotionController rot1(CTRL_LIN, TIME_FREE);
	MotionController rot2(CTRL_SIN, TIME_FREE);
	
	rot1.set_control_axis(CTRL_XY);
	rot1.set_slope(1.08);
	rot2.set_control_axis(CTRL_YZ);
	rot2.set_slope(1.439999);
	rot2.set_sin_func(1.5, 1.5);

	inner_sph = new ObjSphere(inner_sz, 4);
	mat = inner_sph->get_material_ptr();
	mat->set_texture(get_texture("data/img/volcano.jpg"), TEXTYPE_DIFFUSE);
	inner_sph->add_controller(rot1, CTRL_ROTATION);
	inner_sph->add_controller(rot2, CTRL_ROTATION);
	//scene->add_object(obj);

	for(int i=0; i<sph_count; i++) {
		float t = (float)i / (float)sph_count;
		vol_sph[i] = new ObjSphere(inner_sz + (outer_sz - inner_sz) * t, 3);
		mat = vol_sph[i]->get_material_ptr();
		mat->set_texture(get_texture("data/img/cracks.jpg"), TEXTYPE_DIFFUSE);
		mat->alpha = vol_alpha[i] = 1.0 - t;
		mat->emissive_color = 1.0;
		vol_sph[i]->set_handle_blending(false);
		vol_sph[i]->set_blending_mode(BLEND_SRC_ALPHA, BLEND_ONE);
		vol_sph[i]->set_blending(true);
		vol_sph[i]->add_controller(rot1, CTRL_ROTATION);
		vol_sph[i]->add_controller(rot2, CTRL_ROTATION);
		vol_sph[i]->set_zwrite(false);
		//scene->add_object(obj);
	}
}

static void render_vol(unsigned long time) {
	static const float leave_time = 34.0;
	
	float t = (float)time / 1000.0;
	if(t < pexpl_full_time) return;

	float sp = (t - pexpl_full_time) / (pstop_time - pexpl_full_time);
	if(t > pstop_time) sp = 1.0;

	// update
	inner_sph->get_material_ptr()->alpha = sp;
	for(int i=0; i<sph_count + 1; i++) {
		Object *obj = i < sph_count ? vol_sph[i] : inner_sph;

		obj->set_scaling(Vector3(sp, sp, sp));
		if(i < sph_count) obj->get_material_ptr()->alpha = vol_alpha[i] * sp;

		if(t > leave_time) {
			float lt = t - leave_time;
			obj->set_position(Vector3(0, pow(lt * 5.0, 2.0), 0));
		}
	}
	
	// render
	set_front_face(ORDER_CCW);
	set_zbuffering(false);
	for(int i=0; i<sph_count; i++) {
		vol_sph[i]->render(time);
	}
	set_zbuffering(true);
	set_front_face(ORDER_CW);

	inner_sph->render(time);

	set_zbuffering(false);
	for(int i=0; i<sph_count; i++) {
		vol_sph[i]->render(time);
	}
	set_zbuffering(true);
}

static void render_presents(float t) {
	static const float pres_start = pexpl_full_time - 0.5;
	static const Vector2 pres_sz(0.35, 0.12);
	static const Vector2 hack_sz(0.55, 0.12);
	
	t -= pres_start;

	if(t < 0.0 || t > pres_start + 5.0) return;

	float x = pow((t * 0.2) - 1.0, 3.0);
	Vector2 pres_pos(-x + 0.5, 0.3);
	Vector2 hack_pos(x + 0.5, 0.7);
	dsys::overlay(presents, pres_pos - pres_sz / 2.0, pres_pos + pres_sz / 2.0, 1.0);
	dsys::overlay(summer_hack, hack_pos - hack_sz / 2.0, hack_pos + hack_sz / 2.0, 1.0);
}
