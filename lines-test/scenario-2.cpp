#include <catch2/catch_amalgamated.hpp>

//TODO
#include <algorithm>

#include "helpers.hpp"

#include "../draw2d/surface.hpp"
#include "../draw2d/draw.hpp"

TEST_CASE("1 point out of bounds", "[1_OOB]")
{
	Surface surface(100, 100);
	surface.clear();

	ColorU8_sRGB standard_color = { 255, 255, 255 };
	Vec2f standard_point = { 54, 47 };

	SECTION("1 point generic tests")
	{
		// Create a list of Vec2f to use for marking outside bounds
		// Testing all 8 outer regions
		std::vector<Vec2f> outside_vecs{
			{-100, -100}, {30, -100}, {230, -100},   // Below
			{-100, 30}, {250, 90},                  // Left/Right
			{-100, 250}, {30, 200}, {230, 500}      // Above
		};

		// Draw combinations of vecs 
		for (auto& vec : outside_vecs)
		{
			draw_line_solid(surface, standard_point, vec, standard_color);

			// Using gradients to set checks
			float dx = standard_point.x - vec.x;
			float dy = standard_point.y - vec.y;

			float absolute_gradient = (dx != 0) ? abs(dy / dx) : std::numeric_limits<float>::infinity(); 

			// Checking for correct boundary condition
			if (absolute_gradient >= 1) { REQUIRE(max_row_pixel_count(surface) == 1); }
			if (absolute_gradient <= 1) { REQUIRE(max_col_pixel_count(surface) == 1); }
			
			surface.clear();
		}
	}

	SECTION("Edge cases for boundary conditions")
	{
		// Add specific test cases for boundary points
		std::vector<std::pair<Vec2f, Vec2f>> edge_cases = {
			// Testing point edge cases inside sufrace
			{{0, 0}, {50, 150}},      // Start at (0, 0), end outside
			{{99, 99}, {150, 150}},   // Start at (99, 99), end outside

			// Testing all edges
			{{50, 50}, {100, 50}},    // End exactly on the right boundary
			{{50, 50}, {-1, 50}},     // End exactly on the left boundary
			{{50, 50}, {50, 100}},    // End exactly on the top boundary
			{{50, 50}, {50, -1}},     // End exactly on the bottom boundary

			// Testing all corners
			{{50, 50}, {100, -1}},    // End exactly on the bottom right boundary
			{{50, 50}, {-1, -1}},     // End exactly on the bottom left boundary
			{{50, 50}, {100, 100}},    // End exactly on the top right boundary
			{{50, 50}, {-1, 100}}     // End exactly on the top left boundary
		};

		for (const auto& [inside_point, outside_point] : edge_cases)
		{
			draw_line_solid(surface, inside_point, outside_point, standard_color);

			// Assertions to verify correct boundary handling
			REQUIRE(is_point_inside_surface(surface, inside_point)); // Inside point must be valid
			REQUIRE(!is_point_inside_surface(surface, outside_point)); // Outside point must be out of bounds

			surface.clear();
		}
	}
}