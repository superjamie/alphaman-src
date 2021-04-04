del a1.obj
bc a1 /o /s;
del a2.obj
bc a2 /o /s;
del a3.obj
bc a3 /o /s;
del a4.obj
bc a4 /o /s;
del a5.obj
bc a5 /o /s;
del a6.obj
bc a6 /o /s;
del a7.obj
bc a7 /o /s;
del a8.obj
bc a8 /o /s;
\qc25\bin\qlink /CP:1 /f /packc /noe /ex /st:10000 a1+a2+a3+a4+a5+a6+a7+a8+alpclib;
pause
del alphaman.exe
del alpman11.zip
rename a1.exe alphaman.exe
pkzip alpman11 alphaman.exe alphaman.1 alphaman.2 alphaman.3 alphaman.4 alphaman.5 alphaman.6 readme.txt fixes.txt
call d2 a*.exe
call d2 a*.zip
