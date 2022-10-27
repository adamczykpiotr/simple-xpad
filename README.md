# Xbox One Controller library for Linux

Simple library for Xbox one pad/controller. 
Supports both native (usb or wireless dongle) and Bluetooth modes.

### Fully working:
* [x] All buttons (press & release)
* [x] Analogs (left & right, both axes)
* [x] Triggers (left & right)
* [x] Rumble (**only native, doesn't work on Bluetooth!**)
* [x] Scanning for available controllers in the system
* [x] Auto reconnect

## Supported & tested controllers
* Rev 1914 (Red Xbox one controller)
  * [x] USB-C
  * [ ] Dongle (not tested yet)
  * [x] Bluetooth


## Sources & helpful links
* [Xbox Wireless Controller](https://en.wikipedia.org/wiki/Xbox_Wireless_Controller#Third_revision_(2020))
* [Linux force feedback demo](https://gist.github.com/rikusalminen/972e3824350193bbed0c28ff96a82a73)
* [input_event (input.h)](https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/tree/include/uapi/linux/input.h)
