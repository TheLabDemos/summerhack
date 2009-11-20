#include <iostream>
#include "dist.hpp"

using namespace std;

static Vertex *varray;
static Index *iarray;
static TexCoord *tc;

static const int subd = 50;
static const int vsz = subd + 2;
static int vcount, icount;

DistFx::DistFx() : Part("distort") {
	set_clear(false);

	TriMesh mesh;
	create_plane(&mesh, Vector3(0, 0, -1), Vector2(1.0, 1.0), subd);

	vcount = mesh.get_vertex_array()->get_count();
	icount = mesh.get_index_array()->get_count();

	varray = new Vertex[vcount];
	Vertex *vptr = varray;
	for(int i=0; i<vcount; i++) {
		*vptr = mesh.get_vertex_array()->get_data()[i];
		vptr->pos += Vector3(0.5, 0.5);
		vptr++;
	}

	iarray = new Index[icount];
	Index *iptr = iarray;
	for(int i=0; i<icount; i++) {
		*iptr++ = mesh.get_index_array()->get_data()[i];
	}

	tc = new TexCoord[vcount];
	TexCoord *tptr = tc;
	for(int y=0; y<vsz; y++) {
		for(int x=0; x<vsz; x++) {
			*tptr++ = TexCoord((float)x / (float)(vsz - 1), 1.0 - (float)y / (float)(vsz - 1));
		}
	}
	
}

DistFx::~DistFx() {
	delete [] varray;
	delete [] iarray;
}

#define MIN(a, b)	((a) < (b) ? (a) : (b))
#define MAX(a, b)	((a) > (b) ? (a) : (b))
#define CLAMP(x, a, b)	MIN(MAX(x, a), b)

void DistFx::draw_part() {
	// update
	float t = (float)time / 1000.0;
	float cost = cos(t * 2.0);
	float sint = sin(t * 2.0);
	
	Vertex *vptr = varray;
	for(int y=0; y<vsz; y++) {
		for(int x=0; x<vsz; x++) {
			float u = (float)x / (float)(vsz - 1);
			float v = (1.0 - (float)y / (float)(vsz - 1));

			float pu = u * 4.0;
			float pv = v * 4.0;

			float du = sin(pu + cost) * 10.0 +
						cos((pu + sint) * 2.0) * 5.0 +
						sin((pu + cost) * 3.0) * 3.33 + 
						cos((pv + sint) * 2.0) * 5.0 +
						sin(pv + cost) * 10.0;

			float dv = cos(pv + sint) * 10.0 +
						sin((pv + cost) * 2.0) * 5.0 +
						cos((pu + sint) * 3.0) * 3.33 +
						sin((pv + cost) * 2.0) * 6.0;

			du *= sin(t / 2.0) * 0.003 + 0.003;
			dv *= sin(t / 2.0) * 0.003 + 0.003;

			vptr->tex[0].u = CLAMP(u + du, 0.0, 1.0);
			vptr->tex[0].v = CLAMP(v + dv, 0.0, 1.0);
			vptr++;
		}
	}
	
	// draw
	set_matrix(XFORM_TEXTURE, Matrix4x4::identity_matrix);
	load_xform_matrices();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, 1.0, 0.0, 1.0, 0.0, 1.0);

	set_lighting(false);
	set_zbuffering(false);
	set_backface_culling(false);

	set_texture(0, dsys::tex[0]);
	enable_texture_unit(0);

	glBegin(GL_TRIANGLES);
	glColor4f(1.0, 1.0, 1.0, 0.5);
	for(int i=0; i<icount; i++) {
		Vertex *vptr = &varray[iarray[i]];
		glTexCoord2f(tc[iarray[i]].u, tc[iarray[i]].v);
		glVertex3f(vptr->pos.x, vptr->pos.y, vptr->pos.z);
	}
	glEnd();

	set_alpha_blending(true);
	set_blend_func(BLEND_SRC_ALPHA, BLEND_ONE_MINUS_SRC_ALPHA);

	glBegin(GL_TRIANGLES);
	glColor4f(1.0, 1.0, 1.0, 0.7);
	for(int i=0; i<icount; i++) {
		Vertex *vptr = &varray[iarray[i]];
		glTexCoord2f(vptr->tex[0].u, vptr->tex[0].v);
		glVertex3f(vptr->pos.x, vptr->pos.y, vptr->pos.z);
	}
	glEnd();

	set_alpha_blending(false);

	disable_texture_unit(0);
	
	set_lighting(true);
	set_zbuffering(true);
	set_backface_culling(true);
}
