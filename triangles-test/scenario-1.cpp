#include <catch2/catch_amalgamated.hpp>

// TODO: test triangle creation, all 3 apex, all 3 triangle forms
#include "helpers.hpp"

#include "../draw2d/surface.hpp"
#include "../draw2d/draw.hpp"
#include "../draw2d/color.hpp"

TEST_CASE("Triangle creation", "[create]")
{
	Surface surface(100, 100);
	surface.clear();

	ColorF r = { 1.f, 0.f, 0.f }; // Red
	ColorF g = { 0.f, 1.f, 0.f }; // Green
	ColorF b = { 0.f, 0.f, 1.f }; // Blue

	// Define test case struct
	struct TestCase {
		ColorF color1, color2, color3;
		float expected_x, expected_y;
	};

	SECTION("flat bottom triangle")
	{
		// Define test cases
		std::vector<TestCase> cases = {
			{r, g, b, 25.f, 25.f},
			{b, r, g, 40.f, 70.f},
			{g, b, r, 75.f, 25.f}
		};

		for (const auto& test : cases)
		{
			// Draw the triangle
			draw_triangle_interp(surface,
				{ 25.f, 25.f }, { 40.f, 70.f }, { 75.f, 25.f },
				test.color1, test.color2, test.color3);

			// Find the most red pixel
			auto col = linear_from_srgb(find_most_red_pixel(surface));
			auto col_loc = find_most_red_pixel_loc(surface);

			// Verify the red pixel's location and colour
			bool is_red_in_correct_place =
				(col_loc.x == test.expected_x && col_loc.y == test.expected_y &&
					col.r == 1.f && col.g == 0.f && col.b == 0.f);
			REQUIRE(is_red_in_correct_place == true);
		}
	}

	SECTION("flat top triangle")
	{
		// Define test cases
		std::vector<TestCase> cases = {
			{r, g, b, 30.f, 75.f},
			{b, r, g, 40.f, 20.f},
			{g, b, r, 85.f, 75.f}
		};

		for (const auto& test : cases)
		{
			// Draw the triangle
			draw_triangle_interp(surface,
				{ 30.f, 75.f }, { 40.f, 20.f }, { 85.f, 75.f },
				test.color1, test.color2, test.color3);

			// Find the most red pixel
			auto col = linear_from_srgb(find_most_red_pixel(surface));
			auto col_loc = find_most_red_pixel_loc(surface);

			// Verify the red pixel's location and colour
			bool is_red_in_correct_place =
				(col_loc.x == test.expected_x && col_loc.y == test.expected_y &&
					col.r == 1.f && col.g == 0.f && col.b == 0.f);
			REQUIRE(is_red_in_correct_place == true);
		}
	}

	SECTION("generic scalene triangle")
	{
		// Define test cases
		std::vector<TestCase> cases = {
			{r, g, b, 30.f, 55.f},
			{b, r, g, 40.f, 20.f},
			{g, b, r, 85.f, 75.f}
		};

		for (const auto& test : cases)
		{
			// Draw the triangle
			draw_triangle_interp(surface,
				{ 30.f, 55.f }, { 40.f, 20.f }, { 85.f, 75.f },
				test.color1, test.color2, test.color3);

			// Find the most red pixel
			auto col = linear_from_srgb(find_most_red_pixel(surface));
			auto col_loc = find_most_red_pixel_loc(surface);

			// Verify the red pixel's location and colour
			bool is_red_in_correct_place =
				(col_loc.x == test.expected_x && col_loc.y == test.expected_y &&
					col.r == 1.f && col.g == 0.f && col.b == 0.f);
			REQUIRE(is_red_in_correct_place == true);
		}
	}
}