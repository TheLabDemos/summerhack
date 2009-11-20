#ifndef _SPACE_PART_HPP_
#define _SPACE_PART_HPP_

#include "3dengfx/3dengfx.hpp"
#include "dsys/demosys.hpp"

class SpacePart : public dsys::ScenePart {
private:
	virtual void draw_part();

public:
	SpacePart();
	virtual ~SpacePart();
};

#endif	// _SPACE_PART_HPP_
