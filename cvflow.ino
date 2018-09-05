/**
 @file 

 @version 1.0
 @author Arco van Geest <arco@appeltaart.mine.nu>
 @copyright 2018 Arco van Geest <arco@appeltaart.mine.nu> All right reserved.

  CVflow

  CVflow is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  CVflow is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with CVflow.  If not, see <http://www.gnu.org/licenses/>.

 @date       20180905 Initial documented version

 @brief Detect flow with temperature sensors on Arduino
 
*/ 

// which analog pin to connect
#define THERMISTORPIN A1
// resistance at 25 degrees C
#define THERMISTORNOMINAL 10000
// temp. for nominal resistance (almost always 25 C)
#define TEMPERATURENOMINAL 25
// how many samples to take and average, more takes longer
// but is more 'smooth'
#define NUMSAMPLES 50
// The beta coefficient of the thermistor (usually 3000-4000)
#define BCOEFFICIENT 3950
#define BCOEFFICIENT1 3950
#define BCOEFFICIENT2 3950
// the value of the 'other' resistor
#define SERIESRESISTOR 10000

#define VERSION 20180905
//#define UADREF 1.82
#define UADREF 3.3

#define UREF 3.3
#define URREF 3.3
#define SAMPLES 200
#define OFFSET2 0
#define ADCOUNT 11
#define DELAY 0

//#define SHOW_CSV_TEMP
//#define SHOW_CSV_R
#define SHOW_CSV_VAL

int samples[NUMSAMPLES];

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(115200);
  analogReference(EXTERNAL);

Serial.print("# version: \t");
Serial.println( VERSION);

Serial.print("# adcount: \t");
Serial.println( ADCOUNT );

Serial.print("# U ADref: \t");
Serial.println( UADREF);

Serial.print("# samples: \t");
Serial.println( NUMSAMPLES);

Serial.println("#V;MILI_END;VAL_AD0;VAL_AD1...");
Serial.println("#T;MILI_END;TEMP_AD0;TEMP_AD1...");
Serial.println("#R;MILI_END;RTHERM0;RTHERM1...");
  
} //setup


void loop() {
  unsigned long time;
  unsigned long measure[ADCOUNT];
// empty measure values
for (int i=0;i<ADCOUNT;i++){
  measure[i] = 0;
}


// measuring
  for (int s=0;s<NUMSAMPLES;s++){
    for (int i=0;i<ADCOUNT;i++){
      measure[i] += analogRead(i);
    }
  }

// get time marker at the end of the measurement
  time=millis();
#if defined(SHOW_CSV_VAL)
//print AD values CSV
  Serial.print("V;");
  Serial.print(time);
  for (int i=0;i<ADCOUNT;i++){
    Serial.print(";");
       #if defined(CSV_TABBED)
    Serial.print("\t");
#endif
    Serial.print(measure[i]/NUMSAMPLES);
  }
  Serial.println();
#endif


#if defined(SHOW_CSV_R)
//print Rthermistor values CSV
  Serial.print("R;");
  Serial.print(time);
  for (int i=0;i<ADCOUNT;i++){
    Serial.print(";");
       #if defined(CSV_TABBED)
    Serial.print("\t");
#endif
    //Serial.print(measure[i]/NUMSAMPLES);

double utherm = (measure[i]/NUMSAMPLES)  * UADREF / 1024;
double userie = URREF - utherm;
double iserie = userie / SERIESRESISTOR;
double rtherm = utherm  / iserie ;


  Serial.print((long)rtherm);


    
  }
  Serial.println();
#endif




  
#if defined(SHOW_CSV_TEMP)

//print temp values CSV
  Serial.print("T;");
  Serial.print(time);
  for (int i=0;i<ADCOUNT;i++){
    Serial.print(";");
   #if defined(CSV_TABBED)
    Serial.print("\t");
#endif
    double utherm = (measure[i]/NUMSAMPLES)  * UADREF / 1024;
    double userie = URREF - utherm;
    double iserie = userie / SERIESRESISTOR;
    double rtherm = utherm  / iserie ;
  
    float steinhart;
    steinhart = rtherm / THERMISTORNOMINAL;     // (R/Ro)
    steinhart = log(steinhart);                  // ln(R/Ro)
    steinhart /= BCOEFFICIENT;                   // 1/B * ln(R/Ro)
    steinhart += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)
    steinhart = 1.0 / steinhart;                 // Invert
    steinhart -= 273.15;                         // convert to C
  
    Serial.print(steinhart);
  }
  Serial.println();
#endif
  
  
  
  
  if ( DELAY > 0 ) {
  delay(DELAY);
  }

}





