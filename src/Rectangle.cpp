/*
 * Rectangle.cpp
 * Copyright (C) Matěj Týč 2007-2017 <matej.tyc@gmail.com>
 *
 * Rectangle.cpp is is part of stereozoom2
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
#include "Rectangle.h"

#include <cstdio>
#include "stereozoom2.h"


int Rectangle::Init(int pic_dim[2], int img_dim[2])
{
	copy_array<int,2>(pic_dim, Pic_dim);
	copy_array<int,2>(img_dim, Img_dim);
	copy_array<double,2>(0, Pos);
	copy_array<int,2>(0, Offset);
	Zoom = 0.4;
	D_zoom(0.0, Pos);
	return 0;
}

/**
 * Easy transmission of useful values to blit.
 * This works only because arrays are passed as pointers.
 * \param source What to blit? {ULX, ULY, LRX, LRY}
 * \param dest Where to blit? {ULX, ULY, LRX, LRY}
 */
void Rectangle::Set_area(int source [4], int dest[4])
{
	double src_double[4], dest_double[4];
	for (int i = 0; i < 2; i++)
	{//setting up the offsets:
		src_double[i + ULX] = floor(Pos[i] + Offset[i] + Out[i + ULX]);
		src_double[i + LRX] = ceil(Pos[i] + Offset[i] + Out[i + LRX] + Dim[i]);
	}

	Pic_to_img(& src_double [ULX], & dest_double [ULX]);
	Pic_to_img(& src_double [LRX], & dest_double [LRX]);

	for (int i = 0; i < 2; i++)
	{
		source[i + ULX] = floor(src_double[i + ULX] + 0.5);
//		if(source[i + ULX] < 0)
//		{
//			fprintf(stderr, "trying to blit out of bounds: source[%d] = %d\n", i + ULX, source[i + ULX]);
//			source[i + ULX] = 0;
//		}
		source[i + LRX] = floor(src_double[i + LRX] + 0.5);
		if(source[i + LRX] > Pic_dim[i])
		{//this happens when lower zoom values
		//	fprintf(stderr, "trying to blit out of bounds: source[%d] = %d\n", i + LRX, source[i + LRX]);
			source[i + LRX] = Pic_dim[i];
		}
		dest[i + ULX] = floor(dest_double[i + ULX] + 0.5);
		dest[i + LRX] = floor(dest_double[i + LRX] + 0.5);
	}
}

void Rectangle::Pic_to_img(double pic[2], double img[2])
{
	for (int i = 0; i < 2; i++)
		img[i] = (pic[i] - (Pos[i] + Offset[i])) * Zoom;
}

void Rectangle::Img_to_pic(double img[2], double pic[2])
{
	for (int i = 0; i < 2; i++)
		pic[i] = img[i] / Zoom + (Pos[i] + Offset[i]);
}
/**
 * Checks whether
 *  - something from picture is visible on the screen
 * \return
 *  - 0 if OK
 *  - 1 if NOT_OK
 */
int Rectangle::Check()
{
	for (int i = 0; i < 2; i++)
	{// not taking offset into account since it can cause troubles
		Out[i + ULX]	= MAX(0.0, - (Pos[i] + Offset[i])); //Upper left corner
		Out[i + LRX] 	= MIN(0.0, - (Pos[i] + Offset[i] + Dim[i] - Pic_dim[i])); //lower right, notice that Pic_dim[i] is used there
		if (Out[i + ULX] >= Dim[i]/* + Offset[i]*/) return 1; //the picture will be invisible - left upper corner
		if (- Out[i + LRX] >= Dim[i]/* + Offset[i]*/) return 1; //the picture will be invisible - right lower corner
	}
	return 0;
}

void Rectangle::D_zoom(double change, double focus_point [2])
{
	// focus_point == the 'solid point' on the picture (by default)
	double Focus_point[2];
	for (int i = 0; i < 2; i++)
	{
		if (focus_point[i] == -1.0)
			Focus_point[i] = Img_dim[i] / 2.0;
		else
			Focus_point[i] = focus_point[i];
	}

	double old_Dim[2], old_Pos[2];
	double old_Zoom;
	copy_array<double,2> (Dim, old_Dim);
	copy_array<double,2> (Pos, old_Pos);
	old_Zoom = Zoom;

	double temp[2] = {-1.0, -1.0}; //some vars to return results of transform
	Img_to_pic(Focus_point, temp);

	Zoom *= (1 + change); //I have to add the value first, so optimization ends here ! ;-R

	for (int i = 0; i < 2; i++)	//setting right dimension of the rectangle
	{
		Dim[i] = Img_dim[i] / Zoom;
		Pos[i] = temp[i] - Focus_point[i] / Zoom - Offset[i]; // Pos + Offset is everywhere, so now I have to subtract the Offset...
	}
	if (focus_point[0] == -2 && focus_point[1] == -2)
	{ // when we want to center the picture
		for (int i = 0; i < 2; i++)
			Pos[i] = - Img_dim[i] / 2.0 / Zoom + Pic_dim[i] / 2.0;
	}
	if (Check() != 0)
	{
		copy_array<double,2> (old_Dim, Dim);
		copy_array<double,2> (old_Pos, Pos);
		Zoom = old_Zoom;
		Check();
	}
}

void Rectangle::D_pos(double change [2])
{
	double old_Pos[2];
	copy_array<double,2> (Pos, old_Pos);

	for (int i = 0; i < 2; i++)
		Pos[i] += change[i] / Zoom;

	if (Check() != 0)
	{
		copy_array<double,2> (old_Pos, Pos);
		Check();
	}
}

void Rectangle::D_offset(int offset [2])
{
	for (int i = 0; i < 2; i++)
		Offset[i] += offset[i];
	Check();
}
