# Traceroute

## Summary
Command that uses ICMP packets to record the route through the internet from one computer to another. By seting ttl from 1 onwards packets are send back by routers on the path. Command reports which routers send packets back recreating the route.

## How to use
Run ``make``, this compile and link files, and creates executable. 
As an argument pass IP adress you want to check route to. ``sudo ./traceroute x.x.x.x``.
Program requires admin privilages to run (as it uses row sockets to recive ICMP packets).
Program doesn't work on windows, but can be run on wsl.

