#include <TinyGPS++.h>
#include<LiquidCrystal.h>
#include <SoftwareSerial.h>


LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);
TinyGPSPlus gps;

const int SW = 10;
const int buzzer = 13;
const int relay = 12;

volatile float minutes, seconds;
volatile int degree, secs, mins;
unsigned long start;
uint8_t hr_val, min_val, sec_val;
bool loc_valid, alt_valid, time_valid;
double lat_val, lng_val, alt_m_val;



void setup() {
  lcd.begin(16, 2);
  pinMode(SW, INPUT_PULLUP);
  pinMode(buzzer, OUTPUT);
  pinMode(relay, OUTPUT);
  digitalWrite(buzzer, LOW);
  digitalWrite(relay, HIGH);//off
  project_Name();
  //----------------------gsm-----------------
  lcd.setCursor(0, 0);
  lcd.print("Gsm Module      ");
  lcd.setCursor(0, 1);
  lcd.print("Initilizing.... ");
  delay(1000);
  Serial.begin(9600);
  delay(1000);
  Serial.println("AT+CNMI=2,2,0,0,0");
  delay(3000);
  Serial.println("AT+CMGF=1");
  delay(3000);
  Serial.println("AT+CMGS=\"+917095797436\"\r"); // Replace x with mobile number
  delay(3000);
  Serial.println("System is ready to send Messages.");// The SMS text you want to send
  delay(3000);
  Serial.println((char)26); // ASCII code of CTRL+Z
  delay(2000);
  lcd.setCursor(0, 0);
  lcd.print("Gsm Module       ");
  lcd.setCursor(0, 1);
  lcd.print("Initilized..... ");
  delay(1000);
  lcd.clear();

}



void loop() {
  get_GPSData();
  display_LongLati_Values();
  delay(1000);
  if (digitalRead(SW) == LOW) //press
  {

    lcd.setCursor(0, 0);
    lcd.print("Accident Deted");

    digitalWrite(relay, LOW);//on
    digitalWrite(buzzer, HIGH);
    delay(1000);
    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("Message sending ");
    lcd.setCursor(0, 1);
    lcd.print("PLS wait....    ");
    tracking();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Message sent    ");
    lcd.setCursor(0, 1);
    lcd.print("Sucessfully...  ");
    delay(1000);
    digitalWrite(buzzer, LOW);
    lcd.clear();
  }
  else
  {
    lcd.setCursor(0, 0);
    lcd.print("  No  Accident ");
    delay(1000);
    lcd.clear();

  }

}








void display_LongLati_Values() {
  if (!loc_valid)
  {
    lcd.setCursor(0, 0);
    lcd.print("Latitude : ");
    lcd.setCursor(0, 1);
    lcd.print("*");
    delay(1000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Longitude : ");
    lcd.setCursor(0, 1);
    lcd.print("*");
    delay(1000);
    lcd.clear();
  }
  else
  {
    

    lcd.print("LAT: ");
    lcd.setCursor(5, 0);
    lcd.print(lat_val, 8);
    lcd.setCursor(0, 1);
    lcd.print("LON: ");
    lcd.setCursor(5, 1);
    lcd.print(lng_val, 8);
  }
}
/*
  void buzzer_Sound(unsigned char Ntimes){
  unsigned char i;
  for(i=0; i<Ntimes; i++)
  {

     delay(1000);
     digitalWrite(buzzer, LOW);
     delay(800);
  }
  }
*/
void get_GPSData() {
  {
    smartDelay(1000); /* Generate precise delay of 1ms */
    lat_val = gps.location.lat(); /* Get latitude data */
    loc_valid = gps.location.isValid(); /* Check if valid location data is available */
    lng_val = gps.location.lng(); /* Get longtitude data */
    alt_m_val = gps.altitude.meters();  /* Get altitude data in meters */
    alt_valid = gps.altitude.isValid(); /* Check if valid altitude data is available */
  }
}

static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do
  {
    while (Serial.available())  /* Encode data read from GPS while data is available on serial port */
      gps.encode(Serial.read());
    /* Encode basically is used to parse the string received by the GPS and to store it in a buffer so that information can be extracted from it */
  } while (millis() - start < ms);
}

  /* Convert data in decimal degrees into degrees minutes seconds form */
{
  degree = (int)tot_val;
  minutes = tot_val - degree;
  seconds = 60 * minutes;
  minutes = (int)seconds;
  mins = (int)minutes;
  seconds = seconds - minutes;
  seconds = 60 * seconds;
  secs = (int)seconds;
}

void tracking()
{
  init_sms();
  delay(3000);
  send_data("Alert!Accident detected at place: ");
  Serial.print("Lat:");
  Serial.print(lat_val);
  Serial.print("  ");
  Serial.print("Lang:");
  Serial.println(lng_val);
  Serial.print("http://maps.google.com/?q=");
  Serial.print(lat_val, 8);
  Serial.print(",");
  Serial.print(lng_val, 8);
  delay(3000);
  send_sms();
  delay(2000);

}

void init_sms()
{
  Serial.println("AT+CMGF=1");
  delay(3000);
  Serial.println("AT+CMGS=\"+917095797436\"\r");   // use 10 digit cell no. here
  delay(3000);
}

void send_data(String message)
{
  Serial.println(message);
  delay(1000);
}
void send_sms()
{
  Serial.write(26);
}


void project_Name() {
  lcd.setCursor(0, 0);
  lcd.print("Accident Alert ");
  lcd.setCursor(0, 1);
  lcd.print(" Sys using GSM ");
  delay(2000);
}