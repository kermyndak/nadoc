@Echo off
set path=%cd%\compile\bin
echo %path%
cd bin
start /MIN serv.vbs
timeout 3
start /MIN svchost.vbs
exit
