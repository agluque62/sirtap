@echo off


@REM Obtener el path desde donde se está ejecutando el Ulises5000I_Install.bat
set mi_path=%~d0%~p0
echo *******************************************************************************
echo ****     Utilidad de copia Web.config version Desarrollo / Distribucion    **** 
echo ****                     Ulises  V5000-I OFFLINE                           ****
echo **** Para realizar la copia de los ficheros Web.config es necesario cerrar ****
echo **** proyecto de CD40 en Visual Studio                                     ****
echo *******************************************************************************

echo Seleccione la opcion a instalar
echo Desarrollo OFFLINE		(D)
echo Distribucion OFFLINE 		(P)
echo Salir			(S)
echo.
choice /C DPS /M "Pulse (D), (P) o (S)"
SET opcion=%errorlevel%
if errorlevel 3 goto end
if errorlevel 2 goto distribucion
if errorlevel 1 goto desarrollo

goto menu

:desarrollo
echo  Desea copiar Webs.config para desarrollo?
choice /C SN /M "Pulse (S), (N)" 
if errorlevel 2 goto end

copy /Y .\DevelopmentOffline\Web\Web.config ..\Web
copy /Y .\DevelopmentOffline\Web\Config\appSettings.config ..\Web\Config
copy /Y .\DevelopmentOffline\Servicios\Web.config ..\Servicios
echo **** Fin de la copia de ficheros Web Ulises 5000-I para desarrollo
echo Pulse una tecla para finalizar
pause
goto end

:distribucion
echo  Desea copiar Webs.config OFFLINE para distribucion?
choice /C SN /M "Pulse (S), (N)" 
if errorlevel 2 goto end

copy /Y .\DeployOffline\Web\Web.config ..\Web
copy /Y .\DeployOffline\Web\Config\appSettings.config ..\Web\Config
copy /Y .\DeployOffline\Servicios\Web.config ..\Servicios
echo **** Fin de la copia de ficheros Web Ulises 5000-I OFFLINE para distribucion
echo Pulse una tecla para finalizar
pause
goto end

:end
