#include "draw.hpp"

#include <algorithm>

#include <cmath>

#include "surface.hpp"

// Define boundaries for clipping
int CLIP_LEFT = 0;
int CLIP_RIGHT = 0;
int CLIP_BOTTOM = 0;
int CLIP_TOP = 0;


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
int compute_outcode(int x, int y)
{
	int code = INSIDE;
	if (x < CLIP_LEFT) code |= LEFT;
	else if (x > CLIP_RIGHT) code |= RIGHT;
	if (y < CLIP_BOTTOM) code |= BOTTOM;
	else if (y > CLIP_TOP) code |= TOP;
	return code;
}


void draw_line_solid(Surface& aSurface, Vec2f aBegin, Vec2f aEnd, ColorU8_sRGB aColor)
{
	// Set constants according to drawable region	
	CLIP_LEFT = 0;
	CLIP_RIGHT = aSurface.get_width() - 1;
	CLIP_BOTTOM = 0;
	CLIP_TOP = aSurface.get_height() - 1;

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
	int outcode0 = compute_outcode(x0, y0);
	int outcode1 = compute_outcode(x1, y1);

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
				x = x0 + (x1 - x0) * (CLIP_TOP - y0) / (y1 - y0);
				y = CLIP_TOP;
			}
			else if (outcode_out & BOTTOM) { // Point is below the clip window
				x = x0 + (x1 - x0) * (CLIP_BOTTOM - y0) / (y1 - y0);
				y = CLIP_BOTTOM;
			}
			else if (outcode_out & RIGHT) {  // Point is to the right of the clip window
				y = y0 + (y1 - y0) * (CLIP_RIGHT - x0) / (x1 - x0);
				x = CLIP_RIGHT;
			}
			else if (outcode_out & LEFT) {   // Point is to the left of the clip window
				y = y0 + (y1 - y0) * (CLIP_LEFT - x0) / (x1 - x0);
				x = CLIP_LEFT;
			}

			// Replace the outside point with the intersection point and update outcode
			if (outcode_out == outcode0) {
				x0 = x;
				y0 = y;
				outcode0 = compute_outcode(x0, y0);
			}
			else {
				x1 = x;
				y1 = y;
				outcode1 = compute_outcode(x1, y1);
			}
		}
	}

	// Draw the line if it was accepted after clipping
	if (accept) 
	{
		// Implementation of Bresenham’s algorithm (works if line is within drawable region)
		// Calculate difference in x, y directions and step directions
		int dx = abs(x1 - x0);
		int dy = abs(y1 - y0);
		int sx = (x0 < x1) ? 1 : -1;
		int sy = (y0 < y1) ? 1 : -1;

		// Set intial error from true mathematical line
		int err = dx - dy;

		// Construct line
		while (true) 
		{
			aSurface.set_pixel_srgb(x0, y0, aColor);
			if (x0 == x1 && y0 == y1) break;	// Checks for end of line point

			// Checks for direction of line step
			int e2 = 2 * err;
			if (e2 > -dy) { err -= dy; x0 += sx; }
			if (e2 < dx) { err += dx; y0 += sy; }
		}
	}
}

void draw_triangle_wireframe( Surface& aSurface, Vec2f aP0, Vec2f aP1, Vec2f aP2, ColorU8_sRGB aColor )
{
	//TODO: your implementation goes here
	//TODO: your implementation goes here
	//TODO: your implementation goes here

	//TODO: remove the following when you start your implementation
	(void)aSurface; // Avoid warnings about unused arguments until the function
	(void)aP0;   // is properly implemented.
	(void)aP1;
	(void)aP2;
	(void)aColor;
}

void draw_triangle_solid( Surface& aSurface, Vec2f aP0, Vec2f aP1, Vec2f aP2, ColorU8_sRGB aColor )
{
	//TODO: your implementation goes here
	//TODO: your implementation goes here
	//TODO: your implementation goes here

	//TODO: remove the following when you start your implementation
	(void)aSurface; // Avoid warnings about unused arguments until the function
	(void)aP0;   // is properly implemented.
	(void)aP1;
	(void)aP2;
	(void)aColor;
}


ColorF calculate_barycentric_color(Vec2f point, Vec2f p0, Vec2f p1, Vec2f p2, ColorF c0, ColorF c1, ColorF c2)
{
	// Calculate the total area of the triangle using cross product
	float area = (p1.x - p0.x) * (p2.y - p0.y) - (p2.x - p0.x) * (p1.y - p0.y);

	// Guard against division by zero
	if (std::abs(area) < 1e-6f) {
		return c0;
	}

	// Calculate barycentric coordinates
	float w0 = ((p1.x - point.x) * (p2.y - point.y) - (p2.x - point.x) * (p1.y - point.y)) / area;
	float w1 = ((p2.x - point.x) * (p0.y - point.y) - (p0.x - point.x) * (p2.y - point.y)) / area;
	float w2 = 1.0f - w0 - w1;

	// Clamp all weights to [0,1] range and normalize
	w0 = std::clamp(w0, 0.0f, 1.0f);
	w1 = std::clamp(w1, 0.0f, 1.0f);
	w2 = std::clamp(w2, 0.0f, 1.0f);

	// Normalize the weights
	float sum = w0 + w1 + w2;
	if (sum > 0.0f) {
		float invSum = 1.0f / sum;
		w0 *= invSum;
		w1 *= invSum;
		w2 *= invSum;
	}
	else {
		return { 0.0f, 0.0f, 0.0f }; // Return black for points far outside
	}

	// Interpolate color using clamped barycentric coordinates
	ColorF result;
	result.r = w0 * c0.r + w1 * c1.r + w2 * c2.r;
	result.g = w0 * c0.g + w1 * c1.g + w2 * c2.g;
	result.b = w0 * c0.b + w1 * c1.b + w2 * c2.b;
	return result;
}

void draw_horizontal_line(Surface& aSurface, int y, int x_start, int x_end, Vec2f p0, Vec2f p1, Vec2f p2,
	ColorF c0, ColorF c1, ColorF c2)
{
	// Ensure x_start is less than x_end
	if (x_start > x_end) {
		std::swap(x_start, x_end);
	}

	// Calculate the length of the line
	int length = x_end - x_start;
	if (length == 0) {
		// If it's just a single point, draw it and return
		if (x_start >= CLIP_LEFT && x_start <= CLIP_RIGHT &&
			y >= CLIP_BOTTOM && y <= CLIP_TOP) {
			aSurface.set_pixel_srgb(x_start, y, linear_to_srgb(calculate_barycentric_color({ static_cast<float>(x_start), static_cast<float>(y) }, p0, p1, p2, c0, c1, c2)));
		}
		return;
	}

	// Draw each pixel along the line
	for (int x = x_start; x <= x_end; ++x) {
		// Check if pixel is within drawable region
		if (x >= CLIP_LEFT && x <= CLIP_RIGHT && y >= CLIP_BOTTOM && y <= CLIP_TOP) {
			// Create current point vector
			Vec2f point = { static_cast<float>(x), static_cast<float>(y) };

			// Calculate color using barycentric coordinates
			ColorF color = calculate_barycentric_color(point, p0, p1, p2, c0, c1, c2);

			// Set the pixel
			aSurface.set_pixel_srgb(x, y, linear_to_srgb(color));
		}
	}
}

void draw_flat_top_triangle(Surface& aSurface, float x0, float y0, float x1, float y1, float x2, float y2,
	ColorF c0, ColorF c1, ColorF c2)
{
	// Convert integer coordinates to Vec2f
	Vec2f p0(x0, y0);
	Vec2f p1(x1, y1);
	Vec2f p2(x2, y2);

	// Calculate slopes for the edges
	// Slopes represent inverse gradient, if y goes up by 1, x goes up by value
	float inv_slope1 = (x1 - x0) / (y1 - y0);
	float inv_slope2 = (x2 - x0) / (y2 - y0);

	// Starting x-coordinates
	float x_start = x0;
	float x_end = x0;

	// For each scanline
	for (int y = static_cast<int>(y0); y <= static_cast<int>(y1); y++)
	{
		// Calculate start and end points of current scanline
		int x_left = static_cast<int>(round(x_start));
		int x_right = static_cast<int>(round(x_end));

		// Draw the horizontal line
		draw_horizontal_line(aSurface, y, x_left, x_right, p0, p1, p2, c0, c1, c2);

		// Update x coordinates for next scanline
		x_start += inv_slope1;
		x_end += inv_slope2;
	}
}

void draw_flat_bottom_triangle(Surface& aSurface, float x0, float y0, float x1, float y1, float x2, float y2,
	ColorF c0, ColorF c1, ColorF c2)
{
	// Convert integer coordinates to Vec2f
	Vec2f p0(x0, y0);
	Vec2f p1(x1, y1);
	Vec2f p2(x2, y2);

	// Calculate slopes for the edges
	// Slopes represent inverse gradient, if y goes up by 1, x goes up by value
	float inv_slope1 = (x2 - x0) / (y2 - y0);
	float inv_slope2 = (x2 - x1) / (y2 - y1);


	// Starting x-coordinates
	float x_start = x2;
	float x_end = x2;

	// For each scanline
	for (int y = static_cast<int>(y2); y >= static_cast<int>(y0); --y) {

		// Calculate start and end points of current scanline
		int x_left = static_cast<int>(round(x_start));
		int x_right = static_cast<int>(round(x_end));

		// Draw the horizontal line
		draw_horizontal_line(aSurface, y, x_left, x_right, p0, p1, p2, c0, c1, c2);	

		x_start -= inv_slope1;
		x_end -= inv_slope2;
	}
}

void draw_triangle_interp( Surface& aSurface, Vec2f aP0, Vec2f aP1, Vec2f aP2, ColorF aC0, ColorF aC1, ColorF aC2 )
{
	// Set constants according to drawable region	
	CLIP_LEFT = 0;
	CLIP_RIGHT = aSurface.get_width() - 1;
	CLIP_BOTTOM = 0;
	CLIP_TOP = aSurface.get_height() - 1;

	// Extract integer values of the start and end points
	float x0 = round(aP0.x);
	float y0 = round(aP0.y);
	float x1 = round(aP1.x);
	float y1 = round(aP1.y);
	float x2 = round(aP2.x);
	float y2 = round(aP2.y);

	// Similar to the Cohen-Sutherland algorithm
	// We find whether all points of the triangle fall within the drawable region
	// Calculate positions of coordinates relative to drawable region
	int outcode0 = compute_outcode(static_cast<int>(x0), static_cast<int>(y0));
	int outcode1 = compute_outcode(static_cast<int>(x1), static_cast<int>(y1));
	int outcode2 = compute_outcode(static_cast<int>(x2), static_cast<int>(y2));

	// Identify if all points of the triangle lie to one side of the drawable region
	if (outcode0 & outcode1 & outcode2) return;

	// Deal with special case of degenerate triangles
	if ((x0 == x1 && y0 == y1 && (y1 != y2 || x1 != x2)) || (x0 == x2 && y0 == y2) || (x1 == x2 && y1 == y2)) return;

	// Remaining regions may be clipped or fully fall within the drawable region
	// Sort vertices by y-coordinate (and x-coordinate if y-coordinates are the same)
	if (y0 > y1) { std::swap(x0, x1); std::swap(y0, y1); std::swap(aC0, aC1); }
	if (y1 > y2) { std::swap(x1, x2); std::swap(y1, y2); std::swap(aC1, aC2); }
	if (y0 > y1) { std::swap(x0, x1); std::swap(y0, y1); std::swap(aC0, aC1); }

	// Rasterise the triangle in two halves if it’s not already flat-bottomed or flat-topped
	if (y1 == y2) {
		// Flat-top triangle
		draw_flat_top_triangle(aSurface, x0, y0, x1, y1, x2, y2, aC0, aC1, aC2);
	}
	else if (y0 == y1) {
		// Flat-bottom triangle
		draw_flat_bottom_triangle(aSurface, x0, y0, x1, y1, x2, y2, aC0, aC1, aC2);
	}
	else {
		// General triangle, split into flat-top and flat-bottom triangles
		float proportinal_midpoint = (y1 - y0) / (y2 - y0);
		float x_midpoint = x0 + (round(proportinal_midpoint * (x2 - x0)));
		float y3 = y1;
		/*std::printf("prop midpoint: %f\n", proportinal_midpoint);
		std::printf("co ord midpoint: %i, %i\n", x_midpoint, y3);*/

		// Calculate color for the new vertex using barycentric coordinates
		Vec2f p3(x_midpoint, y3);
		ColorF c3 = calculate_barycentric_color(p3, aP0, aP1, aP2, aC0, aC1, aC2);
		

		draw_flat_top_triangle(aSurface, x0, y0, x1, y1, x_midpoint, y1, aC0, aC1, c3);
		draw_flat_bottom_triangle(aSurface, x1, y1, x_midpoint, y1, x2, y2, aC1, c3, aC2);
	}
}




void draw_rectangle_solid( Surface& aSurface, Vec2f aMinCorner, Vec2f aMaxCorner, ColorU8_sRGB aColor )
{
	//TODO: your implementation goes here
	//TODO: your implementation goes here
	//TODO: your implementation goes here

	//TODO: remove the following when you start your implementation
	(void)aSurface; // Avoid warnings about unused arguments until the function
	(void)aMinCorner;   // is properly implemented.
	(void)aMaxCorner;
	(void)aColor;
}

void draw_rectangle_outline( Surface& aSurface, Vec2f aMinCorner, Vec2f aMaxCorner, ColorU8_sRGB aColor )
{
	//TODO: your implementation goes here
	//TODO: your implementation goes here
	//TODO: your implementation goes here

	//TODO: remove the following when you start your implementation
	(void)aSurface; // Avoid warnings about unused arguments
	(void)aMinCorner;
	(void)aMaxCorner;
	(void)aColor;
}
