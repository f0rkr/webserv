/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vortex_socket.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By:  <mashad@student.1337.ma>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/11 07:54:14 by                   #+#    #+#             */
/*   Updated: 2021/09/26 07:44:07 by                  ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef INKSOCKET_HPP
#define INKSOCKET_HPP

# include "./webserv.hpp"

class VortexSocket
{
	private:
		int							_socket_fd;
		int 						_client_fd[100];
		int 						_client_len;
		char const					*_ip_addr;
		uint const					_port;
		struct sockaddr_in			_address;
	public:
		VortexSocket( void ): _ip_addr("0.0.0.0"), _port(8000)
		{
			return ;
		}
		VortexSocket( char const *ip_addr, uint port ): _ip_addr(ip_addr), _port(port)
		{
			this->_socket_fd = 0;
			this->_client_len = 0;
			std::memset(&this->_address, 0, sizeof(this->_address));
			this->_address.sin_family = AF_INET;
			this->_address.sin_port = htons(getPort());
			this->_address.sin_addr.s_addr = inet_addr(getIPaddr());
			return ;
		}
		~VortexSocket( void ) {
			return ;
		}

//		VortexSocket const	&operator=(VortexSocket const &op);
		uint				getPort() const
		{
			return (this->_port);
		}
		char const  		*getIPaddr() const
		{
			return (this->_ip_addr);
		}
		int 				getSocketFd() const
		{
			return (this->_socket_fd);
		}
		struct sockaddr_in 	getAddress() const
		{
			return (this->_address);
		}
		int 				getClientfd( int index ) const
		{
			return (this->_client_fd[index]);
		}
		void 				writeToClients(int fd, char *buffer) const
		{
			for (int i = 0; i < this->_client_len; i++)
			{
				if (getClientfd(i) != fd)
					write(getClientfd(i), buffer, std::strlen(buffer));
				write(fd, "Content-type:text/html\\r\\n\\r\\nit's working\n", std::strlen("Content-type:text/html\\r\\n\\r\\nit's working\n"));
			}
		}
		int					createSocket()
		{
			this->_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
			if (this->_socket_fd < 0)
				return (-1);
			std::cout << "Socket is created successfully." << std::endl;
			return (this->_socket_fd);
		}
		int					bindSocket()
		{
			int 					socket_fd;

			socket_fd = getSocketFd();
			if (bind(socket_fd, (sockaddr *)&this->_address, sizeof(this->_address)) < 0) {
				std::cout << "Can't bind." << std::endl;
				return (-1);
			}
				std::cout << "Socket is binded." << std::endl;
			return (0);
		}
//		void			vortex_clients(int fd) {
////			char	*buffer;
////
////			std::cout << "Client accepted on fd: " << fd << std::endl;
//
////			buffer = (char *) malloc(sizeof(char) * 1024);
////			while (read(fd, buffer, 1024) > 0) {
////				if (buffer != NULL && std::strcmp(buffer, "end\n") == 0) {
////					write(fd, "Thala a zamel\n", std::strlen("Thala a zamel\n"));
////					free(buffer);
////					buffer = NULL;
////					close(fd);
////					break ;
////				}
////				if (buffer != NULL)
////					std::cout << "Buffer received: " << buffer;
////				writeToClients(fd, buffer);
////				free(buffer);
////				buffer = NULL;
////				buffer = (char *) malloc(sizeof(char) * 1024);
////			}
////			close(fd);
//			(void)fd;
//		}

		int 				vortex_startServer( void )
		{
			int		addr_len;
			char *buffer = NULL;
			int 	fd;

			addr_len = sizeof(getAddress());
			buffer = new char[1024];
			if (listen(getSocketFd(), 10) < 0)
				return (-1);
			std::cout << "Listening on " << getIPaddr() << ":" << getPort() << std::endl;
			fd = accept(getSocketFd(), (sockaddr *)&this->_address, (socklen_t *) &addr_len);
			while (1) {
				int d = fork();
				if (d == 0) {
					recv(fd, buffer, 1024, 0);
					std::cout << buffer << std::endl;
					int file = open("pages/404.html", O_RDONLY);
					char res[] = "HTTP/1.1 200 OK\nlocation: http://localhost:1337\nContent-Type: text/html\nContent-Length: 6000\r\n\n";
					send(fd, res, std::strlen(res), 0);
					while (read(file, buffer, 1024)) {
						send(fd, buffer, std::strlen(buffer), 0);
						free(buffer);
						buffer = new char[1024];
					}
					send(fd, "\r\n", std::strlen("\r\n"), 0);
					close(file);
					recv(fd, buffer, 1024, 0);
					std::cout << buffer << std::endl;
					char red[] = "HTTP/1.1 200 OK\nlocation: http://localhost:1337/ink.png\nContent-Type: image/png\nContent-Length: 10000\r\n\n";
					send(fd, red, std::strlen(red), 0);
					free(buffer);
					int img = open("pages/ink.png", O_RDONLY);
					buffer = new char[1024];
					while (read(img, buffer, 1024) >= 0) {
						send(fd, buffer, std::strlen(buffer), 0);
					}
					send(fd, "\r\n", std::strlen("\r\n"), 0);
					close(img);
					close(fd);
				}
				return (0);
			}
		}
};
#endif
