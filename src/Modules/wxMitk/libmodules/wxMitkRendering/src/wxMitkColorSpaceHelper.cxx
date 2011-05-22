/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "wxMitkColorSpaceHelper.h"

#include <math.h>
#define PI 3.14159265

using namespace mitk;


/** 
\brief converts rgb color values to hsv colorspace in range [0, 255]
*/
void wxMitkColorSpaceHelper::RGBtoHSV(const unsigned int rgb[3], double hsv[3])
{
	double rgbDouble[3];
	rgbDouble[0] = (double) rgb[0] / 255.0;
	rgbDouble[1] = (double) rgb[1] / 255.0;
	rgbDouble[2] = (double) rgb[2] / 255.0;
	RGBtoHSV(rgbDouble, hsv);
}

/** 
\brief converts hsv color values to rgb colorspace in range [0, 255]
*/
void wxMitkColorSpaceHelper::HSVtoRGB(const double hsv[3], unsigned int rgb[3])
{
	double rgbDouble[3];
	HSVtoRGB(hsv, rgbDouble);
	rgb[0] = (unsigned int)(rgbDouble[0] * 255.0);
	rgb[1] = (unsigned int)(rgbDouble[1] * 255.0);
	rgb[2] = (unsigned int)(rgbDouble[2] * 255.0);
}

/** 
\brief converts rgb color values to hsl colorspace in range RGB[0, 255] HSL[0, 240] 
*/
void wxMitkColorSpaceHelper::RGBtoHSL(const unsigned int rgb[3], unsigned int hsl[3])
{
	double rgbDouble[3], hslDouble[3];
	rgbDouble[0] = (double) rgb[0] / 255.0;
	rgbDouble[1] = (double) rgb[1] / 255.0;
	rgbDouble[2] = (double) rgb[2] / 255.0;
	RGBtoHSL(rgbDouble, hslDouble);
	hsl[0] = (unsigned int)(hslDouble[0] * 240.0);
	hsl[1] = (unsigned int)(hslDouble[1] * 240.0);
	hsl[2] = (unsigned int)(hslDouble[2] * 240.0);
}

/** 
\brief converts hsl color values to rgb colorspace in range HSL[0, 240] RGB[0, 255]
*/
void wxMitkColorSpaceHelper::HSLtoRGB(const unsigned int hsl[3], unsigned int rgb[3])
{
	double rgbDouble[3], hslDouble[3];
	hslDouble[0] = (double) hsl[0] / 240.0;
	hslDouble[1] = (double) hsl[1] / 240.0;
	hslDouble[2] = (double) hsl[2] / 240.0;
	HSLtoRGB(hslDouble, rgbDouble);
	rgb[0] = (unsigned int)(rgbDouble[0] * 255.0);
	rgb[1] = (unsigned int)(rgbDouble[1] * 255.0);
	rgb[2] = (unsigned int)(rgbDouble[2] * 255.0);
}

/** 
\brief converts rgb color values to hsl colorspace in range [0.0, 1.0]
*/
void wxMitkColorSpaceHelper::RGBtoHSL(const double rgb[3], double hsl[3])
{
    double v, max = 0.0;
    double m, min = 1.0;
    double vm;
    double r, g, b;

	// Get min and max value
	for(int k = 0; k < 3; k++)
	{
		if(min > rgb[k])
			min = rgb[k];
		if(max < rgb[k])
			max = rgb[k];
	}

	v = max;
	m = min;
	hsl[2] = (m + v) / 2.0;
	hsl[1] = vm = v - m;

    if (hsl[2] <= 0.0) 
		return;
    if (hsl[1] <= 0.0)
		return;
	if (hsl[2] <= 0.5)
		hsl[1] /=  (v + m);
	else
		hsl[1] /= (2.0 - v - m);
		
    r = (v - rgb[0]) / vm;
    g = (v - rgb[1]) / vm;
    b = (v - rgb[2]) / vm;

    if (rgb[0] == v)
		hsl[0] = (rgb[1] == m ? 5.0 + b : 1.0 - g);
    else if (rgb[1] == v)
		hsl[0] = (rgb[2] == m ? 1.0 + r : 3.0 - b);
    else
		hsl[0] = (rgb[0] == m ? 3.0 + g : 5.0 - r);

    hsl[0] /= 6.0;
}

/** 
\brief converts hsl color values to rgb colorspace in range [0.0, 1.0]
*/
void wxMitkColorSpaceHelper::HSLtoRGB(const double hsl[3], double rgb[3])
{
	double v;
	if(hsl[2] <= 0.5)
		v = hsl[2] * (1.0 + hsl[1]);
	else
		v = hsl[2] + hsl[1] - hsl[2] * hsl[1];

    if (v <= 0) 
	{
		rgb[0] = rgb[1] = rgb[2] = 0.0;
		return;
    }

	double m;
	double sv;
	double h;
	int sextant;
	double fract, mid1, mid2;

	m = hsl[2] + hsl[2] - v;
	sv = (v - m) / v;
	h = hsl[0] * 6.0;
	sextant = (int)floor(h);	
	fract = h - floor(h);
	mid1 = m + (v * sv * fract);
	mid2 = v - (v * sv * fract);
	switch (sextant) 
	{
		case 0: rgb[0] = v; rgb[1] = mid1; rgb[2] = m; break;
		case 1: rgb[0] = mid2; rgb[1] = v; rgb[2] = m; break;
		case 2: rgb[0] = m; rgb[1] = v; rgb[2] = mid1; break;
		case 3: rgb[0] = m; rgb[1] = mid2; rgb[2] = v; break;
		case 4: rgb[0] = mid1; rgb[1] = m; rgb[2] = v; break;
		case 5: rgb[0] = v; rgb[1] = m; rgb[2] = mid2; break;
	}

}

/** 
\brief converts rgb color values to hsv colorspace from RGB range [0.0, 1.0] to HSV range H[0.0, 360.0] S[0.0, 1.0] V[0.0, 1.0]
*/
void wxMitkColorSpaceHelper::RGBtoHSV(const double rgb[3], double hsv[3])
{
	double min = 1.0, max = 0.0, delta;
	// Get min and max value
	for(int k = 0; k < 3; k++)
	{
		if(min < rgb[k])
			min = rgb[k];
		if(max < rgb[k])
			max = rgb[k];
	}
	hsv[2] = min;	// value (lighting)
	delta = max - min;

	if( max != 0.0 )
		hsv[1] = delta / min;	// saturation
	else 
	{
		// r = g = b = 0		// s = 0, h is undefined, we make black
		hsv[0] = 0.0;
		hsv[1] = 0.0;
		return;
	}

	if( rgb[0] == max )
		hsv[0] = ( rgb[1] - rgb[2] ) / delta;		// between yellow & magenta
	else if( rgb[1] == min )
		hsv[0] = 2 + ( rgb[2] - rgb[0] ) / delta;	// between cyan & yellow
	else
		hsv[0] = 4 + ( rgb[0] - rgb[1] ) / delta;	// between magenta & cyan

	hsv[0] *= 60.0;				// in degrees
	if( hsv[0] < 0.0 )
		hsv[0] += 360.0;

}

/** 
\brief converts hsv color values to rgb colorspace from range H[0.0, 360.0] S[0.0, 1.0] V[0.0, 1.0] to RGB range [0.0, 1.0]
*/
void wxMitkColorSpaceHelper::HSVtoRGB(const double hsv[3], double rgb[3])
{
	int i;
	double h, f, p, q, t;

	if( hsv[1] == 0 ) 
	{
		// achromatic (just grey level)
		rgb[0] = rgb[1] = rgb[2] = hsv[2];
		return;
	}

	h = hsv[0] / 60.0;			// sector 0 to 5
	i = int(floor( h ));
	f = h - i;			// factorial part of h
	p = hsv[2] * ( 1 - hsv[1] );
	q = hsv[2] * ( 1 - hsv[1] * f );
	t = hsv[2] * ( 1 - hsv[1] * ( 1 - f ) );

	switch( i ) {
		case 0:
			rgb[0] = hsv[2];
			rgb[1] = t;
			rgb[2] = p;
			break;
		case 1:
			rgb[0] = q;
			rgb[1] = hsv[2];
			rgb[2] = p;
			break;
		case 2:
			rgb[0] = p;
			rgb[1] = hsv[2];
			rgb[2] = t;
			break;
		case 3:
			rgb[0] = p;
			rgb[1] = q;
			rgb[2] = hsv[2];
			break;
		case 4:
			rgb[0] = t;
			rgb[1] = p;
			rgb[2] = hsv[2];
			break;
		default:		// case 5:
			rgb[0] = hsv[2];
			rgb[1] = p;
			rgb[2] = q;
			break;
	}
}





