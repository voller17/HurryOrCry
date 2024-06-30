@echo off
:: Убедитесь, что пути к Unreal Engine и проекту указаны правильно
set UE_PATH=D:\UE_5.4
set PROJECT_PATH="D:\Git\HurryOrCry"
set PROJECT_NAME=HurryOrCry

:: Проверка и завершение процесса UnrealEditor.exe, если он существует
echo Checking for running UnrealEditor.exe process...
tasklist /FI "IMAGENAME eq UnrealEditor.exe" 2>NUL | find /I /N "UnrealEditor.exe">NUL
if "%ERRORLEVEL%"=="0" (
    echo UnrealEditor.exe is running. Terminating process...
    taskkill /F /IM UnrealEditor.exe
    if %errorlevel% neq 0 (
        echo Failed to terminate UnrealEditor.exe. Press any key to exit.
        pause >nul
        exit /b %errorlevel%
    )
) else (
    echo UnrealEditor.exe is not running.
)

rem Перечень процессов для проверки и закрытия
set "processes=Lightshot.exe MBAMessageCenter.exe AnyDesk.exe GameBarPresenceWriter.exe StartMenuExperienceHost.exe"

rem Проверяем наличие каждого процесса
for %%i in (%processes%) do (
    tasklist /FI "IMAGENAME eq %%i" 2>NUL | find /I /N "%%i" >NUL
    if "%ERRORLEVEL%"=="0" (
        echo Закрываем процесс %%i и его дочерние процессы...
        taskkill /F /T /IM %%i >NUL
    )
)

:: Переход в директорию Unreal Engine
cd /d %UE_PATH%\Engine\Build\BatchFiles

:: Запуск компиляции проекта
call Build.bat %PROJECT_NAME%Editor Win64 Development %PROJECT_PATH%\%PROJECT_NAME%.uproject

:: Проверка, успешно ли завершилась сборка
if %errorlevel% neq 0 (
    echo Compilation failed. Press any key to exit.
    pause >nul
    exit /b %errorlevel%
)

:: Запуск Unreal Engine Editor с проектом
start "" "%UE_PATH%\Engine\Binaries\Win64\UnrealEditor.exe" "%PROJECT_PATH%\%PROJECT_NAME%.uproject"

:: Успешная компиляция и запуск, закрытие окна
exit /b 0