# ink-webserv
<img height="550px" width="2000px" src="https://www.wallpaperflare.com/static/288/670/468/whispers-of-the-old-gods-hearthstone-n-zoth-monster-wallpaper.jpg" />
Implementing HTTP(RFCs 7230 to 7235) protocol from scratch using C++ 

# Introduction
  <b>The Hypertext Transfer Protocol (HTTP)</b> is an application protocol for distributed, collaborative, hypermedia information systems.
HTTP is the foundation of data communication for the World Wide Web, where hypertext documents include hyperlinks to other resources that the user can easily access, for example by a mouse click or by tapping the screen in a web browser.

  HTTP was developed to facilitate hypertext and the World Wide Web.
The primary function of a web server is to store, process, and deliver web pages to clients.
The communication between client and server takes place using the Hypertext Transfer
Protocol (HTTP).

  Pages delivered are most frequently HTML documents, which may include images,
style sheets, and scripts in addition to the text content. Multiple web servers may be used for a high-traffic website.

  <b>A user agent</b>, commonly a web browser or web crawler, initiates communication by
requesting a specific resource using HTTP and the server responds with the content of
that resource or an error message if unable to do so. The resource is typically a real file
on the server’s secondary storage, but this is not necessarily the case and depends on how
the webserver is implemented.
  While the primary function is to serve content, full implementation of HTTP also includes ways of receiving content from clients. This feature is used for submitting web
forms, including the uploading of files.

# InkWe
  Inkwe is an implementation of HTTP(RFCs 7230 to 7235), with multiple web servers using polling system to manage all RAQs. Hence it's derived from the octopus god that has many tails.
