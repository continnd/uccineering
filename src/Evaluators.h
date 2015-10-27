#ifndef EVALUATORS_H_
#define EVALUATORS_H_

#include "DomineeringState.h"

#include <functional>
#include <utility>
#include <vector>
// TODO: Better evaluation
#include <random>
#include <iostream>

static std::random_device rd;
static std::uniform_real_distribution<double> u_dist;

using DS = DomineeringState;

struct EvalTakeAway {
    double operator()(const DS& state) const {
        return u_dist(rd);
    }
};

struct EvalReserve {
    double operator()(const DS& state) const {
	std::cout << state.getCurPlayerSym() << std::endl;
        int count = 0;
	// print board
	 for (int i = 0; i < state.ROWS; i++){
                for (int j = 0; j < state.COLS; j++){
                        std::cout << state.getCell(i, j);

                }
                        std::cout << std::endl;
        }



        // Loop through every square in the board, if homei
	
//	if (state.getCurPlayerSym() == 'W'){

	        for (int i = 0; i < state.ROWS; i++){
        	        for (int j = 0; j < state.COLS; j++){
                	        std::cout << state.getCell(i, j);
				std::cout << i << " " << j << std::endl;
				if (state.getCurPlayerSym() == 'W' && state.getCell(i,j) == state.EMPTYSYM){
	                	// See if adjacent square is open
				        if ((((j+1) < state.ROWS) && (state.getCell(i,j+1) == state.EMPTYSYM))){
//					Check to see if above&below spaces are empty & inbounds

						 if (((i-1) > 0) && ((j+1) > 0) && (state.getCell(i-1,j) != state.EMPTYSYM) && (state.getCell(i-1,j+1) != state.EMPTYSYM) && (((i+1) > state.ROWS) || ((j+1) > state.COLS)) || (((i+1) < state.ROWS) && ((j+1) < state.COLS) && (state.getCell(i+1,j) != state.EMPTYSYM) && (state.getCell(i+1,j+1) != state.EMPTYSYM)&& (((i-1) < 0) || ((j+1 < 0))))){

							count++;
							std::cout << "FOR SURE!!!" << std::endl;
						}
						else if ((((i-1) > 0) && ((j+1) > 0) && (state.getCell(i-1,j) != state.EMPTYSYM) && (state.getCell(i-1,j+1) != state.EMPTYSYM)) && (((i+1) < state.ROWS) && ((j+1) < state.COLS) && (state.getCell(i+1,j) != state.EMPTYSYM) && (state.getCell(i+1,j+1) != state.EMPTYSYM))){
							count++;
	                                        	std::cout << "FOR SURE!!!" << std::endl;
						}

	        		 	}	
				}
				else if (state.getCurPlayerSym() == 'B' && (state.getCell(i,j) == state.EMPTYSYM)){
					 // See if adjacent square is open
	                                if ((((i+1) < state.ROWS) && (state.getCell(i+1,j) == state.EMPTYSYM))){

	                                         if ((((i+1) > 0) && ((j-1) > 0) && (state.getCell(i,j-1) != state.EMPTYSYM) && (state.getCell(i+1,j-1) != state.EMPTYSYM) && (((i+1) > state.ROWS) || ((j+1) > state.COLS))) || (((i+1) < state.ROWS) && ((j+1) < state.COLS) && (state.getCell(i,j+1) != state.EMPTYSYM) && (state.getCell(i+1,j+1) != state.EMPTYSYM)&& (((i-1) < 0) || ((j-1 < 0))))){

        	                                        count++;
                	                                std::cout << "FOR SURE!!!" << std::endl;
                        	                }
					       else if ((((i+1) > 0) && ((j-1) > 0) && (state.getCell(i,j-1) != state.EMPTYSYM) && (state.getCell(i+1,j-1) != state.EMPTYSYM)) && (((i+1) < state.ROWS) && ((j+1) < state.COLS) && (state.getCell(i,j+1) != state.EMPTYSYM) && (state.getCell(i+1,j+1) != state.EMPTYSYM))){
                                        	        count++;
                                                	std::cout << "FOR SURE!!!" << std::endl;
                                        	}

                                 	}

				  }	


        		}
                        //std::cout << std::endl;
	        }
	
//	else if (state.getCurPlayerSym() == 'B'){
			


//	}

//	std::cout << count << std::endl;
        return u_dist(rd);
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
