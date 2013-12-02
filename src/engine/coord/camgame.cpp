#define GEN_IMPL
#include "camgame.h"

#include "../engine.h"
#include "../terrain.h"
#include "phys3.h"
#include "vec2.h"
#include "window.h"

namespace openage {
namespace engine {
namespace coord {

phys3 camgame::to_phys3(phys_t up) {
	return this->as_relative().to_phys3(up - camgame_phys.up) + camgame_phys;
}

phys3_delta camgame_delta::to_phys3(phys_t up) {
	//apply scaling factor, to get 'scaled'
	//scaling factor: w/2 for x, h/2 for y
	//plus the phys_per_tu fixed-point scaling factor
	vec2 scaled;
	scaled.x = (phys_per_tile * (phys_t) x) / (phys_t) tile_halfsize.x;
	scaled.y = (phys_per_tile * (phys_t) y) / (phys_t) tile_halfsize.y;

	//apply transformation matrix to 'scaled', to get the relative phys3 position
	//
	//a camgame position represents a line in the 3D phys space
	//a camgame delta of 0 might actually correlate to an arbitrarily
	//large phys delta.
	//we select one specific point on that line by explicitly specifying the
	//'up' value of the result.
	//
	//the transformation matrix is:
	//
	// (ne) = (+0.5 +0.5 +0.5) (scaled.x)
	// (se) = (+0.5 -0.5 -0.5) (scaled.y)
	// (up) = (+0.0 +0.0 +1.0) (      up)
	phys3_delta result;
	result.ne = (scaled.x + scaled.y + up)/2;
	result.se = (scaled.x - scaled.y - up)/2;
	result.up = (                      up);

	return result;
}

window camgame::to_window() {
	return camgame_window + this->as_relative().to_window();
}

window_delta camgame_delta::to_window() {
	//the direction of the y axis is flipped
	return window_delta {(pixel_t) x, (pixel_t) -y};
}

} //namespace coord
} //namespace engine
} //namespace openage