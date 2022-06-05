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

### I2C Bus

Im using a quite extensive I2C bus system to communicate with the different boards and functional blocks. This cuts WAY down on the number of wires and pins required for the main controller and allows for better modularity within benchy.