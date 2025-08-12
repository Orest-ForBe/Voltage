@echo off
set UE_PATH=E:\Programs\UE_Install\UE_5.6
set UPROJECT=E:\UnrealEngineProjects\Voltage\Voltage.uproject

"%UE_PATH%\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe" -projectfiles -project="%UPROJECT%" -game -engine -progress -log="%~dp0GenerateProjectFiles.log"

pause
