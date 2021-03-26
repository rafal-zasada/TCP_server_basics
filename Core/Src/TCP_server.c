/*
 * TCP_server.c
 *
 *  Created on: Mar 24, 2021
 *      Author: Rafal
 */

//#include "lwip.h"
#include "api.h"
#include <string.h>
#include <stdio.h>
#include "usart.h"

extern char GUI_buffer[500];

void StartTCPserverTask(void *argument)
{

	struct netconn *Connection, *NewConnection;
	err_t err, accept_err;

	Connection = netconn_new(NETCONN_TCP);

	if(Connection != NULL)
	{
		err = netconn_bind(Connection, NULL, 80);

		if(err == ERR_OK)
		{
			netconn_listen(Connection);

			while(1)
			{
				accept_err = netconn_accept(Connection, &NewConnection);

				if(accept_err == ERR_OK)
				{
					struct netbuf *inbuf;
					err_t recv_err_data;
					char* buf;
					u16_t buflen;
					// struct fs_file file;

					recv_err_data = netconn_recv(NewConnection, &inbuf); // Read the data from the port, blocking if nothing yet there. We assume the request (the part we care about) is in one netbuf

					if(recv_err_data == ERR_OK)
					{
						netbuf_data(inbuf, (void**)&buf, &buflen); // Get the data pointer and length of the data inside a netbuf.

						// debug start
						// print whole input buffer
						snprintf(GUI_buffer, sizeof(GUI_buffer), "\n\n%s \n\n ", buf);		// buf is not \0 terminated hence need to use buflen for UART transmit !
						HAL_UART_Transmit(&huart3, (unsigned char*)&GUI_buffer , buflen, 200); //

						
						char *ServerResponse = "\n\nHello internet\n\n";

						netconn_write(NewConnection, (signed char*)ServerResponse, strlen(ServerResponse), NETCONN_NOCOPY);
					}

					netconn_close(NewConnection); // Close the connection (server closes in HTTP)
					netbuf_delete(inbuf); // Delete the buffer (netconn_recv gives us ownership, so we have to make sure to deallocate the buffer)

					netconn_delete(NewConnection);		// delete connection
				}
			}
		}



	}




	while(1)
	{

	}
}
