/*
 * stereozoom2.cpp
 * Copyright (C) Matěj Týč 2007-2017 <matej.tyc@gmail.com>
 *
 * stereozoom2.cpp is is part of stereozoom2
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

#include "stereozoom2_internal.h"

#include <cstdlib>
#include "Image_group.h"

void start()
{
	allegro_init();

	install_keyboard();
	install_mouse();

	install_timer();
	set_alpha_blender();
}


stereozoom2::stereozoom2(const char * arguments)
{
	string args = arguments;	//not to destroy the passed string (that is moreover const)
	//some init
	Expected = EXP_NONE;
	Grow_mode = GROW_DEFAULT;
	copy_array<int,2>(0,Coords);
	copy_array<int,2>(0,Matrix_size);
	copy_array<int,2>(0,Max_coords);

	Resolution[0] = 300; Resolution[1] = 200;	//this is default resolution

	int index_last = 0;
	while (args.size() > 0)
	{
		index_last = args.find_first_of('\n', 0);
		Parse_args(args.substr(0, index_last).c_str());
		args.erase(0, index_last + 1);
	}//endwhile (args.size() > 0)
	if (Entries.size() == 0)
	{// no files specified
		fprintf(stderr, "No image filenames found among program arguments...\n");
		return;
	}

	verbose = true;
	start();
	AllegroImageGrid stereotuple(Max_coords[0] + 1, Max_coords[1] + 1, 400, 300);
	AllegroUI ui(stereotuple);
	ui.createBuffer();
	ILLoader loader;
	for (unsigned int i = 0; i < Entries.size(); i++)
		stereotuple.LoadImageWhere(Entries[i].Filename.c_str(), Entries[i].Coords[0], Entries[i].Coords[1], & loader);
	ui.mainLoop();
	/*
	{// we need  stereopair to be destroyed before calling allegro_exit(); this is why this block is here...
		Image_group stereopair;
		stereopair.Init(& Resolution[0], & Max_coords[0]);
		for (unsigned int i = 0; i < Entries.size(); i++)
			stereopair.Add_image(Entries[i].Filename, & Entries[i].Coords[0]);

		stereopair.Start();
	}
	*/
}


int stereozoom2::Parse_args(const char * input)
{
	if (input[0] == '-')
	{//some argument passed
		if (Expected == EXP_FILE)
			goto IS_FILE;
		if (Expected & (EXP_RES | EXP_MATRIX | EXP_POSITION)) //arguments don't begin with '-'
			fprintf(stderr, "You have specified an option that requires an argument, but you haven't supplied a correct one (now parsing parameter %s)", input);
		switch(input[1])
		{
			case 'r':	// 'r' as ROW
				if (Grow_mode == GROW_ROWS)
				{// we go to another line since we were in row mode earlier
					Coords[1]++;
					Max_coords[1] = MAX<int>(Coords[1], Max_coords[1]);
					Coords[0] = 0;
				}
				else	//or we just change the mode
					Grow_mode = GROW_ROWS;
				Expected = EXP_NONE;
				return MTL_FORMAT;
				break;
			case 'c':	// 'c' as COLUMN
				if (Grow_mode == GROW_COLS)
				{// we go to another column since we were in col mode earlier
					Coords[0]++;
					Max_coords[0] = MAX<int>(Coords[0], Max_coords[0]);
					Coords[1] = 0;
				}
				else	//or we just change the mode
					Grow_mode = GROW_COLS;
				Expected = EXP_NONE;
				return MTL_FORMAT;
				break;
			case 's':	// 's' as SPACE
				Expected = EXP_SPACE;
				return MTL_EXPECT;
				break;
			case 'm':	// 'm' as MATRIX
				Expected = EXP_MATRIX;
				return MTL_EXPECT;
				break;
			case 'f':	// 'f' as FILE
				Expected = EXP_FILE;
				return MTL_EXPECT;
				break;
			case 'p':	// 'p' as POSITION
				Expected = EXP_POSITION;
				return MTL_EXPECT;
				break;
			case 'a':	//'a' as AREA
				Expected = EXP_RES;
				return MTL_EXPECT;
				break;
			case '?':
			case 'h':	// help message
				Expected = EXP_NONE;
				return MTL_HELP;
				break;
			case '-':	//longer option (like --verbose or --test)
				if (strncmp("--verbose", input, sizeof("x--verbose")) == 0)
					verbose = 1;
				if (strncmp("--test", input, sizeof("x--test")) == 0)
				{
					test = 1;
					printf("Testing mode!\n");
				}
				break;
			default:
				;
				//warning about unknown option
		}//endswitch(matlage[1])
	}
	else //possibly a filename was passed
	{
		switch(Expected)
		{
			case EXP_RES:	//We expect parameter defining resolution
				Resolution[0] = 200;
				Resolution[0] = 300;	//assuming 200x300
				if (sscanf(input, "%dx%d", & Resolution[0], & Resolution[1]) != 2)	//scanning not successful, maybe they use ',' instead of 'x' ?
					if(sscanf(input, "%d,%d", & Resolution[0], & Resolution[1]) == 0)
						goto IS_FILE;	//no luck at all, it is most likely a file!
				Expected = EXP_NONE;
				return MTL_RESOLUTION;
				break;
			case EXP_MATRIX:	//We expect parameter defining matrix size
				Matrix_size[0] = Matrix_size[1] = 0;	//assuming zero...
				if (sscanf(input, "%d,%d", & Matrix_size[0], & Matrix_size[1]) != 2) //scanning not successful, maybe they use 'x' instead of ',' ?
					if (sscanf(input, "%dx%d", & Matrix_size[0], & Matrix_size[1]) == 0)
						goto IS_FILE;	//no luck at all, it is most likely a file!
				Expected = EXP_NONE;
				return MTL_MATRIX;
				break;
			case EXP_POSITION:	//We expect defining the exact image position
				if (sscanf(input, "%d,%d", & Coords[0], & Coords[1]) == 0)
					goto IS_FILE;	//no luck at all, it is most likely a file!
				Expected = EXP_NONE;
				return MTL_POSITION;
				break;
			case EXP_SPACE:
			{//We expect defining the space size and if not supplied, lets define it to 1
				int scanf_return;
				int space_number = 1;
				scanf_return = sscanf(input, "%d", & space_number);
				if (Grow_mode != GROW_COLS)	// leave this alone, we have also enum GROW_DEFAULT!!!
					Coords[0] += space_number;
				else
					Coords[1] += space_number;

				Apply_matrix();	// We have moved with the coordinates, so we have to check it!

				Expected = EXP_NONE;
				if (scanf_return == 0)	// actually something else than a number of spaces was passed...
					goto IS_FILE;
				return MTL_FORMAT;
				break;
			}//endcase EXP_SPACE
			case EXP_NONE:
			case EXP_FILE:	//not really needed here, but why not...
				IS_FILE:
				Expected = EXP_NONE;
				if (access(input, R_OK))
				{//We actually should have read permissions for the file :-)
					fprintf(stderr, "The filename \"%s\" is either invalid, or you don't have read permissions...\n", input);
					return MTL_ERRORRE;
				}

				Entry entry(input, & Coords[0]);
				Entries.push_back(entry);

				//We have to compute max coords first, not afrer modifying Coords variable! Otherwise they would be one step ahead...
				Max_coords[0] = MAX<int>(Coords[0], Max_coords[0]);
				Max_coords[1] = MAX<int>(Coords[1], Max_coords[1]);

				if (Grow_mode != GROW_COLS) // leave this alone, we have also enum GROW_DEFAULT!!!
					Coords[0]++;
				else
					Coords[1]++;

				Apply_matrix();

				return MTL_FILENAME;
				break;
		}//endswitch(expected)
	}//endelse(input[0] == '-')
	return MTL_ERRORRE;
}

/**
 * If the Matrix_size[i] == 0, the matrix is not effective...
 */
void stereozoom2::Apply_matrix()
{
	if (Grow_mode != GROW_COLS && Matrix_size[0] > 0)	// leave this alone, we have also enum GROW_DEFAULT!!!
	{
		Coords[1] += Coords[0] / Matrix_size[0];
		Coords[0] %= Matrix_size[0];
	}
	else	//mode is GROW_COLS
		if (Matrix_size[1] > 0)
		{// and the matrix is effective
			Coords[0] += Coords[1] / Matrix_size[1];
			Coords[1] %= Matrix_size[1];
		}
}

int verbose = 0, test = 0;
