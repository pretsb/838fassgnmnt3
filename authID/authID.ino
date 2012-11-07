//Magnetic Stripe Reader hooked to an arduino to authenticate users

//Magnetic Stripe Reader code taken from http://www.instructables.com/id/Arduino-magnetic-stripe-decoder/

#include <WiFi.h>
int cld1Pin = 5;            // Card status pin
int rdtPin = 2;             // Data pin
int reading = 0;            // Reading status
volatile int buffer[400];   // Buffer for data
volatile int i = 0;         // Buffer counter
volatile int bit = 0;       // global bit
char cardData[50];          // holds card info
int charCount = 0;          // counter for info
int DEBUG = 0;

char ssid[] = "CMSC838F";     // the name of your network
char pass[]="Hackerspace";
int status = WL_IDLE_STATUS;     // the Wifi radio's status
 
void setup() {
  Serial.begin(9600);
  Serial.print("Attempting to connect to open network: ");
  Serial.println(ssid);
  status = WiFi.begin(ssid,pass);
  if ( status != WL_CONNECTED) { 
    Serial.println("Couldn't get a wifi connection");
    while(true);
  } 
  else {
      Serial.print("You're connected to the network");
  }
  
  attachInterrupt(0, changeBit, CHANGE);
  attachInterrupt(1, writeBit, FALLING);
}
 
void loop(){
 
  while(digitalRead(cld1Pin) == LOW){
    reading = 1;
  }     
 
  if(reading == 1) {  
 
    if (DEBUG == 1) {
      printBuffer();
    }
 
    decode();
    reading = 0;
    i = 0;
 
    int l;
    for (l = 0; l < 40; l = l + 1) {
      cardData[l] = '\n';
    }
 
    charCount = 0;
  }
}
 
void changeBit(){
  if (bit == 0) {
    bit = 1;
  } else {
    bit = 0;
  }
}
 
void writeBit(){
  buffer[i] = bit;
  i++;
}
 
void printBuffer(){
  int j;
  for (j = 0; j < 200; j = j + 1) {
    Serial.println(buffer[j]);
  }
}
 
int getStartSentinal(){
  int j;
  int queue[5];
  int sentinal = 0;
 
  for (j = 0; j < 400; j = j + 1) {
    queue[4] = queue[3];
    queue[3] = queue[2];
    queue[2] = queue[1];
    queue[1] = queue[0];
    queue[0] = buffer[j];
 
    if (DEBUG == 1) {
      Serial.print(queue[0]);
      Serial.print(queue[1]);
      Serial.print(queue[2]);
      Serial.print(queue[3]);
      Serial.println(queue[4]);
    }
 
    if (queue[0] == 0 & queue[1] == 1 & queue[2] == 0 & queue[3] == 1 & queue[4] == 1) {
      sentinal = j - 4;
      break;
    }
  }
 
  if (DEBUG == 1) {
    Serial.print("sentinal:");
    Serial.println(sentinal);
    Serial.println("");
  }
 
  return sentinal;
}
 
void decode() {
  int sentinal = getStartSentinal();
  int j;
  int i = 0;
  int k = 0;
  int thisByte[5];
 
  for (j = sentinal; j < 400 - sentinal; j = j + 1) {
    thisByte[i] = buffer[j];
    i++;
    if (i % 5 == 0) {
      i = 0;
      if (thisByte[0] == 0 & thisByte[1] == 0 & thisByte[2] == 0 & thisByte[3] == 0 & thisByte[4] == 0) {
        break;
      }
      printMyByte(thisByte);
    }
  }
 
  Serial.print("Stripe_Data:");
  String card = "";
  for (k = 0; k < charCount; k = k + 1) {
    Serial.print(cardData[k]);
    card += cardData[k];
  }
  Serial.println("");
 
  byte server[] = { 128,8,126,40 }; // Test server
  WiFiClient client;
  
  if (client.connect(server, 8080)) {
	    Serial.println("connected");
            Serial.println(card);
            String post = "POST /DOTSServer/authID?checkID=" + card;
            client.println(post);
	    client.println(" HTTP/1.1");
            client.println("Host: mind6.cs.umd.edu");
            client.println("Accept: text/html");
            boolean startRead=false;
            delay(500);
            Serial.println("Receiving data from server");
            while (client.connected()) {
              char c = client.read();
              if(c=='<')
                startRead=true;
              if(startRead==true){
                Serial.print(c);
                if(c=='U')
                {
                  startRead==false;
                  tone(6,200,600);
                  delay(200); 
                  tone(6,200,600);
                  delay(200);                  
                  tone(6,200,600);
                  delay(200); 
                }                
                else if (c=='A')
                {
                  startRead==false;
                  tone(6,200,200);
                  delay(200);
                }
              }
              if(c=='>') 
                startRead=false;
            }
  
  }
  client.stop();
  if (!client.connected()) {}
 
}
 
void printMyByte(int thisByte[]) {
  int i;
  for (i = 0; i < 5; i = i + 1) {
    if (DEBUG == 1) {
      Serial.print(thisByte[i]);
    }
}
    if (DEBUG == 1) {
      Serial.print("\t");
      Serial.print(decodeByte(thisByte));
      Serial.println("");
    }
 
    cardData[charCount] = decodeByte(thisByte);
    charCount ++;
}
 
char decodeByte(int thisByte[]) {
    if (thisByte[0] == 0 & thisByte[1] == 0 & thisByte[2] == 0 & thisByte[3] == 0 & thisByte[4] == 1){
      return '0';
    }
    if (thisByte[0] == 1 & thisByte[1] == 0 & thisByte[2] == 0 & thisByte[3] == 0 & thisByte[4] == 0){
      return '1';
    }
 
    if (thisByte[0] == 0 & thisByte[1] == 1 & thisByte[2] == 0 & thisByte[3] == 0 & thisByte[4] == 0){
      return '2';
    }
 
    if (thisByte[0] == 1 & thisByte[1] == 1 & thisByte[2] == 0 & thisByte[3] == 0 & thisByte[4] == 1){
      return '3';
    }
 
    if (thisByte[0] == 0 & thisByte[1] == 0 & thisByte[2] == 1 & thisByte[3] == 0 & thisByte[4] == 0){
      return '4';
    }
 
    if (thisByte[0] == 1 & thisByte[1] == 0 & thisByte[2] == 1 & thisByte[3] == 0 & thisByte[4] == 1){
      return '5';
    }
 
    if (thisByte[0] == 0 & thisByte[1] == 1 & thisByte[2] == 1 & thisByte[3] == 0 & thisByte[4] == 1){
      return '6';
    }
 
    if (thisByte[0] == 1 & thisByte[1] == 1 & thisByte[2] == 1 & thisByte[3] == 0 & thisByte[4] == 0){
      return '7';
    }
 
    if (thisByte[0] == 0 & thisByte[1] == 0 & thisByte[2] == 0 & thisByte[3] == 1 & thisByte[4] == 0){
      return '8';
    }
 
    if (thisByte[0] == 1 & thisByte[1] == 0 & thisByte[2] == 0 & thisByte[3] == 1 & thisByte[4] == 1){
      return '9';
    }
 
    if (thisByte[0] == 0 & thisByte[1] == 1 & thisByte[2] == 0 & thisByte[3] == 1 & thisByte[4] == 1){
      return 'A';
    }
 
    if (thisByte[0] == 1 & thisByte[1] == 1 & thisByte[2] == 0 & thisByte[3] == 1 & thisByte[4] == 0){
      return 'B';
    }
 
    if (thisByte[0] == 0 & thisByte[1] == 0 & thisByte[2] == 1 & thisByte[3] == 1 & thisByte[4] == 1){
      return 'C';
    }
 
    if (thisByte[0] == 1 & thisByte[1] == 0 & thisByte[2] == 1 & thisByte[3] == 1 & thisByte[4] == 0){
      return 'D';
    }
 
    if (thisByte[0] == 0 & thisByte[1] == 1 & thisByte[2] == 1 & thisByte[3] == 1 & thisByte[4] == 0){
      return 'E';
    }
 
    if (thisByte[0] == 1 & thisByte[1] == 1 & thisByte[2] == 1 & thisByte[3] == 1 & thisByte[4] == 1){
      return 'F';
    }
}

