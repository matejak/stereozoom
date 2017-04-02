
#include "MessageService.h"


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
	for(auto it = messages.begin(); it != messages.end(); it++)
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
	auto it = getMessageRecordPtr(previous_message);
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
	auto it_next = messages.end();
	for (auto it = messages.begin(); it != messages.end(); it = it_next)
	{
		if ((* it)->getRemainingSeconds() <= 0)
		{
			// printf("Erasing %s\n", (*it)->message->text.c_str());
			MessageRecord * to_delete = * it;
			it_next = messages.erase(it);
			delete to_delete;
		}
		else
		{
			it_next = ++it;
		}
	}
}
