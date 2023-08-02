#include <ArduinoJson.h>;

const int BAUDRATE = 9600;

const int NOTE_C = 1046;
const int NOTE_D = 1174;
const int NOTE_E = 1318;
const int NOTE_F = 1396;
const int NOTE_G = 1567;
const int NOTE_A = 1760;

const int noteLength = 14;
// ドレミファミレドミファソラソファミ
int notes[] = {
  NOTE_C,
  NOTE_D,
  NOTE_E,
  NOTE_F,
  NOTE_E,
  NOTE_D,
  NOTE_C,
  NOTE_E,
  NOTE_F,
  NOTE_G,
  NOTE_A,
  NOTE_G,
  NOTE_F,
  NOTE_E,
};

StaticJsonDocument<128> serialJson;
DynamicJsonDocument data(128);

int playCounter = 0;
int noteIndex = 0;

void setup() {
  Serial.begin(BAUDRATE);
}

void loop() {
  writeSerial();

  bool jsonError = false;

  serialJson.clear();
  data.clear();

  if (nextSerialJson(&serialJson, &jsonError)) {
    String type = serialJson["type"];

    if (type.equals("null")) {
    } else if (type.equals("play")) {
      playCounter = serialJson["count"];
      noteIndex = 0;
    } else if (type.equals("stop")) {
      playCounter = 0;
      noteIndex = 0;
    }
  }
  else if (jsonError) {
    data["type"] = "jsonError";
    serializeJson(data, Serial);
    Serial.println("");
  }

  if (playCounter > 0) {
    tone(4, notes[noteIndex]);
    delay(500);

    noteIndex += 1;
    if (noteIndex >= noteLength) {
      playCounter --;
      noteIndex = 0;
    }
  } else {
    noTone(4);
  }
}

void writeSerial() {
  data.clear();
  data["type"] = "pstate";
  data["count"] = playCounter;
  serializeJson(data, Serial);
  Serial.println("");
}
