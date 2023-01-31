@Echo off
set path=%cd%\compile\bin
echo %path%
cd bin
fillvbs bin\serv.exe ../serv.vbs
fillvbs bin\svchost.exe ../svchost.vbs
cd ..
start /MIN serv.vbs
timeout 3
start /MIN svchost.vbs
exit
