@ECHO OFF
:: Sincroniza la tabla de operador de la base de datos new_cd40_offline con la base de datos new_cd40_trans

::Parametros
:: %1:  usuario de conexion a la BD
:: %2:  Contraseña del usuario de conexion a la BD
:: %3:  Nombre del host
:: %4:  nombre de la Base de datos origen
:: %5:  nombre de la Base de datos destino
:: %6:  lista de tablas a actualizar


SET BD_ORIGEN=%4

SET BD_DESTINO=%5

SET FICHERO_LOG=%~dp0Trans_Oper.log

  :: Se exportan los cambios de la BD de configuracion operadores (_trans) se aplican en la BD de configuración offline
  ECHO %DATE% %TIME% - Aplica modificación en la BD destino %BD_DESTINO%. Tablas:%6 >>%FICHERO_LOG% 
  "C:\Program Files\MySQL\MySQL Server 5.6\bin\mysqldump.exe" --user=%1 --password=%2 --host=%3 --skip-triggers --opt --ignore-table=%4.historicoincidencias %4 %~6 | "C:\Program Files\MySQL\MySQL Server 5.6\bin\mysql.exe" --host=%3 --user=%1 --password=%2 -C %5

ECHO %DATE% %TIME% FIN DEL PROCESO ACTUALIZA OPERADOR >>%FICHERO_LOG% 

exit 0





