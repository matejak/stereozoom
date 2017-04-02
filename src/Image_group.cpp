/*
 * Image_group.cpp
 * Copyright (C) Matěj Týč 2007-2017 <matej.tyc@gmail.com>
 *
 * Image_group.cpp is is part of stereozoom
 *
 * This program is free software;
 * You may redistribute it and/or modify it under the terms of the
 * GNU General Public License, as published by the Free Software
 * Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * stereozoom is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with stereozoom.  If not, write to:
 * 	The Free Software Foundation, Inc.,
 * 	51 Franklin Street, Fifth Floor
 * 	Boston, MA  02110-1301, USA.
 */

#include <cmath>
#include "Image_group.h"



void Crosshair::createNormal(unsigned int size)
{
	// also creates a crosshair bitmap
	prepare(size);
	drawCenteredCircle((size * 2) / 3, 0, 0, 0);
	drawCenteredCircle((size * 1) / 3, 0, 0, 0);
	drawCenteredVline(0, 1.0, 1.0, 1.0, 1.0);
	drawCenteredHline(0, 1.0, 1.0, 1.0, 1.0);
}
