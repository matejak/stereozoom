/*
 * frontend.cpp
 * Copyright (C) Matej Tyc 2007 <muna@stcomp.cz>
 *
 * frontend.cpp is is part of stereozoom2
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



/**\mainpage stereozoom2
 * The viewer of stereo pairs
 * \section req_sec Requirements:
 * 		- installed Allegro 4.2 runtime (liballegro4.2)
 * 		- installed devIL 1.6.7(or something like that) runtime (libdevil1c2)
 * 		- enough RAM :-)
 * \section usage_sec Usage:
 * 	stereozoom2 is a command-line invoked multiplatform viewer of images originally created with intention
 * to easily see and align stereo pictures (images taken by a stereo microscope). Now you can compare an arbitrary number of
 * pictures (providing you have enough memory)
 *
 * The application accepts these parameters:
 *
 * stereozoom2 [-a \<res_x\>x\<res_y\>] \<filenames with format specifiers\>
 *
 * The format specifiers look like that:
 *
 *	 -a \<horiz_res\>x\<vert_res\>
 *	 	'a' stands for 'area'. You can set the resolution of  area  per picture where pictures you are examining are shown.
 *
 * 	-m \<matrix_x\>,\<matrix_y\>
 * 		'm' stands for 'matrix'; as you supply filenames, they are going to be stacked in the matrix according to
 * 		stacking method (growing rows or columns). Only one dimension of the matrix is effective, so if you want to
 * 		have N images in a row, just pass -r (row growing mode) first and then -m N,\<whatever number\>
 *
 * 	-r
 * 		'r' stands for 'row'; it either sets the row growing mode, or it begins a new row (it is like you are writing text on the computer and
 * 		press enter)
 *
 * 	-c
 * 		'c' stands for 'columns'; it is the complement of the above, it is like writing in Japanese :-)
 *
 * 	-s [width]
 * 		's' stands for 'space'; you are going to have a "hole" instead of a picture...  If the width is not specified, then width of 1 is assumed.
 *
 *  -f \<filename\>
 *  	'f'  stands for... 'filename'! If you have some weird filename, you can protect it by passing this parameter first.
 *  Everything that is not recognized as the format specifier or its parameter is assumed as filename.
 *
 * 	-p \<pos_x\>,\<pos_y\>
 * 		'p' stands for 'position'. You can reset  the  position  before specifying a filename. This can be particulary useful if you use
 *		stereozoom2 as a library.
 *
 * Example:
 * 	stereozoom2 -r 200x250 -m 4,3 -r shots*.jpg -r average.jpg -s 2 variance.jpg
 *
 * You are going to use this syntax when displaying some images in a matrix (you don't have to care how many of them are)
 * and then displaying the results of processing in the next row
 *
 * Note that the optimal displayed image width should be around 15cm for stereo viewing
 * (input is naturally in pixels since the application cannot determine the width of your monitor's pixel)
 *
 *  Use mouse dragging for moving across the image plane, wheel for zooming and keyboard arrows for aligning. Have fun!
 */

#include "stereozoom2_internal.h"
#include <allegro.h>
#include "stereozoom2.h"

int main(int argc, char * argv [])
{
	std::string arguments("");
	for(int i = 1; i < argc; i++)	// now let's stack the arguments back like they were on the command line :-)
		arguments += (std::string)argv[i] + "\n";
	stereozoom2 session(arguments.c_str());
	return 0;
}
END_OF_MAIN()
