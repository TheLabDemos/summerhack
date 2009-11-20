#include <cassert>
#include "temple.hpp"
#include "common/err_msg.h"

static void make_skycube(Scene *scene);
static void make_temple(Scene *scene);
static bool make_pillars(Scene *scene);
static void make_blobs(Scene *scene);

static void move_blobs(scalar_t t);
static scalar_t eval_func(const Vector3 &vec, scalar_t t);
static Vector3 eval_normals(const Vector3 &vec, scalar_t t);

static const int pillar_udiv = 16;

ParticleSysParams *pp[4];

Texture *greets;

#define BLOB_COUNT	4
static scalar_t bint[BLOB_COUNT] = {30, 28, 24, 15};
static Vector3 pos[BLOB_COUNT];
static ScalarField *sfield;
static Object *blob;

#define BLOBS
#define TEMPLE
#define EXIBITS

#define ANIM

TemplePart::TemplePart() : ScenePart("temple", new Scene) {
#ifdef ANIM
	TargetCamera *cam = new TargetCamera(Vector3(0, 52, 0), Vector3(0, 50, 0));
#else
	TargetCamera *cam = new TargetCamera(Vector3(0, 60, -160), Vector3(0, 50, 0));
#endif
	cam->set_fov(DEG_TO_RAD(50));
	scene->add_camera(cam);

	PointLight *lt;
	
	lt = new PointLight(Vector3(-200, 250, -400));
	lt->set_intensity(0.9);
	scene->add_light(lt);

	lt = new PointLight(Vector3(200, 20, 0));
	lt->set_intensity(0.6);
	scene->add_light(lt);

	lt = new PointLight(Vector3(20, 500, -400));
	lt->set_intensity(0.5);
	scene->add_light(lt);

	scene->set_ambient_light(0.1);

	make_skycube(scene);
#ifdef TEMPLE
	make_temple(scene);
#endif
#ifdef EXIBITS
	make_pillars(scene);
#endif
#ifdef BLOBS
	make_blobs(scene);
#endif
	
	MotionController ctrl;

#ifdef ANIM
	ctrl = MotionController(CTRL_COS, TIME_FREE);
	ctrl.set_sin_func(0.58, 250);
	ctrl.set_control_axis(CTRL_X);
	cam->add_controller(ctrl, CTRL_TRANSLATION);

	ctrl = MotionController(CTRL_SIN, TIME_FREE);
	ctrl.set_sin_func(1.17, 125);
	ctrl.set_control_axis(CTRL_X);
	cam->add_controller(ctrl, CTRL_TRANSLATION);

	ctrl = MotionController(CTRL_SIN, TIME_FREE);
	ctrl.set_sin_func(0.49, 220);
	ctrl.set_control_axis(CTRL_Z);
	cam->add_controller(ctrl, CTRL_TRANSLATION);

	ctrl = MotionController(CTRL_COS, TIME_FREE);
	ctrl.set_sin_func(0.99, 105);
	ctrl.set_control_axis(CTRL_Z);
	cam->add_controller(ctrl, CTRL_TRANSLATION);

	ctrl = MotionController(CTRL_SIN, TIME_FREE);
	ctrl.set_sin_func(0.45, 50);
	ctrl.set_control_axis(CTRL_Y);
	cam->add_controller(ctrl, CTRL_TRANSLATION);
#endif

	greets = get_texture("data/img/greets.png");

	scene->set_background(0.5);

	// in order to precalculate the cubemaps...
	scene->render(0);

	//scene->render_sequence(0, 15000, 25);
	//exit(0);
}

TemplePart::~TemplePart() {}

static void make_skycube(Scene *scene) {
	const float size = 5000;
	Object *face[6];
	Texture *tex[6];

	face[0] = new ObjPlane(Vector3(0, -1, 0), Vector2(size, size), 0);
	face[0]->translate(Vector3(0, size / 2, 0));
	tex[0] = get_texture("data/img/py.jpg");
	
	face[1] = new ObjPlane(Vector3(0, 1, 0), Vector2(size, size), 0);
	face[1]->translate(Vector3(0, -size / 2, 0));
	tex[1] = get_texture("data/img/ny.jpg");

	face[2] = new ObjPlane(Vector3(0, 0, -1), Vector2(size, size), 0);
	face[2]->translate(Vector3(0, 0, size / 2));
	tex[2] = get_texture("data/img/pz.jpg");
	
	face[3] = new ObjPlane(Vector3(0, 0, 1), Vector2(size, size), 0);
	face[3]->translate(Vector3(0, 0, -size / 2));
	tex[3] = get_texture("data/img/nz.jpg");

	face[4] = new ObjPlane(Vector3(-1, 0, 0), Vector2(size, size), 0);
	face[4]->translate(Vector3(size / 2, 0, 0));
	tex[4] = get_texture("data/img/px.jpg");
	
	face[5] = new ObjPlane(Vector3(1, 0, 0), Vector2(size, size), 0);
	face[5]->translate(Vector3(-size / 2, 0, 0));
	tex[5] = get_texture("data/img/nx.jpg");

	for(int i=0; i<6; i++) {
		Material *mat = face[i]->get_material_ptr();
		mat->emissive_color = 1.0;
		mat->set_texture(tex[i], TEXTYPE_DIFFUSE);
		face[i]->set_texture_addressing(TEXADDR_CLAMP);
		scene->add_object(face[i]);
	}
}

void make_temple(Scene *scene) {
	const Vector3 pillar_body[] = {
		Vector3(8, 4, 0),
		Vector3(12, 40, 0),
		Vector3(11, 70, 0),
		Vector3(4, 106, 0)
	};

	const Vector3 pillar_pos[] = {
		Vector3(-50, 0, -50),
		Vector3(-50, 0, 50),
		Vector3(50, 0, 50),
		Vector3(50, 0, -50)
	};


	TriMesh pillar_mesh;
	create_revolution(&pillar_mesh, pillar_body, 4, pillar_udiv, 10);

	Material pillar_mat;
	pillar_mat.specular_color = 1.0;
	pillar_mat.specular_power = 70.0;
	pillar_mat.set_texture(get_texture("data/img/benedeti.jpg"), TEXTYPE_DIFFUSE);

	for(int i=0; i<4; i++) {
		Object *obj = new Object;
		obj->get_mesh() = pillar_mesh;
		obj->set_position(pillar_pos[i]);
		*obj->get_material_ptr() = pillar_mat;
		scene->add_object(obj);

		obj = new ObjCylinder(13.0, 5.0, true, pillar_udiv);
		obj->set_position(pillar_pos[i] + Vector3(0, 6, 0));
		*obj->get_material_ptr() = pillar_mat;
		scene->add_object(obj);
		
		obj = new ObjCylinder(16.0, 5.0, true, pillar_udiv);
		obj->set_position(pillar_pos[i] + Vector3(0, 2, 0));
		*obj->get_material_ptr() = pillar_mat;
		scene->add_object(obj);

		obj = new ObjTorus(2.0, 5.0);
		obj->set_position(pillar_pos[i] + Vector3(0, 102, 0));
		*obj->get_material_ptr() = pillar_mat;
		scene->add_object(obj);

		obj = new ObjTorus(2.0, 7);
		obj->set_position(pillar_pos[i] + Vector3(0, 105, 0));
		*obj->get_material_ptr() = pillar_mat;
		scene->add_object(obj);
	}


	// floor
	Object *obj = new ObjCylinder(95.0, 5, true, 25);
	obj->set_position(Vector3(0, -2.5, 0));
	obj->get_material_ptr()->set_texture(get_texture("data/img/brick1_base1.jpg"), TEXTYPE_DIFFUSE);
	scene->add_object(obj);

	obj = new ObjCylinder(103.0, 5, true, 25);
	obj->set_position(Vector3(0, -7.5, 0));
	obj->get_material_ptr()->set_texture(get_texture("data/img/brick1_base1.jpg"), TEXTYPE_DIFFUSE);
	scene->add_object(obj);

	// ceiling
	const Vector3 ceil[] = {
		Vector3(90, 0, 0),
		Vector3(92, 4, 0),
		Vector3(98, 14, 0),
		Vector3(95, 20, 0),
		Vector3(60, 40, 0),
		Vector3(20, 60, 0),
		Vector3(18, 64, 0),
		Vector3(23, 66, 0),
		Vector3(26, 67, 0),
		Vector3(25, 70, 0),
		Vector3(10, 78, 0),
		Vector3(4, 84, 0),
		Vector3(2, 90, 0),
		Vector3(0.1, 100, 0)
	};

	obj = new Object;
	create_revolution(obj->get_mesh_ptr(), ceil, 14, 24, 28);
	obj->set_pivot(Vector3(0, 40, 0));
	obj->set_position(Vector3(0, 110, 0));
	obj->calculate_normals();

	Texture *cubemap = new Texture(64, 64, TEX_CUBE);
	add_texture(cubemap);

	Material *mat = obj->get_material_ptr();
	mat->diffuse_color = mat->ambient_color = int_color(239, 190, 37) * 0.8;
	mat->specular_color = int_color(255, 198, 43);
	mat->specular_power = 40.0;
	mat->set_texture(get_texture("data/img/brick1_base1.jpg"), TEXTYPE_DIFFUSE);
	mat->set_texture(cubemap, TEXTYPE_ENVMAP);
	mat->auto_refl = false;
	mat->env_intensity = 0.3;
	scene->add_object(obj);

	obj = new ObjCylinder(94, 5, true, 25);
	obj->set_position(Vector3(0, 107.5, 0));
	*obj->get_material_ptr() = *mat;
	scene->add_object(obj);

	// sting
	const Vector3 sting[] = {
		Vector3(12, 0, 0),
		Vector3(9, 4, 0),
		Vector3(5, 6, 0),
		Vector3(2, 10, 0),
		Vector3(0.01, 34, 0)
	};

	cubemap = new Texture(16, 16, TEX_CUBE);
	add_texture(cubemap);

	obj = new Object;
	create_revolution(obj->get_mesh_ptr(), sting, 5, 10, 10);
	obj->set_pivot(Vector3(0, 5, 0));
	mat = obj->get_material_ptr();
	mat->diffuse_color = mat->ambient_color = 0.1;
	mat->specular_color = 1.0;
	mat->specular_power = 80.0;
	mat->set_texture(cubemap, TEXTYPE_ENVMAP);
	mat->auto_refl = false;
	scene->add_object(obj);


	// floor-floor
	obj = new ObjPlane(Vector3(0, 1, 0), Vector2(1200, 1200), 6);
	obj->set_position(Vector3(0, -10, 0));
	mat = obj->get_material_ptr();
	mat->set_texture(get_texture("data/img/mud.jpg"), TEXTYPE_DIFFUSE);
	mat->tmat[TEXTYPE_DIFFUSE].set_scaling(Vector3(4.0, 4.0, 4.0));
	scene->add_object(obj);
}

static bool make_pillars(Scene *scene) {
	static const Vector3 ppos[] = {
		Vector3(-150, -7.5, 0),
		Vector3(0, -7.5, 150),
		Vector3(150, -7.5, 0),
		Vector3(0, -7.5, -150)
	};

	static const char *label_tex[] = {
		"data/img/label_dolphin.png",
		"data/img/label_duck.png",
		"data/img/label_face.png",
		"data/img/label_teapot.png",
		0
	};
	
	Curve *curve = load_curve("data/pillar.curve");

	for(int i=0; i<4; i++) {
		char name[128];
		sprintf(name, "pillar%02d", i);
		
		Object *obj = new Object;
		obj->name = name;
		Material *mat = obj->get_material_ptr();
		create_revolution(obj->get_mesh_ptr(), *curve, pillar_udiv, 10);
		obj->set_scaling(Vector3(2.3, 1.8, 2.3));
		obj->apply_xform();
		obj->normalize_normals();
		obj->set_position(ppos[i]);

		mat->specular_color = 1.0;
		mat->specular_power = 70.0;
		mat->set_texture(get_texture("data/img/benedeti.jpg"), TEXTYPE_DIFFUSE);
		scene->add_object(obj);

		obj = new ObjCylinder(14.0, 3.0);
		obj->set_position(ppos[i]);
		mat = obj->get_material_ptr();
		mat->specular_color = 1.0;
		mat->specular_power = 70.0;
		mat->set_texture(get_texture("data/img/benedeti.jpg"), TEXTYPE_DIFFUSE);
		scene->add_object(obj);


		Texture *cubemap = new Texture(64, 64, TEX_CUBE);
		add_texture(cubemap);
		
		// put exibit
		switch(i) {
		case 1:
			{
				Scene *scene = load_scene("data/geom/duck.3ds");
				if(!scene) {
					error("failed to load duck.3ds");
					return false;
				}
				obj = scene->get_object("Object03");
				assert(obj);
				scene->remove_object(obj);
				delete scene;

				obj->set_scaling(Vector3(0.009, 0.009, 0.009));
				obj->set_auto_normalize(true);
			
				obj->set_position(ppos[i] + Vector3(0, 50, 0));
				obj->set_rotation(Vector3(0, quarter_pi, 0));
			}
			break;

		case 2:
			{
				Scene *scene = load_scene("data/geom/chrmface.3ds");
				if(!scene) {
					error("failed to load chrmface.3ds");
					return false;
				}
				obj = scene->get_object("0main01");
				assert(obj);
				scene->remove_object(obj);
				delete scene;

				obj->set_scaling(Vector3(0.2, 0.2, 0.2));
				obj->set_auto_normalize(true);

				obj->set_position(ppos[i] + Vector3(4, 58, 0));				
				obj->set_rotation(Vector3(0, half_pi, 0));

				obj->get_material_ptr()->two_sided = true;
			}
			break;

		case 0:
			{
				Scene *scene = load_scene("data/geom/dolphin.3ds");
				if(!scene) {
					error("failed to load dolphin.3ds");
					return false;
				}
				obj = scene->get_object("Loft01");
				assert(obj);
				scene->remove_object(obj);
				delete scene;

				obj->set_scaling(Vector3(0.14, 0.14, 0.14));
				obj->set_auto_normalize(true);

				obj->set_position(ppos[i] + Vector3(0, 48, 0));
				obj->set_rotation(Vector3(half_pi, half_pi, 0));
			}
			break;
		
		case 3:
		default:
			obj = new ObjTeapot(6.0);
			obj->set_position(ppos[i] + Vector3(0, 43.7, 0));
			break;
		}

		mat = obj->get_material_ptr();
		mat->diffuse_color = mat->ambient_color = 0.1;
		mat->specular_color = 1.0;
		mat->specular_power = 80.0;
		mat->set_texture(cubemap, TEXTYPE_ENVMAP);
		mat->auto_refl = false;
		
		MotionController ctrl(CTRL_SIN, TIME_FREE);
		ctrl.set_control_axis(CTRL_Y);
		ctrl.set_sin_func(frand(0.5) + 2.3, 3.0, frand(pi));
		obj->add_controller(ctrl, CTRL_TRANSLATION);
		obj->translate(Vector3(0, 3.0, 0));
		scene->add_object(obj);

		ParticleSystem *label = new ParticleSystem("data/exibit.psys");
		pp[i] = label->get_params();
		pp[i]->halo = get_texture(label_tex[i]);
		label->set_position(ppos[i] + Vector3(0, 80, 0));
		scene->add_particle_sys(label);

		ParticleSystem *trail = new ParticleSystem("data/test2.psys");
		trail->set_position(ppos[i] + Vector3(0, 50, 0));

		ctrl = MotionController(CTRL_SIN, TIME_FREE);
		ctrl.set_control_axis(CTRL_X);
		ctrl.set_sin_func(5.0, 30.0, (float)i * half_pi);
		trail->add_controller(ctrl, CTRL_TRANSLATION);

		ctrl = MotionController(CTRL_COS, TIME_FREE);
		ctrl.set_control_axis(CTRL_Z);
		ctrl.set_sin_func(5.0, 30.0, (float)i * half_pi);
		trail->add_controller(ctrl, CTRL_TRANSLATION);

		ctrl = MotionController(CTRL_SIN, TIME_FREE);
		ctrl.set_control_axis(CTRL_Y);
		ctrl.set_sin_func(1.8, 25.0, (float)i * half_pi);
		trail->add_controller(ctrl, CTRL_TRANSLATION);
		
		scene->add_particle_sys(trail);
	}

	delete curve;
	return true;
}

#define MIN(a, b)	((a) < (b) ? (a) : (b))
#define MAX(a, b)	((a) > (b) ? (a) : (b))

void TemplePart::draw_part() {
	static const float greets_time = 18.0;
	float t = (float)time / 1000.0f;

#ifdef EXIBITS
	float yoffs = MIN(150.0, t * 25.0);
	for(int i=0; i<4; i++) {
		pp[i]->spawn_offset = FuzzyVec3(0.0, yoffs, 0.0);
	}
#endif

#ifdef BLOBS
	move_blobs(t);
	sfield->triangulate(blob->get_mesh_ptr(), 1.0, t, true);
#endif

	scene->render(time);

	if(t > greets_time) {
		t *= 1.8;

		float a2 = sin(t);
    	a2 = a2 > 0 ? -a2 : a2;
    	a2 += 1.0f;
    	a2 *= frand(0.8) + 0.2;

    	float a1 = a2;//(cos(t)+1)/2;
    	a1 *= (sin(t / 2.0) > 0) ? 1.0 : 0.0;     // modulate with step func
    	a1 *= sin(t / 2.0);

		Color col(1.0, 1.0, 1.0, MAX(a2, a1) + 0.4);
	
		set_matrix(XFORM_TEXTURE, Matrix4x4::identity_matrix);
		load_xform_matrices();

		for(int i=0; i<10; i++) {
			set_alpha_blending(true);
			set_blend_func(BLEND_SRC_ALPHA, BLEND_ONE);
			float off = (float)i * 0.0025;
			col.a *= 0.8;
			dsys::overlay(greets, Vector2(-off, -off), Vector2(1 + off, 1 + off), col, 0, false);
			set_alpha_blending(false);
		}
	}
}

static void move_blobs(scalar_t t) {
	Vector3 offs = Vector3(0, 2.75 * cos(t*1.5) + 2.25, 0);
	pos[0] = offs + Vector3(sin(t), cos(t), 0) * 8.0;
	pos[1] = offs + Vector3(sin(t*2) + cos(t*4)/2, sin(t*1.5), cos(t)) * 8.0;
	pos[2] = offs + Vector3(cos(t*1.8)/2 + sin(t)/4, cos(t*2) + sin(t), sin(t*1.3)) * 8.0;
	pos[3] = offs + Vector3(sin(t*1.2)/3 + cos(t*1.8)/3.5, sin(cos(t*2) + t)*1.5, cos(t*2.0)/2) * 7.0;

	//cyl = Vector2(cos(t), sin(t*2.0)) * 8.0;
}

static scalar_t eval_func(const Vector3 &vec, scalar_t t) {
	scalar_t val = 0.0;
	for(int i=0; i<BLOB_COUNT; i++) {
		val += (bint[i] * 0.75) / (vec - pos[i]).length_sq();
	}

	return val;
}

//#define NORMAL_GRAD
static Vector3 eval_normals(const Vector3 &vec, scalar_t t) {
#ifdef NORMAL_GRAD
	const scalar_t diff = 0.1;

	Vector3 grad;
	grad.x = eval_func(vec + Vector3(diff, 0, 0), t) - eval_func(vec + Vector3(-diff, 0, 0), t);
	grad.y = eval_func(vec + Vector3(0, diff, 0), t) - eval_func(vec + Vector3(0, -diff, 0), t);
	grad.z = eval_func(vec + Vector3(0, 0, diff), t) - eval_func(vec + Vector3(0, 0, -diff), t);

	return -grad.normalized();
#else
	Vector3 normal(0, 0, 0);
	for(int i=0; i<BLOB_COUNT; i++) {
		scalar_t len_sq = (vec - pos[i]).length_sq();
		scalar_t len_quad = len_sq * len_sq;
		normal += (vec - pos[i]) * 2.0 * bint[i] / len_quad;
	}

	return normal.normalized();
#endif
}


static void make_blobs(Scene *scene) {
	// scalar field
	const int grid = 23;
	const scalar_t field_size = 40.0;
	const scalar_t hsz = field_size / 2.0;

	sfield = new ScalarField(grid, Vector3(-hsz, -hsz, -hsz), Vector3(hsz, hsz, hsz));
	sfield->set_evaluator(eval_func);
	sfield->set_normal_evaluator(eval_normals);
	
	// rendering details
	Texture *cubemap = new Texture(64, 64, TEX_CUBE);
	add_texture(cubemap);

	blob = new Object;
	blob->name = "blob";
	blob->set_dynamic(true);
	blob->set_position(Vector3(0, 50, 0));
	blob->set_scaling(Vector3(2.8, 2.6, 2.8));
	blob->set_auto_normalize(true);

	Material *mat = blob->get_material_ptr();
	mat->ambient_color = mat->diffuse_color = Color(0.7, 0.4, 0.2) * 0.4;
	mat->specular_color = Color(0.9, 0.7, 0.6);
	mat->specular_power = 60.0;
	mat->set_texture(cubemap, TEXTYPE_ENVMAP);
	mat->env_intensity = 0.6;
	mat->auto_refl = false;
	mat->two_sided = true;

	scene->add_object(blob);
}
