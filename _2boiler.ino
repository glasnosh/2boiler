/*
 
2Boiler
JoseM - 2014,2015
v 0.01 - Initial Release
 
This sketch controls 2 relays. Inputs are the status of 2 thermostats.
My house's heating system is composed by 2 boilers:
  - A Diesel boiler with electronical start.
  - A Wood boiler with no electronic parts.
Diesel boiler is a clasical boiler. It is equipped with its own water pump and It starts/stops pumpping hot water to the heating pipes according to the status of an electronic contact. Usually, this contact is connected to a wall thermostat.
Wood boiler has its own water pump. Usually, the pump is controlled by a thermostat fixed to a closer pipe. If water is hot enough (+35C), the pump starts to move.

...but my situation is different. I have both boilers connected to the same pipe's circuit so it is neccesary to ensure many constraints:
  - Only one water pump can be working anytime. If both pumps are pumpping hot water to the pipes, there is a real security problem.
  - If the wood boiler is On (wood burning), its water pump MUST be working (if water is +35C) because of security reasons.
  
So, we have two thermostats: One of then is a wall thermostat placed somewhere in the living room and the other is glued to a pipe closer to the wood boiler (detecting burning wood heating the water).
We want to prioritize the wood boiler. The diesel boiler is only used for supporting the wood boiler if this one is off or if it can't heat the water up to 35C.

An output relay can start/stop the diesel boiler and the other one can start/stop the water pump of the wood boiler.

Here is the status table:
  _____________________________________________________________________________________
  |       I   N    P    U    T    S   ||          O   U   T   P   U   T   S           |
  |___________________________________||______________________________________________|
  | Wall Thermostat | Wood Thermostat || Diesel Boiler Relay | Wood boiler Pump Relay |
  |_________________|_________________||_____________________|________________________|
  |     OFF         |       OFF       ||       OFF           |       OFF              |
  |_________________|_________________||_____________________|________________________|  
  |     OFF         |       ON        ||       OFF           |       ON               |
  |_________________|_________________||_____________________|________________________|
  |     ON          |       OFF       ||       ON            |       OFF              |
  |_________________|_________________||_____________________|________________________|
  |     ON          |       ON        ||       OFF           |       ON               |
  |_________________|_________________||_____________________|________________________| 
  
  
  
*/


//Pinout

//Digital
int PIN_relay_diesel = 3;      // Relay controlling diesel boiler status
int PIN_relay_wood = 2;        // Relay controlling the pump for wood boiler
int PIN_buzzer = 12;            // Buzzer (sound really sucks!!)
int PIN_thermo_diesel = 11;      // Input for wall thermostat (For diesel boiler)
int PIN_thermo_wood = 10;        // Input for pump thermostat (For wood boiler)
int PIN_ledState_off = 6;       // Led on if any boiler are off
int PIN_ledState_diesel = 5;    // Led on if diesel boiler is on
int PIN_ledState_wood = 7;      // Led on if wood boiler is on


//Status vars
int current_status_boiler_diesel;   // Current state for diesel boiler
int current_status_boiler_wood;     // Current state for wood boiler's pump 
int new_status_boiler_diesel;       // New/Next state for diesel boiler
int new_status_boiler_wood;         // New/Next state for wood boiler's pump
int new_status_ledState_off;        // New/Next state for led
int new_status_ledState_diesel;     // New/Next state for led
int new_status_ledState_wood;       // New/Next state for led
int status_thermo_diesel;           // Var for capturing input status for ambient/diesel thermostat
int status_thermo_wood;             // Var for capturing input status for wood thermostat

//Other vars
int delaytime = 1000;          //Default delay time for led blink


void setup(){
  Serial.begin(9600);
  
  //PinModes
  pinMode(PIN_relay_diesel,OUTPUT);
  pinMode(PIN_relay_wood,OUTPUT);
  pinMode(PIN_buzzer,OUTPUT);
  pinMode(PIN_ledState_off,OUTPUT);
  pinMode(PIN_ledState_diesel,OUTPUT);
  pinMode(PIN_ledState_wood,OUTPUT);
  pinMode(PIN_thermo_diesel,INPUT);
  pinMode(PIN_thermo_wood,INPUT);
  
  //Test-Start blink and bell sequence
  digitalWrite(PIN_ledState_off, LOW);
  digitalWrite(PIN_ledState_diesel, LOW);
  digitalWrite(PIN_ledState_wood, LOW);
  delay(delaytime);
  digitalWrite(PIN_ledState_off, HIGH);
  digitalWrite(PIN_ledState_diesel, LOW);
  digitalWrite(PIN_ledState_wood, LOW);
  delay(delaytime);
  digitalWrite(PIN_ledState_off, LOW);
  digitalWrite(PIN_ledState_diesel, HIGH);
  digitalWrite(PIN_ledState_wood, LOW);
  delay(delaytime);
  digitalWrite(PIN_ledState_off, LOW);
  digitalWrite(PIN_ledState_diesel, LOW);
  digitalWrite(PIN_ledState_wood, HIGH);
  delay(delaytime);
  digitalWrite(PIN_ledState_off, LOW);
  digitalWrite(PIN_ledState_diesel, LOW);
  digitalWrite(PIN_ledState_wood, LOW);
  delay(delaytime);
  digitalWrite(PIN_buzzer, HIGH); delay(50);    digitalWrite(PIN_buzzer, LOW);
  
  //Set boiler-relays DOWN  
  digitalWrite(PIN_relay_diesel, LOW);
  digitalWrite(PIN_relay_wood, LOW);
  
  //Initial Status is 0 for any boiler and led for state "off" is on
  current_status_boiler_diesel = 0;
  current_status_boiler_wood = 0;
  digitalWrite(PIN_ledState_off, HIGH);
  
  //Wait for 5 seconds before start the loop (safety for the relays)
  delay(5000);
  
}//setup



void loop() {
  
    //Read thermostats status
    status_thermo_diesel = digitalRead(PIN_thermo_diesel);  //Leer estado de 'PIN_thermo_Diesel'
    status_thermo_wood = digitalRead(PIN_thermo_wood);      //Leer estado de 'PIN_thermo_Wood'

    //Combination of Thermostats' status defines new status for boilers
    if ( (status_thermo_diesel == 0) && (status_thermo_wood == 0) ) {  //Diesel Boiler OFF, Wood Pump OFF
      new_status_boiler_diesel = 0;
      new_status_boiler_wood = 0;
      new_status_ledState_off = 1;
      new_status_ledState_diesel = 0;
      new_status_ledState_wood = 0;
    }else
     if ( (status_thermo_diesel == 0) && (status_thermo_wood == 1) ) {  //Diesel Boiler OFF, Wood Pump ON
      new_status_boiler_diesel = 0;
      new_status_boiler_wood = 1;
      new_status_ledState_off = 0;
      new_status_ledState_diesel = 0;
      new_status_ledState_wood = 1;
    } else
    if ( (status_thermo_diesel == 1) && (status_thermo_wood == 0) ) {  //Diesel Boiler ON, Wood Pump OFF
      new_status_boiler_diesel = 1;
      new_status_boiler_wood = 0;
      new_status_ledState_off = 0;
      new_status_ledState_diesel = 1;
      new_status_ledState_wood = 0;
    } else
    if ( (status_thermo_diesel == 1) && (status_thermo_wood == 1) ) {  //Diesel Boiler ON, Wood Pump ON
      new_status_boiler_diesel = 0;
      new_status_boiler_wood = 1;
      new_status_ledState_off = 0;
      new_status_ledState_diesel = 0;
      new_status_ledState_wood = 1;

    } else  { //This should be an error. The protocol is poweroff any blink every leds and poweron Wood Pump Relay (Security Reasons)
       while (1){
           digitalWrite(PIN_relay_diesel, LOW);
           digitalWrite(PIN_relay_wood, HIGH);
           digitalWrite(PIN_buzzer, HIGH);  delay(50);  digitalWrite(PIN_buzzer, LOW); delay(50);
       }//while
      } 

    //Execute transition if neccesary
    if ( (new_status_boiler_diesel != current_status_boiler_diesel) || (new_status_boiler_wood != current_status_boiler_wood) ){  
       // Pitido de inicio de transicion
      digitalWrite(PIN_buzzer, HIGH); delay(50);    digitalWrite(PIN_buzzer, LOW);
      
      // Set relays OFF
      digitalWrite(PIN_relay_diesel, LOW);
      digitalWrite(PIN_relay_wood, LOW);
      delay(1000);      //Wait sometime for water stabilization
      
       // Blink implicated leds
      if (new_status_ledState_off == 1) {
        digitalWrite(PIN_ledState_off, HIGH); delay (1000);
        digitalWrite(PIN_ledState_off, LOW); delay (1000);
        digitalWrite(PIN_ledState_off, HIGH); delay (1000);
        digitalWrite(PIN_ledState_off, LOW); delay (1000);
        digitalWrite(PIN_ledState_off, HIGH); delay (1000);
        digitalWrite(PIN_ledState_off, LOW); delay (1000);
        digitalWrite(PIN_ledState_off, HIGH); delay (1000);
      }  
      
      if (new_status_ledState_diesel == 1){
        digitalWrite(PIN_ledState_diesel, HIGH); delay (1000);
        digitalWrite(PIN_ledState_diesel, LOW); delay (1000);
        digitalWrite(PIN_ledState_diesel, HIGH); delay (1000);
        digitalWrite(PIN_ledState_diesel, LOW); delay (1000);
        digitalWrite(PIN_ledState_diesel, HIGH); delay (1000);
        digitalWrite(PIN_ledState_diesel, LOW); delay (1000);
        digitalWrite(PIN_ledState_diesel, HIGH); delay (1000);
      }
    
      if (new_status_ledState_wood == 1){
        digitalWrite(PIN_ledState_wood, HIGH); delay (1000);
        digitalWrite(PIN_ledState_wood, LOW); delay (1000);
        digitalWrite(PIN_ledState_wood, HIGH); delay (1000);
        digitalWrite(PIN_ledState_wood, LOW); delay (1000);
        digitalWrite(PIN_ledState_wood, HIGH); delay (1000);
        digitalWrite(PIN_ledState_wood, LOW); delay (1000);
        digitalWrite(PIN_ledState_wood, HIGH); delay (1000);
      }
       


            
      // Set relays in its final state
      digitalWrite(PIN_relay_diesel, new_status_boiler_diesel);
      digitalWrite(PIN_relay_wood, new_status_boiler_wood);
      
      // Update status vars
      current_status_boiler_diesel = new_status_boiler_diesel;
      current_status_boiler_wood = new_status_boiler_wood;
            
      // Let leds static
      digitalWrite(PIN_ledState_off, new_status_ledState_off);
      digitalWrite(PIN_ledState_diesel, new_status_ledState_diesel);
      digitalWrite(PIN_ledState_wood, new_status_ledState_wood);
      
   
      
      //Buzzer
      digitalWrite(PIN_buzzer, HIGH);  delay(50);  digitalWrite(PIN_buzzer, LOW); delay(50);
      digitalWrite(PIN_buzzer, HIGH);  delay(50);  digitalWrite(PIN_buzzer, LOW); delay(50);
      digitalWrite(PIN_buzzer, HIGH);  delay(50);  digitalWrite(PIN_buzzer, LOW); delay(50);
    }//if

  //Loop every 10 seconds
  delay(10000);
}//loop
