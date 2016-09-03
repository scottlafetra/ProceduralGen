#pragma once
#include <ctime>
#include "TGA-Image.h"

namespace GameOfLife
{
	class Board : public TGA::Image
	{
	protected:
		int iteration;

		//Rules that determine the birth and death of cells
		vector<unsigned short> birthNums;
		vector<unsigned short> surviveNums;

	public:
		Board(const string& name, const short& width, const short& height,
			const vector<unsigned short>& birthNums,
			const vector<unsigned short>& surviveNums
			);

		void initilize();//set the boards to random values
		void iterate(const unsigned int& amount = 1);//itterate the board by amount

		inline void saveIteration() { save(name + "_" + to_string(iteration)); }

	private:
		bool isAlive(const unsigned short x, const unsigned short y);
		int numOfNeighbors(const unsigned short x, const unsigned short y);
	};
}
