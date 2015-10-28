#ifndef EVALUATORS_H_
#define EVALUATORS_H_

#include "DomineeringState.h"

#include <functional>
#include <utility>

using DS = DomineeringState;

struct Evaluator {

	bool checkIfNextEmpty(int a, int b, const DS& state) const{
		if (a < state.ROWS && b < state.ROWS && state.getCell(a,b) == state.EMPTYSYM)
			return true;
		return false;
	}

	bool checkEdgePieceReserveHome(int a, int b, int c, int d, const DS& state) const{
		if (((a > 0) && (b > 0) && (state.getCell(a,c) != state.EMPTYSYM) && (state.getCell(a,b) != state.EMPTYSYM) && (d == state.ROWS) || (b == state.COLS)) || ((d < state.ROWS) && (b < state.COLS) && (state.getCell(d,c) != state.EMPTYSYM) && (state.getCell(d,b) != state.EMPTYSYM)&& ((a < 0) || ((b < 0)))))
			return true;
		return false;
	}

	bool checkMiddlePieceReserveHome(int a, int b, int c, int d, const DS& state) const{
		if (((a > 0) && (b > 0) && (state.getCell(a,c) != state.EMPTYSYM) && (state.getCell(a,b) != state.EMPTYSYM)) && ((d < state.ROWS) && (b < state.COLS) && (state.getCell(d,c) != state.EMPTYSYM) && (state.getCell(d,b) != state.EMPTYSYM)))
			return true;
		return false;
	}

	bool checkEdgePieceReserveAway(int a, int b, int c, int d, int e, const DS& state) const{

		if (((b > 0) && (state.getCell(c,b) != state.EMPTYSYM) && (state.getCell(a,b) != state.EMPTYSYM) && (d == state.COLS)) || ((d < state.COLS) && (state.getCell(c,d) != state.EMPTYSYM) && (state.getCell(a,d) != state.EMPTYSYM) && ((b < 0))))
			return true;
		return false;

	}

	bool checkMiddlePieceReserveAway(int a, int b, int c, int d, const DS& state) const{


		if ((a > 0) && (b > 0) && (state.getCell(c,b) != state.EMPTYSYM) && (state.getCell(a,b) != state.EMPTYSYM) && (a < state.ROWS) && (d < state.COLS) && (state.getCell(c,d) != state.EMPTYSYM) && (state.getCell(a,d) != state.EMPTYSYM))
			return true;
		return false;
	}

/*	int checkHomeReserve(int i, int j, int count, const DS& state) const{
		if (checkIfNextEmpty(i, j+1, state)){

			if (checkEdgePieceReserveHome(i-1,j+1,j,i+1,state)){
//				std::cout << "Edge home for sure" << std::endl;
				count++;
			}
			else if (checkMiddlePieceReserveHome(i-1,j+1,j,i+1,state)){
//				std::cout << "middle home for sure" << std::endl;

				count++;
			}
		}
		return count;
	}

	int checkAwayReserve(int i, int j, int count, const DS& state) const{
		if (checkIfNextEmpty(i+1, j, state)){

			if (checkEdgePieceReserveAway(i+1,j-1,i,j+1,i-1,state)){
//				std::cout << "Edge away for sure" << std::endl;

				count++;
			}
			else if (checkMiddlePieceReserveAway(i+1,j-1,i,j+1,state)){
//				std::cout << "middle away for sure" << std::endl;

				count++;
			}
		}
		return count;
	}*/

};

struct EvalTakeAway : public Evaluator{

	double operator()(const DS& state) const {
		int count = 0;

		for (int i = 0; i < state.ROWS; i++){
			for (int j = 0; j < state.COLS; j++){
				if (state.getCurPlayerSym() == state.HOMESYM && state.getCell(i,j) == state.EMPTYSYM){
					if (checkIfNextEmpty(i, j+1, state)){

						if (checkEdgePieceReserveHome(i-1,j+1,j,i+1,state)){
							count++;
						}
						else if (checkMiddlePieceReserveHome(i-1,j+1,j,i+1,state)){
							count++;
						}
					}

				}

				else if (state.getCurPlayerSym() == state.AWAYSYM && (state.getCell(i,j) == state.EMPTYSYM)){
					if ((i+1) < state.ROWS && (state.getCell(i+1,j) == state.EMPTYSYM)){

						if (checkEdgePieceReserveAway(i+1,j-1,i,j+1,i-1,state)){
							count++;
						}
						else if (checkMiddlePieceReserveAway(i+1,j-1,i,j+1,state)){
							count++;
						}
					}
				}
			}
		}
		return count;
	}
};

struct EvalReserve : public Evaluator {
	double operator()(const DS& state) const {
		int count = 0;

		for (int i = 0; i < state.ROWS; i++){
			for (int j = 0; j < state.COLS; j++){
				if (state.getCurPlayerSym() == state.AWAYSYM && state.getCell(i,j) == state.EMPTYSYM){
					if (checkIfNextEmpty(i, j+1, state)){

						if (checkEdgePieceReserveHome(i-1,j+1,j,i+1,state)){
							count++;
						}
						else if (checkMiddlePieceReserveHome(i-1,j+1,j,i+1,state)){
							count++;
						}
					}
}
				else if (state.getCurPlayerSym() == state.HOMESYM && (state.getCell(i,j) == state.EMPTYSYM)){
					if ((i+1) < state.ROWS && (state.getCell(i+1,j) == state.EMPTYSYM)){


						if (checkEdgePieceReserveAway(i+1,j-1,i,j+1,i-1,state)){
							count++;
						}
						else if (checkMiddlePieceReserveAway(i+1,j-1,i,j+1,state)){
							count++;
						}
					}
				}
			}
		}
		return count;
	}
};

using EvalScore = std::function<double(const DS&)>;
using EvalFactor = std::function<double(const DS&)>;
// vector of pairs because std::map requires operator< and std::unordered_map
// requires operator== and hash function for DomineeringState
static const std::vector<std::pair<EvalScore, EvalFactor>> evaluators = {
	{ std::make_pair(EvalTakeAway(), [](const DS& state) { return 1; }) },
	{ std::make_pair(EvalReserve(), [](const DS& state) { return 1; }) },
};

#endif /* end of include guard */

/* vim: tw=78:et:ts=4:sts=4:sw=4 */

