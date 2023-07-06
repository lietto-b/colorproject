@echo off
echo Create directory.. ./Produce/
MD Produce
echo Create directory.. 
echo	./Produce/CUT, 
echo	./Produce/CUTtxt, 
echo	./Produce/Masks, 
echo	./Produce/ProtocolsJSON, 
echo	./Produce/TempImage,
echo	./Produce/BAD
cd Produce/
MD CUT
MD CUTtxt
MD Masks
MD ProtocolsJSON
MD TempImage
MD BAD
echo All necessary directories are created.

pause
