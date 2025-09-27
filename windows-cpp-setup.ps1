# install Chocolatey -- package manager for powershell
Set-ExecutionPolicy Bypass -Scope Process -Force; 
iex ((New-Object System.Net.WebClient).DownloadString('https://community.chocolatey.org/install.ps1'));

# install gcc, g++
choco install mingw -y;

# install make
choco install make;