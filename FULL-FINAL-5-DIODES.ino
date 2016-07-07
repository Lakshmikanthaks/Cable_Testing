const int OUT_PIN = A1;
const int IN_PIN = A0;
const int IN_PIN1 = A2;
const float IN_STRAY_CAP_TO_GND = 24.48;
const float IN_CAP_TO_GND  = IN_STRAY_CAP_TO_GND;
const float R_PULLUP = 34.8;  
const int MAX_ADC_VALUE = 1023; 
const int dely = 1000;//Maintain this dely else the short circuit tests dont perform as expected.

void setup()
{
 Serial.begin(9600);//Enabling serial monitor to view the status of program
 pinMode(OUT_PIN, OUTPUT);
 pinMode(IN_PIN, OUTPUT);
 pinMode(IN_PIN1, OUTPUT);
 pinMode(44,OUTPUT);//yellow cable for pin3 thru diode anode
 pinMode(42,OUTPUT);//green cable for pin5 thru diode anode
 pinMode(40,OUTPUT);//blue cable for pin S thru diode anode
 pinMode(46,OUTPUT);//black cable for pin L thru diode anode
 pinMode(50,OUTPUT);//GREY cable for green LED positive
 pinMode(48,OUTPUT);//RED cable for RED LED positive 
 pinMode(52,OUTPUT);//outer loop Microswitch 
 pinMode(28,INPUT);//outer loop Microswitch 


}

int Lt,St,fivet,threet,Ct = 0;//These are names of the test for ex.,Lt means L cable test, 5t means pin5 test. If the Lt passes, the variable Lt is assigned 1 else 0.

void loop() 
{
  digitalWrite(52 , HIGH);

  delay(dely);
  if (digitalRead(28) == 1)
     {
 digitalWrite(50,LOW); //make LED low 
 digitalWrite(48,LOW); //make LED low     
 pinMode(OUT_PIN, INPUT);
 pinMode(IN_PIN, INPUT); //Changing the analog i/o s modes so that they dont interfere in short circuit tests.Its mode is changed back again at the end of shorts test.
 pinMode(IN_PIN1, INPUT);                   
                                     Serial.println("STARTING TEST FOR SHORTS");
//----------------------------------------------------Ltest:----------------------------------------------------------

  digitalWrite(46 , HIGH);
  delay(dely);

  if (digitalRead(26) == 0 && digitalRead(32) == 1 && digitalRead(24) == 0  && digitalRead(34) == 0 && digitalRead(22) == 0)
    {
      Serial.println("L is joined to pin2 and pin2 is not shorted with any other pins");
      Lt = 1;
      Serial.println(Lt);
    }
  else  
    {
      Serial.println("L may not be joined to pin2 or pin2 may be short with other pins");
      Lt = 0;
      Serial.println(Lt);
    }
    digitalWrite(46 , LOW);

//---------------------------------------------------End of Ltest--------------------------------------------------------
//-----------------------------------------------------Stest-------------------------------------------------------------
  digitalWrite(40 , HIGH);
  delay(dely);
  if (digitalRead(26) == 0 && digitalRead(32)== 0 && digitalRead(24) == 0  && digitalRead(34) == 1 && digitalRead(22) == 0)
    {
      Serial.println("S is joined to pin4 and pin4 is not shorted with any other pins");
      St = 1;
      Serial.println(St);
    }
  else  
    {
      Serial.println("S may not be joined to pin4 or pin4 may be short with other pins");
      St = 0;
      Serial.println(St);
    }
    digitalWrite(40 , LOW);
//----------------------------------------------------End of Stest---------------------------------------------------------
//-------------------------------------------------------5test-------------------------------------------------------------      
  digitalWrite(42 , HIGH);
  delay(dely);
  if (digitalRead(26) == 0 && digitalRead(32)== 0 && digitalRead(24) == 0  && digitalRead(34) == 0)
    {
      Serial.println("pin5 is not shorted with any other pins");
      fivet = 1;
      Serial.println(fivet);
    }
  else  
    {
      Serial.println("pin5 is shorted with other pins");
      fivet = 0;
      Serial.println(fivet);
    }
    digitalWrite(42 , LOW);
//----------------------------------------------------End of 5test--------------------------------------------------------
//-------------------------------------------------------3test------------------------------------------------------------
  digitalWrite(44 , HIGH);
  delay(dely);
  if (digitalRead(32)== 0 && digitalRead(34) == 0 && digitalRead(22) == 0)
    {
      Serial.println("pin3 is not shorted with any other pins");
      threet = 1;
      Serial.println(threet);
    }
  else  
    {
      Serial.println("pin3 is shorted with other pins");
      threet = 0;
      Serial.println(threet);
    }
    digitalWrite(44 , LOW);
//-------------------------------------------------------End of 3test-----------------------------------------------------
pinMode(OUT_PIN, OUTPUT);
pinMode(IN_PIN, OUTPUT);//Changing back the modes of analog i/o s as previous
pinMode(IN_PIN1, OUTPUT);
//--------------------------------------------------Analysing the four tests conducted------------------------------------
      if (threet==1 && fivet==1 && St==1 && Lt==1)
       {
         Serial.println("THE CABLE HAS PASSED ALL SHORT CIRCUIT TESTS AND NO PINS ARE SHORTED");
       }
      else
       {
         Serial.println("THE CABLE HAS FAILED THE SHORT CIRCUIT TESTS PLEASE CHECK THE ZERO CONDITIONS");
       }
//--------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------
//------------------------------------------TESTING IF ANY CAPACITOR IS OPEN CIRCUITED OR SHORTED---------------------
//--------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------checking for 150pF cap---------------------------------------------
     Serial.println("TESTING IF ANY CAPACITOR IS OPEN CIRCUITED OR SHORTED");
     pinMode(IN_PIN, INPUT);//A0
     digitalWrite(OUT_PIN, HIGH);//A1
     int val = analogRead(IN_PIN);//A0
     digitalWrite(OUT_PIN, LOW);//A1
     if (val < 1000)
      {
        pinMode(IN_PIN, OUTPUT);
        float capacitance = (float)val * IN_CAP_TO_GND / (float)(MAX_ADC_VALUE - val);
        Serial.print(F("Capacitance Check Under Progress "));
        //Serial.print(capacitance, 3);
       // Serial.print(F(" pF ("));
        //Serial.print(val);
        //Serial.println(F(") "));
      }
    else
      {
        pinMode(IN_PIN, OUTPUT);
        delay(1);
        pinMode(OUT_PIN, INPUT_PULLUP);
        unsigned long u1 = micros();
        unsigned long t;
        int digVal;

          do
            {
              digVal = digitalRead(OUT_PIN);
              unsigned long u2 = micros();
              t = u2 > u1 ? u2 - u1 : u1 - u2;
            } 
          while ((digVal < 1) && (t < 400000L));

       pinMode(OUT_PIN, INPUT);  
       val = analogRead(OUT_PIN);
       digitalWrite(IN_PIN, HIGH);
       int dischargeTime = (int)(t / 1000L) * 5;
       delay(dischargeTime);   
       pinMode(OUT_PIN, OUTPUT);  
       digitalWrite(OUT_PIN, LOW);
       digitalWrite(IN_PIN, LOW);

       float capacitance = -(float)t / R_PULLUP
                              / log(1.0 - (float)val / (float)MAX_ADC_VALUE);

       Serial.print(F("Capacitance Value of 150pF is = "));
       if (capacitance > 1000.0)
        {
          Serial.print(capacitance / 1000.0, 2);
          Serial.print(F(" uF"));
        }
      else
      {
        Serial.print(capacitance, 2);
        Serial.print(F(" nF"));
      }

      Serial.print(F(" ("));
      Serial.print(digVal == 1 ? F("Normal") : F("HighVal"));
      Serial.print(F(", t= "));
      Serial.print(t);
      Serial.print(F(" us, ADC= "));
      Serial.print(val);
      Serial.println(F(")"));
    }
    while (millis() % 1000 != 0)
      ; 
//-------------------------------------------------------------------150pF CAP checking completed-------------------------------------------------
//------------------------------------------------------------------------checking for 6800pF-----------------------------------------------------     
    pinMode(IN_PIN1, INPUT);
    digitalWrite(OUT_PIN, HIGH);
    int val1 = analogRead(IN_PIN1);
    digitalWrite(OUT_PIN, LOW);

    if (val1 < 1000)
    {
      pinMode(IN_PIN1, OUTPUT);

      float capacitance = (float)val1 * IN_CAP_TO_GND / (float)(MAX_ADC_VALUE - val1);

      //Serial.print(F("Capacitance Value of 6800pF is = "));
      //Serial.print(capacitance, 3);
      //Serial.print(F(" pF ("));
      //Serial.print(val1);
      //Serial.println(F(") "));
    }
    else
    {
      pinMode(IN_PIN1, OUTPUT);
      delay(1);
      pinMode(OUT_PIN, INPUT_PULLUP);
      unsigned long u11 = micros();
      unsigned long t1;
      int digVal1;

      do
      {
        digVal1 = digitalRead(OUT_PIN);
        unsigned long u21 = micros();
        t1 = u21 > u11 ? u21 - u11 : u11 - u21;
      } while ((digVal1 < 1) && (t1 < 400000L));

      pinMode(OUT_PIN, INPUT);  
      val1 = analogRead(OUT_PIN);
      digitalWrite(IN_PIN1, HIGH);
      int dischargeTime1 = (int)(t1 / 1000L) * 5;
      delay(dischargeTime1);   
      pinMode(OUT_PIN, OUTPUT);  
      digitalWrite(OUT_PIN, LOW);
      digitalWrite(IN_PIN1, LOW);

      float capacitance1 = -(float)t1 / R_PULLUP
                              / log(1.0 - (float)val1 / (float)MAX_ADC_VALUE);

      //Serial.print(F("Capacitance Value of 6800pF is = "));
      if (capacitance1 > 1000.0)
      {
        Serial.print(capacitance1 / 1000.0, 2);
        Serial.print(F(" uF"));
      }
      else
      {
        Serial.print(capacitance1, 2);
        Serial.print(F(" nF"));
      }

      Serial.print(F(" ("));
      Serial.print(digVal1 == 1 ? F("Normal") : F("HighVal"));
      Serial.print(F(", t= "));
      Serial.print(t1);
      Serial.print(F(" us, ADC= "));
      Serial.print(val1);
      Serial.println(F(")"));
    }
    while (millis() % 1000 != 0)
      ; 
//----------------------------------------------------checking for 6800pF CAP completed------------------------------------------------
//----------------------------------------------------Analysing both the capacitor tests-----------------------------------------------
       if (val<950 && val>300 && val1>450 && val1<600)
       { Serial.println("-*-BOTH CAPACITORS OK-*-"); 
         Ct=1; 
       }
       else
       {
         Serial.println("--CAPACITORS NOT OK--");
        Ct=0; 

       }
//-----------------------------------------------------Analysis of CAP tests completed-------------------------------------------------
//----------------------------------Analysing both shrt ckt tests and CAP tests to decide the quality of cable-------------------------
    if (threet==1 && fivet==1 && St==1 && Lt==1 && Ct==1)
     {
       Serial.println("-*-CABLE OK-*-");
       digitalWrite(50,HIGH);//THE GREEN LED POSITIVE PIN  
      }
    else
     {
       Serial.println("--CABLE NOT OK--");
       digitalWrite(48,HIGH);//THE RED LED POSITIVE PIN
     }
//----------------------------------End of analysis of  both shrt ckt tests and CAP tests to decide the quality of cable---------------    
  }
  else  
    {
      Serial.println("Waiting for Cable");
    }
}
