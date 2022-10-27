#pragma once

#include <unistd.h>
#include <fcntl.h>
#include <linux/joystick.h>
#include <dirent.h>
#include <cstdint>
#include <string>
#include <string>
#include <stdexcept>
#include <tuple>

#include "XEvent.hpp"
#include "XPadType.hpp"

class XPad {
    int devicePointer = -1;
    XEvent event = {};

    int rumbleEffectId = -1;

    void initializeRumble();

public:
    static std::tuple<std::string, XPadType> getAvailableControllerPath();

    bool isActive() const;

    void connect(const std::string &devicePath, const XPadType &type);

    bool readEvent();

    XEvent getEvent();

    void rumble(int32_t count) const;
};