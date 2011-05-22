


set BIN_FOLDER=H:\Toolkit\TrunkBuild\bin\Release
set OUTPUT_FOLDER=Temp
set DATA_FLAG=true
set ALL_PLUGINS_FLAG=false
set CARDIO_FLAG=true
set ANGIO_FLAG=false

rmdir %OUTPUT_FOLDER% /S/Q
md %OUTPUT_FOLDER%

copy %BIN_FOLDER%\Gimias.exe %OUTPUT_FOLDER%
copy %BIN_FOLDER%\Gimias_4Gb.bat %OUTPUT_FOLDER%
copy %BIN_FOLDER%\*.dll %OUTPUT_FOLDER%
copy %BIN_FOLDER%\*.xml %OUTPUT_FOLDER%
xcopy %BIN_FOLDER%\resource %OUTPUT_FOLDER%\resource /S /I
xcopy %BIN_FOLDER%\Copyrights %OUTPUT_FOLDER%\Copyrights /S /I

IF %ALL_PLUGINS_FLAG%==true GOTO ALL_PLUGINS
IF %CARDIO_FLAG%==true GOTO CARDIO_PLUGINS
IF %ANGIO_FLAG%==true GOTO ANGIO_PLUGINS

GOTO END

:ALL_PLUGINS
if %DATA_FLAG%==true xcopy %BIN_FOLDER%\data %OUTPUT_FOLDER%\data /S /I
xcopy %BIN_FOLDER%\plugins %OUTPUT_FOLDER%\plugins /S /I /EXCLUDE:MyExcludes.txt
GOTO :END

:CARDIO_PLUGINS
xcopy %BIN_FOLDER%\data\Cardio %OUTPUT_FOLDER%\data\Cardio /S /I
xcopy %BIN_FOLDER%\plugins\CardiacSegmentationPlugin %OUTPUT_FOLDER%\plugins\CardiacSegmentationPlugin /S /I /EXCLUDE:MyExcludes.txt
xcopy %BIN_FOLDER%\plugins\dicomplugin %OUTPUT_FOLDER%\plugins\dicomplugin /S /I /EXCLUDE:MyExcludes.txt
xcopy %BIN_FOLDER%\plugins\cardiacbullseyeplugin %OUTPUT_FOLDER%\plugins\cardiacbullseyeplugin /S /I /EXCLUDE:MyExcludes.txt
xcopy %BIN_FOLDER%\plugins\cardiacquantificationplugin %OUTPUT_FOLDER%\plugins\cardiacquantificationplugin /S /I /EXCLUDE:MyExcludes.txt
GOTO :END

:ANGIO_PLUGINS
xcopy %BIN_FOLDER%\data\Angio %OUTPUT_FOLDER%\data\Angio /S /I
xcopy %BIN_FOLDER%\plugins\angiosegmentationplugin %OUTPUT_FOLDER%\plugins\angiosegmentationplugin /S /I /EXCLUDE:MyExcludes.txt
GOTO :END

:END

cd %OUTPUT_FOLDER%
Gimias_4Gb.bat
cd ..

pause

GOTO :EOF
