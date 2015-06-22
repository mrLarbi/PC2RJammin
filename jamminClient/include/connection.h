#ifndef __CONNECTION_H__
#define __CONNECTION_H__

/*Return a socket for the connection, returns 0 if the connection is etablished, -1 if not*/
int connect_to_server(char* name, int port);

/* send a messag msg into the socket sock, returns 0 if the message is sent, -1 if not*/
int send_msg(int sock, char* msg);

int send_audio(int sock, char* buffer, int size);

/* read a message from a socket, return 0 if the message is read, -1 if not.*/
int read_msg(int sock, char* msg, int max_size);


int read_all(int sock,char* msg,int max_size);

/* disconnect from server with the socket sock, returns 0 the deconnection succeed, -1 if not */
int disconnect_from_server(int sock);

#endif /* __CONNECTION_H__ */

