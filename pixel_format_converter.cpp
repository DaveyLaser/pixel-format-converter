#include <array>

#include "PixelFormatConverter.hpp"

constexpr int RGB_MIN = 0;
constexpr int RGB_MAX = 255;
constexpr int Y_SCALAR = 298;
constexpr int V_SCALAR_FOR_R = 409;
constexpr int U_SCALAR_FOR_G = 100;
constexpr int V_SCALAR_FOR_G = 208;
constexpr int U_SCALAR_FOR_B = 516;
constexpr int BIT_OFFSET = 128;
constexpr int Y_ADJUST = 16;
constexpr int U_ADJUST = 128;
constexpr int V_ADJUST = 128;


int rgbClip( int value )
{
  if( value < RGB_MIN )
  {
      value = RGB_MIN;
  }

  if( value > RGB_MAX )
  {
      value = RGB_MAX;
  }

  return value;
}


int rFromYv( int yBits, int vBits )
{
    return ( Y_SCALAR * yBits +
             V_SCALAR_FOR_R * vBits +
             BIT_OFFSET) >> 8;
}


int gFromYuv( int yBits, int uBits, int vBits )
{
    return ( Y_SCALAR * yBits -
             U_SCALAR_FOR_G * uBits -
             V_SCALAR_FOR_G * vBits +
             BIT_OFFSET ) >> 8;
}


int bFromYu( int yBits, int uBits )
{
    return ( Y_SCALAR * yBits +
             U_SCALAR_FOR_B * uBits +
             BIT_OFFSET ) >> 8;
}


std::array< int, 3 > rgbBitsFromYuvBits(
        const std::array< int, 3 > & yuvBits )
{
    return { rgbClip( rFromYv( yuvBits[ 0 ], yuvBits[ 2 ] ) ),
             rgbClip( gFromYuv( yuvBits[ 0 ], yuvBits[ 1 ], yuvBits[ 2 ] ) ),
             rgbClip( bFromYu( yuvBits[ 0 ], yuvBits[ 1 ] ) ) };
}


void appendRgbPixels(
        std::vector< unsigned char > & rgbPixels,
        const std::array< int, 3 > & yuvBits  )
{
    auto rgbBits = rgbBitsFromYuvBits( yuvBits );

    rgbPixels.insert( rgbPixels.end(), rgbBits.begin(), rgbBits.end() );
}


std::array< unsigned char, 4 > yuyvPixelsFromArray(
        const unsigned char * yuyvPixelArray )
{
    return { yuyvPixelArray[ 0 ],
             yuyvPixelArray[ 1 ],
             yuyvPixelArray[ 2 ],
             yuyvPixelArray[ 3 ] };
}


std::array< int, 4 > yuyvBitsFromPixels(
        const std::array< unsigned char, 4 > & pixels )
{
    return { pixels[ 0 ] - Y_ADJUST,
             pixels[ 1 ] - U_ADJUST,
             pixels[ 2 ] - Y_ADJUST,
             pixels[ 3 ] - V_ADJUST };
}


RgbPixelVec PixelFormatConverter::rgbFromYuyv( const YuyvPixelVec & yuyvPixels )
{
    RgbPixelVec rgbPixels;

    for ( auto i = 0u; i < yuyvPixels.size(); i += 4 )
    {
        auto yuyvBits =
                yuyvBitsFromPixels( yuyvPixelsFromArray( &yuyvPixels[ i ] ) );

        appendRgbPixels( rgbPixels,
                         { yuyvBits[ 0 ], yuyvBits[ 1 ], yuyvBits[ 3 ] } );

        appendRgbPixels( rgbPixels,
                         { yuyvBits[ 2 ], yuyvBits[ 1 ], yuyvBits[ 3 ] } );
    }

    return rgbPixels;
}
