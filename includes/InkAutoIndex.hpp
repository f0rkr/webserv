/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InkAutoIndex.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oel-ouar <oel-ouar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/04 10:43:02 by mashad            #+#    #+#             */
/*   Updated: 2022/02/15 08:15:31 by                  ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AUTOINDEX_HPP
#define AUTOINDEX_HPP

# include <dirent.h>
# include <unistd.h>
# include <iostream>
# include <vector>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdint.h>
#include <time.h>
#include <istream>

/** @brief Ft namespace
 * Forthy two namespace where all OOP classes
 * are defined.
 */
namespace ft {
	bool		isnewline( char c )  {
		if (c == '\n') {
			return true;
		} else {
			return false;
		}
	}
	/** @brief Auto index class
	 * AutoIndex class help serving BaseHREF files
	 * containing all folders and file for a specific
	 * root path
	 */	
	class AutoIndex {
		/** @brief Define all typedefs that would be
		 * needed to write this class
		 */
		public:
			typedef size_t		size_type;
			std::string		_urlPath;
		/** @brief Private Attributes
		 * Declaring all private attributes that
		 * would be needed
		 */
		private:
			std::string				_rootPath; // Folder root path
			std::string				_baseHref; // autoindex HTML encoded to be served
			std::vector<char *>		_files;
			std::vector<uint8_t>	_types;
			std::vector<uint64_t>	_sizes;
			struct stat				_sb;
			/** @brief File type
			 * Get file type from the dirent d_type code
			 *
			 * @param uint8_t
			 * @return string indicating file type
			 */
			 std::string 	_getFileType( void ) const {
				switch (_sb.st_mode & S_IFMT) {
					case S_IFBLK:  return("block device\n");            break;
					case S_IFCHR:  return("character device\n");        break;
					case S_IFDIR:  return("directory\n");               break;
					case S_IFIFO:  return("FIFO/pipe\n");               break;
					case S_IFLNK:  return("symlink\n");                 break;
					case S_IFREG:  return("regular file\n");             break;
					case S_IFSOCK: return("socket\n");                  break;
					default:       return("unknown?\n");                break;
				}
			 }

			 /** @brief Get space
			  * Return html tabs indentation
			  *
			  * @param none
			  * @return string
			  */
			  std::string 	_getSpace( void ) const {
				  return ("&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;");
			  }

			  /** @brief Owner ship
			   * Get file ownership
			   *
			   * @param _sb
			   * @return string
			   */
			   std::string 	_getOwnerShip(struct stat sb) const {
				   return ("UID=" + std::to_string(sb.st_uid) + " " + "GID=" + std::to_string(sb.st_gid));
			   }

			   /** @brief File size
			    * return file size in bytes
			    *
			    * @param _sb
			    * @return string
			    */
			    std::string 	_getFileSize(struct stat sb) const {
					return (std::to_string(sb.st_size) + " bytes");
				}

				/** @brief get last file access
				 * return last time access
				 *
				 * @param _sb
				 * @return string
				 */
				 std::string 	_getLastFileAccess(struct stat sb) const {
					 std::string a(ctime(&sb.st_atime));

					 return (a);
				 }
		/** @brief Main section
		 * Main section of this class contains 
		 * all constructos, public methods and destructors
		 */
		public:
			/** @brief Default construtor
			 * Empty default constructor, initializing all private 
			 * attributes with the defaults values
			 *
			 * @param none
			 * @return none
			 */
			AutoIndex( void ) : _files(0), _sizes(0), _types(0){ return ; }


			/** @brief Default constructor
			 * Take root path as parameter and initialize other
			 * private attributes upon it.
			 *
			 * @param std::string, root path folder
			 * @return none
			 */
			explicit AutoIndex( std::string const &urlPath, std::string const &rootPath): _rootPath(rootPath), _urlPath(urlPath) {
				// std::cout << urlPath << std::endl;
				if (_rootPath[_rootPath.length() - 1] == '/')
					_rootPath.erase(_rootPath.length() - 1);
				if (_urlPath[_urlPath.length() - 1] == '/')
					_urlPath.erase(_urlPath.length() - 1);
				_baseHref += "<!DOCTYPE html><html lang='en'><head><meta charset='UTF-8'> <title>AutoIndex</title> </head><body><style>body{font-family: -apple-system, BlinkMacSystemFont, Segoe UI, Roboto, Oxygen, Ubuntu, Cantarell, Fira Sans, Droid Sans, Helvetica Neue, sans-serif; margin: 0;padding: 0;font-size: 25px;}.container {justify-content: center; align-items: center;}.error {display: inline;/* justify-content: center; */ align-items: center;position: absolute;top: 5%;left: 5%;margin-top: -50px;margin-left: -50px;width: 1000px;height: 50px}.indexs {display: inline;/* justify-content: center; */ align-items: center;position: absolute;top: 19%;left: 5%;font-size: 15px;font-family: monospace; width: 1000px;height: 500px;}.indexs th {border: 2px solid black; }.indexs td {border: 2px solid black; }ul {list-style-type: ;}form {display: inline;}h1:hover {font-style: oblique;}h3 {margin-left: 10px;color: #333;}input {background-color: black; color: white;}footer {display: flex;justify-content: center; position: absolute;bottom: 10%;width: 100%;/* Height of the footer*/ height: 40px;}img {-webkit-animation: 3s ease 0s normal forwards 1 fadein; animation: 3s ease 0s normal forwards 1 fadein; }@keyframes fadein {0% {opacity: 0;}66% {opacity: 0.5;}100% {opacity: 1;}}</style><div class='container'> <div class='error'><h1>Index of<h1><h3>" + _rootPath +  "</h3></div><div class='indexs'><table><tr><th>filename</th><th>ownership</th><th>size</th><th>last access</th></tr>";
				return ;
			}


			/** @brief Default destractor
			 * Clear and destory all memory allocations
			 * and private attributes
			 *
			 * @param none
			 * @return none
			 */
			~AutoIndex( void ) { return ; }



			/** @brief Getters section
			 * Define all getter methods	
			 * for the private private attributes
			 *
			 * @param none
			 * @return types of the attribute
			 */
			std::string		getRootPath( void ) const { return (_rootPath); }

			std::string 	getBaseHref( void ) const { return (_baseHref);	}

			/** @brief Setters section
			 * Define all setter methods
			 * to be able to modifie private attributes
			 *
			 * @param types of the attribute
			 * @return none
			 */
			void			setRootPath( std::string const &rootPath ) { _rootPath = rootPath ; return ; }

			/** @brief Read Root Folder
			 * Reading root folder and store its data into
			 * map
			 *
			 * @param none
			 *	 @return none
			 */
			std::string			baseHref( void ) {
				DIR *rootDir;
				struct dirent	*diread;
				std::string		ulrtemp = _urlPath;
				std::string 	urlPath = _urlPath;
				// TO-DO: Open file and read it's content, then store all folders and files
				// into the file's data structure.

				
				// _rootPath.erase(std::remove_if(_rootPath.begin(), _rootPath.end(), isspace), _rootPath.end());
				if ((rootDir = opendir(_rootPath.c_str())) != nullptr) {
					// Looping through all directory and files then store their names
					// in files.
					while ((diread = readdir(rootDir)) != nullptr) {
						std::string dname = std::string(diread->d_name);
						urlPath = ulrtemp;
						// std::cout << urlPath << " " << dname << std::endl;
						if (lstat((_rootPath + "/" + dname).c_str(), &_sb) == -1) {
							perror("lstat");
							return ("HTTP/1.1 500 Internal Server Error\r\n");
						}
						_baseHref += "<tr><td><a href='" + _urlPath + "/" + dname + "'>" + dname + "</a></td>" +
								"<td>" + _getOwnerShip(_sb) + "</td>" +
								"<td>" + _getFileSize(_sb) + "</td>"  +
								"<td>" + _getLastFileAccess(_sb) + "</td></tr>";
					}
					closedir(rootDir);
				} else {
					std::cout << "Error: Open dir failed." << std::endl;
				}
				_baseHref += "</table></div></div><footer><img width=\"100px\" height=\"100px\" src=\"/ink.png\" /> </footer></body></html>";
				return (_baseHref);
			}
	};
}
#endif 
