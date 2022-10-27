#pragma once

#include <map>
#include <string>
#include <cstdint>
#include <linux/joystick.h>
#include "XPadType.hpp"
#include "XEventType.hpp"
#include "XEventSource.hpp"
#include "XDpadDirection.hpp"

class XEvent {

    input_event raw = {};
    XPadType padType = XPadType::ONE_WIRED;
    static const std::map<XPadType, std::map<uint32_t, XEventSource>> sourceMap;
    static const std::map<XEventSource, std::string> sourceNames;

public:
    XEventSource getSource() const;

    std::string getSourceName() const;

    XEventType getNativeType() const;

    int32_t getRawValue() const;

    bool isInvalid() const;

    bool isInputType() const;

    float getJoystickValue() const;

    bool isAxisX() const;

    bool isAxisY() const;

    float getTriggerValue() const;

    bool isButtonPressed() const;

    bool isButtonReleased() const;

    XDpadDirection getDpadDirection() const;

    friend class XPad;
};
