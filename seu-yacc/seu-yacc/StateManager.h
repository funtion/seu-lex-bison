#include "stdafx.h"
#include "LRProduction.h"
#include "LRState.h"

#ifndef _STATEMANAGER_H
#define _STATEMANAGER_H

class StateManager {
public: 
    
    /**
     * @param productions
     */
    LRState& buildState(vector<LRProduction> productions);
private: 
    map<LRState,int> states;
};

#endif //_STATEMANAGER_H