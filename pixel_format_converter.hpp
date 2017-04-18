#ifndef PIXELFORMATCONVERTER_HPP
#define PIXELFORMATCONVERTER_HPP


#include <vector>


using YuyvPixelVec = std::vector< unsigned char >;
using RgbPixelVec = std::vector< unsigned char >;


class PixelFormatConverter
{

public:

    /**
     * @brief rgbFromYuyv
     * Converts YUV 4:2:2 (YUY2, YUYV) to RGB format
     * @param yuyvPixels YUYV pixels to convert
     * @return Pixel vector in RGB
     */
    static RgbPixelVec rgbFromYuyv( const YuyvPixelVec & );

};


#endif // PIXELFORMATCONVERTER_HPP
