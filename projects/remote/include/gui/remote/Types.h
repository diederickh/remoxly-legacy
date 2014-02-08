#ifndef REMOXLY_GUI_REMOTE_TYPES_H
#define REMOXLY_GUI_REMOTE_TYPES_H

#define REMOTE_TASK_GET_GUI_MODEL 1    /* When a WebGui client connects to the Server it will ask for a Gui model that it can use to generate the webgui. */
#define REMOTE_TASK_SET_GUI_MODEL 2    /* When a client (the gui you get in a GL/DX app, or a webgui) connects it will hand over a model of the gui OR the server will send back a previously stored model and hands it over to e.g. the webgui; the webgui needs to "set" the model and store it/create a gui*/
#define REMOTE_TASK_VALUE_CHANGED 3    /* Whenever a value in a gui changes it should dispatch a json object with this as the task value */
#define REMOTE_TASK_PROXY         4    /* When the proxy task is used, we will send the exact data we receive to the clients; we will not wrap it some other json object, pure proxy */
#define REMOTE_TASK_CLOSE         5    /* libwebsocket is using a "interesting" way to close a socket. the protocol handler needs to return -1 to close a socket. what we do: we add a new task, trigger a write request and if the task is REMOTE_TASK_CLOSE we let the callback return -1 which closes the socket */

#define REMOTE_STATE_NONE           0x0000 
#define REMOTE_STATE_CONNECTING     0x0001
#define REMOTE_STATE_DISCONNECTED   0x0002
#define REMOTE_STATE_CONNECTED      0x0004

#endif
