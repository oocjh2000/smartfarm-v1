static void _ethernetInit(void) {
   Ethernet.begin(mac);  
   char st[64];
  IPAddress ip = Ethernet.localIP();
  sprintf(st,"%d %d %d %d",ip[0],ip[1],ip[2],ip[3]);
    u8x8.setFont(u8x8_font_chroma48medium8_r);
   u8x8.drawString(0,1,st);
  
 // Ethernet.begin(mac);    
  Serial.print("[INFO] IP:");
  Serial.println(ip);

}
