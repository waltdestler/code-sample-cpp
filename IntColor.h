#pragma once

/**
 * Stores an rgba color as a 32-bit integer.
 * Each component has 8 bits.
 */
class IntColor
{
private:
	
	unsigned int32 _color;
	
public:
	
	IntColor();
	IntColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a=0);
	IntColor(int32 color);
	
	unsigned char r();
	unsigned char g();
	unsigned char b();
	unsigned char a();
	
	void setR(unsigned char r);
	void setG(unsigned char g);
	void setB(unsigned char b);
	void setA(unsigned char a);
	
	operator int32();
};
