#include "FS.h"
#include "SPIFFS.h"
#include <ArduinoJson.h>

static bool DEBUG_CONFIG = false;

//Structure to ESP32 component send data
char* esp32ComponentTypeStr[] = {"TEMPERATURE_COMPONENT", "HUMIDITY_COMPONENT", "SWITCH_COMPONENT"};
enum esp32ComponentType { TEMPERATURE_COMPONENT,
                          HUMIDITY_COMPONENT,
                          SWITCH_COMPONENT,
};


/* You only need to format SPIFFS the first time you run a
   test or else use the SPIFFS plugin to create a partition
   https://github.com/me−no−dev/arduino−esp32fs−plugin */
#define FORMAT_SPIFFS_IF_FAILED true

void listDir(fs::FS &fs, const char *dirname, uint8_t levels) {
  Serial.printf("Listing directory: %s\r\n", dirname);

  File root = fs.open(dirname);
  if (!root) {
    Serial.println("- failed to open directory");
    return;
  }
  if (!root.isDirectory()) {
    Serial.println(" - not a directory");
    return;
  }

  File file = root.openNextFile();
  while (file) {
    if (file.isDirectory()) {
      Serial.print("  DIR : ");
      Serial.println(file.name());
      if (levels) {
        listDir(fs, file.path(), levels - 1);
      }
    } else {
      Serial.print("  FILE: ");
      Serial.print(file.name());
      Serial.print("\tSIZE: ");
      Serial.println(file.size());
    }
    file = root.openNextFile();
  }
}

void readFile(fs::FS &fs, const char *path) {
  Serial.printf("Reading file: %s\r\n", path);

  File file = fs.open(path);
  if (!file || file.isDirectory()) {
    Serial.println("− failed to open file for reading");
    return;
  }

  Serial.println("− read from file:");
  while (file.available()) {
    Serial.write(file.read());
  }
}

void writeFile(fs::FS &fs, const char *path, const char *message) {
  Serial.printf("Writing file: %s\r\n", path);

  File file = fs.open(path, FILE_WRITE);
  if (!file) {
    Serial.println("− failed to open file for writing");
    return;
  }
  if (file.print(message)) {
    Serial.println("− file written");
  } else {
    Serial.println("− frite failed");
  }
}

void appendFile(fs::FS &fs, const char *path, const char *message) {
  Serial.printf("Appending to file: %s\r\n", path);

  File file = fs.open(path, FILE_APPEND);
  if (!file) {
    Serial.println("− failed to open file for appending");
    return;
  }
  if (file.print(message)) {
    Serial.println("− message appended");
  } else {
    Serial.println("− append failed");
  }
}

void renameFile(fs::FS &fs, const char *path1, const char *path2) {
  Serial.printf("Renaming file %s to %s\r\n", path1, path2);
  if (fs.rename(path1, path2)) {
    Serial.println("− file renamed");
  } else {
    Serial.println("− rename failed");
  }
}

void deleteFile(fs::FS &fs, const char *path) {
  Serial.printf("Deleting file: %s\r\n", path);
  if (fs.remove(path)) {
    Serial.println("− file deleted");
  } else {
    Serial.println("− delete failed");
  }
}

void writeFileJSON(fs::FS &fs, const char *path, JsonDocument doc) {
  if (DEBUG_CONFIG) {
    Serial.printf("Writing file: %s\r\n", path);
  }
  File file = fs.open(path, FILE_WRITE);
  if (!file) {
    if (DEBUG_CONFIG) {
      Serial.println("− failed to open file for writing");
      return;
    }
  }
  String payload;
  serializeJson(doc, payload);
  file.println(payload);  
}

JsonDocument readFileJSON(fs::FS &fs, const char *path) {
  if (DEBUG_CONFIG) {
    Serial.printf("Reading file: %s\r\n", path);
  }
  JsonDocument doc;
  File file = fs.open(path);  
  if (!file || file.isDirectory()) {
    if (DEBUG_CONFIG) {
      Serial.println("− failed to open file for reading");
    }
    return doc;
  }
  if (file.available()) {
    String input;
    while (file.available())
    {
      input += (char)file.read();
    }    
    DeserializationError error = deserializeJson(doc, input);
    if (error) {
      if (DEBUG_CONFIG) {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.c_str());
      }
    }
  }
  return doc;
}

JsonDocument readConfig() {
  Serial.begin(115200);
  
  if (!SPIFFS.begin(true)) {
    Serial.println("SPIFFS Mount Failed");
    delay(10000);
    ESP.restart();
  } 
  listDir(SPIFFS, "/", 0);
  Serial.println("SPIFFS Info:");
  Serial.printf("Total Bytes: %u\n", SPIFFS.totalBytes());
  Serial.printf("Used Bytes: %u\n", SPIFFS.usedBytes());
  Serial.printf("Free Bytes: %u\n", SPIFFS.totalBytes() - SPIFFS.usedBytes());

  Serial.println("readFileJSON");
  JsonDocument doc;
  doc = readFileJSON(SPIFFS, "/config.json");    
  Serial.print("Read from file: "); 
  serializeJson(doc, Serial);  
  Serial.println();
  return doc;
}

void storeConfig(JsonDocument doc) {
  if (!SPIFFS.begin(true)) {
    Serial.begin(115200);
    Serial.println("SPIFFS Mount Failed");
    delay(1000);
    ESP.restart();
  }
  writeFileJSON(SPIFFS, "/config.json", doc);

  listDir(SPIFFS, "/", 0);
}

void removeConfig() {
  deleteFile(SPIFFS, "/config.json");
  ESP.restart();
}

void store1() {
  if (DEBUG_CONFIG) {
    Serial.begin(115200);
    if (!SPIFFS.begin(true)) {
      Serial.println("SPIFFS Mount Failed");
      return;
    }

    Serial.println("SPIFFS Info:");
    Serial.printf("Total Bytes: %u\n", SPIFFS.totalBytes());
    Serial.printf("Used Bytes: %u\n", SPIFFS.usedBytes());
    Serial.printf("Free Bytes: %u\n", SPIFFS.totalBytes() - SPIFFS.usedBytes());

    listDir(SPIFFS, "/", 0);
    //  writeFile(SPIFFS, "/hello.txt", "Hello ");
    //  appendFile(SPIFFS, "/hello.txt", "World!\r\n");
    //  readFile(SPIFFS, "/hello.txt");
    //  renameFile(SPIFFS, "/hello.txt", "/foo.txt");
    //  readFile(SPIFFS, "/foo.txt");
    //  deleteFile(SPIFFS, "/foo.txt");
    //  Serial.println( "Test complete" );
  }
}