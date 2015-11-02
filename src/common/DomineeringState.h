//
//  DomineeringState.h
//  CSE486AIProject
//
//  Created by Yunlong Nick Liu on 5/29/15.
//  Copyright (c) 2015 Yunlong Nick Liu. All rights reserved.
//

#ifndef __CSE486AIProject__DomineeringState__
#define __CSE486AIProject__DomineeringState__

#include "Params.h"
#include "BoardGameState.h"
#include "DomineeringMove.h"
#include <stdio.h>
#include <vector>

class DomineeringState : public BoardGameState {
public:
    
    static Params& getDomineeringParams();
    
    static GameState* create();
    
    bool moveOK(const GameMove &gm) const override;
    
	DomineeringState();

private:
        
    void thisGameMakeMove(const GameMove &gm) override;
    
    Status thisGameCheckTerminalUpdateStatus() override;

};

/**
 * Combine operation of two hash keys. Based on boost::hash_combine.
 */
namespace {
    void hash_combine(std::size_t& h, const std::size_t& v) {
        h ^= v + 0x9e3779b9 + (h << 6) + (h >> 2);
    }
}

namespace std {
    template<>
    struct hash<DomineeringState> {
        size_t operator()(const DomineeringState& ds) const {
            size_t h = 0;
            for (auto&& c : *ds.getBoard1D()) {
                if (c == ds.EMPTYSYM) {
                    hash_combine(h, std::hash<char>()(c));
                }
            }
            return h;
        }
    };
} // namespace std

#endif /* defined(__CSE486AIProject__DomineeringState__) */
