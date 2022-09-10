#include <Servo.h>

int pos;
int x;
int ideal_bottom_pos = 0;
int ideal_top_pos = 0;
float intensity_readings[14];
int degree_array[14];

#define bottom_servo_pin 2
#define top_servo_pin 3
#define bottom_panel_servo_pin 4
#define top_panel_servo_pin 5

#define ldr1 A0
#define ldr2 A1
#define ldr3 A2

Servo bottom_servo;
Servo top_servo;  
Servo bottom_panel_servo;
Servo top_panel_servo;
Servo servo;

void setup() {
  Serial.begin(9600);
  
  bottom_servo.attach(bottom_servo_pin); 
  top_servo.attach(top_servo_pin);
  bottom_panel_servo.attach(bottom_panel_servo_pin);
  top_panel_servo.attach(top_panel_servo_pin);

  bottom_panel_servo.write(100);
  bottom_servo.write(30);
}

void loop() {
  
  light_intensity();            
  //seasons_routine();
  
}

void sweep(int servo_pin, int start_pos, int end_pos){   // Sweep Function
  servo.attach(servo_pin);

  for (pos = start_pos; pos <= end_pos; pos += 1) {
    servo.write(pos);             
    delay(10);                       
  }
  for (pos = end_pos; pos >= start_pos; pos -= 1) {
    servo.write(pos);             
    delay(10);                       
  }
  servo.detach();
}

void light_intensity(){   // Function to read light intensity from LDR
  pos = 30;
  
  for (x=0; x<=14; x+=1){
    top_servo.write(pos);

    delay(400);
    
    int ldr1_intensity = analogRead(ldr1);
    int ldr2_intensity = analogRead(ldr2);
    int ldr3_intensity = analogRead(ldr3);

    Serial.print("Angle: ");
    Serial.print(pos);
    Serial.print("  LDR 1: ");
    Serial.print(ldr1_intensity);
    Serial.print("  LDR 2: ");
    Serial.print(ldr2_intensity);
    Serial.print("  LDR 3: ");
    Serial.print(ldr3_intensity);
  
    float average_intensity = (ldr1_intensity + ldr2_intensity + ldr3_intensity)/3;
  
    Serial.print("  Average Intensity: ");
    Serial.println(average_intensity);
  
    intensity_readings[x] = average_intensity;
    degree_array[x] = pos;

    pos += 10;
  }

  float max_intensity_val = 0;
  int max_angle_val = 30;
  
  for (x = 0; x < (sizeof(intensity_readings) / sizeof(intensity_readings[0])); x++) {
    if (intensity_readings[x] > max_intensity_val) {
       max_intensity_val = intensity_readings[x];
       max_angle_val = 30+(x*10);
    }
  }

  Serial.print("MAXIMUM INTENSITY: ");
  Serial.print(max_intensity_val);
  Serial.print("  MAXIMUM ANGLE: ");
  Serial.println(max_angle_val);

  delay(1000);

  top_panel_servo.write(max_angle_val);

  delay(2000);
}

void seasons_routine() {
  top_panel_servo.write(30);
  
  for (x = 30; x <= 150; x += 20) {
    top_panel_servo.write(x);
    for (pos = 30; pos <= 150; pos += 1) { // goes from 0 degrees to 180 degrees
      // in steps of 1 degree
      bottom_panel_servo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(25);                       // waits 15 ms for the servo to reach the position
    }
    for (pos = 150; pos >= 30; pos -= 1) { // goes from 180 degrees to 0 degrees
      bottom_panel_servo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(25);                       // waits 15 ms for the servo to reach the position
    }
  }
}
