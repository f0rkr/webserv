


#ifndef INKCONFIGPARSER_HPP
#define INKCONFIGPARSER_HPP

# include "InkServerConfig.hpp"
# include "webserv.hpp"

class INKCONFIGPARSER
{
	private:
		std::string				_filename; // Configuration file name
		size_t					_serversCount; // Servers counter
		std::string				_rawData; // Raw file data
		INKSERVERCONFIG			*_inkweConfig; // Configuration key and values holder

		// Private methods declaration.
		size_t 				_GetServersCount( void ) { // Return server counts
			std::stringstream	data(this->_rawData);
			std::string			line;
			size_t 				serversCount;

			serversCount = 0;
			while (std::getline(data, line, '\n')) {
				if (line[0] == '[')
					serversCount++;
			}
			return (serversCount);
		}
		void 				_GetLocationCount( void ) {
			std::stringstream	data(this->_rawData);
			std::string			line;
			size_t 				locationsCount;
			size_t 				serversCount;

			locationsCount = 0;
			serversCount = 0;
			while (std::getline(data, line, '\n'))
			{
				line.erase(std::remove_if(line.begin(), line.end(), isspace), line.end());
				if (line[0] == '#' || line[0] == '\n' || line[0] == '\0')
					continue ;
				while (std::getline(data, line, '\n') && line[0] != '[')
				{
					if (line[0] == '#' || line[0] == '\n' || line[0] == '\0')
						continue ;
					line.erase(std::remove_if(line.begin(), line.end(), isspace), line.end());
					if (line.find("location=") != std::string::npos)
						locationsCount++;
				}
				if (locationsCount == 0)
					throw INKCONFIGPARSER::FileParsingError();
				else
					this->_inkweConfig[serversCount++].setInkLocations(locationsCount);
				if (line[0] == '[')
					locationsCount = 0;
			}
			return ;
		}
		bool 				_FinalCheck( void ) const {
			// TO-DO: Checking if everything is parsed successfully
			size_t		serversCount;
			size_t 		locationsCount;

			for (serversCount = 0; serversCount < this->_serversCount; serversCount++) {
				if (!this->_inkweConfig[serversCount].checkDefaultValues()) {
					return (false);
				}
				for (locationsCount = 0; locationsCount < this->_inkweConfig->getLocationsCount(); locationsCount++) {
					if (!this->_inkweConfig[serversCount].getInkLocation()[locationsCount].checkLocationDefaultValues())
						return (false);
				}
			}
			return (true);
		}
	public:
		INKCONFIGPARSER( void ): _filename("conf.d/default.conf") {
			this->_serversCount = 0;
			this->_inkweConfig = new INKSERVERCONFIG();
			return ;
		}
		INKCONFIGPARSER ( std::string const &filename ): _filename(filename) {
			this->_serversCount = 0;
			return ;
		}
		INKCONFIGPARSER( INKCONFIGPARSER const &COPY) {
			*this = COPY;
		}
		~INKCONFIGPARSER ( void ) {
			return ;
		}

		INKCONFIGPARSER		&operator=(INKCONFIGPARSER const &OP) {
			this->_filename = OP.getFileName();
			return (*this);
		}
		size_t				getServerCount( void ) const {
			return (this->_serversCount);
		}
		std::string			getFileName( void ) const {
			return (this->_filename);
		}
		void 				setConfigFile( std::string const &filename ) {
			this->_filename = filename;
			return ;
		}
		void 				readFileData( void ) {
			std::ifstream		file;
			std::stringstream	buffer;

			// TO-DO: Read Configuration file and store it into a data string
			file.open(this->_filename); // Opening Configuration file
			if (!file.good()) {
				throw INKCONFIGPARSER::FileOpeningError(); // Throwing FileOpeningError Exception in case there's an error in opening the configuration file
				return ;
			}
			buffer << file.rdbuf(); // Reading configuration file data
			this->_rawData = buffer.str(); // Putting raw string file data into _rawData attribute
		}
		void 				ParseFileData() {
			std::stringstream	data(this->_rawData);
			std::string			line;
			size_t				ServersCounter;
			size_t 				LocationsCounter;

			// TO-DO: Parse configuration data from _rawData string and store into inkweConfig
			ServersCounter = -1;
			LocationsCounter = 0;
			this->_serversCount = this->_GetServersCount(); // Get server count in configuration file
			this->_inkweConfig = new INKSERVERCONFIG[this->_serversCount]; // Allocate InkweConfig as size
			this->_GetLocationCount();
			while (std::getline(data, line)) {
				if (line[0] == '#' || line[0] == '\0')
					continue;
				if (line[0] == '[') {
					LocationsCounter = 0;
					this->_inkweConfig[++ServersCounter].setServerName(line.substr(line.find('[') + 1, line.find(']') -1));
				}
				else
				{
					line.erase(std::remove_if(line.begin(), line.end(), isspace), line.end());
					if (line.find(';') == std::string::npos) {
						throw INKCONFIGPARSER::FileParsingError();
					}
					// TO-DO : switch line cases and fill the inkweConfig class with the right attributes
					switch (line[0]) {
						case 'h': {
							if (line.find("host=") == std::string::npos){
								throw INKCONFIGPARSER::FileParsingError();
							}
							this->_inkweConfig[ServersCounter].setHost(std::strtok(&line[5], ";"));
							break ;
						}
						case 'p' : {
							if (line.find("port=") == std::string::npos){
								throw INKCONFIGPARSER::FileParsingError();
							}
							this->_inkweConfig[ServersCounter].setPort(std::atoi(std::strtok(&line[5], ";")));
							break ;
						}
						case 'd' : {
							if (line.find("default_error_pages=") == std::string::npos){
								throw INKCONFIGPARSER::FileParsingError();
							}
							this->_inkweConfig[ServersCounter].setDefaultErrorPage(std::strtok(&line[5], ";"));
							break ;
						}
						case 'b': {
							if (line.find("bodysize_limit=") == std::string::npos){
								throw INKCONFIGPARSER::FileParsingError();
							}
							this->_inkweConfig[ServersCounter].setBodySizeLimit(std::stoi(line.substr(line.find('=') + 1, line.length())));
							break ;
						}
						case 'l': {
							if (line.find("location=") == std::string::npos){
								throw INKCONFIGPARSER::FileParsingError();
							}
							this->_inkweConfig[ServersCounter].getInkLocation()[LocationsCounter++].ParseInkLocation(&line[8]);
							break ;
						}
						default: throw INKCONFIGPARSER::FileParsingError();
					}
				}
			}
			if ( !this->_FinalCheck() ) // Check if everything is parsed successfully
				throw INKCONFIGPARSER::FileParsingError();
		}
		class FileOpeningError : public std::exception{
			public:
				FileOpeningError( void ) throw(){
					return ;
				}
				virtual ~FileOpeningError( void ) throw(){
					return ;
				}
				virtual const char* what() const throw(){
					return ("Error: Can't Open Configuration file.");
				}
		};
		class FileParsingError : public std::exception{
			public:
				FileParsingError( void ) throw(){
					return ;
				}
				virtual ~FileParsingError( void ) throw(){
					return ;
				}
				virtual const char * what() const throw(){
					return ("Error: Can't parse line.");
				}
		};

};

#endif
