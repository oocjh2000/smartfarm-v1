char* actuatingCallback(const char *id, const char *cmd, JsonObject& options) {
  if (strcmp(id, ledId) == 0) {
    t.stop(ledBlinkTimer);
    t.stop(ledOffTimer);

    if (strcmp(cmd, "on") == 0) {
      int duration = options.containsKey("duration") ? options["duration"] : 0;

      digitalWrite(LED_GPIO, HIGH);

      if (duration)
        ledOffTimer = t.after(duration, _ledOff);

      return "success";
    }
    else  if (strcmp(cmd, "off") == 0) {
      _ledOff();
      return "success";
    }
    else  if(strcmp(cmd, "blink") == 0) {
      if (!options.containsKey("interval")) {
        Serial.println(F("[ERR] No blink interval"));
        return NULL;
      }

      ledBlinkTimer = t.oscillate(LED_GPIO, options["interval"], HIGH);

      if (options.containsKey("duration"))
        ledOffTimer = t.after(options["duration"], _ledOff);

      return "success";
    }
    else {
      return NULL;
    }
  }
  else if (strcmp(id, buzzerId) == 0) {
    if (strcmp(cmd, "on") == 0) {
      _buzzerOn();
      return "success";
    }
    else if (strcmp(cmd, "off") == 0) {
      _buzzerOff();
      return "success";
    }
    else {
      return NULL;
    }
  }

  return NULL;
}
