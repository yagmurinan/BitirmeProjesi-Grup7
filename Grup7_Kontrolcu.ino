#include <PID_v1.h> 
#include <Servo.h>
#include "HUSKYLENS.h"
#include "SoftwareSerial.h"

SoftwareSerial BluetoothSerial(10, 11);
HUSKYLENS huskylens;

//definition of servo motors 
Servo servo_x , servo_y;

//definition of servo motors pins
int servo_x_pin = 6;
int servo_y_pin = 5;

int xy_bit = 0;

//definition of starting angle values of servo motors
#define servo_x_start 95
#define servo_y_start 95

int x_current;
int y_current;
int x_coordinate;
int y_coordinate;

double OUTPUT_X;
double OUTPUT_Y;

//PID for Servo Motor x and Servo Motor y
double p_x = 0.2369;
double i_x = 0.15;
double d_x = 0.3851;
double p_y = 0.2369;
double i_y = 0.15;
double d_y = 0.3851;
double input_x;
double input_y;
double output_x;
double output_y;
double setpoint_x = 153;
double setpoint_y = 123;

//definition of PID
PID x_Controller(&input_x, &output_x, &setpoint_x, p_x, i_x, d_x, DIRECT); //direct means the output will increase when error is positive
PID y_Controller(&input_y, &output_y, &setpoint_y, p_y, i_y, d_y, REVERSE); //reverse means the output will increase when error is negative


void setup() { 
  Serial.begin(115200);
  Wire.begin();
    while (!huskylens.begin(Wire))
    {
        delay(100);
    }
    
//attach the servo motors variable to their pins
  servo_x.attach(servo_x_pin); 
  servo_y.attach(servo_y_pin);
  
//setting the servo motors to start degree
  servo_x.write(servo_x_start);
  servo_y.write(servo_y_start);
    
//definition of PID mode and limits
  x_Controller.SetMode(AUTOMATIC);
  x_Controller.SetOutputLimits(-60, 60);
    
  y_Controller.SetMode(AUTOMATIC);
  y_Controller.SetOutputLimits(-60, 60);
}

void loop() {
  
  if (!huskylens.request()){
    servo_x.write(servo_x_start);
    servo_y.write(servo_y_start);
  }
  
  else if(!huskylens.isLearned()) {
    servo_x.write(servo_x_start);
    servo_y.write(servo_y_start);
  }
  
  else if(!huskylens.available()) {
    servo_x.write(servo_x_start);
    servo_y.write(servo_y_start);
  }
  
  else{
    HUSKYLENSResult result = huskylens.read();
//read x and y coordinates from huskylens
    x_current = result.xCenter; 
    y_current = result.yCenter; 

//assign read coordinates to PID input variable       
    input_x = double(x_current); 
    input_y = double(y_current);
    
//pid calculation based on coordinates

    int x_threshold_max_1 = setpoint_x + 8;
    int x_threshold_min_1 = setpoint_x - 8;

    int x_threshold_max_2 = setpoint_x + 30;
    int x_threshold_min_2 = setpoint_x - 30;

    int y_threshold_max_1 = setpoint_y + 8;
    int y_threshold_min_1 = setpoint_y - 8;
    
    int y_threshold_max_2 = setpoint_y + 30;
    int y_threshold_min_2 = setpoint_y - 30;
    
   
    if(x_current < x_threshold_max_1 && x_current > x_threshold_min_1){
       x_Controller.SetTunings(0.2369, 0, 0.3851);
       x_Controller.Compute();  
    }

    else if(x_current > x_threshold_max_1 && x_current < x_threshold_max_2 || x_current < x_threshold_min_1 && x_current > x_threshold_min_2){
       x_Controller.SetTunings(0.2369, 0.16, 0.3851);
       x_Controller.Compute();  
    }
    
    else{
       x_Controller.SetTunings(0.4335, 0.15, 0.245);
       x_Controller.Compute();
    }
    
    if(y_current < y_threshold_max_1 && y_current > y_threshold_min_1){
       y_Controller.SetTunings(0.2369, 0, 0.3851);
       y_Controller.Compute(); 
    }

    else if(y_current > y_threshold_max_1 && y_current < y_threshold_max_2 || y_current < y_threshold_min_1 && y_current > y_threshold_min_2){
       y_Controller.SetTunings(0.2369, 0.16, 0.3851);
       y_Controller.Compute();  
    }
    
    else{
       y_Controller.SetTunings(0.4335, 0.15, 0.245);
       y_Controller.Compute(); 
    }
  
//assign read coordinates to PID input variable       
    input_x = double(x_current); 
    input_y = double(y_current);  

//mapping coordinates for LCD display
    x_coordinate = map(x_current, 43, 263, 0, 50);
    y_coordinate = map(y_current, 23, 223, 0, 50);
    
//sending coordinates to bluetooth ardunio
    if (xy_bit == 0){
    Wire.beginTransmission(8);
    Wire.write(x_coordinate);
    Wire.endTransmission();
    Serial.println("x_coordinate");
    Serial.println(x_coordinate);
    xy_bit=1;
    }

    else if (xy_bit == 1){
    Serial.println(y_coordinate);
    y_coordinate = y_coordinate + 128;
    Wire.beginTransmission(8);
    Wire.write(y_coordinate);
    Wire.endTransmission();
    xy_bit=0;
    }     

    OUTPUT_X = output_x + servo_x_start;
    OUTPUT_Y = output_y + servo_y_start;
  
//sending angle values to motors
    servo_x.write(OUTPUT_X);                    
    servo_y.write(OUTPUT_Y);   
  }
}
