/*********
  DIY Catan Board
  Webserver Template: Rui Santos
  Catan Features by: Jacob Abbott
*********/

#include <ESP8266WiFi.h>
#include <Adafruit_NeoPixel.h>

// Replace with your network credentials
const char* ssid     = "idkaskmark";
const char* password = "idkasksam";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1:
#define LED_PIN D1
 
// How many NeoPixels are attached to the Arduino?
#define LED_COUNT 50

//initialize light strip
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_RGB + NEO_KHZ400);


//Black (Desert) = 0
//Blue (Ore) = 1
//Red (Brick) = 2
//Orange (Wheat) =3
//Green (Wood) = 4
//Light Green (Sheep) = 5

//set colors
uint32_t black = strip.Color(0, 0, 0);
uint32_t blue = strip.Color(0, 0, 255);
uint32_t orange = strip.Color(252, 123, 3);
uint32_t red = strip.Color(255, 0, 0);
uint32_t green = strip.Color(0, 255, 0);
uint32_t white = strip.Color(255, 255, 255);
uint32_t violet = strip.Color(252, 3, 244);

//token order
int tokenOrder[18] = { 5, 2, 6, 3, 8, 10, 9, 12, 11, 4, 8, 10, 9, 4, 5, 6, 3, 11};

//token order
int tokenJumpOrder[12] = { 13, 13, 14, 14, 15, 15, 16, 16, 17, 17, 12, 12 };

//resource options
int resourceOptions[19] = { 0, 1, 1, 1, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5};

// Hex tokens (-1 for desert)
int hexTokens[19];

// Hex colors
int hexColors[19];

// Roll Stats
int rollStats[13];

// Roll Stats
int resourceStats[6];

// Last Roll Result
int currentRoll;

//color index options
uint32_t colorIndex[6] = { black, blue, orange, red, green, white };
String colorNameIndex[6] = { "black", "blue", "orange", "red", "green", "white" };

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

long randNumber;

void setup() {
  Serial.begin(74880);
  randomSeed(analogRead(0));

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  server.begin();
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop(){
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    currentTime = millis();
    previousTime = currentTime;
    while (client.connected() && currentTime - previousTime <= timeoutTime) { // loop while the client's connected
      currentTime = millis();         
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // Respond to requests
            if (header.indexOf("GET /init-board") >= 0) {
              Serial.println("initiating new board...");
              initBoard();

            } else if (header.indexOf("GET /roll-dice") >= 0) {
              Serial.println("Rolling dice...");
              rollDice();

            } else if (header.indexOf("GET /rainbow") >= 0) {
              Serial.println("setting rainbow pattern...");
              theaterChaseRainbow(50);
            } 
            
            // Display the HTML web page
            // boilerplate and css
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #77878A;}</style></head>");
            client.println("<body><h1>Catan Board Controller</h1>");
            
            // Buttons
            client.println("<p>Randomize Board</p>");
            client.println("<p><a href=\"/init-board\"><button class=\"button button2\">Randomize</button></a></p>");
               
            client.println("<p>Roll Dice</p>");
            client.println("<p><a href=\"/roll-dice\"><button class=\"button button2\">Roll</button></a></p>");
 
       client.println("<p>Rainbow Pattern</p>");
            client.println("<p><a href=\"/rainbow\"><button class=\"button button2\">Rainbow</button></a></p>");

            client.println("<p>Current Roll</p>");
            client.println("<p><button class=\"button button2\">"+ String(currentRoll) +"</button></p>");


            client.println("<p>Roll Stats</p>");
            client.println("<ol type = \"1\" start=2>");
            client.println("<li>"+ String(rollStats[2]) +"</li>");
            client.println("<li>"+ String(rollStats[3]) +"</li>");
            client.println("<li>"+ String(rollStats[4]) +"</li>");
            client.println("<li>"+ String(rollStats[5]) +"</li>");
            client.println("<li>"+ String(rollStats[6]) +"</li>");
            client.println("<li>"+ String(rollStats[7]) +"</li>");
            client.println("<li>"+ String(rollStats[8]) +"</li>");
            client.println("<li>"+ String(rollStats[9]) +"</li>");
            client.println("<li>"+ String(rollStats[10]) +"</li>");
            client.println("<li>"+ String(rollStats[11]) +"</li>");
            client.println("<li>"+ String(rollStats[12]) +"</li>");
            client.println("</ol>");

            client.println("<p>Resource Stats</p>");
            client.println("<ul>");
            client.println("<li> Ore: "+   String(resourceStats[1]) +"</li>");
            client.println("<li> Brick: "+ String(resourceStats[2]) +"</li>");
            client.println("<li> Wheat: "+ String(resourceStats[3]) +"</li>");
            client.println("<li> Wood: "+ String(resourceStats[4]) +"</li>");
            client.println("<li> Sheep: "+ String(resourceStats[5]) +"</li>");
            client.println("</ul>");
 
            client.println("</body></html>");
            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}

// Rainbow-enhanced theater marquee. Pass delay time (in ms) between frames.
void theaterChaseRainbow(int wait) {
  int firstPixelHue = 0;     // First pixel starts at red (hue 0)
  for(int a=0; a<30; a++) {  // Repeat 30 times...
    for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...
      strip.clear();         //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in increments of 3...
      for(int c=b; c<strip.numPixels(); c += 1) {
        // hue of pixel 'c' is offset by an amount to make one full
        // revolution of the color wheel (range 65536) along the length
        // of the strip (strip.numPixels() steps):
        int      hue   = firstPixelHue + c * 65536L / strip.numPixels();
        uint32_t color = strip.gamma32(strip.ColorHSV(hue)); // hue -> RGB
        strip.setPixelColor(c, color); // Set pixel 'c' to value 'color'
      }
      strip.show();                // Update strip with new contents
      delay(wait);                 // Pause for a moment
      firstPixelHue += 65536 / 90; // One cycle of color wheel over 90 frames
    }
  }
}

void shuffleArray(int arr[] , int n) {
  int temp = 0;
  int ridx = n-1;

  for(int j=(n-1); j>0; j--)// one pass through array
  {
    randNumber = random(100);
    ridx = randNumber%(j+1);// index = 0 to j
    temp = arr[ridx];// temp val set to random index val
    arr[ridx] = arr[j];// random index set to end val
    arr[j] = temp;// selected element moved to end. This value is final
  }
}

void initBoard(){
  // resetStats
  currentRoll = 0;
  for(int i= 0; i < 13; i++){
    rollStats[i] = 0;
  }
  for(int i= 0; i < 6; i++){
    resourceStats[i] = 0;
  }
  
  //copy starting order from template then randomize resource order
  for( int i = 0 ; i < 19 ; ++i ){
    hexColors[i] = resourceOptions[i];
  }
  shuffleArray(hexColors, 19);

  //set colors
  int lightIndex = 0;
  for(int i=0; i<19; i++){
    strip.setPixelColor(lightIndex, colorIndex[hexColors[i]]);
    strip.setPixelColor(lightIndex+1, colorIndex[hexColors[i]]);
    lightIndex+=2;
    strip.show();
  }

  // set j as a random outer ring start hex
  int startHex = random(11);
  int j = startHex;
  // k to track which token we're on
  int k = 0;
  for(int i=0; i<12; i++){
    if(j >11){
      j = 0;
    }
    Serial.println("setting hex:" +String(j)+" to token:"+ String(tokenOrder[k]) + " to color "+ String(colorNameIndex[hexColors[j]])); 
    if(hexColors[i] != 0){
      hexTokens[j] = tokenOrder[k]; 
      j++;
      k++;
    }else{
      hexTokens[j] = -1; 
      j++;
    }
  }
  // set j as the next inner ring hex
  j = tokenJumpOrder[j-1];
  for(int i=12; i<18; i++){
    if(j > 17){
      j = 12;
    }
    Serial.println("setting hex:" +String(j)+" to token:"+ String(tokenOrder[k]) + " to color "+ String(colorNameIndex[hexColors[j]])); 
    if(hexColors[i] != 0){
      hexTokens[j] = tokenOrder[k]; 
      j++;
      k++;
    }else{
      hexTokens[j] = -1; 
      j++;
    }
  }
  //set center token
  if(hexColors[18] != 0){
    hexTokens[18] = tokenOrder[17];
  }else{
    hexTokens[18] = -1;
  }
  Serial.println("setting hex:" +String(18)+" to token:"+ String(tokenOrder[17]) + " to color "+ String(colorNameIndex[hexColors[18]])); 

  // flash starting token
  int startTokenArr[1] = { startHex };
  flashHexArr(startTokenArr, 1, 5);
}

void rollDice(){
  //rolls 2 die values and sums them
  //records resource and roll stat
  //flashes hexes with rolled value

  int diceRollA = random(5) + 1;
  int diceRollB = random(5) + 1;
  int rollResult = diceRollA + diceRollB;
  rollStats[rollResult]++;
  currentRoll = rollResult;

  int resultIndex = 0;
  if(rollResult == 2 || rollResult == 12){
    int rollOptions[1];
    for(int i=0; i<18; i++){
      if(hexTokens[i] == rollResult){
        rollOptions[resultIndex] = i;
        resultIndex++;
        resourceStats[hexColors[i]]++;
      }
    }
    flashHexArr(rollOptions, 1, 15);
  }else if (rollResult == 7){
    int allIndices[19] = {};
    for(int x = 0; x <19; x++){
      allIndices[x] = x;
    }
    flashHexArr(allIndices, 19, 5);
  } else {
    int rollOptions[2] ;
    for(int i=0; i<19; i++){
      if(hexTokens[i] == rollResult){
        rollOptions[resultIndex] = i;
        resultIndex++;
        resourceStats[hexColors[i]]++;
      }
    }
    flashHexArr(rollOptions, 2, 15);
  }
}

void flashHexArr(int hexIndices[] , int size, int time)
{
  // each loop is 1 second, repeat for {time} seconds
  for(int t=0; t < time; t++){
    //set each to violet
    for(int i=0; i < size; i++){
      int index = hexIndices[i];
      int lightIndex = index * 2;

      strip.setPixelColor(lightIndex, black);
      strip.setPixelColor(lightIndex+1, black);
    }
    strip.show();
    // hold for .25 seconds
    delay(250);

    // set each to original color
    for(int i=0; i < size; i++){
      int index = hexIndices[i];
      int lightIndex = index * 2;

      strip.setPixelColor(lightIndex, colorIndex[hexColors[index]]);
      strip.setPixelColor(lightIndex+1, colorIndex[hexColors[index]]);
    }
    strip.show();
    // hold for .75 seconds
    delay(750); 
  }
}
