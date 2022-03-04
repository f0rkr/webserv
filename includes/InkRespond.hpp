/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InkRespond.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oel-ouar <oel-ouar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/22 09:56:30 by                   #+#    #+#             */
/*   Updated: 2022/03/02 05:00:41 by oel-ouar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef INKRESPOND_HPP
# define INKRESPOND_HPP
// Headers
#include "InkCgi.hpp"
#include "InkRequest.hpp"
#include "InkServerConfig.hpp"
#include "InkAutoIndex.hpp"
#include <fstream>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <dirent.h>

namespace ft {
	class InkRespond {
		public:
			typedef size_t size_type;
			typedef std::string string;
		private:
			string 			_ret;
			string 			_status;
			string			_default_error;
			size_type		_err;
			size_type		_cgi;
			size_type		_file_size;
			size_type		_current_size;
			std::ifstream 	_stream;
			size_type		_header_size;
			bool 			_flag;
			bool			_readstream;
			int				_sstream;
			InkRespond& 	operator=(const InkRespond& op);
		public:
			InkRespond(void) : _err(0), _cgi(0), _file_size(0), _current_size(0), _flag(false), _readstream(false){
				
				return;
			}
			InkRespond(const InkRespond &copy): _err(0), _cgi(0), _file_size(0), _current_size(0), _flag(false), _readstream(false){
				return ;
			}
			explicit InkRespond(const ft::ServerConfig &conf, const ft::request &req, const std::pair<std::string, int> &a) : _err(
					0), _cgi(0), _file_size(0), _current_size(0), _header_size(0), _flag(false) {
				return ;
			}

			// Set respond
			bool	checkFileOpen(const char *file) {
				int d;

				d = open(file, O_RDONLY);

				if (d == -1)
					return (false);
				close(d);
				return (true);
			}

			bool		checkDirOpen(const char *path) {
				DIR *dd;

				dd = nullptr;			
				dd = opendir(path);

				if (dd == nullptr)
					return (false);
				closedir(dd);
				return (true);
			}
			
			void confRespond(const ft::ServerConfig &conf, const ft::request &req, const std::pair<std::string, int> &a) {
				_err = 0;
				_cgi = 0;
				_file_size = 0;
				_current_size = 0;
				_flag = false;
				_readstream = false;
				char get[1024];
				getcwd(get,1024);
				_default_error = To_string(get)+"/var/www/pages/";
				// check if its a valid request nethod
				std::string file;
				if (a.first == "200")
				{
					if (req.getScriptName().length() == 0)
						file = conf.getLocations()[a.second].getRoot() + "/";
					else	
						file = conf.getLocations()[a.second].getRoot() + "/"+ req.getScriptName();
					
					// check if path exists
					if (req.getPath()=="/" && req.getScriptName().length() == 0)
					{
						_status = "index.html";
						_ret = "HTTP/1.1 200 OK\n";
					}
					else if (checkFileOpen(file.c_str()) == false)
					{
						_status = "404";
						_ret ="HTTP/1.1 404 Not Found\n";
						_err = 1;
					}
					
					// check if we will use cgi
					else if ((req.getScriptName().find(".php") != std::string::npos || req.getScriptName().find(".py") != std::string::npos) 
						&& (req.getMethod()=="GET" || req.getMethod()=="POST") && (conf.getLocations()[a.second]).getAutoIndex() == 0)
					{
						std::pair<std::string,std::string> p;
						
						InkCgi cgi(req, conf.getLocations()[a.second]);
						p = cgi.execCgi(req);
						_status = p.first;
						_ret = 
						_ret = p.second;
						if (_status[0] == '4' || _status[0]=='5')
							_err = 1;
						_cgi = 1;
					}
					
					// DELETE request
					else if (req.getMethod()=="DELETE")
					{
						std::string interdit;
						std::string interdit1;
						
						interdit = To_string(get) + "/include";
						interdit1 = To_string(get) + "/var/www/pages";
						std::string rm = conf.getLocations()[a.second].getRoot() + "/"+req.getScriptName();
						if (!strncmp(interdit.c_str(), rm.c_str(), interdit.length()) || !strncmp(interdit1.c_str(), rm.c_str(), interdit1.length()) || remove(rm.c_str()) == -1)
						{
							_status = "403";
							_ret = "HTTP/1.1 403 Not Allowed\n";
							_err = 1;
						}
						else{
						_status = "204";
						_ret = "HTTP/1.1 204 No Content\n\n";
						}
					}
					else {
						_ret = "HTTP/1.1 200 OK\n";
						_status = req.getScriptName();
					}
				}
				else 
				{
					_ret = a.first;
					_status = To_string(a.second);
					_err = 1;
				}
			}

			// Cooking respond
			std::pair<std::string, std::streampos>
			SetRespond(const ft::request &req, const ft::ServerConfig &conf, std::map<std::string, std::string> types,
					   int locationPos) {
				
				std::string opn;
				std::string ext;
				std::string scriptname;
				std::vector<std::string> _headers;
				std::streampos size;
				std::string filePath;
				std::pair<std::string, bool> Autoindex;
				
				if(!_err)
				{
					std::string urlPath = "";

					if (req.getPath()[req.getPath().length() - 1] == '/')
						urlPath = req.getPath() + req.getScriptName();
					else 
						urlPath = req.getPath() + "/" + req.getScriptName();
					Autoindex = check_autoIndex(urlPath ,conf.getLocations()[locationPos].getRoot()+"/" +req.getScriptName(), conf.getLocations()[locationPos]);;
					if (req.getPath() == "/" && _status == "index.html")
						filePath = conf.getLocations()[locationPos].getRoot() +"/"+_status;
					else
						filePath = conf.getLocations()[locationPos].getRoot()+ "/" +req.getScriptName();
				}
				scriptname = req.getScriptName();
				if (scriptname.find('.') != std::string::npos)
					ext = scriptname.substr(scriptname.find('.') + 1, scriptname.length());
				else
					ext = "";
				if (Autoindex.second == 1 && Autoindex.first.length() == 0)
				{
					_headers.push_back("Content-Disposition: attachment\r\n");
				}
				if (req.getMethod()=="DELETE" && _status == "204") {
					_flag = true;
					return (std::make_pair(_ret, _ret.length()));
				}
				else if (types.find(ext)!=types.end() && _err==0)
					_headers.push_back("Content-Type: "+types.find(ext)->second+"\r\nContent-Length: ");
				else
					_headers.push_back("Content-Type: text/html; charset=UTF-8\r\nContent-Length: ");
				if ((req.getScriptName().find(".php") != std::string::npos && (_err == 1 || Autoindex.second == 1)) || req.getScriptName().find(".php") == std::string::npos) {
					for (std::vector<std::string>::iterator it = _headers.begin(); it != _headers.end();it++)
						_ret += *it;
					_header_size = _ret.length();
				}
				if (_err==1 && conf.getDefaultErrorPagePath() != "")
					opn = conf.getFullPath() + conf.getDefaultErrorPagePath()+ "/" +_status+".html";
				else 
					opn = filePath;

				if (_cgi ==1 && _err==0 && Autoindex.second == 0)
				{
					if (req.getScriptName().find(".php") != std::string::npos)
					{
						std::string l = To_string(_status.substr(_status.find("\r\n\r\n")+4, _status.length()).length());
						_ret = _ret +"Content-Length: "+ l +"\r\n" +_status;
					}
					else
						_ret += To_string(_status.length()) + "\r\n\r\n" +_status;
					size = _ret.length();
					_flag = true;
				} else {
					if (Autoindex.first.length() != 0)
					{
						_ret += To_string(Autoindex.first.length())+"\r\n\r\n" + Autoindex.first;
						size = _ret.length();
						_flag = true;
					} else {
						int i;
						int tmp;
						_sstream = open(opn.c_str(), O_RDONLY);

						// _stream.open(opn, std::ios::in | std::ios::binary | std::ios::ate);
						if (_err == 1 && _sstream == -1)
						{
							opn = _default_error +_status+".html";
							_sstream = open(opn.c_str(), O_RDONLY);
						}
						_file_size = lseek(_sstream, 0, SEEK_END);
						_current_size = 0;
						size = 0;
						lseek(_sstream, 0, SEEK_SET);
						_ret += To_string(_file_size) + "\r\n\r\n";
						size += _ret.length();
					}
				}
				return (std::make_pair(_ret, size));
			}
			std::pair<std::string, std::streampos>	readStream() {
				char 			file[2048*100];
				int 			tmp;
				int				size;
				int 			i;
				std::string	ret = "";
				struct pollfd fds;

				fds.fd = _sstream;
				fds.events = POLLIN;

				int rc = poll(&fds, 1, 0);

				if (rc < 0) {
					_flag = true;
					close(_sstream);
				}
				if (rc > 0 && fds.revents & POLLIN) {
					std::memset(&file, 0, 2048*100);
					int nc = read(fds.fd, &file, 2048*100);
					
					tmp = nc;
					if (nc < 0) {
						_flag = true;
						close(fds.fd);
						return (std::make_pair("500", 3));
					} else if (nc > 0) {
						for (i = 0; i < tmp; i++)
							ret.push_back(file[i]);
						std::memset(file, 0, 2048*10);
					} else if (nc == 0) {
						_flag = true;
						close(fds.fd);
						return (std::make_pair("505", 3));
					}	
					size = tmp;
				}
				return (std::make_pair(ret, size));
			}

			void 		streamClose( void ) {
				_stream.close();
				_ret.clear();
				return ;
			}
			size_type	getHeaderSize( void ) const {
				return (_header_size);
			}
			bool		getFlag( void ) const {
				return (_flag);
			}
			std::pair<std::string, bool>
			check_autoIndex(const std::string &urlPath, const std::string &path, const ft::Location &location) {
				int d = -1;
				
				if (location.getAutoIndex()) {
					ft::AutoIndex aut(urlPath, path);
					if (checkDirOpen(path.c_str())) {
						return (std::make_pair(aut.baseHref(), 1));
					} else if (checkFileOpen(path.c_str())) {
						return (std::make_pair("", 1));
					}
				}
				close(d);
				return (std::make_pair("", 0));
			}

			bool is_done(size_type position) {
				if (_flag == true) {
					_flag = false;
					return (true);
				}
				_current_size += position;
				lseek(_sstream, _current_size, SEEK_SET);
				return (false);
			}
			bool	getReadStream(void) const {
				return (_readstream);
			}
		private:
			template<class T>
			std::string To_string(T n) {
				std::ostringstream convert;
				convert << n;
				return (convert.str());
			}
	};
}
#endif
