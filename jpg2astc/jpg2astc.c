/*
 * Copyright (c) 2021 Jeff Boody
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#include <stdlib.h>

#define LOG_TAG "jpg2astc"
#include "libastcwrapper/astc_wrapper.h"
#include "libcc/cc_log.h"
#include "texgz/texgz_jpeg.h"

int main(int argc, char** argv)
{
	if(argc != 3)
	{
		LOGE("usage: %s in.jpg out.astc", argv[0]);
		return EXIT_FAILURE;
	}

	char* src = argv[1];
	char* dst = argv[2];

	astc_wrapper_t* astc = astc_wrapper_new();
	if(astc == NULL)
	{
		return EXIT_FAILURE;
	}

	texgz_tex_t* tex;
	tex = texgz_jpeg_import(src);
	if(tex == NULL)
	{
		goto fail_tex;
	}

	if(texgz_tex_convert(tex, TEXGZ_UNSIGNED_BYTE,
	                     TEXGZ_RGBA) == 0)
	{
		goto fail_convert;
	}

	// extract args from tex
	unsigned int w = (unsigned int) tex->stride;
	unsigned int h = (unsigned int) tex->vstride;
	uint8_t*     p = (uint8_t*)     tex->pixels;

	// compress tex
	uint8_t* buf;
	size_t   size = 0;
	buf = astc_wrapper_compress(astc, w, h, p, &size);
	if(buf == NULL)
	{
		goto fail_compress;
	}

	// save buffer
	FILE* f = fopen(dst, "w");
	if(f == NULL)
	{
		LOGE("fopen failed");
		goto fail_open;
	}

	if(fwrite((void*) buf, size, 1, f) != 1)
	{
		LOGE("fwrite failed");
		goto fail_write;
	}

	fclose(f);
	texgz_tex_delete(&tex);
	astc_wrapper_delete(&astc);

	// success
	return EXIT_SUCCESS;

	// failure
	fail_write:
		fclose(f);
	fail_open:
	fail_compress:
	fail_convert:
		texgz_tex_delete(&tex);
	fail_tex:
		astc_wrapper_delete(&astc);
	return EXIT_FAILURE;
}
