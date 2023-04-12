SET @USR='new_cd40_offline';
SET @BD='new_cd40_offline';
SET @PWD='cd40';


SET @sentencia = CONCAT('DROP USER ', QUOTE(@usr),'@',QUOTE('%'));
SELECT CONCAT('1. ',@sentencia) AS Operacion;
PREPARE STMT FROM @sentencia;
EXECUTE STMT;
DEALLOCATE PREPARE STMT;

-- Para el privilegio FILE (acceso a los ficheros en el servidor) creamos otro usuario historicosexp@localhost
SET @sentencia = CONCAT('DROP USER ', QUOTE(@usr),'@',QUOTE('localhost'));
SELECT CONCAT('2. ',@sentencia) AS Operacion;
PREPARE STMT FROM @sentencia;
EXECUTE STMT;
DEALLOCATE PREPARE STMT;

SET @sentencia = CONCAT('DROP DATABASE ', @BD);
SELECT CONCAT('3. ',@sentencia) AS Operacion;
PREPARE STMT FROM @sentencia;
EXECUTE STMT;
DEALLOCATE PREPARE STMT;

FLUSH PRIVILEGES;
