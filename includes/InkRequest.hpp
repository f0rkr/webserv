/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InkRequest.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oel-ouar <oel-ouar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/21 11:12:47 by                   #+#    #+#             */
/*   Updated: 2021/10/01 10:27:36 by                  ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <iostream>
# include <map>
# include <vector>
# include <iterator>

class request
{
	private:
		std::string _method;
		std::string _serverProtocol;
		std::string _path;
		std::string _query;
		std::map<std::string, std::string> _details;

	public:
		request(void) { }
		~request(void) {}
		void parseRequest(std::vector<std::string> myVec, std::string method)
		{
			splitMethod(method);
			splitPath(_path);
			std::map<std::string, std::string>::iterator mite = _details.end();
			std::vector<std::string>::iterator ite = myVec.end();
			// parse every details of the request into a map with akey and value
			for (std::vector<std::string>::iterator it = myVec.begin(); it != ite; it++)
				split(*it, ": ");
			std::cout << "Method  " << _method << std::endl;
			for(std::map<std::string, std::string>::iterator it = _details.begin(); it != mite; it++)
				std::cout << "key " << it->first << "        value " << it->second << std::endl; 
		}
		std::string getMethod( void ) const {
			return (_method);
		}
		std::string getPath( void ) const {
			return (_path);
		}
		std::string getServerProtocol( void ) const {
			return (_serverProtocol);
		}
		std::string getQuery( void ) const {
			return (_query);
		}

	private:
		void split(std::string text, std::string dilemitter)
		{
			size_t pos = text.find(dilemitter);
			std::string token = text.substr(0, pos);
			std::string token1 = text.substr(pos + 2, text.length());
			_details.insert(std::pair<std::string, std::string>(token, token1));
		}
		void splitMethod(std::string method)
		{
			size_t pos = method.find(" ");
			_method = method.substr(0, pos);
			method.erase(0, pos + 1);
			pos = method.find(" ");
			_path = method.substr(0, pos);
			method.erase(0, pos);
			_serverProtocol = method.substr(pos, method.length());
		}
		void splitPath(std::string path)
		{
			size_t pos;
			if ((pos = path.find("?")))
				_query = path.substr(pos, path.length());
			
		}
};