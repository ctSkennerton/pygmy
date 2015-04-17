# pygmy

<b>[This program is in active development and is currently highly unstable.]</b>

Pygmy is a tree viewer for large trees (1 million taxa) and associated metadata. It aims to be a cross-platform replacement for ARB that allows large trees to be explored using modern visualization and navigation techniques.

## Install

We are currently setting up a Linux development environment for this project. This project currently has the following 3rd-party dependencies:

* Image: a helper library for creating images (included in /3rd_party)
* [boost](http://www.boost.org/): free peer-reviewed portable C++ source libraries.
* [freetype](http://www.freetype.org/): software library to render fonts.
* [ftgl](http://sourceforge.net/projects/ftgl/): library that uses Freetype2 to simplify rendering fonts in OpenGL applications.
* [libpng](http://www.libpng.org/pub/png/libpng.html): offcial PNG reference library.
* [ticpp](https://code.google.com/p/ticpp/): tiny XML parser - included as a submodule of this git repository.
* [wxWidgets](https://www.wxwidgets.org/): cross-platform GUI library.
* [zlib](http://www.zlib.net/): an unobtrusive compression library.
* [premake](http://premake.github.io/): which is required to build ticpp

to compile on linux:
git clone --recursive https://github.com/ctSkennerton/pygmy.git
make

## Copyright

Copyright © 2015 Donovan Parks. See LICENSE for further details.
