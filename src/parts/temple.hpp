#ifndef _TEMPLE_PART_HPP_
#define _TEMPLE_PART_HPP_

#include "3dengfx/3dengfx.hpp"
#include "dsys/demosys.hpp"

class TemplePart : public dsys::ScenePart {
private:
	virtual void draw_part();

public:
	TemplePart();
	virtual ~TemplePart();
};

#endif	// _TEMPLE_PART_HPP_
