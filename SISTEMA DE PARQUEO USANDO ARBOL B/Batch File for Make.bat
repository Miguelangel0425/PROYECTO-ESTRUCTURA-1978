@echo off
REM Windows version of the make commands

if not exist obj mkdir obj
if not exist owners.txt type nul > owners.txt
if not exist vehicles.txt type nul > vehicles.txt
if not exist records.txt type nul > records.txt

g++ -std=c++17 -Wall -c main.cpp -o obj/main.o
g++ -std=c++17 -Wall -c ParkingSystem.cpp -o obj/ParkingSystem.o
g++ -std=c++17 -Wall -c DataEncryption.cpp -o obj/DataEncryption.o
g++ -std=c++17 -Wall -c InteractiveMenu.cpp -o obj/InteractiveMenu.o
g++ -std=c++17 -Wall -c validaciones.cpp -o obj/validaciones.o

g++ obj/main.o obj/ParkingSystem.o obj/DataEncryption.o obj/InteractiveMenu.o obj/validaciones.o -o parking_system.exe

echo Build complete!