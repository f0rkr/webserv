#ifndef SOCKETIO_HPP
# define SOCKETIO_HPP

# include "webserv.hpp"
# include <stdlib.h>

/** @brief ft namespace
 * includes all classes that we need
 */
namespace ft {
	/** @brief socket io class
	 * Contains all methods to execute poll
	 *
	 * @param none
	 * @return none
	 */
	class SocketIO {
	public:
		typedef std::string string;
		typedef std::vector<Socket> vector;
		typedef struct pollfd pollsd;
		typedef size_t size_type;
		typedef std::map<int, InkRespond>	respond_map;
		typedef std::map<int, request>	map;
		typedef std::map<int, std::vector<string> > mapi;
		typedef std::map<int, string>				maps;
		typedef std::map<int, bool>					mapcheck;
	private:
		int _timeout;
		int _close_conn;
		int _current_size;
		int _desc_ready;
		bool _end_server;
		bool _compress_array;
		vector _sockets;
		map								_reqMap;
		std::map<std::string, std::string> _types;
		respond_map 					_resMap;
		// struct pollfd _fds[200];
		std::vector<pollfd> _fds;
		std::pair<string, int>	_request_a;
//		map		_req;
		mapi	_reqVec;
		maps	_reqM;
		mapcheck						_isReadStream;
		int								_counter;

		/** @brief Timestamp method
		 * Return timestamp
		 *
		 * @param none
		 * @return std::time_t
		 */
		std::time_t _getTimestamp(void) const {
			return (std::time(nullptr));
		}


		/** @brief Listen descriptors
		 * Check whether the sd is a listen
		 * descriptor or not
		 *
		 * @param socket descriptor
		 * @return bool, true if it's listen descriptor and false if not
		 */
		bool _isListenSd(int sd) {
			for (int i = 0; i < _sockets.size(); i++) {
				if (sd == _sockets[i].getSocketSd())
					return (true);
			}
			return (false);
		}

		/** @brief Return socket class
		 * Search and return the right socket class
		 * for the incomming socket descriptor
		 *
		 * @param socket descriptors added successfully.
		 * @return Socket
		 */
		Socket *_findSd(int sd) {
			for (int i = 0; i < _fds.size(); i++) {
				if (sd == _sockets[i].getSocketSd())
					return (&(_sockets[i]));
			}
			return (nullptr);
		}

		/** @brief Return socket class
		 * Search and return the right socket class
		 * for the incomming client socket descriptor
		 *
		 * @param socket
		 * @return Socket class
		*/
		Socket *_findCd(int sd) {
			for (int i = 0; i < _sockets.size(); i++) {
				for (int j = 0; j < _sockets.at(i).getClients().size(); j++) {
					if (sd == _sockets.at(i).getClientsd(j))
						return (&_sockets.at(i));
				}
			}
			return (nullptr);
		}

		/** @brief Accept clients
		 * Accept all clients that been queued
		 * in accept
		 * 
		 * @param
		 * @return
		 */
		void		_accept_clients(int i) {
			struct pollfd 	fds;
			int				new_sd;

			// Listening descriptor is readable.
			// std::cout << "[" << _getTimestamp() << "]: " << _fds[i].fd << " Has a incomming connection."
			// << std::endl;
			
			// Accept all incoming connections that are
			// queued up on the listening socket before we
			// loop back and call poll again
			do {

				// Accept each incoming connection. if
				// accept fails with EWOULDBLOCK, then we
				// have accepted all of them. Any other
				// failure on accept will cause us to end
				// the server.
				new_sd = _findSd(_fds[i].fd)->accepts();
				if (new_sd < 0) {
					break;
				}
				// Add the new incoming connection to
				// the pollfd structure
				// std::cout << "[" << _getTimestamp() << "]: New incoming connection " << new_sd
				// << std::endl;
				fds.fd = new_sd;
				fds.events = POLLIN;
				_fds.push_back(fds);
				// Loop back up and accept another incoming
				// connection.
			} while (new_sd >= 0);
		}
	


		/** @brief Recv data
		 * Recv the request on certain socket descriptors
		 * with non-blocking method.
		 * 
		 * @param 
		 * @return
		 */
		bool		_recv_data(int i) {
			const char	*res;
			string 		myText;
			string 		method;
			char 		buffer[1024];
			int	 		rc;
			
			
			rc = 0;

			// Initialize buffer data.
			std::memset(&buffer, 0, sizeof(buffer));

			// Request
			ft::Socket *socket = _findCd(_fds[i].fd);

			// Receive data on this connection until the
			// recv fails with EWOULDBLOCK. if any other
			// failure occurs, we will close the connection
			rc = recv(_fds[i].fd, buffer, 1024, 0);
			// std::cout << buffer << std::endl;
			if (rc < 0) {
				// std::cout << "Error: recv error." << std::endl;
				return (false);
			}
				
			// Check to see if the connection has been
			// closed by the client
			if (rc == 0) {
				std::cout << "[" << _getTimestamp() << "]: Connection closed." << std::endl;
				return (false);
			}

			std::stringstream newBuffer(buffer);
			std::memset(&buffer, 0, sizeof(buffer));

			while (getline(newBuffer, myText)) {
				if (_reqM[_fds[i].fd].empty() && _counter == 0)
				{
					_counter = 1;
					_reqM[_fds[i].fd] = (myText);
				}
				else
					_reqVec[_fds[i].fd].push_back(myText);
			}
			_reqMap[_fds[i].fd].append(_reqVec[_fds[i].fd], _reqM[_fds[i].fd], socket->getServerConfig());
			_request_a = _reqMap[_fds[i].fd].parseReq(socket->getServerConfig());
			if (_reqMap[_fds[i].fd].is_complete())  {
				_fds[i].events = POLLOUT;
				_counter = 0;
			}
				
			return (true);
		}

		void		_reset_send(int i) {
			_reqMap.erase(_fds[i].fd);
			_reqVec.erase(_fds[i].fd);
			_reqM.erase(_fds[i].fd);
			_resMap[_fds[i].fd].streamClose();
			_resMap.erase(_fds[i].fd);
			_isReadStream[_fds[i].fd] = 0;
			return ;
		}

		/** @brief Send data
		 * Send all data to the client
		 * in chunked way
		 * 
		 * @param 
		 * @return none
		 */
		bool	_send_data(int i) {
			// Getting respond
							
			std::pair<std::string, int>		res;
			ft::Socket						*socket;
			int 							ret;
			int 							size;
			
			size = 0;
			socket =  _findCd(_fds[i].fd);
			res.first = "";
			
			if (_isReadStream[_fds[i].fd] == 0) {
				_resMap[_fds[i].fd].confRespond(socket->getServerConfig(), _reqMap[_fds[i].fd], _request_a);
				res = _resMap[_fds[i].fd].SetRespond(_reqMap[_fds[i].fd], socket->getServerConfig(), _types, _request_a.second);
				// std::cout << res.first << std::endl;
				
			} else {
				res = _resMap[_fds[i].fd].readStream();
				// std::cout << res.first << std::endl;
			}
			ret = send(_fds[i].fd, res.first.c_str(), res.second, 0);
			// std::cout << ret << std::endl;
			if (ret == 0 || ret < 0) {
				return (false);
			}
			if (_isReadStream[_fds[i].fd] == 0 && _resMap[_fds[i].fd].getFlag() != true) {
				ret = 0;
				_isReadStream[_fds[i].fd] = 1;
			}
			if (_resMap[_fds[i].fd].is_done(ret)) {
				if (_reqMap[_fds[i].fd].getDetails()["Connection"] != "keep-alive") {
					_reset_send(i);
					return (false);
				} else {
					_fds[i].events = POLLIN;
				}
				_reset_send(i);
			}
			return (true);
		}
	public:
		/** @brief Default constructor
		 * Construct an empty Socket io class
		 *
		 * @param none
		 * @return none
		 */
		SocketIO(void) {
			return;
		}

		/** @brief Default constructor
		 * Construct a new Socket io class
		 * to start the servers through polling
		 *
		 * @param vector
		 * @return none
		 */
		SocketIO(vector const &sockets) : _timeout(3 * 60 * 1000), _close_conn(),
										  _current_size(0), _desc_ready(), _end_server(false), _compress_array(false),
										  _sockets(sockets), _reqVec(mapi()), _reqM(maps()), _reqMap(map()), _resMap(respond_map()), _isReadStream(mapcheck()){
			// Initializing the fds poll array with zeros.
			// std::memset(_fds, -1, sizeof(_fds));
			// Init types
			coock_types();
			_counter = 0;
			// Adding all server sockets into the poll array
			for (int i = 0; i < sockets.size(); i++) {
				struct pollfd fds;
				 _sockets[i].startSocket();
				fds.fd =_sockets[i].getSocketSd();
				fds.events = POLLIN;
				_fds.push_back(fds);
			}
			return;
		}

		/** @brief Default destructor
		 * Destroy and clear all private attributes
		 * and memory allocations
		 *
		 * @param none
		 * @return none
		 */
		~SocketIO(void) {
			// Close all server sockets that are still open
			// before ending the server.
			for (int i = 0; i < _sockets.size(); i++) close(_sockets[i].getSocketSd());
		}

		/** @brief coock types
		 * get all content-types
		 *
		 * @param none
		 * @return none
		 */
		void coock_types(void) {
			std::ifstream file;
			std::stringstream buffer;
			std::string mytext;
			string path;

			char tmp[1024];

			// TO-DO: Get full path
			getcwd(tmp, 1024);

			if (tmp == nullptr)
				throw ft::ServerConfig::GetFolderPathError();

			path = string(tmp);
			file.open(path + "/conf.d/types.txt");
			buffer << file.rdbuf();
			while (getline(buffer, mytext))
				split(mytext, " ");
			file.close();
		}

		

		/** @brief start poll server
		 * Starting poll multiple server
		 *
		 * @param none
		 * @return none
		 */
		void startServer(void) {
			int new_sd = -1;
			
			int len = 1;
			int rc = 1;

			std::cout << "[" << _getTimestamp() << "]: Servers are starting..." << std::endl;
			do {
				
				// Call poll() and wait 3 minutes for it to complete.
				rc = poll(&_fds.front(), _fds.size(), -1);
				
				// Check to see if the poll call failed.
				if (rc < 0) {
					std::cerr << "[" << _getTimestamp() << "]: Error, Poll failed." << std::endl;
					break;
				}

				if (rc == 0) {
					continue;
				}
				
				for (int i = 0; i < _fds.size(); i++) {
					if (_fds[i].revents == 0)
						continue;

					if (_isListenSd(_fds[i].fd)) {
							_accept_clients(i);
					} else if (_fds[i].revents & POLLERR || _fds[i].revents & POLLNVAL || _fds[i].revents & POLLHUP) {
						// std::cerr << "[" << _getTimestamp() << "]: " << _fds[i].fd << " Connection closed." << std::endl;
						ft::Socket *socket = _findCd(_fds[i].fd);
						socket->rmClient(_fds[i].fd);
						close(_fds[i].fd);
						_fds[i].fd = -1;
						_fds.erase(_fds.begin() + i);
					} 
					else if (_fds[i].revents == POLLIN) {
						if (!_recv_data(i)) {
							_fds[i].events = POLLHUP;
							continue;
						}
					} else if (_fds[i].revents == POLLOUT) {
						if (!_send_data(i)) {
							_fds[i].events = POLLHUP;
							continue;
						}
					}
				}
			} while (!_end_server);
			// Clean up all of the sockets that are open
			for (int i = 0; i < _fds.size(); i++) {
				if (_fds[i].fd >= 0)
					close(_fds[i].fd);
			}
		}

	private :
		// anything to string
		template<class T>
		std::string To_string(T n) {
			std::ostringstream convert;
			convert << n;
			return (convert.str());
		}

		void split(std::string text, std::string dilemitter) {
			size_t pos = text.find(dilemitter);
			std::string token = text.substr(0, pos);
			std::string token1 = text.substr(pos + 1, text.length());
			_types.insert(std::pair<std::string, std::string>(token, token1));
		}
	};

}


#endif