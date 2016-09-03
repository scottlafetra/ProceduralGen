#pragma once
#include <iostream>
#include <fstream>
#include "TGA-Image.h"

using namespace TGA;

Image::Image(const unsigned short& width, const unsigned short& height, const string& name)
	: width(width), height(height), name(name), pixels(vector<Pixel>(width*height, Pixel{ 0, 0, 0 })) {};

void Image::setPixel(const unsigned short& x, const unsigned short& y, const Pixel& setTo)
{
	pixels[toLinar(x, y, width)] = setTo;
}

const Pixel& Image::getPixel(const unsigned short& x, const unsigned short& y)
{
	return pixels[toLinar(x, y, width)];
}

void Image::save(string saveName)
{
	if (saveName == "")
	{
		saveName = name;
	}

	ofstream file;
	file.open(saveName + ".tga", ofstream::binary | ofstream::out | ofstream::trunc);

	writeHeader(file);
	writeData(file);

	file.close();
}

void Image::writeHeader(ofstream& writeTo)
{
	writeTo.put(0);//Length of ID
	writeTo.put(0);//No color map
	writeTo.put(UNCOMPRESSED_RGB);//compression type
	writeTo.write(shortToCharArray(0     ).data(), 2);//Index of color map
	writeTo.write(shortToCharArray(0     ).data(), 2);//Color map length
	writeTo.put(0);//Color map size
	writeTo.write(shortToCharArray(0     ).data(), 2);//X origin
	writeTo.write(shortToCharArray(0     ).data(), 2);//Y origin
	writeTo.write(shortToCharArray(width ).data(), 2);//width (x)
	writeTo.write(shortToCharArray(height).data(), 2);//height (y)
	writeTo.put(24);//bits per pixel
	writeTo.put(0);//padding of some sort?
	

	
	//// TGA header, endian independent
	//writeTo.put(0); /* Length of ID */
	//writeTo.put(0); /* No colour map */
	//writeTo.put(2); /* uncompressed RGB  */
	//writeTo.put(0); /* Index of colour map entry */
	//writeTo.put(0);
	//writeTo.put(0); /* Colour map length */
	//writeTo.put(0);
	//writeTo.put(0); /* Colour map size */
	//writeTo.put(0); /* X origin */
	//writeTo.put(0);
	//writeTo.put(0); /* Y origin */
	//writeTo.put(0);
	//writeTo.put((width & 0x00ff)); /* X width */
	//writeTo.put((width & 0xff00) / 256);
	//writeTo.put((height & 0x00ff)); /* Y width */
	//writeTo.put((height & 0xff00) / 256);
	//writeTo.put(24);                      /* 24 bit bitmap     */
	//writeTo.put(0x00);
	//// 18 bytes
	//
}

void Image::writeData(ofstream& writeTo)
{
	for (const Pixel& pixel : pixels)
	{
		writeTo.put(pixel.b);//blue
		writeTo.put(pixel.g);//green
		writeTo.put(pixel.r);//red
	}
}

array<char, 2> Image::shortToCharArray(const short& in)
{
	array<char, 2> out;

	out[1] = in >> 8;
	out[0] = in & 0x00ff;

	return out;
}