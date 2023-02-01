@Echo off
taskkill /F /IM "svchost.exe" /FI "SESSION eq 1"
exit