/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserver.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By:  <mashad@student.1337.ma>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/10 08:32:16 by                   #+#    #+#             */
/*   Updated: 2021/09/30 08:44:40 by                  ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
#define WEBSERV_HPP

// Including necessary libraries

# include <sys/select.h>
# include <sys/socket.h>
# include <sys/types.h>
# include <sys/event.h>
# include <arpa/inet.h>
# include <algorithm>
# include <iostream>
# include <unistd.h>
# include <fstream>
# include <sstream>
# include <fcntl.h>
# include <cstdlib>
# include <poll.h>
# include <cstdio>
# include <ctime>

// End of including

//----- // Define Marcos for general use

# define DEFAULT_CONFIG_PATH 	"config/default.conf.conf"
# define DEFAULT_PORT			80
# define DEFAULT_ROOT_DIRECTIVE "www"
# define DEFAULT_404_PAGE       "pages/404.html"
# define DEFAULT_501_PAGE       "pages/501.html"
# define DEFAULT_500_PAGE       "pages/500.html"

//----- // End of Macros


//----------// Arguments class
class INKARGUMENT {
	private:
		std::string		_configurationFileName;
		bool 			_fileCheck; // -t checking configuration file and exit
		bool			_verbose; // -v print more verbose
		bool 			_help;  // print options usage

	public:
		INKARGUMENT ( void ): _configurationFileName("conf.d/default.conf"), _fileCheck(false), _verbose(false), _help(false) {
			return ;
		}
		INKARGUMENT( INKARGUMENT const &COPY): _configurationFileName(COPY._configurationFileName), _fileCheck(COPY._fileCheck), _verbose(COPY._verbose), _help(COPY._help) {
			return ;
		}
		~INKARGUMENT( void ) {
			return ;
		}
		bool 			getHelp( void ) const {
			return (this->_help);
		}
		bool 			getVerbose( void ) const {
			return (this->_verbose);
		}
		bool 			getFileCheck( void ) const {
			return (this->_fileCheck);
		}
		std::string 	getFileName( void ) const {
			return (this->_configurationFileName);
		}
		void 			parseArguments(int argLen, char **argv) {
			int i;
			// TO-DO: Parse Command line arguments
//
			for (i = 1; i < argLen && argv[i][0] == '-' ; i++) { // Parsing Options
				if (std::string(argv[i]).compare("-h") == 0|| std::string(argv[i]).compare("--help") == 0)
					this->_help = true;
				else if (std::string(argv[i]).compare("-t") == 0 || std::string(argv[i]).compare("--test") == 0)
					this->_fileCheck = true;
				else if (std::string(argv[i]).compare("-v") == 0 || std::string(argv[i]).compare("--verbose") == 0)
					this->_verbose = true;
				else
					throw INKARGUMENT::ArgumentsIllegalOption();
			}
			if (i < argLen)
				this->_configurationFileName = std::string(argv[i++]);
			if (i != argLen)
				throw INKARGUMENT::ArgumentsParsingError();
			return ;
		}
		class ArgumentsParsingError : public std::exception{
			public:
				ArgumentsParsingError( void ) throw(){
					return ;
				}
				virtual ~ArgumentsParsingError( void ) throw(){
					return ;
				}
				virtual const char* what() const throw(){
					return ("Usage: ./webserv [options] [configuration file path]\n-h, --help: for more advanced help.");
				}
		};
		class ArgumentsIllegalOption : public std::exception{
		public:
			ArgumentsIllegalOption( void ) throw(){
				return ;
			}
			virtual ~ArgumentsIllegalOption( void ) throw(){
				return ;
			}
			virtual const char* what() const throw(){
				return ("webserv: illegal option");
			}
		};
};

namespace Color {
	enum Code {
		FG_RED      = 31,
		FG_GREEN    = 32,
		FG_BLUE     = 34,
		FG_DEFAULT  = 39,
		BG_RED      = 41,
		BG_GREEN    = 42,
		BG_BLUE     = 44,
		BG_DEFAULT  = 49
	};
	class Modifier {
		Code code;
	public:
		Modifier(Code pCode) : code(pCode) {}
		friend std::ostream&
		operator<<(std::ostream& os, const Modifier& mod) {
			return os << "\033[" << mod.code << "m";
		}
	};
}
#endif
