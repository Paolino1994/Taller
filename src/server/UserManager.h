#ifndef USER_MANAGER
#define USER_MANAGER

#include "common/Socket.h"
#include "common/YAMLReader.h"
#include "common/Log.h"
#include <string>
#include <vector>
#include <cstring>

#define LOGIN_ERROR false
#define LOGIN_SUCCESS true

#define USERNAME_DELIMETER ";"
#define MAX_LEN_LOGIN 126
#define WRONG_CREDENTIALS "w"
#define GAME_FULL "f"
#define ALREADY_LOGGED_IN "l"
#define GAME_ALREADY_STARTED "s"
#define USER_ACCEPTED "a"

typedef struct {
	Socket* skt;
	std::string name;
} user;

class UserManager {
private:
	std::vector<user> users;
	std::vector<user> logged_out;
	bool playing = false;
	std::string get_name(std::string buff);
	std::string get_password(std::string buff);
	bool is_logged_in(std::string name);
	bool is_reconecting(std::string name);

public:
	/**
		@PRE: Receives a skt already connected to a client, which is about to send the user and password
		@POS: Returns LOGIN_SUCCESS if the user was added and LOGIN_ERROR if it wasn't
	*/
	bool login(Socket* skt);

	/**
		@PRE: Receives a skt already connected to a logged in client
		@POS: The client is no longer logged in
	*/
	void logout(Socket* skt);

	/**
		Sets the manager to game on functions
	*/
	void game_started();

	/**
		Sets the manager to game off functions
	*/
	void game_finished();
};

#endif
