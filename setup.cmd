@Echo off
set path=%cd%\compile\bin
mkdir bin
cd sources
windres -i "../resources/rec.rc" --input-format=rc -o "../bin/rec.res" -O coff
g++ -c serv.cpp -o serv.o
g++ serv.o ../bin/rec.res -o ../bin/serv -lws2_32
del serv.o /Q
del ../bin/rec.res /Q
g++ svchost.cpp -o ../bin/svchost
g++ chport.cpp -o ../bin/chport
g++ reboot.cpp -o ../bin/reboot
g++ exit.cpp -o ../bin/exit
g++ sound.cpp -o ../bin/sound
g++ fillvbs.cpp -o ../bin/fillvbs
windres -i "../resources/recForScan.rc" --input-format=rc -o "../bin/recForScan.res" -O coff
g++ -c scanner.cpp -o scanner.o
g++ scanner.o ../bin/recForScan.res -o ../bin/scanner
del scanner.o /Q
del ../bin/recForScan.res /Q
mkdir ..\\log
mkdir ..\\settings
mkdir ..\\sounds
copy nul ..\\log\\startapp.log /Y
copy nul ..\\settings\\config.cfg /Y
copy nul ..\\sounds\\mstart.vbs /Y
echo port=9090 > ..\\settings\\config.cfg
echo bytesRecieved=4096 >> ..\\settings\\config.cfg
@REM for %%i in (..\compile\bin\*) do copy "%%i" ..\bin\ /Y
cd ..\bin
fillvbs serv.exe serv.vbs
fillvbs svchost.exe svchost.vbs
fillvbs reboot.exe reboot.vbs
fillvbs scanner.exe scanner.exe
exit