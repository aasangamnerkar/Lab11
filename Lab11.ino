SYSTEM_THREAD(ENABLED);
#include "LIS3DH.h"
#include "blynk.h"
#include "oled-wing-adafruit.h"

OledWingAdafruit display;
LIS3DHSPI accel(SPI, D3, WKP);

const unsigned long PRINT_SAMPLE_PERIOD = 100;
unsigned long lastPrintSample = 0;
double avg_x = 0, avg_y = 0, avg_z = 0;
int sum_x = 0, sum_y = 0, sum_z = 0;
int COUNTER = 100;

// setup() runs once, when the device is first turned on.
void setup() {
  //init pinmode
  pinMode(D3, INPUT);
  

  // init display values
  display.setup();
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println();
  display.display();

  //set up blynk
  Blynk.begin("cnjXDn_t7vcs3I-PR-jbSobGd__j2qM9", IPAddress(167, 172, 234, 162), 9090);
  Blynk.run();

  Serial.begin(9600);

	// Initialize sensors
	LIS3DHConfig config;
	config.setAccelMode(LIS3DH::RATE_100_HZ);

	bool setupSuccess = accel.setup(config);
	Serial.printlnf("setupSuccess=%d", setupSuccess);

  for (int i = 0; i < COUNTER; i++)
  {
    if (millis() - lastPrintSample >= PRINT_SAMPLE_PERIOD) {
      lastPrintSample = millis();

      LIS3DHSample sample;
      if (accel.getSample(sample)) {
        String output = "";
        output += (String) (sample.x - avg_z);
        output += "  ";
        output += (String)  (sample.y - avg_y);
        output += " ";
        output += (String) (sample.z - avg_z);

        sum_x += sample.x;
        sum_y += sample.y;
        sum_z += sample.z;

        Blynk.virtualWrite(V0, sample.x- avg_x);
        Blynk.virtualWrite(V1, sample.y - avg_y);
        Blynk.virtualWrite(V2, sample.z - avg_z);
      }
      else {
        Serial.println("no sample");
      }
    }
  }
  avg_x = sum_x / COUNTER;
  avg_y = sum_y / COUNTER;
  avg_z = sum_z / COUNTER;
}

// loop() runs over and over again, as quickly as it can execute.
void loop() {

  display.loop();
  if (millis() - lastPrintSample >= PRINT_SAMPLE_PERIOD) {
		lastPrintSample = millis();

		LIS3DHSample sample;
		if (accel.getSample(sample)) {
      String output = "";
      output += (String) (sample.x - avg_z);
      output += "  ";
      output += (String)  (sample.y - avg_y);
      output += " ";
      output += (String) (sample.z - avg_z);

			//Serial.println(output);

      Blynk.virtualWrite(V0, sample.x- avg_x);
      Blynk.virtualWrite(V1, sample.y - avg_y);
      Blynk.virtualWrite(V2, sample.z - avg_z);
    }
		else {
			Serial.println("no sample");
		}
	}
}
