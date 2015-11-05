Look electric diagram and photos into "Diagrams_and_Images" directory.


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
  
  
