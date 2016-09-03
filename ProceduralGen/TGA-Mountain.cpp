#include "TGA-Mountain.h"
#include <iostream>

Mountain::Mountain(const short& width, const short& height, const string& name, const double& varianceDecay, const int& verboseLevel)
	: TGA::Image(width, height, name), varianceDecay(varianceDecay), verboseLevel(verboseLevel)
{

	//set seed corners
	setPixel(0        , 0         , TGA::value(0xff / 2));
	setPixel(width - 1, 0         , TGA::value(0xff / 2));
	setPixel(0        , height - 1, TGA::value(0xff / 2));
	setPixel(width - 1, height - 1, TGA::value(0xff / 2));

	//init itteration
	iteration = 0;

	//run the diamond square algorithm on the image
	diamondSquare(0, 0, width - 1, height - 1, 0xff/2);
}

void Mountain::setPixelIfEmpty(const unsigned short& x, const unsigned short& y, const TGA::Pixel& setTo)
{
	if (TGA::isBlack(getPixel(x, y)))
	{
		setPixel(x, y, setTo);
	}
}

void Mountain::diamondSquare(
	const unsigned short& tlX, const unsigned short& tlY, //top left
	const unsigned short& brX, const unsigned short& brY, //bottom right
	const uint8_t& maxVariance
	)
{
	if (((brX - tlX) < 2 && (brY - tlY) < 2))//if area is too small to be operated on
	{
		return;
	}

	if (verboseLevel >= 4)
	{
		saveIteration();
	}
	

	//ger corner values for points to be used in average calculations
	const uint8_t& tlValue = getValue(tlX, tlY);
	const uint8_t& trValue = getValue(brX, tlY);
	const uint8_t& blValue = getValue(tlX, brY);
	const uint8_t& brValue = getValue(brX, brY);

	//diamond step
	//get average of square points plus a random one
	uint8_t averageValue = (
		tlValue + 
		trValue + 
		blValue + 
		brValue
		) / 4;
	//set TGA::value of center +/- random variance
	setPixel((brX - tlX) / 2 + tlX, (brY - tlY) / 2 + tlY, TGA::value(safeAdd(averageValue, randVar(maxVariance))));

	//square step
	//diamond edges are the same value so no need to recalculate
	//set diamond centers
	short halfWidth  = (brX - tlX)/2;
	short halfHeight = (brY - tlY)/2;

	//set values for points to be used in average calculations
	uint8_t& middleValue = averageValue; //averageValue == middle value

	//get averages for each diamond edge
	setPixelIfEmpty(tlX + halfWidth, tlY             , TGA::value(safeAdd((tlValue + trValue + middleValue )/3, randVar(maxVariance))));//Top side
	setPixelIfEmpty(brX            , tlY + halfHeight, TGA::value(safeAdd((trValue + brValue + middleValue )/3, randVar(maxVariance))));//right side
	setPixelIfEmpty(tlX + halfWidth, brY             , TGA::value(safeAdd((brValue + blValue + middleValue )/3, randVar(maxVariance))));//bottom side
	setPixelIfEmpty(tlX            , tlY + halfHeight, TGA::value(safeAdd((blValue + tlValue + middleValue )/3, randVar(maxVariance))));//left side
	
	//run again on each sub-quadrent
	diamondSquare(tlX            , tlY             , tlX + halfWidth, tlY + halfHeight, maxVariance / varianceDecay);
	diamondSquare(tlX + halfWidth, tlY             , brX            , tlY + halfHeight, maxVariance / varianceDecay);
	diamondSquare(tlX            , tlY + halfHeight, tlX + halfWidth, brY             , maxVariance / varianceDecay);
	diamondSquare(tlX + halfWidth, tlY + halfHeight, brX            , brY             , maxVariance / varianceDecay);
}

ErodedMountain::ErodedMountain(const short& width, const short& height, const string& name, const double& varianceDecay,
	const double& raindropsPerPixel, const uint8_t maxSlope, const int& verboseLevel)
	:Mountain(width, height, name, varianceDecay, verboseLevel), maxSlope(maxSlope)
{
	// Initial fill in
	fillInBox(0, 0, width - 1, height - 1);

	int raindrops = raindropsPerPixel*width*height;

	for (int i = 0; i < raindrops; ++i)
	{
		for (Raindrop raindrop(rand() % width, rand() % height, *this); !raindrop.isDead(); raindrop.step());

		if (verboseLevel >= 1 && i % int(raindrops / 100) == 0)
		{
			cout << "cast raindrop " << i << " ( " << 100*i/(double)raindrops << "% )" << endl;

			if (verboseLevel == 2)
			{
				saveIteration();
			}
		}
	}
}

short ErodedMountain::safeGetValue(const unsigned short& x, const unsigned short& y)
{
	if (x >= 0 && x < width && y >= 0 && y < height)
	{
		return getValue(x, y);
	}
	else
	{
		return 0xff;
	}
}

bool ErodedMountain::fillInFrom(unsigned short x, unsigned short y)
{
	uint8_t lowestValue = getValue(x, y);
	short lowX = -1;
	short lowY = -1;

	for (int dy = -1; dy <= 1; ++dy)
	{
		for (int dx = -1; dx <= 1; ++dx)
		{
			uint8_t currentValue = safeGetValue(x + dx, y + dy);
			if (currentValue < lowestValue)
			{
				lowestValue = currentValue;

				lowX = x + dx;
				lowY = y + dy;
			}
		}
	}

	uint8_t highValue = getValue(x, y);

	if (highValue > maxSlope + lowestValue)
	{
		uint8_t difference = highValue - lowestValue - maxSlope;

		setPixel(lowX, lowY, TGA::value(lowestValue + difference));
		setPixel(x   , y   , TGA::value(highValue   - difference));

		return true;
	}

	return false;
}

void ErodedMountain::fillInBox(short tlX, short tlY, short brX, short brY)
{
	bool eroded = false;

	for (short y = max(int(tlY), 0); y < min(int(brY), width - 1); ++y)
	{
		for (short x = max(int(tlX), 0); x < min(int(brX), width - 1); ++x)
		{
			if (fillInFrom(x, y))
			{
				eroded = true;
			}
		}
	}

	if (eroded)
	{
		fillInBox(tlX - 1, tlY - 1, brX + 1, brX + 1);
	}
}

Raindrop::Raindrop(unsigned short x, unsigned short y, ErodedMountain& parent)
	: x(x), y(y), parent(parent), isAlive(true), size(1) { }

void Raindrop::step()
{
	uint8_t lowestValue = parent.getValue(x, y);
	short lowX = -1;
	short lowY = -1;

	for (int dy = -1; dy <= 1; ++dy)
	{
		for (int dx = -1; dx <= 1; ++dx)
		{
			uint8_t currentValue = parent.safeGetValue(x + dx, y + dy);
			if (currentValue < lowestValue)
			{
				lowestValue = currentValue;

				lowX = x + dx;
				lowY = y + dy;
			}
		}
	}

	if ((lowX != x || lowY != y) && (lowX != -1 || lowY != -1))//if the droplet has a place move
	{
		//erode
		parent.setPixel(x, y, TGA::value(parent.safeAdd(parent.getValue(x, y), -1)));

		//move
		x = lowX;
		y = lowY;

		//update trail
		trail.push_back(pair<short, short>(x, y));

		if (parent.verboseLevel >= 3)
		{
			uint8_t valueTemp = parent.getValue(x, y);
			parent.setPixel(x, y, TGA_COLOR_CYAN);
			parent.saveIteration();
			parent.setPixel(x, y, TGA::value(valueTemp));
		}
	}
	else
	{
		//deposit
		parent.setPixel(x, y, TGA::value(parent.safeAdd(parent.getValue(x, y), 1)));

		//stop moving
		isAlive = false;

		//fill in step
		for (pair<short, short> breadcrumb : trail)
		{
			parent.fillInBox(breadcrumb.first - 1, breadcrumb.second - 1, breadcrumb.first + 1, breadcrumb.second + 1);
		}
		
	}	
}


bool Raindrop::isDead() const
{
	return !isAlive;
}

const unsigned short& Raindrop::getLocationX() const
{
	return x;
}

const unsigned short& Raindrop::getLocationY() const
{
	return y;
}

//void Raindrop::combineWith(Raindrop target);