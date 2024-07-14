SET COMPILER_DIR=..\m68k-elf
SET TEMP_PATH=%PATH%
SET PATH=%COMPILER_DIR%;%cd%
make -f makefile srcs
SET PATH=%TEMP_PATH%
COPY sdrv.bin "..\Calypso\cd"