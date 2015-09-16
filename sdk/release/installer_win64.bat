@echo OFF

echo "Quotek SDK 2.0.0 Installer"
echo "=========================="

mkdir "C:\Program Files\quotek\sdk"

xcopy doc "C:\Program Files\quotek\sdk\doc\" /e /y
xcopy include "C:\Program Files\quotek\sdk\include\" /e /y
xcopy lib\win64 "C:\Program Files\quotek\sdk\lib\" /e /y

copy lib\win64\libcurl.dll C:\Windows\System\

echo "Quotek SDK installed succesfully, press any key to continue.."
pause

