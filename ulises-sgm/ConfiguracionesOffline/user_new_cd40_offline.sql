SET @USR='new_cd40_offline';
SET @BD='new_cd40_offline';
SET @PWD='cd40';


SET @sentencia = CONCAT('CREATE DATABASE ', @BD);
SELECT CONCAT('1. ',@sentencia) AS Operacion;
PREPARE STMT FROM @sentencia;
EXECUTE STMT;
DEALLOCATE PREPARE STMT;

SET @sentencia = CONCAT('CREATE USER ', QUOTE(@usr),'@',QUOTE('%'),' IDENTIFIED BY ',QUOTE(@pwd));
SELECT CONCAT('2. ',SUBSTR(@sentencia,1,INSTR(@sentencia,'IDENTIFIED BY ')),'IDENTIFIED BY ***') AS Operacion;
PREPARE STMT FROM @sentencia;
EXECUTE STMT;
DEALLOCATE PREPARE STMT;

SET @sentencia = CONCAT('GRANT ALL PRIVILEGES ON ',@bd,'.* TO ', QUOTE(@usr),'@',QUOTE('%'));
SELECT CONCAT('3. ',@sentencia) AS Operacion;
PREPARE STMT FROM @sentencia;
EXECUTE STMT;
DEALLOCATE PREPARE STMT;

SET @sentencia = CONCAT('GRANT SELECT ON *.* TO ', QUOTE(@usr),'@',QUOTE('%'));
SELECT CONCAT('5. ',@sentencia) AS Operacion;
PREPARE STMT FROM @sentencia;
EXECUTE STMT;
DEALLOCATE PREPARE STMT;

-- Para el privilegio FILE (acceso a los ficheros en el servidor) creamos otro usuario historicosexp@localhost
SET @sentencia = CONCAT('CREATE USER ', QUOTE(@usr),'@',QUOTE('localhost'),' IDENTIFIED BY ',QUOTE(@pwd));
SELECT CONCAT('6. ',SUBSTR(@sentencia,1,INSTR(@sentencia,'IDENTIFIED BY ')),'IDENTIFIED BY ***') AS Operacion;
PREPARE STMT FROM @sentencia;
EXECUTE STMT;
DEALLOCATE PREPARE STMT;

SET @sentencia = CONCAT('GRANT ALL PRIVILEGES ON ',@bd,'.* TO ', QUOTE(@usr),'@',QUOTE('localhost'));
SELECT CONCAT('7. ',@sentencia) AS Operacion;
PREPARE STMT FROM @sentencia;
EXECUTE STMT;
DEALLOCATE PREPARE STMT;

SET @sentencia = CONCAT('GRANT SELECT ON *.* TO ', QUOTE(@usr),'@',QUOTE('localhost'));
SELECT CONCAT('9. ',@sentencia) AS Operacion;
PREPARE STMT FROM @sentencia;
EXECUTE STMT;
DEALLOCATE PREPARE STMT;

SET @sentencia = CONCAT('GRANT FILE ON *.* TO ', QUOTE(@usr),'@',QUOTE('localhost'));
SELECT CONCAT('10. ',@sentencia) AS Operacion;
PREPARE STMT FROM @sentencia;
EXECUTE STMT;
DEALLOCATE PREPARE STMT;

FLUSH PRIVILEGES;
