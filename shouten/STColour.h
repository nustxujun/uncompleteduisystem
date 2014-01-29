#ifndef _STColour_H_
#define _STColour_H_

#include "STCommon.h"

namespace ST
{
	typedef unsigned int ARGB;

	class ShoutenExport Colour
	{
	public:
		static const Colour BLACK;
		static const Colour ZERO;
	public :
		Colour();
		Colour(ARGB c);
		Colour(float red, float green, float blue, float alpha = 1.0f);

		Colour operator * (float val)const;


		void setFromARGB(ARGB c);
		ARGB getAsARGB()const;

	public:
		float r, g, b, a;
	};
}

#endif