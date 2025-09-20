#ifndef HELPERS_HPP_DD37133A_D9CE_4998_AA48_41DA09E1517C
#define HELPERS_HPP_DD37133A_D9CE_4998_AA48_41DA09E1517C

#include "../draw2d/forward.hpp"
#include "../draw2d/draw.hpp"


ColorU8_sRGB find_most_red_pixel( Surface const& );
ColorU8_sRGB find_least_red_nonzero_pixel( Surface const& );

// New custom functions
Vec2f find_most_red_pixel_loc(Surface const&);

#endif // HELPERS_HPP_DD37133A_D9CE_4998_AA48_41DA09E1517C
