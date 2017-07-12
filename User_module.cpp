#include "User_module.h"

User::User() {
  initialized = false;
  no_user.uniqueId = "";
  no_user.user = NOT_FOUND;
  current_user = no_user;
}

void User::initializeUsers() {
  setup();
}

void User::setup() {
  if (WiFi.status() == WL_CONNECTED) {
    const size_t bufferSize = JSON_ARRAY_SIZE(30) + 30*JSON_OBJECT_SIZE(6) + 3150;
    DynamicJsonBuffer jsonBuffer(bufferSize);

    const char* json = fetchUsers();
    //const char* json_test = "[{\"_id\":\"591f25fad7fd3300118d3e38\",\"serialnumber\":\"7C:97:51:CB\",\"slackusername\":\"joelmheim\",\"email\":\"joe@statoil.com\",\"mobile\":\"99165627\",\"name\":\"Jørn Ølmheim\"},{\"_id\":\"592459d67cc65600116f045f\",\"name\":\"Marita Midthaug\",\"email\":\"marmid@statoil.com\",\"mobile\":\"99160869\",\"serialnumber\":\"6C:D9:55:CB \",\"slackusername\":\"marmid\"},{\"_id\":\"5925e503683b430011f7523e\",\"name\":\"Asbjørn Aleksander Fellinghaug\",\"email\":\"afel@statoil.com\",\"mobile\":\"\",\"serialnumber\":\"13:01:CF:8D\",\"slackusername\":\"afel\"},{\"_id\":\"5925e503683b430011f7523f\",\"name\":\"Terje Barstad Olsen\",\"email\":\"\",\"mobile\":\"\",\"serialnumber\":\"DD:AF:E4:9B\",\"slackusername\":\"teryeah\"},{\"_id\":\"5925e503683b430011f75240\",\"name\":\"Mats Grønning Andersen\",\"email\":\"mgand@statoil.com\",\"mobile\":\"\",\"serialnumber\":\"BB:26:68:54\",\"slackusername\":\"mgand\"},{\"_id\":\"5925e503683b430011f75241\",\"name\":\"Eivind\",\"email\":\"\",\"mobile\":\"\",\"serialnumber\":\"CB:7E:FE:4C\",\"slackusername\":\"ebsm\"},{\"_id\":\"5925e503683b430011f75242\",\"name\":\"Torgeir Myhre\",\"email\":\"tmyh@statoil.com\",\"mobile\":\"\",\"serialnumber\":\"CB:2C:FF:4C\",\"slackusername\":\"tmyh\"},{\"_id\":\"5925e503683b430011f75243\",\"name\":\"Stein Arne Sivertsen\",\"email\":\"\",\"mobile\":\"\",\"serialnumber\":\"DB:BE:62:54\",\"slackusername\":\"steinsiv\"},{\"_id\":\"5925e503683b430011f75244\",\"name\":\"Hallgeir Hollien\",\"email\":\"\",\"mobile\":\"\",\"serialnumber\":\"DD:19:C6:73\",\"slackusername\":\"hallgeirh\"},{\"_id\":\"5925e503683b430011f75245\",\"name\":\"Øyvind Rønne\",\"email\":\"oyron@statoil.com\",\"mobile\":\"\",\"serialnumber\":\"3C:84:A8:E7\",\"slackusername\":\"oyron\"},{\"_id\":\"5925e503683b430011f75246\",\"name\":\"Atle Haugan\",\"email\":\"atleh@statoil.com\",\"mobile\":\"\",\"serialnumber\":\"53:E1:27:CD\",\"slackusername\":\"atleh\"},{\"_id\":\"5925e503683b430011f75247\",\"name\":\"Kjell Einar Mikkelsen\",\"email\":\"kem@statoil.com\",\"mobile\":\"\",\"serialnumber\":\"D3:DF:03:87\",\"slackusername\":\"kem\"},{\"_id\":\"5925e503683b430011f75248\",\"name\":\"Jørn Hegstad\",\"email\":\"jorhe@statoil.com\",\"mobile\":\"\",\"serialnumber\":\"3B:A5:5E:54\",\"slackusername\":\"jorhe\"},{\"_id\":\"5925e503683b430011f75249\",\"name\":\"Håkon J. H. Heuch\",\"email\":\"hheu@statoil.com\",\"mobile\":\"\",\"serialnumber\":\"04:50:E4:12:FF:38:80\",\"slackusername\":\"hheu\"},{\"_id\":\"5925e503683b430011f7524a\",\"name\":\"Rune Kvamme\",\"email\":\"\",\"mobile\":\"\",\"serialnumber\":\"4C:7F:56:CB\",\"slackusername\":\"rune\"},{\"_id\":\"5925e503683b430011f7524b\",\"name\":\"Ove Petter Gansmo\",\"email\":\"\",\"mobile\":\"\",\"serialnumber\":\"8D:70:E3:9B\",\"slackusername\":\"opg\"},{\"_id\":\"5925e503683b430011f7524c\",\"name\":\"Thor Andre Aaresvik\",\"email\":\"thaar@statoil.com\",\"mobile\":\"\",\"serialnumber\":\"6C:A7:A9:E7\",\"slackusername\":\"thaar\"},{\"_id\":\"5925e503683b430011f7524d\",\"name\":\"Jean-Paul Balabanian\",\"email\":\"jpb@statoil.com\",\"mobile\":\"\",\"serialnumber\":\"A3:36:FD:86\",\"slackusername\":\"jpb\"},{\"_id\":\"5925e503683b430011f7524e\",\"name\":\"Thorvald Johannesen\",\"email\":\"\",\"mobile\":\"\",\"serialnumber\":\"9B:61:51:4D\",\"slackusername\":\"thorvald\"},{\"_id\":\"5925e503683b430011f7524f\",\"name\":\"Pål Grønnås Drange\",\"email\":\"pgdr@statoil.com\",\"mobile\":\"\",\"serialnumber\":\"FB:B9:FF:4C\",\"slackusername\":\"pgdr\"},{\"_id\":\"5925e503683b430011f75250\",\"name\":\"Jørgen Kvalsvik\",\"email\":\"jokva@statoil.com\",\"mobile\":\"\",\"serialnumber\":\"9B:D3:FF:4C\",\"slackusername\":\"jokva\"},{\"_id\":\"5925e503683b430011f75251\",\"name\":\"Thomas Johansen\",\"email\":\"\",\"mobile\":\"\",\"serialnumber\":\"CE:6A:B0:AA\",\"slackusername\":\"tejodd\"},{\"_id\":\"5925e503683b430011f75252\",\"name\":\"Andreas Rossland\",\"email\":\"\",\"mobile\":\"\",\"serialnumber\":\"OE:9B:DE:AE\",\"slackusername\":\"andreas.rossland\"}]";

    JsonArray& root = jsonBuffer.parseArray(json);

    if (!root.success()) {
      Serial.println(F("parseObject() failed"));
      initialized = false;
    } else {
      resetUserArray();
      Serial.println(F("Users:"));
      for (int i = 0; i < root.size(); i++) {
        String slackusername = root[i]["slackusername"].as<String>();
        String serialnumber = root[i]["serialnumber"].as<String>();
        Serial.print(F("User "));
        Serial.print(i);
        Serial.print(F(" added: "));     
        Serial.print(slackusername);
        Serial.print(F(", "));
        Serial.println(serialnumber);
        add(serialnumber, slackusername);
      }
      initialized = true;
    }
  } 
  //else {
    //t.stop(userEvent);
    //userEvent = t.every(60 * 1000, updateUserSetup, (void*)3);
    //Serial.println(F("Fallback to local user management"));
    //addUser("7C:97:51:CB", "joelmheim"); // Me
    //addUser("13:01:CF:8D", "afel"); //Asbjørn 
    //addUser("DD:AF:E4:9B", "teryeah"); // Terje
    //addUser("BB:26:68:54", "mgand"); //Mats
    //addUser("CB:7E:FE:4C", "ebsm"); //Eivind
    //addUser("CB:2C:FF:4C", "tmyh"); //Torgeir 
    //addUser("DB:BE:62:54", "steinsiv"); //Stein Arne 
    //addUser("DD:19:C6:73", "hallgeirh"); //Hallgeir 
    //addUser("3C:84:A8:E7", "oyron"); //Øyvind 
    //addUser("53:E1:27:CD", "atleh"); //Atle 
    //addUser("6C:D9:55:CB", "marmid"); //Marita 
    //addUser("D3:DF:03:87", "kem"); //Kjell Einar 
    //addUser("3B:A5:5E:54", "jorhe"); //Jørn H
    //addUser("04:50:E4:12:FF:38:80", "hheu"); // Håkon J.H. Heuch 
    //addUser("4C:7F:56:CB", "rune"); // Rune Kvamme
    //addUser("8D:70:E3:9B", "opg"); // Ove Petter Gansmo
    //addUser("6C:A7:A9:E7", "thaar"); //Thor Andre 
    //addUser("A3:36:FD:86", "jpb"); //Jean-Paul
    //addUser("9B:61:51:4D", "thorvald"); // Thorvald
    //addUser("FB:B9:FF:4C", "pgdr"); // Pål G
    //addUser("9B:D3:FF:4C", "jokva"); // Jørgen
    //addUser("CE:6A:B0:AA", "tejodd"); // Thomas Johansen
    //addUser("OE:9B:DE:AE", "andreas.rossland"); // Andreas Rossland
  //}
  if (numUsers > 1) {
    Serial.println(F("Ready!"));
    initialized = true;
  } else {
    Serial.println(F("User setup failed"));
    initialized = false;
  }
}

boolean User::checkCard(String uidString) {
  if (initialized) {
    boolean match = getIndex(uidString) >= 0;
    Serial.print(F("Match: "));
    Serial.println(match);
    Serial.print(F("Index: "));
    Serial.println(getIndex(uidString));
    Serial.print(F("Username: "));
    Serial.println(findById(uidString).user);
    
    if(match) {
      current_user = findById(uidString);
      Serial.println(F("\nI know this card!"));
    } else {
      Serial.println(F("\nUnknown Card"));
    }
    return match;
  } else {
    Serial.println(F("Users not set up."));
    return false;
  }
}

String User::currentUser() {
  return current_user.user;
}

boolean User::activeUser() {
  return (current_user.user != no_user.user);
}

void User::reset() {
  Serial.println("Current user reset.");
  current_user = no_user;
}

void User::add(String uid, String user) {
  registered_user newUser;
  newUser.uniqueId = uid;
  newUser.user = user;
  users[numUsers] = newUser;
  numUsers++;
}

int User::getIndex(String uid) {
  for (int i=0; i < MAX_USERS; i++) {
    if (users[i].uniqueId == uid) {
      return i;
    }
  }
  return -1;  
}

boolean User::registered(String uid) {
  getIndex(uid) >= 0;
}

registered_user User::findById(String uid) {
  int idx = getIndex(uid);
  if (idx >= 0) {
    return users[idx];
  } else {
    registered_user none;
    none.uniqueId = "";
    none.user = NOT_FOUND;
    return none;
  }
}

const char* User::fetchUsers() {
  Serial.println(F("Fetching users from SlackbotConfig service."));
  RestClient client = RestClient(config_host);
  
  // We now create a URI for the request
  Serial.print(F("Requesting URL: "));
  Serial.println(config_url);

  String response = "";
  int statusCode = client.get(config_url, &response);

  Serial.print(F("Status: "));
  Serial.println(statusCode);
  
  Serial.print(F("Response: "));
  Serial.println(response);

  return response.c_str();
}

void User::resetUserArray() {
  memset(users, 0, sizeof(users));
  numUsers = 0;  
}


