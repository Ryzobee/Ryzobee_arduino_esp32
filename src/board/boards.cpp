#include "boards.h"
#include "rootmaker/rootmaker_pin.h"

Ryzobee::Ryzobee(Ryzobee_board_t board) {
    _board = board;
}

void Ryzobee::begin(void) {
    switch (_board) {
      case RYZOBEE_ROOTMAKER: {
        // 只调用 RootMaker 的初始化，不要重复初始化 LGFX
        rootmaker.begin(true, true, true, true);
        break;
      }
      default: {
        break;
      }
    }
}

