/* socket.c: Simple Socket Functions */


#include "spidey.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

/**
 * Allocate socket, bind it, and listen to specified port.
 *
 * @param   port        Port number to bind to and listen on.
 * @return  Allocated server socket file descriptor.
 **/
int socket_listen(const char *port) {
    /* Lookup server address information */
    struct addrinfo *results;
    struct addrinfo hints = {
        .ai_family = AF_UNSPEC,
        .ai_socktype = SOCK_STREAM,
        .ai_flags = AI_PASSIVE
    };

    int status = getaddrinfo(NULL, port, &hints, &results);
    if (status != 0) {
        fprintf(stderr, "Error with getaddrinfo: %s\n", strerror(errno));
        return -1;
    }

    /* For each server entry, allocate socket and try to connect */
    int socket_fd = -1;
    for (struct addrinfo *p = results; p != NULL && socket_fd < 0; p = p->ai_next) {
	/* Allocate socket */
        socket_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (socket_fd < 0) {
            fprintf(stderr, "Error with socket: %s\n", strerror(errno));
            continue;
        }
	/* Bind socket */
        if (bind(socket_fd, p->ai_addr, p->ai_addrlen) < 0) {
            fprintf(stderr, "Error with binding: %s\n", strerror(errno));
            close(socket_fd);
            socket_fd = -1;
            continue;
        }

    	/* Listen to socket */
        if (listen(socket_fd, SOMAXCONN) < 0) {
            fprintf(stderr, "Error with listening: %s\n", strerror(errno));
            close(socket_fd);
            socket_fd = -1;
            continue;
        }
    }

    freeaddrinfo(results);
    return socket_fd;
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
