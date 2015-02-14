#ifndef EXCEPTIONS_HPP
	#define EXCEPTIONS_HPP
	class eFileException{};
	class eFileNotFoundException : public eFileException{}; // file not found exception
#endif