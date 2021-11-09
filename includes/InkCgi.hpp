/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InkCgi.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oel-ouar <oel-ouar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/27 12:15:28 by oel-ouar          #+#    #+#             */
/*   Updated: 2021/09/29 13:45:53 by oel-ouar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <iostream>
# include <stdio.h>
# include <unistd.h>
# include <cstdio>
# include <sys/wait.h>
# include "InkRequest.hpp"
# include "InkServerConfig.hpp"

class InkCgi {
    private :
       const char **env;
    public :
        InkCgi(request req, INKLOCATION location, INKSERVERCONFIG serverConf) {
            env = (char **)malloc(sizeof(char *) * 17);
            for (int i = 0; i < 17; i++)
                env[i] = (char*)malloc(sizeof(char) * 1024);
            init_env(req, location, serverConf);
        }   
        ~InkCgi() {
            delete [] env;
        }
        void init_env(request req, INKLOCATION location, INKSERVERCONFIG serverConf) {
            env[0] = "GETEWAY_INTERFACE=CGI/1.1";
            env[1] = "SERVER_NAME=";
            env[2] = "SERVER_SOFTWARE=InkServ/2.0";
            env[3] = "SERVER_PROTOCOL=" + req.getServerProtocol();
            env[4] = "SERVER_PORT=" + serverConf.getPort();
            env[5] = "REQUEST_METHOD=" + req.getMethod();
            env[6] = "PATH_INFO=" + req.getPath();
            env[7] = "PATH_TRANSLATED=" + req.getPath();
            env[8] = "SCRIPT_NAME=" + req.getPath();
            env[9] = "DOCUMENT_ROOT=" + location.getRoot();
            env[10] = "QUERY_STRING=" + req.getQuery();
            env[11] = "REMOTE_HOST=";
            env[12] = "AUTH_TYPE=";
            env[13] = "CONTENT_TYPE=";
            env[14] = "CONTENT_LENGTH=" + serverConf.getBodySizeLimit();
            env[15] = "HTTP_FROM=";
            env[16] = "HTTP_ACCEPT=";
        }
        void    execCgi()
        {
            size_t pid;
            FILE *inFile = tmpfile();
            FILE *outFile = tmpfile();
            int in = fileno(inFile);
            int out = fileno(outFile);
            
            pid = fork();
            if (pid == -1)
            {
                std::cout << "fork failed!" << std::endl;
                // need to change status to 500 HERE!!
            }
            if (pid == 0)
            {
                dup2(in, 0);
                dup2(out, 1);
                execve(env[8], , env);
                std::cout << "execve failed!" << std::endl;
                // need to change status HERE!
            }
            else
            {
                fclose(inFile);
                fclose(outFile);
                waitpid(-1, NULL, 0);
            }
        }
};
