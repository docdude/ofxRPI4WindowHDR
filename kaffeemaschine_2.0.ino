//Version: 2.0 by wintermute, without MQTT

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPUpdateServer.h>
// used for UDP communication
#include <WiFiUdp.h>
// used for general time related stuff
#include <Time.h>
// used to schedule actions at specific times
#include <TimeAlarms.h>
// SoftwareSerial for communicating with the machine
#include <SoftwareSerial.h>
enum machine {NONE,UNSUPPORTED,S90,E50,C5};



/*
 * Configuration START
 */

// set machine type, or set to "NONE" to disable communication with machine (for test mode)
// set to "UNSUPPORTED" if your machine is not listed and not compatible with listed types
machine type=NONE;
// Hostname
const char* host="kaffee.localnet";
// mDNS name (leave empty if not needed)
const char* mdns="jura";
// SSID
const char* ssid="...";
// Passphrase
const char* passphrase="...";
// IP (leave empty if DHCP is used)
//IPAddress ip(192,168,100,200);
IPAddress ip;
// Netmask (can be ignored if DHCP is used)
IPAddress mask(255,255,255,0);
// Gateway (set to (0,0,0,0) if no gateway should be configured or DHCP is used)
IPAddress gateway(0,0,0,0);
// DNS (can be ignored if DHCP is used)
IPAddress dns;
// TimeServer IP (leave empty to use hostname, defined below)
IPAddress NTPserverIP(192,168,1,1);
// TimeServer Hostname (will be overriden by above configuration)
const char* NTPserverName="1.de.pool.ntp.org";
// username and password for web access (leave at least one empty if no authentication should be used)
const char* user="admin";
const char* pass="";
// UDP receiver IP and port, if both is provided, status messages of the machine will be send to that host
const char* udpRemoteAddress="192.168.1.200";
const unsigned int udpRemotePort=2222;
// machine gets queried every "cycle" seconds
unsigned int cycle=60;

/*
 * Configuration END
 */



/*
 * Variables & constants
 */
// Version
const String fw_version="0.1 - 2016112801";
// connection to machine
SoftwareSerial mySerial(D5,D6);
// Webserver & Update
ESP8266WebServer server(80);
ESP8266HTTPUpdateServer httpUpdater;
// NTP stuff
const int NTP_PACKET_SIZE=48;
byte packetBuffer[NTP_PACKET_SIZE];
const int timeZone=1; // CET
// UDP instance for NTP communication
WiFiUDP udp;
// UDP instance for state updates
WiFiUDP Sudp;
// web response
String html="";
String head_main="<html>\
<head>\
 <link rel='shortcut icon' type='image/x-icon' href='data:image/x-icon;base64,AAABAAEAEBAAAAEAIABoBAAAFgAAACgAAAAQAAAAIAAAAAEAIAAAAAAAAAQAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAQEREDwAAAAAmJiYtR0lJgCYmJi1AREQPAAAAAAAAAAAAAAAAAAAAAEBERA8AAAAAQEREDwAAAABNTU3/TU1N/01NTf9MTEz/TU1N/01NTf9NTU3/TU1N/0xMTP9NTU3/TU1N/wAAAAAAAAAAQEREDwAAAABMTEz/TU1N/01NTf9NTU3/AAAAAEBERA8nJyd/AAAAAAAAAABNTU3/TExM/01NTf9NTU3/AAAAAAAAAAAAAAAAAAAAAEBERA9NTU3/AAAAAE1NTf9NTU3/TExM/01NTf9NTU3/AAAAAE1NTf9AREQPAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAQERED01NTf9LTExwTU1N/01NTf9MTEz/TU1N/01NTf8AAAAAAAAAAEBERA8AAAAAAAAAAAAAAAAAAAAAAAAAAE1NTf9AREQPTU1N/01NTf9NTU3/TU1N/0xMTP9NTU3/TU1N/01NTf9NTU3/QEREDwAAAABAREQPAAAAAAAAAABNTU3/AAAAAExMTP9NTU3/TU1N/01NTf9NTU3/TExM/01NTf8AAAAATU1N/wAAAABAREQPAAAAAEBERA8AAAAATU1N/wAAAABNTU3/TExM/01NTf9NTU3/TU1N/01NTf9MTEz/AAAAAExMTIxGSEh5AAAAAAAAAAAAAAAARkhIPU1NTf9NTU3/TU1N/01NTf9MTEz/TU1N/01NTf9NTU3/TU1N/0xMTP9NTU3/AAAAAAAAAAAAAAAAAAAAACcnJxVMTEz/AAAAAAAAAAAAAAAAAAAAAEBERA8AAAAAAAAAAE1NTf8mJiYmQEREDwAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAExMTP9NTU3/TU1N/01NTf9NTU3/TExM/01NTf8AAAAAAAAAAAAAAABAREQPAAAAAEBERA8AAAAAAAAAAAAAAAAAAAAAQEREDwAAAAAAAAAAAAAAAAAAAABAREQPAAAAAAAAAAAAAAAAAAAAAEBERA8AAAAAQEREDwAAAAAAAAAAAAAAAE1NTf9AREQPTU1NpwAAAABNTU3/AAAAAEBERA8AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABAREQPAAAAAAAAAABNTU3/AAAAAExMTP8AAAAATU1N/wAAAAAAAAAAQEREDwAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAEBERA8AAAAAAAAAAAAAAAAAAAAAQEREDwAAAAAAAAAAAAAAAAAAAABAREQPAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAQEREDwAAAAAAAAAAAAAAAAAAAABAREQPAAAAAAAAAAAAAAAAAAAAAEBERA8AAAAA/v8AAMAHAACHwwAA6C8AAPQfAADoAwAA6AsAAOgLAADgAwAA7+8AAPAfAAD//wAA+r8AAPq/AAD//wAA//8AAA=='/> \
 <title>"+String(host)+"</title>\
 <meta http-equiv='refresh' content='60'>\
 <style>\
  body,a { background-color: #000000; font-family: Arial, Helvetica, Sans-Serif; Color: #d0d0d0; }\
  a:link { text-decoration: none; }\
  a:visited { text-decoration: none; }\
  a:hover { text-decoration: underline; }\
  a:active { text-decoration: underline; }\
 </style>\
</head>\
<body>\
";
String head="<html>\
<head>\
 <link rel='shortcut icon' type='image/x-icon' href='data:image/x-icon;base64,AAABAAEAEBAAAAEAIABoBAAAFgAAACgAAAAQAAAAIAAAAAEAIAAAAAAAAAQAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAQEREDwAAAAAmJiYtR0lJgCYmJi1AREQPAAAAAAAAAAAAAAAAAAAAAEBERA8AAAAAQEREDwAAAABNTU3/TU1N/01NTf9MTEz/TU1N/01NTf9NTU3/TU1N/0xMTP9NTU3/TU1N/wAAAAAAAAAAQEREDwAAAABMTEz/TU1N/01NTf9NTU3/AAAAAEBERA8nJyd/AAAAAAAAAABNTU3/TExM/01NTf9NTU3/AAAAAAAAAAAAAAAAAAAAAEBERA9NTU3/AAAAAE1NTf9NTU3/TExM/01NTf9NTU3/AAAAAE1NTf9AREQPAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAQERED01NTf9LTExwTU1N/01NTf9MTEz/TU1N/01NTf8AAAAAAAAAAEBERA8AAAAAAAAAAAAAAAAAAAAAAAAAAE1NTf9AREQPTU1N/01NTf9NTU3/TU1N/0xMTP9NTU3/TU1N/01NTf9NTU3/QEREDwAAAABAREQPAAAAAAAAAABNTU3/AAAAAExMTP9NTU3/TU1N/01NTf9NTU3/TExM/01NTf8AAAAATU1N/wAAAABAREQPAAAAAEBERA8AAAAATU1N/wAAAABNTU3/TExM/01NTf9NTU3/TU1N/01NTf9MTEz/AAAAAExMTIxGSEh5AAAAAAAAAAAAAAAARkhIPU1NTf9NTU3/TU1N/01NTf9MTEz/TU1N/01NTf9NTU3/TU1N/0xMTP9NTU3/AAAAAAAAAAAAAAAAAAAAACcnJxVMTEz/AAAAAAAAAAAAAAAAAAAAAEBERA8AAAAAAAAAAE1NTf8mJiYmQEREDwAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAExMTP9NTU3/TU1N/01NTf9NTU3/TExM/01NTf8AAAAAAAAAAAAAAABAREQPAAAAAEBERA8AAAAAAAAAAAAAAAAAAAAAQEREDwAAAAAAAAAAAAAAAAAAAABAREQPAAAAAAAAAAAAAAAAAAAAAEBERA8AAAAAQEREDwAAAAAAAAAAAAAAAE1NTf9AREQPTU1NpwAAAABNTU3/AAAAAEBERA8AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABAREQPAAAAAAAAAABNTU3/AAAAAExMTP8AAAAATU1N/wAAAAAAAAAAQEREDwAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAEBERA8AAAAAAAAAAAAAAAAAAAAAQEREDwAAAAAAAAAAAAAAAAAAAABAREQPAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAQEREDwAAAAAAAAAAAAAAAAAAAABAREQPAAAAAAAAAAAAAAAAAAAAAEBERA8AAAAA/v8AAMAHAACHwwAA6C8AAPQfAADoAwAA6AsAAOgLAADgAwAA7+8AAPAfAAD//wAA+r8AAPq/AAD//wAA//8AAA=='/> \
 <title>"+String(host)+"</title>\
 <style>\
  body,a { background-color: #000000; font-family: Arial, Helvetica, Sans-Serif; Color: #d0d0d0; }\
  a:link { text-decoration: none; }\
  a:visited { text-decoration: none; }\
  a:hover { text-decoration: underline; }\
  a:active { text-decoration: underline; }\
 </style>\
</head>\
<body>\
";
// machine stuff
String mState="";
// misc stuff
unsigned long lastRun=0;
const int led = LED_BUILTIN;
// Debug stuff
unsigned int debug_pointer=0;
char debug_0[100]="";
char debug_1[100]="";
char debug_2[100]="";
char debug_3[100]="";
char debug_4[100]="";
char debug_5[100]="";
char debug_6[100]="";
char debug_7[100]="";
char debug_8[100]="";
char debug_9[100]="";
char debug_10[100]="";
char debug_11[100]="";
char debug_12[100]="";
char debug_13[100]="";
char debug_14[100]="";
char debug_15[100]="";
char debug_16[100]="";
char debug_17[100]="";
char debug_18[100]="";
char debug_19[100]="";
String debug_table[]={
  debug_0,
  debug_1,
  debug_2,
  debug_3,
  debug_4,
  debug_5,
  debug_6,
  debug_7,
  debug_8,
  debug_9,
  debug_10,
  debug_11,
  debug_12,
  debug_13,
  debug_14,
  debug_15,
  debug_16,
  debug_17,
  debug_18,
  debug_19
};
unsigned int debug_count=20;
char debug_buffer[100];
boolean debug_lastnewline=false;

/////////////////////////////

String date2text() {
  char buf[11]="";
  if (timeStatus()==timeSet) sprintf(buf,"%02d.%02d.%04d",day(),month(),year());
  return String(buf);
}

String time2text() {
  char buf[8]="";
  if (timeStatus()==timeSet) sprintf(buf, "%02d:%02d:%02d",hour(),minute(),second());
  return String(buf);
}

void debug(String text="", boolean newline=true) {
  if (debug_lastnewline && timeStatus()==timeSet && text.length()) text=time2text()+" :: "+text;
  Serial.print(text);
  if (debug_lastnewline) {
    if (text.length()) {
      debug_pointer++;
      if (debug_pointer>=debug_count) debug_pointer=0;
    }
  } else {
    text=debug_table[debug_pointer]+text;
  }

  if (newline) {
    Serial.println();
  }
  debug_lastnewline=newline;
  // add debug message to debug array
  if (text.length()) {
    text.toCharArray(debug_buffer,100);
    debug_table[debug_pointer]=debug_buffer;
  }
}

void http_debug(String txt, boolean newline=true) {
  debug(txt + String(" [") + server.client().remoteIP().toString() + String("]"), newline);
}

void restart() {
  ESP.restart();
}

void reset() {
  ESP.reset();
}

/*
 * NTP Functions BEGIN
 */
unsigned long sendNTPpacket(IPAddress& ip) {
  // build and send a NTP query packet
  memset(packetBuffer,0,NTP_PACKET_SIZE);
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;            // Stratum, or type of clock
  packetBuffer[2] = 6;            // Polling Interval
  packetBuffer[3] = 0xEC;         // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]= 49;
  packetBuffer[13]= 0x4E;
  packetBuffer[14]= 49;
  packetBuffer[15]= 52;
  udp.beginPacket(ip, 123);
  udp.write(packetBuffer,NTP_PACKET_SIZE);
  udp.endPacket();
}

time_t getNTPtime() {
  int pp=0;
  while (pp==0) {
    sendNTPpacket(NTPserverIP);
    delay(1000);
    pp=udp.parsePacket();
    delay(100);
  }

  udp.read(packetBuffer,NTP_PACKET_SIZE);
  unsigned long secsSince1900;
  // convert four bytes starting at location 40 to a long integer
  secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
  secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
  secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
  secsSince1900 |= (unsigned long)packetBuffer[43];
  return secsSince1900-2208988800UL+timeZone*SECS_PER_HOUR;
}
/*
 * NTP functions END
 */

/*
 * Webserver functions BEGIN
 */
void auth() {
  digitalWrite(led,LOW);
  if(user!="" && pass!="") {
    if (!server.authenticate(user,pass)) server.requestAuthentication();
  }
  delay(50);
  digitalWrite(led,HIGH);
}

boolean ARGplain() {
  for (uint8_t i=0; i<server.args(); i++) {
    if (server.argName(i)=="plain") return true;
  }
  return false;
}

String ARGcommand() {
  for (uint8_t i=0; i<server.args(); i++) {
    if (server.argName(i)=="cmd") return server.arg(i);
  }
  return "";
}

String ARGparam() {
  for (uint8_t i=0; i<server.args(); i++) {
    if (server.argName(i)=="param") return server.arg(i);
  }
  return("");
}

void web_root() {
  auth();
  http_debug("/ accessed");
  int s=WiFi.RSSI();
  if (ARGplain()) {
    html ="Status: "+readState()+"\n";
    switch (type) {
      case NONE:
        html+="Machine: none\n";
        break;
      case UNSUPPORTED:
        html+="Machine: unsupported\n";
        break;
      case C5:
        html+="Machine: C5\n";
        break;
      case S90:
        html+="Machine: S90\n";
        break;
      case E50:
        html+="Machine: E50\n";
        break;
      default:
        html+="Machine: none\n";
        break;
    }
    html+="Controller uptime: "+String(millis())+"\n";
    html+="SSID: "+String(ssid)+"\n";
    html+="Signal: "+String(s)+"\n";
    html+="Time: "+time2text()+"\n";
    html+="Date: "+date2text()+"\n";
    html+="Firmware: "+fw_version+"\n";
    server.send(200,"text/plain",html);
  } else {
    html=head_main;
    html+="<h3>Status: "+readState()+"</h3>";
    html+="<a href='/power?param=0'>Abschalten</a><br/>";
    html+="<a href='/power?param=1'>Anschalten</a><br/>";
    html+="<a href='/flush'>Sp&uuml;len</a><br/>";
    html+="<hr/>";
    html+="<a href='/proffer?param=1'>Produkt 1 beziehen</a><br/>";
    html+="<a href='/proffer?param=2'>Produkt 2 beziehen (Ger&auml;t sp&uuml;len)</a><br/>";
    html+="<a href='/proffer?param=3'>Produkt 3 beziehen (Pulverkaffee)</a><br/>";
    html+="<a href='/proffer?param=4'>Produkt 4 beziehen (1 kleine Tasse)</a><br/>";
    html+="<a href='/proffer?param=5'>Produkt 5 beziehen (2 kleine Tassen)</a><br/>";
    html+="<a href='/proffer?param=6'>Produkt 6 beziehen (1 gro&szlig;e Tasse)</a><br/>";
    html+="<a href='/proffer?param=7'>Produkt 7 beziehen (2 gro&szlig;e Tassen)</a><br/>";
    html+="<a href='/proffer?param=8'>Produkt 8 beziehen (Dampf Portion)</a><br/>";
    html+="<a href='/proffer?param=9'>Produkt 9 beziehen (Dampf)</a><br/>";
    html+="<a href='/proffer?param=10'>Produkt 10 beziehen</a><br/>";
    html+="<a href='/proffer?param=11'>Produkt 11 beziehen</a><br/>";
    html+="<a href='/proffer?param=12'>Produkt 12 beziehen (Kaffee Spezial)</a><br/>";
    html+="<a href='/proffer?param=13'>Produkt 13 beziehen</a><br/>";
    html+="<hr/>";
    html+="<a href='/reset'>Controller Reset</a><br/>";
    html+="<a href='/restart'>Controller Restart</a><br/>";
    html+="<a href='/update'>Controller Update</a><br/>";
    html+="<a href='/debug'>Debug Log</a><br/>";
    html+="<hr/>";
    html+=String(host)+", verbunden mit "+String(ssid)+", Signalst&auml;rke "+String(s)+"dBm&nbsp;-&nbsp;";
    html+="Systemzeit "+date2text()+", "+time2text()+"<br/>";
    html+="<small>Controller uptime "+String(millis())+"ms&nbsp;-&nbsp;";
    html+="Firmwareversion "+fw_version;
    html+="</small></body></html>";
    server.send(200,"text/html",html);
  }
  delay(100);
}

void web_reset() {
  auth();
  http_debug("/reset accessed");
  debug("Reset requested");
  if (ARGplain()) {
    server.send(200,"text/plain","ok");
  } else {
    html=head;
    html+="<h3>Controller is resetting</h3><a href='/'>back</a></body</html>";
    server.send(200,"text/html",html);
  }
  reset();
}

void web_restart() {
  auth();
  http_debug("/restart accessed");
  debug("Restart requested");
  if (ARGplain()) {
    server.send(200,"text/plain","ok");
  } else {
    html=head;
    html+="<h3>Controller is restarting</h3><a href='/'>back</a></body</html>";
    server.send(200,"text/html",html);
  }
  restart();
}

void web_send() {
  auth();
  if (ARGplain()) {
    server.send(200,"text/plain","ok");
  } else {
    html=head;
    html+="<h3>Controller is resetting</h3><a href='/'>back</a></body</html>";
    server.send(200,"text/html",html);
  }
  restart();
}

void web_power() {
  auth();
  http_debug("/power accessed");
  unsigned int p=ARGparam().toInt();
  if (ARGparam()=="") {
    if (mState=="off") {
      server.send(200,"text/plain","0");
    } else {
      server.send(200,"text/plain","1");
    }
    return;
  }
  String command="AN:02";
  String answer="";
  switch (p) {
    case 1:
      debug("Power On"); command="AN:01"; break;
    default:
      debug("Power Off"); command="AN:02"; break;
  }
  toMachine(command);
  delay(50);
  answer=fromMachine();
  if (ARGplain()) {
    server.send(200,"text/plain","Answer: "+answer);
  } else {
    html=head;
    html+="<h3>Maschine an/abschalten</h3>R&uuml;ckmeldung: "+answer+"<hr/><a href='/'>back</a></body</html>";
    server.send(200,"text/html",html);
  }

}

void web_flush() {
  auth();
  http_debug("/flush accessed");
  String answer="";
  toMachine("FA:0B");
  delay(50);
  answer=fromMachine();
  if (ARGplain()) {
    server.send(200,"text/plain","Answer: "+answer);
  } else {
    html=head;
    html+="<h3>Sp&uuml;len</h3>R&uuml;ckmeldung: "+answer+"<hr/><a href='/'>back</a></body</html>";
    server.send(200,"text/html",html);
  }

}

void web_proffer() {
  auth();
  http_debug("/proffer accessed");
  unsigned int p=ARGparam().toInt();
  String command="";
  String answer="";
  switch (p) {
    case 1:
      debug("Product 1 requested"); command="FA:01"; break;
    case 2:
      debug("Product 2 requested"); command="FA:02"; break;
    case 3:
      debug("Product 3 requested"); command="FA:03"; break;
    case 4:
      debug("Product 4 requested (small cup???)"); command="FA:04"; break;
    case 5:
      debug("Product 5 requested (two small cups???)"); command="FA:05"; break;
    case 6:
      debug("Product 6 requested (large cup???)"); command="FA:06"; break;
    case 7:
      debug("Product 7 requested (two large cups???)"); command="FA:07"; break;
    case 8:
      debug("Product 8 requested (hot water???)"); command="FA:08"; break;
    case 9:
      debug("Product 9 requested (steam???)"); command="FA:09"; break;
    case 10:
      debug("Product 10 requested (steam???)"); command="FA:0A"; break;
    case 11:
      debug("Product 11 requested (steam???)"); command="FA:0B"; break;
    case 12:
      debug("Product 12 requested (XXL)"); command="FA:0C"; break;
    case 13:
      debug("Product 13 requested (steam???)"); command="FA:0D"; break;
    default:
      debug("Unknown product "+String(p)+" requested, aborting"); return;
  }
  toMachine(command);
  delay(50);
  answer=fromMachine();
  if (ARGplain()) {
    server.send(200,"text/plain","Answer: "+answer);
  } else {
    html=head;
    html+="<h3>Produkt "+String(p)+" angefordert</h3>R&uuml;ckmeldung: "+answer+"<hr/><a href='/'>back</a></body</html>";
    server.send(200,"text/html",html);
  }
}

void web_debug() {
  auth();
  //http_debug("/debug accessed");
  uint8_t p=debug_pointer;
  if (ARGplain()) {
    html="";
    for (uint8_t i=1; i<=debug_count; i++) p++;
    if (p>=debug_count) p-=debug_count;
    html+=String(debug_table[p]) + String("\n");
    server.send(200,"text/plain",html);
  } else {
    html=head+String("<h3>Debug Ausgaben</h3><pre>");
    for (uint8_t i=1; i<=debug_count; i++) {
      p++;
      if (p>=debug_count) p-=debug_count;
      html+=String(debug_table[p]) + String("\n");
    }
    html+=String("</pre><hr/><a href='/'>back</a></body></html>");
    server.send(200,"text/html",html);
  }
}
/*
 * Webserver functions END
 */

void sendState() {
  if (udpRemotePort==0 || udpRemoteAddress=="") return;
  char b[101];
  mState.toCharArray(b,mState.length()+1);
  Sudp.beginPacket(udpRemoteAddress,udpRemotePort);
  Sudp.write(b);
  Sudp.endPacket();
  debug("UDP packet sent: "+mState);
}

/*
 * Communication with machine BEGIN
 * Parts of this code (especially the essential communicaton with the machine) are taken from
 * or are inspired by code published by Oliver Krohn and the c't sharepresso project
 */
String fromMachine() {
  if (type==NONE) {
    debug("FROM machine: N/A - no machine type set");
    return("");
  }
  delay(10);
  String inputString="";
  char d4=255;
  while (mySerial.available()) {
    byte d0=mySerial.read();
    delay(1); 
    byte d1=mySerial.read();
    delay(1); 
    byte d2=mySerial.read();
    delay(1); 
    byte d3=mySerial.read();
    delay(7);
    bitWrite(d4,0,bitRead(d0,2));
    bitWrite(d4,1,bitRead(d0,5));
    bitWrite(d4,2,bitRead(d1,2));
    bitWrite(d4,3,bitRead(d1,5));
    bitWrite(d4,4,bitRead(d2,2));
    bitWrite(d4,5,bitRead(d2,5));
    bitWrite(d4,6,bitRead(d3,2));
    bitWrite(d4,7,bitRead(d3,5));
    if (d4!=10) inputString+=d4;
    yield();
  }
  inputString.trim();
  debug("FROM machine: "+inputString);
  return(inputString);
}

void toMachine(String outputString) {
  outputString.toUpperCase();
  if (type==NONE) {
    debug("TO machine: "+outputString+" - nothing sent, machine type not set");
    return;
  }
  debug("TO machine: "+outputString);
  outputString+="\r\n";
  for (byte a=0; a<outputString.length(); a++) {
    byte d0=255;
    byte d1=255;
    byte d2=255;
    byte d3=255;
    bitWrite(d0,2,bitRead(outputString.charAt(a),0));
    bitWrite(d0,5,bitRead(outputString.charAt(a),1));
    bitWrite(d1,2,bitRead(outputString.charAt(a),2));
    bitWrite(d1,5,bitRead(outputString.charAt(a),3));
    bitWrite(d2,2,bitRead(outputString.charAt(a),4));
    bitWrite(d2,5,bitRead(outputString.charAt(a),5));
    bitWrite(d3,2,bitRead(outputString.charAt(a),6));
    bitWrite(d3,5,bitRead(outputString.charAt(a),7));
    mySerial.write(d0);
    delay(1);
    mySerial.write(d1);
    delay(1);
    mySerial.write(d2);
    delay(1);
    mySerial.write(d3);
    delay(7);
  }
}

String readState() {
  String answer="";
  String power="";
  String state="";
  switch (type) {
    case NONE:
      return("unknown");
      break;
    case UNSUPPORTED:
      return("unknown");
      break;
    case C5:
      toMachine("RR:BB");
      delay(50);
      answer=fromMachine().substring(3,7);
      if (answer=="0104") {
        debug("STATE: on");
        mState="on";
        sendState();
        return(mState);
      } else if (answer=="0100") {
        debug("STATE: off");
        mState="off";
        sendState();
        return(mState);
      } else {
        debug("STATE UNKNOWN: "+state+"/"+answer);
        mState="unknown :: "+state+"/"+answer;
        sendState();
        return(mState);
      }
      break;
    case S90:
      toMachine("RR:03");
      delay(50);
      answer=fromMachine();
      power=answer.substring(4,5);
      state=answer.substring(6,7)+answer.substring(25,28);
      if (power=="0") {
        debug("STATE: off");
        mState="off";
        sendState();
        return(mState);
      } else if (state=="C404") {
        debug("STATE: out of water");
        mState="no_water";
        sendState();
        return(mState);
      } else if (state=="4805") {
        debug("STATE: out of beans");
        mState="no_beans";
        sendState();
        return(mState);
      } else if (state=="C045") {
        debug("STATE: tray is missing");
        mState="no_tray";
        sendState();
        return(mState);
      } else if (state=="C444") {
        debug("STATE: tray is missing, out of water");
        mState="no_tray|no_water";
        sendState();
        return(mState);
      } else if (state=="4205") {
        debug("STATE: pomace full");
        mState="pomace_full";
        sendState();
        return(mState);
      } else if (state=="C105") {
        debug("STATE: tray is full");
        mState="tray_full";
        sendState();
        return(mState);
      } else if (state=="4005") {
        debug("STATE: on");
        mState="on";
        sendState();
        return(mState);
      } else {
        debug("STATE UNKNOWN: "+state+"/"+answer);
        mState="unknown :: "+state+"/"+answer;
        sendState();
        return(mState);
      }
      break;
    case E50:
      toMachine("RR:20");
      delay(50);
      answer=fromMachine().substring(3,11);
      state=answer.substring(0,2)+answer.substring(6,8);
      if (state=="0000") {
        debug("STATE: off");
        return("off");
      } else if (state=="0101") {
        debug("STATE: ready");
        return("ready");
      } else if (state=="1111") {
        debug("STATE: cleaning");
        return("cleaning");
      } else if (state=="4000") {
        debug("STATE: flushing");
        return("flushing");
      } else if (state=="8180") {
        debug("STATE: out of water");
        return("no_water");
      } else if (state=="9190") {
        debug("STATE: out of water");
        return("no_water");
      } else if (state=="4040") {
        debug("STATE: need flushing");
        return("need_flushing");
      } else if (state=="0505") {
        debug("STATE: need powder");
        return("no_powder");
      } else if (state=="2120") {
        debug("STATE: ??? full (2120)");
        return("??2_full");
      } else if (state=="3130") {
        debug("STATE: ??? full (3130)");
        return("??3_full");
       } else {
        debug("STATE UNKNOWN: "+state+"/"+answer);
        return("unknown");
      }
      break;
    default:
      return("unknown");
      break;
  }
}

void testCommand() {
  String outputString = server.arg(0);

  toMachine(outputString);
  delay(50);  // wait for answer?
  String inputString = fromMachine();

  delay(100);

  server.send(200, "text/plain", inputString);//a html_message);
}

void light1() {
  long int value = 0;
  value=server.arg(0).toInt();

  analogWrite(D1,value);
  server.send(200, "text/plain", "OK");//a html_message);
}

void light2() {
  long int value = 0;
  value=server.arg(0).toInt();

  analogWrite(D2,value);
  server.send(200, "text/plain", "OK");//a html_message);
}

/*
 * Communication with machine END
 */


void setup() {
  // serial communication with the machine
  mySerial.begin(9600);
  // serial communication for debugging
  Serial.begin(115200);

  debug("Controller is starting...");
  debug("ChipID is "+String(ESP.getChipId()));
  // establish WLAN connection
  if (ip!=0) {
    debug("Static IP configuration");
    if (dns!=0) {
      WiFi.config(ip,gateway,mask,dns);
    } else {
      WiFi.config(ip,gateway,mask);
    }
  } else {
    debug("DHCP configured");
  }
  debug("Connecting to " + String(ssid),false);
  WiFi.begin(ssid,passphrase);
  while (WiFi.status()!=WL_CONNECTED) {
    delay(100);
    debug(".",false);
  }
  debug();
  debug("Connected...");

  // disable SoftAP
  WiFi.mode(WIFI_STA);
  
  ip=WiFi.localIP();
  mask=WiFi.subnetMask();
  gateway=WiFi.gatewayIP();
  debug();
  debug("IP     : " + ip.toString());
  debug("Netmask: " + mask.toString());
  debug("Gateway: " + gateway.toString());

  if (NTPserverIP.toString()=="0.0.0.0") {
    debug("NTPname: " + String(NTPserverName) + " (resolving)");
    WiFi.hostByName(NTPserverName,NTPserverIP);
  }
  debug("NTP IP : " + NTPserverIP.toString());
  debug();
  debug("Establishing UDP socket for NTP communication, syncing time");
  udp.begin(1230);
  setSyncProvider(getNTPtime);
  debug("Will perform a new NTP query every hour/2");
  setSyncInterval(1800000);
  debug("Local time is now: ",false);
  debug(date2text()+String(", ")+time2text());

  debug();
  if (mdns!="" && MDNS.begin(mdns)) {
    debug("Established mDNS responder for ("+String(mdns)+")");
    MDNS.addService("http","tcp",80);
  } else {
    debug("mDNS responder not established");
  }

  debug("Starting Web server");
  httpUpdater.setup(&server);
  server.begin();
  server.on("/",web_root);
  server.on("/reset",web_reset);
  server.on("/restart",web_restart);
  server.on("/debug",web_debug);
  server.on("/proffer",web_proffer);
  server.on("/flush",web_flush);
  server.on("/power",web_power);
  server.on("/command",testCommand);
  server.on("/light1",light1);
  server.on("/light2",light2);

  debug();
  if (udpRemotePort==0 || udpRemoteAddress=="") {
    debug("No UDP listener configured");
  } else {
    debug("Establishing socket for UDP listener");
    Sudp.begin(1231);
  }

  debug();
  debug("Restarting controller every night at 01:30");
  Alarm.alarmRepeat(01,30,0,restart);

  pinMode(led,OUTPUT);
  debug();
  debug("Setup done...");
  switch (type) {
    case NONE:
      debug("CONTROLLER IS CURRENTLY RUNNING IN TESTMODE - communication with machine disabled");
      for(uint8_t i=0; i<10; i++) {
        digitalWrite(led,LOW);
        delay(50);
        digitalWrite(led,HIGH);
        delay(50);
      }
      break;
    case UNSUPPORTED:
      debug("Unsupported machine type, won't read machine status");
      break;
    case C5:
      debug("Machine type set to C5");
      break;
    case S90:
      debug("Machine type set to S90");
      break;
    case E50:
      debug("Machine type set to E50");
      break;
  }
}

void loop() {
  server.handleClient();
  Alarm.delay(0);
  //status=fromMachine();
  if (millis()-lastRun>cycle*1000) {
    lastRun=millis();
//    debug(String(lastRun));
    readState();
  }
}

/*
 * known things
 * 
 * S90:
 * -RR:03
 * --00A :: machine is off, change filter
 * --8105/rr:1408E0000000E6000901001056A8000A :: schale leeren, grad angeschaltet
 * --C105/rr:040CE000000051000911001056A8000A :: schale leeren, laeuft ein paar minuten
 * --0404E0000000DB00090E000050A8000A :: need flushing
 * --0404E0000000BD000914000050A8000A
 * --0404E0000000A8000903000050A8000A
 * --0404E00000008E00090D000050A8000A
 * --0404E000000095000911000050A8000A
 * --0005/rr:1400E000000086000912000056A8000A :: bereit, Filter wechseln, grad eingeschaltet (sonderbarerweise ohne Spuelzwang)
 * --0005/rr:0C00E0000000E000090A000056A8000A
 * --0005/rr:1400E000000005000901000056A8000A
 * --0005/rr:0C00E000000025000904000056A8000A
 * --0005/rr:0C00E0000000E000090A000056A8000A
 * --0005/rr:1400E000000005000901000056A8000A
 * --0005/rr:0C00E000000025000904000056A8000A
 * --:130/ok:rr:0004000000006D00011300005600000A :: danach wieder abgeschaltte
 * --14:38:35 :: STATE UNKNOWN: 4205/rr:0424200000006D000D12002050A8000A :: Trester leeren
 * 14:39:03 :: STATE UNKNOWN: 4205/rr:042420000000A9000D0B002050A8000A
 * 14:39:36 :: STATE UNKNOWN: 4205/rr:0424200000002D000D0D002050A8000A
 * 14:40:03 :: STATE UNKNOWN: 4205/rr:04242000000054000D13002050A8000A
 * 14:41:03 :: STATE UNKNOWN: 4205/rr:042420000000FF000D07002050A8000A
 * --14:51:05 :: STATE UNKNOWN: C245/rr:042C20000000CC000D0B002450A8000A :: Schublade entnommen um Trester zu leeren
 * --12:39:02 :: STATE UNKNOWN: C245/rr:042C20000000CE000D0D002450A8000A :: Schublade entnommen um Trester zu leeren
 * --14:14:02 :: STATE UNKNOWN: C105/rr:040C200000003D000D0D001050A8000A :: Schale leeren
 * 14:15:02 :: STATE UNKNOWN: C105/rr:040C20000000E8000D01001050A8000A
 * 14:16:02 :: STATE UNKNOWN: C105/rr:040C2000000093000D0A001050A8000A
 * 14:17:02 :: STATE UNKNOWN: C105/rr:040C200000003E000D12001050A8000A
 * 14:18:02 :: STATE UNKNOWN: C105/rr:040C20000000E9000D06001050A8000A
 * --8404/rr:1408E000000074000911004046A8000A :: kein Wasser, grad angeschaltet
 * --0005/rr:1400E00000006600090A00005EA8000A :: Wasser, grad angeschaltet
 * 0005/rr:1400E0000000AA00090D00005EA8000A
 * 0005/rr:1400E0000000F800091000005EA8000A
 * 0005/rr:0C00E0000000FA00090600005EA8000A
 * --rr:060C200000007A0004130A4044A8000A :: Spuelen beim Abschalten
 * rr:060C200000007A0004130A4044A8000A
 * rr:060C200000007A0004130A4044A8000A
 * --4805/rr:0404E0000000B2000D09008054A8000A :: Bohnen fuellen
 * 4805/rr:1404E00000002A000D11008054A8000A
 * 4805/rr:1404E000000019000D0E008054A8000A
 * 4805/rr:0404E0000000E5000D00008054A8000A
 * --8404/rr:1408E00001001D00090B00404000000A :: Angeschaltet, kein Wasser
 * 8404/rr:0408E0000000E700090800404000000A
 * gefolgt von "no_water" :: das bedeutet dann: an, kein Wasser, muss Spuelen
 * --8005/rr:0408E0C000007300000B00005C00000A :: Entkalkt
 * 8005/rr:0408E0C000001E00000B00005C00000A
 * 8005/rr:0408E0C00000C800000B00005C00000A
 * 8005/rr:0408E0C00000B100000700005C00000A
 * 
 * 
 * 
 */
