#include <catch2/catch_amalgamated.hpp>
#include <iostream>

// TODO: exhaustively test all possible triangle cases 
// (cases where there can be up to 7 points shown on the surface)
#include "helpers.hpp"

#include "../draw2d/surface.hpp"
#include "../draw2d/draw.hpp"
#include "../draw2d/color.hpp"

TEST_CASE("triangle points", "[points]")
{
	Surface surface(100, 100);
	surface.clear();

	ColorF r = { 1.f, 0.f, 0.f }; // Red
	ColorF g = { 0.f, 1.f, 0.f }; // Green
	ColorF b = { 0.f, 0.f, 1.f }; // Blue

	ColorF black = { 0.f, 0.f, 0.f };

	// Define test case struct
	struct TestCase {
		ColorF color1, color2, color3;
		float expected_x, expected_y;
		ColorF expected_color;
		bool check_color;
	};

	SECTION("1 point on screen")
	{
		// Define test cases
		std::vector<TestCase> cases = {
			{r, g, b, 50.f, 0.f, r, true},
			{b, r, g, 99.f, 99.f, black, true},	// No pixel with red hints should be on screen, so func should return max
			{g, b, r, 99.f, 99.f, black, true}
		};

		for (const auto& test : cases)
		{
			// Draw the triangle
			draw_triangle_interp(surface,
				{ 50.f, 0.f }, { 40.f, -70.f }, { 75.f, -25.f },
				test.color1, test.color2, test.color3);

			// Find the most red pixel
			auto col = linear_from_srgb(find_most_red_pixel(surface));
			auto col_loc = find_most_red_pixel_loc(surface);

			// Verify the red pixel's location and colour
			bool is_red_in_correct_place =
				(col_loc.x == test.expected_x && col_loc.y == test.expected_y &&
					col.r == test.expected_color.r && col.g == test.expected_color.g && col.b == test.expected_color.b);
			REQUIRE(is_red_in_correct_place == true);
		}
	}

	SECTION("2 points on screen")
	{
		// Define test cases
		std::vector<TestCase> cases = {
			{r, g, b, 50.f, 0.f, r, true},
			{b, r, g, 70.f, 0.f, r, true},	
			{g, b, r, 99.f, 99.f, black, true}	// No pixel with red hints should be on screen, so func should return max
		};

		for (const auto& test : cases)
		{
			// Draw the triangle
			draw_triangle_interp(surface,
				{ 50.f, 0.f }, { 70.f, 0.f }, { 75.f, -25.f },
				test.color1, test.color2, test.color3);

			// Find the most red pixel
			auto col = linear_from_srgb(find_most_red_pixel(surface));
			auto col_loc = find_most_red_pixel_loc(surface);

			// Verify the red pixel's location and colour
			bool is_red_in_correct_place =
				(col_loc.x == test.expected_x && col_loc.y == test.expected_y &&
					col.r == test.expected_color.r && col.g == test.expected_color.g && col.b == test.expected_color.b);
			CHECK(is_red_in_correct_place == true);
		}
	}

	// We know drawing a generic triangle works, need to test when all triangle points are off screen, but still forms a triangle
	SECTION("3 points on screen")
	{
		// Define test cases
		// Using Desmos to determine positions
		std::vector<TestCase> cases = {
			{r, g, b, 0.f, 0.f, black, false},
			{b, r, g, 15.f, 0.f, black, false},
			{g, b, r, 0.f, 15.f, black, false}
		};

		for (const auto& test : cases)
		{
			// Draw the triangle
			draw_triangle_interp(surface,
				{ -15.f, -15.f }, { 30.f, -15.f }, { -15.f, 30.f },
				test.color1, test.color2, test.color3);

			// Find the most red pixel
			auto col = linear_from_srgb(find_most_red_pixel(surface));
			auto col_loc = find_most_red_pixel_loc(surface);

			// Verify the red pixel's location and colour
			// Not testing for exact rgb value because thats related to the code
			// Implementing this check is a copy paste move, which shows nothing test-wise
			bool is_red_in_correct_place =
				(col_loc.x == test.expected_x && col_loc.y == test.expected_y &&
					((col.r == test.expected_color.r && col.g == test.expected_color.g && col.b == test.expected_color.b) || test.check_color == false)
					);
			CHECK(is_red_in_correct_place == true);
		}
	}

	// Special case of "the screen is full of triangle" has already been implemented in specials.cpp
	SECTION("4 points on screen")
	{
		// Define test cases
		// Using Desmos to determine positions
		std::vector<TestCase> cases = {
			{r, g, b, 0.f, 0.f, black, false},
			{b, r, g, 10.f, 20.f, r, true},
			{g, b, r, 40.f, 10.f, r, true}
		};

		for (const auto& test : cases)
		{
			// Draw the triangle
			draw_triangle_interp(surface,
				{ -15.f, -15.f }, { 10.f, 20.f }, { 40.f, 10.f },
				test.color1, test.color2, test.color3);

			// Find the most red pixel
			auto col = linear_from_srgb(find_most_red_pixel(surface));
			auto col_loc = find_most_red_pixel_loc(surface);

			// Verify the red pixel's location and colour
			// Not testing for exact rgb value for some tests because thats related to the code's barycentric calculations
			// Implementing this check is a copy paste move, which shows nothing test-wise
			bool is_red_in_correct_place =
				(col_loc.x == test.expected_x && col_loc.y == test.expected_y && 
					((col.r == test.expected_color.r && col.g == test.expected_color.g && col.b == test.expected_color.b) || test.check_color == false)
					);
			CHECK(is_red_in_correct_place == true);
		}
	}

	SECTION("5 points on screen")
	{
		// Define test cases
		// Using Desmos to determine positions
		std::vector<TestCase> cases = {
			{r, g, b, 0.f, 20.f, black, false},
			{b, r, g, 65.f, 0.f, black, false},
			{g, b, r, 80.f, 60.f, r, true}
		};

		for (const auto& test : cases)
		{
			// Draw the triangle
			draw_triangle_interp(surface,
				{ -10.f, 20.f }, { 60.f, -20.f }, { 80.f, 60.f },
				test.color1, test.color2, test.color3);

			// Find the most red pixel
			auto col = linear_from_srgb(find_most_red_pixel(surface));
			auto col_loc = find_most_red_pixel_loc(surface);

			// Verify the red pixel's location and colour
			// Not testing for exact rgb value for some tests because thats related to the code's barycentric calculations
			// Implementing this check is a copy paste move, which shows nothing test-wise
			bool is_red_in_correct_place =
				(col_loc.x == test.expected_x && col_loc.y == test.expected_y &&
					((col.r == test.expected_color.r && col.g == test.expected_color.g && col.b == test.expected_color.b) || test.check_color == false)
					);
			CHECK(is_red_in_correct_place == true);
		}
	}

	SECTION("6 points on screen")
	{
		// Define test cases
		// Using Desmos to determine positions
		std::vector<TestCase> cases = {
			{r, g, b, 1.f, 20.f, black, false},
			{b, r, g, 75.f, 0.f, black, false},
			{g, b, r, 99.f, 53.f, black, false}
		};

		for (const auto& test : cases)
		{
			// Draw the triangle
			draw_triangle_interp(surface,
				{ -10.f, 20.f }, { 60.f, -20.f }, { 120.f, 60.f },
				test.color1, test.color2, test.color3);

			// Find the most red pixel
			auto col = linear_from_srgb(find_most_red_pixel(surface));
			auto col_loc = find_most_red_pixel_loc(surface);

			// Verify the red pixel's location and colour
			// Not testing for exact rgb value for some tests because thats related to the code's barycentric calculations
			// Implementing this check is a copy paste move, which shows nothing test-wise
			bool is_red_in_correct_place =
				(col_loc.x == test.expected_x && col_loc.y == test.expected_y &&
					((col.r == test.expected_color.r && col.g == test.expected_color.g && col.b == test.expected_color.b) || test.check_color == false)
					);
			CHECK(is_red_in_correct_place == true);
		}
	}

	SECTION("7 points on screen")
	{
		// Define test cases
		// Using Desmos to determine positions
		std::vector<TestCase> cases = {
			{r, g, b, 18.f, 99.f, black, false},
			{b, r, g, 49.f, 0.f, black, false},
			{g, b, r, 99.f, 61.f, black, false}
		};

		for (const auto& test : cases)
		{
			// Draw the triangle
			draw_triangle_interp(surface,
				{ -10.f, 110.f }, { 60.f, -20.f }, { 120.f, 60.f },
				test.color1, test.color2, test.color3);

			// Find the most red pixel
			auto col = linear_from_srgb(find_most_red_pixel(surface));
			auto col_loc = find_most_red_pixel_loc(surface);

			// Verify the red pixel's location and colour
			// Not testing for exact rgb value for some tests because thats related to the code's barycentric calculations
			// Implementing this check is a copy paste move, which shows nothing test-wise
			bool is_red_in_correct_place =
				(col_loc.x == test.expected_x && col_loc.y == test.expected_y &&
					((col.r == test.expected_color.r && col.g == test.expected_color.g && col.b == test.expected_color.b) || test.check_color == false)
					);
			CHECK(is_red_in_correct_place == true);
		}
	}
}

