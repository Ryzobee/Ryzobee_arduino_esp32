#pragma once

#include "rootmaker/rootmaker.h"

enum Ryzobee_board_t {
    RYZOBEE_ROOTMAKER, // RootMaker
};

class Ryzobee {
  
  public:

    RootMaker rootmaker;

    Ryzobee_board_t _board;

    /**
     * @brief Constructor
     * @param board Board type to initialize
     */
    Ryzobee(Ryzobee_board_t board);

    /**
     * @brief Initialize the board hardware
     */
    void begin(void);
};
