# TermboxWidgets -- A high/low-level library for tui based programs
**Disclaimer:** This an early release of the library, the underlying concepts of the api may heavily change in the future.

## Documentation
To generate the documentation, run `doxygen default` in the `doc/` directory.
This documentation is for extending the library, it is not a documentation for building applications with TermboxWidgets.

## Building
```
git clone http://git.pundalik.org/ef3d0c3e/termbox src/termbox
mkdir build && cd build
cmake ..
make
./termbox-widgets
```

## Example window
In the folder `src/my/`, you will find an example window.

## Events
 * Keyboard/Mouse driven events, in `src/Input.hpp`: will trigger a redrawing of the widget from which they originate
 * Timed (async) events, in `src/Timed.hpp`: can happen at anytime, redrawing will be the job of the event (lowish level)
 * EventListener/Signals, in `src/Listener.hpp`

## Current issues
 * The `InputLine` does not work.
 * `Resizeable` pure abstract class has not been implemented (i.e resizing a window will make it dissapear)
 * Lack of consistency in `src/Settings.hpp`
 * No (true) mutex for timed events

## External libraries
 * This project makes use of Termbox
 * I have also written two helper headers: `src/Vector.hpp` and `src/Cenum.hpp`, these are standalone headers and may be re-used under the GNU GPLv3 license.

## License
This project is licensed under the GNU GPL version 3 <<https://www.gnu.org/licenses/gpl-3.0.en.html>>
