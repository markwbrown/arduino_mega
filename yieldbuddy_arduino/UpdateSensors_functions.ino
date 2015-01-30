#include <Wire.h>
/*
/!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 /!!UPDATE SENSOR VALUES!!UPDATE SENSOR VALUES!!UPDATE SENSOR VALUES!!UPDATE SENSOR VALUES!!UPDATE SENSOR VALUES!!UPDATE SENSOR VALUES!!UPDATE SENSOR VALUES!!UPDATE SENSOR VALUES!!UPDATE SENSOR VALUES!!UPDATE SENSOR VALUES!!
 /!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 */

//READ ALL SENSOR VALUES AND CONVERT FOR LCD DISPLAY
void updateSensorValues() {
 
  analogReference(DEFAULT);  //Seems more accurate.
                             //more accurate? If using analogReference, shouldn't it be 
                             //coming from an actual reference voltage? The LM4040 does 2.048 and 4.096V
                             // and if used behind an isolated power supply like the ROE-0505 and SI8600 for 
                             //I2C/UART communications, pH readings become much more accurate, stable, and readings can be taken
                             //when pumps are running, ballasts are on, and motors are operating. 
                             //i.e. pH probe mounted to linear rail for automatic calibration, measurement, and storage?
                             //that seems to be the holy grail here -- the open source equivilent of a Mettler Toledo
 /*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
   !!PH SENSORS!!PH SENSORS!!PH SENSORS!!PH SENSORS!!PH SENSORS!!PH SENSORS!!PH SENSORS!!PH SENSORS!!PH SENSORS!!
   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
  /*PH1------------------------------------------------*/
  float pH1Sum = 0;
  int j = 0;
  
  /*edited for Atlas Scientific pH chip on I2C*/
  //analogRead(pH1Pin);  //Get ADC to switch to correct pin
  /* huh?*/
  //delay(20); //Wait for Pin to Change
  
  //while(j<30) {
  //  pH1Sum = pH1Sum +
  //pH1Sum = pH1Sum + analogRead(pH1Pin);
  //  j++;
  //} 
  
  /*
  Ok, so 30 times, over the course of approximately ? seconds at 16mHZ
  the analog reading is added to itself and an average taken
  */
  //pH1RawValue = pH1Sum/30;
  
  
/* here you're multiplying the voltage by another float and subtracting yet another float.
this isn't great for precision
a better practice would be to use ints and scale your analogReads of the pH1pin to be a whole number, rather than a decimal. 
Math operations are faster, especially when using division, and the cumulative effect on the loss of precision with the way floats
are stored is eliminated. I understand the hipocracy of this with Atlas Scientific's probe returning a float, but there is a built-in delay between reading and response.
I suspect they're doing the same thing here. 
Will attempt to verify the code on their chip, though I'm betting they are going to try and be tight-lipped about it.*/

  //pH1Value = (0.0178 * pH1RawValue - 1.889);
  /*
  if pH1Value is not a number... When would it not be? PString appears to be pretty 
  //efficient at ensuring the pH1Value 
  */
  
  //if(isnan(pH1Value)){
  //  pH1Value = 0;        
  //}
  
  
  /*
  I was unfamiliar with PString, so I may not have that right after a glance at their .cpp and .h files
   Here, were're passing the address in memory of pH1 char and it's size to PString with a name of my_pH1_string
   then, if the pH value is under 10 (why 10?)empty quotation marks and then the pH value is stored. If over 10, just the pH value is stored
  */
 
 
  /*
  here's Atlas Scientific code for the pH probe
  included Wire.h here, also included in yieldbuddy_arduino
  */
  
  /*
#include <Wire.h>
*/
//define pH1address 99 //address is set on chip. Make sure to not configure other devices to run on address 99 (0x99)
char computerdata[20]; //used to hold the message sent to the Atlas chip
/*
possible messages are:
*/

byte received_from_computer=0; //used to
byte serial_event=0; //used to
byte code=0; //used to
char ph_data[20]; //used to
byte in_char=0; //used to
byte i=0; //used to
int time=1400; //used to wait 1.4 seconds between requesting data from Atlas chip to requesting response.
float ph_float;  //here, can I do int instead? Doesn't seem so. Chip is setup to transmit a float. 
  

 
/*void serialEvent(){ 
//this interrupt will trigger when the data coming from
received_from_computer=Serial.readBytesUntil(13,computerdata,20); 
//the serial monitor(pc/mac/other) is received.
computerdata[received_from_computer]=0;
//we read the data sent from the serial monitor
serial_event=1;//(pc/mac/other) until we see a <CR>. We also count
}//how many characters have been received.
//stop the buffer from transmitting leftovers or garbage.

*/
 Wire.beginTransmission(pH1address);
 Wire.write(computerdata);
 Wire.endTransmission(); 
  
   delay(time);
 Wire.requestFrom(pH1address,20,1); 
 //requests 20 bytes of data from Atlas Scientific Chip via I2C
 code=Wire.read(); 
 //reads the response to variable "code"
 
  switch (code){
 case 1:
 Serial.println("Success");
 break;
 case 2:
 Serial.println("Failed");
 break;
 case 254:
 Serial.println("Pending");
 break;
 case 255:
 Serial.println("No Data");
 break;
 
  while(Wire.available()){
 in_char = Wire.read();
 ph_data[i]= in_char;
 i+=1;
 if(in_char==0){
   //assign ph_data, bytes in i to ph1_char[6] defined in yieldbuddy_arduino (or pH1Value, as defined next with PString)
 Wire.endTransmission();
 break;
 }
 }
 
 Serial.println(ph_data);
 serial_event=0;
 }
}
  
  
  //Uncomment this section if you want to take the pH value and convert it into floating point number.
 /*
 ph_float=atof(ph_data);
 */
  
  

  
/*  PString my_pH1_string(pH1_char, sizeof(pH1_char));
  if (pH1Value < 10){//why less than 10, and then print pH1 value to my_pH1_string
    my_pH1_string.print(" "); //why are these quotation marks empty?
    my_pH1_string.println(pH1Value);
  } 
  else {
    my_pH1_string.println(pH1Value);
  }
  
  */
  
  /*PH2------------------------------------------------
  float pH2Sum = 0;
  j = 0;
  analogRead(pH2Pin);  //Get ADC to switch to correct pin
  delay(15); //Wait for Pin to Change

  while(j<30) {
    pH2Sum = pH2Sum + analogRead(pH2Pin);
    j++;
  }
  pH2RawValue = pH2Sum/30;

  pH2Value = (0.0178 * pH2RawValue - 1.889);
  if(isnan(pH2Value)){
    pH2Value = 0;        
  }
  PString my_pH2_string(pH2_char, sizeof(pH2_char));
  if (pH2Value < 20){
    my_pH2_string.print(" "); 
    my_pH2_string.println(pH2Value);
  } 
  else {
    my_pH2_string.println(pH2Value);
  }
  */
  
  
  
 /*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
   !!TEMPERATURE SENSOR!!TEMPERATURE SENSOR!!TEMPERATURE SENSOR!!TEMPERATURE SENSOR!!TEMPERATURE SENSOR!!
   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
  /*
  I'm leaving these here to test OneWire against analogRead of thermocouple both with and without 
  
  */
  double TempSum = 0;
  int j = 0;
  //analogRead(TempPin);  //Get ADC to switch to correct pin
  /*K-type thermocouple with op-amp might work here... but DS1820 would be nicer.
  DallasTemperature and OneWire would be needed as imports
  include one wire address finder sketch
  if no DS1820, and Max31855 is used, make sure there is a decoupling capacitor installed to thermocouple amplifier. 
  (also a good idea to have hooked up to analog reference with reference voltage tallied and used in TempValue calculation
  */
  //delay(15); //Wait for Pin to Change

  while(j<10) {
    TempSum = TempSum + thermocouple.readCelsius();
    j++;
  }

/* for Max31855
*/

  TempRawValue = TempSum/((j-1);

  //TempValue = ((5.00 * TempRawValue * 100.0)/1024.0) + 8.50;
  TempValue = TempRawValue;
    
  //dht.readTemperature(); to use the DHT11
  
  if(isnan(TempValue)){
    TempValue = 0;        
  }
  PString my_Temp_string(Temp_char, sizeof(Temp_char));
  my_Temp_string.print(TempValue);
  my_Temp_string.println(" C"); 
  
  

    my_Temp_string.print(c);
    my_Temp_string.println(" C");
//    Serial.print("C=");
  //  Serial.println(c);
    //Serial.print("F = ");
    //Serial.println(thermocouple.readFarenheit());
    delay(1000);
  }
  
  
 /*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
   !!HUMIDITY DHT11 SENSOR!!HUMIDITY DHT11 SENSOR!!HUMIDITY DHT11 SENSOR!!HUMIDITY DHT11 SENSOR!!HUMIDITY DHT11!!
   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
  //       float RHSum = 0;
  //       j = 0;
  //       
  //       while(j<30) {
  //       RHSum = RHSum + ;
  //       j++;
  //       delay(10);
  //       }
  //       RHRawValue = RHSum/30;
  dht.readHumidity();  //The DHT11 Sensor works differently, but for 'good measure'.
  delay(15); //Wait for Pin to Change
  RHValue = dht.readHumidity();
  if (isnan(RHValue)) {
    RHValue = 0;
  } 
  PString my_RH_string(RH_char, sizeof(RH_char));
  my_RH_string.print(RHValue);

  if (RHValue < 10){
    my_RH_string.println("% "); 
  } 
  else {
    my_RH_string.println("%"); 
  }
  
 /*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
   !!TDS1 SENSOR!!TDS1 SENSOR!!TDS1 SENSOR!!TDS1 SENSOR!!TDS1 SENSOR!!TDS1 SENSOR!!TDS1 SENSOR!!TDS1 SENSOR!!TDS1 SENSOR!!
   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
  float TDS1Sum = 0;
  j = 0;
  analogRead(TDS1Pin);  //Get ADC to switch to correct pin
  delay(15); //Wait for Pin to Change

  while(j<10) {
    TDS1Sum = TDS1Sum + analogRead(TDS1Pin);
    j++;
  }

  TDS1RawValue = TDS1Sum/((j-1) * 2);

  TDS1Value = ((TDS1RawValue * 100.0)/1024.0);
  
  if(isnan(TDS1Value)){
    TDS1Value = 0;        
  }
  PString my_TDS1_string(TDS1_char, sizeof(TDS1_char));
  my_TDS1_string.println(TDS1Value);
  my_TDS1_string.println(" ppm"); 
  
  
 /*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
   !!TDS2 SENSOR!!TDS2 SENSOR!!TDS2 SENSOR!!TDS2 SENSOR!!TDS2 SENSOR!!TDS2 SENSOR!!TDS2 SENSOR!!TDS2 SENSOR!!TDS2 SENSOR!!
   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
  float TDS2Sum = 0;
  j = 0;
  analogRead(TDS2Pin);  //Get ADC to switch to correct pin
  delay(15); //Wait for Pin to Change

  while(j<10) {
    TDS2Sum = TDS2Sum + analogRead(TDS2Pin);
    j++;
  }

  TDS2RawValue = TDS2Sum/((j-1) * 2);

  TDS2Value = ((TDS2RawValue * 100.0)/1024.0);
  
  if(isnan(TDS2Value)){
    TDS2Value = 0;        
  }
  PString my_TDS2_string(TDS2_char, sizeof(TDS2_char));
  my_TDS2_string.print(TDS2Value);
  my_TDS2_string.println(" ppm"); 
  
 /*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
   !!CO2 SENSOR!!CO2 SENSOR!!CO2 SENSOR!!CO2 SENSOR!!CO2 SENSOR!!CO2 SENSOR!!CO2 SENSOR!!CO2 SENSOR!!CO2 SENSOR!!
   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
  float CO2Sum = 0;
  j = 0;
  analogRead(CO2Pin);  //Get ADC to switch to correct pin
  delay(15); //Wait for Pin to Change

  while(j<10) {
    CO2Sum = CO2Sum + analogRead(CO2Pin);
    j++;
  }

  CO2RawValue = CO2Sum/((j-1) * 2);

  CO2Value = ((CO2RawValue * 100.0)/1024.0);
  
  if(isnan(CO2Value)){
    CO2Value = 0;        
  }
  PString my_CO2_string(CO2_char, sizeof(CO2_char));
  my_CO2_string.print(CO2Value);
  my_CO2_string.println(" ppm"); 
  
  
 /*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
   !!LIGHT SENSOR!!LIGHT SENSOR!!LIGHT SENSOR!!LIGHT SENSOR!!LIGHT SENSOR!!LIGHT SENSOR!!LIGHT SENSOR!!
   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
  float LightSum = 0;
  j = 0;
  analogRead(LightPin);  //Get ADC to switch to correct pin
  delay(10); //Wait for Pin to Change
  
  while(j<10) {
    LightSum = LightSum + analogRead(LightPin);
    j++;
  }

  LightRawValue = LightSum/10;

  LightValue = ((LightRawValue * 100.0) / 1024.0);
  
  if (isnan(LightValue)) {
    LightValue = 0;
  } 
  PString my_Light_string(Light_char, sizeof(Light_char));
  my_Light_string.print(LightValue);
  if (LightValue < 10){
    my_Light_string.println("% "); 
  } 
  else {
    my_Light_string.println("%"); 
  }
  
  
  
  

  //pH
  if (pH1Value < pH1Value_Low) {
    pH1_Status = "LOW";
  } 
  else if (pH1Value > pH1Value_Low && pH1Value < pH1Value_High) {
    pH1_Status = "OK";
  } 
  else if (pH1Value > pH1Value_High) {
    pH1_Status = "HIGH";
  }


  if (pH2Value < pH2Value_Low) {
    pH2_Status = "LOW";
  } 
  else if (pH2Value > pH2Value_Low && pH2Value < pH2Value_High) {
    pH2_Status = "OK";
  } 
  else if (pH2Value > pH2Value_High) {
    pH2_Status = "HIGH";
  }

  //Temp
  if (TempValue < TempValue_Low) {
    Temp_Status = "LOW";
  } 
  else if (TempValue > TempValue_Low && TempValue < TempValue_High) {
    Temp_Status = "OK";
  } 
  else if (TempValue > TempValue_High) {
    Temp_Status = "HIGH";
  }
  //RH
  if (RHValue < RHValue_Low) {
    RH_Status = "LOW";
  } 
  else if (RHValue > RHValue_Low && RHValue < RHValue_High) {
    RH_Status = "OK";
  }
  else if (RHValue < RHValue_High) {
    RH_Status = "HIGH";
  }
  //TDS1
  if (TDS1Value < TDS1Value_Low) {
    TDS1_Status = "LOW";
  } 
  else if (TDS1Value > TDS1Value_Low && TDS1Value < TDS1Value_High) {
    TDS1_Status = "OK";
  } 
  else if (TDS1Value < TDS1Value_High) {
    TDS1_Status = "HIGH";
  }
  //TDS2
  if (TDS2Value < TDS2Value_Low) {
    TDS2_Status = "LOW";
  } 
  else if (TDS2Value > TDS2Value_Low && TDS2Value < TDS2Value_High) {
    TDS2_Status = "OK";
  } 
  else if (TDS2Value < TDS2Value_High) {
    TDS2_Status = "HIGH";
  }
  //CO2
  if (CO2Value < CO2Value_Low) {
    CO2_Status = "LOW";
  } 
  else if (CO2Value > CO2Value_Low && CO2Value < CO2Value_High) {
    CO2_Status = "OK";
  } 
  else if (CO2Value > CO2Value_High){
    CO2_Status = "HIGH";
  }
  //Light
  if (LightValue < LightValue_Low) {
    Light_Status = "LOW";
  } 
  else if (LightValue > LightValue_Low && LightValue < LightValue_High) {
    Light_Status = "OK";
  } 
  else if (LightValue < LightValue_High) {
    Light_Status = "HIGH";
  }

}
