#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>

/* Assign a unique ID to this sensor at the same time */
Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);


int vib_motor_pin[] =  {6, 7, 8, 9};
int vib_val[] = {0, 0, 0, 0};
float init_magtic_x;
float init_magtic_y;
float init_magtic_z;

boolean initial = true;


void setup() {
  Serial.begin( 9600 );

  //Initialise the sensor
  if (!mag.begin()) {
    // There was a problem detecting the HMC5883 ... check your connections
    Serial.println("Ooops, no HMC5883 detected ... Check your wiring!");
    while (1);
  }

  //Display some basic information on this sensor
  displaySensorDetails();

  //vib
  for (int i = 0; i < 4; i++)pinMode( vib_motor_pin[i], OUTPUT );

}

void loop() {

  //catch acceralate

  sensors_event_t event;
  mag.getEvent(&event);
  //Serial.println(1000);

  if (initial) {
    //  Serial.println(2000);

    //初期化
    init_magtic_x = event.magnetic.x;
    init_magtic_y = event.magnetic.y;
    init_magtic_z = event.magnetic.z;

    initial = false;


  }
  // Serial.println(3000);


  float accX = event.magnetic.x;
  float accY = event.magnetic.y;
  float accZ = event.magnetic.z;

  float init_angleX = atan2(init_magtic_x, init_magtic_z);
  float init_angleY = atan2(init_magtic_y, init_magtic_z);
  float init_angleZ = atan2(init_magtic_x, init_magtic_y);

  float angleX = atan2(accX, accZ) - init_angleX;
  float angleY = atan2(accY, accZ) - init_angleY;
  float angleZ = atan2(accX, accY) - init_angleZ;

  angleX = degrees(angleX);
  angleY = degrees(angleY);
  angleZ = degrees(angleZ);

  // Serial.print("angleX : "); Serial.print(angleX);
  //Serial.print(" angleZ : "); Serial.print(angleZ);



  if ( abs(angleX) > 330) angleX = 360 - abs(angleX);
  //if ( abs(angleY) > 330) angleY = 360 - abs(angleY);
  if ( abs(angleZ) > 330) angleZ = 360 - abs(angleZ);


  Serial.print("angleX : "); Serial.print(angleX);
  Serial.print(" angleY : "); Serial.print(angleY);
  Serial.print(" angleZ : "); Serial.println(angleZ);

  if ( angleY < -40 || angleY > 20) {

    if (abs(angleX) < 30 && abs(angleZ) < 25 ) {
      delay(500);
      return;
    }

    Serial.println("safe zone");
  }

  //vib move



  //持ち手左側
  if (angleX < 30) vib_val[0] = 0;
  else vib_val[0] = 100;

  //持ち手外側
  if (angleZ > -30) vib_val[1] = 0;
  else vib_val[1] = 100;

  //持ち手右側
  if (angleX > -25) vib_val[2] = 0;
  else vib_val[2] = 100;

  //持ち手内側
  if (angleZ < 30) vib_val[3] = 0;
  else vib_val[3] = 100;

  // if (angleX < -40 && angleZ > -120 && angleZ < 0) vib_val[1] = 0;
  //if (angleX < -70 && angleZ > 30) vib_val[3] = 0;



  //振動
  for (int i = 0; i < 4; i++)digitalWrite( vib_motor_pin[i], vib_val[i]);

  delay( 500 );


}

void displaySensorDetails(void)
{
  sensor_t sensor;
  mag.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" uT");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" uT");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" uT");
  Serial.println("------------------------------------");
  Serial.println("");



  delay(500);
}




