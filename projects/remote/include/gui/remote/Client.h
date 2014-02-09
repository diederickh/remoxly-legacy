/*

  Client
  ------

  This Client class is used to connect to the Server. It uses websockets to 
  communicate information about the GUI. The Client can be used in two situations.

  A) You create your own GUI and add the Panels and Groups to a Client
     object. Then you connect to a server. When used in this manner the 
     Client will sent a description of the GUI to the server, and makes sure
     that all changes are sent/received serialized/deserialized correctly.
   

  B) You don't create the GUI (Panels/Groups) yourself but connect to
     a server and request the information that you need to rebuild the
     GUI yourself. When connected to the server the server sends the
     GUI model and you use a generator to create all the widgets. 


 */
#ifndef REMOXLY_GUI_REMOTE_CLIENT_H
#define REMOXLY_GUI_REMOTE_CLIENT_H

#include <string>
#include <vector>
#include <map>
#include <gui/Remoxly.h>
#include <gui/remote/Utils.h>
#include <gui/remote/Buffer.h>
#include <gui/remote/Types.h>
#include <gui/remote/Serializer.h>
#include <gui/remote/Deserializer.h>
#include <gui/remote/ClientListener.h>
#include <gui/WidgetListener.h>
#include <stdint.h>

extern "C" {
#  include <libwebsockets.h>
#  include <jansson.h>
}

// -----------------------------------------------------------

int remoxly_client_websocket(struct libwebsocket_context* ctx, 
                             struct libwebsocket* ws,
                             enum libwebsocket_callback_reasons reason,
                             void* user,
                             void* in,
                             size_t len);

// -----------------------------------------------------------

static struct libwebsocket_protocols remoxly_client_protocol[] = { 
  { "remoxly", remoxly_client_websocket,  0, 128 },
  { NULL, NULL, 0, 0, }
};

// -----------------------------------------------------------

class Client : public WidgetListener {

 public:
  Client(std::string host, int port, bool ssl = false, ClientListener* listener = NULL);
  ~Client();

  /* websocket */
  bool connect();                                 /* connect to the server and sends the gui model for the added Panels/Groups. when no Panels/Groups have been added we still connect and process the messages from the server */
  void update();                                  /* call this regurlarly to process incoming data */
  void shutdown();                                /* shutsdown the connection and clears up all allocated memory */
  bool isApplication();                           /* a client can be used in two situations. one, a client is used to send a gui model to the server. in this case the user creates and adds the panels and groups to the client. On the other side a client can be used to retrieve a gui model */
  bool isConnected();                             /* returns true when the client is connected */
  void onDisconnected();                          /* is called when we get disconnected; is also passed on to the listener */
                        
  /* used to serialize + events */
  void addPanel(Panel* panel);                    /* adds a panel to the serializer, we will also start listening to these panels and their children. when values change we make sure they are sent to the server. when a panel is added we use the term "application" */
  void addGroup(Group* group);                    /* adds a gruop to the serializer, we will also start listening to these groups and their children. when values change we make sure they are sent to the server. when a group is added we use the term "application" */

  /* processing tasks */
  bool addTask(int taskID, int appID, std::string value = "");            /* add a task to the send queue (*/
  bool sendTask(ConnectionTask* task);                                    /* send a specific task, used internally */
  bool onTaskValueChanged(char* data, size_t len, std::string value);     /* gets called when we receive a message from the server that a value has changed */
  bool onTaskGetValues(char* data, size_t len, std::string value);        /* gets called when a client wants to update all of it's values for the gui */
  bool onTaskSetValues(char* data, size_t len, std::string value);        /* gets called when a client (which is not the application) wants to update the values */

  /* websocket callbacks */
  int onCallbackClientWritable();                /* gets called from the websocket callback when necessary; do not call this your self */
  int onCallbackReceive(char* data, size_t len); /* gets called whenever there is data to be processed from the server */ 
  bool createSetGuiModelTask();                  /* when possible this will create the SET gui task (in this case the client is used as an application) */
  bool createGetGuiModelTask();                  /* when possible this will create a GET gui task. */
  bool createGetValuesTask();
  bool createSetValuesTask();

  /* listener implementation */
  void onEvent(int event, Widget* w);             /* gets called whenever a value of one of the created/added widgets notifies us about an event */

 private:
  bool createContext();                          /* creates the libwebsocket context */
  bool createConnection();                       /* tries to connect to the server */
  void setWidgets(Panel* panel);                 /* extracts widgets from the Panel, and stores them in our `widgets` maps. This is used to set the values we receive from the server */
  void setWidgets(Group* group);                 /* extracts widgets from te Group,  "" ""  "" ... */
                                                 
 public: 
                                        
  /* connection info */                          
  std::string host;                               /* address of the server */
  int port;                                       /* port of the server */
  bool use_ssl;                                   /* use SSL */
                                                 
  /* websocket */                    
  uint64_t reconnect_timeout;                     /* when we reach this timeout we will reconnect after being disconnected  */
  uint64_t reconnect_delay;                       /* try to reconnect every `reconnect_delay` millis */
  bool auto_reconnect;                            /* by default this is set to true; it will make sure that the client tries to reconnect when disconnected */
  int state;                                      /* the current state of the Client. used to keep track if we're connected or not */
  lws_context_creation_info info;                 /* used to create a libwebsocket context */
  libwebsocket_context* context;                  /* the libwebsocket context object */
  struct libwebsocket* ws;                        /* this represents the connection to the websocket server */
                                                 
  /* communication with server */                
  Buffer buffer;                                  /* we use Buffer object to manage the memory that we send to the server */
  Serializer serializer;                          /* the serializer is used to serialize the gui model and event data */
  Deserializer deserializer;                      /* used to deserialize the values we get from the server */
  std::vector<ConnectionTask*> tasks;             /* all the tasks that we want to deliver to the server */

  bool is_application;                            /* is set to true, when a client adds panels and/or groups to this object. */
  ClientListener* listener;                       /* listener that can be used to handle certain client events, see the ClientListener interface */
  std::map<int, Widget*> widgets;                 /* the widgets that are added with addPanel/addGroup, used to set changed values */
};

#endif
