A Simple TCP server Written in C
===============================

It uses a pool of 10 connections to serve multiple requests concurrently that are handed down from the Tornando Webserver. The request will simulate up to 100K iteration of N chosen poker hands and send back a json dictionary of hand results.

A Tornando Webserver Written in Python
===============================

This is a Web server written in the Python using the tornando Framework.

Since HTTP uses TCP I am able to create a TCP connection with the C server. That will receive the poker simulation results in a string representation of a jason dictionary.


Compiling and Using the System
==============================

On a Linux system system simply run these two commands 
gcc server.c Poker.c -o backendServer
python3 PokerWebServer.py

Use the browser and naviagte to http://localhost:8889/
result shown in the example photos.
