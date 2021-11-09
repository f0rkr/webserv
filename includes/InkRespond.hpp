/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ink_respond.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By:  <mashad@student.1337.ma>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/22 09:56:30 by                   #+#    #+#             */
/*   Updated: 2021/09/26 14:26:01 by                  ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INKRESPOND_HPP
# define INKRESPOND_HPP

// Define Successful responses codes
# define SUCCESS_OK								200 /* The request has succeeded. The meaning of the success depends on the HTTP method. */
# define SUCCESS_CREATED						201 /* The request has succeeded and a new resource has been created as a result. This is typically the response sent after POST requests, or some PUT requests. */
# define SUCCESS_ACCEPTED						202 /* The request has been received but not yet acted upon. It is noncommittal, since there is no way in HTTP to later send an asynchronous response indicating the outcome of the request. It is intended for cases where another process or server handles the request, or for batch processing. */
# define SUCCESS_NON_AUTHORITATIVE_INFORMATION	203 /* This response code means the returned meta-information is not exactly the same as is available from the origin server, but is collected from a local or a third-party copy. This is mostly used for mirrors or backups of another resource. Except for that specific case, the "200 OK" response is preferred to this status. */
# define SUCCESS_NO_CONTENT						204 /* There is no content to send for this request, but the headers may be useful. The user-agent may update its cached headers for this resource with the new ones. */
# define SUCCESS_RESET_CONTENT					205 /* Tells the user-agent to reset the document which sent this request. */
// Define Status Error Codes
# define REDIRECTION_MOVED_PERMANENTLY			301 /* The URL of the requested resource has been changed permanently. The new URL is given in the response. */
# define REDIRECTION_TEMPORARY_REDIRECT			307 /* The server sends this response to direct the client to get the requested resource at another URI with same method that was used in the prior request. This has the same semantics as the 302 Found HTTP response code, with the exception that the user agent must not change the HTTP method used: If a POST was used in the first request, a POST must be used in the second request. */
# define REDIRECTION_PERMANENT_REDIRECT			308 /* This means that the resource is now permanently located at another URI, specified by the Location: HTTP Response header. This has the same semantics as the 301 Moved Permanently HTTP response code, with the exception that the user agent must not change the HTTP method used: If a POST was used in the first request, a POST must be used in the second request. */
# define CLIENT_BAD_REQUEST						400 /* The server could not understand the request due to invalid syntax. */
# define CLIENT_UNAUTHORIZED					401 /* Although the HTTP standard specifies "unauthorized", semantically this response means "unauthenticated". That is, the client must authenticate itself to get the requested response. */
# define CLIENT_FORBIDDEN						403 /* The client does not have access rights to the content; that is, it is unauthorized, so the server is refusing to give the requested resource. Unlike 401, the client's identity is known to the server. */
# define CLIENT_NOT_FOUND						404 /* The server can not find the requested resource. In the browser, this means the URL is not recognized. In an API, this can also mean that the endpoint is valid but the resource itself does not exist. Servers may also send this response instead of 403 to hide the existence of a resource from an unauthorized client. This response code is probably the most famous one due to its frequent occurrence on the web. */
# define CLIENT_METHOD_NOT_ALLOWED				405 /* The request method is known by the server but is not supported by the target resource. For example, an API may forbid DELETE-ing a resource. */
# define CLIENT_REQUEST_TIMEOUT					408 /* This response is sent on an idle connection by some servers, even without any previous request by the client. It means that the server would like to shut down this unused connection. This response is used much more since some browsers, like Chrome, Firefox 27+, or IE9, use HTTP pre-connection mechanisms to speed up surfing. Also note that some servers merely shut down the connection without sending this message. */
# define CLIENT_TOO_MANY_REQUESTS				429 /* The user has sent too many requests in a given amount of time ("rate limiting"). */
# define SERVER_INTERNAL_SERVER_ERROR   		500 /* The server has encountered a situation it doesn't know how to handle. */
# define SERVER_NOT_IMPLEMENTED         		501 /* The request method is not supported by the server and cannot be handled. The only methods that servers are required to support (and therefore that must not return this code) are GET and HEAD. */
# define SERVER_BAD_GATEWAY             		502 /* This error response means that the server, while working as a gateway to get a response needed to handle the request, got an invalid response. */
# define SERVER_UNAVAILABLE						503 /* The server is not ready to handle the request. Common causes are a server that is down for maintenance or that is overloaded. */
# define SERVER_GATEWAY_TIMEOUT         		504 /* This error response is given when the server is acting as a gateway and cannot get a response in time. */
# define SERVER_HTTP_VERSION_NOT_SUPPORTED		505 /* The HTTP version used in the request is not supported by the server. */

class InkRespond {
	private:

	public:
};

#endif