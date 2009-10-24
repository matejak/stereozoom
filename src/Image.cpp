/*
 * Image.cpp
 * Copyright (C) Matej Tyc 2007 <muna@stcomp.cz>
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

Image::Image()
{
	Pic = 0;
	Img = 0;
}

Image::Image(const Image & rhs)
{
	copy_array<int,2>(rhs.Pic_dim, Pic_dim);
	copy_array<int,2>(rhs.Img_dim, Img_dim);
	copy_array<int,2>(rhs.Img_coords, Img_coords);

	Pic = create_bitmap(Pic_dim[0], Pic_dim[1]);
	blit(rhs.Pic, Pic, 0, 0, 0, 0, Pic_dim[0], Pic_dim[1]);

	Img = create_bitmap(Img_dim[0], Img_dim[1]);
	blit(rhs.Img, Img, 0, 0, 0, 0, Img_dim[0], Img_dim[1]);

	Rect = rhs.Rect; ///< Rect controls carrying out of user's orders
}

/// This function performs initialization of everything that may change
/**
 * \param filename Filename of the displayed picture
 * \param img_dim Array of image dimensions
 * \param coords Array of image position coordinates
 */
int Image::Init(const char * filename, int img_dim[2], int coords[2])
{
	copy_array<int,2>(img_dim, Img_dim);
	copy_array<int,2>(coords, Img_coords);
	Img = create_bitmap(Img_dim[0], Img_dim[1]);
	//first some IL warmup
	ILuint loaded_image;
	ilGenImages(1, & loaded_image);
	ilBindImage(loaded_image);
	ilLoadImage((char *)filename);
	//then some spying on the image
	Pic_dim[0] = ilGetInteger(IL_IMAGE_WIDTH);
	Pic_dim[1] = ilGetInteger(IL_IMAGE_HEIGHT); //filling clas vars
	if (verbose)
		printf("Image resolution: %dx%d\n", Pic_dim[0], Pic_dim[1]);
	//IL memory alloc
	int to_allocate = Pic_dim[0] * Pic_dim[1] * 3;
	unsigned char * temp_space = (unsigned char *) new unsigned char [to_allocate];
	ilCopyPixels(0, 0, 0, Pic_dim[0], Pic_dim[1], 1, IL_RGB, IL_UNSIGNED_BYTE, (void *)temp_space );
	//Allegro bitmap creation
	Pic = create_bitmap(Pic_dim[0], Pic_dim[1]);
	for (int i = 0; i < Pic_dim[1]; i++)
		for (int j = 0; j < Pic_dim[0]; j++)
		{// conversion from IL colours to Allegro ones (the format is colour depth dependent), not so slow as feared :-)
			int color_pos = (i * Pic_dim[0] + j) * 3;
			int color = makecol(temp_space[color_pos], temp_space[color_pos + 1], temp_space[color_pos + 2]);
			putpixel(Pic, j, Pic_dim[1] - i - 1, color);	//flipping the image upside down seems to be quite smart :-)
		}

	ilDeleteImages(1, & loaded_image);
	delete [] temp_space;

	Rect.Init(Pic_dim, Img_dim);

	if (test)
	{// tests to see whether everything goes smoothly...
		printf("Image added!\n");
		printf("\tImg filename: \t%s\n\tResolution \t%dx%d\n", filename, Pic_dim[0], Pic_dim[1]);
		printf("\tAllocated: \t%d\n", to_allocate);
		int coord_x = 3, coord_y = 5;
		int color = getpixel(Pic, coord_x, coord_y);
		unsigned char
			r = getr(color),
			g = getg(color),
			b = getb(color);
		printf("\tColor at [%d,%d]:\t(%u,%u,%u)\n", coord_x, coord_y, r, g, b);
	}

	return 0;
}

void Image::Destroy()
{
	if (Img)
	{
		destroy_bitmap(Img);
		Img = 0;
	}
	if (Pic)
	{
		destroy_bitmap(Pic);
		Pic = 0;
	}
}

Image::operator BITMAP * ()
{
	int source[4], dest[4];
	Rect.Set_area(source, dest);
	clear_to_color(Img, 0);
	//negative parameters passed are O.K.
	stretch_blit(Pic, Img, source[ULX], source[ULY], source[LRX] - source[ULX], source[LRY] - source[ULY], dest[ULX], dest[ULY], dest[LRX] - dest[ULX], dest[LRY] - dest[ULY]);
	return Img;
}

void Image::Show_whole_picture()
{
	double image_ratio = Img_dim[0] / (double)Img_dim[1];
	double picture_ratio = Pic_dim[0] / (double)Pic_dim[1];
	if (image_ratio > picture_ratio) 	// the picture is proportionally higher than the image
		Set_zoom(Img_dim[1] / (double)Pic_dim[1], -2, -2);
	else  								// the picture is proportionally wider than the image
		Set_zoom(Img_dim[0] / (double)Pic_dim[0], -2, -2);
	if (test)
	{

	}
}
