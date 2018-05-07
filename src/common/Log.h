#ifndef __LOG_H__
#define __LOG_H__

#include <string>
#include <sstream>
#include <fstream>
#include <time.h>

#define LOG_DEBUG "debug"
#define LOG_INFO "info"
#define LOG_ERROR "error"

class Log {
private:
	static std::string filename;
	// = true despues de primer llamada a initialize
	static bool initialized;
	/* Instancia guardada */
	static Log* instance;
	std::string type;
	std::ofstream file;
	/* Constructor privado para evitar instaciación */
	Log();
	Log(std::string type);
	/**
		@PRE: recive un mensaje y su tipo (debug, info o error), ya esta controlado si el tipo es imprimible.
		@POS: se realizo un nuevo ingreso en el log con el mensaje
	*/
	void log(std::string msg, std::string type);

public:
	static void setFilenamePrefix(std::string filename);
	/**
		@PRE: recive el tipo de log (debug, info o error)
		@POS: el log esta inicializado
	*/
	static void initialize(std::string type);
	/**
		@POS: retorna true si ya fue inicializado, false si no
	*/
	static bool is_initialized();
	/**
		@POS: retorna una referencia a la instancia del log si existe, NULL si no
	*/
	static Log* get_instance();
	/**
		@PRE: recive un mensaje
		@POS: se realizo un nuevo ingreso en el log con el mensaje si corresponde
	*/
	void debug(std::string msg);
	/**
		@PRE: recive un mensaje
		@POS: se realizo un nuevo ingreso en el log con el mensaje si corresponde
	*/
	void info(std::string msg);
	/**
		@PRE: recive un mensaje
		@POS: se realizo un nuevo ingreso en el log
	*/
	void error(std::string msg);

	void setType(std::string newType);

	~Log();
};

#endif
