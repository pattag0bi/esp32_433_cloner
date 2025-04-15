
// Libraries
#include <RCSwitch.h>

// Pinout declaration
int builtInLed = 2;  // Built-in LED pin (GPIO2)
int rfReceiverPin = 21;
int rfTransmitterPin = 22; 

// Additional variables needed through the code
unsigned long currentSecounds, previousSecoundsRecord;
String receivedCommand;
String commandToSend;

unsigned long receivedCodes[100];
int receivedProtocols[100];
int receiverCounter = 0;
bool canStoreCurrentCode = true;
unsigned long previousMillisLed = 0;
bool blinkLed = false;
int ledCounter = 0;

RCSwitch mySwitch = RCSwitch();

void setup() {
  Serial.begin(115200);
  delay(1000); // Give time for serial to initialize
  Serial.println("\n\n==========================================");
  Serial.println("         ESP32 RF Cloner Started");
  Serial.println("==========================================");
  Serial.println("\nSystem Configuration:");
  Serial.println("----------------------");
  Serial.println("1. Serial port: 115200 baud");
  Serial.println("2. RF Receiver: Pin 21");
  Serial.println("3. RF Transmitter: Pin 22");
  Serial.println("4. Status LED: Built-in LED (GPIO2)");
  Serial.println("\nAvailable Commands:");
  Serial.println("-------------------");
  Serial.println("1. 'refresh data' - Get all stored codes");
  Serial.println("2. 'code|protocol' - Send a specific code");
  Serial.println("   Example: '12345|1'");
  Serial.println("3. 'clear signals' - Clear all stored signals");
  Serial.println("\nWaiting for commands...");
  Serial.println("==========================================\n");
  
  mySwitch.enableReceive(rfReceiverPin);
  mySwitch.enableTransmit(rfTransmitterPin);
  pinMode(builtInLed, OUTPUT);
  digitalWrite(builtInLed, HIGH); // Turn on LED to show setup is complete
  delay(500);
  digitalWrite(builtInLed, LOW);
}

void loop() {
  currentSecounds = millis() / 1000;
  receiveSerialData();
  decodeRfSignals();
  checkLedState();
  
  if ((currentSecounds - previousSecoundsRecord) >= 4) {
    previousSecoundsRecord = currentSecounds;
    Serial.println("\n[System Status] Active and waiting for commands...\n");
  }
}

void receiveSerialData() {
  if (Serial.available()) {
    receivedCommand = Serial.readStringUntil('\n');
    receivedCommand.trim();
    receivedCommand.toLowerCase();
    
    Serial.println("\n[Command Received] " + receivedCommand);
    
    if (receivedCommand == "refresh data") {
      sendCurrentRfData();
    } else if (receivedCommand == "clear signals") {
      clearAllSignals();
    } else if (receivedCommand.indexOf("|") != -1) {
      int codeForSend = receivedCommand.substring(0, receivedCommand.indexOf("|")).toInt();
      int protocolForSend = receivedCommand.substring(receivedCommand.indexOf("|")+1).toInt();
      Serial.println("\n[Transmit]");
      Serial.println("Code: " + String(codeForSend));
      Serial.println("Protocol: " + String(protocolForSend));
      sendCodeOverRfModule(codeForSend, protocolForSend);
    }
  }
}

void sendCurrentRfData() {
  if (receiverCounter != 0) {
    Serial.println("\n[Stored Codes]");
    Serial.println("----------------");
    for (int i=0; i<receiverCounter; i++) {
      commandToSend = String(receivedCodes[i])+"|"+String(receivedProtocols[i]);
      Serial.println("Signal #" + String(i+1) + ": " + commandToSend);
      delay(200);
    }
    Serial.println("----------------");
    Serial.println("Total signals: " + String(receiverCounter));
  } else {
    Serial.println("\n[Info] No codes stored in memory");
  }
}

void sendCodeOverRfModule(int code, int protocol) {
  // First blink
  blinkLed = true;
  while(blinkLed) {
    checkLedState();
  }
  delay(100);
  
  // Second blink
  blinkLed = true;
  while(blinkLed) {
    checkLedState();
  }
  
  mySwitch.setProtocol(protocol);
  mySwitch.send(code, 24);
  Serial.println("[Success] Code transmitted successfully");
}

void decodeRfSignals() {
  if (mySwitch.available()) {
    blinkLed = true;
    unsigned long receivedValue = mySwitch.getReceivedValue();
    int receivedProtocol = mySwitch.getReceivedProtocol();
    int receivedBitlength = mySwitch.getReceivedBitlength();
    int receivedDelay = mySwitch.getReceivedDelay();
    
    Serial.println("\n[Signal Received]");
    Serial.println("----------------");
    Serial.println("Code: " + String(receivedValue));
    Serial.println("Protocol: " + String(receivedProtocol));
    Serial.println("Bit Length: " + String(receivedBitlength) + " bits");
    Serial.println("Delay: " + String(receivedDelay) + " µs");
    
    // Add protocol description based on known protocols
    String protocolDesc = "Unknown Protocol";
    switch(receivedProtocol) {
      case 1: protocolDesc = "PT2262"; break;
      case 2: protocolDesc = "Standard"; break;
      case 3: protocolDesc = "Standard"; break;
      case 4: protocolDesc = "Standard"; break;
      case 5: protocolDesc = "Standard"; break;
      case 6: protocolDesc = "HT6P20B"; break;
      case 7: protocolDesc = "HS2303-PT"; break;
      case 8: protocolDesc = "Conrad RS-200 RX"; break;
      case 9: protocolDesc = "Conrad RS-200 TX"; break;
      case 10: protocolDesc = "1ByOne Doorbell"; break;
      case 11: protocolDesc = "HT12E"; break;
      case 12: protocolDesc = "SM5212"; break;
    }
    Serial.println("Protocol Type: " + protocolDesc);
    
    // Add signal quality information
    String signalQuality = "Good";
    if (receivedDelay > 5000) {
      signalQuality = "Poor (High Delay)";
    } else if (receivedBitlength < 12) {
      signalQuality = "Poor (Short Bit Length)";
    }
    Serial.println("Signal Quality: " + signalQuality);
    Serial.println("----------------");
    
    for (int i=0; i<receiverCounter; i++) {
      if (receivedCodes[i] == receivedValue) {
        canStoreCurrentCode = false;
        break;
      }
    }

    if (canStoreCurrentCode) {
      receivedCodes[receiverCounter] = receivedValue;
      receivedProtocols[receiverCounter] = receivedProtocol;
      receiverCounter++;
      Serial.println("[Info] Signal stored successfully");
      Serial.println("Total signals: " + String(receiverCounter));
    } else {
      Serial.println("[Info] Signal already exists in memory");
    }

    canStoreCurrentCode = true;
    mySwitch.resetAvailable();
  }
}

void checkLedState() {
  unsigned long currentMillisLed = millis();  
  if (blinkLed) {
    if ((unsigned long)(currentMillisLed - previousMillisLed) >= 100) {
      if (ledCounter == 0) {
        digitalWrite(builtInLed, HIGH);
        ledCounter = 1;
      } else if (ledCounter == 1) {
        digitalWrite(builtInLed, LOW);
        ledCounter = 0;
        blinkLed = false;
      }
      previousMillisLed = currentMillisLed;
    }
  }
}

void clearAllSignals() {
  receiverCounter = 0;
  Serial.println("\n[Info] All signals cleared from memory");
  Serial.println("Total signals: 0");
} 