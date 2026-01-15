@echo off
echo === FULL PROJECT REBUILD ===

echo 1. Killing Unreal processes...
taskkill /f /im UnrealEditor.exe 2>nul
taskkill /f /im UE4Editor.exe 2>nul

echo 2. Cleaning build folders...
rmdir /s /q "C:\Users\laptop\Desktop\DeprivationGame\Binaries" 2>nul
rmdir /s /q "C:\Users\laptop\Desktop\DeprivationGame\Intermediate" 2>nul

echo 3. Regenerating project files...
"C:\Program Files\Epic Games\UE_5.5\Engine\Build\BatchFiles\Build.bat" -projectfiles -project="C:/Users/laptop/Desktop/DeprivationGame/DeprivationGame.uproject" -game -rocket -progress

echo 4. Building project...
"C:\Program Files\Epic Games\UE_5.5\Engine\Build\BatchFiles\Build.bat" DeprivationGameEditor Win64 Development -Project="C:/Users/laptop/Desktop/DeprivationGame/DeprivationGame.uproject" -WaitMutex -FromMsBuild

echo === DONE ===
pause