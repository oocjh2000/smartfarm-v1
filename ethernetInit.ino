static void _ethernetInit(void) {
  Ethernet.begin(mac);    
  Serial.print("[INFO] IP:");
  Serial.println(Ethernet.localIP());
}
