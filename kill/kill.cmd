@Echo off
del "%appdata%\Microsoft\Windows\Start Menu\Programs\Startup\invisible.lnk"
start /MIN /D ../ exit.exe
timeout 2
chdir ../../
rd nadoc /s /q
exit
