#include "includes.h"
#include "Vector2.h"

CollisionInfo CollisionRectangle::Intersects(const CollisionRectangle& a_other)
{
	CollisionInfo info;

	if(y >= a_other.y + a_other.height
		|| y + height <= a_other.y
		|| x >= a_other.x + a_other.width
		|| x + width <= a_other.x)
	{ return info;}
	//Now we know there is a collision.

	if(x + width > a_other.x)
	{
		info.depth.x = a_other.x - (x + width);
	}
		
	if(x < a_other.x + a_other.width)
	{
		if(!info.depth.x || (info.depth.x && abs((a_other.x + a_other.width) - x) < abs(info.depth.x)))
			info.depth.x = (a_other.x + a_other.width) - x;
	}

	if(y + height > a_other.y)
	{
		info.depth.y = a_other.y - (y + height);
	}

	if(y < a_other.y + a_other.height)
	{
		if(!info.depth.y || (info.depth.y && abs((a_other.y + a_other.height) - y) < abs(info.depth.y)))
			info.depth.y = (a_other.y + a_other.height) - y;
	}
	if(info.depth.x != 0 && info.depth.y != 0)
		info.isColliding = true;

	return info;
}


RgbColor HsvToRgb(HsvColor hsv)
{
    RgbColor rgb;
    unsigned char region, remainder, p, q, t;

    if (hsv.s == 0)
    {
        rgb.r = hsv.v;
        rgb.g = hsv.v;
        rgb.b = hsv.v;
        return rgb;
    }

    region = hsv.h / 43;
    remainder = (hsv.h - (region * 43)) * 6; 

    p = (hsv.v * (255 - hsv.s)) >> 8;
    q = (hsv.v * (255 - ((hsv.s * remainder) >> 8))) >> 8;
    t = (hsv.v * (255 - ((hsv.s * (255 - remainder)) >> 8))) >> 8;

    switch (region)
    {
        case 0:
            rgb.r = hsv.v; rgb.g = t; rgb.b = p;
            break;
        case 1:
            rgb.r = q; rgb.g = hsv.v; rgb.b = p;
            break;
        case 2:
            rgb.r = p; rgb.g = hsv.v; rgb.b = t;
            break;
        case 3:
            rgb.r = p; rgb.g = q; rgb.b = hsv.v;
            break;
        case 4:
            rgb.r = t; rgb.g = p; rgb.b = hsv.v;
            break;
        default:
            rgb.r = hsv.v; rgb.g = p; rgb.b = q;
            break;
    }

    return rgb;
}

HsvColor RgbToHsv(RgbColor rgb)
{
    HsvColor hsv;
    unsigned char rgbMin, rgbMax;

    rgbMin = rgb.r < rgb.g ? (rgb.r < rgb.b ? rgb.r : rgb.b) : (rgb.g < rgb.b ? rgb.g : rgb.b);
    rgbMax = rgb.r > rgb.g ? (rgb.r > rgb.b ? rgb.r : rgb.b) : (rgb.g > rgb.b ? rgb.g : rgb.b);

    hsv.v = rgbMax;
    if (hsv.v == 0)
    {
        hsv.h = 0;
        hsv.s = 0;
        return hsv;
    }

    hsv.s = 255 * long(rgbMax - rgbMin) / hsv.v;
    if (hsv.s == 0)
    {
        hsv.h = 0;
        return hsv;
    }

    if (rgbMax == rgb.r)
        hsv.h = 0 + 43 * (rgb.g - rgb.b) / (rgbMax - rgbMin);
    else if (rgbMax == rgb.g)
        hsv.h = 85 + 43 * (rgb.b - rgb.r) / (rgbMax - rgbMin);
    else
        hsv.h = 171 + 43 * (rgb.r - rgb.g) / (rgbMax - rgbMin);

    return hsv;
}
