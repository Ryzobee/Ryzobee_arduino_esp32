#include "boards.h"
#include "board_pin.h"

using namespace lgfx::v1;

Panel_Device* panel_load_from_rootmaker(Ryzobee_board_pin_t* pins);

Ryzobee::Ryzobee(Ryzobee_board_t board) {
    _board = board;
    setPanel(nullptr);
}

bool Ryzobee::init_impl(bool use_reset, bool use_clear) {
    Panel_Device* panel = nullptr;
    switch (_board) {
      case RYZOBEE_ROOTMAKER: panel = panel_load_from_rootmaker(&pins); break;
      default: break;
    }

    if (panel == nullptr) {
      assert(0);
    }

    setPanel(panel);
    return LGFX_Device::init_impl(false, use_clear);
}

