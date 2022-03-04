# include "includes/webserv.hpp"

void handler(int s) {
	printf("Caught SIGPIPE\n");
}

int main( int args, char **argv ) {
	INKARGUMENT		CMLArguments;
	ft::ConfigParsing ConfigParser;
	// TO-DO: Parsing Command line arguments
	try {
		CMLArguments.parseArguments(args, argv);
		if (CMLArguments.getHelp()) {
			std::cout << "Usage: ./webserv [options] [Configuration file path]" << std::endl;
			std::cout << "\t-t,--test		: Don't run, just test the configuration file." << std::endl;
			std::cout << "\t-v, --verbose		: Activate verbose mode." << std::endl;
			std::cout << "\t-h, --help		: Print help." << std::endl;
			return (0);
		}
		if (CMLArguments.getVerbose())
			std::cout << "\033[1;33m[!] Verbose mode activated\033[0m" << std::endl;
		if (CMLArguments.getFileCheck()) {
			ConfigParser.setFilename(CMLArguments.getFileName());
			try {
				if (CMLArguments.getVerbose())
					std::cout << "\033[1;32m[+] Testing Configuration file\033[0m" << std::endl;
				ConfigParser.readFileData();
				ConfigParser.parseFileData();
				std::cout << "inkwe: the configuration file " << CMLArguments.getFileName() << "syntax is ok" << std::endl;
				std::cout << "inkwe: configuration file " << CMLArguments.getFileName() << "test is successful" << std::endl;
				return (0);
			} catch (std::exception &e){
				std::cout << e.what() << std::endl;
				return (-1);
			}
		}
	} catch (std::exception &e) {
		std::cout << e.what() << std::endl;
		return (-1);
	}
	ConfigParser.setFilename(CMLArguments.getFileName());// Parsing Configuration file
	try {
		if (CMLArguments.getVerbose())
			std::cout << "\033[1;32m[+] Parsing Configuration file\033[0m" << std::endl;
		ConfigParser.parseFileData();
	} catch (std::exception &e){
		std::cout << e.what() << std::endl;
		return (-1);
	}
	try{
		std::vector<ft::Socket> sockets;
		ft::ServerConfig tmp;

		for (int i = 0; i < ConfigParser.getServerCount() ; i++) {
			tmp = ConfigParser.getServerConfig()[i];
			{
				ft::Socket sock(tmp.getHost().c_str(), tmp.getPort(), tmp);
				sockets.push_back(sock);
			}
		}
		ft::SocketIO sockio(sockets);
		signal(SIGPIPE, SIG_IGN);
		sockio.startServer();
	}
	catch (std::exception &e) {
		std::cout << e.what() << std::endl;
	}
	return (0);
}