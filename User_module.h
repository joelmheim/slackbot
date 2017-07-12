#ifndef _USERS_
#define _USERS_

#include "secrets.h"

#include <RestClient.h>
#include <ArduinoJson.h>
#include <Event.h>
#include <Timer.h>


const int MAX_USERS = 30;
typedef struct registered_user { String uniqueId; String user; } registered_user;

////////////////////////////////////////////////////////////

class User {
public:
  User();
  boolean initialized;
  void initializeUsers();
  void setup();
  boolean checkCard(String uidString);
  String currentUser();
  boolean activeUser();
  void reset();
  const String NOT_FOUND = "NONE";
private:
  registered_user current_user;
  void resetUserArray();
  const char* fetchUsers();
  void add(String uid, String user);
  int getIndex(String uid);
  int numUsers = 0;
  boolean registered(String uid);
  registered_user findById(String uid);
  registered_user users[MAX_USERS];
  registered_user no_user;
};

#endif // _USERS_
