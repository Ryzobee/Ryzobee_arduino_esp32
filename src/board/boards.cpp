#include "boards.h"
#include "rootmaker/rootmaker_pin.h"

Ryzobee::Ryzobee(Ryzobee_board_t board) {
    _board = board;
}

void Ryzobee::begin(void) {
    switch (_board) {
      case RYZOBEE_ROOTMAKER: {
        rootmaker.begin(true, true, true, true);
        break;
      }
      default: {
        break;
      }
    }
}

