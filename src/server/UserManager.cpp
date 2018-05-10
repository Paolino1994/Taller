#include "UserManager.h"

std::string UserManager::get_name(std::string buff) {
	return buff.substr(0, buff.find(USERNAME_DELIMETER));
}

std::string UserManager::get_password(std::string buff) {
	return buff.substr(buff.find(USERNAME_DELIMETER) + 1, buff.length() - 1);
}

bool UserManager::is_logged_in(std::string name) {
	for (user u : users) {
		if (name.compare(u.name) == 0)
			return true;
	}
	return false;
}

bool UserManager::is_reconecting(std::string name) {
	for (unsigned int i = 0; i < logged_out.size(); i++) {
		if (name.compare(logged_out[i].name) == 0) {
			logged_out.erase(logged_out.begin() + i);
			Log::get_instance()->info("Jugador reconectandose");
			return true;
		}
	}
	return false;
}

bool UserManager::login(Socket* skt){
	char buffer[MAX_LEN_LOGIN];
	if (skt->receive(buffer, MAX_LEN_LOGIN, MSG_NOSIGNAL) == ERROR){
		Log::get_instance()->error("Error reciviendo info de login");
		return LOGIN_ERROR;
	}
	
	std::string name = get_name(buffer);
	std::string pwd = get_password(buffer);

	std::string log = "Usuario con nombre: ";
	log.append(name);
	log.append("; contraseña: ");
	log.append(pwd);
	Log::get_instance()->debug(log);

	if (YAMLReader::get_instance().validarUsuario(name, pwd)) {
		skt->send(WRONG_CREDENTIALS, std::strlen(WRONG_CREDENTIALS), MSG_NOSIGNAL);
		Log::get_instance()->info("Credenciales invalidas");
		return LOGIN_ERROR;
	} else if (users.size() > 4) {
		skt->send(GAME_FULL, std::strlen(GAME_FULL), MSG_NOSIGNAL);
		Log::get_instance()->info("Juego Completo");
		return LOGIN_ERROR;
	} else if (is_logged_in(name)) {
		skt->send(ALREADY_LOGGED_IN, std::strlen(ALREADY_LOGGED_IN), MSG_NOSIGNAL);
		Log::get_instance()->info("El usuario que intenta ingresar ya esta conectado");
		return LOGIN_ERROR;
	} else if (playing && !is_reconecting(name)) {
		Log::get_instance()->info("El Juego ya esta empezado");
		skt->send(GAME_ALREADY_STARTED, std::strlen(GAME_ALREADY_STARTED), MSG_NOSIGNAL);
		return LOGIN_ERROR;
	}

	skt->send(USER_ACCEPTED, std::strlen(USER_ACCEPTED), MSG_NOSIGNAL);

	user u;
	u.skt = skt;
	u.name = name;
	users.push_back(u);
	
	log = "Usuario ";
	log.append(name);
	log.append(" conectado");
	Log::get_instance()->info(log);

	return LOGIN_SUCCESS;
}

void UserManager::logout(Socket* skt) {
	for (unsigned int i = 0; i < users.size(); i++) {
		if (users[i].skt == skt) {
			if (playing) {
				logged_out.push_back(users[i]);
			}

			std::string name = users[i].name;
			std::string log = "Usuario ";
			log.append(name);
			log.append(" desconectado");

			users.erase(users.begin() + i);
			Log::get_instance()->info(log);

			return;
		}
	}
}

void UserManager::game_started() {
	Log::get_instance()->debug("UserManager: partido iniciado");
	playing = true;
}

void UserManager::game_finished() {
	Log::get_instance()->debug("UserManager: partido finalizado");
	playing = false;
	logged_out.clear();
}
