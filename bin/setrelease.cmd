@echo off
if "A" == "A%RELEASE_PATH%" goto undefined
PATH=%RELEASE_PATH%
echo Release mode set
goto end
:undefined
ECHO "RELEASE_PATH is undefined.  Did you run setup?"
:end
