
#include <ArduinoBLE.h>

int sensorTime = 3000;               

BLEService environmentService("181A");                         // standard Environmental Sensing BLE service
BLEIntCharacteristic Prediction_Index("2A6E",                // standard 16-bit Index characteristic
                                        BLERead | BLENotify);  // remote clients can read and get updates



//=== Setup code here, runs once ================

void setup() {
  Serial.begin(9600);                                          // initialize serial communication
  while (!Serial);                                             // uncomment for laptop use & comment for wall wart use

  pinMode(LED_BUILTIN, OUTPUT);                                // initialize built-in LED pin

  if (!BLE.begin()) {                                          // initialize NINA B306 BLE radio
    Serial.println("BLE failure!");
    while (1);
  }
  // https://forum.arduino.cc/index.php?topic=660360.0

  BLE.setLocalName("SenseMood");                                 
  BLE.setAdvertisedService(environmentService);                 
  environmentService.addCharacteristic(Prediction_Index);       
                                                                
  BLE.addService(environmentService);                           

  //BLE.advertise();                                              
                                                                  
  // set initial values for the characteristics
  Prediction_Index.writeValue( 0 );

}

//=== Main code, runs/loops repeatedly==========

void loop() {                                                 // Or
  nonBlockingTimer();                                         // refined sensor calibration & output delay timer      // All
  PeripheralBLE();                                            // BLE Peripheral advertising
  CentralBLE();                                               // BLE Central connection
                                                              //   func provides temp & humidity data via BLE radio                                                         
                                                              //   func provides pressure data via BLE radio
}

//===Timer function ==============================

void nonBlockingTimer() {                                    // code runs while “waiting" for event
  unsigned long time_now = 0;
  int period = 1000;
  if(millis() >= time_now + period){                         // ensures that the loop runs as often as we want, regardless of the execution time
        time_now += period;
        //Serial.println("Hello");                           // uncomment to test if function works
  }
}

//===BLE functions ==============================

void modelble() {                                                         
  
  float index    = 1;   
              
  nonBlockingTimer();
  if(index!=-1)Prediction_Index.writeValue(index);                       // advertise index (2A6E) via BLE
  
}

void PeripheralBLE() {
    BLE.advertise();                                          // Start advertising BLE peripheral
}

void CentralBLE() {
  BLEDevice central = BLE.central();                          // Wait for a BLE central to connect
  if (central) {                                              // if central connects
     digitalWrite(LED_BUILTIN, HIGH);                         // turn on the LED
     while (central.connected()) {                            // keep looping while connected
       nonBlockingTimer();
       modelble();
       }
     digitalWrite(LED_BUILTIN, LOW);                          // when the central disconnects, turn off the LED
  }
}
