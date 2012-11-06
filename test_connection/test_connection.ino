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

char ssid[] = "CMSC838F";     // the name of your network
//int keyIndex=1;
char pass[]="Hackerspace";
int status = WL_IDLE_STATUS;     // the Wifi radio's status

void setup() {
  // initialize serial:
  Serial.begin(9600);

  // attempt to connect to an open network:
  Serial.print("Attempting to connect to open network: ");
  Serial.println(ssid);
  status = WiFi.begin(ssid,pass);

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
	    client.println("POST /DOTSServer/authID?checkID=111217052");
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

