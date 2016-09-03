#pragma once
#include <ctime>
#include "TGA-Image.h"

//must seed random to use
class Mountain : public TGA::Image
{
private:
	double varianceDecay;

protected:
	int verboseLevel;
	// 0 = no debug output
	// 1 = output % done
	// 2 = output image after each raindrop cast
	// 3 = output image after each raindrop step
	// 4 = output image after each diamond-square step
	unsigned int iteration;

public:
	Mountain(const short& width, const short& height, const string& name, const double& varianceDecay, 
		const int& verboseLevel = 0);

private:
	short randVar(const short& maxVariance) { return maxVariance == 0 ? 0 : rand() % (maxVariance * 2) - maxVariance ; }

	void setPixelIfEmpty(const unsigned short& x, const unsigned short& y, const TGA::Pixel& setTo);

	void diamondSquare(const unsigned short& tlX,//top left
				       const unsigned short& tlY,
				       const unsigned short& brX,//bottom right
				       const unsigned short& brY,
				       const uint8_t& maxVariance
				      );

protected:
	inline void saveIteration() { save(name + "_" + to_string(iteration++)); }
	inline uint8_t getValue(const short& x, const short& y) { return getPixel(x, y).r; }
	inline uint8_t safeAdd(const short& a, const short& b) { return (a + b < 0) ? 0 : (a + b > 0xff) ? 0xff : uint8_t(a + b);; }
};

class ErodedMountain : public Mountain
{
private:
	uint8_t maxSlope;

public:
	friend class Raindrop;

	ErodedMountain(const short& width, const short& height, const string& name, const double& varianceDecay,
		const double& raindropsPerPixel, const uint8_t maxSlope, const int& verboseLevel = 0);

private:
	short safeGetValue(const unsigned short& x, const unsigned short& y);

	bool fillInFrom(unsigned short x, unsigned short y);//returns true if filled
	void fillInBox(short tlX, short tlY, short brX, short brY);
};

class Raindrop
{
private:
	ErodedMountain& parent;
	bool isAlive;
	unsigned short x, y;
	vector<pair<short, short>> trail;
	unsigned short size;

public:
	Raindrop(unsigned short x, unsigned short y, ErodedMountain& parent);

	void step();
	bool isDead() const;

	const unsigned short& getLocationX() const;
	const unsigned short& getLocationY() const;

	//void combineWith(Raindrop target);
};