#include "TGA-GameOfLife.h"
using namespace GameOfLife;

Board::Board(const string& name, const short& width, const short& height,
	const vector<unsigned short>& birthNums,
	const vector<unsigned short>& surviveNums
	)
	: TGA::Image(width, height, name), birthNums(birthNums), surviveNums(surviveNums) 
{
	srand(time(0));
}

void Board::initilize()
{
	iteration = 0;

	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			uint8_t alive = rand() % 2;
			uint8_t max   = 0xff;

			if (rand() % 2)
			{
				setPixel(x, y, TGA_COLOR_WHITE);
			}
			else
			{
				setPixel(x, y, TGA_COLOR_BLACK);
			}
		}
	}
}

void Board::iterate(const unsigned int& amount)
{
	vector<TGA::Pixel> nextPixels(width*height, TGA_COLOR_BLACK);

	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			int neighbors = numOfNeighbors(x, y);

			if (isAlive(x, y))
			{
				for (short surviveNum : surviveNums)
				{
					if (neighbors == surviveNum)
					{
						uint8_t greeness = (neighbors * 0xff) / 16;
						nextPixels[toLinar(x, y, width)] = TGA::Pixel{ 0xff, greeness, 0xff };
						break;
					}
				}
			}
			else
			{
				for (short birthNum : birthNums)
				{
					if (neighbors == birthNum)
					{
						uint8_t redness = (neighbors * 0xff) / 16;
						nextPixels[toLinar(x, y, width)] = TGA::Pixel{ redness, 0xff, 0xff };
						break;
					}
				}	
			}
		}
	}

	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			setPixel(x, y, nextPixels[toLinar(x, y, width)]);
		}
	}

	++iteration;
}

bool Board::isAlive(const unsigned short x, const unsigned short y)
{
	if (x >= 0 && x < width && y >= 0 && y < height)
	{
		const TGA::Pixel& pixel = getPixel(x, y);

		return !TGA::isBlack(pixel);//alive if not black
	}
	
	return false;
}

int Board::numOfNeighbors(const unsigned short x, const unsigned short y)
{
	int neighbors = 0;

	for (int dy = -1; dy <= 1; ++dy)
	{
		for (int dx = -1; dx <= 1; ++dx)
		{
			if (!(dx == 0 && dy == 0) && isAlive(x + dx, y + dy))
			{
				++neighbors;
			}
		}
	}

	return neighbors;
}