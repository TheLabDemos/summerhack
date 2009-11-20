#ifndef _PIPES_PART_HPP_
#define _PIPES_PART_HPP_

#include "3dengfx/3dengfx.hpp"
#include "dsys/demosys.hpp"

class PipesPart : public dsys::ScenePart {
private:
	virtual void draw_part();

public:
	PipesPart();
	virtual ~PipesPart();
};

#endif	// _PIPES_PART_HPP_
