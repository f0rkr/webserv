/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InkServerConfig.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oel-ouar <oel-ouar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/10 18:58:38 by f0rkr             #+#    #+#             */
/*   Updated: 2022/02/28 19:29:58 by oel-ouar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERCONFIG_HPP
# define SERVERCONFIG_HPP


# include "webserv.hpp"
# include "InkLocation.hpp"

/** @brief ft namespace
 * define all oop classes and functions
 * inside ft namespaces for better use and
 * accessibility
 */
namespace ft {
	/** @brief Server config class
	 * After parsing file all values for each server are
	 * stored in this class.
	 *
	 * @param none
	 * @return none
	 */
	class ServerConfig {
	public:
		typedef size_t								size_type;
		typedef	std::string							string;
		typedef	std::vector<Location>				vector;
		typedef	std::allocator<vector>	allocator_type;
	private:
		string		_servername;		// Server name
		string		_host;				// Host ip address to bind
		string		_defaultErrorPages;	// Path to default error pages
		size_type	_port;				// Port number to bind
		size_type	_bodySizeLimit;		// Request body size limit
		size_type	_locationsCount;	// Location size
		vector		_locations;			// Vector of location classes
		string 		_dirPath;			// Directory path

		// Allocator for vector
		allocator_type	_alloc;
	public:
		/** @brief Default constructor
		 * Construct new Server config class
		 * with default value configuration
		 *
		 * @param none
		 * @return none
		 */
		ServerConfig( void ): _servername(""), _host(""), _defaultErrorPages("/var/www/pages"), _port(0), _bodySizeLimit(0), _locationsCount(0), _locations(0), _alloc(allocator_type()) {
			char tmp[1024];

			getcwd(tmp, 1024);
			if (tmp == nullptr) {
				throw ServerConfig::GetFolderPathError();
			}
			_dirPath = string(tmp);
			return ;
		}

		/** @brief Default destructor
		 * destroy all memory allocations and clear all
		 * private attributes before destroying the class
		 *
		 * @param none
		 * @return none
		 */
		~ServerConfig() {};

		/** @brief Private attributes getters
		 * Return the called attribute value
		 *
		 * @param none
		 * @return string, size_type, vector
		 */
		string		getServerName( void ) const { return (_servername); }
		string		getHost( void ) const { return (_host); }
		string		getDefaultErrorPagePath( void ) const { return (_defaultErrorPages); }
		size_type	getPort( void ) const { return (_port); }
		size_type	getBodySizeLimit( void ) const { return (_bodySizeLimit); }
		size_type	getLocationsCount( void ) const { return (_locations.size()); }
		vector		getLocations() const { return (_locations); }
		string 		getFullPath() const { return (_dirPath); }

		/** @brief Private attributes setters
		 * Set private attributes values from parameter
		 *
		 * @param string, size_type
		 * @return none
		 */
		void		setServerName( string const &servername ) { _servername = servername;}
		void		setHost( string const &host ) { _host = host;}
		void		setDefaultErrorPagePath( string const &errorPath ) { _defaultErrorPages = errorPath;}
		void		setPort( size_type	port ) { _port = port;}
		void		setBodySizeLimit( size_type sizeLimit ) { _bodySizeLimit = sizeLimit;}
		void		setLocationsCount( size_type locationscount ) { _locationsCount = locationscount;}
		void		setLocations( size_type locationsCount ) { _locationsCount = locationsCount; _locations.resize(locationsCount); }
		void 		parseLocations(size_type position, const std::string& RawData) {_locations.at(position).parseLocation(RawData);}
		/** @brief Check default values
		 * Check if all values are set properly
		 * if not return false
		 *
		 * @param none
		 * @return bool
		 */
		bool	checkDefaultValues( void ) const {
			// TO-DO: Check if all variables are set if not return false, else return true
			if (_host.empty() || _port == 0 || _bodySizeLimit == 0 | _locationsCount == 0)
				return (false);
			return (true);
		}

		class GetFolderPathError : public std::exception {
		public:
			GetFolderPathError( void ) throw() {
				return ;
			}
			virtual ~GetFolderPathError( void ) throw() {
				return ;
			}
			virtual const char * what() const throw() {
				return ("Error: Can't read this directory full path.");
			}
		};
	};
}

#endif
