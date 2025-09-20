#include <catch2/catch_amalgamated.hpp>

//TODO
#include <algorithm>

#include "helpers.hpp"

#include "../draw2d/surface.hpp"
#include "../draw2d/draw.hpp"

TEST_CASE("2 points out of bounds", "[2_OOB]")
{
	Surface surface(100, 100);
	surface.clear();

	ColorU8_sRGB standard_color = { 255, 255, 255 };

	SECTION("2 points OOB, all genric combinations")
	{
		// Create a list of Vec2f to use for marking outside bounds
		std::vector<Vec2f> outside_vecs{
			{-30, -30}, {30, -30}, {130, -30},   // Below
			{-30, 30}, {130, 30},                  // Left/Right
			{-30, 230}, {30, 130}, {130, 130}      // Above
		};

		// Looping through all vecs
		for (auto& vec1 : outside_vecs)
		{
			// Draw combinations of 
			for (auto& vec2 : outside_vecs)
			{
				draw_line_solid(surface, vec1, vec2, standard_color);

				// Compute outcode to see if line is outside drawable region
				int vec1_outcode = compute_outcode(static_cast<int>(vec1.x), static_cast<int>(vec1.y));
				int vec2_outcode = compute_outcode(static_cast<int>(vec2.x), static_cast<int>(vec2.y));

				// Checking if line should be drawn
				if (vec1_outcode & vec2_outcode)
				{
					// Trivially reject the line (both points are outside in the same side)
					REQUIRE(max_col_pixel_count(surface) == 0);
					REQUIRE(max_row_pixel_count(surface) == 0);
				}
				else
				{
					// Line falls within the drawable region
					// Following similar process to check correct line is drawn as with 1 point OOB
					// Using gradients to set checks
					float dx = vec1.x - vec2.x;
					float dy = vec1.y - vec2.y;

					float absolute_gradient = (dx != 0) ? abs(dy / dx) : std::numeric_limits<float>::infinity();

					// Checking for correct boundary condition
					if (absolute_gradient >= 1) { REQUIRE(max_row_pixel_count(surface) == 1); }
					if (absolute_gradient <= 1) { REQUIRE(max_col_pixel_count(surface) == 1); }

					surface.clear();
				}
			}
		}
	}

	SECTION("Edge cases for boundary conditions, 2 OOB")
	{
		// Add specific test cases for boundary points
		// Testing point edge cases outside surface that should not draw anything
		std::vector<std::pair<Vec2f, Vec2f>> edge_cases = {
			{{-1, -1}, {-1, 100}},
			{{-1, 100}, {100, 100}},
			{{100, 100}, {100, -1}},
			{{100, -1}, {-1, -1}},
		};

		for (const auto& [inside_point, outside_point] : edge_cases)
		{
			draw_line_solid(surface, inside_point, outside_point, standard_color);

			// Assertions to verify correct boundary handling
			REQUIRE(max_col_pixel_count(surface) == 0);
			REQUIRE(max_row_pixel_count(surface) == 0);

			surface.clear();
		}
	}
}