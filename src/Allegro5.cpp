#define UI
#include "module_interface.h"

#include "Allegro5.h"
#include <allegro5/allegro_font.h>


enum { NOT_DRAGGING, STARTING_DRAGGING, DRAGGING};

void AllegroImage::clean()
{
	if (bitmap)
	{
		// TODO: Should not be called before Allegro exits
		al_destroy_bitmap(bitmap);
		bitmap = 0;
	}
}


void AllegroImage::mkImageDataStructure(unsigned char * rgbdata)
{
	clean();
	bitmap = al_create_bitmap(width, height);
	al_set_target_bitmap(bitmap);
	ALLEGRO_COLOR color;
	int color_pos;
	for(int jj = 0; jj < height; jj++)
	{
		for(int ii = 0; ii < width; ii++)
		{
			color_pos = (jj * width + ii) * 3;
			color = al_map_rgb(rgbdata[color_pos], rgbdata[color_pos + 1], rgbdata[color_pos + 2]);
			// for some reason, it is better to flip the image upside down
			al_put_pixel(ii, height - jj - 1, color);
		}
	}
}


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
	al_init();

	al_install_keyboard();
	al_install_mouse();

	al_init_font_addon();
	al_init_primitives_addon();
}


void PUBLIC(exit)()
{
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
		// TODO: Make the rectangle use floats
		al_draw_scaled_bitmap(image->bitmap, 
				coords.from_start[X], coords.from_start[Y],
				coords.getFromSize()[X], coords.getFromSize()[Y],
				coords.to_start[X] + horizontal_offset, coords.to_start[Y] + vertical_offset,
				coords.getToSize()[X], coords.getToSize()[Y], 0
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


valarray<int> AllegroImageGrid::getViewCoordinates(int mouse_x, int mouse_y) const
{
	valarray<int> local_mouse_position = XY<int>(mouse_x, mouse_y) % XY<int, unsigned int>(view_size);
	return local_mouse_position;
}


void AllegroMessageService::displayMessages() const
{
	vector<string> rows;
	string temp, text;
	ALLEGRO_COLOR color;
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
		current_row_height = rows.size() * al_get_font_line_height(font) + 10;

		color = al_map_rgba(64, 64, 64, int(relative_alpha * 96));
		al_draw_filled_rectangle(row_break, previous_row_end, row_break + max_row_len * al_get_text_width(font, "@") + 10, previous_row_end + current_row_height, color);
		color = al_map_rgba(255,255,255, int(relative_alpha * 192));
		for (unsigned int i = 0; i < rows.size(); i++)
		{
			al_draw_text(font, color, 5, previous_row_end + i * al_get_font_line_height(font) + 5, ALLEGRO_ALIGN_LEFT, rows[i].c_str());
		}
		previous_row_end += row_break;
		previous_row_end += current_row_height;
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
			case ALLEGRO_KEY_H:
				printHelp();
				break;
			case ALLEGRO_KEY_ESCAPE:
			case ALLEGRO_KEY_Q:
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
	al_set_new_display_flags(ALLEGRO_WINDOWED);
	display = al_create_display(hres, vres);
	screen_buffer = al_get_backbuffer(display);
	al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);
	al_hide_mouse_cursor(display);

	if (verbose)
		printf("Trying to set resolution: %dx%d, ?? bpp\n", hres, vres);
}


void AllegroUI::processEvent(ALLEGRO_EVENT & event)
{
	if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN
	|| event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP
	)
	{
		processMouseDrag(event);
	}
	else if (event.type == ALLEGRO_EVENT_MOUSE_AXES)
	{
		if (event.mouse.dz != 0)
		{
			applyMouseZoom(event.mouse.x, event.mouse.y, event.mouse.dz);
		}
		else
		{
			processMouseDrag(event);
		}
	}
	else if (event.type == ALLEGRO_EVENT_KEY_DOWN)
	{
		vector<int> keystrokes;
		keystrokes.push_back(event.keyboard.keycode);
		keystrokes = processUIControl(keystrokes);
		keystrokes = processUserInput(keystrokes);
	}
}


void AllegroUI::processMouseDrag(ALLEGRO_EVENT & evt)
{
	if (evt.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
	{
		if (evt.mouse.button == 1)
		{
			dragging = STARTING_DRAGGING;
		}
	}
	else if (evt.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
	{
		if (evt.mouse.button == 1)
		{
			dragging = NOT_DRAGGING;
		}
	}
	else if (evt.type == ALLEGRO_EVENT_MOUSE_AXES)
	{
		if (dragging == DRAGGING)
		{
			applyMouseDrag(evt.mouse.dx, evt.mouse.dy);
		}
		else if (dragging == STARTING_DRAGGING)
		{
			dragging = DRAGGING;
		}
	}
}


void AllegroUI::applyMouseDrag(int dmouse_x, int dmouse_y)
{
	double base_sensitivity = sensitivities->get(OF_SHIFT, BY_MOUSE);
	ChangeDrag change = ChangeDrag(- base_sensitivity * dmouse_x, - base_sensitivity * dmouse_y);
	stereotuple->applyChangeToAll(& change);
}


void AllegroUI::applyMouseZoom(int x, int y, int dz)
{
	ChangeZoomViewGeneral change = ChangeZoomViewGeneral(pow(sensitivities->get(OF_ZOOM, BY_MOUSE), dz), stereotuple->getViewCoordinates(x, y));
	stereotuple->applyChangeToAll(& change);
	// TODO: Print_zoom();
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
			case ALLEGRO_KEY_A:
				global_change = new ChangeOffset(- sensitivities->get(OF_SHIFT, BY_KEYBOARD), 0);
				break;
			case ALLEGRO_KEY_S:
				global_change = new ChangeOffset(0, sensitivities->get(OF_SHIFT, BY_KEYBOARD));
				break;
			case ALLEGRO_KEY_D:
				global_change = new ChangeOffset(sensitivities->get(OF_SHIFT, BY_KEYBOARD), 0);
				break;
			case ALLEGRO_KEY_W:
				global_change = new ChangeOffset(0, - sensitivities->get(OF_SHIFT, BY_KEYBOARD));
				break;
			case ALLEGRO_KEY_EQUALS:
				global_change = new ChangeZoomViewCentered(sensitivities->get(OF_ZOOM, BY_KEYBOARD));
				break;
			case ALLEGRO_KEY_MINUS:
				global_change = new ChangeZoomViewCentered(1.0 / sensitivities->get(OF_ZOOM, BY_KEYBOARD));
				break;
			case ALLEGRO_KEY_8:
			case ALLEGRO_KEY_PAD_ASTERISK:
				global_change = new ChangeSetViewToSeeEverything();
				break;
			case ALLEGRO_KEY_PAD_SLASH:
			case ALLEGRO_KEY_SLASH:
				global_change = new ChangeSetViewToZoom(1.0);
				break;
			case ALLEGRO_KEY_UP:
				local_change = new ChangeOffset(0, sensitivities->get(OF_OFFSET, BY_KEYBOARD));
				message_service->showOffsetMessage("Changed offset");
				break;
			case ALLEGRO_KEY_DOWN:
				local_change = new ChangeOffset(0, - sensitivities->get(OF_OFFSET, BY_KEYBOARD));
				break;
			case ALLEGRO_KEY_LEFT:
				local_change = new ChangeOffset(sensitivities->get(OF_OFFSET, BY_KEYBOARD), 0);
				break;
			case ALLEGRO_KEY_RIGHT:
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
			stereotuple->applyChangeToCurrent(local_change, mouse.x, mouse.y);
			delete local_change;
			local_change = nullptr;
		}
	}
	return unprocessed_keystokes;
}


void AllegroCrosshair::drawCenteredHline(double start, double end, double r, double g, double b)
{
	al_draw_line(start * size, size / 2, end * size, size / 2, al_map_rgb(r * 255, g * 255, b * 255), 0);
}


void AllegroCrosshair::drawCenteredVline(double start, double end, double r, double g, double b)
{
	al_draw_line(size / 2, start * size, size / 2, end * size, al_map_rgb(r * 255, g * 255, b * 255), 0);
}


void AllegroCrosshair::prepare()
{
	if (our_crosshair == nullptr)
		our_crosshair = al_create_bitmap(size, size);
	al_set_target_bitmap(our_crosshair);
	al_clear_to_color(al_map_rgba(0, 0, 0, 0));
}


void AllegroCrosshair::finish()
{
	al_set_target_bitmap(* dest_bitmap_ptr);
}
