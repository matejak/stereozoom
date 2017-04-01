
#define UI
#include "module_interface.h"

#include "AllegroImageGroup.h"
#include "AllegroImage.h"


void * PUBLIC(get_ui)()
{
	auto sensitivity = new AllegroSensitivity();
	auto ret = new AllegroUI(sensitivity);
	return reinterpret_cast<void *>(ret);
}


void PUBLIC(delete_ui)(void * loader)
{
	auto todelete = reinterpret_cast<AllegroUI *>(loader);
	delete todelete->sensitivities;
	delete todelete;
}


void PUBLIC(init)()
{
	allegro_init();

	install_keyboard();
	install_mouse();

	install_timer();
	set_alpha_blender();
}


void PUBLIC(exit)()
{
	allegro_exit();
}


void AllegroImageGrid::blit()
{
	int vertical_offset, horizontal_offset;
	for (int ii = 0; ii < images.size(); ii++)
	{
		valarray<unsigned int> image_xy = indexToXY(ii);
		horizontal_offset = image_xy[X] * view_size[X];
		vertical_offset = image_xy[Y] * view_size[Y];

		AllegroImage * image = (AllegroImage *)images[ii];
		BlitData coords = views[ii]->getBlitData();
		stretch_blit(image->bitmap, buffer, 
				coords.from_start[X], coords.from_start[Y],
				coords.getFromSize()[X], coords.getFromSize()[Y],
				coords.to_start[X] + horizontal_offset, coords.to_start[Y] + vertical_offset,
				coords.getToSize()[X], coords.getToSize()[Y]
			    );
	}
}


void AllegroImageGrid::drawCrosshairs(unsigned int coord_x, unsigned int coord_y)
{
	valarray <unsigned int> base_coordinate = XY<unsigned int>(coord_x % view_size[X], coord_y % view_size[Y]);
	for (int ii = 0; ii < images.size(); ii++)
	{
		valarray<unsigned int> base_coordinate_multiplier = indexToXY(ii);
		valarray<unsigned int> destination = base_coordinate_multiplier * view_size + base_coordinate;
		if (destination[X] == coord_x && destination[Y] == coord_y)
			focused_crosshair.draw(destination[X], destination[Y]);
		else
			normal_crosshair.draw(destination[X], destination[Y]);
	}
}


void AllegroImageGrid::loadImageWhere(const char * filename, int x, int y, const Loader * loader)
{
	int image_index = xyToIndex(x, y);
	AllegroImage * image = (AllegroImage *)images[image_index];
	ViewWithRectangle * view = views[image_index];
	if (image != nullptr)
	{
		delete image;
	}
	if (view != nullptr)
	{
		delete view;
	}
	image = new AllegroImage();
	images[image_index] = image;
	loader->loadFromFileToImage(filename, image);

	view = new ViewWithRectangle(view_size[X], view_size[Y], image->width, image->height);
	views[image_index] = view;
}


valarray<int> AllegroImageGrid::getViewCoordinates() const
{
	valarray<int> local_mouse_position = XY<int>(mouse_x, mouse_y) % XY<int, unsigned int>(view_size);
	return local_mouse_position;
}


void AllegroMessageService::displayMessages() const
{
	vector<string> rows;
	string temp, text;
	int color;
	unsigned int max_row_len = 0, current_row_height = 0, previous_row_end = 0, row_break = 5;
	double relative_alpha = 0;
	for (auto it = messages.begin(); it != messages.end(); it++)
	{
		text = (* it)->getMessageText();
		for (unsigned int i = 0, current_length = 0; i <= text.size(); current_length++, i++)
		{// trying to put all rows into a vector and also to determine length of the longest row
			if (text[i] == '\n' || text[i] == '\0')
			{// another row finished!
				rows.push_back(temp);
				max_row_len = MAX<int>(max_row_len, current_length);
				// now just setting back to original values
				current_length = 0;
				temp = "";
				continue;
			}
			temp += text[i];	// quite important, filling the string that will be put into the vector later
		}
		text += "\n";
		relative_alpha = convertTimeRemainingToAlpha((* it)->getRemainingSeconds());
		color = makeacol(64, 64, 64, int(relative_alpha * 96));
		current_row_height = rows.size() * text_height(font) + 10;

		// TODO: This should be part of a yet-to-be-introduced AllegroMessageRecord
		BITMAP * notice = create_bitmap(max_row_len * text_length(font, "@") + 10, current_row_height);
		clear_to_color(notice, color);
		for (unsigned int i = 0; i < rows.size(); i++)
		{
			color = makeacol32(255,255,255, int(relative_alpha * 192));
			textout_ex(notice, font, rows[i].c_str(), 5, i * text_height(font) + 5, color, -1);
		}
		previous_row_end += row_break;
		draw_trans_sprite(* screen_buffer_ptr, notice, row_break, previous_row_end);
		previous_row_end += current_row_height;
		destroy_bitmap(notice);
		rows.clear();
	}
}


vector<int> AllegroUI::processUIControl(vector<int> keystrokes)
{
	int key;
	vector<int> unprocessed_keystokes;
	for (int ii = 0; ii < keystrokes.size(); ii ++)
	{
		key = keystrokes[ii];
		switch (key)
		{
			case KEY_H:
				printHelp();
				break;
			case KEY_ESC:
			case KEY_Q:
				dont_stop = false;
				break;
			default:
				unprocessed_keystokes.push_back(key);
				break;
		}
	}
	return unprocessed_keystokes;
}


void AllegroUI::initGfxMode(unsigned int hres, unsigned int vres)
{
	set_color_depth(desktop_color_depth());

	int setmode_code = set_gfx_mode(GFX_AUTODETECT_WINDOWED, hres, vres, 0, 0);

	if (verbose)
		printf("Trying to set resolution: %dx%d, %d bpp\n", hres, vres, get_color_depth());

	screen_buffer = create_bitmap(hres, vres);
}


vector<int> AllegroUI::pollForKeystrokes()
{
	vector<int> result;
	while(keypressed())
	{
		result.push_back(readkey() >> 8);
	}
	return result;
}


void AllegroUI::processMouseDrag()
{
	bool boosting_input = false;
	if (mouse_b & 1) //left button is pressed, someone is trying to drag...
	{
		int dmouse_x, dmouse_y;
		get_mouse_mickeys(& dmouse_x, & dmouse_y);
		if (dragging_now)
		{
			double base_sensitivity = sensitivities->get(OF_SHIFT, BY_MOUSE);
			ChangeDrag change = ChangeDrag(- base_sensitivity * dmouse_x, - base_sensitivity * dmouse_y);
			stereotuple->applyChangeToAll(& change);
		}
		// get_mouse_mickeys() will reset the counter, so we won't get garbage from it next time. 
		dragging_now = true;
	}
	else
	{
		dragging_now = false;
	}
}


void AllegroUI::processMouseZoom()
{
	bool boosting_input = false;
	if (mouse_z) //if change of zoom occured
	{
		// TODO: pass the mouse coords along, get the right transform to view coords, apply the transform on the view.
		ChangeZoomViewGeneral change = ChangeZoomViewGeneral(pow(sensitivities->get(OF_ZOOM, BY_MOUSE), mouse_z), stereotuple->getViewCoordinates());
		stereotuple->applyChangeToAll(& change);
		// TODO: Print_zoom();
	}
	position_mouse_z(0); // just in order to detect another zoom change correctly
}


void AllegroUI::printHelp()
{
	const char * message = "STEREOZOOM HELP\n\nUse your mouse wheel to zoom in and out.\nand then move it with the mouse.\n\
Hold SHIFT to speed moving and zooming up.\nPress / to the 1:1 ratio and * to fit to the screen.\nFinally, press ESC or q to quit.";
	message_service->addMessage(message, 10);
}


// TODO: Process keypresses using class inheritance
vector<int> AllegroUI::processUserInput(vector<int> keystrokes)
{
	Change * global_change = nullptr;
	Change * local_change = nullptr;
	int key;
	bool boosting_input = false;
	vector<int> unprocessed_keystokes;
	for (int ii = 0; ii < keystrokes.size(); ii ++)
	{
		key = keystrokes[ii];
		switch (key)
		{
			case KEY_A:
				global_change = new ChangeOffset(- sensitivities->get(OF_SHIFT, BY_KEYBOARD), 0);
				break;
			case KEY_S:
				global_change = new ChangeOffset(0, sensitivities->get(OF_SHIFT, BY_KEYBOARD));
				break;
			case KEY_D:
				global_change = new ChangeOffset(sensitivities->get(OF_SHIFT, BY_KEYBOARD), 0);
				break;
			case KEY_W:
				global_change = new ChangeOffset(0, - sensitivities->get(OF_SHIFT, BY_KEYBOARD));
				break;
			case KEY_EQUALS:
				global_change = new ChangeZoomViewCentered(sensitivities->get(OF_ZOOM, BY_KEYBOARD));
				break;
			case KEY_MINUS:
				global_change = new ChangeZoomViewCentered(1.0 / sensitivities->get(OF_ZOOM, BY_KEYBOARD));
				break;
			case KEY_8:
			case KEY_ASTERISK:
				global_change = new ChangeSetViewToSeeEverything();
				break;
			case KEY_SLASH_PAD:
			case KEY_SLASH:
				global_change = new ChangeSetViewToZoom(1.0);
				break;
			case KEY_UP:
				local_change = new ChangeOffset(0, sensitivities->get(OF_OFFSET, BY_KEYBOARD));
				message_service->showOffsetMessage("Changed offset");
				break;
			case KEY_DOWN:
				local_change = new ChangeOffset(0, - sensitivities->get(OF_OFFSET, BY_KEYBOARD));
				break;
			case KEY_LEFT:
				local_change = new ChangeOffset(sensitivities->get(OF_OFFSET, BY_KEYBOARD), 0);
				break;
			case KEY_RIGHT:
				local_change = new ChangeOffset(- sensitivities->get(OF_OFFSET, BY_KEYBOARD), 0);
				break;
			default:
				unprocessed_keystokes.push_back(key);
				break;
		}
		if (global_change != nullptr)
		{
			stereotuple->applyChangeToAll(global_change);
			delete global_change;
			global_change = nullptr;
		}
		if (local_change != nullptr)
		{
			stereotuple->applyChangeToCurrent(local_change);
			delete local_change;
			local_change = nullptr;
		}
	}
	return unprocessed_keystokes;
}


void AllegroCrosshair::drawCenteredHline(double start, double end, double r, double g, double b)
{
	int size = our_crosshair->w;
	hline(our_crosshair, start * size, size / 2, end * size, makecol(r * 255, g * 255, b * 255));
}


void AllegroCrosshair::drawCenteredVline(double start, double end, double r, double g, double b)
{
	int size = our_crosshair->w;
	vline(our_crosshair, size / 2, start * size, end * size, makecol(r * 255, g * 255, b * 255));
}


void AllegroCrosshair::prepare(unsigned int size)
{
	if (our_crosshair == nullptr)
		our_crosshair = create_bitmap(size, size);
	clear_to_color(our_crosshair, makecol(255,0,255));
}
