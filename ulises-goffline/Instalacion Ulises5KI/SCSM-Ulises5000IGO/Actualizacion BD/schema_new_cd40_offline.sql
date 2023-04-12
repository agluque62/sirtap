USE new_cd40_offline;
SET @ConfOnline='new_cd40';

DELIMITER $$
-- CONFIGURACIONES:
-- Tabla con la lista de configuraciones guardadas en el sistema
DROP TABLE IF EXISTS operadores
$$
DROP TABLE IF EXISTS LogProcesos
$$
DROP TABLE IF EXISTS CONFIGURACIONES
$$
DROP VIEW IF EXISTS HistoricoIncidencias
$$

CREATE TABLE CONFIGURACIONES 
           ( NOMBRE       VARCHAR(100)
           , CONF_EDICION VARCHAR(100)
           , TIPO         VARCHAR(100) NOT NULL DEFAULT 'OFFLINE'
           , DESCRIPCION  VARCHAR(100), 
PRIMARY KEY (NOMBRE),
CONSTRAINT chkTipo CHECK (TIPO IN ('ONLINE','OFFLINE')))
$$

CREATE TABLE LogProcesos
           ( Id               varchar(50) NOT NULL
           , PID              int(10)
           , IdSistema        varchar(32)
           , FechaHora        datetime DEFAULT CURRENT_TIMESTAMP
           , FechaHoraLeido   datetime DEFAULT NULL
           , Usuario          varchar(32) DEFAULT NULL
           , Descripcion      varchar(500) DEFAULT NULL
           , Activo           TINYINT(1) DEFAULT NULL,
PRIMARY KEY (Id),
KEY LogProcesosFecha_Idx (FechaHora))
$$

CREATE TABLE operadores
           ( IdOperador       varchar(32) NOT NULL
           , IdSistema        varchar(32) NOT NULL
           , Clave            varchar(10) DEFAULT NULL
           , NivelAcceso      int(1) unsigned DEFAULT NULL
           , Nombre           varchar(32) DEFAULT NULL
           , Apellidos        varchar(32) DEFAULT NULL
           , Telefono         varchar(32) DEFAULT NULL
           , FechaUltAcceso   date DEFAULT NULL
           , Comentarios      varchar(100) DEFAULT NULL
           , TimeoutSesion    int(11) DEFAULT '30'
		   , SesionActiva     int(1) DEFAULT '0'
		   , SesionEstado     int(1) DEFAULT '0',
PRIMARY KEY (IdOperador, IdSistema),
KEY Operadores_FKIndex1 (IdSistema))
$$

SET @sql = CONCAT('INSERT INTO OPERADORES '
                 ,    ' ( IdOperador'
                 ,    ' , IdSistema'
                 ,    ' , Clave'
                 ,    ' , NivelAcceso'
                 ,    ' , Nombre'
                 ,    ' , Apellidos'
                 ,    ' , Telefono'
                 ,    ' , FechaUltAcceso'
                 ,    ' , Comentarios'
                 ,    ' , TimeoutSesion'
                 ,    ' , SesionActiva'
                 ,    ' , SesionEstado'
                 ,    ' )'
                 ,' SELECT IdOperador'
                 ,   ' , IdSistema'
                 ,   ' , Clave'
                 ,   ' , NivelAcceso'
                 ,   ' , Nombre'
                 ,   ' , Apellidos'
                 ,   ' , Telefono'
                 ,   ' , FechaUltAcceso'
                 ,   ' , Comentarios'
                 ,   ' , TimeoutSesion'
                 ,   ' , SesionActiva'
                 ,   ' , SesionEstado'
                 ,' FROM ', @ConfOnline, '.OPERADORES')
$$
PREPARE STMT FROM @sql
$$
EXECUTE STMT
$$
DEALLOCATE PREPARE STMT
$$

# Creamos la vista HistoricoIncidencias para poder acceder a la tabla como si fuera de new_cd40_offline
# No hay sinónimos en MySQL, así que me ha parecido la mejor manera para no tener que usar otros nombres 
# de esquemas que no sean new_cd40_offline
# (excepto en la instalación, claro
SET @sql = CONCAT('CREATE VIEW HistoricoIncidencias as SELECT * FROM ', @ConfOnline, '.HistoricoIncidencias')
$$
PREPARE STMT FROM @sql
$$
EXECUTE STMT
$$
DEALLOCATE PREPARE STMT
$$


DROP FUNCTION IF EXISTS GetNombreConf
$$
CREATE FUNCTION GetNombreConf(PConfiguracion varchar(100)) RETURNS VARCHAR(100) DETERMINISTIC
# Devuelve el nombre que le corresponde a la configuración
BEGIN
  DECLARE ConfOnline        VARCHAR(100);
  DECLARE NombreConf        VARCHAR(100);
  DECLARE Sufijo            VARCHAR(100);
  DECLARE ExisteConf        TINYINT(1);
  DECLARE ExisteBD          TINYINT(1);
  DECLARE MensajeErr        VARCHAR(1000);

  DECLARE exNotFound INT DEFAULT FALSE;
  DECLARE CONTINUE HANDLER FOR NOT FOUND SET exNotFound = TRUE;

  SELECT NOMBRE INTO ConfOnline FROM CONFIGURACIONES WHERE TIPO = 'ONLINE';
  
  IF exNotFound THEN
    SET MensajeErr = CONCAT('No hay configurada ninguna configuración ONLINE (new_cd40)');
    SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = MensajeErr;
  END IF;

  SET NombreConf = REPLACE(PConfiguracion, '_trans', '');
  
  IF NombreConf NOT REGEXP CONCAT('^', ConfOnline, '.*') THEN
    IF INSTR(NombreConf, ConfOnline) > 0 THEN
      SET Sufijo = SUBSTRING(NombreConf, INSTR(NombreConf, ConfOnline) + LENGTH(ConfOnline) + 1);
      IF LENGTH(Sufijo) = 0 THEN
        SET Sufijo = SUBSTRING(NombreConf, 1, INSTR(NombreConf, ConfOnline)-1);
      END IF;
    ELSE
      SET Sufijo = NombreConf;
    END IF;
  ELSE
    SET Sufijo = SUBSTRING(NombreConf, LENGTH(ConfOnline) + 1);
  END IF;
  
  SET Sufijo = TRIM(BOTH '_' FROM Sufijo);

  SET NombreConf = CONCAT(ConfOnline, '_', Sufijo);
  SET NombreConf = TRIM(BOTH '_' FROM NombreConf );
  RETURN NombreConf;
END;
$$


DROP FUNCTION IF EXISTS GetNombreConfEdicion
$$
CREATE FUNCTION GetNombreConfEdicion(PConfiguracion varchar(100)) RETURNS VARCHAR(100) DETERMINISTIC
# Devuelve el nombre que le corresponde a la configuración de edición
BEGIN
  DECLARE ConfOnline        VARCHAR(100);
  DECLARE NombreConf        VARCHAR(100);
  DECLARE NombreConfEdicion VARCHAR(100);
  DECLARE Sufijo            VARCHAR(100);
  DECLARE ExisteConf        TINYINT(1);
  DECLARE ExisteBD          TINYINT(1);
  DECLARE MensajeErr        VARCHAR(1000);

  DECLARE exNotFound INT DEFAULT FALSE;
  DECLARE CONTINUE HANDLER FOR NOT FOUND SET exNotFound = TRUE;

  SELECT NOMBRE INTO ConfOnline FROM CONFIGURACIONES WHERE TIPO = 'ONLINE';
  
  IF exNotFound THEN
    SET MensajeErr = CONCAT('No hay configurada ninguna configuración ONLINE (new_cd40)');
    SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = MensajeErr;
  END IF;

  SET NombreConf = GetNombreConf(PConfiguracion);
  SET Sufijo = SUBSTRING(NombreConf, LENGTH(ConfOnline) + 1);
  SET Sufijo = TRIM(BOTH '_' FROM Sufijo);

  IF LENGTH(Sufijo) = 0 THEN
    SET NombreConfEdicion = CONCAT(ConfOnline, '_trans');
  ELSE
    SET NombreConfEdicion = CONCAT(ConfOnline, '_trans_', Sufijo);
  END IF;
  RETURN NombreConfEdicion;
END;
$$


DROP PROCEDURE IF EXISTS InsertConfiguracionesBD
$$
CREATE PROCEDURE InsertConfiguracionesBD (PConfiguracion_Activa varchar(100)) MODIFIES SQL DATA
# Comprueba que todos los schemas offline de la BD están cargadas en la BD
BEGIN
  DECLARE SchemaName    VARCHAR(100);
  DECLARE NombreConf    VARCHAR(100);
  DECLARE NombreConf_Ed VARCHAR(100);
  DECLARE TipoConf      VARCHAR(100);
  DECLARE DescConf      VARCHAR(100);
  DECLARE MensajeErr    VARCHAR(1000);

  DECLARE exNotFound INT DEFAULT FALSE;
  DECLARE cSchemas CURSOR FOR 
          SELECT SCHEMA_NAME 
          FROM   INFORMATION_SCHEMA.SCHEMATA
          WHERE  SCHEMA_NAME REGEXP CONCAT('^', PConfiguracion_Activa, '.*')
          AND    SCHEMA_NAME NOT REGEXP CONCAT('^', PConfiguracion_Activa, '_trans.*')
          AND    SCHEMA_NAME <> DATABASE();
  DECLARE cConfiguraciones CURSOR FOR 
          SELECT NOMBRE 
          FROM   CONFIGURACIONES;

  DECLARE CONTINUE HANDLER FOR NOT FOUND SET exNotFound = TRUE;

  # SELECT 'Comprobar que todos los schemas de tipo new_cd40* están incluidos en la tabla CONFIGURACIONES' OPERACION;
  OPEN cSchemas;
  cSchemas_loop:
  LOOP
     FETCH cSchemas INTO SchemaName;
     IF exNotFound THEN LEAVE cSchemas_loop; END IF;
     
     # SELECT CONCAT('   cSchemas: ', SchemaName) OPERACION;
     # Insertamos el schema en la tabla de configuraciones
     SELECT NOMBRE INTO NombreConf FROM CONFIGURACIONES WHERE NOMBRE = SchemaName;
     IF exNotFound THEN
        IF SchemaName = PConfiguracion_Activa THEN
          SET TipoConf = 'ONLINE';
          SET DescConf = 'Configuración en producción';
        ELSE
          SET TipoConf = 'OFFLINE';
          SET DescConf = CONCAT('Descripción ',SchemaName);
        END IF;

        INSERT INTO CONFIGURACIONES ( NOMBRE    , TIPO    , DESCRIPCION) 
                             VALUES ( SchemaName, TipoConf, DescConf);
     END IF;
     SET exNotFound = FALSE;
  END LOOP cSchemas_loop;
  CLOSE cSchemas;
  
  # Buscamos las configuraciones de edición
  # SELECT 'Buscamos las configuraciones de edición' OPERACION;
  SET exNotFound = FALSE;
  OPEN cConfiguraciones;
  cConfiguraciones_loop:
  LOOP
     FETCH cConfiguraciones INTO NombreConf;
     IF exNotFound THEN LEAVE cConfiguraciones_loop; END IF;
     
     SET NombreConf_Ed = GetNombreConfEdicion(NombreConf);
     # SELECT CONCAT('    ', NombreConf, ' -> ', NombreConf_Ed) OPERACION;
     SELECT SCHEMA_NAME INTO NombreConf_Ed FROM INFORMATION_SCHEMA.SCHEMATA WHERE SCHEMA_NAME = NombreConf_Ed;
     IF exNotFound THEN
        UPDATE CONFIGURACIONES SET CONF_EDICION = NombreConf_Ed, DESCRIPCION = CONCAT(DESCRIPCION, ' - ERROR: No existe la BD de edición correspondiente')
         WHERE NOMBRE = NombreConf;
     ELSE
        UPDATE CONFIGURACIONES SET CONF_EDICION = NombreConf_Ed
         WHERE NOMBRE = NombreConf;
     END IF;
     SET exNotFound = FALSE;
  END LOOP cConfiguraciones_loop;
  CLOSE cConfiguraciones;
END;
$$

CALL InsertConfiguracionesBD(@ConfOnline)
$$

DROP PROCEDURE IF EXISTS GetConfiguracionesOfflineBD
$$
CREATE PROCEDURE GetConfiguracionesOfflineBD () READS SQL DATA 
# Devuelve la lista de schemas de conf. offline de la BD
BEGIN
  DECLARE ConfOnline    VARCHAR(100);
  DECLARE MensajeErr    VARCHAR(1000);

  DECLARE exNotFound INT DEFAULT FALSE;
  DECLARE CONTINUE HANDLER FOR NOT FOUND SET exNotFound = TRUE;

  # SELECT 'Nombre de la conf. online' OPERACION;
  SELECT NOMBRE INTO ConfOnline FROM CONFIGURACIONES WHERE TIPO = 'ONLINE';  
  IF exNotFound THEN
    SET MensajeErr = CONCAT('No hay configurada ninguna configuración ONLINE (new_cd40)');
    SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = MensajeErr;
  END IF;
  
	SELECT SCHEMA_NAME 
  FROM   INFORMATION_SCHEMA.SCHEMATA
  WHERE  SCHEMA_NAME REGEXP CONCAT('^', ConfOnline, '.*')
  AND    SCHEMA_NAME <> DATABASE()
  AND    SCHEMA_NAME <> ConfOnline
  AND    SCHEMA_NAME <> CONCAT(ConfOnline, '_trans');
END;
$$

DROP PROCEDURE IF EXISTS CheckConfiguracion
$$
CREATE PROCEDURE CheckConfiguracion (INOUT PNombre varchar(100), INOUT PExiste TINYINT(1), INOUT PExisteBD TINYINT(1), INOUT POnline TINYINT(1)) READS SQL DATA 
# Comprueba que el nombre de la configuración cumple con la norma y si existe en la tabla de configuraciones y su schema en la BD 
BEGIN
  DECLARE ConfOnline  VARCHAR(100);
  DECLARE NombreConf  VARCHAR(100);
  DECLARE MensajeErr  VARCHAR(1000);

  DECLARE exNotFound INT DEFAULT FALSE;
  DECLARE CONTINUE HANDLER FOR NOT FOUND SET exNotFound = TRUE;

  # SELECT CONCAT('CheckConfiguracion: ', PNombre) OPERACION;  
  IF PNombre = DATABASE() THEN
    SET MensajeErr = CONCAT('El schema ', DATABASE(), ' es el schema para mantener las configuraciones offline.');
    SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = MensajeErr;
  END IF;
  
  SELECT NOMBRE INTO ConfOnline FROM CONFIGURACIONES WHERE TIPO = 'ONLINE';  
  IF exNotFound THEN
    SET MensajeErr = CONCAT('No hay configurada ninguna configuración ONLINE (new_cd40)');
    SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = MensajeErr;
  END IF;

  SET NombreConf = REPLACE(PNombre, '_trans', '');
  
  IF NombreConf <> ConfOnline THEN
    IF NombreConf NOT REGEXP CONCAT('^', ConfOnline, '.*') THEN
      # Se asume que se trata de un sufijo
      SET NombreConf = CONCAT(ConfOnline, '_', NombreConf);
    END IF;
  ELSE
    SET POnline = TRUE;
  END IF;
  
  SET PNombre = NombreConf;
  SET exNotFound = FALSE;
  # SELECT CONCAT('Comprobar si existe en la configuración: ', PNombre) OPERACION;  
  SELECT NOMBRE INTO NombreConf FROM CONFIGURACIONES WHERE NOMBRE = PNombre;
  IF exNotFound THEN
    SET PExiste = FALSE;
  ELSE
    SET PExiste = TRUE;
  END IF;
  
  # SELECT CONCAT('Comprobar si existe la configuración en la BD: ', PNombre) OPERACION;  
  SET exNotFound = FALSE;
  SELECT SCHEMA_NAME INTO NombreConf FROM INFORMATION_SCHEMA.SCHEMATA WHERE SCHEMA_NAME = PNombre;
  IF exNotFound THEN
    SET PExisteBD = FALSE;
  ELSE
    SET PExisteBD = TRUE;
  END IF;
END;
$$

DROP PROCEDURE IF EXISTS CheckConfiguracionEdicion
$$
CREATE PROCEDURE CheckConfiguracionEdicion (INOUT PNombre varchar(100), INOUT PExiste TINYINT(1), INOUT PExisteBD TINYINT(1), INOUT POnline TINYINT(1)) READS SQL DATA 
# Comprueba que el nombre de la configuración de edicion cumple con la norma y si existe en la tabla de configuraciones y su schema en la BD 
BEGIN
  DECLARE ConfOnline  VARCHAR(100);
  DECLARE NombreConf  VARCHAR(100);
  DECLARE ExisteConf  TINYINT(1);
  DECLARE ExisteBD    TINYINT(1);
  DECLARE MensajeErr  VARCHAR(1000);

  DECLARE exNotFound INT DEFAULT FALSE;
  DECLARE CONTINUE HANDLER FOR NOT FOUND SET exNotFound = TRUE;

  # SELECT CONCAT('CheckConfiguracionEdicion: ', PNombre) OPERACION;  

  SET NombreConf = REPLACE(PNombre, '_trans', '');
  CALL CheckConfiguracion(NombreConf, ExisteConf, ExisteBD, POnline);
  
  SELECT NOMBRE INTO ConfOnline FROM CONFIGURACIONES WHERE TIPO = 'ONLINE';
  IF exNotFound THEN
    SET MensajeErr = CONCAT('No hay configurada ninguna configuración ONLINE (new_cd40)');
    SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = MensajeErr;
  END IF;

  SET PNombre = GetNombreConfEdicion(NombreConf);
  
  # SELECT CONCAT('Comprobar si existe en la configuración: ', PNombre) OPERACION;  
  SET exNotFound = FALSE;
  SELECT CONF_EDICION INTO NombreConf FROM CONFIGURACIONES WHERE CONF_EDICION = PNombre;
  IF exNotFound THEN
    SET PExiste = FALSE;
  ELSE
    SET PExiste = TRUE;
  END IF;
  
  # SELECT CONCAT('Comprobar si existe la configuración en la BD: ', PNombre) OPERACION;  
  SET exNotFound = FALSE;
  SELECT SCHEMA_NAME INTO NombreConf FROM INFORMATION_SCHEMA.SCHEMATA WHERE SCHEMA_NAME = PNombre;
  IF exNotFound THEN
    # SELECT '     NO EXISTE' OPERACION;  
    SET PExisteBD = FALSE;
  ELSE
    # SELECT '     EXISTE' OPERACION;  
    SET PExisteBD = TRUE;
  END IF;
END;
$$

DROP PROCEDURE IF EXISTS AddConfiguracion
$$
CREATE PROCEDURE AddConfiguracion (PNombre varchar(100), PDescripcion varchar(100), PBorrarSiExiste TINYINT(1)) MODIFIES SQL DATA
# Añade la configuración (ppal y de edición) a la lista de configuraciones y crea las BDs
BEGIN
  DECLARE NombreConf     VARCHAR(100);
  DECLARE NombreConfEd   VARCHAR(100);
  DECLARE ExisteConf     TINYINT(1);
  DECLARE ExisteBD       TINYINT(1);
  DECLARE EsOnline       TINYINT(1);
  DECLARE MensajeErr     VARCHAR(1000);

  SET NombreConf = PNombre;
  CALL CheckConfiguracion(NombreConf, ExisteConf, ExisteBD, EsOnline);
  SET NombreConfEd = GetNombreConfEdicion(NombreConf);
  
  IF EsOnline THEN
    SET MensajeErr = CONCAT('La configuración ya existe y es ONLINE: ', PNombre);
    SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = MensajeErr;
  END IF;

	-- Si existe y no hay que borrar lo dejamos que continue
  -- IF ExisteConf AND not PBorrarSiExiste THEN
  --   SET MensajeErr = CONCAT('La configuración ya existe: ', PNombre);
  --   SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = MensajeErr;
  -- END IF;
  
  IF ExisteConf AND PBorrarSiExiste THEN
    DELETE FROM CONFIGURACIONES WHERE NOMBRE = NombreConf;
  END IF;
  
  IF NOT ExisteConf OR PBorrarSiExiste THEN
    INSERT INTO CONFIGURACIONES ( NOMBRE    , CONF_EDICION, TIPO     , DESCRIPCION) 
                         VALUES ( NombreConf, NombreConfEd, 'OFFLINE', PDescripcion);
  END IF;
  
  IF ExisteBD AND PBorrarSiExiste THEN
    SET @sql = CONCAT('DROP DATABASE ', NombreConf);
    PREPARE STMT FROM @sql;
    EXECUTE STMT;
    DEALLOCATE PREPARE STMT;
  END IF;
  
  IF NOT ExisteBD OR PBorrarSiExiste THEN
    SET @sql = CONCAT('CREATE DATABASE ', NombreConf);
    PREPARE STMT FROM @sql;
    EXECUTE STMT;
    DEALLOCATE PREPARE STMT;
  END IF;
  
  -- CONFIGURACION EDICION
  CALL CheckConfiguracionEdicion(NombreConfEd, ExisteConf, ExisteBD, EsOnline);
  
  IF ExisteBD AND PBorrarSiExiste THEN
    SET @sql = CONCAT('DROP DATABASE ', NombreConfEd);
    # SELECT @SQL OPERACION;
    PREPARE STMT FROM @sql;
    EXECUTE STMT;
    DEALLOCATE PREPARE STMT;
  END IF;
  
  IF NOT ExisteBD OR PBorrarSiExiste THEN
    SET @sql = CONCAT('CREATE DATABASE ', NombreConfEd);
    # SELECT @SQL OPERACION;
    PREPARE STMT FROM @sql;
    EXECUTE STMT;
    DEALLOCATE PREPARE STMT;
  END IF;
END;
$$

DROP PROCEDURE IF EXISTS DelConfiguracion
$$
CREATE PROCEDURE DelConfiguracion (PNombre varchar(100)) MODIFIES SQL DATA
# Añade la configuración a la lista de configuraciones y crea la BD
BEGIN
  DECLARE NombreConf       VARCHAR(100);
  DECLARE NombreConfEd     VARCHAR(100);
  DECLARE ExisteConf       TINYINT(1);
  DECLARE ExisteBD         TINYINT(1);
  DECLARE EsOnline         TINYINT(1);
  DECLARE MensajeErr       VARCHAR(1000);

  SET NombreConf = PNombre;
  CALL CheckConfiguracion(NombreConf, ExisteConf, ExisteBD, EsOnline);
  SET NombreConfEd = GetNombreConfEdicion(NombreConf);
  
  IF EsOnline THEN
    SET MensajeErr = CONCAT('La configuración es ONLINE y no se puede borrar desde este procedimiento: ', PNombre);
    SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = MensajeErr;
  END IF;

  IF ExisteConf THEN
    DELETE FROM CONFIGURACIONES WHERE NOMBRE = NombreConf;
  END IF;
  
  IF ExisteBD THEN
    SET @sql = CONCAT('DROP DATABASE ', NombreConf);
    PREPARE STMT FROM @sql;
    EXECUTE STMT;
    DEALLOCATE PREPARE STMT;
  END IF;
    
  -- CONFIGURACION EDICION
  CALL CheckConfiguracionEdicion(NombreConfEd, ExisteConf, ExisteBD, EsOnline);
  
  IF ExisteBD THEN
    SET @sql = CONCAT('DROP DATABASE ', NombreConfEd);
    PREPARE STMT FROM @sql;
    EXECUTE STMT;
    DEALLOCATE PREPARE STMT;
  END IF;
END;
$$

DROP PROCEDURE IF EXISTS DelHistoricos
$$
CREATE PROCEDURE DelHistoricos (PNombre varchar(100)) MODIFIES SQL DATA
# Vacía los historicos (historicoincidencias) de la conf que se le pasa como parámetro
BEGIN
  DECLARE NombreConf       VARCHAR(100);
  DECLARE NombreConfEd     VARCHAR(100);
  DECLARE ExisteConf       TINYINT(1);
  DECLARE ExisteBD         TINYINT(1);
  DECLARE EsOnline         TINYINT(1);
  DECLARE MensajeErr       VARCHAR(1000);

  DECLARE exNotFound INT DEFAULT FALSE;
  DECLARE CONTINUE HANDLER FOR NOT FOUND SET exNotFound = TRUE;

  SET NombreConf = PNombre;
  CALL CheckConfiguracion(NombreConf, ExisteConf, ExisteBD, EsOnline);
  
  IF EsOnline THEN
    SET MensajeErr = CONCAT('La configuración es ONLINE y no se puede borrar el histórico de incidencias: ', NombreConf);
    SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = MensajeErr;
  END IF;

  IF NOT ExisteConf THEN
    SET MensajeErr = CONCAT('La configuracion ', NombreConf, ' no existe');
    SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = MensajeErr;
  END IF;

  IF NOT ExisteBD THEN
    SET MensajeErr = CONCAT('El esquema de datos de La configuración ', NombreConf, ' no existe');
    SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = MensajeErr;
  ELSE
    SET @sql = CONCAT('TRUNCATE TABLE ', NombreConf, '.historicoincidencias');
    PREPARE STMT FROM @sql;
    EXECUTE STMT;
    DEALLOCATE PREPARE STMT;
  END IF;

  SET NombreConfEd = GetNombreConfEdicion(NombreConf);
  CALL CheckConfiguracionEdicion(NombreConfEd, ExisteConf, ExisteBD, EsOnline);

	SET @sql = CONCAT('TRUNCATE TABLE ', NombreConfEd, '.historicoincidencias');
	PREPARE STMT FROM @sql;
	EXECUTE STMT;
	DEALLOCATE PREPARE STMT;

END;
$$

DROP PROCEDURE IF EXISTS ActivarConfiguracion
$$
CREATE PROCEDURE ActivarConfiguracion (PConfiguracion varchar(100)) MODIFIES SQL DATA
# Para la conf. PConfiguracion (que debe de ser de edición) a new_cd40_trans
# (todo menos los operadores)
BEGIN
  DECLARE ConfOnlineED        VARCHAR(100);
  DECLARE ConfOfflineED       VARCHAR(100);
  DECLARE ExisteConf          TINYINT(1);
  DECLARE ExisteBD            TINYINT(1);
  DECLARE EsOnline            TINYINT(1);
  DECLARE Tabla               VARCHAR(100);
  DECLARE Columnas            VARCHAR(1000);
  DECLARE MensajeErr          VARCHAR(1000);

  DECLARE exNotFound INT DEFAULT FALSE;
  
  DECLARE cTablas CURSOR FOR 
          SELECT TABLE_NAME
          FROM   INFORMATION_SCHEMA.TABLES
          WHERE  TABLE_SCHEMA = ConfOnlineED 
          AND    TABLE_TYPE = 'BASE TABLE'
          AND    TABLE_NAME NOT IN ( 'OPERADORES'
                                   , 'HISTORICOINCIDENCIAS');
          
  DECLARE CONTINUE HANDLER FOR NOT FOUND SET exNotFound = TRUE;

  # Comprobamos la configuración de edición ONLINE
	SELECT CONF_EDICION INTO ConfOnlineED FROM CONFIGURACIONES WHERE TIPO = 'ONLINE';
  SET ExisteConf = FALSE;
  SET ExisteBD   = FALSE;
  SET EsOnline   = FALSE;
  CALL CheckConfiguracionEdicion (ConfOnlineED, ExisteConf, ExisteBD, EsOnline);
  
  IF NOT ExisteBD THEN
    SET MensajeErr = CONCAT('El schema/base de datos ONLINE de edicion (', ConfOfflineED, ') no existe');
    SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = MensajeErr;
  END IF;

  # Comprobamos la configuración que se quiere activar
  SET ConfOfflineED  = PConfiguracion;
  SET ExisteConf = FALSE;
  SET ExisteBD   = FALSE;
  SET EsOnline   = FALSE;
  CALL CheckConfiguracionEdicion (ConfOfflineED, ExisteConf, ExisteBD, EsOnline);
  
  IF NOT ExisteConf THEN
    SET MensajeErr = CONCAT('La configuración ', ConfOfflineED, ' no existe');
    SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = MensajeErr;
	END IF;
  IF NOT ExisteBD THEN
    SET MensajeErr = CONCAT('El schema/base de datos ', ConfOfflineED, ' no existe');
    SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = MensajeErr;
  END IF;
  IF EsOnline THEN
    SET MensajeErr = CONCAT('El schema/base de datos ', ConfOfflineED, ' es la de producción');
    SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = MensajeErr;
  END IF;
  
  SET FOREIGN_KEY_CHECKS=0;
  
  OPEN cTablas;
  cTablas_loop:
  LOOP
    FETCH cTablas INTO Tabla;
    IF exNotFound THEN LEAVE cTablas_loop; END IF;

    # Borrar el contenido de la tabla
    SET @sql = CONCAT('DELETE FROM ', ConfOnlineED, '.', Tabla);
    SELECT @sql OPERACION;
    PREPARE STMT FROM @sql;
    EXECUTE STMT;
    DEALLOCATE PREPARE STMT;
    
    SELECT GROUP_CONCAT(COLUMN_NAME) INTO Columnas 
    FROM   INFORMATION_SCHEMA.COLUMNS
    WHERE  TABLE_SCHEMA = ConfOnlineED
    AND    TABLE_NAME = Tabla;

    # Insertar los datos de la configuración a activar
    SET @sql = CONCAT('INSERT INTO ', ConfOnlineED, '.', Tabla, '(', Columnas, ') SELECT ', Columnas, ' FROM ', ConfOfflineED, '.', Tabla);
    SELECT @sql OPERACION;
    PREPARE STMT FROM @sql;
    EXECUTE STMT;
    DEALLOCATE PREPARE STMT;
  END LOOP cTablas_loop;
  CLOSE cTablas;  

  SET FOREIGN_KEY_CHECKS=1;
  
  -- Fallan las FK ... CALL CheckForeignKeys(ConfOnlineED);
  
  # Insertamos las tablas modificadas
  CALL InsertarTablasModificadas(ConfOnlineED);
END;
$$

DROP PROCEDURE IF EXISTS InsertarTablasModificadas
$$
CREATE PROCEDURE InsertarTablasModificadas (PConfiguracionED varchar(100)) MODIFIES SQL DATA
# Rellena la lista de TablasModificadas 
BEGIN
  DECLARE ConfiguracionED     VARCHAR(100);
  DECLARE ExisteConf          TINYINT(1);
  DECLARE ExisteBD            TINYINT(1);
  DECLARE EsOnline            TINYINT(1);

  DECLARE MensajeErr          VARCHAR(1000);
  DECLARE exNotFound INT DEFAULT FALSE;
  
  DECLARE CONTINUE HANDLER FOR NOT FOUND SET exNotFound = TRUE;

  # Comprobamos que se trata de una configuración de edición
  SET ConfiguracionED = PConfiguracionED;
  SET ExisteConf = FALSE;
  SET ExisteBD   = FALSE;
  SET EsOnline   = FALSE;
  CALL CheckConfiguracionEdicion (ConfiguracionED, ExisteConf, ExisteBD, EsOnline);
  
  IF NOT ExisteBD THEN
    SET MensajeErr = CONCAT('El schema/base de datos de la configuración ', ConfiguracionED, ' no existe');
    SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = MensajeErr;
  END IF;

  SET @sql = CONCAT('REPLACE INTO ', ConfiguracionED, '.tablasmodificadas VALUES '
                                  , '(\'Agenda\')'
                                  ,',(\'Agrupaciones\')'
                                  ,',(\'Destinos\')'
                                  ,',(\'DestinosExternos\')'
                                  ,',(\'DestinosExternosSector\')'
                                  ,',(\'DestinosInternos\')'
                                  ,',(\'DestinosInternosSector\')'
                                  ,',(\'DestinosRadio\')'
                                  ,',(\'DestinosRadioSector\')'
                                  ,',(\'DestinosTelefonia\')'
                                  ,',(\'Emplazamientos\')'
                                  ,',(\'Encaminamientos\')'
                                  ,',(\'EquiposEU\')'
                                  ,',(\'EstadoRecursos\')'
                                  ,',(\'EstadosRecursos\')'
                                  ,',(\'externos\')'
                                  ,',(\'GruposTelefonia\')'
                                  ,',(\'GwActivas\')'
                                  ,',(\'internos\')'
                                  ,',(\'metodosbss_destinosradio\')'
                                  ,',(\'Niveles\')'
                                  ,',(\'Nucleos\')'
                                  ,',(\'Operadores\')'
                                  ,',(\'ParametrosRecurso\')'
                                  ,',(\'ParametrosSector\')'
                                  ,',(\'permisosopedepats\')'
                                  ,',(\'PermisosRedes\')'
                                  ,',(\'radio\')'
                                  ,',(\'radio_param\')'
                                  ,',(\'Rangos\')'
                                  ,',(\'Recursos\')'
                                  ,',(\'RecursosRadio\')'
                                  ,',(\'RecursosTF\')'
                                  ,',(\'Redes\')'
                                  ,',(\'Rutas\')'
                                  ,',(\'Sectores\')'
                                  ,',(\'SectoresAgrupacion\')'
                                  ,',(\'SectoresSector\')'
                                  ,',(\'SectoresSectorizacion\')'
                                  ,',(\'tabla_bss\')'
                                  ,',(\'TeclasSector\')'
                                  ,',(\'Tifx\')'
                                  ,',(\'Top\')'
                                  ,',(\'Troncales\')'
                                  ,',(\'UsuariosAbonados\')'
                                  ,',(\'Valores_Tabla\')'
                                  ,',(\'Zonas\')');
  PREPARE STMT FROM @sql;
  EXECUTE STMT;
  DEALLOCATE PREPARE STMT;
END;
$$

DROP PROCEDURE IF EXISTS VaciaConfiguracion
$$
CREATE PROCEDURE VaciaConfiguracion (PNombre varchar(100)) MODIFIES SQL DATA
# Vacía de datos la configuración que se le pasa como parámetro. Comprobará que no se trata de la configuración de producción (new_cd40 ONLINE)
BEGIN
  DECLARE NombreConf       VARCHAR(100);
  DECLARE NombreConfEd     VARCHAR(100);
  DECLARE ConfPPAL         TINYINT(1);
  DECLARE ExisteConf       TINYINT(1);
  DECLARE ExisteBD         TINYINT(1);
  DECLARE EsOnline         TINYINT(1);
  DECLARE TableName        VARCHAR(100);
  DECLARE MensajeErr       VARCHAR(1000);

  DECLARE exNotFound INT DEFAULT FALSE;
  DECLARE cTablas CURSOR FOR 
          SELECT TABLE_NAME
          FROM   INFORMATION_SCHEMA.TABLES
          WHERE  TABLE_SCHEMA = PNombre 
          AND    TABLE_TYPE = 'base table';
  DECLARE CONTINUE HANDLER FOR NOT FOUND SET exNotFound = TRUE;

  SET NombreConf = PNombre;
  CALL CheckConfiguracion(NombreConf, ExisteConf, ExisteBD, EsOnline);
  IF PNombre = NombreConf THEN
    SET ConfPPAL = TRUE;
  ELSE
    # Comprobar si se trata de la configuración de edición
    SET NombreConf = PNombre;
    CALL CheckConfiguracionEdicion(NombreConf, ExisteConf, ExisteBD, EsOnline);
    IF PNombre = NombreConf THEN
      SET ConfPPAL = FALSE;
    ELSE
      SET MensajeErr = CONCAT('Ha sido imposible determinar si se trata de una configuración ppal o una de edición', PNombre);
      SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = MensajeErr;
    END IF;
  END IF;
  
  IF EsOnline & ConfPPAL THEN
    SET MensajeErr = CONCAT('La configuración es la ONLINE principal (la configuración de producción) y no se puede borrar su contenido: ', NombreConf);
    SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = MensajeErr;
  END IF;

  IF NOT ExisteConf THEN
    SET MensajeErr = CONCAT('La configuracion ', NombreConf, ' no existe en CONFIGURACIONES');
    SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = MensajeErr;
  END IF;

  IF NOT ExisteBD THEN
    SET MensajeErr = CONCAT('El esquema de datos de La configuración ', NombreConf, ' no existe');
    SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = MensajeErr;
  END IF;

  -- Llegados a este punto está comprobado que la configuración existe, junto con su esquema de datos
  -- Recorremos las tablas para vaciarlas
  OPEN cTablas;
  cTablas_loop:
  LOOP
    FETCH cTablas INTO TableName;
    IF exNotFound THEN LEAVE cTablas_loop; END IF;
     
    # SELECT CONCAT('   cTablas: ', TableName) OPERACION;
    # Borrar el contenido de la tabla
    SET @sql = CONCAT('TRUNCATE TABLE ', NombreConf, '.', TableName);
    # SELECT @sql OPERACION;
    PREPARE STMT FROM @sql;
    EXECUTE STMT;
    DEALLOCATE PREPARE STMT;
  END LOOP cTablas_loop;
  CLOSE cTablas;  
END;
$$

DROP FUNCTION IF EXISTS DiffDescripcionTabla
$$

CREATE FUNCTION DiffDescripcionTabla(PConfiguracion VARCHAR(64), PTabla VARCHAR(64)) RETURNS TEXT READS SQL DATA 
# Comprueba que la descripción de la tabla de la Configuración es la misma que la de producción
BEGIN
    DECLARE VConfOnLine         VARCHAR(64);
    DECLARE VConf               VARCHAR(64);
    DECLARE VResultado          TEXT;
	
    DECLARE I                   INT;

    DECLARE VTabla              VARCHAR(1000);
    DECLARE VColumna            VARCHAR(1000); 
    DECLARE VEs_Null            VARCHAR(1000);
    DECLARE VTipo_Columna       VARCHAR(1000);

    DECLARE MensajeErr          VARCHAR(1000);
    DECLARE exNotFound INT DEFAULT FALSE;

    DECLARE cColumnasConfOnline CURSOR FOR
            SELECT A.TABLE_NAME
                 , A.COLUMN_NAME
                 , A.IS_NULLABLE
                 , A.COLUMN_TYPE
             FROM (SELECT * FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_SCHEMA = VConfOnline And TABLE_NAME = PTabla) A
             LEFT JOIN (SELECT * FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_SCHEMA = VConf And TABLE_NAME = PTabla) B 
                    ON IFNULL(A.TABLE_NAME , 'ES NULL') = IFNULL(B.TABLE_NAME, 'ES NULL')
                   AND IFNULL(A.COLUMN_NAME, 'ES NULL') = IFNULL(B.COLUMN_NAME, 'ES NULL')
                   AND IFNULL(A.IS_NULLABLE, 'ES NULL') = IFNULL(B.IS_NULLABLE, 'ES NULL') 
                   AND IFNULL(A.COLUMN_TYPE, 'ES NULL') = IFNULL(B.COLUMN_TYPE, 'ES NULL')
            WHERE B.TABLE_NAME IS NULL;

    DECLARE cColumnasConfCheck CURSOR FOR
            SELECT A.TABLE_NAME
                 , A.COLUMN_NAME
                 , A.IS_NULLABLE
                 , A.COLUMN_TYPE
             FROM (SELECT * FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_SCHEMA = VConf And TABLE_NAME = PTabla) A
             LEFT JOIN (SELECT * FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_SCHEMA = VConfOnline And TABLE_NAME = PTabla) B 
                    ON IFNULL(A.TABLE_NAME , 'ES NULL') = IFNULL(B.TABLE_NAME, 'ES NULL')
                   AND IFNULL(A.COLUMN_NAME, 'ES NULL') = IFNULL(B.COLUMN_NAME, 'ES NULL')
                   AND IFNULL(A.IS_NULLABLE, 'ES NULL') = IFNULL(B.IS_NULLABLE, 'ES NULL') 
                   AND IFNULL(A.COLUMN_TYPE, 'ES NULL') = IFNULL(B.COLUMN_TYPE, 'ES NULL')
            WHERE B.TABLE_NAME IS NULL;

    DECLARE CONTINUE HANDLER FOR NOT FOUND SET exNotFound = TRUE;

    SELECT NOMBRE INTO VConfOnline FROM CONFIGURACIONES WHERE TIPO = 'ONLINE';
    SELECT SCHEMA_NAME INTO VConfOnline FROM INFORMATION_SCHEMA.SCHEMATA
     WHERE SCHEMA_NAME = VConfOnline;
     
    IF exNotFound THEN
      SET MensajeErr = CONCAT('La configuración online ', IFNULL(VConfOnline, ''), ' no existe');
      SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = MensajeErr;
    END IF;

    SET exNotFound = FALSE;
    SELECT SCHEMA_NAME INTO VConf FROM INFORMATION_SCHEMA.SCHEMATA
     WHERE SCHEMA_NAME = PConfiguracion;
     
    IF exNotFound THEN
      SET MensajeErr = CONCAT('La configuración a comprobar ', IFNULL(PConfiguracion, ''), ' no existe');
      SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = MensajeErr;
    END IF;

    SET VResultado = '';

    SET exNotFound = FALSE;
    OPEN cColumnasConfOnline;
    cColumnasConfOnline_Loop: 
    LOOP               
      FETCH cColumnasConfOnline INTO VTabla
                                   , VColumna
                                   , VEs_Null
                                   , VTipo_Columna;
      IF exNotFound THEN LEAVE cColumnasConfOnline_Loop; END IF;

      IF LENGTH(VResultado) = 0 THEN
        SET VResultado = CONCAT('Columnas SOLO en ', VConfOnline); 
      END IF;
      SET VResultado = CONCAT(VResultado, '\r   ', VTabla, '.', VColumna, ' ', VTipo_Columna, ' ... IS NULLABLE? ', VEs_Null); 
    END LOOP cColumnasConfOnline_Loop;
    CLOSE cColumnasConfOnline;

    SET exNotFound = FALSE;
    SET I = 0;
    OPEN cColumnasConfCheck;
    cColumnasConfCheck_Loop: 
    LOOP               
      FETCH cColumnasConfCheck INTO VTabla
                                  , VColumna
                                  , VEs_Null
                                  , VTipo_Columna;
      IF exNotFound THEN LEAVE cColumnasConfCheck_Loop; END IF;
      
      SET I = I + 1;
      IF LENGTH(VResultado) = 0 THEN
        SET VResultado = CONCAT('Columnas SOLO en ', VConf); 
      END IF;
      IF I = 1 THEN
				SET VResultado = CONCAT(VResultado, '\rColumnas SOLO en ', VConf); 
      END IF;
      SET VResultado = CONCAT(VResultado, '\r   ', VTabla, '.', VColumna, ' ', VTipo_Columna, ' ... IS NULLABLE? ', VEs_Null); 
    END LOOP cColumnasConfCheck_Loop;
    CLOSE cColumnasConfCheck;

    RETURN VResultado;
END$$

DROP PROCEDURE IF EXISTS CopyTabla
$$
CREATE PROCEDURE CopyTabla (PConfiguracion varchar(100), PTabla varchar(100)) MODIFIES SQL DATA
# Copia los datos de la tabla de la configuración online en la tabla de la configuración que se le pasa como parámetro
BEGIN
  DECLARE ConfOnline       VARCHAR(100);
  DECLARE NombreConf       VARCHAR(100);
  DECLARE NombreConfEd     VARCHAR(100);  
  DECLARE ConfPPAL         TINYINT(1);
  DECLARE ExisteConf       TINYINT(1);
  DECLARE ExisteBD         TINYINT(1);
  DECLARE EsOnline         TINYINT(1);
  DECLARE DiffDesc         TEXT;
  DECLARE Columnas         VARCHAR(1000);

  DECLARE MensajeErr       VARCHAR(1000);
  DECLARE exNotFound INT DEFAULT FALSE;
  DECLARE CONTINUE HANDLER FOR NOT FOUND SET exNotFound = TRUE;

  SET NombreConf = PConfiguracion;
  CALL CheckConfiguracion(NombreConf, ExisteConf, ExisteBD, EsOnline);
  IF PConfiguracion = NombreConf THEN
    SET ConfPPAL = TRUE;
  ELSE
    # Comprobar si se trata de la configuración de edición
		SET NombreConf = PConfiguracion;
    CALL CheckConfiguracionEdicion(NombreConf, ExisteConf, ExisteBD, EsOnline);
    IF PConfiguracion = NombreConf THEN
      SET ConfPPAL = FALSE;
    ELSE
      SET MensajeErr = CONCAT('Ha sido imposible determinar si se trata de una configuración ppal o una de edición', PNombre);
      SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = MensajeErr;
    END IF;
  END IF;
  
  IF EsOnline & ConfPPAL THEN
    SET MensajeErr = CONCAT('La configuración es la ONLINE principal (la configuración de producción) y no se puede modificar su contenido desde aquí: ', NombreConf);
    SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = MensajeErr;
  END IF;

  IF NOT ExisteConf THEN
    SET MensajeErr = CONCAT('La configuracion ', NombreConf, ' no existe en CONFIGURACIONES');
    SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = MensajeErr;
  END IF;

  IF NOT ExisteBD THEN
    SET MensajeErr = CONCAT('El esquema de datos de La configuración ', NombreConf, ' no existe');
    SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = MensajeErr;
  END IF;

  SELECT NOMBRE INTO ConfOnline FROM CONFIGURACIONES WHERE TIPO = 'ONLINE';  
  IF exNotFound THEN
    SET MensajeErr = CONCAT('No hay configurada ninguna configuración ONLINE (new_cd40)');
    SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = MensajeErr;
  END IF;

  -- Llegados a este punto está comprobado que la configuración existe, junto con su esquema de datos
  -- Comprobamos la descripción de la tabla
	SELECT DiffDescripcionTabla(PConfiguracion, PTabla) INTO DiffDesc;
	IF LENGTH(DiffDesc) > 0 THEN
    SET MensajeErr = CONCAT('La tabla ', PConfiguracion, PTabla, ' es diferente a la de produccion');
    SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = MensajeErr;
  END IF;

  SET FOREIGN_KEY_CHECKS=0;

  -- Borramos e insertamos la información de la tabla con los datos de la configuración ONLINE
	SET @sql = CONCAT('DELETE FROM ', NombreConf, '.', PTabla);
	# SELECT CONCAT('   ', @sql) OPERACION;
	PREPARE STMT FROM @sql;
	EXECUTE STMT;
	DEALLOCATE PREPARE STMT;
	
	SELECT GROUP_CONCAT(COLUMN_NAME) INTO Columnas 
	FROM   INFORMATION_SCHEMA.COLUMNS
	WHERE  TABLE_SCHEMA = ConfOnline
	AND    TABLE_NAME = PTabla;

	# Borrar el contenido de la tabla
	SET @sql = CONCAT('INSERT INTO ', NombreConf, '.', PTabla, '(', Columnas, ') SELECT ', Columnas, ' FROM ', ConfOnline, '.', PTabla);
	# SELECT CONCAT('   ', @sql) OPERACION;
	PREPARE STMT FROM @sql;
	EXECUTE STMT;
	DEALLOCATE PREPARE STMT;

  SET FOREIGN_KEY_CHECKS=1;
  
  CALL CheckForeignKeys(PConfiguracion);
END;
$$

DROP PROCEDURE IF EXISTS CopyDatosIniciales
$$
CREATE PROCEDURE CopyDatosIniciales (PConfDestino varchar(100), PBorrarDatos TINYINT(1)) MODIFIES SQL DATA
# Copia los datos iniciales de la configuración ONLINE (la de producción). Si así se indica con el parámetor PBorrarDatos, antes vaciará la tabla correspondiente
BEGIN
  DECLARE ConfOnline       VARCHAR(100);
  DECLARE NombreConf       VARCHAR(100);
  DECLARE NombreConfEd     VARCHAR(100);
  DECLARE ConfPPAL         TINYINT(1);
  DECLARE ExisteConf       TINYINT(1);
  DECLARE ExisteBD         TINYINT(1);
  DECLARE EsOnline         TINYINT(1);
  DECLARE Tabla            VARCHAR(100);
  DECLARE Columnas         VARCHAR(1000);
  DECLARE MensajeErr       VARCHAR(1000);

  DECLARE exNotFound INT DEFAULT FALSE;
  DECLARE cTablas CURSOR FOR 
          SELECT TABLE_NAME
          FROM   INFORMATION_SCHEMA.TABLES
          WHERE  TABLE_SCHEMA = PConfDestino 
          AND    TABLE_TYPE = 'BASE TABLE'
          AND    TABLE_NAME IN ( 'DEPENDENCIASATS'
                               , 'METODOS_BSS'
                               , 'PREFIJOS'
                               , 'REDES'
                               , 'SISTEMA'
                               , 'OPERADORES');
  DECLARE CONTINUE HANDLER FOR NOT FOUND SET exNotFound = TRUE;

  SET NombreConf = PConfDestino;
  CALL CheckConfiguracion(NombreConf, ExisteConf, ExisteBD, EsOnline);
  IF PConfDestino = NombreConf THEN
    SET ConfPPAL = TRUE;
  ELSE
    # Comprobar si se trata de la configuración de edición
     SET NombreConf = PConfDestino;
    CALL CheckConfiguracionEdicion(NombreConf, ExisteConf, ExisteBD, EsOnline);
    IF PConfDestino = NombreConf THEN
      SET ConfPPAL = FALSE;
    ELSE
      SET MensajeErr = CONCAT('Ha sido imposible determinar si se trata de una configuración ppal o una de edición', PNombre);
      SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = MensajeErr;
    END IF;
  END IF;
  
  IF EsOnline & ConfPPAL THEN
    SET MensajeErr = CONCAT('La configuración es la ONLINE principal (la configuración de producción) y modificar su contenido desde aquí: ', NombreConf);
    SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = MensajeErr;
  END IF;

  IF NOT ExisteConf THEN
    SET MensajeErr = CONCAT('La configuracion ', NombreConf, ' no existe en CONFIGURACIONES');
    SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = MensajeErr;
  END IF;

  IF NOT ExisteBD THEN
    SET MensajeErr = CONCAT('El esquema de datos de La configuración ', NombreConf, ' no existe');
    SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = MensajeErr;
  END IF;

  SELECT NOMBRE INTO ConfOnline FROM CONFIGURACIONES WHERE TIPO = 'ONLINE';  
  IF exNotFound THEN
    SET MensajeErr = CONCAT('No hay configurada ninguna configuración ONLINE (new_cd40)');
    SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = MensajeErr;
  END IF;

  -- Llegados a este punto está comprobado que la configuración existe, junto con su esquema de datos
  -- Insertamos la información de las tablas con los Datos Iniciales
  SET FOREIGN_KEY_CHECKS=0;
  
  OPEN cTablas;
  cTablas_loop:
  LOOP
    FETCH cTablas INTO Tabla;
    IF exNotFound THEN LEAVE cTablas_loop; END IF;
    SELECT CONCAT('CopyDatosIniciales en ', PConfDestino, ' ... ', Tabla);
    IF PBorrarDatos THEN
      SET @sql = CONCAT('DELETE FROM ', NombreConf, '.', Tabla);
      # SELECT CONCAT('   ', @sql) OPERACION;
      PREPARE STMT FROM @sql;
      EXECUTE STMT;
      DEALLOCATE PREPARE STMT;
    END IF;
    
    SELECT GROUP_CONCAT(COLUMN_NAME) INTO Columnas 
    FROM   INFORMATION_SCHEMA.COLUMNS
    WHERE  TABLE_SCHEMA = ConfOnline
    AND    TABLE_NAME = Tabla;

    # Borrar el contenido de la tabla
    SET @sql = CONCAT('INSERT INTO ', NombreConf, '.', Tabla, '(', Columnas, ') SELECT ', Columnas, ' FROM ', ConfOnline, '.', Tabla);
    # SELECT CONCAT('   ', @sql) OPERACION;
    PREPARE STMT FROM @sql;
    EXECUTE STMT;
    DEALLOCATE PREPARE STMT;
  END LOOP cTablas_loop;
  CLOSE cTablas;  

  SET FOREIGN_KEY_CHECKS=1;
  
  # SELECT CONCAT('Comprobar Foreign Keys ... CALL CheckForeignKeys(',PConfDestino,')') OPERACION;
  
  CALL CheckForeignKeys(PConfDestino);
END;
$$

DROP PROCEDURE IF EXISTS CheckForeignKeys
$$

CREATE PROCEDURE CheckForeignKeys(PConfiguracion VARCHAR(64)) READS SQL DATA
    LANGUAGE SQL
    NOT DETERMINISTIC
    READS SQL DATA
BEGIN
    DECLARE VEsquema            VARCHAR(64);
    DECLARE VTabla              VARCHAR(64);
    DECLARE VColumna            VARCHAR(64); 
    DECLARE VNombreConstraint   VARCHAR(64);
    DECLARE VEsquema_Ref        VARCHAR(64);
    DECLARE VTabla_Ref          VARCHAR(64);
    DECLARE VColumna_Ref        VARCHAR(64);
  
    DECLARE exNotFound INT DEFAULT FALSE;

    DECLARE cForeignKeys CURSOR FOR
            SELECT TABLE_SCHEMA
                 , TABLE_NAME
                 , COLUMN_NAME
                 , CONSTRAINT_NAME
                 , REFERENCED_TABLE_SCHEMA
                 , REFERENCED_TABLE_NAME
                 , REFERENCED_COLUMN_NAME
              FROM INFORMATION_SCHEMA.KEY_COLUMN_USAGE 
             WHERE CONSTRAINT_SCHEMA LIKE PConfiguracion 
               AND REFERENCED_TABLE_SCHEMA IS NOT NULL;

    DECLARE CONTINUE HANDLER FOR NOT FOUND SET exNotFound = TRUE;

    SET @Resultado = '';

    SET @prev_NombreConstraint = '';
    SET @prev_Esquema          = '';
    SET @prev_Tabla            = '';
    SET @prev_Esquema_Ref      = '';
    SET @prev_Tabla_Ref        = '';

    SET @from_part      = '';
    SET @where_part     = '';
    SET @where_nullable = '';

    SET @Columnas = '';
    SET @Columns_Ref = '';

    OPEN cForeignKeys;
    cForeignKeys_Loop: 
  LOOP               
        FETCH cForeignKeys INTO VEsquema
                              , VTabla
                              , VColumna
                              , VNombreConstraint
                              , VEsquema_Ref
                              , VTabla_Ref
                              , VColumna_Ref;
        IF exNotFound THEN LEAVE cForeignKeys_Loop; END IF;

        IF (@prev_NombreConstraint <> VNombreConstraint AND @from_part <> '' AND @where_part <> '') THEN
            SET @full_query    = CONCAT('SELECT COUNT(*) ', @from_part, ' WHERE (', @where_nullable , ') AND ', @from_where_part, 'WHERE ', @where_part, ') INTO @invalid_key_count;');
            SET @invalid_query = CONCAT('SELECT * ', @from_part, ' WHERE (', @where_nullable , ') AND ', @from_where_part, 'WHERE ', @where_part, ')');
            PREPARE stmt FROM @full_query;

            EXECUTE stmt;
            IF @invalid_key_count > 0 THEN
               IF LENGTH(@Resultado) = 0 THEN
                  SET @Resultado = 'FOREIGN KEYS NO VALIDAS:'; 
               END IF;
               SET @Resultado = CONCAT(@Resultado, '\r', @prev_Esquema, '.', @prev_Tabla, ' (', @Columnas, ') [', @prev_NombreConstraint, '] -> ', @prev_Esquema_Ref, '.', @prev_Tabla_Ref, ' (', @Columns_Ref, ') ', @invalid_key_count,' reg. sin FK: ', @invalid_query); 
            END IF;
            DEALLOCATE PREPARE stmt; 

            SET @where_part = '';
            SET @where_nullable = '';

            SET @Columnas = '';
            SET @Columns_Ref = '';
        END IF;

        IF (LENGTH(@where_part) > 0) THEN
            SET @where_nullable = CONCAT(@where_nullable, ' OR ');
            SET @where_part = CONCAT(@where_part, ' AND ');

            SET @Columnas = CONCAT(@Columnas, ', ', VColumna);
            SET @Columns_Ref = CONCAT(@Columns_Ref, ', ', VColumna_Ref);
        ELSE
            SET @from_part = CONCAT('FROM ', '`', VEsquema, '`.`', VTabla, '`', ' AS REFERRING ');
            SET @from_where_part = CONCAT('NOT EXISTS (SELECT * FROM `', VEsquema_Ref, '`.`', VTabla_Ref, '`', ' AS REFERRED ');

            SET @Columnas = VColumna;
            SET @Columns_Ref = VColumna_Ref;
        END IF;

        SET @where_nullable = CONCAT(@where_nullable, 'REFERRING.', VColumna, ' IS NOT NULL');
        SET @where_part = CONCAT(@where_part, 'REFERRING.', VColumna, ' = ', 'REFERRED.', VColumna_Ref);

        SET @prev_NombreConstraint = VNombreConstraint;
        SET @prev_Esquema = VEsquema;
        SET @prev_Tabla = VTabla;
        SET @prev_Esquema_Ref = VEsquema_Ref;
        SET @prev_Tabla_Ref = VTabla_Ref;

    END LOOP cForeignKeys_Loop;
    CLOSE cForeignKeys;

    IF (@where_part <> '' AND @from_part <> '') THEN
        SET @full_query = CONCAT('SELECT COUNT(*) ', @from_part, ' WHERE (', @where_nullable , ') AND ', @from_where_part, 'WHERE ', @where_part, ') INTO @invalid_key_count;');
        SET @invalid_query = CONCAT('SELECT * ', @from_part, ' WHERE (', @where_nullable , ') AND ', @from_where_part, 'WHERE ', @where_part, ')');
        PREPARE stmt FROM @full_query;

        EXECUTE stmt;
        IF @invalid_key_count > 0 THEN
            IF LENGTH(@Resultado) = 0 THEN
              SET @Resultado = 'FOREIGN KEYS NO VALIDAS:'; 
            END IF;
            SET @Resultado = CONCAT(@Resultado, '\r', @prev_Esquema, '.', @prev_Tabla, ' (', @Columnas, ') ', @prev_NombreConstraint, ' -> ', @prev_Esquema_Ref, '.', @prev_Tabla_Ref, ' (', @Columns_Ref, ') ', @invalid_key_count,'reg. sin FK: ', @invalid_query); 
        END IF;
        DEALLOCATE PREPARE stmt; 
    END IF;
    
    SELECT @Resultado RESULTADO;
END$$

DROP FUNCTION IF EXISTS DiffEsquemaConf
$$

CREATE FUNCTION DiffEsquemaConf(PConfiguracion VARCHAR(64)) RETURNS TEXT READS SQL DATA
# Comprueba que el modelo de datos es el mismo que el del esquema ONLINE
BEGIN
    DECLARE VEsquemaOnLine      VARCHAR(64);
    DECLARE VEsquema            VARCHAR(64);
	DECLARE VResultado          TEXT;

    DECLARE I                   INT;

    DECLARE VTabla              VARCHAR(1000);
    DECLARE VColumna            VARCHAR(1000); 
    DECLARE VEs_Null            VARCHAR(1000);
    DECLARE VTipo_Columna       VARCHAR(1000);

    DECLARE VTrigger            VARCHAR(1000);
    DECLARE VEvento             VARCHAR(1000);
    DECLARE VEvento_Tabla       VARCHAR(1000);
    DECLARE VAccion_Sobre       VARCHAR(1000);
    DECLARE VAccion             VARCHAR(1000);

    DECLARE VRutina             VARCHAR(1000);
    DECLARE VTipo_Rutina        VARCHAR(1000);
    DECLARE VIdentificador_DTD  VARCHAR(1000);
    
    DECLARE MensajeErr          VARCHAR(1000);
  
    DECLARE exNotFound INT DEFAULT FALSE;

    DECLARE cColumnasEsqOnline CURSOR FOR
            SELECT A.TABLE_NAME
                 , A.COLUMN_NAME
                 , A.IS_NULLABLE
                 , A.COLUMN_TYPE
             FROM (SELECT * FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_SCHEMA = VEsquemaOnline) A
             LEFT JOIN (SELECT * FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_SCHEMA = VEsquema) B 
                    ON IFNULL(A.TABLE_NAME , 'ES NULL') = IFNULL(B.TABLE_NAME, 'ES NULL')
                   AND IFNULL(A.COLUMN_NAME, 'ES NULL') = IFNULL(B.COLUMN_NAME, 'ES NULL')
                   AND IFNULL(A.IS_NULLABLE, 'ES NULL') = IFNULL(B.IS_NULLABLE, 'ES NULL') 
                   AND IFNULL(A.COLUMN_TYPE, 'ES NULL') = IFNULL(B.COLUMN_TYPE, 'ES NULL')
            WHERE B.TABLE_NAME IS NULL;

    DECLARE cColumnasEsqCheck CURSOR FOR
            SELECT A.TABLE_NAME
                 , A.COLUMN_NAME
                 , A.IS_NULLABLE
                 , A.COLUMN_TYPE
             FROM (SELECT * FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_SCHEMA = VEsquema) A
             LEFT JOIN (SELECT * FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_SCHEMA = VEsquemaOnline) B 
                    ON IFNULL(A.TABLE_NAME , 'ES NULL') = IFNULL(B.TABLE_NAME, 'ES NULL')
                   AND IFNULL(A.COLUMN_NAME, 'ES NULL') = IFNULL(B.COLUMN_NAME, 'ES NULL')
                   AND IFNULL(A.IS_NULLABLE, 'ES NULL') = IFNULL(B.IS_NULLABLE, 'ES NULL') 
                   AND IFNULL(A.COLUMN_TYPE, 'ES NULL') = IFNULL(B.COLUMN_TYPE, 'ES NULL')
            WHERE B.TABLE_NAME IS NULL;

    DECLARE cTriggersEsqOnline CURSOR FOR
            SELECT A.TRIGGER_NAME
                 , A.EVENT_MANIPULATION
                 , A.EVENT_OBJECT_TABLE
                 , A.ACTION_ORIENTATION
                 , A.ACTION_TIMING
             FROM (SELECT * FROM INFORMATION_SCHEMA.TRIGGERS WHERE TRIGGER_SCHEMA = VEsquemaOnline) A
             LEFT JOIN (SELECT * FROM INFORMATION_SCHEMA.TRIGGERS WHERE TRIGGER_SCHEMA = VEsquema) B 
                    ON IFNULL(A.TRIGGER_NAME      , 'ES NULL') = IFNULL(B.TRIGGER_NAME, 'ES NULL')
                   AND IFNULL(A.EVENT_MANIPULATION, 'ES NULL') = IFNULL(B.EVENT_MANIPULATION, 'ES NULL')
                   AND IFNULL(A.EVENT_OBJECT_TABLE, 'ES NULL') = IFNULL(B.EVENT_OBJECT_TABLE, 'ES NULL') 
                   AND IFNULL(A.ACTION_ORIENTATION, 'ES NULL') = IFNULL(B.ACTION_ORIENTATION, 'ES NULL')
                   AND IFNULL(A.ACTION_TIMING     , 'ES NULL') = IFNULL(B.ACTION_TIMING, 'ES NULL')
             WHERE B.TRIGGER_NAME IS NULL;

    DECLARE cTriggersEsqCheck CURSOR FOR
            SELECT A.TRIGGER_NAME
                 , A.EVENT_MANIPULATION
                 , A.EVENT_OBJECT_TABLE
                 , A.ACTION_ORIENTATION
                 , A.ACTION_TIMING
             FROM (SELECT * FROM INFORMATION_SCHEMA.TRIGGERS WHERE TRIGGER_SCHEMA = VEsquema) A
             LEFT JOIN (SELECT * FROM INFORMATION_SCHEMA.TRIGGERS WHERE TRIGGER_SCHEMA = VEsquemaOnline) B 
                    ON IFNULL(A.TRIGGER_NAME      , 'ES NULL') = IFNULL(B.TRIGGER_NAME, 'ES NULL')
                   AND IFNULL(A.EVENT_MANIPULATION, 'ES NULL') = IFNULL(B.EVENT_MANIPULATION, 'ES NULL')
                   AND IFNULL(A.EVENT_OBJECT_TABLE, 'ES NULL') = IFNULL(B.EVENT_OBJECT_TABLE, 'ES NULL') 
                   AND IFNULL(A.ACTION_ORIENTATION, 'ES NULL') = IFNULL(B.ACTION_ORIENTATION, 'ES NULL')
                   AND IFNULL(A.ACTION_TIMING     , 'ES NULL') = IFNULL(B.ACTION_TIMING, 'ES NULL')
             WHERE B.TRIGGER_NAME IS NULL;

    DECLARE cRutinasEsqOnline CURSOR FOR
            SELECT A.ROUTINE_NAME
                 , A.ROUTINE_TYPE
                 , A.DTD_IDENTIFIER
             FROM (SELECT * FROM INFORMATION_SCHEMA.ROUTINES WHERE ROUTINE_SCHEMA = VEsquemaOnline) A
             LEFT JOIN (SELECT * FROM INFORMATION_SCHEMA.ROUTINES WHERE ROUTINE_SCHEMA = VEsquema) B 
                    ON IFNULL(A.ROUTINE_NAME  , 'ES NULL') = IFNULL(B.ROUTINE_NAME  , 'ES NULL')
                   AND IFNULL(A.ROUTINE_TYPE  , 'ES NULL') = IFNULL(B.ROUTINE_TYPE  , 'ES NULL')
                   AND IFNULL(A.DTD_IDENTIFIER, 'ES NULL') = IFNULL(B.DTD_IDENTIFIER, 'ES NULL') 
             WHERE B.ROUTINE_NAME IS NULL;

    DECLARE cRutinasEsqCheck CURSOR FOR
            SELECT A.ROUTINE_NAME
                 , A.ROUTINE_TYPE
                 , A.DTD_IDENTIFIER
             FROM (SELECT * FROM INFORMATION_SCHEMA.ROUTINES WHERE ROUTINE_SCHEMA = VEsquema) A
             LEFT JOIN (SELECT * FROM INFORMATION_SCHEMA.ROUTINES WHERE ROUTINE_SCHEMA = VEsquemaOnline) B 
                    ON IFNULL(A.ROUTINE_NAME  , 'ES NULL') = IFNULL(B.ROUTINE_NAME  , 'ES NULL')
                   AND IFNULL(A.ROUTINE_TYPE  , 'ES NULL') = IFNULL(B.ROUTINE_TYPE  , 'ES NULL')
                   AND IFNULL(A.DTD_IDENTIFIER, 'ES NULL') = IFNULL(B.DTD_IDENTIFIER, 'ES NULL') 
             WHERE B.ROUTINE_NAME IS NULL;

    DECLARE CONTINUE HANDLER FOR NOT FOUND SET exNotFound = TRUE;

    SELECT NOMBRE INTO VEsquemaOnline FROM CONFIGURACIONES WHERE TIPO = 'ONLINE';
    SELECT SCHEMA_NAME INTO VEsquemaOnline FROM INFORMATION_SCHEMA.SCHEMATA
     WHERE SCHEMA_NAME = VEsquemaOnline;
     
    IF exNotFound THEN
      SET MensajeErr = CONCAT('El esquema online ', IFNULL(VEsquemaOnline, ''), ' no existe');
      SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = MensajeErr;
    END IF;

    SET exNotFound = FALSE;
    SELECT SCHEMA_NAME INTO VEsquema FROM INFORMATION_SCHEMA.SCHEMATA
     WHERE SCHEMA_NAME = PConfiguracion;
     
    IF exNotFound THEN
      SET MensajeErr = CONCAT('El esquema a comprobar ', IFNULL(PConfiguracion, ''), ' no existe');
      SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = MensajeErr;
    END IF;

    SET VResultado = '';

    SET exNotFound = FALSE;
    OPEN cColumnasEsqOnline;
    cColumnasEsqOnline_Loop: 
    LOOP               
      FETCH cColumnasEsqOnline INTO VTabla
                                  , VColumna
                                  , VEs_Null
                                  , VTipo_Columna;
      IF exNotFound THEN LEAVE cColumnasEsqOnline_Loop; END IF;

      IF LENGTH(VResultado) = 0 THEN
        SET VResultado = CONCAT('Columnas SOLO en ', VEsquemaOnline); 
      END IF;
      SET VResultado = CONCAT(VResultado, '\r   ', VTabla, '.', VColumna, ' ', VTipo_Columna, ' ... IS NULLABLE? ', VEs_Null); 
    END LOOP cColumnasEsqOnline_Loop;
    CLOSE cColumnasEsqOnline;

    SET exNotFound = FALSE;
    SET I = 0;
    OPEN cColumnasEsqCheck;
    cColumnasEsqCheck_Loop: 
    LOOP               
      FETCH cColumnasEsqCheck INTO VTabla
                                 , VColumna
                                 , VEs_Null
                                 , VTipo_Columna;
      IF exNotFound THEN LEAVE cColumnasEsqCheck_Loop; END IF;
      
      SET I = I + 1;
      IF LENGTH(VResultado) = 0 THEN
        SET VResultado = CONCAT('Columnas SOLO en ', VEsquema); 
      END IF;
      IF I = 1 THEN
				SET VResultado = CONCAT(VResultado, '\rColumnas SOLO en ', VEsquema); 
      END IF;
      SET VResultado = CONCAT(VResultado, '\r   ', VTabla, '.', VColumna, ' ', VTipo_Columna, ' ... IS NULLABLE? ', VEs_Null); 
    END LOOP cColumnasEsqCheck_Loop;
    CLOSE cColumnasEsqCheck;

    SET exNotFound = FALSE;
    SET I = 0;
    OPEN cTriggersEsqOnline;
    cTriggersEsqOnline_Loop: 
    LOOP               
      FETCH cTriggersEsqOnline INTO VTrigger
	                                , VEvento
	                                , VEvento_Tabla
	                                , VAccion_Sobre
	                                , VAccion;
      IF exNotFound THEN LEAVE cTriggersEsqOnline_Loop; END IF;
      
      SET I = I + 1;
      IF LENGTH(VResultado) = 0 THEN
        SET VResultado = CONCAT('Triggers SOLO en ', VEsquemaOnline); 
      END IF;
      IF I = 1 THEN
				SET VResultado = CONCAT(VResultado, '\rTriggers SOLO en ', VEsquemaOnline); 
      END IF;
      SET VResultado = CONCAT(VResultado, '\r   ', VTrigger, ': ', VAccion, ' ', VEvento, ' ', VEvento_Tabla, ' - ', VAccion_Sobre); 
    END LOOP cTriggersEsqOnline_Loop;
    CLOSE cTriggersEsqOnline;

    SET exNotFound = FALSE;
    SET I = 0;
    OPEN cTriggersEsqCheck;
    cTriggersEsqCheck_Loop: 
    LOOP               
      FETCH cTriggersEsqCheck INTO VTrigger
																 , VEvento
	                               , VEvento_Tabla
	                               , VAccion_Sobre
	                               , VAccion;
      IF exNotFound THEN LEAVE cTriggersEsqCheck_Loop; END IF;
      
      SET I = I + 1;
      IF LENGTH(VResultado) = 0 THEN
        SET VResultado = CONCAT('Triggers SOLO en ', VEsquema); 
      END IF;
      IF I = 1 THEN
				SET VResultado = CONCAT(VResultado, '\rTriggers SOLO en ', VEsquema); 
      END IF;
      SET VResultado = CONCAT(VResultado, '\r   ', VTrigger, ': ', VAccion, ' ', VEvento, ' ', VEvento_Tabla, ' - ', VAccion_Sobre); 
    END LOOP cTriggersEsqCheck_Loop;
    CLOSE cTriggersEsqCheck;

    SET exNotFound = FALSE;
    SET I = 0;
    OPEN cRutinasEsqOnline;
    cRutinasEsqOnline_Loop: 
    LOOP               
      FETCH cRutinasEsqOnline INTO VRutina
	                               , VTipo_Rutina
	                               , VIdentificador_DTD;
      IF exNotFound THEN LEAVE cRutinasEsqOnline_Loop; END IF;
      
      SET I = I + 1;
      IF LENGTH(VResultado) = 0 THEN
        SET VResultado = CONCAT('Rutinas SOLO en ', VEsquemaOnline); 
      END IF;
      IF I = 1 THEN
				SET VResultado = CONCAT(VResultado, '\rRutinas SOLO en ', VEsquemaOnline); 
      END IF;
      SET VResultado = CONCAT(VResultado, '\r   ', VTipo_Rutina, ' ', VRutina, ' - ', VIdentificador_DTD); 
    END LOOP cRutinasEsqOnline_Loop;
    CLOSE cRutinasEsqOnline;

    SET exNotFound = FALSE;
    SET I = 0;
    OPEN cRutinasEsqCheck;
    cRutinasEsqCheck_Loop: 
    LOOP               
      FETCH cRutinasEsqCheck INTO VRutina
	                              , VTipo_Rutina
	                              , VIdentificador_DTD;
      IF exNotFound THEN LEAVE cRutinasEsqCheck_Loop; END IF;
      
      SET I = I + 1;
      IF LENGTH(VResultado) = 0 THEN
        SET VResultado = CONCAT('Rutinas SOLO en ', VEsquema); 
      END IF;
      IF I = 1 THEN
				SET VResultado = CONCAT(VResultado, '\rRutinas SOLO en ', VEsquema); 
      END IF;
      SET VResultado = CONCAT(VResultado, '\r   ', VTipo_Rutina, ' ', VRutina, ' - ', VIdentificador_DTD); 
    END LOOP cRutinasEsqCheck_Loop;
    CLOSE cRutinasEsqCheck;

    RETURN VResultado;
END$$

DROP FUNCTION IF EXISTS DiffEsquemaConfEdicion
$$

CREATE FUNCTION DiffEsquemaConfEdicion(PConfiguracion VARCHAR(64)) RETURNS TEXT READS SQL DATA
# Comprueba que el modelo de datos es el mismo que el del esquema ONLINE
BEGIN
    DECLARE VEsquemaOnLine      VARCHAR(64);
    DECLARE VEsquema            VARCHAR(64);
    DECLARE VResultado          TEXT;
    
    DECLARE ExisteConf          TINYINT(1);
    DECLARE ExisteBD            TINYINT(1);
    DECLARE EsOnline            TINYINT(1);

    DECLARE I                   INT;

    DECLARE VTabla              VARCHAR(1000);
    DECLARE VColumna            VARCHAR(1000); 
    DECLARE VEs_Null            VARCHAR(1000);
    DECLARE VTipo_Columna       VARCHAR(1000);

    DECLARE VTrigger            VARCHAR(1000);
    DECLARE VEvento             VARCHAR(1000);
    DECLARE VEvento_Tabla       VARCHAR(1000);
    DECLARE VAccion_Sobre       VARCHAR(1000);
    DECLARE VAccion             VARCHAR(1000);

    DECLARE VRutina             VARCHAR(1000);
    DECLARE VTipo_Rutina        VARCHAR(1000);
    DECLARE VIdentificador_DTD  VARCHAR(1000);
    
    DECLARE MensajeErr          VARCHAR(1000);
  
    DECLARE exNotFound INT DEFAULT FALSE;

    DECLARE cColumnasEsqOnline CURSOR FOR
            SELECT A.TABLE_NAME
                 , A.COLUMN_NAME
                 , A.IS_NULLABLE
                 , A.COLUMN_TYPE
             FROM (SELECT * FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_SCHEMA = VEsquemaOnline) A
             LEFT JOIN (SELECT * FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_SCHEMA = VEsquema) B 
                    ON IFNULL(A.TABLE_NAME , 'ES NULL') = IFNULL(B.TABLE_NAME, 'ES NULL')
                   AND IFNULL(A.COLUMN_NAME, 'ES NULL') = IFNULL(B.COLUMN_NAME, 'ES NULL')
                   AND IFNULL(A.IS_NULLABLE, 'ES NULL') = IFNULL(B.IS_NULLABLE, 'ES NULL') 
                   AND IFNULL(A.COLUMN_TYPE, 'ES NULL') = IFNULL(B.COLUMN_TYPE, 'ES NULL')
            WHERE B.TABLE_NAME IS NULL;

    DECLARE cColumnasEsqCheck CURSOR FOR
            SELECT A.TABLE_NAME
                 , A.COLUMN_NAME
                 , A.IS_NULLABLE
                 , A.COLUMN_TYPE
             FROM (SELECT * FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_SCHEMA = VEsquema) A
             LEFT JOIN (SELECT * FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_SCHEMA = VEsquemaOnline) B 
                    ON IFNULL(A.TABLE_NAME , 'ES NULL') = IFNULL(B.TABLE_NAME, 'ES NULL')
                   AND IFNULL(A.COLUMN_NAME, 'ES NULL') = IFNULL(B.COLUMN_NAME, 'ES NULL')
                   AND IFNULL(A.IS_NULLABLE, 'ES NULL') = IFNULL(B.IS_NULLABLE, 'ES NULL') 
                   AND IFNULL(A.COLUMN_TYPE, 'ES NULL') = IFNULL(B.COLUMN_TYPE, 'ES NULL')
            WHERE B.TABLE_NAME IS NULL;

    DECLARE cTriggersEsqOnline CURSOR FOR
            SELECT A.TRIGGER_NAME
                 , A.EVENT_MANIPULATION
                 , A.EVENT_OBJECT_TABLE
                 , A.ACTION_ORIENTATION
                 , A.ACTION_TIMING
             FROM (SELECT * FROM INFORMATION_SCHEMA.TRIGGERS WHERE TRIGGER_SCHEMA = VEsquemaOnline) A
             LEFT JOIN (SELECT * FROM INFORMATION_SCHEMA.TRIGGERS WHERE TRIGGER_SCHEMA = VEsquema) B 
                    ON IFNULL(A.TRIGGER_NAME      , 'ES NULL') = IFNULL(B.TRIGGER_NAME, 'ES NULL')
                   AND IFNULL(A.EVENT_MANIPULATION, 'ES NULL') = IFNULL(B.EVENT_MANIPULATION, 'ES NULL')
                   AND IFNULL(A.EVENT_OBJECT_TABLE, 'ES NULL') = IFNULL(B.EVENT_OBJECT_TABLE, 'ES NULL') 
                   AND IFNULL(A.ACTION_ORIENTATION, 'ES NULL') = IFNULL(B.ACTION_ORIENTATION, 'ES NULL')
                   AND IFNULL(A.ACTION_TIMING     , 'ES NULL') = IFNULL(B.ACTION_TIMING, 'ES NULL')
             WHERE B.TRIGGER_NAME IS NULL;

    DECLARE cTriggersEsqCheck CURSOR FOR
            SELECT A.TRIGGER_NAME
                 , A.EVENT_MANIPULATION
                 , A.EVENT_OBJECT_TABLE
                 , A.ACTION_ORIENTATION
                 , A.ACTION_TIMING
             FROM (SELECT * FROM INFORMATION_SCHEMA.TRIGGERS WHERE TRIGGER_SCHEMA = VEsquema) A
             LEFT JOIN (SELECT * FROM INFORMATION_SCHEMA.TRIGGERS WHERE TRIGGER_SCHEMA = VEsquemaOnline) B 
                    ON IFNULL(A.TRIGGER_NAME      , 'ES NULL') = IFNULL(B.TRIGGER_NAME, 'ES NULL')
                   AND IFNULL(A.EVENT_MANIPULATION, 'ES NULL') = IFNULL(B.EVENT_MANIPULATION, 'ES NULL')
                   AND IFNULL(A.EVENT_OBJECT_TABLE, 'ES NULL') = IFNULL(B.EVENT_OBJECT_TABLE, 'ES NULL') 
                   AND IFNULL(A.ACTION_ORIENTATION, 'ES NULL') = IFNULL(B.ACTION_ORIENTATION, 'ES NULL')
                   AND IFNULL(A.ACTION_TIMING     , 'ES NULL') = IFNULL(B.ACTION_TIMING, 'ES NULL')
             WHERE B.TRIGGER_NAME IS NULL;

    DECLARE cRutinasEsqOnline CURSOR FOR
            SELECT A.ROUTINE_NAME
                 , A.ROUTINE_TYPE
                 , A.DTD_IDENTIFIER
             FROM (SELECT * FROM INFORMATION_SCHEMA.ROUTINES WHERE ROUTINE_SCHEMA = VEsquemaOnline) A
             LEFT JOIN (SELECT * FROM INFORMATION_SCHEMA.ROUTINES WHERE ROUTINE_SCHEMA = VEsquema) B 
                    ON IFNULL(A.ROUTINE_NAME  , 'ES NULL') = IFNULL(B.ROUTINE_NAME  , 'ES NULL')
                   AND IFNULL(A.ROUTINE_TYPE  , 'ES NULL') = IFNULL(B.ROUTINE_TYPE  , 'ES NULL')
                   AND IFNULL(A.DTD_IDENTIFIER, 'ES NULL') = IFNULL(B.DTD_IDENTIFIER, 'ES NULL') 
             WHERE B.ROUTINE_NAME IS NULL;

    DECLARE cRutinasEsqCheck CURSOR FOR
            SELECT A.ROUTINE_NAME
                 , A.ROUTINE_TYPE
                 , A.DTD_IDENTIFIER
             FROM (SELECT * FROM INFORMATION_SCHEMA.ROUTINES WHERE ROUTINE_SCHEMA = VEsquema) A
             LEFT JOIN (SELECT * FROM INFORMATION_SCHEMA.ROUTINES WHERE ROUTINE_SCHEMA = VEsquemaOnline) B 
                    ON IFNULL(A.ROUTINE_NAME  , 'ES NULL') = IFNULL(B.ROUTINE_NAME  , 'ES NULL')
                   AND IFNULL(A.ROUTINE_TYPE  , 'ES NULL') = IFNULL(B.ROUTINE_TYPE  , 'ES NULL')
                   AND IFNULL(A.DTD_IDENTIFIER, 'ES NULL') = IFNULL(B.DTD_IDENTIFIER, 'ES NULL') 
             WHERE B.ROUTINE_NAME IS NULL;

    DECLARE CONTINUE HANDLER FOR NOT FOUND SET exNotFound = TRUE;

    SELECT CONF_EDICION INTO VEsquemaOnline FROM CONFIGURACIONES WHERE TIPO = 'ONLINE';
    SELECT SCHEMA_NAME INTO VEsquemaOnline FROM INFORMATION_SCHEMA.SCHEMATA
     WHERE SCHEMA_NAME = VEsquemaOnline;
		
    IF exNotFound THEN
      SET MensajeErr = CONCAT('El esquema online ', IFNULL(VEsquemaOnline, ''), ' no existe');
      SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = MensajeErr;
    END IF;

    SET VEsquema = GetNombreConfEdicion(PConfiguracion);
    CALL CheckConfiguracionEdicion(VEsquema, ExisteConf, ExisteBD, EsOnline);
  
    IF EsOnline THEN
      SET MensajeErr = CONCAT('La configuración es ONLINE. No puede haber diferencias consigo misma.');
      SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = MensajeErr;
    END IF;

    IF NOT ExisteConf THEN
      SET MensajeErr = CONCAT('La configuración offline ', VEsquema, ' no está configurada en el sistema.');
      SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = MensajeErr;
    END IF;
  
    IF NOT ExisteBD THEN
      SET MensajeErr = CONCAT('No existe el esquema de la configuración offline ', VEsquema);
      SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = MensajeErr;
    END IF;
    
    SET VResultado = '';

    SET exNotFound = FALSE;
    SET I = 0;
    OPEN cColumnasEsqOnline;
    cColumnasEsqOnline_Loop: 
    LOOP               
      FETCH cColumnasEsqOnline INTO VTabla
                                  , VColumna
                                  , VEs_Null
                                  , VTipo_Columna;
      IF exNotFound THEN LEAVE cColumnasEsqOnline_Loop; END IF;
      
      SET I = I + 1;
      IF LENGTH(VResultado) = 0 THEN
        SET VResultado = CONCAT('Columnas SOLO en ', VEsquemaOnline); 
      END IF;
      SET VResultado = CONCAT(VResultado, '\r   ', VTabla, '.', VColumna, ' ', VTipo_Columna, ' ... IS NULLABLE? ', VEs_Null); 
    END LOOP cColumnasEsqOnline_Loop;
    CLOSE cColumnasEsqOnline;

    SET exNotFound = FALSE;
    SET I = 0;
    OPEN cColumnasEsqCheck;
    cColumnasEsqCheck_Loop: 
    LOOP               
      FETCH cColumnasEsqCheck INTO VTabla
                                 , VColumna
                                 , VEs_Null
                                 , VTipo_Columna;
      IF exNotFound THEN LEAVE cColumnasEsqCheck_Loop; END IF;
      
      SET I = I + 1;
      IF LENGTH(VResultado) = 0 THEN
        SET VResultado = CONCAT('Columnas SOLO en ', VEsquema); 
      END IF;
      IF I = 1 THEN
				SET VResultado = CONCAT(VResultado, '\rColumnas SOLO en ', VEsquema); 
      END IF;
      SET VResultado = CONCAT(VResultado, '\r   ', VTabla, '.', VColumna, ' ', VTipo_Columna, ' ... IS NULLABLE? ', VEs_Null); 
    END LOOP cColumnasEsqCheck_Loop;
    CLOSE cColumnasEsqCheck;

    SET exNotFound = FALSE;
    SET I = 0;
    OPEN cTriggersEsqOnline;
    cTriggersEsqOnline_Loop: 
    LOOP               
      FETCH cTriggersEsqOnline INTO VTrigger
	                                , VEvento
	                                , VEvento_Tabla
	                                , VAccion_Sobre
	                                , VAccion;
      IF exNotFound THEN LEAVE cTriggersEsqOnline_Loop; END IF;
      
      SET I = I + 1;
      IF LENGTH(VResultado) = 0 THEN
        SET VResultado = CONCAT('Triggers SOLO en ', VEsquemaOnline); 
      END IF;
      IF I = 1 THEN
				SET VResultado = CONCAT(VResultado, '\rTriggers SOLO en ', VEsquemaOnline); 
      END IF;
      SET VResultado = CONCAT(VResultado, '\r   ', VTrigger, ': ', VAccion, ' ', VEvento, ' ', VEvento_Tabla, ' - ', VAccion_Sobre); 
    END LOOP cTriggersEsqOnline_Loop;
    CLOSE cTriggersEsqOnline;

    SET exNotFound = FALSE;
    SET I = 0;
    OPEN cTriggersEsqCheck;
    cTriggersEsqCheck_Loop: 
    LOOP               
      FETCH cTriggersEsqCheck INTO VTrigger
																 , VEvento
	                               , VEvento_Tabla
	                               , VAccion_Sobre
	                               , VAccion;
      IF exNotFound THEN LEAVE cTriggersEsqCheck_Loop; END IF;
      
      SET I = I + 1;
      IF LENGTH(VResultado) = 0 THEN
        SET VResultado = CONCAT('Triggers SOLO en ', VEsquema); 
      END IF;
      IF I = 1 THEN
				SET VResultado = CONCAT(VResultado, '\rTriggers SOLO en ', VEsquema); 
      END IF;
      SET VResultado = CONCAT(VResultado, '\r   ', VTrigger, ': ', VAccion, ' ', VEvento, ' ', VEvento_Tabla, ' - ', VAccion_Sobre); 
    END LOOP cTriggersEsqCheck_Loop;
    CLOSE cTriggersEsqCheck;

    SET exNotFound = FALSE;
    SET I = 0;
    OPEN cRutinasEsqOnline;
    cRutinasEsqOnline_Loop: 
    LOOP               
      FETCH cRutinasEsqOnline INTO VRutina
	                               , VTipo_Rutina
	                               , VIdentificador_DTD;
      IF exNotFound THEN LEAVE cRutinasEsqOnline_Loop; END IF;
      
      SET I = I + 1;
      IF LENGTH(VResultado) = 0 THEN
        SET VResultado = CONCAT('Rutinas SOLO en ', VEsquemaOnline); 
      END IF;
      IF I = 1 THEN
				SET VResultado = CONCAT(VResultado, '\rRutinas SOLO en ', VEsquemaOnline); 
      END IF;
      SET VResultado = CONCAT(VResultado, '\r   ', VTipo_Rutina, ' ', VRutina, ' - ', VIdentificador_DTD); 
    END LOOP cRutinasEsqOnline_Loop;
    CLOSE cRutinasEsqOnline;

    SET exNotFound = FALSE;
    SET I = 0;
    OPEN cRutinasEsqCheck;
    cRutinasEsqCheck_Loop: 
    LOOP               
      FETCH cRutinasEsqCheck INTO VRutina
	                              , VTipo_Rutina
	                              , VIdentificador_DTD;
      IF exNotFound THEN LEAVE cRutinasEsqCheck_Loop; END IF;
      
      SET I = I + 1;
      IF LENGTH(VResultado) = 0 THEN
        SET VResultado = CONCAT('Rutinas SOLO en ', VEsquema); 
      END IF;
      IF I = 1 THEN
				SET VResultado = CONCAT(VResultado, '\rRutinas SOLO en ', VEsquema); 
      END IF;
      SET VResultado = CONCAT(VResultado, '\r   ', VTipo_Rutina, ' ', VRutina, ' - ', VIdentificador_DTD); 
    END LOOP cRutinasEsqCheck_Loop;
    CLOSE cRutinasEsqCheck;

    RETURN SUBSTRING(VResultado, 1, 1000);
END$$

DROP PROCEDURE IF EXISTS DropTriggersErr
$$

CREATE PROCEDURE DropTriggersErr(PEsquema VARCHAR(64)) NOT DETERMINISTIC
# Borra los triggers del esquema de la configuración que se le pasa como parámetro
# LOS TRIGGERS NO SE PUEDEN BORRAR DESDE UN PROCEDIMIENTO ALMACENADO, 
# habrá que hacerlo desde el script .bat
BEGIN
    DECLARE VEsquema            VARCHAR(64);
    DECLARE VTrigger            VARCHAR(1000);

    DECLARE MensajeErr          VARCHAR(1000);  
    DECLARE exNotFound INT DEFAULT FALSE;

    DECLARE cTriggers CURSOR FOR
            SELECT TRIGGER_NAME
             FROM INFORMATION_SCHEMA.TRIGGERS WHERE TRIGGER_SCHEMA = VEsquema;

    DECLARE CONTINUE HANDLER FOR NOT FOUND SET exNotFound = TRUE;

    SET exNotFound = FALSE;
    SELECT SCHEMA_NAME INTO VEsquema FROM INFORMATION_SCHEMA.SCHEMATA WHERE SCHEMA_NAME = PEsquema;
    IF exNotFound THEN
      SET MensajeErr = CONCAT('El esquema de BD ', PEsquema, ' no existe.');
      SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = MensajeErr;
    END IF;

    SET exNotFound = FALSE;
    OPEN cTriggers;
    cTriggers_Loop: 
    LOOP               
      FETCH cTriggers  INTO VTrigger;
      IF exNotFound THEN LEAVE cTriggers_Loop; END IF;
      
      SET @sql = CONCAT('DROP TRIGGER IF EXISTS ', VEsquema, '.', VTrigger);
      PREPARE STMT FROM @sql;
      EXECUTE STMT;
      DEALLOCATE PREPARE STMT;
    END LOOP cTriggers_Loop;
    CLOSE cTriggers;
END$$


DELIMITER ;