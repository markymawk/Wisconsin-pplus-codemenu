for %%f in (.\Stagelist*.asm) do (
    ..\..\..\GCTRealMate.exe -q %%f
)

imdisk -a -t file -m Z: -o rem -f "C:\Users\m\Desktop\emulator\dolphin 2018\sd\dev wi pplus.raw"

robocopy "C:\Users\m\Desktop\Wisco PM\wisco build P+\build\Project+\pf\stage\stagelist" "Z:\Project+\pf\stage\stagelist" *

imdisk -D -m Z: