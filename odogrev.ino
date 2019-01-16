
#define DS18B20_PIN   11


int raw_temp;
boolean knopka =false;

float temp;



 

void setup(void) {
pinMode(12, INPUT_PULLUP);
 pinMode(3, OUTPUT); 
  Serial.begin(9600);
  delay (2000);
   if(ds18b20_read(&raw_temp)) {
  Serial.println(temp);    
       if(temp < 10){
      digitalWrite(3,HIGH);
      knopka =true;
     }
     else {
      digitalWrite(3,LOW);
        knopka =false;
     }
   }

}

 

void loop(void) {

  if(ds18b20_read(&raw_temp)  ) {



    Serial.print("Temperature = ");

    temp = (float)raw_temp / 16;     // Convert temperature raw value into degree Celsius (temp in °C = raw/16)

    Serial.print(temp);              // Print temperature value in degree Celsius

    Serial.println("°C");            // Print '°C'

    if(temp < 30 && knopka){
      digitalWrite(3,HIGH);
     }
     

     if(temp > 37){
      digitalWrite(3,LOW);
     }
     
    if( digitalRead(12) == 0){
      delay(2000);
      if( digitalRead(12) == 0){
      
        knopka = not knopka;
       Serial.print("knopka= "); 
        Serial.println(knopka);  
        
        if(knopka){
           digitalWrite(3,HIGH); 
        }else {
          digitalWrite(3,LOW); 
        }


      }
        
    }
  }

  else {

    Serial.println("Communication Error!");
  }

  delay(1000);

}

 

bool ds18b20_start(){

  bool ret = 0;

  digitalWrite(DS18B20_PIN, LOW);     // Send reset pulse to the DS18B20 sensor

  pinMode(DS18B20_PIN, OUTPUT);

  delayMicroseconds(500);             // Wait 500 us

  pinMode(DS18B20_PIN, INPUT);

  delayMicroseconds(100);             //wait to read the DS18B20 sensor response

  if (!digitalRead(DS18B20_PIN)) {

    ret = 1;                          // DS18B20 sensor is present

    delayMicroseconds(400);           // Wait 400 us

  }

  return(ret);

}

 

void ds18b20_write_bit(bool value){

  digitalWrite(DS18B20_PIN, LOW);

  pinMode(DS18B20_PIN, OUTPUT);

  delayMicroseconds(2);

  digitalWrite(DS18B20_PIN, value);

  delayMicroseconds(80);

  pinMode(DS18B20_PIN, INPUT);

  delayMicroseconds(2);

}

 

void ds18b20_write_byte(byte value){

  byte i;

  for(i = 0; i < 8; i++)

    ds18b20_write_bit(bitRead(value, i));

}

 

bool ds18b20_read_bit(void) {

  bool value;

  digitalWrite(DS18B20_PIN, LOW);

  pinMode(DS18B20_PIN, OUTPUT);

  delayMicroseconds(2);

  pinMode(DS18B20_PIN, INPUT);

  delayMicroseconds(5);

  value = digitalRead(DS18B20_PIN);

  delayMicroseconds(100);

  return value;

}

 

byte ds18b20_read_byte(void) {

  byte i, value;

  for(i = 0; i  <8; i++)

    bitWrite(value, i, ds18b20_read_bit());

  return value;

}

 

bool ds18b20_read(int *raw_temp_value) {

  if (!ds18b20_start())                     // Send start pulse

    return(0);                              // Return 0 if error

  ds18b20_write_byte(0xCC);                 // Send skip ROM command

  ds18b20_write_byte(0x44);                 // Send start conversion command

  while(ds18b20_read_byte() == 0);          // Wait for conversion complete

  if (!ds18b20_start())                     // Send start pulse

    return(0);                              // Return 0 if error

  ds18b20_write_byte(0xCC);                 // Send skip ROM command

  ds18b20_write_byte(0xBE);                 // Send read command

  *raw_temp_value = ds18b20_read_byte();    // Read temperature LSB byte and store it on raw_temp_value LSB byte

  *raw_temp_value |= (unsigned int)(ds18b20_read_byte() << 8);     // Read temperature MSB byte and store it on raw_temp_value MSB byte

  return(1);                                // OK --> return 1

}