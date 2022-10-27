#include "XPad.hpp"

/**
 * @return
 */
bool XPad::isActive() const {
    return this->devicePointer > 0;
}

/**
 * Attempts to connet to requested device
 *
 * @param devicePath
 */
void XPad::connect(const std::string &devicePath, const XPadType &type) {
    this->devicePointer = open(devicePath.c_str(), O_RDWR);
    if (this->devicePointer < 0) {
        throw std::runtime_error("Unable to connect to device" + devicePath);
    }

    // Assign event padType
    this->event.padType = type;

    // Try to initialize rumble effect
    this->initializeRumble();
}

/**
 * @return
 */
bool XPad::readEvent() {
    ssize_t bytes = read(this->devicePointer, &this->event.raw, sizeof(input_event));

    // Detach device on error
    if (bytes < 0) {
        this->devicePointer = -1;
    }

    return bytes == sizeof(input_event);
}

XEvent XPad::getEvent() {
    return this->event;
}

/**
 * Find all devices /dev/input/event*
 *
 * TODO: [Future] Switch to C++20 std::filesystem syntax
 */
std::tuple<std::string, XPadType> XPad::getAvailableControllerPath() {
    std::string basePath("/dev/input/");
    std::string devicePath;
    XPadType deviceType;

    DIR *dirp = opendir(basePath.c_str());
    struct dirent *dp;

    while ((dp = readdir(dirp)) != nullptr) {
        std::string candidateName(dp->d_name);

        // Filter out entries that don't start with "event"
        if (candidateName.rfind("event", 0) == std::string::npos) {
            continue;
        }

        std::string candidatePath = basePath + candidateName;

        // Test whether device is openable
        int fd = open(candidatePath.c_str(), O_RDWR);
        if (fd < 0) {
            continue;
        }

        // Get device reported human-readable name using ioctl
        const int maxNameLength = 128;
        char rawCandidateVisibleName[maxNameLength];
        int candidateVisibleNameLength = ioctl(fd, EVIOCGNAME(maxNameLength), rawCandidateVisibleName);
        if (candidateVisibleNameLength < 0) {
            continue;
        }
        std::string candidateVisibleName(rawCandidateVisibleName, candidateVisibleNameLength - 1);

        // Filter out non-xbox devices
        if (candidateVisibleName.find("Xbox") == std::string::npos) {
            continue;
        }

        // At this point we have definitely an Xbox controller that we can connect to
        devicePath = candidatePath;
        deviceType = (candidateVisibleName.find("Wireless") != std::string::npos)
                     ? XPadType::ONE_WIRELESS_BT
                     : XPadType::ONE_WIRED;
        break;
    }

    closedir(dirp);
    return std::make_tuple(devicePath, deviceType);
}

/**
 *
 * @param count
 */
void XPad::rumble(int32_t count) const {
    // Terminate if device doesn't support rumble (i.e. wireless)
    if (this->rumbleEffectId == -1) {
        return;
    }

    // Play force feedback effect
    struct input_event play = {
            {},
            EV_FF,
            static_cast<uint16_t>(this->rumbleEffectId),
            count
    };
    write(this->devicePointer, (const void *) &play, sizeof(play));
}

/**
 *
 */
void XPad::initializeRumble() {
    // Create effect object
    struct ff_effect fx = {
            FF_RUMBLE,
            -1,
            0x0,
            {
                    0,
                    0,
            },
            {
                    100,
                    0
            },
            {}
    };

    // Intialize union variables
    fx.u.rumble.strong_magnitude = 0x2000;
    fx.u.rumble.weak_magnitude = 0x2000;

    // Create effect
    if (ioctl(this->devicePointer, EVIOCSFF, &fx) < 0) {
        this->rumbleEffectId = -1;
        return;
    }

    this->rumbleEffectId = fx.id;
}

