#ifndef _LRPRODUCTION_H
#define _LRPRODUCTION_H
#include "TOKEN.h"

class LRProduction {
public: 
    int productionId;
    int pos;
	vector<TerminalToken> lookAhead;
};

#endif //_LRPRODUCTION_H