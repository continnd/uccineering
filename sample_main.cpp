//
//  main.cpp
//  CSE486AIProject
//
//  Created by Yunlong Nick Liu on 6/19/15.
//  Copyright (c) 2015 Yunlong Nick Liu. All rights reserved.
//
#include "RandomDomineeringPlayer.h"
#include "main.h"

int main(int argc, char *argv[]) {
    RandomDomineeringPlayer p("Random");
	// DomineeringState st;
	// GameMove *mv = p->getMove(st, "");
    //
	// std::cout << mv->toString() << std::endl;
	// st.makeMove(*mv);
	// std::cout << st.toDisplayStr() << std::endl;

    p.compete(argc, argv);
    return 0;
}
