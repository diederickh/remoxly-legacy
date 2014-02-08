#include <stdio.h>
#include <sstream>
#include <gui/remote/Serializer.h>
#include <gui/remote/Types.h>
#include <gui/remote/Server.h>

// -----------------------------------------------------------

int remoxly_server_websocket(struct libwebsocket_context* ctx,
                             struct libwebsocket* ws,
                             enum libwebsocket_callback_reasons reason,
                             void* user,
                             void* in, 
                             size_t len)
{

  int r = 0;
  Server* server = static_cast<Server*>(libwebsocket_context_user(ctx));

  switch(reason) {

    case LWS_CALLBACK_ESTABLISHED: {
      return server->onCallbackEstablished(ws);
    }

    case LWS_CALLBACK_SERVER_WRITEABLE: {
      return server->onCallbackServerWritable(ws);
    }

    case LWS_CALLBACK_RECEIVE: {
      return server->onCallbackReceive(ws, (char*)in, len);
    }

    case LWS_CALLBACK_CLOSED: {
      return server->onCallbackClosed(ws);
    }

    case LWS_CALLBACK_DEL_POLL_FD: {
      return server->onCallbackDelPollFD(ws);
      break;
    }

    default: {
      printf("Unhandled reason: %s\n", remoxly_websocket_reason_to_string(reason).c_str());
      break;
    }
  }

  return r;
}

// -----------------------------------------------------------

ApplicationData::ApplicationData()
  :ws(0)
  ,app_id(-1)
{
}

// -----------------------------------------------------------

Connection::Connection(struct libwebsocket* ws) 
  :ws(ws)
  ,app_id(-1)
  ,is_app(false)
{
}

Connection::~Connection() {

  printf("~Connection()\n");

  for(size_t i = 0; i < tasks.size(); ++i) {
    delete tasks[i];
  }

  tasks.clear();
}

// -----------------------------------------------------------

Server::Server(int port, bool ssl) 
  :port(port)
  ,use_ssl(ssl)
  ,context(NULL)
{
}

Server::~Server() {
  
  if(context) {
    libwebsocket_context_destroy(context);
    context = NULL;
  }
}

bool Server::start() {

  lws_set_log_level(LLL_DEBUG, NULL);
  
  lws_context_creation_info info;
  memset(&info, 0, sizeof info);

  info.port = port;
  info.protocols = remoxly_server_protocol;
  info.gid = -1;
  info.uid = -1;
  info.user = (void*) this;

#ifndef LWS_NO_EXTENSIONS
  info.extensions = libwebsocket_get_internal_extensions();
#endif

  // create the context handler.
  context = libwebsocket_create_context(&info);

  if(context == NULL) {
    printf("Error: while creating the websocket context.\n");
    return false;
  }

  return true;
}

void Server::update() {

#if !defined(NDEBUG)
  if(!context) {
    printf("Error: not valid context. Cannot update()\n");
    ::exit(EXIT_FAILURE);
  }
#endif

  int n = libwebsocket_service(context, 8);
}

bool Server::getApplicationData(int appID, ApplicationData& result) {

  std::map<int, ApplicationData>::iterator it = applications.find(appID);

  if(it == applications.end()) {
    return false;
  }

  result = it->second;

  return true;
}

void Server::addConnection(struct libwebsocket* ws) {

  Connection* c = new Connection(ws);
  connections.insert(std::pair<struct libwebsocket*, Connection*>(ws, c));
}

void Server::closeConnection(struct libwebsocket* ws) {
  
  std::map<struct libwebsocket*, Connection*>::iterator it = connections.find(ws);

  if(it == connections.end()) {
    printf("Error: trying to remove a connection but it's not found in the connections member (?!).\n");
    return;
  }

  Connection* con = it->second;
  if(con->is_app) {
    closeApplicationConnections(con->app_id);
  }
}

// removes application data + closes all connections that use this application data
void Server::removeApplicationData(int appID) {

  std::map<int, ApplicationData>::iterator it = applications.find(appID);

  if(it == applications.end()) {
    printf("Error: cannot find application data for app id: %d\n", appID);
    return;
  }

  applications.erase(it);

  printf("-- application data removed, new size: %ld\n", applications.size());
}

void Server::removeConnection(struct libwebsocket* ws) {

  std::map<struct libwebsocket*, Connection*>::iterator it = connections.find(ws);

  if(it == connections.end()) {
    printf("Error: cannot find the connection to remove; not supposed to arrive here.\n");
    return;
  }

  Connection* con = it->second;
  
  if(con->is_app) {
    removeApplicationData(con->app_id);
  }

  delete con;
  con = NULL;

  connections.erase(it);
}

// makes sure that all connections that rely on the given application data are closed and cleaned up.
void Server::closeApplicationConnections(int appID) {

  std::map<struct libwebsocket*, Connection*>::iterator it = connections.begin();

  while(it != connections.end()) {

    struct libwebsocket* ws = it->first;
    Connection* con = it->second;
    
    if(con->app_id == appID && !con->is_app) {
      ConnectionTask* task = new ConnectionTask();
      task->task_name = REMOTE_TASK_CLOSE;
      task->task_id = appID;
      con->tasks.push_back(task);
      libwebsocket_callback_on_writable(context, ws);
    }
    ++it;
  }
}

Connection* Server::getConnection(struct libwebsocket* ws) {
  
  std::map<struct libwebsocket*, Connection*>::iterator it = connections.find(ws);

  if(it == connections.end()) {
    return NULL;
  }

  return it->second;
}

int Server::onCallbackEstablished(struct libwebsocket* ws) {
  addConnection(ws);
  return 0;
}

// gets called when we receive a gui model from the given websocket.
int Server::onReceiveSetGuiModel(struct libwebsocket* ws, int appID, char* data, size_t len) {
  
  Connection* c = getConnection(ws);

  if(!c) {
    printf("Cannot find the connection for the socket that gave us the gui model.\n");
    return -1;
  }

  c->is_app = true;
  c->app_id = appID;

  ApplicationData ad;
  ad.app_id = appID;
  ad.ws = ws;
  ad.json_model.assign(data, len);

  applications[appID] = ad;
 
  return 0;
}

void Server::proxyData(int appID, char* data, size_t len) {

  if(!len || !data) {
    printf("Warning: trying to proxy data, but data/len is invalid: %p/%ld\n", data, len);
    return;
  }

  // find all connections for the given appID
 std::map<struct libwebsocket*, Connection*>::iterator it = connections.begin();

 while(it != connections.end()) {
   
   struct libwebsocket* ws = it->first;
   Connection* c = it->second;

   if(c->app_id != appID) {
     ++it;
     continue;
   }

   ConnectionTask* task = new ConnectionTask();
   task->task_name = REMOTE_TASK_PROXY;
   task->task_id = appID;
   task->task_data.assign(data, len);

   c->tasks.push_back(task);

   libwebsocket_callback_on_writable(context, ws);

   ++it;
 }
}

// proxies the given data to all clients that listen for the given app id
int Server::onReceiveValueChanged(struct libwebsocket* ws, int appID, char* data, size_t len) {
  proxyData(appID, data, len);
  return 0;
}

// this will add a task that sends the gui model to the client.
int Server::onReceiveGetGuiModel(struct libwebsocket* ws, int appID, char* data, size_t len) {

  Connection* c = getConnection(ws);

  if(!c) {
    printf("Error: connection not found; but wants to retrieve a gui model.\n");
    return -1;
  }
      
  ApplicationData gd;

  if(!getApplicationData(appID, gd)) {
    printf("Error: cannot find any gui data for the given id: %d\n", appID);
    return -1;
  }

  c->app_id = appID;

  ConnectionTask* con_task = new ConnectionTask();
  con_task->task_name = REMOTE_TASK_SET_GUI_MODEL; 
  con_task->task_data = gd.json_model;
  con_task->task_id = appID; 

  c->tasks.push_back(con_task);

  libwebsocket_callback_on_writable(context, ws);
  return 0;
}

// parses incoming data
int Server::onCallbackReceive(struct libwebsocket* ws, char* data, size_t len) {

  int task = 0;
  int id = 0;
  std::string value;

  if(!deserializer.deserializeTask(data, id, task, value)) {
    return -1;
  }

  switch(task) {

    case REMOTE_TASK_VALUE_CHANGED: {
      return onReceiveValueChanged(ws, id, data, len);
    }

    case REMOTE_TASK_SET_GUI_MODEL: {
      return onReceiveSetGuiModel(ws, id, data, len);
    }

    case REMOTE_TASK_GET_GUI_MODEL: {
      return onReceiveGetGuiModel(ws, id, data, len); 
    }      

    default: {
      printf("Error: unhandled task on server: %d\n", task);
      break;
    }
  }

  return 0;
}

// writes data to the given websocket.
int Server::onCallbackServerWritable(struct libwebsocket* ws) {

  if(!ws) {
    printf("Error: invalid ws ptr.\n");
    return 0;
  }

  Connection* c = getConnection(ws);

  if(!c) {
    printf("Error: cannot find the websocket connection.\n");
    return 0;
  }

  int result = 0;

  for(size_t i = 0; i < c->tasks.size(); ++i) {

    ConnectionTask* task = c->tasks[i];
    
    switch(task->task_name) {

      case REMOTE_TASK_PROXY: 
      case REMOTE_TASK_SET_GUI_MODEL: {
        c->buffer.set(task->task_data); // task data contains a complete task json string
        result = remoxly_websocket_write(ws, c->buffer.ptr(), c->buffer.getDataNumBytes());
        break;
      }

       // We were ask to close the given `ws` socket; returning -1 will do this.
      case REMOTE_TASK_CLOSE: {
        return -1;
      }

      default: {
        printf("Warning: cannot handle task: %d.\n", task->task_name);
        break;
      }
    }

    delete task;
    task = NULL;
  }

  c->tasks.clear();

  return result;
}

int Server::onCallbackClosed(struct libwebsocket* ws) {
  removeConnection(ws);
  return 0;
}

int Server::onCallbackDelPollFD(struct libwebsocket* ws) {
  closeConnection(ws);
  return 0;
}
