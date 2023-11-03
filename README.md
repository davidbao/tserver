# tserver
A Lightweight Web Backend Low Code System

What is tserver?
tserver is a lightweight web back-end low code application development program, using tserver for development is a semi-low code development method, that is, the front-end or the traditional development method, the back-end uses tserver as the back-end program. Back-end business processing and back-end connection can be completed without programming or a little programming (using scripting languages such as python).
tserver is suitable for lightweight or small web applications.
tserver supports linux, windows, and MacOS.

Function implementation:
1. As a web server, it provides an Http interface exchange for front-end and back-end data interaction, with built-in national secret encryption authentication;
2, simulation back-end data, this function is mainly used for the front and back end of the coordination;
3. Arrange scheduled tasks for the regular execution of business programs, which can be executable programs, python scripts and database scripts;

How to compile:
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=/usr/local ../