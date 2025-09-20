#include <benchmark/benchmark.h>

#include <algorithm>

#include <cassert>

#include "../draw2d/image.hpp"
#include "../draw2d/draw-ex.hpp"
#include "../draw2d/surface-ex.hpp"

namespace
{
	void default_blit_128( benchmark::State& aState )
	{
		auto const width = std::uint32_t(aState.range(0));
		auto const height = std::uint32_t(aState.range(1));

		SurfaceEx surface( width, height );
		surface.clear();

		auto source = load_image("assets/128x128.png");
		assert( source );

		for( auto _ : aState )
		{
			// Make sure as much as possible of the image is visible. The
			// sample image here is 1k*1k. Blitting to center = (500,500) makes
			// sure that as much as possible of the image ends up in the target
			// surface.
			blit_masked( surface, *source, { width / 2.f, height / 2.f });

			// ClobberMemory() ensures that the compiler won't optimize away
			// our blit operation. (Unlikely, but technically poossible.)
			benchmark::ClobberMemory(); 
		}

		// The following enables the benchmarking library to print information
		// about the memory bandwidth. The total number of bytes processed is
		// *approximatively* two times the total number of bytes in the blit,
		// accounding for both reading and writing. ("Approximatively" since
		// not all pixels are written.)
		auto const maxBlitX = std::min( width, source->get_width() );
		auto const maxBlitY = std::min( height, source->get_height() );

		aState.SetBytesProcessed( 2*maxBlitX*maxBlitY*4 * aState.iterations() );
	}


	 void blit_with_memcpy_128( benchmark::State& aState )
	 {
		 auto const width = std::uint32_t(aState.range(0));
		 auto const height = std::uint32_t(aState.range(1));

		 SurfaceEx surface(width, height);
		 surface.clear();

		 auto source = load_image("assets/128x128.png");
		 assert(source);

		 for (auto _ : aState)
		 {
			 // Make sure as much as possible of the image is visible. The
			 // sample image here is 1k*1k. Blitting to center = (500,500) makes
			 // sure that as much as possible of the image ends up in the target
			 // surface.
			 blit_ex_memcpy(surface, *source, { width/2.f, height/2.f });

			 // ClobberMemory() ensures that the compiler won't optimize away
			 // our blit operation. (Unlikely, but technically poossible.)
			 benchmark::ClobberMemory();
		 }

		 // The following enables the benchmarking library to print information
		 // about the memory bandwidth. The total number of bytes processed is
		 // *approximatively* two times the total number of bytes in the blit,
		 // accounding for both reading and writing. ("Approximatively" since
		 // not all pixels are written.)
		 auto const maxBlitX = std::min(width, source->get_width());
		 auto const maxBlitY = std::min(height, source->get_height());

		 aState.SetBytesProcessed(2 * maxBlitX * maxBlitY * 4 * aState.iterations());
	 }

	 void blit_without_alpha_masking_128(benchmark::State& aState)
	 {
		 auto const width = std::uint32_t(aState.range(0));
		 auto const height = std::uint32_t(aState.range(1));

		 SurfaceEx surface(width, height);
		 surface.clear();

		 auto source = load_image("assets/128x128.png");
		 assert(source);

		 for (auto _ : aState)
		 {
			 // Make sure as much as possible of the image is visible. The
			 // sample image here is 1k*1k. Blitting to center = (500,500) makes
			 // sure that as much as possible of the image ends up in the target
			 // surface.
			 blit_ex_solid(surface, *source, { width / 2.f, height / 2.f });

			 // ClobberMemory() ensures that the compiler won't optimize away
			 // our blit operation. (Unlikely, but technically poossible.)
			 benchmark::ClobberMemory();
		 }

		 // The following enables the benchmarking library to print information
		 // about the memory bandwidth. The total number of bytes processed is
		 // *approximatively* two times the total number of bytes in the blit,
		 // accounding for both reading and writing. ("Approximatively" since
		 // not all pixels are written.)
		 auto const maxBlitX = std::min(width, source->get_width());
		 auto const maxBlitY = std::min(height, source->get_height());

		 aState.SetBytesProcessed(2 * maxBlitX * maxBlitY * 4 * aState.iterations());
	 }

	 void default_blit_1024(benchmark::State& aState)
	 {
		 auto const width = std::uint32_t(aState.range(0));
		 auto const height = std::uint32_t(aState.range(1));

		 SurfaceEx surface(width, height);
		 surface.clear();

		 auto source = load_image("assets/1024x1024.png");
		 assert(source);

		 for (auto _ : aState)
		 {
			 // Make sure as much as possible of the image is visible. The
			 // sample image here is 1k*1k. Blitting to center = (500,500) makes
			 // sure that as much as possible of the image ends up in the target
			 // surface.
			 blit_masked(surface, *source, { width / 2.f, height / 2.f });

			 // ClobberMemory() ensures that the compiler won't optimize away
			 // our blit operation. (Unlikely, but technically poossible.)
			 benchmark::ClobberMemory();
		 }

		 // The following enables the benchmarking library to print information
		 // about the memory bandwidth. The total number of bytes processed is
		 // *approximatively* two times the total number of bytes in the blit,
		 // accounding for both reading and writing. ("Approximatively" since
		 // not all pixels are written.)
		 auto const maxBlitX = std::min(width, source->get_width());
		 auto const maxBlitY = std::min(height, source->get_height());

		 aState.SetBytesProcessed(2 * maxBlitX * maxBlitY * 4 * aState.iterations());
	 }


	 void blit_with_memcpy_1024(benchmark::State& aState)
	 {
		 auto const width = std::uint32_t(aState.range(0));
		 auto const height = std::uint32_t(aState.range(1));

		 SurfaceEx surface(width, height);
		 surface.clear();

		 auto source = load_image("assets/1024x1024.png");
		 assert(source);

		 for (auto _ : aState)
		 {
			 // Make sure as much as possible of the image is visible. The
			 // sample image here is 1k*1k. Blitting to center = (500,500) makes
			 // sure that as much as possible of the image ends up in the target
			 // surface.
			 blit_ex_memcpy(surface, *source, { width / 2.f, height / 2.f });

			 // ClobberMemory() ensures that the compiler won't optimize away
			 // our blit operation. (Unlikely, but technically poossible.)
			 benchmark::ClobberMemory();
		 }

		 // The following enables the benchmarking library to print information
		 // about the memory bandwidth. The total number of bytes processed is
		 // *approximatively* two times the total number of bytes in the blit,
		 // accounding for both reading and writing. ("Approximatively" since
		 // not all pixels are written.)
		 auto const maxBlitX = std::min(width, source->get_width());
		 auto const maxBlitY = std::min(height, source->get_height());

		 aState.SetBytesProcessed(2 * maxBlitX * maxBlitY * 4 * aState.iterations());
	 }

	 void blit_without_alpha_masking_1024(benchmark::State& aState)
	 {
		 auto const width = std::uint32_t(aState.range(0));
		 auto const height = std::uint32_t(aState.range(1));

		 SurfaceEx surface(width, height);
		 surface.clear();

		 auto source = load_image("assets/1024x1024.png");
		 assert(source);

		 for (auto _ : aState)
		 {
			 // Make sure as much as possible of the image is visible. The
			 // sample image here is 1k*1k. Blitting to center = (500,500) makes
			 // sure that as much as possible of the image ends up in the target
			 // surface.
			 blit_ex_solid(surface, *source, { width / 2.f, height / 2.f });

			 // ClobberMemory() ensures that the compiler won't optimize away
			 // our blit operation. (Unlikely, but technically poossible.)
			 benchmark::ClobberMemory();
		 }

		 // The following enables the benchmarking library to print information
		 // about the memory bandwidth. The total number of bytes processed is
		 // *approximatively* two times the total number of bytes in the blit,
		 // accounding for both reading and writing. ("Approximatively" since
		 // not all pixels are written.)
		 auto const maxBlitX = std::min(width, source->get_width());
		 auto const maxBlitY = std::min(height, source->get_height());

		 aState.SetBytesProcessed(2 * maxBlitX * maxBlitY * 4 * aState.iterations());
	 }
}

BENCHMARK(blit_without_alpha_masking_1024)
->Args({ 320, 240 })
->Args({ 1280, 720 })
->Args({ 1920, 1080 })
->Args({ 7680, 4320 })
;

BENCHMARK( default_blit_128)
->Args( { 320, 240 } )
->Args( { 1280, 720 } )
->Args( { 1920, 1080 } )
->Args( { 7680, 4320 } )
;

BENCHMARK(blit_with_memcpy_128)
->Args({ 320, 240 })
->Args({ 1280, 720 })
->Args({ 1920, 1080 })
->Args({ 7680, 4320 })
;

BENCHMARK(blit_without_alpha_masking_128)
->Args({ 320, 240 })
->Args({ 1280, 720 })
->Args({ 1920, 1080 })
->Args({ 7680, 4320 })
;

BENCHMARK(default_blit_1024)
->Args({ 320, 240 })
->Args({ 1280, 720 })
->Args({ 1920, 1080 })
->Args({ 7680, 4320 })
;

BENCHMARK(blit_with_memcpy_1024)
->Args({ 320, 240 })
->Args({ 1280, 720 })
->Args({ 1920, 1080 })
->Args({ 7680, 4320 })
;

BENCHMARK(blit_without_alpha_masking_1024)
->Args({ 320, 240 })
->Args({ 1280, 720 })
->Args({ 1920, 1080 })
->Args({ 7680, 4320 })
;


//BENCHMARK( my_other_blit_ )
//	->Args( { 320, 240 } )
//	->Args( { 7680, 4320 } )
//;

BENCHMARK_MAIN();
