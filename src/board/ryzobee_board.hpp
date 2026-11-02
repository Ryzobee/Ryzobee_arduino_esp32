#pragma once

#include <cstdint>
#include <cstdio>
#include <map>
#include <string>
#include <vector>

#define RYZOBEE_DEBUG

#ifdef RYZOBEE_DEBUG
#define ryzobee_d_printf(...) fprintf(stderr, __VA_ARGS__)
#define ryzobee_i_printf(...) fprintf(stdout, __VA_ARGS__)
#else
#define ryzobee_d_printf(...) ((void)0)
#define ryzobee_i_printf(...) ((void)0)
#endif

enum ryzobee_port_field : uint8_t {
  PORT_TYPE = 0,
  PORT_USED = 1,
  PORT_INVALID = 2,
  PORT_PIN_NUM = 3,
  PORT_PIN_START = 4,
};

enum ryzobee_port_type : uint8_t {
  PORT_TYPE_RS,
  PORT_TYPE_S_BUS,
  PORT_TYPE_F_BUS,
};

class ryzobee_board {
  public:
    ryzobee_board(std::string name) : _board_name(name) {}
    virtual void begin() = 0;
    virtual std::string getBoardName() = 0;
    virtual std::string getBoardInfo() = 0;

    std::string portsInfo() {
        std::string info;
        for (const auto& port : ports) {
            const auto& data = port.second;
            if (data.size() < PORT_PIN_START) {
                info += port.first + ": invalid port data\n";
                continue;
            }
            info += port.first + ": type=" + std::to_string(data[PORT_TYPE]);
            info += ", " + std::string(data[PORT_USED] ? "used" : "unused");
            info += ", " + std::string(data[PORT_INVALID] ? "invalid" : "valid");
            info += "\n";
        }
        return info;
    }

    std::vector<uint8_t>* useport(std::string port_name) {
        auto it = ports.find(port_name);
        if (it == ports.end()) {
            ryzobee_d_printf("Port %s not found.\n", port_name.c_str());
            return nullptr;
        }
        auto& data = it->second;
        if (data.size() < PORT_PIN_START) {
            ryzobee_d_printf("Port %s has invalid data.\n", port_name.c_str());
            return nullptr;
        }
        if (data[PORT_USED]) {
            ryzobee_d_printf("Port %s is already in use.\n", port_name.c_str());
            return {};
        }
        return &data;
    }

  public:
    // std::vector<uint8_t> layout:
    // [0] = port type
    // [1] = used
    // [2] = invalid
    // [3] = pin num
    // [4] = pin1, [5] = pin2, ...
    std::map<std::string, std::vector<uint8_t>> ports;
  protected:
    std::string _board_name;
};