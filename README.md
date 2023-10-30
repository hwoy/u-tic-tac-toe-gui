![](https://raw.githubusercontent.com/hwoy/u-tic-tac-toe-gui/master/res/pic1.png?raw=true)

# u3t-gui

u3t-gui is a GUI version of micro Tic-Tac-Toe game (or called Unbeatable Tic-Tac-Toe Game).

## Features

1. Full open source.
2. Cross platform ability.
3. Supports C++11/14 , possible latest STD C++
4. 100% of C99 ``` long long in RNG lib ``` compatible for game core and 100% of ISO C++14 ``` Generic lambda ``` for front-end GUI.
5. Unbeatable AI.
6. Cool wxWidgets GUI.

## Requirement

1. GUI: wxWidgets Stable Release 3.2.3 from <http://wxwidgets.org/downloads/>
2. C/C++ toolchain: MingGW+clang MSYS2 <https://www.msys2.org> (used in development)

Try it if you can!


## How to build wxWidgets 64 bit

```sh

git clone --recurse-submodules --depth 1 --branch v3.2.3 https://github.com/wxWidgets/wxWidgets.git
cd wxWidgets
mkdir x64
cd x64
cmake -DCMAKE_BUILD_TYPE="Release" -DCMAKE_INSTALL_PREFIX=/c/wx64 ..
ninja -j8
ninja install

```

## How to build wxWidgets 32 bit

```sh

git clone --recurse-submodules --depth 1 --branch v3.2.3 https://github.com/wxWidgets/wxWidgets.git
cd wxWidgets
mkdir x32
cd x32
cmake -DCMAKE_BUILD_TYPE="Release" -DCMAKE_INSTALL_PREFIX=/c/wx32 ..
ninja -j8
ninja install

```

## How to build 64 bit

```sh

git clone --recurse-submodules https://github.com/hwoy/u-tic-tac-toe-gui.git
cd u-tic-tac-toe-gui
make distclean
make

```

## How to build 32 bit

```sh

git clone --recurse-submodules https://github.com/hwoy/u-tic-tac-toe-gui.git
cd u-tic-tac-toe-gui
make -f Makefile.32 distclean
make -f Makefile.32

```

## Contact me

- Web: <https://github.com/hwoy>
- Email: <mailto:bosskillerz@gmail.com>
- Facebook: <https://www.facebook.com/watt.duean>
