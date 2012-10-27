/*

 This example connects to an unencrypted Wifi network. 
 Then it prints the  MAC address of the Wifi shield,
 the IP address obtained, and other network details.

 Circuit:
 * WiFi shield attached

 created 13 July 2010
 by dlf (Metodo2 srl)
 modified 29 Feb 2012
 by Scott Fitzgerald
 */
 #include <WiFi.h>

char ssid[] = "tulane3422";     // the name of your network
int keyIndex=1;
char key[]="09181AF186";
int status = WL_IDLE_STATUS;     // the Wifi radio's status

void setup() {
  // initialize serial:
  Serial.begin(9600);

  // attempt to connect to an open network:
  Serial.print("Attempting to connect to open network: ");
  Serial.println(ssid);
  status = WiFi.begin(ssid,keyIndex,key);

  // if you're not connected, stop here:
  if ( status != WL_CONNECTED) { 
    Serial.println("Couldn't get a wifi connection");
    while(true);
  } 
  // if you are connected :
  else {
      Serial.print("You're connected to the network");
      //printCurrentNet();
      //printWifiData();
  }
  byte server[] = { 128,8,126,40 }; // Test server
  WiFiClient client;
  
  if (client.connect(server, 8080)) {
	    Serial.println("connected");
	    client.println("POST /DOTSServer/authID?checkID=112613457");
            //Serial.print("POST /DOTSServer/authID?checkID=111217052"); 
            client.println(" HTTP/1.1");
            client.println("Host: mind6.cs.umd.edu");
            //Serial.println("Host: mind6.cs.umd.edu");
            client.println("Accept: text/html");
            //client.println("Connection: close");
            //Serial.println("Connection: close");
            boolean startRead=false;
            delay(500);
            Serial.println("Receiving data from server");
            //Serial.println(client.available());
            //Serial.println(client.connected());
            while (client.connected()) {
              //Serial.println(client.available());
              //Serial.println(client.connected());
              char c = client.read();
              if(c=='<')
              startRead=true;
              //continue;
              if(startRead==true)
              Serial.print(c);
              if(c=='>') startRead=false;
            }
  
  }
  client.stop();
    //Serial.write(c);
   if (!client.connected()) {
	    //Serial.println();
   //Serial.println("disconnecting.");
	    
	  }
}

void loop() {
  // check the network connection once every 10 seconds:
  //delay(10000);
  //printCurrentNet();
}

void printWifiData() {
  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
    Serial.print("IP Address: ");
  Serial.println(ip);
  Serial.println(ip);

  // print your MAC address:
  byte mac[6];  
  WiFi.macAddress(mac);
  Serial.print("MAC address: ");
  Serial.print(mac[5],HEX);
  Serial.print(":");
  Serial.print(mac[4],HEX);
  Serial.print(":");
  Serial.print(mac[3],HEX);
  Serial.print(":");
  Serial.print(mac[2],HEX);
  Serial.print(":");
  Serial.print(mac[1],HEX);
  Serial.print(":");
  Serial.println(mac[0],HEX);

  // print your subnet mask:
  IPAddress subnet = WiFi.subnetMask();
  Serial.print("NetMask: ");
  Serial.println(subnet);

  // print your gateway address:
  IPAddress gateway = WiFi.gatewayIP();
  Serial.print("Gateway: ");
  Serial.println(gateway);
}

void printCurrentNet() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print the MAC address of the router you're attached to:
  byte bssid[6];
  WiFi.BSSID(bssid);    
  Serial.print("BSSID: ");
  Serial.print(bssid[5],HEX);
  Serial.print(":");
  Serial.print(bssid[4],HEX);
  Serial.print(":");
  Serial.print(bssid[3],HEX);
  Serial.print(":");
  Serial.print(bssid[2],HEX);
  Serial.print(":");
  Serial.print(bssid[1],HEX);
  Serial.print(":");
  Serial.println(bssid[0],HEX);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.println(rssi);

  // print the encryption type:
  byte encryption = WiFi.encryptionType();
  Serial.print("Encryption Type:");
  Serial.println(encryption,HEX);
}
