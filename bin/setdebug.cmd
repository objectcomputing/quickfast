@echo off
if "A" == "A%DEBUG_PATH%" goto undefined
PATH=%DEBUG_PATH%
echo Debug mode set
goto end
:undefined
ECHO "DEBUG_PATH is undefined.  Did you run setup?"
:end
