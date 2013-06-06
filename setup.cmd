@REM Copyright (c) 2009, 2010 Object Computing, Inc.
@REM All rights reserved.
@REM See the file license.txt for licensing information.

REM Setting up QuickFAST environment

@REM QuickFAST depends on MPC V 3.6 or later. (http://www.ociweb.com/products/mpc)
@REM QuickFAST depends on BOOST V 1.36.0 or later. (http://www.boost.org/)
@REM           Visual Studio 2010 requires BOOST V 1.43.0 or later
@REM QuickFAST depends on Xerces V3.0 or later. (http://xerces.apache.org/xerces-c/)
@REM Assumes VC8, VC9, or VC10 installed in the default location (see VC_ROOT below) and VC..COMNTOOLS is set.

@REM Customize this file by setting variables to suit your environment
@REM Also you should customize QuickFAST.features to enable particular features on your system.
@REM Delete the following line when you finishing customizing this file.
@echo See remarks in %0 for information about setting your build environment
@echo off
REM =====================================================================================
REM EDIT THE FOLLOWING LINES OR SET THESE VALUES IN YOUR ENVIRONMENT BEFORE RUNNING SETUP
if "a" == "a%MPC_ROOT%" set MPC_ROOT=c:\MPC
if "a" == "a%XERCESCROOT%" set XERCESCROOT=C:\Progs\xerces-c-3.0.1-x86-windows-vc-9.0
if "a" == "a%BOOST_VERSION%" set BOOST_VERSION=boost_1_43_0
if "a" == "a%BOOST_ROOT%" set BOOST_ROOT=c:\boost\%BOOST_VERSION%
REM END OF VALUES TO BE SET
REM =====================================================================================

REM Microsoft moved 32 bit apps to a new program files directory on 64 bit systems
set PROGRAM_FILES_X86=Program Files
if exist "C:\Program Files (x86)" set PROGRAM_FILES_X86=Program Files (x86)

REM Verify setup by checking for expected files/directories
set SETUP_CHECKING=MPC_ROOT=%MPC_ROOT%
if not exist "%MPC_ROOT%\mpc.pl" goto setup_is_bad

set SETUP_CHECKING=XERCESCROOT=%XERCESCROOT%
if not exist "%XERCESCROOT%\lib" goto setup_is_bad

set SETUP_CHECKING=BOOST_ROOT=%BOOST_ROOT%
if not exist "%BOOST_ROOT%\boost" goto setup_is_bad

set SETUP_CHECKING=BOOST_ROOT lib=%BOOST_ROOT%\lib
if not exist "%BOOST_ROOT%\lib" goto setup_is_bad

REM Find visual studio.  
REM You can short-circuit this by setting VCVER before running this
REM However this also avoids the check to see if VC is installed in the expected place.
set SETUP_CHECKING=Setup checking visual studio common tools
if not "a" == "a%VCVER%" goto setup_is_ok
set VCVER=10
set SETUP_CHECKING=VS100COMNTOOLS=%VS100COMNTOOLS%
if exist "%VS100COMNTOOLS%VSVARS32.BAT" goto setup_is_ok
set VCVER=9
set SETUP_CHECKING=VS90COMNTOOLS=%VS90COMNTOOLS%
if exist "%VS90COMNTOOLS%VSVARS32.BAT" goto setup_is_ok
set VCVER=8
set SETUP_CHECKING=VS80COMNTOOLS=%VS80COMNTOOLS%
if exist "%VS80COMNTOOLS%VSVARS32.BAT" goto setup_is_ok
REM goto setup_is_bad  (you are here) 

:setup_is_bad
ECHO Setup check failed: %SETUP_CHECKING%
ECHO Edit the setup.cmd file or change environment variables
goto end

:setup_is_ok
set SETUP_CHECKING=

set XERCESLIB=%XERCESCROOT%\lib
set QUICKFAST_ROOT=%CD%

rem must be outside the if because of the parens in the directory name [what was microsoft thinking?]
if %VCVER%==10 goto isVC10
if %VCVER%==9 goto isVC9
if %VCVER%==8 goto isVC8
echo "UNKNOWN OR UNSUPPORTED VISUAL STUDIO VERSION: %VCVER%
goto end

:isVC10
set VC_ROOT=C:\%PROGRAM_FILES_X86%\Microsoft Visual Studio 10.0\VC\bin
@call "%VS100COMNTOOLS%VSVARS32.BAT" >nul
goto vcIsSet
:isVC9
set VC_ROOT=C:\%PROGRAM_FILES_X86%\Microsoft Visual Studio 9.0\VC\bin
call "%VS90COMNTOOLS%VSVARS32.BAT" >nul
goto vcIsSet
:isVC8
set VC_ROOT=C:\%PROGRAM_FILES_X86%\Microsoft Visual Studio 8\VC\bin
call "%VS80COMNTOOLS%VSVARS32.BAT" >nul
:vcIsSet

REM: This avoids growing PATH and INCLUDE every time setup is run
if "a" == "a%BASE_PATH%" set BASE_PATH=%PATH%
if "a" == "a%BASE_INCLUDE%" set BASE_INCLUDE=%INCLUDE%

set RELEASE_PATH=%QUICKFAST_ROOT%\bin;%QUICKFAST_ROOT%\Output\Release;%MPC_ROOT%;%BOOST_ROOT%\lib;%QUICKFAST_ROOT%\lib;%XERCESCROOT%\bin;%BASE_PATH%
set DEBUG_PATH=%QUICKFAST_ROOT%\bin;%QUICKFAST_ROOT%\Output\Debug;%MPC_ROOT%;%BOOST_ROOT%\lib;%QUICKFAST_ROOT%\lib;%XERCESCROOT%\bin;%BASE_PATH%
set PATH=%DEBUG_PATH%
set INCLUDE=%BOOST_ROOT%;%XERCESCROOT%\include;%BASE_INCLUDE%

title QuickFAST
:end


