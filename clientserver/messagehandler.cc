#include "messagehandler.h"
#include "connection.h"
#include "protocol.h"
#include "utility.h"

#include <iostream>
#include <cstdlib>

void MessageHandler::sendCode(int code)
{
	conn.write(static_cast<unsigned char>(code));
}

bool MessageHandler::isConnected() const
{
	return conn.isConnected();
}

void MessageHandler::sendInt(int val)
{
	sendCode((val >> 24) & 0xFF);
	sendCode((val >> 16) & 0xFF);
	sendCode((val >> 8) & 0xFF);
	sendCode(val & 0xFF);
}

void MessageHandler::sendIntParameter(int param)
{
    sendCode(Protocol::PAR_NUM);
    sendInt(param);
}

void MessageHandler::sendStringParameter(const std::string &s)
{
    sendCode(Protocol::PAR_STRING);
    sendInt(s.size());  // s.size() does not actually return an int.
    for (char c : s)
        sendCode(c);
}

// No special codes?
int MessageHandler::receiveCode()
{
    int code = conn.read();
    return code;
}

int MessageHandler::receiveInt()
{
    int b1 = receiveCode();
    int b2 = receiveCode();
    int b3 = receiveCode();
    int b4 = receiveCode();

    return b1 << 24 | b2 << 16 | b3 << 8 | b4;
}

int MessageHandler::receiveIntParameter()
{
    int code = receiveCode();
    if (code != Protocol::PAR_NUM) // Error.
        util::error("Error in recieveIntParameter(): expected PAR_NUM but received something else.");
    return receiveInt();
}

void MessageHandler::receiveAnsEnd()
{
	int code = receiveCode();
	if (code != Protocol::ANS_END)
		util::error("Error: Expected ANS_END but received something else.");
}

void MessageHandler::receiveComEnd()
{
	int code = receiveCode();
	if (code != Protocol::COM_END)
		util::error("Error: Expected COM_END but received something else.");
}

std::string MessageHandler::receiveString()
{
	std::string s;
	char ch;
	while ((ch = receiveCode()) != '$') {
		s += ch;
	}
	return s;
}

std::string MessageHandler::receiveStringParameter()
{
    int code = receiveCode();
    if (code != Protocol::PAR_STRING)
        util::error("Error in recieveStringParameter(): expected PAR_STRING but received something else.");

    int length = receiveInt();
    if (length < 0)
        util::error("Error: string length recieved in recieveStringParameter() less than zero.");

    std::string str;
    for (int i = 0; i != length; ++i) {
        str.push_back(static_cast<unsigned char>(conn.read()));
    }
    return str;
}
