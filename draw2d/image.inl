inline
ColorU8_sRGB_Alpha ImageRGBA::get_pixel( Index aX, Index aY ) const
{
	assert( aX < mWidth && aY < mHeight ); // Leave this at the top of the function.

    // Get a pointer to the image data
    const uint8_t* image_ptr = this->get_image_ptr();

    // Calculate the linear index for the pixel at (aX, aY)
    Index linear_index = this->get_linear_index(aX, aY);

    // Return the ColorU8_sRGB_Alpha struct with the pixel values
    return ColorU8_sRGB_Alpha{
        image_ptr[linear_index],     // Red
        image_ptr[linear_index + 1], // Green
        image_ptr[linear_index + 2], // Blue
        image_ptr[linear_index + 3]  // Alpha
    };
}

inline
auto ImageRGBA::get_width() const noexcept -> Index
{
	return mWidth;
}
inline
auto ImageRGBA::get_height() const noexcept -> Index
{
	return mHeight;
}

inline
std::uint8_t* ImageRGBA::get_image_ptr() noexcept
{
	return mData;
}
inline
std::uint8_t const* ImageRGBA::get_image_ptr() const noexcept
{
	return mData;
}

inline
ImageRGBA::Index ImageRGBA::get_linear_index( Index aX, Index aY ) const noexcept
{
	// Compute the linear index
	return (aY * mWidth + aX) * 4;
}
