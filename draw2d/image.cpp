#include "image.hpp"

#include <memory>
#include <algorithm>

#include <cstdio>
#include <cstring>
#include <cassert>

#include <stb_image.h>

#include "surface.hpp"

#include "../support/error.hpp"

namespace
{
	struct STBImageRGBA_ : public ImageRGBA
	{
		STBImageRGBA_( Index, Index, std::uint8_t* );
		virtual ~STBImageRGBA_();
	};
}

ImageRGBA::ImageRGBA()
	: mWidth( 0 )
	, mHeight( 0 )
	, mData( nullptr )
{}

ImageRGBA::~ImageRGBA() = default;


std::unique_ptr<ImageRGBA> load_image( char const* aPath )
{
	assert( aPath );

	stbi_set_flip_vertically_on_load( true );

	int w, h, channels;
	stbi_uc* ptr = stbi_load( aPath, &w, &h, &channels, 4 );
	if( !ptr )
		throw Error( "Unable to load image \"%s\"", aPath );

	return std::make_unique<STBImageRGBA_>(
		ImageRGBA::Index(w),
		ImageRGBA::Index(h),
		ptr
	);
}

void blit_masked( Surface& aSurface, ImageRGBA const& aImage, Vec2f aPosition )
{
	// Retrieve data 
	int max_height_surface = aSurface.get_height();
	int max_width_surface = aSurface.get_width();

	int max_height_image = aImage.get_height();
	int max_width_image = aImage.get_width();

	// Precompute constants
	int position_x = static_cast<int>(round(aPosition.x));
	int position_y = static_cast<int>(round(aPosition.y));

	int half_width_image = max_width_image / 2;
	int half_height_image = max_height_image / 2;

	// Calculate start and end bounds
	int x_start = std::max(0, half_width_image - position_x);
	int x_end = std::min(max_width_image, max_width_surface - position_x + half_width_image);
	int y_start = std::max(0, half_height_image - position_y);
	int y_end = std::min(max_height_image, max_height_surface - position_y + half_height_image);

	// Blitting loop
	for (int y = y_start; y < y_end; y++) 
	{
		for (int x = x_start; x < x_end; x++) 
		{
			// Retrieve image pixel
			ColorU8_sRGB_Alpha image_pixel_color = aImage.get_pixel(x, y);

			// Skip pixels with alpha < 128
			if (image_pixel_color.a < 128) continue;

			// Compute surface coordinates
			int surface_x = position_x - half_width_image + x;
			int surface_y = position_y - half_height_image + y;

			// Directly write to the surface
			aSurface.set_pixel_srgb(surface_x, surface_y,
				{ image_pixel_color.r, image_pixel_color.g, image_pixel_color.b });
		}
	}
}

namespace
{
	STBImageRGBA_::STBImageRGBA_( Index aWidth, Index aHeight, std::uint8_t* aPtr )
	{
		mWidth = aWidth;
		mHeight = aHeight;
		mData = aPtr;
	}

	STBImageRGBA_::~STBImageRGBA_()
	{
		if( mData )
			stbi_image_free( mData );
	}
}
