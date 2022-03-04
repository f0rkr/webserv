/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InkSocket.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oel-ouar <oel-ouar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/11 12:51:54 by f0rkr             #+#    #+#             */
/*   Updated: 2022/02/28 19:09:46 by oel-ouar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
# define SOCKET_HPP

# include "webserv.hpp"
# include "InkRequest.hpp"
# include "InkServerConfig.hpp"
# include <vector>


/** @brief ft namespace
 * This namespace includes all class and object
 * that would be needed to complete this project
 */
namespace ft {
	/** @brief Socket class
	 * Socket class allow you to create socket
	 * descriptor, bind it and listen on it
	 *
	 * @param none
	 * @return none
	 */
	class Socket {
		public:
			typedef size_t								size_type;
			typedef std::string							string;
			typedef  std::vector<int>					vector;
			typedef struct sockaddr_in					sockaddrs;
		
		private:
			int						_socketsd; 	// Socket descriptor
			vector					_clientsd; // Vector of client socket descriptors
			char const				*_ipaddr; 	// Ip address
			uint const				_port;		// Port
			struct sockaddr_in				_address; 	// Internet Address
			ft::ServerConfig			_serverConfig; // Server configuration
			char					buffer[1024];

			/** @brief Default constructor
			 * Is made private because there's no use for it
			 * in the main class, but it's a good practice
			 * to include in the private methods field.
			 *
			 * @param none
			 * @return none
			 */
			Socket( void );
		
			/** @brief Timestamp method
			 * Return a timestamp
			 *
			 * @param none
			 * @return std::time_t
			 */
			std::time_t		getTimestamp( void ) const {
				return (std::time(nullptr));
			}

		public:
			/** @brief Default constructor
			 * Construct a new socket class using default values
			 * 
			 * @param port, ipaddress
			 * @return none
			 */
			Socket( char const *ip_addr, uint port, ft::ServerConfig &serverConfig) : _socketsd(0), _clientsd(vector(0, 0)),  _ipaddr(ip_addr), _port(port), _address(), _serverConfig(serverConfig), buffer(){
				// TO-DO: Initialize all attributes 
				std::memset(&_address, 0, sizeof(_address));
				std::memset(&_address.sin_zero, 0, sizeof(_address.sin_zero));
				_address.sin_family = AF_INET;
				_address.sin_port = htons(_port);
				inet_aton(_ipaddr, (struct in_addr *)&_address.sin_addr.s_addr);
				std::memset(buffer, 0, sizeof(buffer));
				return ;
			}
			
			/** @brief Default destructor
			 * Clear all private attributes and memory
			 * allocation before destroying the class
			 *
			 * @param none
			 * @return none
			 */
			~Socket( void ) { 
				// TO-DO: Reset to zero all attributes and deallocate all memory allocations.
				return ;
			}

			/** @brief Default private attributes getters
			 * Return the called attribute value
			 *
			 * @param none
			 * @return uint, char *, sockaddr, vector
			 */
			int					getSocketSd( void ) const { return (_socketsd); }
			int					getClientsd( int index ) const { return (_clientsd.at(index)); }
			vector				getClients( void ) const { return (_clientsd); }
			string				getIpaddr( void ) const { return (string(_ipaddr)); }
			uint				getPort( void ) const { return (_port); }
			ft::ServerConfig		getServerConfig( void ) const { return (_serverConfig); }
			char				*getBuffer( void ) const { return ((char *)&buffer[0]);}
			/** @brief Default private attributes setters
			 * Set new values to the attributes
			 *
			 * @param int
			 * @return bool
			 */
			bool	setClient(int clientsd) 
			{ 
				for (size_t i = 0 ; i < _clientsd.size() ; i++) {
					if (clientsd == _clientsd[0]) {
						std::cout << "[" << getTimestamp() << "]: Client is already in the list." << std::endl;
						return (false);
					}
				}
				// std::cout << "[" << getTimestamp() << "]: Client is added successfully." << std::endl;
				_clientsd.push_back(clientsd);
				return (true);
			}

			/** @brief Remove client descriptor
			 *
			 * @param clientsd
			 * @return boolean
			 */
			bool	rmClient(int clientsd) {
				std::vector<int>::iterator it = _clientsd.begin();
				for (;it != _clientsd.end();++it) {
					if (*it == clientsd)
						break;
				}
				_clientsd.erase(it);
				return (true);
			}
			/** @brief Socket creation
			 * Create socket descriptor
			 *
			 * @param none
			 * @return none
			 */
			void	createSocket( void ) {
				_socketsd = socket(AF_INET, SOCK_STREAM, 0);
				if (_socketsd < 0)
				{
					std::cout << "[" << getTimestamp() << "]: Error can't create socket." << std::endl;
					return ;
				}
				std::cout << "[" << getTimestamp() << "]: Socket is create successfully." << std::endl;
				return ;
			}

			/** @brief Bind socket
			 * Bind socket to the ipaddress and port
			 *
			 * @param none
			 * @return none
			 */
			void	bindSocket( void ) {
				if (bind(_socketsd, (struct sockaddr *)&_address, sizeof(_address)) < 0)
				{
					std::cout << "[" << getTimestamp() << "]: Can't bind socket." << std::endl;
					return ;
				}
				std::cout << "[" << getTimestamp() << "]: Socket is binded successfully." << std::endl;
				return ;
			}

			/** @brief socket listening on incoming connections
			 * listen for incoming connections
			 *
			 * @param none
			 * @return none
			 */
			void	listens( void ) const {
				if (listen(_socketsd, 1024) < 0) {
					std::cout << "[" << getTimestamp() << "]: Error can't listen on socket." << std::endl;
					return ;
				}
				std::cout << "[" << getTimestamp() << "]: Listening on " << _ipaddr << ":" << _port << std::endl;
				return ;
			}


			/** @brief Accept clients
			 * Accept incoming connection and store
			 * new client descriptor
			 *
			 * @param client socket
			 * @return socket descriptor
			 */
			int		accepts() {
				int	clientsd;
			
				clientsd = accept(_socketsd, NULL, NULL);
				if (clientsd != -1)
					setClient(clientsd);
				return (clientsd);
			}

			/** @brief configure socket
			 * Configure socket descriptor for listening
			 *
			 * @param none
			 * @return bool
			 */
			void	startSocket( void ) {
				int on = 1;

				createSocket();
				if (setsockopt(_socketsd, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on)) < 0)
				{
					std::cerr << "[" << getTimestamp() << "]: Setsockopt failed." << std::endl;
					close(_socketsd);
					exit(-1);
				}
				// Set Socket to be non-blocking. All sockets for
				// the incoming connections will also be non-blocking since
				// they will inherit that state from the listening socket.
				if (fcntl(_socketsd, F_SETFL, O_NONBLOCK) < 0) 
				{
					std::cerr << "[" << getTimestamp() << "]: Ioctl failed." << std::endl;
					close(_socketsd);
					exit(-1);
				}
				bindSocket();
				listens();
				return ;
			}
	};
}

#endif
