### Day 1 (18 June 2025)
- First commit, used hello_world example project from ESP-IDF.
- Setup main component
- Falied to setup python virtual environment, so can't use idf.py. Gotta fix this next.


### Day 2 (23 June 2025)
- The /dev/ttyUSB0 group permission is giving me so much trouble to flash the program to the ESP32. But I got it handled and everything is working nicely now!
- The C++ ESP32 program works like a charm and I'm well set up to for development!
- The very first step is to set up and get the RTC working correctly to capture real time.
- However, I know very little about this so I need to do more research on the availble options... tomorrow...


### Day 3 (26 June 2025)
- Setup clock initialization using SNTP with ESP32's RTC.
- Setup Wi-fi initialization for ESP32 (Need documentation tho...)
*** Todo: Document Wifi.cpp


### Day 4 (29 June 2025)
- Successfully connected to home wi-fi and sync SNTP 
- Implemented and documented Display Driver (ILI9488) init for the LCD screen.
- The display driver don't have any graphic render or text drawing capabillity yet.
*** TODO: Implement text drawing for display driver