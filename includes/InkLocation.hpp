/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp 	                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mashad <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/06 16:16:38 by mashad            #+#    #+#             */
//*   Updated: 2022/02/02 12:55:42 by                  ###   ########.fr       *//*                                                                            */
/* ************************************************************************** */

# ifndef LOCATION_HPP
# define LOCATION_HPP

# include "webserv.hpp"

namespace ft {
	/** @brief Location class
	 * Location class that contain all key and value
	 * variables to parse the location from configuration
	 * file
	 *
	 * @param none
	 * @return none
	 */
	class Location {
	public:
		typedef size_t size_type;
		typedef std::string string;
	private:
		string _location;  // Location path
		string _method;    // Allowed method to use in this location
		string _root;        // Root file path
		bool _autoIndex;    // Enable or disable directory listing
	public:
		/** @brief Default constructor
		 * Construct a new location class with
		 * default values
		 *
		 * @param none
		 * @return none
		 */
		Location(void) : _location(""), _method(""), _root(""), _autoIndex(false) {
			return;
		}

		/** @brief Default destructor
		 * Destroy any memory allocation and clear all
		 * private attribute
		 *
		 * @param none
		 * @return none
		 */
		~Location(void) { return; }

		/** @brief Private attributes getters
		 * Return private attributes values
		 *
		 * @param none
		 * @return string, bool
		 */

		string getLocation(void) const { return (_location); }

		string getMethod(void) const { return (_method); }

		string getRoot(void) const { return (_root); }

		bool getAutoIndex(void) const { return (_autoIndex); }


		/** @brief Private attributes setters
		 * Set private attributes values
		 *
		 * @param string, bool
		 * @return none
		 */

		void setLocation(string location) {
			_location = location;
			return;
		}

		void setMethod(string method) {
			_method = method;
			return;
		}

		void setRoot(string root) {
			_root = root;
			return;
		}

		void setAutoIndex(bool autoIndex) {
			_autoIndex = autoIndex;
			return;
		}

		/** @brief Check Location default values
		 * Check if all location values are set and not empty
		 *
		 * @param none
		 * @return bool, true if all values are set and false if one them is empty.
		 */
		bool checkLocationDefaultValues(void) const {
			// TO-DO: checking if default values are set, if not return false

			if (_location.empty() || _method.empty() || _root.empty())
				return (false);
			return (true);
		}

		/** @brief Parse Location
		 * Parsing rawData location into private attributes of this class
		 *
		 * @param rawData, no parsed location line
		 * @return bool, true if the location is parsed, false if an error occurred.
		 */
		bool parseLocation(string rawData) {
			std::stringstream data(rawData); // changing rawData string into a string-stream
			string line;

			// TO-DO: Parse data line and extra values into their right attributes

			if (rawData.find('{') == string::npos || rawData.find("};") == string::npos)
				return (false);
			data.clear();
			rawData.replace(rawData.find('}'), 2, ",");
			rawData = rawData.substr(2, rawData.length());
			data.str(rawData);
			while (std::getline(data, line, ',')) {
				switch (line[0]) {
					case 'p': {
						if (line.find("path=") == string::npos)
							return (false);
						setLocation(line.substr(line.find("=") + 1, line.length()));
						break;
					}
					case 'm': {
						if (line.find("method=") == string::npos)
							return (false);
						setMethod(line.substr(line.find('=') + 1, line.length()));
						break;
					}
					case 'r': {
						if (line.find("root=") == string::npos)
							return (false);
						setRoot(line.substr(line.find('=') + 1, line.length()));
						break;
					}
					case 'a': {
						if (line.find("autoindex=") == string::npos)
							return (false);
						setAutoIndex(line.substr(line.find("=") + 1, line.length()) == "on");
						break;
					}
					default :
						return (false);
				}
			}
			return (true);
		}
	};
}

#endif
