@echo off

cd /d %~dp0

del build
mkdir build

windres -i icon.rc -o "./build/icon-x86_64.o" -F pe-x86-64

windres -i icon.rc -o "./build/icon-i386.o" -F pe-i386
