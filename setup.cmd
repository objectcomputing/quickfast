REM Setting up QuickFAST environment

@REM QuickFAST depends on MPC V 3.6 or later. (http://www.ociweb.com/products/mpc)
@REM QuickFAST depends on BOOST V 1.36.0 or later. (http://www.boost.org/)
@REM QuickFAST depends on Xerces V3.0 or later. (http://xerces.apache.org/xerces-c/)
@REM QuickFAST.net depends on nunit [details to come later]
@REM Assumes VC9 installed in the default location (see VC_ROOT below) and VC90COMNTOOLS is set.

@REM Customize this file by setting variables to suit your environment
@REM Also you should customize QuickFAST.features to enable particular features on your system.

@echo off
REM: This avoids growing PATH and INCLUDE every time setup is run
if "a" == "a%BASE_PATH%" set BASE_PATH=%PATH%
if "a" == "a%BASE_INCLUDE%" set BASE_INCLUDE=%INCLUDE%

REM SET THE FOLLOWING VALUES HERE OR IN YOUR ENVIRONMENT
if "a" == "a%MPC_ROOT%" set MPC_ROOT=c:\MPC
if "a" == "a%XERCES_ROOT%" set XERCES_ROOT=C:\Progs\xerces-c-3.0.0-x86-windows-vc-9.0
if "a" == "a%XERCES_LIBNAME%" set XERCES_LIBNAME=xerces-c_3
if "a" == "a%BOOST_VERSION%" set BOOST_VERSION=boost_1_36_0
if "a" == "a%BOOST_ROOT%" set BOOST_ROOT=c:\boost\%BOOST_VERSION%
REM END OF VALUES TO BE SET

REM Verify setup by checking for expected files/directories
set SETUP_CHECKING=MPC_ROOT=%MPC_ROOT%
if not exist "%MPC_ROOT%\mpc.pl" goto setup_is_bad
set SETUP_CHECKING=XERCES_ROOT=%XERCES_ROOT%
if not exist "%XERCES_ROOT%\lib" goto setup_is_bad
set SETUP_CHECKING=BOOST_ROOT=%BOOST_ROOT%
if not exist "%BOOST_ROOT%\lib" goto setup_is_bad
set VCVER=9
set SETUP_CHECKING=VS90COMNTOOLS=%VS90COMNTOOLS%
if exist "%VS90COMNTOOLS%VSVARS32.BAT" goto setup_is_ok
set SETUP_CHECKING=VS80COMNTOOLS=%VS80COMNTOOLS%
if not exist "%VS80COMNTOOLS%VSVARS32.BAT" goto setup_is_bad
set VCVER=8
set SETUP_CHECKING=
goto setup_is_ok

:setup_is_bad
ECHO Setup check failed: %SETUP_CHECKING%
ECHO Edit the setup.cmd file or change environment variables
goto end

:setup_is_ok
set XERCES_LIBPATH=%XERCES_ROOT%\lib
set XERCES_INCLUDE=%XERCES_ROOT%\include
set QUICKFAST_ROOT=%CD%

if %VCVER%==9 (
  set VC_ROOT=C:\Program Files\Microsoft Visual Studio 9.0\VC\bin
  set MWC_EXTRA=-value_template toolsversion=3.5
  @call "%VS90COMNTOOLS%VSVARS32.BAT" >nul
) else (
  set VC_ROOT=C:\Program Files\Microsoft Visual Studio 8\VC\bin
  @call "%VS80COMNTOOLS%VSVARS32.BAT" >nul
)

set RELEASE_PATH=%QUICKFAST_ROOT%\bin;%QUICKFAST_ROOT%\Output\Release;%MPC_ROOT%;%BOOST_ROOT%\lib;%QUICKFAST_ROOT%\lib;%XERCES_ROOT%\bin;%BASE_PATH%
set DEBUG_PATH=%QUICKFAST_ROOT%\bin;%QUICKFAST_ROOT%\Output\Debug;%MPC_ROOT%;%BOOST_ROOT%\lib;%QUICKFAST_ROOT%\lib;%XERCES_ROOT%\bin;%BASE_PATH%
set PATH=%DEBUG_PATH%
set INCLUDE=%BOOST_ROOT%;%XERCES_ROOT%\include;%BASE_INCLUDE%
PATH=%QUICKFAST_ROOT%\bin;%QUICKFAST_ROOT%\Output\Release;%QUICKFAST_ROOT%\Output\Debug;%MPC_ROOT%;%BOOST_ROOT%\lib;%QUICKFAST_ROOT%\lib;%XERCES_ROOT%\bin;%BASE_PATH%
set INCLUDE=%BOOST_ROOT%;%XERCES_ROOT%\include;%BASE_INCLUDE%
title QuickFAST
:end


