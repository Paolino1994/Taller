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

short UserManager::_login(Socket* skt){
	Protocol com(skt);
	com.protect();
	com.read();
	if (com.request() != Request::LOGIN){
		Log::get_instance()->error("Error reciviendo info de login");
		return LOGIN_ERROR;
	}

	std::string buffer = com.dataBuffer();
	std::string name = get_name(buffer.c_str());
	std::string pwd = get_password(buffer.c_str());

	std::string log = "Usuario con nombre: ";
	log.append(name);
	log.append("; contraseña: ");
	log.append(pwd);
	Log::get_instance()->debug(log);

	YAMLReader& yamlReader = YAMLReader::get_instance();
    yamlReader.readYamlGeneral("");
	if (yamlReader.validarUsuario(name, pwd)) {
		com.write(Request::LOGIN, std::to_string(WRONG_CREDENTIALS).c_str(), sizeof(int));
		Log::get_instance()->info("Credenciales invalidas");
		return LOGIN_INVALID;
	} else if (users.size() > 4) {
		com.write(Request::LOGIN, std::to_string(GAME_FULL).c_str(), sizeof(int));
		Log::get_instance()->info("Juego Completo");
		return LOGIN_INVALID;
	} else if (is_logged_in(name)) {
		com.write(Request::LOGIN, std::to_string(ALREADY_LOGGED_IN).c_str(), sizeof(int)); 
		Log::get_instance()->info("El usuario que intenta ingresar ya esta conectado");
		return LOGIN_INVALID;
	} else if (playing && !is_reconecting(name)) {
		com.write(Request::LOGIN, std::to_string(GAME_ALREADY_STARTED).c_str(), sizeof(int));
		Log::get_instance()->info("El Juego ya esta empezado");
		return LOGIN_INVALID;
	}

	com.write(Request::LOGIN, std::to_string(USER_ACCEPTED).c_str(), sizeof(int));

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

short UserManager::login(Socket* skt){
	short status = LOGIN_INVALID;
	for (int i = 0; status == LOGIN_INVALID && i < MAX_LOGIN_ATTEMPTS; i++) {
		status = _login(skt);
	}
	return status;	
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
