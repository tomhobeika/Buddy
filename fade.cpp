/*
  Fade
  This gradually turns the light from low power (0) to high power (255).

  Must plug light into a pin with a ~ (Currently on 9).
 */

int led = 9;
int brightness = 0;
int increment = 10;

// the setup routine runs once when you press reset:
void setup() { 
  Serial.begin(9600);
  Serial.println("Hello World!");              
  // initialize the digital pin as an output.
  pinMode(led, OUTPUT);     
}

// the loop routine runs over and over again forever:
void loop() {
  for(brightness = 0; brightness <=255; brightness += increment){ 
    analogWrite(led, brightness); 
    delay(30);               // wait for 30 miliseconds
  }
  Serial.println("Faded in");
  delay(2000);
  for(brightness = 255; brightness >=0; brightness -= increment){ 
    analogWrite(led, brightness);  
    delay(30);               // wait for 30 miliseconds
  }
  Serial.println("Faded out");
}
