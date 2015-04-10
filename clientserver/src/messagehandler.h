#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include <string>

class Connection;

/* Handles communication on a connection, inspired my MessageHandler.java */
class MessageHandler {
public:
	MessageHandler(Connection &c) : conn(c) {}
	bool isConnected() const;
	void sendCode(int code);
	void sendInt(int val);
	void sendIntParameter(int param);
	void sendStringParameter(const std::string &s);
	int receiveCode();
	int receiveInt();
	int receiveIntParameter();
	void receiveAnsEnd();
	void receiveComEnd();
	std::string receiveStringParameter();
	std::string receiveString();
private:
	Connection &conn;
};

#endif /* MESSAGEHANDLER_H */
