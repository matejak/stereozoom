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


valarray<double> ViewWithRectangle::viewToBitmapCoord(valarray<double> view_coord) const
{
	valarray<double> view_ratio = view_coord / view_size;
	return view_ratio * current_size() + upper_left_corner;
}

valarray<double> ViewWithRectangle::bitmapToViewCoord(valarray<double> bitmap_coord) const
{
	valarray<double> bitmap_ratio = (bitmap_coord - upper_left_corner) / current_size();
	return bitmap_ratio * view_size;
}
