/*
 * PROKEY HARDWARE WALLET
 * Copyright (C) Prokey.io

 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "wsServer/include/ws.h"

static int last_client_id = 0;
static unsigned char ws_msg[1024];
static size_t ws_msg_size = 0;

/**
 * @brief This function is called whenever a new connection is opened.
 * @param fd The new client file descriptor.
 */
void onopen(int fd)
{
    char *cli;
    cli = ws_getaddress(fd);
    printf("Connection opened, client: %d | addr: %s\n", fd, cli);
    free(cli);
}

/**
 * @brief This function is called whenever a connection is closed.
 * @param fd The client file descriptor.
 */
void onclose(int fd)
{
    if (last_client_id == fd)
        last_client_id = 0;

    char *cli;
    cli = ws_getaddress(fd);
    printf("Connection closed, client: %d | addr: %s\n", fd, cli);
    free(cli);
}

/**
 * @brief Message events goes here.
 * @param fd   Client file descriptor.
 * @param msg  Message content.
 * @param size Message size.
 * @param type Message type.
 */
void onmessage(int fd, const unsigned char *msg, size_t size, int type)
{
    char *cli;
    cli = ws_getaddress(fd);
    printf("I receive a message: %s (%zu), from: %s/%d\n", msg,
        size, cli, fd);

    last_client_id = fd;
    ws_msg_size = size;
    memcpy(ws_msg, msg, size);

    free(cli);
}

static void* WebSocketThread(void* _user_data)
{
    /* Register events. */
    struct ws_events evs;
    evs.onopen    = &onopen;
    evs.onclose   = &onclose;
    evs.onmessage = &onmessage;

    /* Main loop, this function never returns. */
    ws_socket(&evs, 55500);    
}

void emulatorWebSocketInit(void)
{
    pthread_t thread_id;
    pthread_create(&thread_id, NULL, WebSocketThread, NULL);
}

size_t emulatorWebSocketRead(int *iface, void *buffer, size_t size)
{
    *iface = 0;
    if (ws_msg_size > 0)
    {
        memcpy(buffer, ws_msg, ws_msg_size);
        size_t t = ws_msg_size;
        ws_msg_size = 0;
        return t;        
    }
    return 0;
}

size_t emulatorWebSocketWrite(int iface, const void *buffer, size_t size)
{
    if (last_client_id > 0)
    {
        return ws_sendframe_bin(last_client_id, buffer, size, false);
    }
    return 0;
}
