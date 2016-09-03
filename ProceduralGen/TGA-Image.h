#pragma once
#include <string>
#include <vector>
#include <array>

#define toLinar(x, y, width) ( (width * y) + x )

//colors
#define TGA_COLOR_WHITE   TGA::Pixel{ 0xff, 0xff, 0xff }
#define TGA_COLOR_BLACK   TGA::Pixel{ 0   , 0   , 0    }

#define TGA_COLOR_RED     TGA::Pixel{ 0xff, 0   , 0    }
#define TGA_COLOR_GREEN   TGA::Pixel{ 0   , 0xff, 0    }
#define TGA_COLOR_BLUE    TGA::Pixel{ 0   , 0   , 0xff }

#define TGA_COLOR_CYAN    TGA::Pixel{ 0   , 0xff, 0xff }
#define TGA_COLOR_MAGENTA TGA::Pixel{ 0xff, 0   , 0xff }
#define TGA_COLOR_YELLOW  TGA::Pixel{ 0xff, 0xff, 0    }

using namespace std;

namespace TGA
{
	enum CompressionType
	{
		UNCOMPRESSED_RGB  = 2
	};

	struct Pixel
	{
		uint8_t r, g, b;
	};

	static Pixel value(uint8_t x) { return Pixel{ x, x, x }; }
	static bool isBlack(const Pixel& pixel) { return pixel.r == 0 && pixel.g == 0 && pixel.b == 0; }

	class Image
	{
	protected:
		unsigned short width, height;
		vector<Pixel> pixels;
		string name;

	public:
		Image(const unsigned short& width, const unsigned short& height, const string& name);

		void setPixel(const unsigned short& x, const unsigned short& y, const Pixel& setTo);
		const Pixel& getPixel(const unsigned short& x, const unsigned short& y);

		void save(string saveName = "");

	private:
		void writeHeader(ofstream& writeTo);
		void writeData(ofstream& writeTo);

		array<char, 2> shortToCharArray(const short& in);
	};
}


/*
void WriteImageHeader(int width, int height, FILE* fptr)
{
// TGA header, endian independent
putc(0,fptr); /* Length of ID 
putc(0, fptr); /* No colour map 
putc(2, fptr); /* uncompressed RGB  
putc(0, fptr); /* Index of colour map entry 
putc(0, fptr);
putc(0, fptr); /* Colour map length 
putc(0, fptr);
putc(0, fptr); /* Colour map size 
putc(0, fptr); /* X origin 
putc(0, fptr);
putc(0, fptr); /* Y origin 
putc(0, fptr);
putc((width & 0x00ff), fptr); /* X width 
putc((width & 0xff00) / 256, fptr);
putc((height & 0x00ff), fptr); /* Y width 
putc((height & 0xff00) / 256, fptr);
putc(24, fptr);                      /* 24 bit bitmap     
putc(0x00, fptr);
// 18 bytes
}
*/