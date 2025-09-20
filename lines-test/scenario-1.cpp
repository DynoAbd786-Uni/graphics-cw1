#include <catch2/catch_amalgamated.hpp>

//TODO
#include <algorithm>

#include "helpers.hpp"

#include "../draw2d/surface.hpp"
#include "../draw2d/draw.hpp"


TEST_CASE( "Identical lines", "[identical]" )
{
	Surface surface_1(100, 100);
	Surface surface_2(100, 100);
	surface_1.clear();
	surface_2.clear();

	ColorU8_sRGB standard_color = { 255, 255, 255 };
	
	// Testing fundamental directions
	SECTION( "horizontal" )
	{
		Vec2f p0 = { 10.f, 5.f };
		Vec2f p1 = { 20.f, 5.f };

		// Draw lines
		draw_line_solid( surface_1, p0, p1, standard_color);
		auto result1 = count_pixel_neighbours(surface_1);
		
		draw_line_solid(surface_2, p1, p0, standard_color);
		auto result2 = count_pixel_neighbours(surface_2);

		// Any differnece indicates a flaw on drawing in terms of line position and miscalcualted pixels
		draw_line_solid(surface_1, p1, p0, standard_color);
		auto combined_result = count_pixel_neighbours(surface_1);
		
		// Checking for identical line length 
		REQUIRE(result1 == result2);
		// Checking for identical line position
		REQUIRE(result1 == combined_result);
	}

	SECTION("vertical")
	{
		Vec2f p0 = { 10.f, 5.f };
		Vec2f p1 = { 10.f, 15.f };

		// Draw lines
		draw_line_solid(surface_1, p0, p1, standard_color);
		auto result1 = count_pixel_neighbours(surface_1);
		
		draw_line_solid(surface_2, p1, p0, standard_color);
		auto result2 = count_pixel_neighbours(surface_2);

		// Any differnece indicates a flaw on drawing in terms of line position and miscalcualted pixels
		draw_line_solid(surface_1, p1, p0, standard_color);
		auto combined_result = count_pixel_neighbours(surface_1);

		// Checking for identical line length 
		REQUIRE(result1 == result2);
		// Checking for identical line position
		REQUIRE(result1 == combined_result);
	}

	// Testing more complex lines travelling in different directions
	SECTION("perfect diagonal (increasing)")
	{
		Vec2f p0 = { 5.f, 5.f };
		Vec2f p1 = { 20.f, 20.f };

		// Draw lines
		draw_line_solid(surface_1, p0, p1, standard_color);
		auto result1 = count_pixel_neighbours(surface_1);
		
		draw_line_solid(surface_2, p1, p0, standard_color);
		auto result2 = count_pixel_neighbours(surface_2);

		// Any differnece indicates a flaw on drawing in terms of line position and miscalcualted pixels
		draw_line_solid(surface_1, p1, p0, standard_color);
		auto combined_result = count_pixel_neighbours(surface_1);

		// Checking for identical line length 
		REQUIRE(result1 == result2);
		// Checking for identical line position
		REQUIRE(result1 == combined_result);
	}

	SECTION("diagonal (low slope increasing)")
	{
		Vec2f p0 = { 5.f, 5.f };
		Vec2f p1 = { 20.f, 10.f };

		// Draw lines
		draw_line_solid(surface_1, p0, p1, standard_color);
		auto result1 = count_pixel_neighbours(surface_1);
		
		draw_line_solid(surface_2, p1, p0, standard_color);
		auto result2 = count_pixel_neighbours(surface_2);

		// Any differnece indicates a flaw on drawing in terms of line position and miscalcualted pixels
		draw_line_solid(surface_1, p1, p0, standard_color);
		auto combined_result = count_pixel_neighbours(surface_1);

		// Checking for identical line length 
		REQUIRE(result1 == result2);
		// Checking for identical line position
		REQUIRE(result1 == combined_result);
	}

	SECTION("diagonal (high slope increasing)")
	{
		Vec2f p0 = { 5.f, 5.f };
		Vec2f p1 = { 20.f, 50.f };

		// Draw lines
		draw_line_solid(surface_1, p0, p1, standard_color);
		auto result1 = count_pixel_neighbours(surface_1);
		
		draw_line_solid(surface_2, p1, p0, standard_color);
		auto result2 = count_pixel_neighbours(surface_2);

		// Any differnece indicates a flaw on drawing in terms of line position and miscalcualted pixels
		draw_line_solid(surface_1, p1, p0, standard_color);
		auto combined_result = count_pixel_neighbours(surface_1);

		// Checking for identical line length 
		REQUIRE(result1 == result2);
		// Checking for identical line position
		REQUIRE(result1 == combined_result);
	}



	SECTION("perfect diagonal (decreasing)")
	{
		Vec2f p0 = { 1.f, 6.f };
		Vec2f p1 = { 6.f, 1.f };

		// Draw lines
		draw_line_solid(surface_1, p0, p1, standard_color);
		auto result1 = count_pixel_neighbours(surface_1);

		draw_line_solid(surface_2, p1, p0, standard_color);
		auto result2 = count_pixel_neighbours(surface_2);

		// Any differnece indicates a flaw on drawing in terms of line position and miscalcualted pixels
		draw_line_solid(surface_1, p1, p0, standard_color);
		auto combined_result = count_pixel_neighbours(surface_1);

		// Checking for identical line length 
		REQUIRE(result1 == result2);
		// Checking for identical line position
		REQUIRE(result1 == combined_result);
	}

	SECTION("diagonal (low slope decreasing)")
	{
		Vec2f p0 = { 5.f, 5.f };
		Vec2f p1 = { 20.f, 2.f };

		// Draw lines
		draw_line_solid(surface_1, p0, p1, standard_color);
		auto result1 = count_pixel_neighbours(surface_1);
		
		draw_line_solid(surface_2, p1, p0, standard_color);
		auto result2 = count_pixel_neighbours(surface_2);

		// Any differnece indicates a flaw on drawing in terms of line position and miscalcualted pixels
		draw_line_solid(surface_1, p1, p0, standard_color);
		auto combined_result = count_pixel_neighbours(surface_1);

		// Checking for identical line length 
		REQUIRE(result1 == result2);
		// Checking for identical line position
		REQUIRE(result1 == combined_result);
	}

	SECTION("diagonal (high slope decreasing)")
	{
		Vec2f p0 = { 25.f, 5.f };
		Vec2f p1 = { 20.f, 20.f };

		// Draw lines
		draw_line_solid(surface_1, p0, p1, standard_color);
		auto result1 = count_pixel_neighbours(surface_1);

		draw_line_solid(surface_2, p1, p0, standard_color);
		auto result2 = count_pixel_neighbours(surface_2);

		// Any differnece indicates a flaw on drawing in terms of line position and miscalcualted pixels
		draw_line_solid(surface_1, p1, p0, standard_color);
		auto combined_result = count_pixel_neighbours(surface_1);

		// Checking for identical line length 
		REQUIRE(result1 == result2);
		// Checking for identical line position
		REQUIRE(result1 == combined_result);
	}


	// Testing the effects of clipped lines to see if the whole line has been drawn

	SECTION("Clipped line (top)")
	{
		Vec2f p0 = { 25.f, 195.f };
		Vec2f p1 = { 20.f, 20.f };

		// Draw lines
		draw_line_solid(surface_1, p0, p1, standard_color);
		auto result1 = count_pixel_neighbours(surface_1);

		draw_line_solid(surface_2, p1, p0, standard_color);
		auto result2 = count_pixel_neighbours(surface_2);

		// Any differnece indicates a flaw on drawing in terms of line position and miscalcualted pixels
		draw_line_solid(surface_1, p1, p0, standard_color);
		auto combined_result = count_pixel_neighbours(surface_1);

		// Checking for identical line length 
		REQUIRE(result1 == result2);
		// Checking for identical line position
		REQUIRE(result1 == combined_result);
	}

	SECTION("Clipped line (bottom)")
	{
		Vec2f p0 = { 25.f, -105.f };
		Vec2f p1 = { 20.f, 50.f };

		// Draw lines
		draw_line_solid(surface_1, p0, p1, standard_color);
		auto result1 = count_pixel_neighbours(surface_1);

		draw_line_solid(surface_2, p1, p0, standard_color);
		auto result2 = count_pixel_neighbours(surface_2);

		// Any differnece indicates a flaw on drawing in terms of line position and miscalcualted pixels
		draw_line_solid(surface_1, p1, p0, standard_color);
		auto combined_result = count_pixel_neighbours(surface_1);

		// Checking for identical line length 
		REQUIRE(result1 == result2);
		// Checking for identical line position
		REQUIRE(result1 == combined_result);
	}

	SECTION("Clipped line (left)")
	{
		Vec2f p0 = { -25.f, 5.f };
		Vec2f p1 = { 20.f, 20.f };

		// Draw lines
		draw_line_solid(surface_1, p0, p1, standard_color);
		auto result1 = count_pixel_neighbours(surface_1);

		draw_line_solid(surface_2, p1, p0, standard_color);
		auto result2 = count_pixel_neighbours(surface_2);

		// Any differnece indicates a flaw on drawing in terms of line position and miscalcualted pixels
		draw_line_solid(surface_1, p1, p0, standard_color);
		auto combined_result = count_pixel_neighbours(surface_1);

		// Checking for identical line length 
		REQUIRE(result1 == result2);
		// Checking for identical line position
		REQUIRE(result1 == combined_result);
	}

	SECTION("Clipped line (right)")
	{
		Vec2f p0 = { 125.f, 5.f };
		Vec2f p1 = { 20.f, 20.f };

		// Draw lines
		draw_line_solid(surface_1, p0, p1, standard_color);
		auto result1 = count_pixel_neighbours(surface_1);

		draw_line_solid(surface_2, p1, p0, standard_color);
		auto result2 = count_pixel_neighbours(surface_2);

		// Any differnece indicates a flaw on drawing in terms of line position and miscalcualted pixels
		draw_line_solid(surface_1, p1, p0, standard_color);
		auto combined_result = count_pixel_neighbours(surface_1);

		// Checking for identical line length 
		REQUIRE(result1 == result2);
		// Checking for identical line position
		REQUIRE(result1 == combined_result);
	}
}