#include "Log.h"


Log* Log::instance = 0;
std::string Log::filename = "log"; //default log name
bool Log::initialized = false;
std::mutex Log::mtx_class;

Log::Log(std::string t){
	time_t rawtime;
  	struct tm * timeinfo;
  	char charTime [80];
	setType(t);
  	time (&rawtime);
  	timeinfo = localtime (&rawtime);
  	strftime (charTime,80,"[%F]-%X",timeinfo);
	std::stringstream file_name;
	file_name << "log/" << Log::filename << "-" << charTime << ".txt";
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
	time_t rawtime;
  	struct tm * timeinfo;
  	char charTime [80];
  	time (&rawtime);
  	timeinfo = localtime (&rawtime);
  	strftime (charTime,80,"[%F] - %X ",timeinfo);
	file << "" << charTime << "> " << type << ": " << msg << std::endl;
}

void Log::setFilenamePrefix(std::string filename)
{
	Log::filename = filename;
}

void Log::debug(std::string msg) {
	std::unique_lock<std::mutex> lck(this->mtx_log);
	if (this->type.compare(LOG_DEBUG) == 0) {
		log(msg, LOG_DEBUG);
	}
}

void Log::info(std::string msg) {
	std::unique_lock<std::mutex> lck(this->mtx_log);
	if (this->type.compare(LOG_DEBUG) == 0 || this->type.compare(LOG_INFO) == 0) {
		log(msg, LOG_INFO);
	}
}

void Log::error(std::string msg) {
	std::unique_lock<std::mutex> lck(this->mtx_log);
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
