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

#include <cstdlib>
#include <cstdio>

#include "../astc-encoder/Source/astcenc.h"
#include "astc_header.h"
#include "astc_wrapper.h"

typedef struct astc_wrapper_s
{
	size_t   size;
	uint8_t* buffer;

	astcenc_config cfg;

	astcenc_context* ctx;
} astc_wrapper_t;

/***********************************************************
* public                                                   *
***********************************************************/

astc_wrapper_t* astc_wrapper_new(void)
{
	astc_wrapper_t* self;
	self = (astc_wrapper_t*)
	       calloc(1, sizeof(astc_wrapper_t));
	if(self == NULL)
	{
		printf("calloc failed\n");
		return NULL;
	}

	if(astcenc_config_init(ASTCENC_PRF_LDR, 4, 4, 1,
	                       ASTCENC_PRE_THOROUGH, 0,
	                       self->cfg) != ASTCENC_SUCCESS)
	{
		printf("astcenc_config_init failed\n");
		free(self);
		return NULL;
	}

	astcenc_context_alloc(self->cfg, 1, &self->ctx);
	if(self->ctx == NULL)
	{
		printf("astcenc_context_alloc failed\n");
		free(self);
		return NULL;
	}

	return self;
}

void astc_wrapper_delete(astc_wrapper_t** _self)
{
	astc_wrapper_t* self = *_self;
	if(self)
	{
		astcenc_context_free(self->ctx);
		free(self->buffer);
		free(self);
		*_self = NULL;
	}
}

uint8_t* astc_wrapper_compress(astc_wrapper_t* self,
                               unsigned int width,
                               unsigned int height,
                               uint8_t* pixels,
                               size_t* _size)
{
	astcenc_config*  cfg = &self->cfg;
	astcenc_context* ctx = self->ctx;

	// check for valid image size
	if(((width  % cfg->block_x) != 0) ||
	   ((height % cfg->block_y) != 0))
	{
		printf("invalid width=%u, height=%u\n",
		     width, height);
		return NULL;
	}

	// initialize swizzle
	astcenc_swizzle swizzle =
	{
		.r = ASTCENC_SWZ_R,
		.g = ASTCENC_SWZ_G,
		.b = ASTCENC_SWZ_B,
		.a = ASTCENC_SWZ_1
	};

	// get the block count
	unsigned int bx = width/cfg->block_x;
	unsigned int by = height/cfg->block_y;
	unsigned int bz = 1;

	// ASTC blocks are 128-bits or 16 bytes
	size_t hsize = sizeof(astc_header_t);
	size_t bsize  = bx*by*bz*16;
	size_t size   = hsize + bsize;

	// resize buffer if needed
	if(size > self->size)
	{
		uint8_t* buffer;
		buffer = (uint8_t*) realloc(self->buffer, size);
		if(buffer == NULL)
		{
			printf("realloc failed\n");
			return NULL;
		}

		self->size   = size;
		self->buffer = buffer;
	}

	astc_header_init((astc_header_t*) self->buffer,
	                 cfg->block_x,
	                 cfg->block_y,
	                 cfg->block_z,
	                 width, height, 1);

	astcenc_image image =
	{
		.dim_x     = width,
		.dim_y     = height,
		.dim_z     = 1,
		.data_type = ASTCENC_TYPE_U8,
		.data      = (void**) &pixels
	};

	if(astcenc_compress_image(ctx, image, swizzle,
	                          &self->buffer[hsize], bsize,
	                          0) != ASTCENC_SUCCESS)
	{
		printf("astcenc_compress_image failed\n");
		return NULL;
	}

	if(astcenc_compress_reset(self->ctx) != ASTCENC_SUCCESS)
	{
		printf("astcenc_compress_reset failed\n");
		return NULL;
	}

	*_size = size;

	return self->buffer;
}
