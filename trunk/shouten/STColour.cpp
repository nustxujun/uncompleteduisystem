#include "STColour.h"

using namespace ST;

const Colour Colour::BLACK(0xff000000);
const Colour Colour::ZERO(0);

Colour::Colour() :
r(0.f), g(0.f), b(0.f), a(0.f)
{
}

Colour::Colour(ARGB c)
{
	setFromARGB(c);
}

Colour::Colour(float red, float green, float blue, float alpha ):
r(red), g(green), b(blue), a(alpha)
{}

Colour Colour::operator * (float val)const
{
	Colour c;
	c.r = val * r;
	c.g = val * g;
	c.b = val * b;
	c.a = val * a;
	return c;
}



void Colour::setFromARGB(ARGB c)
{
	const int mask = 0xff;
	const float inv = 1.0f / 255.0f;
	a = ( c >> 24) * inv;
	r = ((c >> 16) & mask) * inv;
	g = ((c >> 8) & mask) * inv;
	b = ( c & mask) * inv;
}

ARGB Colour::getAsARGB()const
{

	ARGB ret = ((ARGB)(a * 255) << 24) + 
				((ARGB)(r * 255) << 16) + 
				((ARGB)(g * 255) << 8) + 
				((ARGB)(b * 255));

	return ret;
}

