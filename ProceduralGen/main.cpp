#include "TGA-GameOfLife.h"
#include "TGA-Mandelbrot.h"
#include "TGA-Mountain.h"
#include <iostream>

///////////////////////////////////////////////////////////////////////
// DEMO FUNCTIONS - call these if you want to use the other programs //
//                 I wrote to learn procedural genneration.          //
///////////////////////////////////////////////////////////////////////

void erosionDemo()
{
	//x, y, name, varianceDecay, raindropsPerPixel, max Slope, verbosity
	ErodedMountain mount(65, 65, "erosion/erosion", 2, 2, 20);
	mount.save();
}

void mountainDemo()
{
	Mountain mount(65, 65, "mountain/mountain", 2, 3);
	mount.save();
}

void mandelbrotDemo()
{
	int scanLines = 10000;
	Mandelbrot testMandel(scanLines*1.25, scanLines, "mandel/blueMandel");
	testMandel.save();
}

void gameOfLifeDemo()
{
	GameOfLife::Board board("swamp/swamp", 64, 64,
	{ 6, 7, 8 },
	{ 4, 5, 6, 7, 8 }
	);

	board.initilize();
	board.save("swamp/swamp_0");

	for (int i = 0; i < 10; ++i)
	{
		board.iterate();
		board.saveIteration();
	}
}

////////////////////////
// DEMO FUNCTIONS END //
////////////////////////

void printMountainUsage()
{
	cout << "Usage:" << endl;
	cout << "mountainGen [options]" << endl;
}

void printMountainHelp()
{
	printMountainUsage();
	cout << "Options:" << endl;
	cout << "-size [width] [height] => The size of the output image." << endl;
	cout << "-filename [name] => The name of the output file." << endl;
	cout << "-varianceDecay [decay] => The number the random variance is devided by after each iteration of the"
		 << "diamond-square algorithm" << endl;
	cout << "-seed [seed number] => random seed to use for the diamond-square genneration." << endl;
	cout << "-raindropsPerPixel => The number of raindrops cast per pixel of the output image." << endl;
	cout << "-maxSlope [dValue] => The maximum difference allowed before the terrain erodes. "
		 << "Valid values range from 1 to 255" << endl;
	cout << "-verboseLevel [level] => The level of debug output the program provides. Verbosity levels include all "
		 << "features of non-zero lower levels. Valid values are listed below:" << endl;
	cout << "\t0 or left blank - no debug output" << endl;
	cout << "\t1 - output the percentage complete of the raindrop algorithm" << endl;
	cout << "\t2 - output an image after each raindrop cast" << endl;
	cout << "\t3 - output an image after each raindrop step (raindrop drawn)" << endl;
	cout << "\t4 - output image after each diamond-square step and after each raindrop step" << endl;
	cout << "-help - show this help message" << endl;

}

void printArgError( const string message = "" )
{
	cout << "ERROR - Invalid arguments " << message << endl;
	printMountainUsage();
	exit(EXIT_FAILURE);
}

void printArgNumError()
{
	cout << "ERROR - Invalid number of arguments" << endl;
	printMountainUsage();
	exit(EXIT_FAILURE);
}

void printParameterError(const string& invalidParameterName)
{
	cout << "ERROR - \"" << invalidParameterName << "\" parameter was not entered correctly" << endl;
	printMountainUsage();
	exit(EXIT_FAILURE);
}

int main(const int argc, const char* argv[])
{
	//parameters that need to be converted and stored
	short width = 65,
		height = 65;
	string filename = "mountain";
	double varianceDecay = 2,
		raindropsPerPixel = 2;
	uint8_t maxSlope = 15;
	int verboseLevel = 0;

	int seed = time(0);

	int currentIndex = 1;//1 not 0 because first argument is program name

	//convert c-style arguments into cpp-style arguments
	vector<string> cppArgv;
	for (int i = 0; i < argc; ++i)
	{
		cppArgv.push_back(string(argv[i]));
	}

	if (argc == 2 && (cppArgv[1] == "-help" || cppArgv[1] == "help"))
	{
		printMountainHelp();
	}
	else
	{
		while (argc > currentIndex)
		{
			if (cppArgv[currentIndex][0] == '-')
			{
				if (cppArgv[currentIndex] == "-size")
				{
					if (argc > currentIndex + 2)
					{
						try
						{
							width = stoi(cppArgv[++currentIndex]);
							height = stoi(cppArgv[++currentIndex]);
						}
						catch (const invalid_argument e)
						{
							printArgError("Can't parse size");
						}
					}
					else
					{
						printArgError("Too few arguments following \"-size\"");
					}
				}
				else if (cppArgv[currentIndex] == "-filename")
				{
					if (argc > currentIndex + 1)
					{
						filename = cppArgv[++currentIndex];
					}
					else
					{
						printArgError("Too few arguments following \"-filename\"");
					}
				}
				else if (cppArgv[currentIndex] == "-varianceDecay")
				{
					if (argc > currentIndex + 1)
					{
						try
						{
							varianceDecay = (double)stof(cppArgv[++currentIndex]);
						}
						catch (const invalid_argument e)
						{
							printArgError("Can't parse varianceDecay");
						}
					}
					else
					{
						printArgError("Wrong number of arguments following \"-varianceDecay\"");
					}
				}
				else if (cppArgv[currentIndex] == "-raindropsPerPixel")
				{
					if (argc > currentIndex + 1)
					{
						try
						{
							raindropsPerPixel = stof(cppArgv[++currentIndex]);
						}
						catch (const invalid_argument e)
						{
							printArgError("Can't parse raindropsPerSecond");
						}
					}
					else
					{
						printArgError("Wrong number of arguments following \"-raindropsPerPixel\"");
					}
				}
				else if (cppArgv[currentIndex] == "-maxSlope")
				{
					if (argc > currentIndex + 1)
					{
						try
						{
							maxSlope = (uint8_t)stoi(cppArgv[++currentIndex]);
						}
						catch (const invalid_argument e)
						{
							printArgError("Can't parse maxSlope");
						}
					}
					else
					{
						printArgError("Wrong number of arguments following \"-maxSlope\"");
					}
				}
				else if (cppArgv[currentIndex] == "-verboseLevel")
				{
					if (argc > currentIndex + 1)
					{
						try
						{
							verboseLevel = stoi(cppArgv[++currentIndex]);
						}
						catch (const invalid_argument e)
						{
							printArgError("Can't parse verboseError");
						}
					}
					else
					{
						printArgError("Wrong number of arguments following \"-verboseError\"");
					}
				}
				else if (cppArgv[currentIndex] == "-seed")
				{
					if (argc > currentIndex + 1)
					{
						try
						{
							seed = stoi(cppArgv[++currentIndex]);
						}
						catch (const invalid_argument e)
						{
							printArgError("Can't parse seed");
						}
					}
					else
					{
						printArgError("Wrong number of arguments following \"-seed\"");
					}
				}
				else
				{
					printArgError();
				}
				++currentIndex;
			}
			else
			{
				printArgError();
			}
		}

		//seed random for mountain
		srand(seed);

		//gennerate and save the image
		ErodedMountain mount(width, height, filename, varianceDecay, raindropsPerPixel, maxSlope, verboseLevel);
		mount.save();
	}
}

