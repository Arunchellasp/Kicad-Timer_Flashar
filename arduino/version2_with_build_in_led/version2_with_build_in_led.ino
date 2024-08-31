int onDuration = 0;    // duration for LED ON in seconds
int offDuration = 0;   // duration for LED OFF in seconds
int delayDuration = 0; // delay duration between ON and OFF cycles in seconds
unsigned long lastOnTimestamp = 0;  // Last timestamp when LED turned on
unsigned long lastOffTimestamp = 0; // Last timestamp when LED turned off

unsigned long previousMillis = 0;   // stores the last time LED was updated
const long interval = 2000;         // interval at which to double burst (milliseconds)

void setup() {
  Serial_begin(9600);
  pinMode(6, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);  // Set the built-in LED pin as an output
  digitalWrite(6, HIGH);         // Keep LED on initially
  digitalWrite(LED_BUILTIN, HIGH); // Keep built-in LED on initially

  // Print the initial timestamps to Serial Monitor
  Serial_println_u(lastOnTimestamp);
  Serial_println_u(lastOffTimestamp);
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

    if (strncmp(command, "at+ on=", 6) == 0) {
      onDuration = atoi(command + 6);
      Serial_println_u(onDuration);
    } else if (strncmp(command, "at+off=", 7) == 0) {
      offDuration = atoi(command + 7);
      Serial_println_u(offDuration);
    } else if (strncmp(command, "at+delay=", 9) == 0) {
      delayDuration = atoi(command + 9);
      Serial_println_u(delayDuration);
    } else if (strcmp(command, "atd") == 0) {
      Serial_println_u(delayDuration);
    } else {
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

  // If durations are set, blink the LED
  if (onDuration > 0 && offDuration > 0) {
    digitalWrite(6, HIGH);           // Turn the LED on
    digitalWrite(LED_BUILTIN, HIGH); // Turn the built-in LED on
    lastOnTimestamp = onDuration;    // Record the on timestamp
    delay(onDuration * 1000);        // Wait for the on duration in milliseconds
    
    digitalWrite(6, LOW);            // Turn the LED off
    digitalWrite(LED_BUILTIN, LOW);  // Turn the built-in LED off
    lastOffTimestamp = offDuration;  // Record the off timestamp
    delay(offDuration * 1000);       // Wait for the off duration in milliseconds
    
    delay(delayDuration * 1000);     // Wait for the delay duration in milliseconds
  } else {
    if (delayDuration > 0) {
      delay(delayDuration * 1000);
    }
  }
}
