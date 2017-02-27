/*
 * Image_group.cpp
 * Copyright (C) Matěj Týč 2007-2017 <matej.tyc@gmail.com>
 *
 * Image_group.cpp is is part of stereozoom2
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

#include "Image_group.h"

volatile int Image_group::Duration = 0;

Image_group::Image_group()
{
	ilInit();
	ilEnable(IL_ORIGIN_SET);
	allegro_init();

	install_keyboard();
	install_mouse();

	mouse_callback = button_down;

	install_timer();
	install_int( & Timer_callback, 100);
	// necessary due to use of allegro timer
	LOCK_VARIABLE (Transparency);
	LOCK_FUNCTION(Timer_callback);
	// allegro stuff, necessary to make nice "notices"
	set_alpha_blender();

	Notice = 0;
	Crosshair = 0;
}

Image_group::Image_group(const Image_group & rhs)
{
	Zoom_boost = rhs.Zoom_boost;
	Move_boost = rhs.Move_boost;
	Offset_boost = rhs.Offset_boost;

	Zoom_sens = rhs.Zoom_sens;
	Move_sens = rhs.Move_sens;
	Offset_sens = rhs.Offset_sens;

	Current_image = rhs.Current_image;

	copy_array<int,2>(rhs.Img_dim, Img_dim);

	Images = rhs.Images;
	copy_array<int,2>(rhs.Max_coords, Max_coords);

	Notice = create_bitmap(rhs.Notice->w, rhs.Notice->h);
	blit(rhs.Notice, Notice, 0, 0, 0, 0, rhs.Notice->w, rhs.Notice->h);

	Crosshair = create_bitmap(rhs.Crosshair->w, rhs.Crosshair->h);
	blit(rhs.Crosshair, Crosshair, 0, 0, 0, 0, rhs.Crosshair->w, rhs.Crosshair->h);
}

Image_group::~Image_group()
{
	destroy_bitmap(Notice);		Notice = 0;
	destroy_bitmap(Crosshair);	Crosshair = 0;
}

/**
 * \param filename The filename of the image
 * \param x The horizontal coordinate of the image in the program
 * \param y The vertical coordinate
 */
int Image_group::Add_image(string filename, int img_coords[2] )
{
	int coords[2];
	copy_array<int,2>(img_coords, coords);
	// we make a new image, we feed it with the correct data and then we dump it to the vector
	Image new_image;
	new_image.Init(filename.c_str(), Img_dim, coords);
	if (verbose)
		printf("Adding image \"%s\", coords. [%d, %d]\n", filename.c_str(), coords[0], coords[1]);
	Images.push_back(new_image);
	return 0;
}

int Image_group::Init(int img_dims[2], int max_coords[2])
{
	// setting of class variables
	copy_array<int,2>(img_dims, Img_dim);
	copy_array<int,2>(max_coords, Max_coords);

	// init of Allegro
	set_color_depth(desktop_color_depth());
	int hres = Img_dim[0] * (Max_coords[0] + 1),
		vres = Img_dim[1] * (Max_coords[1] + 1);

	int setmode_code = set_gfx_mode(GFX_AUTODETECT_WINDOWED, hres, vres, 0, 0);

	if (test)
		printf("Graphics mode: %dx%d, result: %d\n", hres, vres, setmode_code);

	if (verbose)
		printf("Trying to set resolution: %dx%d, %d bpp\n", hres, vres, get_color_depth());
	//set_gfx_mode(GFX_TEXT, 300, 300, 0, 0);

	//default values
	Notice = NULL;
	Zoom_sens = 0.2;	Zoom_boost = 10.0;
	Move_sens = 2.5;	Move_boost = 2.5;
	Offset_sens = 1.0;	Offset_boost = 2.0;

	return 0;
}

int Image_group::Start()
{
	if (verbose)
		printf("Displaying %d images, matrix %dx%d\n", Images.size(), Max_coords[0] + 1, Max_coords[1] + 1);
	// The thing that marks where the mouse cursor is has to be made here procedurally...
	Make_crosshair();

	if (test == 0)
		Loop();
	// and destroyed afterwards...
	Destroy_crosshair();

	return 0;
}

/**
 * \param text The text we want to display. It will be copied here to a safe location (of course)
 * \param time_decsecs Time for how long (in tenth of seconds) we want the message to be displayed
 * Internal details:
 *
 * First of all, the text is broken into rows. They are then put into vectors. The length of
 * the longest row is recorded. Then the corresponding notice bitmap is made and we tell how long the
 * loop should display it...
 */
void Image_group::Display_notice(const char * text, int time_decsecs)
{
	vector<string> rows;
	string temp;
	unsigned int max_row_len = 0, num_rows = 0;
	for (unsigned int i = 0, current_length = 0; i <= strlen(text); current_length++, i++)
	{// trying to put all rows into a vector and also to determine length of the longest row
		if (text[i] == '\n' || text[i] == '\0')
		{// another row finished!
			rows.push_back(temp);
			num_rows++;
			max_row_len = MAX<int>(max_row_len, current_length);
			// now just setting back to original values
			current_length = 0;
			temp = "";
			continue;
		}
		temp += text[i];	// quite important, filling the string that will be put into the vector later
	}
	if (Notice) // we want to create our own notice :-)
		destroy_bitmap(Notice);
	Notice = create_bitmap(max_row_len * text_length(font, "@") + 10, num_rows * text_height(font) + 10);
	clear_to_color(Notice, makeacol(64, 64, 64, 96));
	for (unsigned int i = 0; i < rows.size(); i++)
		textout_ex(Notice, font, rows[i].c_str(), 5, i * text_height(font) + 5, makeacol32(255,255,255,192), -1);
	Duration = time_decsecs;
}

int Image_group::Loop()
{
	int Num_images = Images.size();
	position_mouse_z(0);
	double boost = 1.0;
	//int mouse_coords[2];

	BITMAP * screen_buffer = create_bitmap(SCREEN_W, SCREEN_H);
	Current_image = 0;
	int loop = 1;
	while(loop)
	{
		//read user input
		{// What is the current image number in the stack?
			int mouse_coords[2] = {mouse_x / Img_dim[0], mouse_y / Img_dim[1]};
			if (Images[Current_image].Get_coords(0) ==  mouse_coords[0] && Images[Current_image].Get_coords(1) ==  mouse_coords[1])
				;// just checking whether it remained the same
			else
				for (int i = 0; i < Num_images ; i++)
					if (Images[i].Get_coords(0) ==  mouse_coords[0] && Images[i].Get_coords(1) ==  mouse_coords[1])
					{// going through all images in the stack...
						Current_image = i;
						break;
					}
		}

		if (mouse_b & 1) //left button is pressed, someone is trying to drag...
		{
			if (key_shifts & KB_SHIFT_FLAG) boost = Move_boost; else boost = 1.0;
			int dmouse_x, dmouse_y;
			get_mouse_mickeys(& dmouse_x, & dmouse_y);
			for (int i = 0; i < Num_images; i++)
				Images[i].Change_pos(- dmouse_x * boost, - dmouse_y * boost);
		}//endif (mouse_b & 1)

		if (mouse_z) //if change of zoom occured
		{
			if (key_shifts & KB_SHIFT_FLAG) boost = Zoom_boost; else boost = 1.0;
			for (int i = 0; i < Num_images; i++)
				Images[i].Change_zoom(mouse_z * Zoom_sens * boost , mouse_x % Img_dim[0] , mouse_y % Img_dim[1]);
			Print_zoom();
		}//endif (mouse_z)
		position_mouse_z(0); // just in order to detect another zoom change correctly

		ProcessUserInput(loop);
		//adjust everything

		//finally, draw everything

		clear_to_color(screen_buffer, makecol(0,0,0));

		for (int i = 0; i < Num_images; i++)
		{
			blit(Images[i], screen_buffer, 0, 0, Images[i].Get_position(0), Images[i].Get_position(1), Img_dim[0], Img_dim[1]);

			draw_sprite(screen_buffer, Crosshair, (mouse_x % Img_dim[0]) + Images[i].Get_position(0) - Crosshair->w / 2, (mouse_y % Img_dim[1]) + Images[i].Get_position(1) - Crosshair->h / 2);
		}
		circle(screen_buffer, mouse_x, mouse_y, 5, makecol(255,255,0)); //to distinguish the real mouse position

		if (Duration)
			draw_trans_sprite(screen_buffer, Notice, 5, 5);

		// double buffering :-)
		blit(screen_buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

		rest(50);	//to be nice to the CPU
	}

	destroy_bitmap(screen_buffer);	screen_buffer = 0;
	return loop;
}


void Image_group::ProcessUserInput(int & loop)
{
	// TODO: DRY violation
	int Num_images = Images.size();
	double boost = 1.0;

	while(keypressed())
	{// process user input
		if (key_shifts & KB_SHIFT_FLAG)
			boost = Move_boost;
		else
			boost = 1.0;
		switch (readkey() >> 8)
		{
			case KEY_A:
				for(int i = 0; i < Num_images; i++)
					Images[i].Change_pos( - Move_sens * boost, 0);
				break;
			case KEY_S:
				for(int i = 0; i < Num_images; i++)
					Images[i].Change_pos( 0, Move_sens * boost);
				break;
			case KEY_D:
				for(int i = 0; i < Num_images; i++)
					Images[i].Change_pos( Move_sens * boost, 0);
				break;
			case KEY_W:
				for(int i = 0; i < Num_images; i++)
					Images[i].Change_pos( 0, - Move_sens * boost);
				break;
			case KEY_EQUALS:
				for (int i = 0; i < Num_images; i++)
					Images[i].Change_zoom(Zoom_sens * boost , -1, -1);
				break;
			case KEY_MINUS:
				for (int i = 0; i < Num_images; i++)
					Images[i].Change_zoom( - Zoom_sens * boost , -1, -1);
				break;
			case KEY_UP:
				if (Current_image != -1)
					Images[Current_image].Change_offset(0, 1);
				Print_offset();
				break;
			case KEY_DOWN:
				if (Current_image != -1)
					Images[Current_image].Change_offset(0, -1);
				Print_offset();
				break;
			case KEY_LEFT:
				if (Current_image != -1)
					Images[Current_image].Change_offset(1, 0);
				Print_offset();
				break;
			case KEY_RIGHT:
				if (Current_image != -1)
					Images[Current_image].Change_offset(-1, 0);
				Print_offset();
				break;
			case KEY_8:
			case KEY_ASTERISK:
				for(int i = 0; i < Num_images; i++)
					Images[i].Show_whole_picture();
				Print_zoom();
				break;
			case KEY_SLASH_PAD:
			case KEY_SLASH:
				for(int i = 0; i < Num_images; i++)
					Images[i].Set_zoom(1.0);
				Display_notice("Zoom ratio 1:1", 10);
				break;
			case KEY_F:
				//display filenames
				break;
			case KEY_H:
				Print_help();
				break;
			case KEY_ESC:
			case KEY_Q:
				/*Display_notice("REALLY QUIT?\nPress 'y' to quit, other keys to cancel", -1);
				if (readkey() >> 8 == KEY_Y) */loop = false;
				break;
		}//endswitch (readkey() >> 8)
	}//endwhile(keypressed())
}

void Image_group::Make_crosshair()
{
	// also creates a crosshair bitmap
	if (Crosshair == NULL)
		Crosshair = create_bitmap(32,32);
	clear_to_color(Crosshair, makecol(255,0,255));
	circle(Crosshair, Crosshair->w / 2, Crosshair->h / 2, (Crosshair->w * 2) / 3, 0);
	circle(Crosshair, Crosshair->w / 2, Crosshair->h / 2, (Crosshair->w * 1) / 3, 0);
	hline(Crosshair, 0, Crosshair->h / 2, Crosshair->w, makecol(255, 255, 255));
	vline(Crosshair, Crosshair->w / 2, 0, Crosshair->h, makecol(255, 255, 255));
}

void Image_group::Print_help()
{
	const char * message = "STEREOZOOM HELP\n\nUse your mouse wheel to zoom in and out.\nand then move it with the mouse.\n\
Hold SHIFT to speed moving and zooming up.\nPress / to the 1:1 ratio and * to fit to the screen.\nFinally, press ESC or q to quit.";
	Display_notice(message, 50);
}

void Image_group::Print_zoom()
{
	char buffer[256];
	sprintf(buffer, "Current zoom: %f", Images[0].Get_zoom());
	Display_notice(buffer, 10);
}

void Image_group::Print_offset()
{
	if (Current_image == -1)
		return;	//print offset of what??
	char buffer[256];
	int offset [2];
	Images[Current_image].Get_offset(offset);
	sprintf(buffer, "Current offset: (x) %d , (y) %d pixels", offset[0], offset[1]);
	Display_notice(buffer, 10);
}
