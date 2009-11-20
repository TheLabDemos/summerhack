#ifndef _DISTORTION_FX_HPP_
#define _DISTORTION_FX_HPP_

#include "3dengfx/3dengfx.hpp"
#include "dsys/demosys.hpp"

class DistFx : public dsys::Part {
private:
	virtual void draw_part();

public:
	DistFx();
	virtual ~DistFx();
};

#endif	// _DISTORTION_FX_HPP_
