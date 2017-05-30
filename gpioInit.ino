static void _gpioInit(void) {  
  pinMode(LED_GPIO, OUTPUT);
  pinMode(BUZZER_GPIO, OUTPUT);
  pinMode(BUTTON_GPIO, INPUT_PULLUP); 
  attachInterrupt(digitalPinToInterrupt(BUTTON_GPIO), _buttonISR, CHANGE);

  // Thingplus connection indicator LED
  pinMode(CONNECT_LED_GPIO, OUTPUT);
  digitalWrite(CONNECT_LED_GPIO, LOW);
}
