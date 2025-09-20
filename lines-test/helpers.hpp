#ifndef HELPERS_HPP_D7CD1385_1BA6_4B6B_9F31_72B9B8A05AA7
#define HELPERS_HPP_D7CD1385_1BA6_4B6B_9F31_72B9B8A05AA7

#include <array>

#include <cstddef>

#include "../draw2d/forward.hpp"
#include "../draw2d/draw.hpp"

std::size_t max_row_pixel_count( Surface const& );
std::size_t max_col_pixel_count( Surface const& );

std::array<std::size_t,9> count_pixel_neighbours( Surface const& );

// New Custom functions for tests
bool is_point_inside_surface(Surface& surface, Vec2f point);

int compute_outcode(int x, int y);


#endif // HELPERS_HPP_D7CD1385_1BA6_4B6B_9F31_72B9B8A05AA7
