#include "XEvent.hpp"

const std::map<XPadType, std::map<uint32_t, XEventSource>> XEvent::sourceMap = { // NOLINT(cert-err58-cpp)
        {
                XPadType::ONE_WIRED,       {
                                                   {0x00, XEventSource::ANALOG_LEFT_JOYSTICK_X},
                                                   {0x01, XEventSource::ANALOG_LEFT_JOYSTICK_Y},
                                                   {0x03, XEventSource::ANALOG_RIGHT_JOYSTICK_X},
                                                   {0x04, XEventSource::ANALOG_RIGHT_JOYSTICK_Y},

                                                   {0x02, XEventSource::ANALOG_LEFT_TRIGGER},
                                                   {0x05, XEventSource::ANALOG_RIGHT_TRIGGER},

                                                   {0x10, XEventSource::DPAD_X},
                                                   {0x11, XEventSource::DPAD_Y},

                                                   {0xA7, XEventSource::BUTTON_UPLOAD},

                                                   {0x130, XEventSource::BUTTON_A},
                                                   {0x131, XEventSource::BUTTON_B},
                                                   {0x133, XEventSource::BUTTON_X},
                                                   {0x134, XEventSource::BUTTON_Y},
                                                   {0x136, XEventSource::BUTTON_LEFT_TRIGGER},
                                                   {0x137, XEventSource::BUTTON_RIGHT_TRIGGER},
                                                   {0x13A, XEventSource::BUTTON_VIEW},
                                                   {0x13B, XEventSource::BUTTON_MENU},
                                                   {0x13C, XEventSource::BUTTON_XBOX},
                                                   {0x13D, XEventSource::BUTTON_LEFT_JOYSTICK},
                                                   {0x13E, XEventSource::BUTTON_RIGHT_JOYSTICK},
                                           }
        },
        {       XPadType::ONE_WIRELESS_BT, {
                                                   {0x00, XEventSource::ANALOG_LEFT_JOYSTICK_X},
                                                   {0x01, XEventSource::ANALOG_LEFT_JOYSTICK_Y},
                                                   {0x02, XEventSource::ANALOG_RIGHT_JOYSTICK_X},
                                                   {0x05, XEventSource::ANALOG_RIGHT_JOYSTICK_Y},

                                                   {0x0A, XEventSource::ANALOG_LEFT_TRIGGER},
                                                   {0x09, XEventSource::ANALOG_RIGHT_TRIGGER},

                                                   {0x10, XEventSource::DPAD_X},
                                                   {0x11, XEventSource::DPAD_Y},

                                                   {0xA7, XEventSource::BUTTON_UPLOAD},

                                                   {0x130, XEventSource::BUTTON_A},
                                                   {0x131, XEventSource::BUTTON_B},
                                                   {0x133, XEventSource::BUTTON_X},
                                                   {0x134, XEventSource::BUTTON_Y},
                                                   {0x136, XEventSource::BUTTON_LEFT_TRIGGER},
                                                   {0x137, XEventSource::BUTTON_RIGHT_TRIGGER},
                                                   {0x13A, XEventSource::BUTTON_VIEW},
                                                   {0x13B, XEventSource::BUTTON_MENU},
                                                   {0x13C, XEventSource::BUTTON_XBOX},
                                                   {0x13D, XEventSource::BUTTON_LEFT_JOYSTICK},
                                                   {0x13E, XEventSource::BUTTON_RIGHT_JOYSTICK},
                                           }
        }
};

const std::map<XEventSource, std::string> XEvent::sourceNames = { // NOLINT(cert-err58-cpp)
        {ANALOG_LEFT_JOYSTICK_X,  "ANALOG_LEFT_JOYSTICK_X"},
        {ANALOG_LEFT_JOYSTICK_Y,  "ANALOG_LEFT_JOYSTICK_Y"},
        {ANALOG_RIGHT_JOYSTICK_X, "ANALOG_RIGHT_JOYSTICK_X"},
        {ANALOG_RIGHT_JOYSTICK_Y, "ANALOG_RIGHT_JOYSTICK_Y"},
        {ANALOG_RIGHT_TRIGGER,    "ANALOG_RIGHT_TRIGGER"},
        {ANALOG_LEFT_TRIGGER,     "ANALOG_LEFT_TRIGGER"},
        {DPAD_X,                  "DPAD_X"},
        {DPAD_Y,                  "DPAD_Y"},
        {BUTTON_A,                "BUTTON_A"},
        {BUTTON_B,                "BUTTON_B"},
        {BUTTON_X,                "BUTTON_X"},
        {BUTTON_Y,                "BUTTON_Y"},
        {BUTTON_LEFT_TRIGGER,     "BUTTON_LEFT_TRIGGER"},
        {BUTTON_RIGHT_TRIGGER,    "BUTTON_RIGHT_TRIGGER"},
        {BUTTON_MENU,             "BUTTON_MENU"},
        {BUTTON_VIEW,             "BUTTON_VIEW"},
        {BUTTON_XBOX,             "BUTTON_XBOX"},
        {BUTTON_LEFT_JOYSTICK,    "BUTTON_LEFT_JOYSTICK"},
        {BUTTON_RIGHT_JOYSTICK,   "BUTTON_RIGHT_JOYSTICK"},
        {BUTTON_UPLOAD,           "BUTTON_UPLOAD"},
        {SOURCE_UNKNOWN,          "SOURCE_UNKNOWN"},
};

/**
 *
 * @return
 */
XEventSource XEvent::getSource() const {
    try {
        return XEvent::sourceMap
                .at(this->padType)
                .at(this->raw.code);
    } catch (...) {
        return XEventSource::SOURCE_UNKNOWN;
    }
}

/**
 *
 * @return
 */
std::string XEvent::getSourceName() const {
    return XEvent::sourceNames.at(this->getSource());
}

/**
 *
 * @return
 */
XEventType XEvent::getNativeType() const {
    return XEventType(this->raw.type);
}


/**
 *
 * @return
 */
int32_t XEvent::getRawValue() const {
    return this->raw.value;
}


/**
 *
 * @return
 */
bool XEvent::isInvalid() const {
    return this->raw.type == 0 && this->raw.code == 0;
}

/**
 *
 * @return
 */
bool XEvent::isInputType() const {
    const auto nativeType = this->getNativeType();
    return nativeType == XEventType::EVENT_AXIS || nativeType == XEventType::EVENT_BUTTON;
}


/**
 * Float joystick value <-100;100>
 *
 * @return
 */
float XEvent::getJoystickValue() const {
    auto value = static_cast<float>(this->raw.value);

    // Apply offset for wireless controller
    if (this->padType == XPadType::ONE_WIRELESS_BT) {
        value -= static_cast<float>(__SHRT_MAX__);

        // Fix for inverted y-axis
        if (isAxisY()) {
            value *= -1.f;
        }
    }

    // Scale
    value /= static_cast<float>(__SHRT_MAX__);
    value *= 100.f;

    // Cap range to <-100; 100>
    return std::min(
            std::max(-100.f, value),
            100.f
    );
}


bool XEvent::isAxisX() const {
    auto source = this->getSource();
    return source == XEventSource::DPAD_X
           || source == XEventSource::ANALOG_LEFT_JOYSTICK_X
           || source == XEventSource::ANALOG_RIGHT_JOYSTICK_X;
}

bool XEvent::isAxisY() const {
    auto source = this->getSource();
    return source == XEventSource::DPAD_Y
           || source == XEventSource::ANALOG_LEFT_JOYSTICK_Y
           || source == XEventSource::ANALOG_RIGHT_JOYSTICK_Y;
}

/**
 * Float trigger value <0;100>
 *
 * @return
 */
float XEvent::getTriggerValue() const {
    auto value = static_cast<float>(this->raw.value);
    value /= 1023.f;
    value *= 100.f;

    return std::min(
            std::max(0.f, value),
            100.f
    );
}

/**
 *
 * @return
 */
bool XEvent::isButtonPressed() const {
    return this->raw.value == 1;
}

/**
 *
 * @return
 */
bool XEvent::isButtonReleased() const {
    return this->raw.value == 0;
}

XDpadDirection XEvent::getDpadDirection() const {
    // Get source
    auto source = this->getSource();

    // Handle x-axis
    if (source == XEventSource::DPAD_X) {
        if (this->raw.value == -1) {
            return DPAD_LEFT;
        }

        if (this->raw.value == 1) {
            return DPAD_RIGHT;
        }

        // Returning to middle
        return DPAD_NONE;
    }

    // Handle y-axis
    if (source == XEventSource::DPAD_Y) {
        if (this->raw.value == -1) {
            return DPAD_UP;
        }

        if (this->raw.value == 1) {
            return DPAD_DOWN;
        }

        // Returning to middle
        return DPAD_NONE;
    }

    // Fallback
    return DPAD_NONE;
}

