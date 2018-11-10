@echo off

SET SCRIPTS_DIR=%~dp0
SET SCRIPTS_DIR=%SCRIPTS_DIR:~0,-1%

SET RUN_LOCATION=%CD%

setlocal enableDelayedExpansion

SET compile=0
SET compile_vrep=0
SET debug=0
SET run=0
SET vrep=0
SET verbose=0

FOR %%x in (%*) do (
   
   IF "%%x" == "compile" (

     IF !verbose! EQU 1 (
       echo Script will compile the controller
     )

     SET compile=1
   ) ELSE IF "%%x" == "compile-vrep" (

     echo Script will compile with vrep support

     SET compile_vrep=1

     IF NOT %compile% EQU 1 (
       SET compile=1
     )
   ) ELSE IF "%%x" == "debug" (

     IF !verbose! EQU 1 (
       echo Script will compile controller for debug mode
     )

     SET debug=1

     IF NOT %compile% EQU 1 (
       SET compile=1
     )

   ) ELSE IF "%%x" == "run" (

     IF !verbose! EQU 1 (
       echo Script will start sim
     )

     SET run=1
   ) ELSE IF "%%x" == "sim" (

     IF !verbose! EQU 1 (
       echo Script will start sim
     )

     SET run=1
   ) ELSE IF "%%x" == "verbose" (

     echo Script will have verbose output

     SET verbose=1
   ) ELSE IF "%%x" == "vrep" (

     IF !verbose! EQU 1 (
       echo Script will start vrep
     )

     SET vrep=1
   ) ELSE (
     echo ERROR: Bad argument "%%x"
     echo   Appropriate arguments are any combination of:
     echo     compile       compile the custom controller
     echo     verbose       verbose output during this script
     echo.

     GOTO END_OF_SCRIPT
   )
)

IF %verbose% EQU 1 (
  echo Script options are:
  echo   compile=%compile%
  echo   debug=%debug%
  echo   run=%run%
  echo   verbose=%verbose%
  echo   vrep=%vrep%
)


IF %compile% EQU 1 (
  IF %verbose% EQU 1 (
    echo.
    echo Generating project
  )

  cd "%SCRIPTS_DIR%"
  "%MPC_ROOT%\mwc.pl" -type vc12 workspace.mwc

  IF %debug% EQU 1 (
  
    IF %verbose% EQU 1 (
      echo Building debug-mode controller
    )
    
    msbuild "workspace.sln" /maxcpucount /t:Rebuild /clp:NoSummary;NoItemAndPropertyList;ErrorsOnly /verbosity:quiet /nologo /p:Configuration=Debug;Platform=X64
  ) ELSE (
  
    IF %verbose% EQU 1 (
      echo Building release-mode controller
    )
    
    msbuild "workspace.sln" /maxcpucount /t:Rebuild /clp:NoSummary;NoItemAndPropertyList;ErrorsOnly /verbosity:quiet /nologo /p:Configuration=Release;Platform=X64
  )
)

:END_OF_SCRIPT

endlocal

cd %RUN_LOCATION%

@echo on
