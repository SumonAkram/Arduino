const int BUTTON_PIN = 2;  // Pin connected to the push button
const int LED_PIN = 13;    // Pin connected to the LED

bool ledState = false;     // Current state of the LED

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);  // Set the button pin as input with internal pull-up resistor
  pinMode(LED_PIN, OUTPUT);            // Set the LED pin as output
}

void loop() {
  if (digitalRead(BUTTON_PIN) == LOW) {
    // Button is pressed
    ledState = !ledState;       // Toggle the LED state
    digitalWrite(LED_PIN, ledState);  // Update the LED state
    delay(200);                 // Debounce delay
  }
}
