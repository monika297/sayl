CC=g++
FLAGS= -I"sources" #-Wall
SOURCES=sources
BINARIES=binaries

all: Sayl

#
# Simulation
# ----------------------------------------------------------------------------------------------
Sayl : Model Messages Stream Service Network Protocol Services Test
	$(CC) $(BINARIES)/*.o -o sayl

Test: $(SOURCES)/Test.cpp
	$(CC) -o $(BINARIES)/Test.o -c $(SOURCES)/Test.cpp $(FLAGS)


#
# Model
# ----------------------------------------------------------------------------------------------
Model: File FileEntry Peer Directory

File: $(SOURCES)/model/File.cpp $(SOURCES)/model/File.h
	$(CC) -o $(BINARIES)/File.o -c $(SOURCES)/model/File.cpp $(FLAGS)

FileEntry: $(SOURCES)/model/FileEntry.cpp $(SOURCES)/model/FileEntry.h
	$(CC) -o $(BINARIES)/FileEntry.o -c $(SOURCES)/model/FileEntry.cpp $(FLAGS)

Peer: $(SOURCES)/model/Peer.cpp $(SOURCES)/model/Peer.h
	$(CC) -o $(BINARIES)/Peer.o -c $(SOURCES)/model/Peer.cpp $(FLAGS)
Directory: $(SOURCES)/model/Directory.cpp $(SOURCES)/model/Directory.h
	$(CC) -o $(BINARIES)/Directory.o -c $(SOURCES)/model/Directory.cpp $(FLAGS)

#
# Messages
# ----------------------------------------------------------------------------------------------
Messages: Request Response Criterion Comparator

Request: $(SOURCES)/messages/Request.cpp $(SOURCES)/messages/Request.h
	$(CC) -o $(BINARIES)/Request.o -c $(SOURCES)/messages/Request.cpp $(FLAGS)

Response: $(SOURCES)/messages/Response.cpp $(SOURCES)/messages/Response.h
	$(CC) -o $(BINARIES)/Response.o -c $(SOURCES)/messages/Response.cpp $(FLAGS)

Criterion: $(SOURCES)/messages/Criterion.cpp $(SOURCES)/messages/Criterion.h
	$(CC) -o $(BINARIES)/Criterion.o -c $(SOURCES)/messages/Criterion.cpp  $(FLAGS)

Comparator: $(SOURCES)/draft/Comparator.h $(SOURCES)/draft/Comparator.cpp
	$(CC) -o $(BINARIES)/Comparator.o -c $(SOURCES)/draft/Comparator.cpp $(FLAGS)
#
# Stream
# ----------------------------------------------------------------------------------------------
Stream: StreamWriter StreamReader

StreamReader: $(SOURCES)/stream/StreamReader.cpp $(SOURCES)/stream/StreamReader.h
	$(CC) -o $(BINARIES)/StreamReader.o -c $(SOURCES)/stream/StreamReader.cpp $(FLAGS)

StreamWriter: $(SOURCES)/stream/StreamWriter.cpp $(SOURCES)/stream/StreamWriter.h
	$(CC) -o $(BINARIES)/StreamWriter.o -c $(SOURCES)/stream/StreamWriter.cpp $(FLAGS)

#
# Service
# ----------------------------------------------------------------------------------------------
Service: $(SOURCES)/Service.cpp $(SOURCES)/Service.h
	$(CC) -o $(BINARIES)/Service.o -c $(SOURCES)/Service.cpp $(FLAGS)

#
# Network
# ----------------------------------------------------------------------------------------------
Network: Socket ClientSocket ServerSocket ServerConnectionSocket ConnectionSocket

Socket: $(SOURCES)/network/Socket.h $(SOURCES)/network/Socket.cpp
	$(CC) -o $(BINARIES)/Socket.o -c $(SOURCES)/network/Socket.cpp $(FLAGS)

ClientSocket: $(SOURCES)/network/ClientSocket.cpp $(SOURCES)/network/ClientSocket.h
	$(CC) -o $(BINARIES)/ClientSocket.o -c $(SOURCES)/network/ClientSocket.cpp $(FLAGS)

ServerSocket: $(SOURCES)/network/ServerSocket.cpp $(SOURCES)/network/ServerSocket.h
	$(CC) -o $(BINARIES)/ServerSocket.o -c $(SOURCES)/network/ServerSocket.cpp $(FLAGS)

ServerConnectionSocket: $(SOURCES)/network/ServerConnectionSocket.cpp $(SOURCES)/network/ServerConnectionSocket.h 
	$(CC) -o $(BINARIES)/ServerConnectionSocket.o -c $(SOURCES)/network/ServerConnectionSocket.cpp $(FLAGS)
ConnectionSocket:
	$(CC) -o $(BINARIES)/ConnectionSocket.o -c $(SOURCES)/network/ConnectionSocket.cpp $(FLAGS)
#
# Protocol
# ----------------------------------------------------------------------------------------------
Protocol: Server

Server: $(SOURCES)/protocol/Server.h $(SOURCES)/protocol/Server.cpp
	$(CC) -o $(BINARIES)/Server.o -c $(SOURCES)/protocol/Server.cpp $(FLAGS)

#
# Services
# ----------------------------------------------------------------------------------------------
Services: Tracker

Tracker: $(SOURCES)/services/Tracker.cpp $(SOURCES)/services/Tracker.h
	$(CC) -o $(BINARIES)/Tracker.o -c $(SOURCES)/services/Tracker.cpp $(FLAGS)


clean:
	rm $(BINARIES)/*.o

