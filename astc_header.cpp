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

#include <cstdio>

#include "astc_header.h"

/***********************************************************
* public                                                   *
***********************************************************/

void astc_header_init(astc_header_t* self,
                      unsigned int block_x,
                      unsigned int block_y,
                      unsigned int block_z,
                      unsigned int dim_x,
                      unsigned int dim_y,
                      unsigned int dim_z)
{
	self->magic[0] = 0x13;
	self->magic[1] = 0xAB;
	self->magic[2] = 0xA1;
	self->magic[3] = 0x5C;
	self->block_x  = (uint8_t) block_x;
	self->block_y  = (uint8_t) block_y;
	self->block_z  = (uint8_t) block_z;
	self->dim_x[0] = (uint8_t) (dim_x         & 0xFF);
	self->dim_x[1] = (uint8_t) ((dim_x >> 8)  & 0xFF);
	self->dim_x[2] = (uint8_t) ((dim_x >> 16) & 0xFF);
	self->dim_y[0] = (uint8_t) (dim_y         & 0xFF);
	self->dim_y[1] = (uint8_t) ((dim_y >> 8)  & 0xFF);
	self->dim_y[2] = (uint8_t) ((dim_y >> 16) & 0xFF);
	self->dim_z[0] = (uint8_t) (dim_z         & 0xFF);
	self->dim_z[1] = (uint8_t) ((dim_z >> 8)  & 0xFF);
	self->dim_z[2] = (uint8_t) ((dim_z >> 16) & 0xFF);
}
