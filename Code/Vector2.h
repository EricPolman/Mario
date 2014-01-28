#pragma once
#include <math.h>


enum Direction{DOWN,RIGHT,UP,LEFT};
enum CollisionType{FULL, NONE, HALF_LEFT, HALF_RIGHT, HALF_TOP, HALF_BOTTOM, BOOMERANGABLE};

class Vector2{
public: 
	Vector2(){x = 0; y = 0;}
	Vector2(const Vector2& a_vec){x = a_vec.x; y = a_vec.y;}
	Vector2(float a_fX, float a_fY){x = a_fX; y = a_fY;}
	float x; 
	float y;
	void Normalize(){
		if(x != 0 || y != 0)
		{
			float mag = sqrt(x*x + y*y);
			x /= mag; 
			y /= mag;
		}
	}
	float Magnitude(){return sqrt(x*x + y*y);}
	float Distance(Vector2 a_vec){float distX = x - a_vec.x; float distY = y - a_vec.y;return sqrt(distX*distX + distY*distY);}
	float DistanceSquared(Vector2 a_vec){return (x - a_vec.x)*(x - a_vec.x) + (y - a_vec.y)*(y - a_vec.y);}
	Vector2 operator+(Vector2 a_vec){return Vector2(x + a_vec.x, y + a_vec.y);}
	Vector2 operator-(Vector2 a_vec){return Vector2(x - a_vec.x, y - a_vec.y);}
	Vector2 operator*(float a_scalar){return Vector2(x * a_scalar, y * a_scalar);}
	Vector2 operator-(){return Vector2(-x, -y);}
	std::string ToString()
	{
		std::stringstream ss;		
		ss << "{ " << x << ", " << y << " }";
		return ss.str();
	}
};


class Vector2i
{
public:
	Vector2i(int a_x, int a_y){x = a_x; y = a_y;}
	Vector2i(){x = 0; y = 0;}
	int x;
	int y;
};

class CollisionInfo{
public:
	CollisionInfo(){isColliding = false;}
	
	Vector2 depth;
	bool isColliding;
};

class CollisionRectangle
{
public:
	CollisionRectangle(float a_x, float a_y, float a_width, float a_height)
	{
		x = a_x;
		y = a_y;
		width = a_width;
		height = a_height;
	}
	CollisionRectangle(){x = 0; y = 0; width = 0; height = 0;}
	float x;
	float y;
	float width;
	float height;

	CollisionInfo Intersects(const CollisionRectangle& a_other);

	std::string ToString()
	{
		std::stringstream ss;		
		ss << "{ " << x << ", " << y << ", " << width << ", " << height << " }";
		return ss.str();
	}
};


static std::vector<std::string> explode(std::string const & s, char delim)
{
    std::vector<std::string> result;
    std::istringstream iss(s);

    for (std::string token; std::getline(iss, token, delim); )
    {
        result.push_back(std::move(token));
    }

    return result;
};


/* To be able to do the Hue-shifting, I found this code on stackoverflow
http://stackoverflow.com/questions/3018313/algorithm-to-convert-rgb-to-hsv-and-hsv-to-rgb */
typedef struct RgbColor
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
} RgbColor;

typedef struct HsvColor
{
    unsigned char h;
    unsigned char s;
    unsigned char v;
} HsvColor;

RgbColor HsvToRgb(HsvColor hsv);

HsvColor RgbToHsv(RgbColor rgb);