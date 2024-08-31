int delayDuration = 0; // delay duration between LED cycles in seconds
unsigned long previousMillis = 0;   // stores the last time the LED was updated
const long interval = 2000;         // interval at which to double burst (milliseconds)


// Equipment details
const char* equipment = "DeviceName";
const char* partNumber = "PN123456";
const char* serialNumber = "SN654321";
const char* pcbVersion = "PCBv1.0";
const char* firmwareversion = "FWv2.3";
const char* manufacturer = "ManufacturerName";


void setup() {
  Serial_begin(9600);
  pinMode(6, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);  // Set the built-in LED pin as an output
  digitalWrite(6, HIGH);         // Keep LED on initially
  digitalWrite(LED_BUILTIN, HIGH); // Keep built-in LED on initially
}

void loop() {
  if (Serial_available() > 0) {
    char command[20];
    int index = 0;
    while (Serial_available() > 0 && index < sizeof(command) - 1) {
      command[index++] = Serial_read();
      delay(5); // Small delay to ensure we get the full command
    }
    command[index] = '\0';

    if (strncmp(command, "at+delay=", 9) == 0) {
      delayDuration = atoi(command + 9);
      Serial_println_u(delayDuration);

     
    } 
    
    else if (strcmp(command, "atd") == 0) {
      Serial_println_u(delayDuration);
    } 

 else if (strcmp(command, "ats") == 0) {

      Serial_println_s(equipment);
      Serial_println_s(partNumber);
      Serial_println_s(serialNumber);
      Serial_println_s(pcbVersion);
      Serial_println_s(firmwareversion);
      Serial_println_s(manufacturer);
    }
    
    else {
      Serial_println_s("Invalid AT command");
    }
  }

  // Get the current time
  unsigned long currentMillis = millis();

  // Check if it's time to double burst the LED
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    // Double burst pattern: on-off-on-off
    digitalWrite(6, LOW);            // Turn the LED off
    digitalWrite(LED_BUILTIN, LOW);  // Turn the built-in LED off
    delay(400);                      // Short off duration
    digitalWrite(6, HIGH);           // Turn the LED on
    digitalWrite(LED_BUILTIN, HIGH); // Turn the built-in LED on
  }

  // If a delay duration is set, wait for it
  if (delayDuration > 0) {
    delay(delayDuration * 1000);
  }
}
