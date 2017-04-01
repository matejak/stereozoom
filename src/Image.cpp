/*
 * Image.cpp
 * Copyright (C) Matěj Týč 2007-2017 <matej.tyc@gmail.com>
 *
 * Image.cpp is is part of stereozoom2
 *
 * This program is free software;
 * You may redistribute it and/or modify it under the terms of the
 * GNU General Public License, as published by the Free Software
 * Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * stereozoom2 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with stereozoom2.  If not, write to:
 * 	The Free Software Foundation, Inc.,
 * 	51 Franklin Street, Fifth Floor
 * 	Boston, MA  02110-1301, USA.
 */

#include "Image.h"
#include "stereozoom2.h"


void Image::supplyData(unsigned char * rgbData, unsigned int width, unsigned int height, const char * filename)
{
	this->width = width;
	this->height = height;
	this->filename = std::string(filename);

	mkImageDataStructure(rgbData);
}
