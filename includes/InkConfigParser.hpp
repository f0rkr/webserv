/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParsing.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mashad <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/06 10:27:37 by mashad            #+#    #+#             */
/*   Updated: 2022/02/14 14:02:15 by                  ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGPARSING_HPP
# define CONFIGPARSING_HPP

# include "webserv.hpp"
# include "InkServerConfig.hpp"

/** @Brief ft namespace
 * Namespace includes all classes needed to realise this project
 */
namespace ft {

	/** @brief Configuration file class
	 * Read and parse server configuration file
	 * if any error occurred during either the parsing or reading
	 * phase it throws exceptions
	 */
	class ConfigParsing {
	public:
		typedef size_t						size_type;
		typedef std::string					string;
		typedef std::vector<ServerConfig>	vector_type;
		typedef std::allocator<vector_type>	allocator_type;

	private:
		string			_filename;
		string			_rawData;
		vector_type		_servers;
		allocator_type	_alloc;
		size_type		_serversCount;

		/** @brief Servers count
		 * Calculating servers count from _rawData
		 *
		 * @param none
		 * @return size_type
		 */
		size_type		_getServersCount( void ) {
			std::stringstream	data(_rawData);
			string				line;
			size_type			serversCount;

			serversCount = 0;
			while (std::getline(data, line, '\n')){
				if (line[0] == '[')
					serversCount++;
			}
			return (serversCount);
		}

		/** @brief Get Location count
		 * Get number of location in the configuration file
		 *
		 * @param none
		 * @return none
		 */
		void		_getLocationCount( void ) {
			std::stringstream	data(_rawData);
			string				line;
			size_type			locationsCount;
			size_type			serversCount;

			serversCount = 0;
			locationsCount = 0;
			while (std::getline(data, line, '\n')) {
				line.erase(std::remove_if(line.begin(), line.end(), isspace), line.end());
				if (line[0] == '#' || line[0] == '\n' || line[0] == '\0')
					continue;
				while (std::getline(data, line, '\n') && line[0] != '[')
				{
					if (line[0] == '#' || line[0] == '\n' || line[0] == '\0')
						continue ;
					line.erase(std::remove_if(line.begin(), line.end(), isspace), line.end());
					if (line.find("location=") != std::string::npos)
						locationsCount++;
				}
				if (locationsCount == 0)
					throw ConfigParsing::FileParsingError();
				else
					_servers.at(serversCount++).setLocations(locationsCount);
				if (line[0] == '[')
					locationsCount = 0;
			}
		}

		/** @brief Final check
		 * Checking if file is parsed successfully
		 *
		 * @param none
		 * @return bool
		 */
		bool		_finalCheck() const {
			size_type	serversCount;
			size_type	locationsCount;

			// TO-DO: Checking if everything is parsed successfully
			for (serversCount = 0; serversCount < this->_serversCount; serversCount++) {
				if (!_servers.at(serversCount).checkDefaultValues()) {
					return (false);
				}
				for (locationsCount = 0; locationsCount < _servers.at(serversCount).getLocationsCount();locationsCount++) {
					if (!_servers[serversCount].getLocations().at(locationsCount).checkLocationDefaultValues())
						return (false);
				}
			}
			return (true);
		}
	public:

		/** @brief ConfigParsing default constructor
		 * Construct a configuration parsing class
		 * using default file name.
		 *
		 * @param none
		 * @return none
		 */
		ConfigParsing () : _filename(DEFAULT_CONFIG_PATH) , _serversCount(0), _servers(0), _alloc(allocator_type()){readFileData();}


		/** @brief ConfigParsing default constructor
		 * Construct a configuration parsing class
		 * with filename from parameters
		 *
		 * @param filename configuration filename path
		 * @return none
		 */
		explicit ConfigParsing (string const &filename): _filename(filename), _serversCount(0), _servers(0), _alloc(allocator_type()) {readFileData();}


		/** @brief ConfigParsing copy constructor
		 * Construct ConfigParsing class using COPY class
		 *
		 * @param COPY Already construct ConfigParsing class
		 * @return none;
		 */
		ConfigParsing (ConfigParsing const &COPY) : _serversCount(0) {
			*this = COPY;
		}


		/** @brief ConfigParsing destructor
		 * Clearing all private variables
		 *
		 * @param none
		 * @return none
		 */
		~ConfigParsing ( void ) {
			_rawData = "";
			_filename = "";
			// Destroy Allocated Servers holder.
			_servers.clear();
			_serversCount = 0;
		}

		/** @brief Assignment operator
		 * Assign OP ConfigParsing class to this class
		 *
		 * @param OP ConfigParsing class
		 * @return *this A reference to this class
		 */
		ConfigParsing	&operator=(ConfigParsing const &OP) {
			if (this != &OP) {
				_filename = OP._filename;
				_rawData = OP._filename;
				// Destroy Allocated Servers holder.
				_servers.clear();
				_servers = OP._servers;
				_serversCount = _servers.size();
			}
			return (*this);
		}


		/** @brief Filename getter
		 * Return filename value
		 *
		 * @param none
		 * @return filename, Configuration file path
		 */
		string		getFilename( void ) const { return (_filename); }

		/** @brief GEt servers
		 *
		 * @param none
		 * @return _servers
		 */
		 vector_type	getServerConfig( void ) const {return (_servers);}
		/** @brief Filename setter
		 * Set filename path
		 *
		 * @param filename
		 * @return none
		 */
		void		setFilename( string const &filename ) {_filename = filename;}


		/** @brief Servers Count
		 *
		 * @param none
		 * @return _serversCount
		 */
		 size_type	getServerCount( void ) const {return (_servers.size());}
		/** @brief Read file data method
		 * Reading configuration file data
		 * And store it into the rawData attribute
		 *
		 * @param none
		 * @return Returns servers count on the configuration file
		 */
		void		readFileData ( void ) {
			std::ifstream		file;
			std::stringstream	buffer;

			// TO-DO: Read Configuration file and store it into a data string
			file.open(_filename);
			if (!file.good())
				throw ConfigParsing::FileOpeningError(); // Throwing FileOpeningError Exception is case there's an error occurred while opening the configuration file
			buffer << file.rdbuf(); // Reading configuration file data
			_rawData = buffer.str(); // Putting raw string file data into _rawData attribute
			file.close();
			buffer.clear();
		}


		/** @brief Parse file data
		 * Parsing configuration file using string
		 * streams
		 *
		 * @param none
		 * @return none
		 */
		void		parseFileData( void ) {
			std::stringstream	data(_rawData);
			string				line;
			size_type			serversCount;
			size_type			locationsCount;

			// TO-DO: Parse Configuration data from _rawData string and store it into ServerConfig class
			serversCount = -1;
			locationsCount = 0;
			_serversCount = _getServersCount();
			if (_serversCount == 0) {
				throw ConfigParsing::EmptyFileError();
			}
			_servers.resize(_serversCount);
			_getLocationCount();
			while (std::getline(data, line)) {
				if (line[0] == '#' || line[0] == '\0')
					continue;
				if (line[0] == '[') {
					locationsCount = 0;
					_servers.at(++serversCount).setServerName(line.substr(line.find('[') + 1, line.find(']') - 1));
				}
				else {
					line.erase(std::remove_if(line.begin(), line.end(), isspace), line.end());
					if (line.find(';') == std::string::npos) {
						throw ConfigParsing::FileParsingError();
					}
					// TO-DO: Switch line cases and fill the ServersConfig class with the right attributes
					switch (line[0]) {
						case 'h': {
							if (line.find("host=") == std::string::npos){
								throw ConfigParsing::FileParsingError();
							}
							_servers.at(serversCount).setHost(std::strtok(&line[5], ";"));
							break ;
						}
						case 'p' : {
							if (line.find("port=") == std::string::npos){
								throw ConfigParsing::FileParsingError();
							}
							_servers.at(serversCount).setPort(std::atoi(std::strtok(&line[5], ";")));
							break ;
						}
						case 'd': {
							if (line.find("default_error_pages=") == std::string::npos)
								throw ConfigParsing::FileParsingError();
							_servers.at(serversCount).setDefaultErrorPagePath(std::strtok(&line[std::strlen("default_error_pages=")], ";"));
							break ;
						}
						case 'b': {
							if (line.find("bodysize_limit=") == std::string::npos)
								throw ConfigParsing::FileParsingError();
							_servers.at(serversCount).setBodySizeLimit(std::stoi(line.substr(line.find('=') + 1, line.length())));
							break ;
						}
						case 'l': {
							if (line.find("location=") == std::string::npos)
								throw ConfigParsing::FileParsingError();
							_servers.at(serversCount).parseLocations(locationsCount++, &line[8]);
							break ;
						}
						default: throw ConfigParsing::FileParsingError();
					}
				}
			}
			if (!_finalCheck()) // Check if everything is parsed successfully
				throw ConfigParsing::FileParsingError();
			data.clear();
			return ;
		}
		class FileOpeningError : public std::exception{
		public:
			FileOpeningError ( void ) throw() {
				return ;
			}
			virtual ~FileOpeningError( void ) throw() {
				return ;
			}
			virtual const char *what() const throw() {
				return ("Error: Can't Open Configuration file.");
			}
		};
		class FileParsingError : public std::exception {
		public:
			FileParsingError( void ) throw() {
				return ;
			}
			virtual ~FileParsingError( void ) throw() {
				return ;
			}
			virtual const char * what() const throw() {
				return ("Error: Can't parse line.");
			}
		};
		class EmptyFileError : public std::exception {
		public:
			EmptyFileError( void ) throw() {
				return ;
			}
			virtual ~EmptyFileError( void ) throw() {
				return ;
			}
			virtual const char * what() const throw() {
				return ("Error: Empty configuration file.");
			}
		};
	};
}

#endif
