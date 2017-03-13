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


void AllegroImage::clean()
{
	if (bitmap)
	{
		// TODO: Should not be called before Allegro exits
		destroy_bitmap(bitmap);
		bitmap = 0;
	}
}


void ILLoader::loadFromFileToImage(const char * filename, Image * result) const
{
	// IL overhead
	ILuint loaded_image;
	ilGenImages(1, & loaded_image);
	ilBindImage(loaded_image);
	ilLoadImage(filename);

	// We read parameters and data
	unsigned int width = ilGetInteger(IL_IMAGE_WIDTH);
	unsigned int height = ilGetInteger(IL_IMAGE_HEIGHT);
	int to_allocate = width * height * 3;
	unsigned char * image_data = (unsigned char *) new unsigned char [to_allocate];
	ilCopyPixels(0, 0, 0, width, height, 1, IL_RGB, IL_UNSIGNED_BYTE, (void *)image_data );

	// We create the result
	result->supplyData(image_data, width, height, filename);
	delete [] image_data;
	ilDeleteImages(1, & loaded_image);
	// The image itself takes care of deleting the data
}


void AllegroImage::mkImageDataStructure(unsigned char * rgbdata)
{
	clean();
	bitmap = create_bitmap(width, height);
	int color_pos, color;
	for(int jj = 0; jj < height; jj++)
	{
		for(int ii = 0; ii < width; ii++)
		{
			color_pos = (jj * width + ii) * 3;
			color = makecol(rgbdata[color_pos], rgbdata[color_pos + 1], rgbdata[color_pos + 2]);
			// for some reason, it is better to flip the image upside down
			putpixel(bitmap, ii, height - jj - 1, color);
		}
	}
}
