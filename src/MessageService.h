#pragma once


class Message
{
public:
	Message(const char * text):text(text) {}
	std::string text;
};


class MessageRecord
{
public:
	MessageRecord(const char * text, double duration);
	~MessageRecord() 
	{
		delete message;
		message = nullptr;
	}
	double time_to_expire;
	time_point<steady_clock> time_inserted;

	double getRemainingSeconds()
	{
		double ret = (duration_cast<milliseconds>(time_inserted.time_since_epoch() - steady_clock::now().time_since_epoch())).count() / 1000.0 + time_to_expire;
		return ret;
	}

	std::string getMessageText() const
	{
		return message->text;
	}

	Message * message;
};


class MessageService
{
public:
	virtual ~MessageService();
	void addMessage(const char * msg, double time_to_live);
	MessageRecord * addRefreshableMessage(const char * msg, double time_to_live, MessageRecord * previous_message=nullptr);
	void cleanOldMessages();
	virtual void displayMessages() const = 0;
	void purgeOldMessages();
protected:
	set<MessageRecord *, APtrComp> messages;
private:
	MessageRecord * _addMessage(const char * msg, double time_to_live);
	std::set<MessageRecord *, APtrComp>::iterator getMessageRecordPtr(MessageRecord * ptr);
};


class SpecializedMessageService: public MessageService
{
public:
	virtual ~SpecializedMessageService() {}
	SpecializedMessageService():MessageService(), offset_message(nullptr) {}
	void showOffsetMessage(const char * offset)
	{
		offset_message = addRefreshableMessage(offset, 4, offset_message);
	}
private:
	MessageRecord * offset_message;
};
