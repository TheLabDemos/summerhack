#ifndef _PARTS_HPP_
#define _PARTS_HPP_

#include "pipes.hpp"
#include "space.hpp"
#include "temple.hpp"
#include "dist.hpp"

#define INIT_PARTS()	{\
	parts.push_back(new PipesPart());\
	parts.push_back(new SpacePart());\
	parts.push_back(new TemplePart());\
	parts.push_back(new DistFx());\
}

#endif	// _PARTS_HPP_
