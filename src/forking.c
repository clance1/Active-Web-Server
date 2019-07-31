/* forking.c: Forking HTTP Server */


#include "spidey.h"

#include <errno.h>
#include <signal.h>
#include <string.h>

#include <unistd.h>

/**
 * Fork incoming HTTP requests to handle the concurrently.
 *
 * @param   sfd         Server socket file descriptor.
 * @return  Exit status of server (EXIT_SUCCESS).
 *
 * The parent should accept a request and then fork off and let the child
 * handle the request.
 **/
int forking_server(int sfd) {
    log("Forking Server");
    /* Accept and handle HTTP request */
    Request *r;
    pid_t pid;
    while (true) {
    	/* Accept request */
        r = accept_request(sfd);
        if (!r)
            continue;

	/* Ignore children */
        signal(SIGCHLD, SIG_IGN);

	/* Fork off child process to handle request */
        pid = fork();

        if (pid < 0) {
            fprintf(stderr, "Error with forking: %s\n", strerror(errno));
            free_request(r);
            exit(EXIT_FAILURE);
        }
        else if (pid == 0) {
            handle_request(r);
            free_request(r);
            close(sfd);
            exit(EXIT_SUCCESS);
        }
        else {
            free_request(r);
        }
    }

    /* Close server socket */
    close(sfd);
    return EXIT_SUCCESS;
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
