#include <Mouse.h>

// Pin definitions
const int horizPin = A1;
const int vertPin = A2;

// Sensitivity to mouse movement
const int sensitivity = 1; // Adjust this value to change how fast the mouse moves

// Screen resolution (example values)
const int screenWidth = 1920;
const int screenHeight = 1080;

int arrayH[200];
int arrayV[200];
int index_arrayH = 0;
int index_arrayV = 0;

void setup() {
  // Initialize serial communication for debugging (optional)
  Serial.begin(9600);
  
  // Initialize the Mouse library
  Mouse.begin();
  for (int i = 0; i<200; i++){
    arrayH[i] = 0;
    arrayV[i] = 0;
  }
  delay(3000);

  // Move the cursor to the middle of the screen initially
  Mouse.move(screenWidth / 2, screenHeight / 2, 0);
 
}

void loop() {
  // Read the voltages from A1 and A2
  int horizReading = analogRead(horizPin);
  int vertReading = analogRead(vertPin);
  arrayH[index_arrayH] = horizReading;
  index_arrayH = (index_arrayH + 1)%200;
  arrayV[index_arrayV] = vertReading;
  index_arrayV=(index_arrayV + 1)%200;
  // Map the readings to screen coordinates
  int movingavrH=0;;
  int movingavrV=0;

  for(int i=0 ;i<200;i++){
    movingavrH=movingavrH+arrayH[i];
    movingavrV=movingavrV+arrayV[i];
    }
  horizReading=movingavrH/200;
  vertReading=movingavrV/200;
  int horizCoord = map(horizReading, 0, 1023, 0, screenWidth)*3;
  int vertCoord = map(vertReading, 0, 1023, 0, screenHeight)*3;

  // Debugging output (optional)
  Serial.print("Horiz Reading: ");
  Serial.print(horizReading);
  Serial.print(", Vert Reading: ");
  Serial.print(vertReading);
  Serial.print(" -> Horiz Coord: ");
  Serial.print(horizCoord);
  Serial.print(", Vert Coord: ");
  Serial.println(vertCoord);

  // Move the mouse to the mapped coordinates
  // Since Mouse.move() only moves the cursor relatively, we need a different approach
  // Unfortunately, the standard Arduino Mouse library does not support absolute positioning directly
  // As a workaround, we move the cursor relatively in small steps to simulate absolute positioning
  static int prevHorizCoord = screenWidth / 2;
  static int prevVertCoord = screenHeight / 2;

  int horizMove = horizCoord - prevHorizCoord;
  int vertMove = vertCoord - prevVertCoord;

  Mouse.move(horizMove, vertMove);

  prevHorizCoord = horizCoord;
  prevVertCoord = vertCoord;

  // Small delay to allow time for the next reading
  delay(10);
}
