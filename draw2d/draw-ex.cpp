#include "draw-ex.hpp"

#include <algorithm>

#include <cstring> // for std::memcpy()

#include "image.hpp"
#include "surface-ex.hpp"

// Define boundaries for clipping
int CLIP_LEFT_EX = 0;
int CLIP_RIGHT_EX = 0;
int CLIP_BOTTOM_EX = 0;
int CLIP_TOP_EX = 0;


// Bitwise outcodes for each boundary
enum Outcode {
	INSIDE = 0,      // 0000
	LEFT = 1,        // 0001
	RIGHT = 2,       // 0010
	BOTTOM = 4,      // 0100
	TOP = 8          // 1000
};

// Function to compute the outcode for a given point
// Use of bitwise operations gives best performance for determining position of point
// Relative to drawablew region
int compute_outcode_ex(int x, int y)
{
	int code = INSIDE;
	if (x < CLIP_LEFT_EX) code |= LEFT;
	else if (x > CLIP_RIGHT_EX) code |= RIGHT;
	if (y < CLIP_BOTTOM_EX) code |= BOTTOM;
	else if (y > CLIP_TOP_EX) code |= TOP;
	return code;
}

void draw_ex_line_solid( SurfaceEx& aSurface, Vec2f aBegin, Vec2f aEnd, ColorU8_sRGB aColor )
{
	// Set constants according to drawable region	
	CLIP_LEFT_EX = 0;
	CLIP_RIGHT_EX = aSurface.get_width() - 1;
	CLIP_BOTTOM_EX = 0;
	CLIP_TOP_EX = aSurface.get_height() - 1;

	// Extract integer values of the start and end points
	int x0 = static_cast<int>(aBegin.x);
	int y0 = static_cast<int>(aBegin.y);
	int x1 = static_cast<int>(aEnd.x);
	int y1 = static_cast<int>(aEnd.y);

	// Consistency issue when attempting to calculate lines.
	// Tiny floating point error can result in lines 1px next to each other. 
	// This doesnt affect start and end positions, but does affect the gradient, clipping and culling algorithms across long lines
	// Order points to eliminate this consistency error 
	// Order points by y-coordinate
	if (y0 > y1) {
		std::swap(x0, x1);
		std::swap(y0, y1);
	}

	// Using Cohen-Sutherland Clipping Algorithm
	// Calculate positions of coordinates relative to drawable region
	int outcode0 = compute_outcode_ex(x0, y0);
	int outcode1 = compute_outcode_ex(x1, y1);

	bool accept = false;

	// Determine point positions based on outcode result
	while (true) {
		if (!(outcode0 | outcode1))
		{
			// Trivially accept the line
			// Line falls within the drawable region
			accept = true;
			break;
		}
		else if (outcode0 & outcode1)
		{
			// Trivially reject the line (both points are outside in the same side)
			return;
		}
		else
		{
			// Clip the line
			int x = 0;
			int y = 0;
			int outcode_out = outcode0 ? outcode0 : outcode1;

			// Find the intersection point with the clipping boundary
			if (outcode_out & TOP) {           // Point is above the clip window
				x = x0 + (x1 - x0) * (CLIP_TOP_EX - y0) / (y1 - y0);
				y = CLIP_TOP_EX;
			}
			else if (outcode_out & BOTTOM) { // Point is below the clip window
				x = x0 + (x1 - x0) * (CLIP_BOTTOM_EX - y0) / (y1 - y0);
				y = CLIP_BOTTOM_EX;
			}
			else if (outcode_out & RIGHT) {  // Point is to the right of the clip window
				y = y0 + (y1 - y0) * (CLIP_RIGHT_EX - x0) / (x1 - x0);
				x = CLIP_RIGHT_EX;
			}
			else if (outcode_out & LEFT) {   // Point is to the left of the clip window
				y = y0 + (y1 - y0) * (CLIP_LEFT_EX - x0) / (x1 - x0);
				x = CLIP_LEFT_EX;
			}

			// Replace the outside point with the intersection point and update outcode
			if (outcode_out == outcode0) {
				x0 = x;
				y0 = y;
				outcode0 = compute_outcode_ex(x0, y0);
			}
			else {
				x1 = x;
				y1 = y;
				outcode1 = compute_outcode_ex(x1, y1);
			}
		}
	}
	
	// Implementation of DDA with floats
	// Calculate the differences
	float dx = round(x1) - round(x0);
	float dy = round(y1) - round(y0);

	// Determine the number of steps based on the larger delta
	int steps = static_cast<int>(std::max(std::abs(dx), std::abs(dy)));

	// Compute the incremental step size for x and y
	float x_inc = dx / steps;
	float y_inc = dy / steps;

	// Start at the initial point
	float x = static_cast<float>(x0);
	float y = static_cast<float>(y0);

	// Draw the line one step at a time
	for (int i = 0; i <= steps; i++) {
		// Plot the current point
		aSurface.set_pixel_srgb(static_cast<uint32_t>(std::round(x)), static_cast<uint32_t>(std::round(y)), aColor);

		// Increment to the next point
		x += x_inc;
		y += y_inc;
	}

}

void blit_ex_solid( SurfaceEx& aSurface, ImageRGBA const& aImage, Vec2f aPosition )
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
	for (int y = y_start; y < y_end; y++) {
		for (int x = x_start; x < x_end; x++) {
			// Retrieve image pixel
			ColorU8_sRGB_Alpha image_pixel_color = aImage.get_pixel(x, y);

			// Compute surface coordinates
			int surface_x = position_x - half_width_image + x;
			int surface_y = position_y - half_height_image + y;

			// Directly write to the surface
			aSurface.set_pixel_srgb(surface_x, surface_y,
				{ image_pixel_color.r, image_pixel_color.g, image_pixel_color.b });
		}
	}
}

void blit_ex_memcpy(SurfaceEx& aSurface, ImageRGBA const& aImage, Vec2f aPosition)
{
    // Retrieve data 
    int max_height_surface = aSurface.get_height();
    int max_width_surface = aSurface.get_width();
    int max_height_image = aImage.get_height();
    int max_width_image = aImage.get_width();

    // Calculate the top-left position where we should start blitting
    // Subtract half dimensions to center the image at the target position
    int position_x = static_cast<int>(round(aPosition.x - max_width_image / 2.0f));
    int position_y = static_cast<int>(floor(aPosition.y + 1 - max_height_image / 2.0f));

    // Calculate start and end bounds in the image space
    int x_start = std::max(0, -position_x);
    int y_start = std::max(0, -position_y);
    int x_end = std::min(max_width_image, max_width_surface - position_x);
    int y_end = std::min(max_height_image, max_height_surface - position_y);

    // If the image is completely outside the surface bounds, return early
    if (x_start >= x_end || y_start >= y_end) {
        return;
    }

    // Obtain ptr's for image and surface 
    auto const surface_ptr = aSurface.get_surface_ptr();
    auto const image_ptr = aImage.get_image_ptr();

    // Blitting loop
    for (int y = y_start; y < y_end; y++) {
        // Calculate the actual surface y-coordinate
        int surface_y = position_y + y;

        // Calculate the starting memory position in the surface and the image
        auto surface_row = surface_ptr + aSurface.get_linear_index(0, surface_y);
        auto image_row = image_ptr + aImage.get_linear_index(0, y);

        // Calculate number of bytes to copy
        size_t row_size = (x_end - x_start) * sizeof(ColorU8_sRGB_Alpha);

        // Perform memcpy for the row, offsetting the surface position by position_x
        memcpy(surface_row + (position_x + x_start) * sizeof(ColorU8_sRGB_Alpha),
               image_row + x_start * sizeof(ColorU8_sRGB_Alpha),
               row_size);
    }
}

