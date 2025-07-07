#include <TinyGPSPlus.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <TimeLib.h> // Required for Unix time conversion
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <avr/pgmspace.h>
#include "ceres_data.h"
#include "haumea_data.h"
#include "makemake_data.h"
#include "eris_data.h"

#define PI 3.14159265358979323846
#define TWO_PI (2.0 * PI)
#ifndef degrees
double degrees(double rad) {
 return rad * 180.0 / PI;
}
#endif

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
TinyGPSPlus gps;

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1  // or 4 if your screen uses that
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int pressed = 0;
int planet = 4;
int mode = 1;

#define PAN_CHANNEL 0
#define TILT_CHANNEL 1
#define PAN_MIN_PULSE 1008
#define PAN_MAX_PULSE 1242
#define TILT_MIN_PULSE 500
#define TILT_MAX_PULSE 2500

static const uint32_t GPSBaud = 9600;

double array[3];
int gpsYear, gpsMonth, gpsDay, gpsHour, gpsMinute;
double lat = 37.3387;
double lng = -121.8853;
double headingOffset = 0.0;
double lastAzimuth = 0.0;
double lastServoAz = 0.0;
double timeOffsetHours = 0.0;
double tiltAngle = 0.0;
double trueAzimuth = 0.0;
double trueAltitude = 0.0;
int projectedHour = 0;
int projectedMinute = 0;

tmElements_t tm;

unsigned long getUnixTime(int y, int mo, int d, int h, int mi, int s = 0) {
tm.Year = y - 1970;
tm.Month = mo;
tm.Day = d;
tm.Hour = h;
tm.Minute = mi;
tm.Second = s;
return makeTime(tm);
}

String getPlanetName(int num) {
switch (num) {
  case 1: return "Mercury";
  case 2: return "Venus";
  case 3: return "Earth";    // Keep Earth for calculations only
  case 4: return "Mars";
  case 5: return "Jupiter";
  case 6: return "Saturn";
  case 7: return "Uranus";
  case 8: return "Neptune";
  case 9: return "Pluto";
  case 10: return "Moon";
  case 11: return "Ceres";
  case 12: return "Haumea";
  case 13: return "Makemake";
  case 14: return "Eris";
  default: return "None";
}
}

int planetInput(int potval) {
if (potval <= 78) return 1;      // Mercury
else if (potval <= 156) return 2;  // Venus
else if (potval <= 234) return 4;  // Mars (skip Earth = 3)
else if (potval <= 313) return 5;  // Jupiter
else if (potval <= 392) return 6;  // Saturn
else if (potval <= 471) return 7;  // Uranus
else if (potval <= 550) return 8;  // Neptune
else if (potval <= 629) return 9;  // Pluto
else if (potval <= 708) return 10; // Moon
else if (potval <= 787) return 11; // Ceres
else if (potval <= 866) return 12; // Haumea
else if (potval <= 945) return 13; // Makemake
else return 14;                    // Eris
}

// The ephemeris tables all start at 2025‑Jan‑01 00:00 UTC
unsigned long dwarf_start_unix;
const float    dwarf_step_days = 3.0;

void setup() {
Serial.begin(9600);
Serial2.begin(GPSBaud);
Wire.begin();
pwm.begin();
pwm.setPWMFreq(50);
pinMode(13, OUTPUT);
pinMode(46, INPUT_PULLUP);
 if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Most OLEDs use 0x3C
 Serial.println(F("SSD1306 allocation failed"));
 for (;;); // Stop here if OLED fails
}
display.clearDisplay();
display.setTextSize(1);
display.setTextColor(SSD1306_WHITE);

dwarf_start_unix = getUnixTime(2025, 1, 1, 0, 0);

}

void loop() {
if (gps.location.isValid()) {
 lat = gps.location.lat();
 lng = gps.location.lng();
}

gpsYear = gps.date.year();
gpsMonth = gps.date.month();
gpsDay = gps.date.day();
gpsHour = gps.time.hour();
gpsMinute = gps.time.minute();

if (digitalRead(46) == HIGH) pressed = 0;
if (digitalRead(46) == LOW && pressed == 0) {
 mode = !mode;
 pressed = 1;
 Serial.println("**Mode changed**");
 delay(200);
}

if (mode == 1) {
 int potval = analogRead(A14);
 planet = planetInput(potval);
} else if (mode == 0) {
 int potval = analogRead(A14);
 int step = map(potval, 0, 1023, 0, 576);
 int centeredStep = step - 288;
 timeOffsetHours = centeredStep * 5.0 / 60.0;
 double projectedHourTotal = gpsHour + gpsMinute / 60.0 + timeOffsetHours;
 int totalMinutes = int(projectedHourTotal * 60);
 int projectedDay = gpsDay + totalMinutes / (24 * 60);
 projectedHour = (totalMinutes / 60) % 24;
 projectedMinute = totalMinutes % 60;
 if (projectedMinute < 0) {
   projectedMinute += 60;
   projectedHour -= 1;
 }
 if (projectedHour < 0) {
   projectedHour += 24;
   projectedDay -= 1;
 }
}

if (mode == 0) {
 Serial.println("== Trajectory Mode Info ==");
 Serial.print("Time Offset (hours): ");
 Serial.println(timeOffsetHours, 2);

 Serial.print("Projected Time (UTC): ");
 Serial.print(projectedHour < 10 ? "0" : ""); Serial.print(projectedHour);
 Serial.print(":");
 Serial.print(projectedMinute < 10 ? "0" : ""); Serial.println(projectedMinute);

 // Reconstruct normalized date from Unix time
time_t projectedTime = getUnixTime(gpsYear, gpsMonth, gpsDay, gpsHour, gpsMinute) + long(timeOffsetHours * 3600.0);
tmElements_t projectedTm;
breakTime(projectedTime, projectedTm);

Serial.print("Projected Date (UTC): ");
Serial.print(projectedTm.Month < 10 ? "0" : ""); Serial.print(projectedTm.Month);
Serial.print("/");
Serial.print(projectedTm.Day < 10 ? "0" : ""); Serial.print(projectedTm.Day);
Serial.print("/");
Serial.println(projectedTm.Year + 1970);
}

int useHour = (mode == 0) ? projectedHour : gpsHour;
int useMinute = (mode == 0) ? projectedMinute : gpsMinute;
int useDay = gpsDay + ((mode == 0) ? (int)((gpsHour + gpsMinute / 60.0 + timeOffsetHours) / 24) : 0);
int useSecond = gps.time.second();
unsigned long unixTime = getUnixTime(gpsYear, gpsMonth, useDay, useHour, useMinute, useSecond);

double d = (unixTime - 946728000.0) / 86400.0; // days since J2000.0
double GMST = fmod(280.46061837 + 360.98564736629 * d, 360.0);
double LST = fmod(GMST + lng + 360.0, 360.0);

Serial.print("Unix Time: "); Serial.println(unixTime);
Serial.print("Days since J2000.0: "); Serial.println(d);
Serial.print("GMST: "); Serial.println(GMST);
Serial.print("LST: "); Serial.println(LST);

calculate(array, mode == 0 ? timeOffsetHours : 0.0, radians(LST), gps.time.second());

String name = getPlanetName(planet);
Serial.print("Planet Selected: "); Serial.print(name);
Serial.print(", #"); Serial.println(planet);

Serial.print("Latitude: "); Serial.println(lat, 4);
Serial.print("Longitude: "); Serial.println(lng, 4);

char dateStr[32];
sprintf(dateStr, "%02d/%02d/%04d", gpsMonth, gpsDay, gpsYear);
Serial.print("Date(UTC): "); Serial.println(dateStr);

char timeStr[32];
sprintf(timeStr, "%02d:%02d:%02d", gpsHour, gpsMinute, gps.time.second());
Serial.print("Time(UTC): "); Serial.println(timeStr);

Serial.print("Distance: "); Serial.print(array[2], 4); Serial.println(" AU");
Serial.print("RA: "); Serial.println(array[0], 4);
Serial.print("Dec: "); Serial.println(array[1], 4);
Serial.print("Pan Angle: "); Serial.print(lastServoAz, 2); Serial.println("°");
Serial.print("Tilt Angle: "); Serial.print(tiltAngle, 2); Serial.println("°");

trueAzimuth = lastServoAz;
trueAltitude = tiltAngle;

Serial.print("Azimuth (servo-based): "); Serial.print(lastAzimuth, 2); Serial.println("°");
Serial.print("Altitude (servo-based): "); Serial.print(trueAltitude, 2); Serial.println("°");

smartDelay(1000);

display.clearDisplay();

display.setCursor(0, 0);
display.print(getPlanetName(planet));
display.print(" ");
if (gps.time.isValid()) {
 if (gpsHour < 10) display.print("0");
 display.print(gpsHour);
 display.print(":");
 if (gpsMinute < 10) display.print("0");
 display.print(gpsMinute);
 display.print(":");
 if (gps.time.second() < 10) display.print("0");
 display.print(gps.time.second());
}

int16_t x1, y1;
uint16_t w, h;
display.getTextBounds("LIVE", 0, 0, &x1, &y1, &w, &h);
display.setCursor(SCREEN_WIDTH - w - 1, SCREEN_HEIGHT - h);  // Bottom-right corner
display.print(mode == 0 ? "TRJ" : "LIVE");

int y = 8;

if (mode == 0) {
 display.setCursor(0, y); y += 8;
 display.print("Offset: ");
 display.print(timeOffsetHours, 2);
 display.print("h");

 display.setCursor(0, y); y += 8;
 display.print("New: ");
 if (projectedHour < 10) display.print("0");
 display.print(projectedHour);
 display.print(":");
 if (projectedMinute < 10) display.print("0");
 display.print(projectedMinute);
 display.print(":");
 if (gps.time.second() < 10) display.print("0");
 display.print(gps.time.second());
}

display.setCursor(0, y); y += 8;
display.print("RA: ");
display.print(array[0], 2); display.print("h");

display.setCursor(0, y); y += 8;
display.print("Dec:");
display.print(array[1], 2); display.write(248);  // degree symbol

display.setCursor(0, y); y += 8;
display.print("Az: ");
display.print(lastAzimuth, 1); display.write(248);

display.setCursor(0, y); y += 8;
display.print("Alt:");
display.print(trueAltitude, 1); display.write(248);

display.display();
}

static void smartDelay(unsigned long ms) {
unsigned long start = millis();
do {
 while (Serial2.available()) gps.encode(Serial2.read());
} while (millis() - start < ms);
}

void calculate(double *array, double futureOffsetHours, double LST, int seconds) {
 // Compute total hour offset including seconds
 double hourTotal = gpsHour + gpsMinute / 60.0 + seconds / 3600.0 + futureOffsetHours;

 // Handle wrap-around for negative/overflow hours
 int addedDays = int(hourTotal / 24);
 hourTotal = fmod(hourTotal, 24.0);
 if (hourTotal < 0) {
   hourTotal += 24.0;
   addedDays--;
 }

 // Convert hourTotal into rounded hour/minute/second
 int adjHour = int(hourTotal);
 double minuteFraction = (hourTotal - adjHour) * 60.0;
 int adjMinute = int(minuteFraction);
 int adjSecond = int(round((minuteFraction - adjMinute) * 60.0));

 if (adjSecond >= 60) {
   adjSecond -= 60;
   adjMinute += 1;
 }
 if (adjMinute >= 60) {
   adjMinute -= 60;
   adjHour += 1;
 }
 if (adjHour >= 24) {
   adjHour -= 24;
   addedDays++;
 }

 // Get Unix time for adjusted date/time
 unsigned long unixTime = getUnixTime(gpsYear, gpsMonth, gpsDay + addedDays, adjHour, adjMinute, adjSecond);

 // Days since J2000.0
 double d = (unixTime - 946728000UL) / 86400.0;

 // TT correction
 double TT = d + (69.0 / 86400.0);
 double T = TT / 36525.0;

 // Moon
 if (planet == 10) {
   double D = radians(fmod(297.8501921 + 445267.1114034 * T
     - 0.0018819 * T * T + T * T * T / 545868 - T * T * T * T / 113065000, 360.0));
   double M_sun = radians(fmod(357.5291092 + 35999.0502909 * T
     - 0.0001536 * T * T + T * T * T / 24490000, 360.0));
   double M_moon = radians(fmod(134.9633964 + 477198.8675055 * T
     + 0.0087414 * T * T + T * T * T / 69699 - T * T * T * T / 14712000, 360.0));
   double F = radians(fmod(93.2720950 + 483202.0175233 * T
     - 0.0036539 * T * T - T * T * T / 3526000 + T * T * T * T / 863310000, 360.0));

   double lon = 218.3164591
     + 481267.88134236 * T
     + 6.289 * sin(M_moon)
     - 1.274 * sin(2*D - M_moon)
     + 0.658 * sin(2*D)
     - 0.214 * sin(2*M_moon)
     - 0.11 * sin(D);
   lon = fmod(lon, 360.0);

   double lat_moon = 5.128 * sin(F)
     + 0.28 * sin(M_moon + F)
     - 0.28 * sin(F - M_moon)
     - 0.17 * sin(F - 2*D);

   double dist = 60.36298
     - 3.27746 * cos(M_moon)
     - 0.57994 * cos(2*D - M_moon)
     - 0.46357 * cos(2*D)
     + 0.08904 * cos(2*M_moon);

   double dist_au = dist * 0.000042635;

   double lon_rad = radians(lon);
   double lat_rad = radians(lat_moon);
   double x = dist_au * cos(lat_rad) * cos(lon_rad);
   double y = dist_au * cos(lat_rad) * sin(lon_rad);
   double z = dist_au * sin(lat_rad);

   double ec = radians(23.439292);
   double Xq = x;
   double Yq = y * cos(ec) - z * sin(ec);
   double Zq = y * sin(ec) + z * cos(ec);

   double alpha = atan2(Yq, Xq);
   if (alpha < 0) alpha += TWO_PI;
   double delta = atan2(Zq, sqrt(Xq*Xq + Yq*Yq));
   double distance = sqrt(Xq*Xq + Yq*Yq + Zq*Zq);

   array[0] = degrees(alpha) / 15.0;
   array[1] = degrees(delta);
   array[2] = distance;

   goto horizontal;
 }

 // ────────────────────────────────────────────────────
 // Dwarf planets via PROGMEM tables + linear interpolation
 // ────────────────────────────────────────────────────
 if (planet >= 11 && planet <= 14) {
   // 1) how many days have elapsed since the table start?
   double daysSinceStart = (double)(unixTime - dwarf_start_unix) / 86400.0;
   double rawIndex      = daysSinceStart / dwarf_step_days;
   int    i0            = constrain(int(floor(rawIndex)), 0, ceres_num_records - 2);
   int    i1            = i0 + 1;
   float  frac          = rawIndex - i0;

   // 2) pick the right table pointers
   const float *raTable, *decTable, *distTable;
   switch (planet) {
     case 11: raTable = ceres_ra_deg;   decTable = ceres_dec_deg;   distTable = ceres_dist_au;   break;
     case 12: raTable = haumea_ra_deg;  decTable = haumea_dec_deg;  distTable = haumea_dist_au;  break;
     case 13: raTable = makemake_ra_deg;decTable = makemake_dec_deg;distTable = makemake_dist_au;break;
     case 14: raTable = eris_ra_deg;    decTable = eris_dec_deg;    distTable = eris_dist_au;    break;
   }

   // 3) read & interpolate via pgm_read_float_near()
   float ra0  = pgm_read_float_near(&raTable[i0]);
   float ra1  = pgm_read_float_near(&raTable[i1]);
   float dec0 = pgm_read_float_near(&decTable[i0]);
   float dec1 = pgm_read_float_near(&decTable[i1]);
   float d0   = pgm_read_float_near(&distTable[i0]);
   float d1   = pgm_read_float_near(&distTable[i1]);

   float ra_deg   = ra0   + frac * (ra1   - ra0);
   float dec_deg  = dec0  + frac * (dec1  - dec0);
   float dist_au  = d0    + frac * (d1    - d0);

   // 4) store into your array (RA in hours)
   array[0] = ra_deg / 15.0;
   array[1] = dec_deg;
   array[2] = dist_au;

   // convert back to radians for the horizontal section:
   double alpha = radians(ra_deg);
   double delta = radians(dec_deg);

   // jump straight to your horizontal‑coordinates code:
   goto horizontalDwarf;
 }

 // Classical planets
 double a[] = {0,0.387098,0.723330,1.000000,1.523688,5.20260,9.55491,19.2184,30.1104,39.4821};
 double e[] = {0,0.205630 + 0.000020406*T,0.006773 - 0.000049*T,0.016710 - 0.000042037*T,0.093312 + 0.000092064*T,0.048334 + 0.00016418*T,0.055892 - 0.000346818*T,0.046344 - 0.00002658*T,0.008997 + 0.00000633*T,0.24880766};
 double i[] = {0,7.00487 - 23.51e-6*T,3.39471 - 2.86e-6*T,0.00005 - 46.94e-6*T,1.85061 - 25.47e-6*T,1.30530 - 4.15e-6*T,2.48446 + 6.11e-6*T,0.76986 - 2.09e-6*T,1.76917 - 3.64e-6*T,17.14175};
 double Omega[] = {0,48.33167 - 446.30e-6*T,76.68069 - 996.89e-6*T,0.0,49.57854 - 1.105e-5*T,100.55615 + 1.019e-5*T,113.71504 - 1.143e-5*T,74.22988 - 1.578e-5*T,131.72169 - 1.151e-5*T,110.30347};
 double Pi[] = {0,77.45645 + 573.57e-6*T,131.53298 - 108.80e-6*T,102.94719 + 1198.28e-6*T,336.04084 + 1590.20e-6*T,14.75385 + 870.53e-6*T,92.43194 + 541.14e-6*T,170.96424 + 408.52e-6*T,44.97135 + 292.73e-6*T,224.06676};
 double L[] = {0,
   fmod(252.250906 + 149472.6746358*T,360.0),
   fmod(181.979801 + 58517.8156760*T,360.0),
   fmod(100.466457 + 35999.3728565*T,360.0),
   fmod(355.433000 + 19140.2993039*T,360.0),
   fmod(34.351519 + 3034.9056606*T,360.0),
   fmod(50.077444 + 1222.1138488*T,360.0),
   fmod(314.055005 + 428.4669983*T,360.0),
   fmod(304.348665 + 218.4862002*T,360.0),
   fmod(238.929038 + 145.2078052*T,360.0)
 };

 double M = radians(fmod(L[planet]-Pi[planet]+360.0,360.0));
 double ecc = e[planet];
 double v = M + (2*ecc -0.25*pow(ecc,3))*sin(M)
               + (1.25*pow(ecc,2))*sin(2*M)
               + ((13.0/12.0)*pow(ecc,3))*sin(3*M);
 double r = a[planet]*(1 - ecc*ecc)/(1 + ecc*cos(v));
 double i_rad = radians(i[planet]);
 double o_rad = radians(Omega[planet]);
 double w_rad = radians(Pi[planet]-Omega[planet]);
 double v_plus_w = v + w_rad;

 double x = r*(cos(o_rad)*cos(v_plus_w)-sin(o_rad)*sin(v_plus_w)*cos(i_rad));
 double y = r*(sin(o_rad)*cos(v_plus_w)+cos(o_rad)*sin(v_plus_w)*cos(i_rad));
 double z = r*sin(v_plus_w)*sin(i_rad);

 double ec = radians(23.439292);
 double Xq = x;
 double Yq = y*cos(ec)-z*sin(ec);
 double Zq = y*sin(ec)+z*cos(ec);

 double alpha = atan2(Yq,Xq);
 if (alpha<0) alpha+=TWO_PI;
 double delta = atan2(Zq,sqrt(Xq*Xq+Yq*Yq));
 double distance = sqrt(Xq*Xq+Yq*Yq+Zq*Zq);

 array[0]=degrees(alpha)/15.0;
 array[1]=degrees(delta);
 array[2]=distance;

 horizontal:
 // Horizontal computation (Moon and planets)
 ;
 horizontalDwarf:
 // Compute horizontal coordinates
 double HA = fmod(LST - alpha + TWO_PI,TWO_PI);
 double x_hor = cos(HA)*cos(delta);
 double y_hor = sin(HA)*cos(delta);
 double z_hor = sin(delta);

 double xhor = x_hor*sin(radians(lat)) - z_hor*cos(radians(lat));
 double yhor = y_hor;
 double zhor = x_hor*cos(radians(lat)) + z_hor*sin(radians(lat));

 double az = atan2(yhor,xhor);
 az=degrees(az);
 az=fmod(az+180.0,360.0);
 double alt = -degrees(asin(zhor));

 float servoAz=fmod((az-headingOffset+360.0),360.0);
 float panPulse=PAN_MIN_PULSE+(servoAz/360.0)*(PAN_MAX_PULSE-PAN_MIN_PULSE);
 panPulse=constrain(panPulse,PAN_MIN_PULSE,PAN_MAX_PULSE);

 float tiltPulse=TILT_MIN_PULSE+((90.0-alt)/180.0)*(TILT_MAX_PULSE-TILT_MIN_PULSE);
 tiltPulse=constrain(tiltPulse,TILT_MIN_PULSE,TILT_MAX_PULSE);

 tiltAngle=((tiltPulse-TILT_MIN_PULSE)/(TILT_MAX_PULSE-TILT_MIN_PULSE))*180.0-90.0;

 pwm.writeMicroseconds(PAN_CHANNEL,panPulse);
 pwm.writeMicroseconds(TILT_CHANNEL,tiltPulse);
 digitalWrite(13,HIGH);

 lastAzimuth=az;
 lastServoAz=servoAz;
}
