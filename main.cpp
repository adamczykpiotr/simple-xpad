#include <iostream>
#include "src/XPad/XPad.hpp"

int main() {

    // Instantiate object
    XPad pad;

    // Event loop
#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
    while (true) {
        // Resolve joystick device if disconnected
        if (!pad.isActive()) {
            try {
                auto device = XPad::getAvailableControllerPath();
                pad.connect(std::get<0>(device), std::get<1>(device));
                std::cout << "Succesfully connected to device " << std::get<0>(device) << " ("
                          << (std::get<1>(device) == XPadType::ONE_WIRED ? "Wired" : "Wireless") << ")\n";
            } catch (...) {
                std::cerr << "Unable to connect to pad...\n";
                sleep(1);
                continue;
            }
        }

        // Skip if no event was read
        if (!pad.readEvent()) {
            continue;
        }

        // Read & validate event
        auto event = pad.getEvent();
        if (!event.isInputType()) {
            continue;
        }

        std::cout << "Event: " << event.getSourceName() << "\n";

        if (event.getNativeType() == XEventType::EVENT_BUTTON) {
            std::cout << "\t[Button]: Pressed: " << (event.isButtonPressed() ? "true" : "false")
                      << ", Released: " << (event.isButtonReleased() ? "true" : "false") << "\n";

            if (event.getSource() == XEventSource::BUTTON_X) {
                std::cout << "Trigger rumble (1)\n";
                pad.rumble(1);
            }

            if (event.getSource() == XEventSource::BUTTON_Y) {
                std::cout << "Trigger rumble (3)\n";
                pad.rumble(3);
            }
        }

        if (event.getNativeType() == XEventType::EVENT_AXIS) {
            // Analog
            const XEventSource Sources[] = {
                    XEventSource::ANALOG_LEFT_JOYSTICK_X,
                    XEventSource::ANALOG_LEFT_JOYSTICK_Y,
                    XEventSource::ANALOG_RIGHT_JOYSTICK_X,
                    XEventSource::ANALOG_RIGHT_JOYSTICK_Y,
            };
            for (const auto &joystickSource: Sources) {
                if (event.getSource() == joystickSource) {
                    std::cout << "\t[Joystick] Position: " << event.getJoystickValue() << "\n";
                }
            }

            // Trigger
            const XEventSource triggerSources[] = {
                    XEventSource::ANALOG_LEFT_TRIGGER,
                    XEventSource::ANALOG_RIGHT_TRIGGER,
            };
            for (const auto &triggerSource: triggerSources) {
                if (event.getSource() == triggerSource) {
                    std::cout << "\t[Trigger] Position: " << event.getTriggerValue() << "\n";
                }
            }

            // DPAD
            if (event.getSource() == XEventSource::DPAD_Y || event.getSource() == XEventSource::DPAD_X) {
                std::cout << "\t[DPAD] Direction: " << event.getDpadDirection() << "\n";
            }
        }

        std::cout << "\n";
    }
#pragma clang diagnostic pop
}