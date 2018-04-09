#include "Log.h"

Log* Log::instance = 0;

Log::Log(std::string t){
	type = t;
	time_t now;
	time(&now);
	std::stringstream file_name;
	file_name << "log/log-" << ctime(&now) << ".txt";
	file.open(file_name.str(), std::ofstream::out);
}

void Log::initialize(std::string t) {
	if (instance == 0) {
		if (t.compare(LOG_DEBUG) == 0 || t.compare(LOG_INFO) == 0 || t.compare(LOG_ERROR) == 0){
			instance = new Log(t);
		}
	}
}

bool Log::is_initialized() {
	return (instance != 0);
}

Log* Log::get_instance() {
	if (instance != 0) {
		return instance;
	}
	return NULL;
}

void Log::log(std::string msg, std::string type) {
	time_t now;
	time(&now);
	std::string c_time;
	c_time = ctime(&now);
	c_time.pop_back();
	file << "<" << c_time.c_str() << "> " << type << ": " << msg << std::endl;
}

void Log::debug(std::string msg) {
	if (this->type.compare(LOG_DEBUG) == 0) {
		log(msg, LOG_DEBUG);
	}
}

void Log::info(std::string msg) {
	if (this->type.compare(LOG_DEBUG) == 0 || this->type.compare(LOG_INFO) == 0) {
		log(msg, LOG_INFO);
	}
}

void Log::error(std::string msg) {
	if (this->type.compare(LOG_DEBUG) == 0 || this->type.compare(LOG_INFO) == 0 || this->type.compare(LOG_ERROR) == 0) {
		log(msg, LOG_ERROR);
	}
}

Log::~Log() {
	file.close();
}
