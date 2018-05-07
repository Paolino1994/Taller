#include "Log.h"

Log* Log::instance = 0;
std::string Log::filename = "log"; //default log name
bool Log::initialized = false;

Log::Log(std::string t){
	type = t;
	time_t now;
	time(&now);
	std::string c_time = ctime(&now);
	c_time.pop_back();
	std::stringstream file_name;
	file_name << "log/" << Log::filename << "-" << c_time << ".txt";
	file.open(file_name.str(), std::ofstream::out);
}

Log::Log(): 
	Log(LOG_ERROR)
{}

void Log::initialize(std::string t) {
    if (t.compare(LOG_DEBUG) == 0 || t.compare(LOG_INFO) == 0 || t.compare(LOG_ERROR) == 0){
        if (instance == 0) {
        	instance = new Log(t);
        } else {
        	instance->setType(t);
        }
        initialized = true;
   	}
}

bool Log::is_initialized() {
	return initialized;
}

Log* Log::get_instance() {
	if (instance == 0) {
		instance = new Log();
	}
	return instance;
}

void Log::log(std::string msg, std::string type) {
	time_t now;
	time(&now);
	std::string c_time;
	c_time = ctime(&now);
	c_time.pop_back();
	file << "" << c_time.c_str() << "> " << type << ": " << msg << std::endl;
}

void Log::setFilenamePrefix(std::string filename)
{
	Log::filename = filename;
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

void Log::setType(std::string newType) {
	this->type = newType;
}

Log::~Log() {
	file.close();
}
