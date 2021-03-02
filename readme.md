About
=====

This ASTC wrapper allows a C program to compress an ASTC
texture using the C++ astcenc encoder library.

Building astcenc
================

Install clang dependency.

	sudo apt install clang

Build astcenc library.

	# use gcc since we want to link with library from C
	# export CXX=clang++

	mkdir build
	cd build
	cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=./ -DISA_AVX2=ON ..
	make install -j4

References
==========

1. https://github.com/ARM-software/astc-encoder
2. https://www.khronos.org/blog/create-astc-textures-faster-with-the-new-astcenc-2.0-open-source-compression-tool
3. https://nachtimwald.com/2017/08/18/wrapping-c-objects-in-c/
