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

#include <cmath>
#include "Image_group.h"


void AllegroUI::setSensitivities()
{
	sensitivities.values[OF_ZOOM][BY_KEYBOARD] = 1.2;
	sensitivities.values[OF_ZOOM][BY_MOUSE] = 1.15;
	sensitivities.values[OF_ZOOM][BOOST] = 2;
	sensitivities.values[OF_SHIFT][BY_KEYBOARD] = 2.5;
	sensitivities.values[OF_SHIFT][BY_MOUSE] = 1;
	sensitivities.values[OF_SHIFT][BOOST] = 2.5;
	sensitivities.values[OF_OFFSET][BY_KEYBOARD] = 1.0;
	sensitivities.values[OF_OFFSET][BOOST] = 2.0;
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
			double base_sensitivity = sensitivities.get(OF_SHIFT, BY_MOUSE);
			ChangeDrag change = ChangeDrag(- base_sensitivity * dmouse_x, - base_sensitivity * dmouse_y);
			stereotuple.applyChangeToAll(& change);
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
		ChangeZoomViewGeneral change = ChangeZoomViewGeneral(pow(sensitivities.get(OF_ZOOM, BY_MOUSE), mouse_z), stereotuple.getViewCoordinates());
		stereotuple.applyChangeToAll(& change);
		// TODO: Print_zoom();
	}
	position_mouse_z(0); // just in order to detect another zoom change correctly
}


void AllegroUI::printHelp()
{
	const char * message = "STEREOZOOM HELP\n\nUse your mouse wheel to zoom in and out.\nand then move it with the mouse.\n\
Hold SHIFT to speed moving and zooming up.\nPress / to the 1:1 ratio and * to fit to the screen.\nFinally, press ESC or q to quit.";
	message_service.addMessage(message, 10);
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
				global_change = new ChangeOffset(- sensitivities.get(OF_SHIFT, BY_KEYBOARD), 0);
				break;
			case KEY_S:
				global_change = new ChangeOffset(0, sensitivities.get(OF_SHIFT, BY_KEYBOARD));
				break;
			case KEY_D:
				global_change = new ChangeOffset(sensitivities.get(OF_SHIFT, BY_KEYBOARD), 0);
				break;
			case KEY_W:
				global_change = new ChangeOffset(0, - sensitivities.get(OF_SHIFT, BY_KEYBOARD));
				break;
			case KEY_EQUALS:
				global_change = new ChangeZoomViewCentered(sensitivities.get(OF_ZOOM, BY_KEYBOARD));
				break;
			case KEY_MINUS:
				global_change = new ChangeZoomViewCentered(1.0 / sensitivities.get(OF_ZOOM, BY_KEYBOARD));
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
				local_change = new ChangeOffset(0, sensitivities.get(OF_OFFSET, BY_KEYBOARD));
				message_service.showOffsetMessage("Changed offset");
				break;
			case KEY_DOWN:
				local_change = new ChangeOffset(0, - sensitivities.get(OF_OFFSET, BY_KEYBOARD));
				break;
			case KEY_LEFT:
				local_change = new ChangeOffset(sensitivities.get(OF_OFFSET, BY_KEYBOARD), 0);
				break;
			case KEY_RIGHT:
				local_change = new ChangeOffset(- sensitivities.get(OF_OFFSET, BY_KEYBOARD), 0);
				break;
			default:
				unprocessed_keystokes.push_back(key);
				break;
		}
		if (global_change != nullptr)
		{
			stereotuple.applyChangeToAll(global_change);
			delete global_change;
			global_change = nullptr;
		}
		if (local_change != nullptr)
		{
			stereotuple.applyChangeToCurrent(local_change);
			delete local_change;
			local_change = nullptr;
		}
	}
	return unprocessed_keystokes;
}


void Crosshair::createNormal(unsigned int size)
{
	// also creates a crosshair bitmap
	if (bitmap == NULL)
		bitmap = create_bitmap(size, size);
	clear_to_color(bitmap, makecol(255,0,255));
	circle(bitmap, bitmap->w / 2, bitmap->h / 2, (bitmap->w * 2) / 3, 0);
	circle(bitmap, bitmap->w / 2, bitmap->h / 2, (bitmap->w * 1) / 3, 0);
	hline(bitmap, 0, bitmap->h / 2, bitmap->w, makecol(255, 255, 255));
	vline(bitmap, bitmap->w / 2, 0, bitmap->h, makecol(255, 255, 255));
}


double time_remaining_to_alpha(double ttl)
{
	double result = MIN(ttl / 2.0, 1.0);
	return result * 0.8 + 0.2;
}


MessageRecord::MessageRecord(const char * text, double duration):
	time_to_expire(duration), time_inserted(steady_clock::now()), message(0)
{
	message = new Message(text);
}


MessageService::~MessageService()
{
	auto it_next = messages.end();
	for (auto it = messages.begin(); it != messages.end(); it = it_next)
	{
		MessageRecord * to_delete = * it;
		it_next = messages.erase(it);
		delete to_delete;
	}
}


std::set<MessageRecord *, APtrComp>::iterator MessageService::getMessageRecordPtr(MessageRecord * ptr)
{
	for(std::set<MessageRecord *, APtrComp>::iterator it = messages.begin(); it != messages.end(); it++)
	{
		if ((* it) == ptr)
		{
			return it;
		}
	}
	return messages.end();
}


MessageRecord * MessageService::addRefreshableMessage(const char * msg, double time_to_live, MessageRecord * previous_message)
{
	std::set<MessageRecord *, APtrComp>::iterator it = getMessageRecordPtr(previous_message);
	if (it != messages.end())
	{
		// we need to erase and insert in order to ensure proper set ordering with new insert time
		MessageRecord * to_delete = * it;
		messages.erase(it);
		delete to_delete;
	}
	MessageRecord * ret = _addMessage(msg, time_to_live);
	return ret;
}


void MessageService::addMessage(const char * msg, double time_to_live)
{
	_addMessage(msg, time_to_live);
}


MessageRecord * MessageService::_addMessage(const char * msg, double time_to_live)
{
	MessageRecord * message = new MessageRecord(msg, time_to_live);
	messages.insert(message);
	return message;
}


void MessageService::purgeOldMessages()
{
	set<MessageRecord *, APtrComp>::iterator it_next = messages.end();
	for (set<MessageRecord *, APtrComp>::iterator it = messages.begin(); it != messages.end(); it = it_next)
	{
		if ((* it)->getRemainingSeconds() <= 0)
		{
			// printf("Erasing %s\n", (*it)->message->text.c_str());
			MessageRecord * to_delete = * it;
			it_next = messages.erase(it);
			delete to_delete;
		}
	}
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
		relative_alpha = time_remaining_to_alpha((* it)->getRemainingSeconds());
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
