#ifndef USER_MANAGER
#define USER_MANAGER

#include "common/Protocol.h"
#include "common/Request.h"
#include "common/GameConstants.h"
#include "common/Socket.h"
#include "common/YAMLReader.h"
#include "common/Log.h"
#include <string>
#include <vector>
#include <cstring>
#include <mutex>

#define MAX_LOGIN_ATTEMPTS 100000

typedef struct {
	Socket* skt;
	std::string name;
	User_ID id;
} user;

class UserManager {
private:
	std::vector<user> users;
	std::vector<user> ready_users;
	std::vector<user> logged_out;
	bool playing = false;
	std::string get_name(std::string buff);
	std::string get_password(std::string buff);
	bool is_logged_in(std::string name);
	bool is_reconecting(std::string name);
	short _login(Socket* skt);

	std::mutex mtx;

	static User_ID userIdToAssign;
	static User_ID getNextUserId();

	// Cabeceadas para solución rapida
	User_ID lastUserId;
	User_ID reconnectedUserId;

	UserManager();

public:
	static UserManager& get_instance();
	/**
		@PRE: Receives a skt already connected to a client, which is about to send the user and password
		@POS: Returns LOGIN_SUCCESS if the user was added and LOGIN_ERROR if it wasn't
	*/
	short login(Socket* com);

	/**
		@PRE: Receives a protocol with a socket already connected to a logged in client
		@POS: The client is no longer logged in
	*/
	void logout(Protocol& p);

	/**
		@PRE: Receives a protocol with a socket already connected to a logged in client
		@POS: The client is counted as ready
	*/
	void user_ready(Protocol& p);

	/**
		Returns true if the manager is set to playing functions
	*/
	bool game_started();

	/**
		Sets the manager to not playing functions
	*/
	void game_finished();

	/**
		Get User_ID assigned to the user with the last successful login
		CABECEADA para solucion rapida: ESTO FUNCIONA PORQUE ESTO TRABAJA SECUENCIALMENTE (ver main.cpp)
	*/
	User_ID getLastUserId();
};

#endif
