#include "IntColor.h"

/**
 * Creates a new black/transparent color.
 */
IntColor::IntColor()
{
	_color = 0;
}

/**
 * Creates a new color with the specified rgba color channels.
 */
IntColor::IntColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	setR(r);
	setG(g);
	setB(b);
	setA(a);
}

/**
 * Creates a new color from the specified 32-bit integer.
 */
IntColor::IntColor(int32 color)
{
	_color = color;
}

/**
 * Returns the red component of this color.
 */
unsigned char IntColor::r()
{
	return (_color >> 24) & 0xff;
}

/**
 * Returns the green component of this color.
 */
unsigned char IntColor::g()
{
	return (_color >> 16) & 0xff;
}

/**
 * Returns the blue component of this color.
 */
unsigned char IntColor::b()
{
	return (_color >> 8) & 0xff;
}

/**
 * Returns the alpha component of this color.
 */
unsigned char IntColor::a()
{
	return _color & 0xff;
}

/**
 * Sets the red component of this color.
 */
void IntColor::setR(unsigned char r)
{
	_color = (_color & 0x00ffffff) | (r << 24);
}

/**
 * Sets the green component of this color.
 */
void IntColor::setG(unsigned char g)
{
	_color = (_color & 0xff00ffff) | (g << 16);
}

/**
 * Sets the blue component of this color.
 */
void IntColor::setB(unsigned char b)
{
	_color = (_color & 0xffff00ff) | (b << 8);
}

/**
 * Sets the alpha component of this color.
 */
void IntColor::setA(unsigned char a)
{
	_color = (_color & 0xffffff00) | a;
}

/**
 * Returns this color as a 32-bit integer.
 */
IntColor::operator int32()
{
	return _color;
}