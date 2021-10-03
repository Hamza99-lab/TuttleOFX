// Copyright 2007 Christian Henning.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/*************************************************************************************************/

#ifndef GIL_HSL_H
#define GIL_HSL_H

////////////////////////////////////////////////////////////////////////////////////////
/// \file
/// \brief Support for HSL color space
/// \author Christian Henning \n
////////////////////////////////////////////////////////////////////////////////////////

#include <boost/gil/color_convert.hpp>
#include <boost/gil.hpp>

namespace boost
{
namespace gil
{

/// \addtogroup ColorNameModel
/// \{
namespace hsl_color_space
{
/// \brief Hue
struct hue_t
{
};
/// \brief Saturation
struct saturation_t
{
};
/// \brief Lightness
struct lightness_t
{
};
}
/// \}

/// \ingroup ColorSpaceModel
typedef mpl::vector3<hsl_color_space::hue_t, hsl_color_space::saturation_t, hsl_color_space::lightness_t> hsl_t;

/// \ingroup LayoutModel
typedef layout<hsl_t> hsl_layout_t;

GIL_DEFINE_ALL_TYPEDEFS(32f, hsl);

/// \ingroup ColorConvert
/// \brief RGB to HSL
template <>
struct default_color_converter_impl<rgb_t, hsl_t>
{
    template <typename P1, typename P2>
    void operator()(const P1& src, P2& dst) const
    {
        using namespace hsl_color_space;

        // only bits32f for hsl is supported
        bits32f temp_red = channel_convert<bits32f>(get_color(src, red_t()));
        bits32f temp_green = channel_convert<bits32f>(get_color(src, green_t()));
        bits32f temp_blue = channel_convert<bits32f>(get_color(src, blue_t()));

        bits32f hue, saturation, lightness;

        bits32f min_color = std::min(temp_red, std::min(temp_green, temp_blue));
        bits32f max_color = std::max(temp_red, std::max(temp_green, temp_blue));

        if(std::abs(max_color - min_color) < 0.001)
        {
            // rgb color is gray
            hue = 0.f;
            saturation = 0.f;

            // doesn't matter which rgb channel we use.
            lightness = temp_red;
        }
        else
        {
            bits32f diff = max_color - min_color;

            // lightness calculation

            lightness = (min_color + max_color) / 2.f;

            // saturation calculation

            if(lightness < 0.5f)
            {
                saturation = diff / (min_color + max_color);
            }
            else
            {
                saturation = (max_color - min_color) / (2.f - diff);
            }

            // hue calculation
            if(std::abs(max_color - temp_red) < 0.0001f)
            {
                // max_color is red
                hue = (temp_green - temp_blue) / diff;
            }
            else if(std::abs(max_color - temp_green) < 0.0001f)
            {
                // max_color is green
                // 2.0 + (b - r) / (maxColor - minColor);
                hue = 2.f + (temp_blue - temp_red) / diff;
            }
            else
            {
                // max_color is blue
                hue = 4.f + (temp_red - temp_blue) / diff;
            }

            hue /= 6.f;

            if(hue < 0.f)
            {
                hue += 1.f;
            }
        }

        get_color(dst, hue_t()) = hue;
        get_color(dst, saturation_t()) = saturation;
        get_color(dst, lightness_t()) = lightness;
    }
};

/// \ingroup ColorConvert
/// \brief HSL to RGB
template <>
struct default_color_converter_impl<hsl_t, rgb_t>
{
    template <typename P1, typename P2>
    void operator()(const P1& src, P2& dst) const
    {
        using namespace hsl_color_space;

        bits32f red, green, blue;

        if(std::abs(get_color(src, saturation_t())) < 0.0001)
        {
            // If saturation is 0, the color is a shade of gray
            red = green = blue = get_color(src, lightness_t());
        }
        else
        {
            float temp1, temp2;
            float tempr, tempg, tempb;

            // Set the temporary values
            if(get_color(src, lightness_t()) < 0.5)
            {
                temp2 = get_color(src, lightness_t()) * (1.f + get_color(src, saturation_t()));
            }
            else
            {
                temp2 = (get_color(src, lightness_t()) + get_color(src, saturation_t())) -
                        (get_color(src, lightness_t()) * get_color(src, saturation_t()));
            }

            temp1 = 2.f * get_color(src, lightness_t()) - temp2;

            tempr = get_color(src, hue_t()) + 1.f / 3.f;

            if(tempr > 1.f)
            {
                tempr--;
            }

            tempg = get_color(src, hue_t());
            tempb = get_color(src, hue_t()) - 1.f / 3.f;

            if(tempb < 0.f)
            {
                tempb++;
            }

            // Red
            if(tempr < 1.f / 6.f)
            {
                red = temp1 + (temp2 - temp1) * 6.f * tempr;
            }
            else if(tempr < 0.5f)
            {
                red = temp2;
            }
            else if(tempr < 2.f / 3.f)
            {
                red = temp1 + (temp2 - temp1) * ((2.f / 3.f) - tempr) * 6.f;
            }
            else
            {
                red = temp1;
            }

            // Green
            if(tempg < 1.f / 6.f)
            {
                green = temp1 + (temp2 - temp1) * 6.f * tempg;
            }
            else if(tempg < 0.5f)
            {
                green = temp2;
            }
            else if(tempg < 2.f / 3.f)
            {
                green = temp1 + (temp2 - temp1) * ((2.f / 3.f) - tempg) * 6.f;
            }
            else
            {
                green = temp1;
            }

            // Blue
            if(tempb < 1.f / 6.f)
            {
                blue = temp1 + (temp2 - temp1) * 6.f * tempb;
            }
            else if(tempb < 0.5f)
            {
                blue = temp2;
            }
            else if(tempb < 2.f / 3.f)
            {
                blue = temp1 + (temp2 - temp1) * ((2.f / 3.f) - tempb) * 6.f;
            }
            else
            {
                blue = temp1;
            }
        }

        get_color(dst, red_t()) = channel_convert<typename color_element_type<P2, red_t>::type>(red);
        get_color(dst, green_t()) = channel_convert<typename color_element_type<P2, green_t>::type>(green);
        get_color(dst, blue_t()) = channel_convert<typename color_element_type<P2, blue_t>::type>(blue);
    }
};

/// \ingroup ColorConvert
///  \brief Converting HSL to any pixel type. Note: Supports homogeneous pixels only.
///
/// Done by an intermediate RGB conversion
template <typename C2>
struct default_color_converter_impl<hsl_t, C2>
{
    template <typename P1, typename P2>
    void operator()(const P1& src, P2& dst) const
    {
        typedef hsl_t C1;
        typedef typename channel_type<P1>::type T1;
        typedef typename channel_type<P2>::type T2;
        pixel<T2, rgb_layout_t> tmp;
        default_color_converter_impl<C1, rgb_t>()(src, tmp);
        default_color_converter_impl<rgb_t, C2>()(tmp, dst);
    }
};

/// \ingroup ColorConvert
/// \brief Unfortunately HSL to HSL must be explicitly provided - otherwise we get ambiguous specialization error.
template <>
struct default_color_converter_impl<hsl_t, hsl_t>
{
    template <typename P1, typename P2>
    void operator()(const P1& src, P2& dst) const
    {
        static_for_each(src, dst, default_channel_converter());
    }
};
}
} // namespace boost::gil

#endif // GIL_HSL_H
