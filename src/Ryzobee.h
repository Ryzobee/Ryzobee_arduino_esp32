#pragma once

#ifdef BOARD_RYZOBEE_ROOTMAKER
#include "board/rootmaker/rootmaker.hpp"
#endif

class Ryzobee {
  
  public:
  #ifdef BOARD_RYZOBEE_ROOTMAKER
    rootmaker maker;
  #endif
};

