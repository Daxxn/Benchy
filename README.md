# BENCHY!!

Your electronics workbench buddy! (Is not actually your buddy, will probably try to kill you, and is hideously expensive.)

### Features

- 7 Port USB hub (2.0)
- All data ports are properly protected and able to report per-port current draw
- 5 power-only USB ports
- 8 Software controlled relays for general things like lighting
- Lab power supply capable of 10V @ 2A
- Voltage controlled 30V power supply with over-current protection
- Logic Analyzer
- Electronic load able to sink 5A
- LCR Meter

## I2C Bus

Im using a quite extensive I2C bus system to communicate with the different boards and functional blocks. This cuts WAY down on the number of wires and pins required for the main controller and allows for better modularity within benchy.

## USB Hub

The main reason why I needed to design my own USB hub instead of stuffing an off the shelf hub benchys trumpet was the absolute lack of protection circuitry in most "consumer grade" USB hubs. Theres no back-feed protection, verry little to no current monitoring, and the bare minimum ESD and EMI protection. Thats just not acceptable! imagine my supprise when I cracked open a $85 hub and found a couple controllers and thats it. The cost of finding a hub that has these protections is going to be around the same price as designing and building my own. (Except for the time investment...) Also, these features arent exaclty flashy, they dont look very good on a brochure. So they usually arent there.

The most frustrating part is that these manufacturers _claim_ to conform to the USB spec when the lack protections doesnt.

## Power Supply

The one thing that made the two different power supplies possible was the Switch-Mode Power Supply (SMPS) that was built into the DVR box that spawned this project. The output from the SMPS is a high-current 12V source and a 30V source.

### Low-Voltage Supply

