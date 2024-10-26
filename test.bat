@echo off
setlocal enabledelayedexpansion

:COMPILATION
set /p choix="Voulez-vous compiler le projet ? (O/N) : "

if /i "%choix%"=="O" (
    echo Compilation en cours...
    gcc projet.c -o projet.exe
    if %errorlevel% neq 0 (
        echo Erreur lors de la compilation.
        pause
        exit /b 1
    )
    echo Compilation reussie.
) else if /i "%choix%"=="N" (
    echo Compilation sautee.
) else (
    echo Reponse invalide. Veuillez entrer 'O' pour Oui ou 'N' pour Non.
    goto COMPILATION
)

echo.
echo Debut des tests...
echo.

for /f %%a in ('dir /b "tests\in-sp*.txt" ^| find /c /v ""') do set filecount=%%a
echo Nombre de tests detectes dans le dossier tests : %filecount%
echo.


for /L %%i in (1,1,%filecount%) do (
    echo ------------------------------
    echo Test %%i :
    
    projet.exe < tests/in-sp%%i.txt > output/test%%i.txt
    
    fc /w "tests\out-sp%%i.txt" "output\test%%i.txt"
    if !errorlevel! equ 0 (
        echo Test %%i : PASSE
    ) else (
        echo Test %%i : ECHEC
		pause
    )
    echo.
)

echo Tous les tests sont termines.
pause
