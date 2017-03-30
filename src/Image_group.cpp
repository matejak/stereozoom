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
