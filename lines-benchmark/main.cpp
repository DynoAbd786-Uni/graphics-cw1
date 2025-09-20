#include <benchmark/benchmark.h>

#include "../draw2d/draw.hpp"
#include "../draw2d/draw-ex.hpp"
#include "../draw2d/surface-ex.hpp"


// Standard diagonal line
Vec2f line5_p0 = { 0.f, 0.f };
Vec2f line5_p1 = { 320.f, 240.f };

// Standard color
ColorU8_sRGB const STANDARD_COLOR = { 255, 255, 255 };

namespace
{
	// Horizontal line, variable according to frame buffer
	void bresenham_line_1( benchmark::State& aState )
	{
		auto const width = std::uint32_t(aState.range(0));
		auto const height = std::uint32_t(aState.range(1));

		Vec2f line1_p0 = { 0.f, 30.f };
		Vec2f line1_p1 = { static_cast<float>(width - 1.f), 30.f };

		SurfaceEx surface( width, height );
		surface.clear();

		for( auto _ : aState )
		{
			draw_line_solid(surface, line1_p0, line1_p1, STANDARD_COLOR);

			// ClobberMemory() ensures that the compiler won't optimize away
			// our blit operation. (Unlikely, but technically poossible.)
			benchmark::ClobberMemory(); 
		}
	}

	// Vertical line, variable according to frame buffer
	void bresenham_line_2(benchmark::State& aState)
	{
		
		auto const width = std::uint32_t(aState.range(0));
		auto const height = std::uint32_t(aState.range(1));

		Vec2f line2_p0 = { 30.f, 0.f };
		Vec2f line2_p1 = { 30.f, static_cast<float>(height - 1.f) };

		SurfaceEx surface(width, height);
		surface.clear();

		for (auto _ : aState)
		{
			draw_line_solid(surface, line2_p0, line2_p1, STANDARD_COLOR);

			// ClobberMemory() ensures that the compiler won't optimize away
			// our blit operation. (Unlikely, but technically poossible.)
			benchmark::ClobberMemory();
		}
	}

	// Cross section diagonal line, variable according to frame buffer
	void bresenham_line_3(benchmark::State& aState)
	{
		// Cross sectional diagonal line, variable according to frame buffer
		auto const width = std::uint32_t(aState.range(0));
		auto const height = std::uint32_t(aState.range(1));

		Vec2f line3_p0 = { 0.f, 0.f };
		Vec2f line3_p1 = { static_cast<float>(width - 1.f), static_cast<float>(height - 1.f) };

		SurfaceEx surface(width, height);
		surface.clear();

		for (auto _ : aState)
		{
			draw_line_solid(surface, line3_p0, line3_p1, STANDARD_COLOR);

			// ClobberMemory() ensures that the compiler won't optimize away
			// our blit operation. (Unlikely, but technically poossible.)
			benchmark::ClobberMemory();
		}
	}

	// Steep diagonal line, variable according to frame buffer
	void bresenham_line_4(benchmark::State& aState)
	{
		// Steep gradient diagonal line, variable according to frame buffer
		auto const width = std::uint32_t(aState.range(0));
		auto const height = std::uint32_t(aState.range(1));

		Vec2f line4_p0 = { 0.f, 0.f };
		Vec2f line4_p1 = { static_cast<float>(width / 3.f), static_cast<float>(height - 1.f) };

		SurfaceEx surface(width, height);
		surface.clear();

		for (auto _ : aState)
		{
			draw_line_solid(surface, line4_p0, line4_p1, STANDARD_COLOR);

			// ClobberMemory() ensures that the compiler won't optimize away
			// our blit operation. (Unlikely, but technically poossible.)
			benchmark::ClobberMemory();
		}
	}

	// Cross section diagonal line, fixed according to smallest frame buffer
	void bresenham_line_5(benchmark::State& aState)
	{
		// Cross sectional diagonal line, fixed according to smallest frame buffer
		auto const width = std::uint32_t(aState.range(0));
		auto const height = std::uint32_t(aState.range(1));

		SurfaceEx surface(width, height);
		surface.clear();

		for (auto _ : aState)
		{
			draw_line_solid(surface, line5_p0, line5_p1, STANDARD_COLOR);

			// ClobberMemory() ensures that the compiler won't optimize away
			// our blit operation. (Unlikely, but technically poossible.)
			benchmark::ClobberMemory();
		}
	}

	void dda_line_1(benchmark::State& aState)
	{
		// Horizontal line, variable according to frame buffer
		auto const width = std::uint32_t(aState.range(0));
		auto const height = std::uint32_t(aState.range(1));

		Vec2f line1_p0 = { 0.f, 30.f };
		Vec2f line1_p1 = { static_cast<float>(width - 1.f), 30.f };

		SurfaceEx surface(width, height);
		surface.clear();

		for (auto _ : aState)
		{
			draw_ex_line_solid(surface, line1_p0, line1_p1, STANDARD_COLOR);

			// ClobberMemory() ensures that the compiler won't optimize away
			// our blit operation. (Unlikely, but technically poossible.)
			benchmark::ClobberMemory();
		}
	}

	void dda_line_2(benchmark::State& aState)
	{
		// Vertical line, variable according to frame buffer
		auto const width = std::uint32_t(aState.range(0));
		auto const height = std::uint32_t(aState.range(1));

		Vec2f line2_p0 = { 30.f, 0.f };
		Vec2f line2_p1 = { 30.f, static_cast<float>(height - 1.f) };

		SurfaceEx surface(width, height);
		surface.clear();

		for (auto _ : aState)
		{
			draw_ex_line_solid(surface, line2_p0, line2_p1, STANDARD_COLOR);

			// ClobberMemory() ensures that the compiler won't optimize away
			// our blit operation. (Unlikely, but technically poossible.)
			benchmark::ClobberMemory();
		}
	}

	void dda_line_3(benchmark::State& aState)
	{
		// Cross sectional diagonal line, variable according to frame buffer
		auto const width = std::uint32_t(aState.range(0));
		auto const height = std::uint32_t(aState.range(1));

		Vec2f line3_p0 = { 0.f, 0.f };
		Vec2f line3_p1 = { static_cast<float>(width - 1.f), static_cast<float>(height - 1.f) };

		SurfaceEx surface(width, height);
		surface.clear();

		for (auto _ : aState)
		{
			draw_ex_line_solid(surface, line3_p0, line3_p1, STANDARD_COLOR);

			// ClobberMemory() ensures that the compiler won't optimize away
			// our blit operation. (Unlikely, but technically poossible.)
			benchmark::ClobberMemory();
		}
	}

	void dda_line_4(benchmark::State& aState)
	{
		// Steep diagonal line, variable according to frame buffer
		auto const width = std::uint32_t(aState.range(0));
		auto const height = std::uint32_t(aState.range(1));

		Vec2f line4_p0 = { 0.f, 0.f };
		Vec2f line4_p1 = { static_cast<float>(width / 3.f), static_cast<float>(height - 1.f) };

		SurfaceEx surface(width, height);
		surface.clear();

		for (auto _ : aState)
		{
			draw_ex_line_solid(surface, line4_p0, line4_p1, STANDARD_COLOR);

			// ClobberMemory() ensures that the compiler won't optimize away
			// our blit operation. (Unlikely, but technically poossible.)
			benchmark::ClobberMemory();
		}
	}

	// Cross section diagonal line, fixed according to smallest frame buffer
	void dda_line_5(benchmark::State& aState)
	{
		// Cross sectional diagonal line, fixed according to smallest frame buffer
		auto const width = std::uint32_t(aState.range(0));
		auto const height = std::uint32_t(aState.range(1));

		SurfaceEx surface(width, height);
		surface.clear();

		for (auto _ : aState)
		{
			draw_ex_line_solid(surface, line5_p0, line5_p1, STANDARD_COLOR);

			// ClobberMemory() ensures that the compiler won't optimize away
			// our blit operation. (Unlikely, but technically poossible.)
			benchmark::ClobberMemory();
		}
	}
}

BENCHMARK(bresenham_line_1)
->Args({ 320, 240 })
->Args({ 1280, 720 })
->Args({ 1920, 1080 })
->Args({ 7680, 4320 })
;

BENCHMARK(bresenham_line_2)
->Args({ 320, 240 })
->Args({ 1280, 720 })
->Args({ 1920, 1080 })
->Args({ 7680, 4320 })
;

BENCHMARK(bresenham_line_3)
->Args({ 320, 240 })
->Args({ 1280, 720 })
->Args({ 1920, 1080 })
->Args({ 7680, 4320 })
;

BENCHMARK(bresenham_line_4)
->Args({ 320, 240 })
->Args({ 1280, 720 })
->Args({ 1920, 1080 })
->Args({ 7680, 4320 })
;

BENCHMARK(bresenham_line_5)
->Args({ 320, 240 })
->Args({ 1280, 720 })
->Args({ 1920, 1080 })
->Args({ 7680, 4320 })
;

BENCHMARK(dda_line_1)
->Args({ 320, 240 })
->Args({ 1280, 720 })
->Args({ 1920, 1080 })
->Args({ 7680, 4320 })
;

BENCHMARK(dda_line_2)
->Args({ 320, 240 })
->Args({ 1280, 720 })
->Args({ 1920, 1080 })
->Args({ 7680, 4320 })
;

BENCHMARK(dda_line_3)
->Args({ 320, 240 })
->Args({ 1280, 720 })
->Args({ 1920, 1080 })
->Args({ 7680, 4320 })
;

BENCHMARK(dda_line_4)
->Args({ 320, 240 })
->Args({ 1280, 720 })
->Args({ 1920, 1080 })
->Args({ 7680, 4320 })
;

BENCHMARK(dda_line_5)
->Args({ 320, 240 })
->Args({ 1280, 720 })
->Args({ 1920, 1080 })
->Args({ 7680, 4320 })
;


BENCHMARK_MAIN();
