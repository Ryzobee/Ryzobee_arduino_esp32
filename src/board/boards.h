#pragma once

#include "rootmaker/rootmaker.h"

enum Ryzobee_board_t {
    RYZOBEE_ROOTMAKER, // RootMaker
};

class Ryzobee {
  
  public:

    RootMaker rootmaker;

    Ryzobee_board_t _board;
    Ryzobee(Ryzobee_board_t board);
    void begin(void);
};
