/*********************
 *10 to GPS Module RX*
 *11 to GPS Module TX*
 *********************/

#include <SoftwareSerial.h>
#include <TinyGPS.h>

SoftwareSerial mySerial(10, 11);
TinyGPS gps;

void gpsdump(TinyGPS &gps);
void printFloat(double f, int digits = 2);

void setup()
{
  // Oploen serial communications and wait for port to open:
  Serial.begin(9600);
  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);
  delay(1000);
  Serial.println("uBlox Neo 6M");
  Serial.println();
}

void loop() // run over and over
{
  bool newdata = false;
  unsigned long start = millis();
  // Every 5 seconds we print an update
  while (millis() - start < 5000)
  {
    if (mySerial.available())

    {
      char c = mySerial.read();
      //Serial.print(c);  // uncomment to see raw GPS data
      if (gps.encode(c))
      {
        newdata = true;
        break;  // uncomment to print new data immediately!
      }
    }
  }

  if (newdata)
  {
    Serial.println("Acquired Data");
    Serial.println("-------------");
    gpsdump(gps);
    Serial.println();
  }

}

void gpsdump(TinyGPS &gps)
{
  float flat, flon;
  int year;//4 bytes
  byte month, day, hour, minute, second, hundredths;

  gps.f_get_position(&flat, &flon);
  Serial.print("Lat/Long(float): "); printFloat(flat, 5); Serial.print(", "); printFloat(flon, 5);

  gps.crack_datetime(&year, &month, &day, &hour, &minute, &second, &hundredths);
  Serial.print("  Date: "); Serial.print(static_cast<int>(month));/*ép kiểu*/ Serial.print("/");
  Serial.print(static_cast<int>(day)); Serial.print("/"); Serial.print(year);
  Serial.print("  Time: "); Serial.print(static_cast<int>(hour+7));  Serial.print(":"); //Serial.print("UTC +07:00 Viet Nam");
  Serial.print(static_cast<int>(minute)); Serial.print(":"); Serial.print(static_cast<int>(second));
  Serial.print("."); Serial.print(static_cast<int>(hundredths)); Serial.print(" UTC +07:00 Viet Nam");

}

void printFloat(double number, int digits)
{
  // Handle negative numbers
  if (number < 0.0)
  {
     Serial.print('-');
     number = -number;
  }

  // Round correctly so that print(1.999, 2) prints as "2.00"
  double rounding = 0.5;
  for (uint8_t i=0; i<digits; ++i)
    rounding /= 10.0;

  number += rounding;

  // Extract the integer part of the number and print it
  unsigned long int_part = (unsigned long)number;
  double remainder = number - (double)int_part;
  Serial.print(int_part);

  // Print the decimal point, but only if there are digits beyond
  if (digits > 0)
    Serial.print(".");

  // Extract digits from the remainder one at a time
  while (digits-- > 0)
  {
    remainder *= 10.0;
    int toPrint = int(remainder);
    Serial.print(toPrint);
    remainder -= toPrint;
  }
}
