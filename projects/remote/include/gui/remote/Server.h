/*
  
  Server
  -------

  This creates a websocket server that is used to communicate messages
  for GUIs. It will keep track of GUI models for applications (stored
  in the applications member).  The server proxies all data to from a 
  client for a specific application to the correct other clients which 
  are also listening for changes from this application.

 */
#ifndef REMOXLY_GUI_REMOTE_SERVER_H
#define REMOXLY_GUI_REMOTE_SERVER_H

#include <string> 
#include <map>
#include <gui/remote/Utils.h>
#include <gui/remote/Buffer.h>
#include <gui/remote/Deserializer.h>

extern "C" {
#  include <libwebsockets.h>
#  include <jansson.h>
}

// -----------------------------------------------------------

class Server;

// -----------------------------------------------------------

int remoxly_server_websocket(struct libwebsocket_context* ctx,
                             struct libwebsocket* ws,
                             enum libwebsocket_callback_reasons reason,
                             void* user,
                             void* in, 
                             size_t len);


// -----------------------------------------------------------

class Connection {
 public:
  Connection(struct libwebsocket* ws);
  ~Connection();

 public:
  Buffer buffer;                                                                        /* basic wrapper that we use to send data; this buffer handles the PRE and POST padding necessary for libwebsockets */
  int app_id;                                                                           /* this ID represents the ID on which the connection works; this basically represents an application ID, @todo - maybe we need to change this to app_id */
  bool is_app;                                                                          /* set to true, which this is the connection that gave us the gui model */
  struct libwebsocket* ws;                                                              /* the connection ptr */
  std::vector<ConnectionTask*> tasks;                                                   /* tasks for this specific connections; mostly involves writing to the socket */
};

// -----------------------------------------------------------

static struct libwebsocket_protocols remoxly_server_protocol[] = {
  { "remoxly", remoxly_server_websocket, 0, 128 },
  { NULL, NULL, 0, 0, }
};

// -----------------------------------------------------------

struct ApplicationData {
  ApplicationData();                                                                     /* keeps data for a specific application */

  std::string json_model;                                                                /* the representation of the GUI in JSON. this is given to us by an application */
  struct libwebsocket* ws;                                                               /* this is a reference to the websocket that gave us this model. when it disconnects we will disconnect all clients that make use of this model */
  int app_id;                                                                            /* the ID of this gui data, @todo maybe rename to app_id */
  Connection* connection;
};

// -----------------------------------------------------------

class Server {
 public:
  Server(int port, bool ssl = false);
  ~Server();

  bool start();                                                                            /* start the server; let it accept client connections */
  void update();                                                                           /* call this often; it will process incoming data */

  /* libwebsocket callbacks */
  int onCallbackEstablished(struct libwebsocket* ws);                                      /* gets called when a client has established a connection */
  int onCallbackReceive(struct libwebsocket* ws, char* data, size_t len);                  /* gets called when we receive some data from a client */
  int onCallbackServerWritable(struct libwebsocket* ws);                                   /* gets called when the given socket becomes writable (this is how libwebsocket works, we have to trigger writes). when it becomes writable we will process all the tasks for this connection */
  int onCallbackClosed(struct libwebsocket* ws);                                           /* gets called when the remote connection is closed */
  int onCallbackDelPollFD(struct libwebsocket* ws);                                        /* gets called when libwesocket has removed the socket */

  /* handling of incoming tasks */
  int onReceiveSetGuiModel(struct libwebsocket* ws, int appID, char* data, size_t len);    /* gets called when a client sends us a REMOTE_TASK_SET_GUI_MODEL event */
  int onReceiveGetGuiModel(struct libwebsocket* ws, int appID, char* data, size_t len);    /* gets called when a client sends us a REMOTE_TASK_GET_GUI_MODEL event */
  int onReceiveValueChanged(struct libwebsocket* ws, int appID, char* data, size_t len);   /* gets called when a client sends us a REMOTE_TASK_VALUE_CHANGED event */
  int onReceiveGetValues(struct libwebsocket* ws, int appID, char* data, size_t len);      /* gets called when a client sends us a REMOTE_TASK_GET_VALUES event. */
  int onReceiveSetValues(struct libwebsocket* ws, int appID, char* data, size_t len);      /* gets called when a client sends us a REMOTE_TASK_SET_VALUES event. */
  void proxyData(int appID, char* data, size_t len);                                       /* proxy the given data to the clients for the given "appID". */

  /* connection management */
  void addConnection(struct libwebsocket* ws);                                             /* add a new connection, is used to keep state/data for all connections */
  void closeConnection(struct libwebsocket* ws);                                           /* remove the given connection and cleanup all related data */
  void removeConnection(struct libwebsocket* ws);                                          /* closing and removing a connection is a two step process with libwebsocket; first we close the connection, then we remove it */
  Connection* getConnection(struct libwebsocket* ws);                                      /* get connection state info based on the given websocket connection */
  Connection* getApplicationConnection(int appID);

  /* application data (gui models) */
  void removeApplicationData(int appID);                                                   /* removes the gui data and all clients which are listening for information about this gui; when this is called it means that the application has been closed */
  void closeApplicationConnections(int appID);                                             /* closes all the connections for the given application ID */
  bool getApplicationData(int appID, ApplicationData& result);                             /* get gui information for the give gui model id. GuiData holds information about specific guis */

 public:
  /* connection info */
  int port;                                                                                /* port that clients can connect to */
  bool use_ssl;                                                                            /* use SSL */

  /* websocket */
  libwebsocket_context* context;
  std::map<int, ApplicationData> applications;                                             /* contains the received gui models */
  std::map<struct libwebsocket*, Connection*> connections;                                 /* custom data we keep per connection */

  /* protocol */
  Deserializer deserializer;
};

#endif
