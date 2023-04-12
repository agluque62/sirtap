-- MySQL dump 10.13  Distrib 5.7.17, for Win64 (x86_64)
--
-- Host: localhost    Database: new_cd40
-- ------------------------------------------------------
-- Server version	5.6.11-log

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Current Database: `new_cd40`
--

CREATE DATABASE /*!32312 IF NOT EXISTS*/ `new_cd40` /*!40100 DEFAULT CHARACTER SET utf8 */;

USE `new_cd40`;

--
-- Table structure for table `abonados`
--

DROP TABLE IF EXISTS `abonados`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `abonados` (
  `IdSistema` varchar(32) NOT NULL,
  `IdAbonado` varchar(32) NOT NULL,
  PRIMARY KEY (`IdSistema`,`IdAbonado`),
  KEY `ABONADOS_FKIndex1` (`IdSistema`),
  CONSTRAINT `abonados_ibfk_1` FOREIGN KEY (`IdSistema`) REFERENCES `sistema` (`IdSistema`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `agenda`
--

DROP TABLE IF EXISTS `agenda`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `agenda` (
  `IdSistema` varchar(32) NOT NULL,
  `IdNucleo` varchar(32) NOT NULL,
  `IdSector` varchar(700) NOT NULL,
  `Prefijo` int(2) unsigned NOT NULL DEFAULT '0',
  `Numero` varchar(20) NOT NULL DEFAULT '',
  `Nombre` varchar(32) DEFAULT NULL,
  PRIMARY KEY (`IdSistema`,`IdNucleo`,`IdSector`,`Prefijo`,`Numero`),
  KEY `FK_AGENDA_REDES_idx` (`IdSistema`,`Prefijo`),
  CONSTRAINT `FK_AGENDA_REDES` FOREIGN KEY (`IdSistema`, `Prefijo`) REFERENCES `redes` (`IdSistema`, `IdPrefijo`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `FK_SECTORES` FOREIGN KEY (`IdSistema`, `IdNucleo`, `IdSector`) REFERENCES `sectores` (`IdSistema`, `IdNucleo`, `IdSector`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `agrupaciones`
--

DROP TABLE IF EXISTS `agrupaciones`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `agrupaciones` (
  `IdSistema` varchar(32) NOT NULL,
  `IdAgrupacion` varchar(32) NOT NULL,
  PRIMARY KEY (`IdSistema`,`IdAgrupacion`),
  KEY `Agrupaciones_FKIndex1` (`IdSistema`),
  CONSTRAINT `agrupaciones_ibfk_1` FOREIGN KEY (`IdSistema`) REFERENCES `sistema` (`IdSistema`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `alarmas`
--

DROP TABLE IF EXISTS `alarmas`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `alarmas` (
  `IdSistema` varchar(32) NOT NULL,
  `IdIncidencia` int(10) unsigned NOT NULL,
  `Alarma` tinyint(1) NOT NULL,
  PRIMARY KEY (`IdSistema`,`IdIncidencia`),
  KEY `Alarmas_FKIndex1` (`IdSistema`),
  KEY `Alarmas_FKIndex2` (`IdIncidencia`),
  CONSTRAINT `alarmas_ibfk_1` FOREIGN KEY (`IdSistema`) REFERENCES `sistema` (`IdSistema`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `alarmas_ibfk_2` FOREIGN KEY (`IdIncidencia`) REFERENCES `incidencias` (`IdIncidencia`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `altavoces`
--

DROP TABLE IF EXISTS `altavoces`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `altavoces` (
  `IdSistema` varchar(32) NOT NULL,
  `IdDestino` varchar(700) NOT NULL,
  `TipoDestino` int(10) unsigned NOT NULL,
  `IdNucleo` varchar(32) NOT NULL,
  `IdSector` varchar(700) NOT NULL,
  `PosHMI` int(10) unsigned NOT NULL,
  `NumAltavoz` int(10) unsigned NOT NULL,
  `Estado` varchar(1) DEFAULT NULL,
  PRIMARY KEY (`IdSistema`,`IdDestino`,`TipoDestino`,`IdNucleo`,`IdSector`,`PosHMI`,`NumAltavoz`),
  KEY `Altavoces_FKIndex1` (`IdSistema`,`IdDestino`,`TipoDestino`,`IdNucleo`,`IdSector`,`PosHMI`),
  CONSTRAINT `altavoces_ibfk_1` FOREIGN KEY (`IdSistema`, `IdDestino`, `TipoDestino`, `IdNucleo`, `IdSector`, `PosHMI`) REFERENCES `destinosradiosector` (`IdSistema`, `IdDestino`, `TipoDestino`, `IdNucleo`, `IdSector`, `PosHMI`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `controlbackup`
--

DROP TABLE IF EXISTS `controlbackup`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `controlbackup` (
  `IdSistema` varchar(32) NOT NULL,
  `TipoBackup` int(11) NOT NULL,
  `Profundidad` int(11) unsigned NOT NULL,
  `Recurso` varchar(128) NOT NULL,
  PRIMARY KEY (`IdSistema`,`TipoBackup`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `dependenciasats`
--

DROP TABLE IF EXISTS `dependenciasats`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `dependenciasats` (
  `IdDependenciaATS` varchar(32) NOT NULL,
  `IdSistema` varchar(32) NOT NULL,
  PRIMARY KEY (`IdDependenciaATS`,`IdSistema`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `destinos`
--

DROP TABLE IF EXISTS `destinos`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `destinos` (
  `IdSistema` varchar(32) NOT NULL,
  `IdDestino` varchar(700) NOT NULL,
  `TipoDestino` int(10) unsigned NOT NULL COMMENT '0: Destino Radio\n1: Destino de telefonía\n2: Destino de telefonía interno\n',
  PRIMARY KEY (`IdSistema`,`IdDestino`,`TipoDestino`),
  KEY `Destinos_FKIndex1` (`IdSistema`),
  CONSTRAINT `destinos_ibfk_1` FOREIGN KEY (`IdSistema`) REFERENCES `sistema` (`IdSistema`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Temporary view structure for view `destinosenequipoexterno`
--

DROP TABLE IF EXISTS `destinosenequipoexterno`;
/*!50001 DROP VIEW IF EXISTS `destinosenequipoexterno`*/;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
/*!50001 CREATE VIEW `destinosenequipoexterno` AS SELECT 
 1 AS `IdDestino`,
 1 AS `IpRed1`,
 1 AS `IpRed2`,
 1 AS `idEquipos`*/;
SET character_set_client = @saved_cs_client;

--
-- Temporary view structure for view `destinosenequipoexternoparaoptions`
--

DROP TABLE IF EXISTS `destinosenequipoexternoparaoptions`;
/*!50001 DROP VIEW IF EXISTS `destinosenequipoexternoparaoptions`*/;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
/*!50001 CREATE VIEW `destinosenequipoexternoparaoptions` AS SELECT 
 1 AS `idEquipos`,
 1 AS `IpRed1`,
 1 AS `IpRed2`,
 1 AS `SipPort`,
 1 AS `TipoEquipo`,
 1 AS `modeloequipo`,
 1 AS `tiporadio`,
 1 AS `IdRecurso`,
 1 AS `IdDestino`*/;
SET character_set_client = @saved_cs_client;

--
-- Table structure for table `destinosexternos`
--

DROP TABLE IF EXISTS `destinosexternos`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `destinosexternos` (
  `IdSistema` varchar(32) NOT NULL,
  `IdDestino` varchar(700) NOT NULL,
  `TipoDestino` int(10) unsigned NOT NULL,
  `IdPrefijo` int(10) unsigned NOT NULL,
  `IdAbonado` varchar(32) DEFAULT NULL,
  PRIMARY KEY (`IdSistema`,`IdDestino`,`TipoDestino`,`IdPrefijo`),
  KEY `Table_44_FKIndex1` (`IdSistema`,`IdDestino`,`TipoDestino`,`IdPrefijo`),
  CONSTRAINT `Table_44_FKIndex1` FOREIGN KEY (`IdSistema`, `IdDestino`, `TipoDestino`, `IdPrefijo`) REFERENCES `destinostelefonia` (`IdSistema`, `IdDestino`, `TipoDestino`, `IdPrefijo`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `destinosexternossector`
--

DROP TABLE IF EXISTS `destinosexternossector`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `destinosexternossector` (
  `IdSistema` varchar(32) NOT NULL,
  `IdNucleo` varchar(32) NOT NULL,
  `IdSector` varchar(700) NOT NULL,
  `IdDestino` varchar(700) NOT NULL,
  `TipoDestino` int(10) unsigned NOT NULL,
  `IdPrefijo` int(10) unsigned NOT NULL,
  `PosHMI` int(10) unsigned DEFAULT NULL,
  `Prioridad` int(10) unsigned DEFAULT NULL,
  `OrigenR2` varchar(32) DEFAULT NULL,
  `PrioridadSIP` int(10) unsigned DEFAULT NULL,
  `TipoAcceso` varchar(2) NOT NULL COMMENT 'Tipo de Acceso: DA (Panel de telefonía), IA (Panel de Linea caliente)',
  `Literal` varchar(32) DEFAULT NULL,
  `IdPrefijoDestinoLCEN` int(10) unsigned DEFAULT NULL COMMENT 'Para Destinos ATS asignados al panel de línea caliente (TipoAcceso=IA): Prefijo del destino LCEN asignado al Destino ATS. Tomará el valor 1',
  `IdDestinoLCEN` varchar(32) DEFAULT NULL COMMENT 'Para Destinos ATS asignados al panel de línea caliente (TipoAcceso=IA): Identificador del destino LCEN asignado al Destino ATS.',
  PRIMARY KEY (`IdSistema`,`IdNucleo`,`IdSector`,`IdDestino`,`TipoDestino`,`IdPrefijo`,`TipoAcceso`),
  KEY `Table_45_FKIndex1` (`IdSistema`,`IdDestino`,`TipoDestino`,`IdPrefijo`),
  KEY `Table_45_FKIndex2` (`IdSistema`,`IdNucleo`,`IdSector`),
  KEY `DestinosExternosSector_idDestinoLcen_idx` (`IdSistema`,`TipoAcceso`,`IdPrefijoDestinoLCEN`),
  KEY `DestinosExternosSector_PosHMI_idx` (`IdSistema`,`IdNucleo`,`PosHMI`,`IdPrefijo`,`TipoAcceso`),
  KEY `DestinosExternosSector_idDestinoLcen_fk` (`IdSistema`,`IdDestinoLCEN`,`TipoDestino`,`IdPrefijoDestinoLCEN`),
  CONSTRAINT `destinosexternossector_ibfk_1` FOREIGN KEY (`IdSistema`, `IdDestino`, `TipoDestino`, `IdPrefijo`) REFERENCES `destinosexternos` (`IdSistema`, `IdDestino`, `TipoDestino`, `IdPrefijo`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `destinosexternossector_ibfk_2` FOREIGN KEY (`IdSistema`, `IdNucleo`, `IdSector`) REFERENCES `sectores` (`IdSistema`, `IdNucleo`, `IdSector`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `destinosinternos`
--

DROP TABLE IF EXISTS `destinosinternos`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `destinosinternos` (
  `IdSistema` varchar(32) NOT NULL,
  `IdDestino` varchar(700) NOT NULL,
  `TipoDestino` int(10) unsigned NOT NULL,
  `IdPrefijo` int(10) unsigned NOT NULL,
  `IdSector` varchar(700) NOT NULL,
  `IdNucleo` varchar(32) NOT NULL,
  PRIMARY KEY (`IdSistema`,`IdDestino`,`TipoDestino`,`IdPrefijo`,`IdSector`,`IdNucleo`),
  KEY `Table_42_FKIndex1` (`IdSistema`,`IdDestino`,`TipoDestino`,`IdPrefijo`),
  KEY `Table_42_FKIndex2` (`IdSistema`,`IdNucleo`,`IdSector`),
  CONSTRAINT `destinosinternos_ibfk_1` FOREIGN KEY (`IdSistema`, `IdDestino`, `TipoDestino`, `IdPrefijo`) REFERENCES `destinostelefonia` (`IdSistema`, `IdDestino`, `TipoDestino`, `IdPrefijo`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `destinosinternos_ibfk_2` FOREIGN KEY (`IdSistema`, `IdNucleo`, `IdSector`) REFERENCES `sectores` (`IdSistema`, `IdNucleo`, `IdSector`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `destinosinternossector`
--

DROP TABLE IF EXISTS `destinosinternossector`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `destinosinternossector` (
  `IdSistema` varchar(32) NOT NULL,
  `IdDestino` varchar(32) NOT NULL,
  `TipoDestino` int(10) unsigned NOT NULL,
  `IdNucleo` varchar(32) NOT NULL,
  `IdSector` varchar(700) NOT NULL,
  `IdPrefijo` int(10) unsigned NOT NULL,
  `PosHMI` int(10) unsigned NOT NULL,
  `Prioridad` int(1) unsigned DEFAULT NULL,
  `OrigenR2` varchar(32) DEFAULT NULL,
  `PrioridadSIP` int(1) unsigned DEFAULT NULL,
  `TipoAcceso` varchar(2) NOT NULL,
  `Literal` varchar(32) DEFAULT NULL,
  PRIMARY KEY (`IdSistema`,`IdDestino`,`TipoDestino`,`IdNucleo`,`IdSector`,`IdPrefijo`),
  KEY `Table_43_FKIndex1` (`IdSistema`,`IdDestino`,`TipoDestino`,`IdPrefijo`),
  KEY `Table_43_FKIndex2` (`IdSistema`,`IdNucleo`,`IdSector`),
  CONSTRAINT `destinosinternossector_ibfk_1` FOREIGN KEY (`IdSistema`, `IdDestino`, `TipoDestino`, `IdPrefijo`) REFERENCES `destinosinternos` (`IdSistema`, `IdDestino`, `TipoDestino`, `IdPrefijo`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `destinosinternossector_ibfk_2` FOREIGN KEY (`IdSistema`, `IdNucleo`, `IdSector`) REFERENCES `sectores` (`IdSistema`, `IdNucleo`, `IdSector`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `destinosradio`
--

DROP TABLE IF EXISTS `destinosradio`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `destinosradio` (
  `fk_metodosbss` int(11) NOT NULL AUTO_INCREMENT,
  `IdSistema` varchar(32) NOT NULL,
  `IdDestino` varchar(700) NOT NULL,
  `TipoDestino` int(10) unsigned NOT NULL COMMENT '0: Destino Radio\n\n',
  `zonas_idZonas` int(11) DEFAULT NULL COMMENT 'Zona Tx por defecto',
  `TipoFrec` int(1) unsigned DEFAULT NULL COMMENT 'Tipo Frecuencia: 0 --> Modo Normal tipo VHF, 1-->Modo Normal frecuencia UHF, 2 Modo Normal frecuencia HF, 4 --> Modo 1+1, 5 --> Modo FD, 6 --> Modo EM',
  `ExclusividadTXRX` tinyint(1) DEFAULT NULL,
  `Frecuencia` int(8) DEFAULT '0',
  `MetodoCalculoClimax` int(1) DEFAULT '0' COMMENT '0: Relativo\n1: Absoluto',
  `VentanaSeleccionBss` int(11) DEFAULT '50' COMMENT '50..2000 msg.\n',
  `SincronizaGrupoClimax` tinyint(1) DEFAULT '0',
  `AudioPrimerSqBss` tinyint(1) DEFAULT '1',
  `FrecuenciaNoDesasignable` tinyint(1) DEFAULT '0',
  `VentanaReposoZonaTxDefecto` int(11) DEFAULT NULL COMMENT 'Ventana de Reposo para Zona TX por Defecto (Para emplazamientos múltiples, ahora no se utilzará)',
  `PrioridadSesionSip` int(11) DEFAULT '2' COMMENT '2: Normal\n0: Emergencia',
  `MetodosBssOfrecidos` int(1) DEFAULT '0' COMMENT '0: Ninguno\n1: RSSI\n2: RSSI + NUCLEO\n3: CENTRAL',
  `CldSupervisionTime` int(11) DEFAULT '1' COMMENT '1..60 sg.\n',
  `CnfModoDestino` varchar(1) DEFAULT NULL COMMENT 'Modo del destino: 0 (Normal), 1 (1+1),  2 (FD), 3 (EM)\nSólo se usa en la Web de configuración',
  `CnfTipoFrecuencia` varchar(1) DEFAULT NULL COMMENT 'Tipo de Frecuencia: 0 --> VHF, 1  --> UHF, 2 --> HF\nSólo se usa en la Web de configuración',
  `ModoTransmision` enum('R','C','M') DEFAULT NULL COMMENT '''Modo de Transmision para el modo destino FD(CnfModoDestino=2): R (Último Receptor), C (Climax), M (Manual). Para otro, modo de destino tomará el valor null.''',
  `EmplazamientoDefecto` varchar(32) DEFAULT NULL COMMENT 'Indica, para el caso de frecuencias desplazadas, el emplazamiento por defecto. Sino se deja a NULL para no devolverlo.',
  `TiempoVueltaADefecto` int(11) DEFAULT NULL COMMENT 'Tiempo en segundos en el que el que la frecuencia vuelve al emplazamiento por defecto. Si es cero la frecuencia nunca vuelve al emplazamiento por defecto.',
  `PorcentajeRSSI` int(1) DEFAULT NULL COMMENT 'Porcentaje de cálculo para el método BSS RSSI. \n0: No aplica\n1-9 indica el porcentaje',
  `ConRedundancia` enum('0','1') DEFAULT '0' COMMENT 'Indicador de si el destino es 1+1: 1 (si), 0 (No)',
  PRIMARY KEY (`fk_metodosbss`,`IdSistema`,`IdDestino`,`TipoDestino`),
  KEY `DestinosRadio_FKIndex1` (`IdSistema`,`IdDestino`,`TipoDestino`),
  KEY `fk_destinosradio_zonas1_idx` (`zonas_idZonas`),
  KEY `destinoRadio_emplazamiento_fk_idx` (`IdSistema`,`EmplazamientoDefecto`),
  CONSTRAINT `destinoRadio_emplazamiento_fk` FOREIGN KEY (`IdSistema`, `EmplazamientoDefecto`) REFERENCES `emplazamientos` (`IdSistema`, `IdEmplazamiento`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `destinosradio_ibfk_1` FOREIGN KEY (`IdSistema`, `IdDestino`, `TipoDestino`) REFERENCES `destinos` (`IdSistema`, `IdDestino`, `TipoDestino`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `fk_destinosradio_zonas1` FOREIGN KEY (`zonas_idZonas`) REFERENCES `zonas` (`idZonas`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB AUTO_INCREMENT=187 DEFAULT CHARSET=latin1 COMMENT='Destinos Radio';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `destinosradiosector`
--

DROP TABLE IF EXISTS `destinosradiosector`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `destinosradiosector` (
  `IdSistema` varchar(32) NOT NULL,
  `IdDestino` varchar(700) NOT NULL,
  `TipoDestino` int(10) unsigned NOT NULL,
  `IdNucleo` varchar(32) NOT NULL,
  `IdSector` varchar(700) NOT NULL,
  `PosHMI` int(10) unsigned NOT NULL,
  `Prioridad` int(1) unsigned DEFAULT NULL,
  `PrioridadSIP` int(1) unsigned DEFAULT NULL,
  `ModoOperacion` varchar(1) DEFAULT NULL,
  `Cascos` varchar(1) DEFAULT NULL,
  `Literal` varchar(32) DEFAULT NULL,
  `SupervisionPortadora` tinyint(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`IdSistema`,`IdDestino`,`TipoDestino`,`IdNucleo`,`IdSector`,`PosHMI`),
  KEY `DestinosRadioSector_FKIndex1` (`IdSistema`,`IdDestino`,`TipoDestino`),
  KEY `DestinosRadioSector_FKIndex2` (`IdSistema`,`IdNucleo`,`IdSector`),
  CONSTRAINT `destinosradiosector_ibfk_1` FOREIGN KEY (`IdSistema`, `IdDestino`, `TipoDestino`) REFERENCES `destinosradio` (`IdSistema`, `IdDestino`, `TipoDestino`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `destinosradiosector_ibfk_2` FOREIGN KEY (`IdSistema`, `IdNucleo`, `IdSector`) REFERENCES `sectores` (`IdSistema`, `IdNucleo`, `IdSector`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `destinostelefonia`
--

DROP TABLE IF EXISTS `destinostelefonia`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `destinostelefonia` (
  `IdSistema` varchar(32) NOT NULL,
  `IdDestino` varchar(700) NOT NULL,
  `TipoDestino` int(10) unsigned NOT NULL,
  `IdPrefijo` int(10) unsigned NOT NULL,
  `IdGrupo` varchar(32) DEFAULT NULL,
  PRIMARY KEY (`IdSistema`,`IdDestino`,`TipoDestino`,`IdPrefijo`),
  KEY `DestinosTelefonia_FKIndex1` (`IdSistema`,`IdDestino`,`TipoDestino`),
  KEY `DestinosTelefonia_FKIndex2` (`IdSistema`,`IdGrupo`),
  KEY `DestinosTelefonia_FKIndex3` (`IdSistema`,`IdPrefijo`),
  CONSTRAINT `destinostelefonia_ibfk_1` FOREIGN KEY (`IdSistema`, `IdDestino`, `TipoDestino`) REFERENCES `destinos` (`IdSistema`, `IdDestino`, `TipoDestino`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `destinostelefonia_ibfk_2` FOREIGN KEY (`IdSistema`, `IdGrupo`) REFERENCES `grupostelefonia` (`IdSistema`, `IdGrupo`) ON DELETE NO ACTION ON UPDATE CASCADE,
  CONSTRAINT `destinostelefonia_ibfk_3` FOREIGN KEY (`IdSistema`, `IdPrefijo`) REFERENCES `prefijos` (`IdSistema`, `IdPrefijo`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `emplazamientos`
--

DROP TABLE IF EXISTS `emplazamientos`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `emplazamientos` (
  `IdSistema` varchar(32) NOT NULL,
  `IdEmplazamiento` varchar(32) NOT NULL,
  PRIMARY KEY (`IdSistema`,`IdEmplazamiento`),
  KEY `Emplazamientos_FKIndex1` (`IdSistema`),
  CONSTRAINT `emplazamientos_ibfk_1` FOREIGN KEY (`IdSistema`) REFERENCES `sistema` (`IdSistema`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER `new_cd40`.`PRDELETE`
BEFORE DELETE ON `new_cd40`.`emplazamientos`
FOR EACH ROW
UPDATE RECURSOSRADIO SET IdEmplazamiento=NULL
			WHERE IdSistema = old.IdSistema AND IdEmplazamiento=old.IdEmplazamiento */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;

--
-- Table structure for table `encaminamientos`
--

DROP TABLE IF EXISTS `encaminamientos`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `encaminamientos` (
  `IdSistema` varchar(32) NOT NULL,
  `Central` varchar(32) NOT NULL,
  `CentralPropia` tinyint(1) DEFAULT '0',
  `Throwswitching` tinyint(1) DEFAULT '0',
  `NumTest` varchar(6) DEFAULT NULL,
  `CentralIp` tinyint(1) DEFAULT '0',
  PRIMARY KEY (`IdSistema`,`Central`),
  KEY `Encaminamientos_FKIndex1` (`IdSistema`),
  CONSTRAINT `encaminamientos_ibfk_1` FOREIGN KEY (`IdSistema`) REFERENCES `sistema` (`IdSistema`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `equiposeu`
--

DROP TABLE IF EXISTS `equiposeu`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `equiposeu` (
  `IdSistema` varchar(32) NOT NULL,
  `idEquipos` varchar(32) NOT NULL,
  `IpRed1` varchar(60) DEFAULT NULL COMMENT 'Si el equipo externo está asociado a una CentralIP (TipoEquipo=3 y Min=Max= -1), almacena la Dirección IP y puerto del primer proxy configurado. El valor almacenara la dirección en el formato XXX.XXX.XXX.XXX:YYYY donde YYYY es el puerto. Si es un equipo externo de otro tipo, almacena sólo la dirección IP 1  del equipo. El puerto se almacena en el campo SipPort',
  `IpRed2` varchar(60) DEFAULT NULL COMMENT 'Si el equipo externo está asociado a una CentralIP (TipoEquipo=3 y Min=Max= -1), almacena la Dirección IP y puerto del segundo proxy configurado. El valor almacenara la dirección en el formato XXX.XXX.XXX.XXX:YYYY donde YYYY es el puerto. Si es un equipo externo de otro tipo, almacena sólo la dirección IP 2  del equipo. El puerto se almacena en el campo SipPort',
  `TipoEquipo` int(10) unsigned DEFAULT NULL,
  `Interno` tinyint(1) DEFAULT '0',
  `Min` int(11) DEFAULT '0',
  `Max` int(11) DEFAULT '0',
  `SipPort` int(6) DEFAULT '5060',
  `IpRed3` varchar(60) DEFAULT NULL COMMENT 'Si el equipo externo está asociado a una CentralIP (TipoEquipo=3 y Min=Max= -1), almacena la Dirección IP y puerto del tercer proxy configurado. El valor almacenara la dirección en el formato XXX.XXX.XXX.XXX:YYYY donde YYYY es el puerto.Si es de otro tipo,  este campo tomará el valor null',
  `SrvPresenciaIpRed1` varchar(60) DEFAULT NULL COMMENT 'Si el equipo externo está asociado a una CentralIP (TipoEquipo=3 y Min=Max= -1), almacena la Dirección IP y puerto del primer servidor de presencia configurado. El valor almacenara la dirección en el formato XXX.XXX.XXX.XXX:YYYY donde YYYY es el puerto. Si es un equipo externo de otro tipo, tomará el valor null.',
  `SrvPresenciaIpRed2` varchar(60) DEFAULT NULL COMMENT 'Si el equipo externo está asociado a una CentralIP (TipoEquipo=3 y Min=Max= -1), almacena la Dirección IP y puerto del segundo servidor de presencia configurado. El valor almacenara la dirección en el formato XXX.XXX.XXX.XXX:YYYY donde YYYY es el puerto. Si es un equipo externo de otro tipo, tomará el valor null.',
  `SrvPresenciaIpRed3` varchar(60) DEFAULT NULL COMMENT 'Si el equipo externo está asociado a una CentralIP (TipoEquipo=3 y Min=Max= -1), almacena la Dirección IP y puerto del tercer servidor de presencia configurado. El valor almacenara la dirección en el formato XXX.XXX.XXX.XXX:YYYY donde YYYY es el puerto. Si es un equipo externo de otro tipo, tomará el valor null.',
  PRIMARY KEY (`IdSistema`,`idEquipos`),
  KEY `EquiposEU_FKIndex1` (`IdSistema`),
  CONSTRAINT `equiposeu_ibfk_1` FOREIGN KEY (`IdSistema`) REFERENCES `sistema` (`IdSistema`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `estadoaltavoces`
--

DROP TABLE IF EXISTS `estadoaltavoces`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `estadoaltavoces` (
  `IdDestino` varchar(700) NOT NULL,
  `PosHMI` int(10) unsigned NOT NULL,
  `IdSector` varchar(700) NOT NULL,
  `IdNucleo` varchar(32) NOT NULL,
  `IdSectorizacion` varchar(32) NOT NULL,
  `IdSistema` varchar(32) NOT NULL,
  `NumAltavoz` int(10) unsigned NOT NULL,
  `Estado` varchar(1) DEFAULT NULL,
  PRIMARY KEY (`IdDestino`,`PosHMI`,`IdSector`,`IdNucleo`,`IdSectorizacion`,`IdSistema`,`NumAltavoz`),
  KEY `Altavoz_FKIndex1` (`IdSistema`,`IdSectorizacion`,`IdNucleo`,`IdSector`,`PosHMI`,`IdDestino`),
  CONSTRAINT `estadoaltavoces_ibfk_1` FOREIGN KEY (`IdSistema`, `IdSectorizacion`, `IdNucleo`, `IdSector`, `PosHMI`, `IdDestino`) REFERENCES `radio` (`IdSistema`, `IdSectorizacion`, `IdNucleo`, `IdSector`, `PosHMI`, `IdDestino`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `estadocluster`
--

DROP TABLE IF EXISTS `estadocluster`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `estadocluster` (
  `Name` varchar(32) NOT NULL,
  `Presencia` tinyint(1) DEFAULT '0',
  `Estado` int(11) DEFAULT '0',
  PRIMARY KEY (`Name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `estadorecursos`
--

DROP TABLE IF EXISTS `estadorecursos`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `estadorecursos` (
  `IdDestino` varchar(32) NOT NULL,
  `PosHMI` int(10) unsigned NOT NULL,
  `IdSector` varchar(700) NOT NULL,
  `IdNucleo` varchar(32) NOT NULL,
  `IdSectorizacion` varchar(32) NOT NULL,
  `IdSistema` varchar(32) NOT NULL,
  `IdRecurso` varchar(32) NOT NULL,
  `Estado` varchar(1) DEFAULT NULL,
  PRIMARY KEY (`IdDestino`,`PosHMI`,`IdSector`,`IdNucleo`,`IdSectorizacion`,`IdSistema`,`IdRecurso`),
  KEY `Recurso_FKIndex1` (`IdSistema`,`IdSectorizacion`,`IdNucleo`,`IdSector`,`PosHMI`,`IdDestino`),
  CONSTRAINT `estadorecursos_ibfk_1` FOREIGN KEY (`IdSistema`, `IdSectorizacion`, `IdNucleo`, `IdSector`, `PosHMI`, `IdDestino`) REFERENCES `radio` (`IdSistema`, `IdSectorizacion`, `IdNucleo`, `IdSector`, `PosHMI`, `IdDestino`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `estadosrecursos`
--

DROP TABLE IF EXISTS `estadosrecursos`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `estadosrecursos` (
  `IdSistema` varchar(32) NOT NULL,
  `IdSector` varchar(700) NOT NULL,
  `IdNucleo` varchar(32) NOT NULL,
  `PosHMI` int(10) unsigned NOT NULL,
  `IdRecurso` varchar(32) NOT NULL,
  `TipoRecurso` int(1) unsigned NOT NULL,
  `IdDestino` varchar(700) NOT NULL,
  `TipoDestino` int(10) unsigned NOT NULL,
  `Estado` varchar(1) DEFAULT NULL,
  PRIMARY KEY (`IdSistema`,`IdSector`,`IdNucleo`,`PosHMI`,`IdRecurso`,`TipoRecurso`,`IdDestino`,`TipoDestino`),
  KEY `Table_44_FKIndex1` (`IdSistema`,`IdRecurso`,`TipoRecurso`),
  KEY `Table_44_FKIndex2` (`IdSistema`,`IdDestino`,`TipoDestino`,`IdNucleo`,`IdSector`,`PosHMI`),
  CONSTRAINT `estadosrecursos_ibfk_1` FOREIGN KEY (`IdSistema`, `IdRecurso`, `TipoRecurso`) REFERENCES `recursosradio` (`IdSistema`, `IdRecurso`, `TipoRecurso`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `estadosrecursos_ibfk_2` FOREIGN KEY (`IdSistema`, `IdDestino`, `TipoDestino`, `IdNucleo`, `IdSector`, `PosHMI`) REFERENCES `destinosradiosector` (`IdSistema`, `IdDestino`, `TipoDestino`, `IdNucleo`, `IdSector`, `PosHMI`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `externos`
--

DROP TABLE IF EXISTS `externos`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `externos` (
  `IdSistema` varchar(32) NOT NULL,
  `IdSectorizacion` varchar(32) NOT NULL,
  `IdNucleo` varchar(32) NOT NULL,
  `IdSector` varchar(700) NOT NULL,
  `IdDestino` varchar(700) NOT NULL,
  `IdPrefijo` int(10) unsigned NOT NULL,
  `TipoAcceso` varchar(2) NOT NULL,
  `PosHMI` int(10) unsigned DEFAULT NULL,
  `Prioridad` int(10) unsigned DEFAULT NULL,
  `OrigenR2` varchar(32) DEFAULT NULL,
  `PrioridadSIP` int(10) unsigned DEFAULT NULL,
  `Literal` varchar(32) DEFAULT NULL,
  `Grupo` varchar(80) DEFAULT '""',
  PRIMARY KEY (`IdSistema`,`IdSectorizacion`,`IdNucleo`,`IdSector`,`IdDestino`,`IdPrefijo`,`TipoAcceso`),
  KEY `Table_41_FKIndex1` (`IdSistema`,`IdSectorizacion`,`IdNucleo`,`IdSector`),
  CONSTRAINT `externos_ibfk_1` FOREIGN KEY (`IdSistema`, `IdSectorizacion`, `IdNucleo`, `IdSector`) REFERENCES `sectoressectorizacion` (`IdSistema`, `IdSectorizacion`, `IdNucleo`, `IdSector`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `funciones`
--

DROP TABLE IF EXISTS `funciones`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `funciones` (
  `IdSistema` varchar(32) NOT NULL,
  `Clave` int(11) NOT NULL,
  `Tipo` int(11) NOT NULL COMMENT '0: Radio; 1: Telefonía',
  `Funcion` varchar(32) NOT NULL,
  PRIMARY KEY (`IdSistema`,`Clave`),
  CONSTRAINT `FK_Sistema_Funciones` FOREIGN KEY (`IdSistema`) REFERENCES `sistema` (`IdSistema`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `grupostelefonia`
--

DROP TABLE IF EXISTS `grupostelefonia`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `grupostelefonia` (
  `IdSistema` varchar(32) NOT NULL,
  `IdGrupo` varchar(32) NOT NULL,
  PRIMARY KEY (`IdSistema`,`IdGrupo`),
  KEY `GruposTelefonia_FKIndex1` (`IdSistema`),
  CONSTRAINT `grupostelefonia_ibfk_1` FOREIGN KEY (`IdSistema`) REFERENCES `sistema` (`IdSistema`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER `new_cd40`.`DeleteGrupoTelefonia`
BEFORE DELETE ON `new_cd40`.`grupostelefonia`
FOR EACH ROW
BEGIN
  UPDATE DestinosTelefonia 
    SET IdGrupo=null 
    WHERE IdSistema=old.IdSistema AND IdGrupo=old.IdGrupo;
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;

--
-- Table structure for table `gwactivas`
--

DROP TABLE IF EXISTS `gwactivas`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `gwactivas` (
  `IdSistema` varchar(32) NOT NULL,
  `IdTifx` varchar(32) NOT NULL,
  `IpRed` varchar(60) NOT NULL,
  PRIMARY KEY (`IdSistema`,`IdTifx`),
  CONSTRAINT `FK_Tifx` FOREIGN KEY (`IdSistema`, `IdTifx`) REFERENCES `tifx` (`IdSistema`, `IdTIFX`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `hfparams`
--

DROP TABLE IF EXISTS `hfparams`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `hfparams` (
  `IdSistema` varchar(32) NOT NULL COMMENT 'Identificador del sistema',
  `IdRecurso` varchar(32) NOT NULL COMMENT 'Identificador del recurso radio',
  `IpGestor` varchar(15) DEFAULT NULL,
  `Oid` varchar(45) DEFAULT NULL,
  `Frecuencia` varchar(10) DEFAULT '',
  `TipoEquipo` int(11) DEFAULT '1' COMMENT '0: Receptor\n1: Transmisor\n2: Transceptor',
  `TipoCanal` int(11) DEFAULT '0' COMMENT '0: Monocanal\n1: Multicanal\n2: Otros',
  `TipoFrecuencia` int(11) DEFAULT '1' COMMENT '0: HF\n1: VHF\n2: UHF',
  `TipoModo` int(11) DEFAULT '0' COMMENT '0: Principal\n1: Reserva\n2: Ambos',
  `PrioridadEquipo` int(11) DEFAULT '0' COMMENT '0: Baja\n1: Normal\n2: Alta',
  `Puerto` int(10) DEFAULT '0',
  `Offset` int(10) DEFAULT '0',
  `Canalizacion` int(10) DEFAULT '0',
  `Modulacion` int(10) DEFAULT '0',
  `Potencia` int(10) DEFAULT '0',
  `FormatoFrecuenciaPrincipal` int(10) DEFAULT '0',
  `ModeloEquipo` int(11) DEFAULT '1000',
  PRIMARY KEY (`IdSistema`,`IdRecurso`),
  CONSTRAINT `hfparams_ibfk_1` FOREIGN KEY (`IdSistema`, `IdRecurso`) REFERENCES `recursosradio` (`IdSistema`, `IdRecurso`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COMMENT='Parámetros de recursos radio HF y M+N';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `hfrangofrecuencias`
--

DROP TABLE IF EXISTS `hfrangofrecuencias`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `hfrangofrecuencias` (
  `IdSistema` varchar(32) NOT NULL,
  `IdRecurso` varchar(32) NOT NULL,
  `Min` varchar(15) DEFAULT NULL,
  `Max` varchar(15) DEFAULT NULL,
  KEY `hfrangofrecuencias_ibfk_1` (`IdSistema`,`IdRecurso`),
  CONSTRAINT `hfrangofrecuencias_ibfk_1` FOREIGN KEY (`IdSistema`, `IdRecurso`) REFERENCES `hfparams` (`IdSistema`, `IdRecurso`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `historicoincidencias`
--

DROP TABLE IF EXISTS `historicoincidencias`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `historicoincidencias` (
  `IdSistema` varchar(32) NOT NULL,
  `Scv` int(1) NOT NULL DEFAULT '0',
  `IdHw` varchar(32) NOT NULL,
  `TipoHw` int(1) unsigned NOT NULL,
  `IdIncidencia` int(10) unsigned NOT NULL,
  `FechaHora` datetime NOT NULL,
  `Reconocida` datetime DEFAULT NULL,
  `Descripcion` varchar(200) DEFAULT NULL,
  `Usuario` varchar(32) DEFAULT NULL,
  KEY `HistoricoIncidencias_FKIndex1` (`IdIncidencia`),
  KEY `HistoricoIncidencias_FKIndex2` (`IdSistema`),
  KEY `HistoricoIncidenciasIndex` (`IdSistema`,`IdHw`,`TipoHw`,`IdIncidencia`,`FechaHora`),
  KEY `HistoricoIncidenciasFecha_Idx` (`FechaHora`),
  CONSTRAINT `historicoincidencias_ibfk_1` FOREIGN KEY (`IdIncidencia`) REFERENCES `incidencias` (`IdIncidencia`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `historicoincidencias_ibfk_2` FOREIGN KEY (`IdSistema`) REFERENCES `sistema` (`IdSistema`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `incidencias`
--

DROP TABLE IF EXISTS `incidencias`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `incidencias` (
  `IdIncidencia` int(10) unsigned NOT NULL,
  `IdIncidenciaCorrectora` int(10) unsigned DEFAULT NULL,
  `Incidencia` varchar(180) DEFAULT NULL,
  `Descripcion` varchar(180) DEFAULT NULL,
  `GeneraError` tinyint(1) DEFAULT NULL,
  `OID` varchar(32) DEFAULT NULL,
  `trep` int(4) NOT NULL DEFAULT '0' COMMENT 'Segundos de filtro de repeticion del historico con el mimso metadata. 0 Anula el filtro',
  PRIMARY KEY (`IdIncidencia`),
  KEY `Incidencias_FKIndex1` (`IdIncidenciaCorrectora`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `incidencias_frances`
--

DROP TABLE IF EXISTS `incidencias_frances`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `incidencias_frances` (
  `IdIncidencia` int(10) unsigned NOT NULL,
  `IdIncidenciaCorrectora` int(10) unsigned DEFAULT NULL,
  `Incidencia` varchar(180) DEFAULT NULL,
  `Descripcion` varchar(180) DEFAULT NULL,
  `GeneraError` tinyint(1) DEFAULT NULL,
  `OID` varchar(32) DEFAULT NULL,
  `trep` int(4) NOT NULL DEFAULT '0' COMMENT 'Tiempo en segundos del filtro personalizado de repeticiones',
  PRIMARY KEY (`IdIncidencia`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `incidencias_ingles`
--

DROP TABLE IF EXISTS `incidencias_ingles`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `incidencias_ingles` (
  `IdIncidencia` int(10) unsigned NOT NULL,
  `IdIncidenciaCorrectora` int(10) unsigned DEFAULT NULL,
  `Incidencia` varchar(180) DEFAULT NULL,
  `Descripcion` varchar(180) DEFAULT NULL,
  `GeneraError` tinyint(1) DEFAULT NULL,
  `OID` varchar(32) DEFAULT NULL,
  `trep` int(4) NOT NULL DEFAULT '0' COMMENT 'Tiempo en segundos para el filtro de repeticiones personalizado',
  PRIMARY KEY (`IdIncidencia`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `internos`
--

DROP TABLE IF EXISTS `internos`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `internos` (
  `IdSistema` varchar(32) NOT NULL,
  `IdSectorizacion` varchar(32) NOT NULL,
  `IdNucleo` varchar(32) NOT NULL,
  `IdSector` varchar(700) NOT NULL,
  `IdDestino` varchar(700) NOT NULL,
  `IdPrefijo` int(10) unsigned NOT NULL,
  `TipoAcceso` varchar(2) NOT NULL,
  `PosHMI` int(10) unsigned DEFAULT NULL,
  `Prioridad` int(10) unsigned DEFAULT NULL,
  `OrigenR2` varchar(32) DEFAULT NULL,
  `PrioridadSIP` int(10) unsigned DEFAULT NULL,
  `Literal` varchar(32) DEFAULT NULL,
  PRIMARY KEY (`IdSistema`,`IdSectorizacion`,`IdNucleo`,`IdDestino`,`IdPrefijo`,`TipoAcceso`,`IdSector`),
  KEY `Table_40_FKIndex1` (`IdSistema`,`IdSectorizacion`,`IdNucleo`,`IdSector`),
  CONSTRAINT `internos_ibfk_1` FOREIGN KEY (`IdSistema`, `IdSectorizacion`, `IdNucleo`, `IdSector`) REFERENCES `sectoressectorizacion` (`IdSistema`, `IdSectorizacion`, `IdNucleo`, `IdSector`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `llamadas_entrantes_sector`
--

DROP TABLE IF EXISTS `llamadas_entrantes_sector`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `llamadas_entrantes_sector` (
  `IdSistema` varchar(32) NOT NULL COMMENT 'Identificador del sistema al que pertenece el sector',
  `IdNucleo` varchar(32) NOT NULL COMMENT 'Identificador del núcleo al que pertenece el sector',
  `IdSector` varchar(700) NOT NULL COMMENT 'Identificador del sector',
  `IdRed` varchar(32) NOT NULL COMMENT 'Identificador único de la red',
  `Tipo` enum('A','R','T') NOT NULL COMMENT 'Tipo: A (Abonado), R (Rango), T (Todas)',
  `NumAbonadoInicial` varchar(45) NOT NULL COMMENT 'Si tipo es A, almacena el número del abonado al que se le permite realizar llamadas.\nSi  tipo es R, almacena el número de abonado inicial del rango al que se le permite realizar llamadas.',
  `NumAbonadoFinal` varchar(45) DEFAULT NULL COMMENT 'Si  tipo es R, almacena el número de abonado final del rango al que se le permite realizar llamadas.\nSi tipo es A, estará vacío',
  `FechaGbd` datetime DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP COMMENT 'Fecha de inserción o modificación del registro en la BD',
  PRIMARY KEY (`IdSistema`,`IdNucleo`,`IdSector`,`IdRed`,`Tipo`,`NumAbonadoInicial`),
  UNIQUE KEY `Llam_Ent_Sector_Rango_idx` (`IdSistema`,`IdNucleo`,`IdSector`,`IdRed`,`Tipo`,`NumAbonadoInicial`,`NumAbonadoFinal`),
  KEY `Llam_Ent_Sector_Red_FK_idx` (`IdSistema`,`IdRed`),
  CONSTRAINT `Llam_Ent_Sector_Red_FK` FOREIGN KEY (`IdSistema`, `IdRed`) REFERENCES `new_cd40_trans`.`redes` (`IdSistema`, `IdRed`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `Llam_Ent_Sector_Sectores_FK` FOREIGN KEY (`IdSistema`, `IdNucleo`, `IdSector`) REFERENCES `new_cd40_trans`.`sectores` (`IdSistema`, `IdNucleo`, `IdSector`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COMMENT='Lista blanca de abonados o rango de abonados de cada red con prefijo >= 4 a los que un sector podrá efectuar llamadas ';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `llamadas_salientes_sector`
--

DROP TABLE IF EXISTS `llamadas_salientes_sector`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `llamadas_salientes_sector` (
  `IdSistema` varchar(32) NOT NULL COMMENT 'Identificador del sistema al que pertenece el sector',
  `IdNucleo` varchar(32) NOT NULL COMMENT 'Identificador del núcleo al que pertenece el sector',
  `IdSector` varchar(700) NOT NULL COMMENT 'Identificador del sector',
  `IdRed` varchar(32) NOT NULL COMMENT 'Identificador único de la red',
  `Tipo` enum('A','R','T') NOT NULL COMMENT 'Tipo: A (Abonado), R (Rango), T (Todas)',
  `NumAbonadoInicial` varchar(45) NOT NULL COMMENT 'Si tipo es A, almacena el número del abonado al que se le permite realizar llamadas.\nSi  tipo es R, almacena el número de abonado inicial del rango al que se le permite realizar llamadas.',
  `NumAbonadoFinal` varchar(45) DEFAULT NULL COMMENT 'Si  tipo es R, almacena el número de abonado final del rango al que se le permite realizar llamadas.\nSi tipo es A, estará vacío',
  `FechaGbd` datetime DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP COMMENT 'Fecha de inserción o modificación del registro en la BD',
  PRIMARY KEY (`IdSistema`,`IdNucleo`,`IdSector`,`IdRed`,`Tipo`,`NumAbonadoInicial`),
  UNIQUE KEY `Llam_Sal_Sector_Rango_idx` (`IdSistema`,`IdNucleo`,`IdSector`,`IdRed`,`Tipo`,`NumAbonadoInicial`,`NumAbonadoFinal`),
  KEY `Llam_Sal_Sector_Red_FK_idx` (`IdSistema`,`IdRed`),
  CONSTRAINT `Llam_Sal_Sector_Red_FK` FOREIGN KEY (`IdSistema`, `IdRed`) REFERENCES `new_cd40_trans`.`redes` (`IdSistema`, `IdRed`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `Llam_Sal_Sector_Sectores_FK` FOREIGN KEY (`IdSistema`, `IdNucleo`, `IdSector`) REFERENCES `new_cd40_trans`.`sectores` (`IdSistema`, `IdNucleo`, `IdSector`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COMMENT='Lista blanca de abonados o rango de abonados de cada red con prefijo >= 4 a los que un sector podrá efectuar llamadas ';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `metodos_bss`
--

DROP TABLE IF EXISTS `metodos_bss`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `metodos_bss` (
  `idmetodos_bss` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`idmetodos_bss`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `metodosbss_destinosradio`
--

DROP TABLE IF EXISTS `metodosbss_destinosradio`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `metodosbss_destinosradio` (
  `idmetodosbss_destinosradio` int(11) NOT NULL AUTO_INCREMENT,
  `metodos_bss_idmetodos_bss` int(11) NOT NULL,
  `metodosbss_destinosradio_fk` int(11) NOT NULL,
  PRIMARY KEY (`idmetodosbss_destinosradio`,`metodos_bss_idmetodos_bss`,`metodosbss_destinosradio_fk`),
  KEY `fk_metodosbss_destinosradio_metodos_bss1_idx` (`metodos_bss_idmetodos_bss`),
  KEY `fk_metodosbss_destinosradio_fk_idx` (`metodosbss_destinosradio_fk`),
  CONSTRAINT `fk_metodosbss_destinosradio_fk` FOREIGN KEY (`metodosbss_destinosradio_fk`) REFERENCES `destinosradio` (`fk_metodosbss`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `fk_metodosbss_destinosradio_metodos_bss1` FOREIGN KEY (`metodos_bss_idmetodos_bss`) REFERENCES `metodos_bss` (`idmetodos_bss`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `multidestinos`
--

DROP TABLE IF EXISTS `multidestinos`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `multidestinos` (
  `IdKey` int(11) NOT NULL AUTO_INCREMENT COMMENT 'Clave Primaria',
  `IdSistema` varchar(32) DEFAULT NULL COMMENT 'Id del Sistema con el que estamos trabajando.',
  `IdNucleo` varchar(32) DEFAULT NULL COMMENT 'Id del Núcleo en la que se encuentra el elemento del Multi Destino.',
  `IdSector` varchar(700) DEFAULT NULL COMMENT 'Id del Sector en el que se encuentra configurado el elemento del Multi Destino.',
  `IdMultiDestino` varchar(32) NOT NULL COMMENT 'Identificador para representar al Multi Destino.',
  `Dominio` varchar(32) DEFAULT 'PROPIO' COMMENT 'Representa si es propio (es conocido por el SCV) o ajeno (trabaja fuera del SCV).',
  `IdSectorizacion` varchar(32) DEFAULT NULL COMMENT 'Id de la Sectorización en la que se encuentra el elemento del  Multi Destino.',
  PRIMARY KEY (`IdKey`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COMMENT='Tabla para gestionar los Multi Destinos (También llamados Grupos.)';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `multidestinos_elementos`
--

DROP TABLE IF EXISTS `multidestinos_elementos`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `multidestinos_elementos` (
  `IdKey` int(11) NOT NULL AUTO_INCREMENT COMMENT 'Clave Primaria.',
  `IdMultiDestino` int(11) NOT NULL COMMENT 'Clave ajena a multidestino.',
  `IdDestino` varchar(700) NOT NULL COMMENT 'Identificador del destino para el Multi Destino.',
  `IdPrefijo` int(10) DEFAULT NULL COMMENT 'Identifica el tipo de Destino que se está añadiendo: 1=LC - 3=ATS - 4=RTB - 5=RTB2 - 7=redMXONE - 0=Interno.',
  PRIMARY KEY (`IdKey`),
  KEY `FK_multi_destinos_idx` (`IdMultiDestino`),
  CONSTRAINT `FK_multi_destinos` FOREIGN KEY (`IdMultiDestino`) REFERENCES `multidestinos` (`IdKey`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COMMENT='Tabla con los elementos (destinos internos o externos) que componen el grupo Multi destino.';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `niveles`
--

DROP TABLE IF EXISTS `niveles`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `niveles` (
  `IdSistema` varchar(32) NOT NULL,
  `IdNucleo` varchar(32) NOT NULL,
  `IdSector` varchar(700) NOT NULL,
  `CICL` int(10) unsigned DEFAULT '0',
  `CIPL` int(10) unsigned DEFAULT '0',
  `CPICL` int(10) unsigned DEFAULT '0',
  `CPIPL` int(10) unsigned DEFAULT '0',
  PRIMARY KEY (`IdSistema`,`IdNucleo`,`IdSector`),
  KEY `Niveles_FKIndex1` (`IdSistema`,`IdNucleo`,`IdSector`),
  CONSTRAINT `niveles_ibfk_1` FOREIGN KEY (`IdSistema`, `IdNucleo`, `IdSector`) REFERENCES `sectores` (`IdSistema`, `IdNucleo`, `IdSector`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `nucleos`
--

DROP TABLE IF EXISTS `nucleos`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `nucleos` (
  `IdSistema` varchar(32) NOT NULL,
  `IdNucleo` varchar(32) NOT NULL,
  PRIMARY KEY (`IdSistema`,`IdNucleo`),
  KEY `Nucleos_FKIndex1` (`IdSistema`),
  CONSTRAINT `nucleos_ibfk_1` FOREIGN KEY (`IdSistema`) REFERENCES `sistema` (`IdSistema`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `operadores`
--

DROP TABLE IF EXISTS `operadores`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `operadores` (
  `IdOperador` varchar(32) NOT NULL,
  `IdSistema` varchar(32) NOT NULL,
  `Clave` varchar(10) DEFAULT NULL,
  `NivelAcceso` int(1) unsigned DEFAULT NULL,
  `Nombre` varchar(32) DEFAULT NULL,
  `Apellidos` varchar(32) DEFAULT NULL,
  `Telefono` varchar(32) DEFAULT NULL,
  `FechaUltAcceso` date DEFAULT NULL,
  `Comentarios` varchar(100) DEFAULT NULL,
  PRIMARY KEY (`IdOperador`,`IdSistema`),
  KEY `Operadores_FKIndex1` (`IdSistema`),
  CONSTRAINT `operadores_ibfk_1` FOREIGN KEY (`IdSistema`) REFERENCES `sistema` (`IdSistema`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `parametrosrecurso`
--

DROP TABLE IF EXISTS `parametrosrecurso`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `parametrosrecurso` (
  `IdSistema` varchar(32) NOT NULL,
  `IdRecurso` varchar(32) NOT NULL,
  `TipoRecurso` int(1) unsigned NOT NULL,
  `GananciaAGCTX` int(1) unsigned DEFAULT '0',
  `GananciaAGCTXdBm` float DEFAULT '0',
  `GananciaAGCRX` int(1) unsigned DEFAULT '0',
  `GananciaAGCRXdBm` float DEFAULT '0',
  `SupresionSilencio` tinyint(1) DEFAULT '1',
  `TamRTP` int(10) unsigned DEFAULT '20',
  `Codec` int(1) unsigned DEFAULT NULL,
  `TipoEM` int(1) DEFAULT '0',
  `GrabacionEd137` tinyint(1) DEFAULT '1',
  `TReleaseBL` int(1) DEFAULT '3' COMMENT 'Tiempo (sg) teniendo información acústica (tonos) antes de pasar el interfaz a reposo. 0<valor<=10 . Aplica en líneas BL.',
  `iDetCallerId` tinyint(1) DEFAULT '0' COMMENT 'Habilita la detección de Caller-Id en llamadas entrantes. 0: Lineas FX0 con detección callerID deshabilitada 1: Deteccion callerId activada. Aplica a líneas de tipo AB.\n',
  `iTmCallerId` int(11) DEFAULT '3000' COMMENT 'Tiempo para detectar CallerId en unidades de 100 ms. Valor: 0< valor <= 50. Aplica a líneas de tipo AB.',
  `iDetInversionPol` tinyint(1) DEFAULT '0' COMMENT 'Tiene en cuenta la inversión de polaridad en el comportamiento de la línea de AB. 0: no tiene encuenta la inversion polaridad 1: tiene en cuenta la inversión de polaridad. Aplica a líneas de tipo AB.\n',
  `iTmLlamEntrante` int(1) DEFAULT '30' COMMENT 'Tiempo maximo llamada entrante en lineas tlf (sg). 0<valor<=60. Aplica a líneas de tipo AB, BL y BC.',
  `iTmDetFinLlamada` int(1) DEFAULT '6' COMMENT 'Tiempo para detectar fin de llamada desde que llega ringoff para lineas de AB (sg). 0<valor<=8. Aplica a líneas AB.',
  `iPeriodoSpvRing` int(2) DEFAULT '200' COMMENT 'Periodo para supervisar señal ring en llamadas entrantes (en ms). 50<=valor<=400. Aplica a líneas AB y BL.',
  `iFiltroSpvRing` int(1) DEFAULT '2' COMMENT 'número de veces que hay que supervisar para dar por válido un valor. 0<valor<=6. Aplica a líneas AB y BL.',
  `iDetDtmf` tinyint(1) DEFAULT '0' COMMENT 'Deteccion dtmf para líneas BC. 0: no detecta 1: detecta. Aplica a líneas BC.\n',
  PRIMARY KEY (`IdSistema`,`IdRecurso`,`TipoRecurso`),
  KEY `ParametrosRecurso_FKIndex1` (`IdSistema`,`IdRecurso`,`TipoRecurso`),
  CONSTRAINT `parametrosrecurso_ibfk_1` FOREIGN KEY (`IdSistema`, `IdRecurso`, `TipoRecurso`) REFERENCES `recursos` (`IdSistema`, `IdRecurso`, `TipoRecurso`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `parametrossector`
--

DROP TABLE IF EXISTS `parametrossector`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `parametrossector` (
  `IdSistema` varchar(32) NOT NULL,
  `IdNucleo` varchar(32) NOT NULL,
  `IdSector` varchar(700) NOT NULL,
  `NumLlamadasEntrantesIDA` int(10) unsigned DEFAULT '5',
  `NumLlamadasEnIDA` int(10) unsigned DEFAULT '10',
  `NumFreqPagina` int(10) unsigned DEFAULT NULL,
  `NumPagFreq` int(10) unsigned DEFAULT NULL,
  `NumDestinosInternosPag` int(10) unsigned DEFAULT NULL,
  `NumPagDestinosInt` int(10) unsigned DEFAULT NULL,
  `Intrusion` tinyint(1) DEFAULT '1',
  `Intruido` tinyint(1) DEFAULT '1',
  `KeepAlivePeriod` int(10) unsigned DEFAULT '200',
  `KeepAliveMultiplier` int(10) unsigned DEFAULT '10',
  `NumEnlacesAI` int(10) unsigned DEFAULT '18',
  `GrabacionEd137` tinyint(1) DEFAULT '1',
  PRIMARY KEY (`IdSistema`,`IdNucleo`,`IdSector`),
  KEY `ParametrosSector_FKIndex1` (`IdSistema`,`IdNucleo`,`IdSector`),
  CONSTRAINT `parametrossector_ibfk_1` FOREIGN KEY (`IdSistema`, `IdNucleo`, `IdSector`) REFERENCES `sectores` (`IdSistema`, `IdNucleo`, `IdSector`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `permisosopedepats`
--

DROP TABLE IF EXISTS `permisosopedepats`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `permisosopedepats` (
  `IdOperador` varchar(32) NOT NULL,
  `IdSistema` varchar(32) NOT NULL,
  `IdDependenciaATS` varchar(32) NOT NULL,
  PRIMARY KEY (`IdOperador`,`IdDependenciaATS`,`IdSistema`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `permisosredes`
--

DROP TABLE IF EXISTS `permisosredes`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `permisosredes` (
  `IdSistema` varchar(32) NOT NULL COMMENT 'Identificador del sistema',
  `IdRed` varchar(32) NOT NULL COMMENT 'Identificador de la red',
  `IdSector` varchar(700) NOT NULL COMMENT 'Identificador del sector',
  `IdNucleo` varchar(32) NOT NULL COMMENT 'Identificador del núcleo al que pertenece el sector',
  `Llamar` enum('true','false') DEFAULT NULL COMMENT 'Indicador de si el sector puede realizar llamadas por esa red',
  `Recibir` enum('true','false') DEFAULT NULL COMMENT 'Indicador de si  el sector puede recibir llamadas',
  `IdRecursoPref` varchar(32) DEFAULT NULL COMMENT 'Identificador del recurso de red predeterminado para encaminar las llamadas salientes',
  `TipoRecursoPref` int(1) unsigned DEFAULT NULL COMMENT 'Tipo del recurso predeterminado para encaminar las llamadas salientes',
  PRIMARY KEY (`IdSistema`,`IdRed`,`IdSector`,`IdNucleo`),
  KEY `PermisosRedes_FKIndex1` (`IdSistema`,`IdRed`),
  KEY `PermisosRedes_FKIndex2` (`IdSistema`,`IdNucleo`,`IdSector`),
  KEY `permisosRedes_recurso_pref_idx` (`IdSistema`,`IdRecursoPref`,`TipoRecursoPref`),
  CONSTRAINT `permisosredes_ibfk_1` FOREIGN KEY (`IdSistema`, `IdRed`) REFERENCES `redes` (`IdSistema`, `IdRed`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `permisosredes_ibfk_2` FOREIGN KEY (`IdSistema`, `IdNucleo`, `IdSector`) REFERENCES `sectores` (`IdSistema`, `IdNucleo`, `IdSector`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `permisosRedes_recurso_pref_fk` FOREIGN KEY (`IdSistema`, `IdRecursoPref`, `TipoRecursoPref`) REFERENCES `recursostf` (`IdSistema`, `IdRecurso`, `TipoRecurso`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COMMENT='Configura los permisos de cada sector en cada red';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `prefijos`
--

DROP TABLE IF EXISTS `prefijos`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `prefijos` (
  `IdSistema` varchar(32) NOT NULL,
  `IdPrefijo` int(10) unsigned NOT NULL,
  PRIMARY KEY (`IdSistema`,`IdPrefijo`),
  KEY `Prefijos_FKIndex1` (`IdSistema`),
  CONSTRAINT `fk_prefijos_sistema` FOREIGN KEY (`IdSistema`) REFERENCES `sistema` (`IdSistema`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `radio`
--

DROP TABLE IF EXISTS `radio`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `radio` (
  `IdSistema` varchar(32) NOT NULL,
  `IdSectorizacion` varchar(32) NOT NULL,
  `IdNucleo` varchar(32) NOT NULL,
  `IdSector` varchar(700) NOT NULL,
  `PosHMI` int(10) unsigned NOT NULL,
  `IdDestino` varchar(700) NOT NULL,
  `Prioridad` int(1) unsigned DEFAULT NULL,
  `PrioridadSIP` int(1) unsigned DEFAULT NULL,
  `ModoOperacion` varchar(1) DEFAULT NULL,
  `Cascos` varchar(1) DEFAULT NULL,
  `Literal` varchar(32) DEFAULT NULL,
  `SupervisionPortadora` tinyint(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`IdSistema`,`IdSectorizacion`,`IdNucleo`,`IdSector`,`PosHMI`,`IdDestino`),
  KEY `Table_42_FKIndex1` (`IdSistema`,`IdSectorizacion`,`IdNucleo`,`IdSector`),
  CONSTRAINT `radio_ibfk_1` FOREIGN KEY (`IdSistema`, `IdSectorizacion`, `IdNucleo`, `IdSector`) REFERENCES `sectoressectorizacion` (`IdSistema`, `IdSectorizacion`, `IdNucleo`, `IdSector`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `radio_param`
--

DROP TABLE IF EXISTS `radio_param`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `radio_param` (
  `idradio_param` int(11) NOT NULL AUTO_INCREMENT,
  `GrsDelay` int(11) DEFAULT '0' COMMENT '0..200 msg\n',
  `OffSetFrequency` int(11) DEFAULT NULL,
  `EnableEventPttSq` tinyint(1) DEFAULT '0',
  `metodos_bss_idmetodos_bss` int(11) NOT NULL,
  PRIMARY KEY (`idradio_param`),
  KEY `fk_radio_param_metodos_bss1_idx` (`metodos_bss_idmetodos_bss`),
  CONSTRAINT `fk_radio_param_metodos_bss1` FOREIGN KEY (`metodos_bss_idmetodos_bss`) REFERENCES `metodos_bss` (`idmetodos_bss`) ON DELETE NO ACTION ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=295 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `rangos`
--

DROP TABLE IF EXISTS `rangos`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `rangos` (
  `IdSistema` varchar(32) NOT NULL,
  `Central` varchar(32) NOT NULL,
  `Tipo` varchar(1) NOT NULL,
  `Inicial` varchar(6) NOT NULL,
  `IdPrefijo` int(10) unsigned NOT NULL,
  `IdAbonado` varchar(32) NOT NULL,
  `Final` varchar(6) DEFAULT NULL,
  PRIMARY KEY (`IdSistema`,`Tipo`,`Central`,`Inicial`),
  KEY `Rangos_FKIndex2` (`IdSistema`,`IdPrefijo`),
  KEY `Rangos_FKIndex4` (`IdSistema`,`Central`),
  CONSTRAINT `FK_Encaminamientos` FOREIGN KEY (`IdSistema`, `Central`) REFERENCES `encaminamientos` (`IdSistema`, `Central`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `recursos`
--

DROP TABLE IF EXISTS `recursos`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `recursos` (
  `IdSistema` varchar(32) NOT NULL,
  `IdRecurso` varchar(32) NOT NULL,
  `TipoRecurso` int(1) unsigned NOT NULL DEFAULT '1',
  `idEquipos` varchar(32) DEFAULT NULL,
  `IdTIFX` varchar(32) DEFAULT NULL,
  `Tipo` int(1) unsigned NOT NULL DEFAULT '0',
  `Interface` int(2) unsigned DEFAULT '0',
  `SlotPasarela` int(1) DEFAULT '-1',
  `NumDispositivoSlot` int(1) DEFAULT '-1',
  `ServidorSIP` varchar(32) DEFAULT '0.0.0.0:5060',
  `Diffserv` tinyint(1) DEFAULT '0',
  PRIMARY KEY (`IdSistema`,`IdRecurso`,`TipoRecurso`),
  KEY `RECURSOS_FKIndex1` (`IdSistema`),
  KEY `RECURSOS_FKIndex2` (`IdSistema`,`IdTIFX`),
  KEY `RECURSOS_FKIndex3` (`IdSistema`,`idEquipos`),
  CONSTRAINT `recursos_ibfk_1` FOREIGN KEY (`IdSistema`) REFERENCES `sistema` (`IdSistema`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `recursos_ibfk_2` FOREIGN KEY (`IdSistema`, `IdTIFX`) REFERENCES `tifx` (`IdSistema`, `IdTIFX`) ON DELETE NO ACTION ON UPDATE CASCADE,
  CONSTRAINT `recursos_ibfk_3` FOREIGN KEY (`IdSistema`, `idEquipos`) REFERENCES `equiposeu` (`IdSistema`, `idEquipos`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `recursoslcen`
--

DROP TABLE IF EXISTS `recursoslcen`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `recursoslcen` (
  `IdSistema` varchar(32) NOT NULL,
  `IdRecurso` varchar(32) NOT NULL,
  `TipoRecurso` int(1) unsigned NOT NULL,
  `IdPrefijo` int(10) unsigned DEFAULT NULL,
  `IdDestino` varchar(700) DEFAULT NULL,
  `TipoDestino` int(10) unsigned DEFAULT NULL,
  `T1` int(10) unsigned DEFAULT '20',
  `T1Max` int(10) unsigned DEFAULT '100',
  `T2` int(10) unsigned DEFAULT '20',
  `T2Max` int(10) unsigned DEFAULT '100',
  `T3` int(10) unsigned DEFAULT '40',
  `T4` int(10) unsigned DEFAULT '40',
  `T4Max` int(10) unsigned DEFAULT '160',
  `T5` int(10) unsigned DEFAULT '60',
  `T5Max` int(10) unsigned DEFAULT '100',
  `T6` int(10) unsigned DEFAULT '5000',
  `T6Max` int(10) unsigned DEFAULT '6000',
  `T8` int(10) unsigned DEFAULT '80',
  `T8Max` int(10) unsigned DEFAULT '150',
  `T9` int(10) unsigned DEFAULT '40',
  `T9Max` int(10) unsigned DEFAULT '60',
  `T10` int(10) unsigned DEFAULT '20',
  `T10Max` int(10) unsigned DEFAULT '130',
  `T11` int(10) unsigned DEFAULT '20',
  `T11Max` int(10) unsigned DEFAULT '130',
  `T12` int(10) unsigned DEFAULT '200',
  `FrqTonoSQ` int(10) unsigned DEFAULT '2280',
  `UmbralTonoSQ` int(11) DEFAULT '-35',
  `FrqTonoPTT` int(10) unsigned DEFAULT '2280',
  `UmbralTonoPTT` int(11) DEFAULT '-10',
  `RefrescoEstados` int(10) unsigned DEFAULT '200',
  `Timeout` int(10) unsigned DEFAULT '600',
  `LongRafagas` int(10) unsigned DEFAULT '6',
  `isuperv_options` tinyint(1) DEFAULT '0' COMMENT 'Supervisa colateral IP. Valor: 0: no supervisa; 1: supervisa. Aplica en líneas LC.',
  `itm_superv_options` int(1) unsigned DEFAULT '10' COMMENT 'Si SupervisionOptions=1, tiempo entre options cuando el colateral está presente (en sg). Valor: 0<valor<20. Aplica en líneas LC.\n',
  PRIMARY KEY (`IdSistema`,`IdRecurso`,`TipoRecurso`),
  KEY `RecursosLCEN_FKIndex1` (`IdSistema`,`IdRecurso`,`TipoRecurso`),
  KEY `RecursosLCEN_FKIndex2` (`IdSistema`,`IdDestino`,`TipoDestino`,`IdPrefijo`),
  CONSTRAINT `recursoslcen_ibfk_1` FOREIGN KEY (`IdSistema`, `IdRecurso`, `TipoRecurso`) REFERENCES `recursos` (`IdSistema`, `IdRecurso`, `TipoRecurso`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `recursosradio`
--

DROP TABLE IF EXISTS `recursosradio`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `recursosradio` (
  `IdSistema` varchar(32) NOT NULL,
  `IdRecurso` varchar(32) NOT NULL,
  `TipoRecurso` int(1) unsigned NOT NULL,
  `zonas_idZonas` int(11) NOT NULL,
  `radio_param_idradio_param` int(11) NOT NULL,
  `tabla_bss_idtabla_bss` int(11) DEFAULT NULL,
  `TipoDestino` int(10) unsigned DEFAULT NULL,
  `IdDestino` varchar(700) DEFAULT NULL,
  `IdEmplazamiento` varchar(32) NOT NULL,
  `EM` tinyint(1) DEFAULT NULL,
  `SQ` varchar(1) DEFAULT NULL,
  `PTT` varchar(1) DEFAULT NULL,
  `FrqTonoE` int(10) unsigned DEFAULT NULL,
  `UmbralTonoE` int(11) DEFAULT NULL,
  `FrqTonoM` int(10) unsigned DEFAULT NULL,
  `UmbralTonoM` int(11) DEFAULT NULL,
  `FrqTonoSQ` int(10) unsigned DEFAULT NULL,
  `UmbralTonoSQ` int(11) DEFAULT NULL,
  `FrqTonoPTT` int(10) unsigned DEFAULT NULL,
  `UmbralTonoPTT` int(11) DEFAULT NULL,
  `BSS` tinyint(1) DEFAULT NULL,
  `NTZ` tinyint(1) DEFAULT NULL,
  `TipoNTZ` int(1) unsigned DEFAULT '0',
  `Cifrado` tinyint(1) DEFAULT NULL,
  `SupervPortadoraTX` tinyint(1) DEFAULT NULL,
  `SupervModuladoraTX` tinyint(1) DEFAULT NULL,
  `ModoConfPTT` int(1) unsigned DEFAULT NULL,
  `RepSQyBSS` int(10) unsigned DEFAULT '1',
  `DesactivacionSQ` int(10) unsigned DEFAULT '1',
  `TimeoutPTT` int(10) unsigned DEFAULT '200',
  `UmbralVAD` int(11) DEFAULT '-33',
  `TiempoPTT` int(10) unsigned DEFAULT '120',
  `NumFlujosAudio` int(1) unsigned DEFAULT '1',
  `KeepAlivePeriod` int(10) unsigned DEFAULT '200',
  `KeepAliveMultiplier` int(10) unsigned DEFAULT '10',
  `RedundanciaRol` enum('P','R') DEFAULT NULL COMMENT 'Rol del recurso si está asociado a un destino Radio con redundancia (1+1): P (Principal), R (Reserva). En otro caso, tomará el valor null.',
  `RedundanciaIdPareja` varchar(70) DEFAULT NULL COMMENT '''Identificador único del grupo de redundancia. Se compone como IdRecursoPrincipal#idRecursoRerva''',
  PRIMARY KEY (`IdSistema`,`IdRecurso`,`TipoRecurso`),
  KEY `RecursosRadio_FKIndex1` (`IdSistema`,`IdRecurso`,`TipoRecurso`),
  KEY `RecursosRadio_FKIndex2` (`IdSistema`,`IdEmplazamiento`),
  KEY `RecursosRadio_FKIndex3` (`IdSistema`,`IdDestino`,`TipoDestino`),
  KEY `fk_recursosradio_zonas1_idx` (`zonas_idZonas`),
  KEY `fk_recursosradio_radio_param1_idx` (`radio_param_idradio_param`),
  KEY `fk_recursosradio_tabla_bss1_idx` (`tabla_bss_idtabla_bss`),
  CONSTRAINT `fk_recursosradio_radio_param1` FOREIGN KEY (`radio_param_idradio_param`) REFERENCES `radio_param` (`idradio_param`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `fk_recursosradio_tabla_bss1` FOREIGN KEY (`tabla_bss_idtabla_bss`) REFERENCES `tabla_bss` (`idtabla_bss`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `fk_recursosradio_zonas1` FOREIGN KEY (`zonas_idZonas`) REFERENCES `zonas` (`idZonas`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `recursosradio_ibfk_1` FOREIGN KEY (`IdSistema`, `IdRecurso`, `TipoRecurso`) REFERENCES `recursos` (`IdSistema`, `IdRecurso`, `TipoRecurso`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `recursosradio_ibfk_2` FOREIGN KEY (`IdSistema`, `IdEmplazamiento`) REFERENCES `emplazamientos` (`IdSistema`, `IdEmplazamiento`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `recursosradio_ibfk_3` FOREIGN KEY (`IdSistema`, `IdDestino`, `TipoDestino`) REFERENCES `destinosradio` (`IdSistema`, `IdDestino`, `TipoDestino`) ON DELETE NO ACTION ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `recursostf`
--

DROP TABLE IF EXISTS `recursostf`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `recursostf` (
  `IdSistema` varchar(32) NOT NULL,
  `IdRecurso` varchar(32) NOT NULL,
  `TipoRecurso` int(1) unsigned NOT NULL,
  `IdPrefijo` int(10) unsigned DEFAULT NULL,
  `TipoDestino` int(10) unsigned DEFAULT NULL,
  `IdDestino` varchar(700) DEFAULT NULL,
  `IdTroncal` varchar(32) DEFAULT NULL,
  `IdRed` varchar(32) DEFAULT NULL,
  `Lado` varchar(5) NOT NULL DEFAULT 'A',
  `Modo` varchar(10) DEFAULT NULL,
  PRIMARY KEY (`IdSistema`,`IdRecurso`,`TipoRecurso`),
  KEY `RecursosTF_FKIndex1` (`IdSistema`,`IdRecurso`,`TipoRecurso`),
  KEY `RecursosTF_FKIndex2` (`IdSistema`,`IdRed`),
  KEY `RecursosTF_FKIndex3` (`IdSistema`,`IdTroncal`),
  KEY `RecursosTF_FKIndex4` (`IdSistema`,`IdDestino`,`TipoDestino`,`IdPrefijo`),
  CONSTRAINT `recursostf_ibfk_1` FOREIGN KEY (`IdSistema`, `IdRecurso`, `TipoRecurso`) REFERENCES `recursos` (`IdSistema`, `IdRecurso`, `TipoRecurso`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `recursostf_ibfk_2` FOREIGN KEY (`IdSistema`, `IdRed`) REFERENCES `redes` (`IdSistema`, `IdRed`) ON DELETE NO ACTION ON UPDATE CASCADE,
  CONSTRAINT `recursostf_ibfk_3` FOREIGN KEY (`IdSistema`, `IdTroncal`) REFERENCES `troncales` (`IdSistema`, `IdTroncal`) ON DELETE NO ACTION ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `redes`
--

DROP TABLE IF EXISTS `redes`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `redes` (
  `IdSistema` varchar(32) NOT NULL,
  `IdRed` varchar(32) NOT NULL,
  `IdPrefijo` int(10) unsigned NOT NULL,
  PRIMARY KEY (`IdSistema`,`IdRed`),
  UNIQUE KEY `Redes_Unique_Pref_idx` (`IdSistema`,`IdPrefijo`),
  KEY `REDES_FKIndex1` (`IdSistema`),
  CONSTRAINT `redes_ibfk_1` FOREIGN KEY (`IdSistema`) REFERENCES `sistema` (`IdSistema`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `redes_ibfk_2` FOREIGN KEY (`IdSistema`, `IdPrefijo`) REFERENCES `prefijos` (`IdSistema`, `IdPrefijo`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER `new_cd40`.`REDDELETE`
BEFORE DELETE ON `new_cd40`.`redes`
FOR EACH ROW
BEGIN
UPDATE RECURSOSTF SET IdRed=NULL 
	WHERE IdRed=old.IdRed AND IdSistema=old.IdSistema;
DELETE FROM destinostelefonia WHERE
  IdSistema=old.IdSistema AND
  TipoDestino=1 AND
  IdPrefijo=old.IdPrefijo;
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;

--
-- Table structure for table `rutas`
--

DROP TABLE IF EXISTS `rutas`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `rutas` (
  `IdSistema` varchar(32) NOT NULL,
  `IdRuta` varchar(32) NOT NULL,
  `Central` varchar(32) NOT NULL,
  `Tipo` varchar(1) DEFAULT NULL,
  `Orden` int(11) DEFAULT NULL,
  PRIMARY KEY (`IdSistema`,`IdRuta`,`Central`),
  KEY `Rutas_FKIndex1` (`IdSistema`,`Central`),
  CONSTRAINT `rutas_ibfk_1` FOREIGN KEY (`IdSistema`, `Central`) REFERENCES `encaminamientos` (`IdSistema`, `Central`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `sectores`
--

DROP TABLE IF EXISTS `sectores`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `sectores` (
  `IdSistema` varchar(32) NOT NULL,
  `IdNucleo` varchar(32) NOT NULL,
  `IdSector` varchar(700) NOT NULL,
  `IdSistemaParejaUCS` varchar(32) DEFAULT NULL,
  `IdNucleoParejaUCS` varchar(32) DEFAULT NULL,
  `IdParejaUCS` varchar(32) DEFAULT NULL,
  `SectorSimple` tinyint(1) DEFAULT '1',
  `Tipo` varchar(1) DEFAULT 'R',
  `TipoPosicion` varchar(1) DEFAULT 'C',
  `PrioridadR2` int(1) unsigned DEFAULT '4',
  `TipoHMI` int(10) unsigned DEFAULT NULL,
  `NumSacta` int(10) unsigned DEFAULT NULL,
  `SeleccionadoFS` tinyint(1) DEFAULT '0' COMMENT 'Modificación para marca de sector seleccionado modelo Fuera Sectorización',
  `IdDependenciaATS` varchar(32) DEFAULT NULL,
  PRIMARY KEY (`IdSistema`,`IdNucleo`,`IdSector`),
  KEY `Sectores_FKIndex1` (`IdSistema`,`IdNucleo`),
  KEY `Sectores_FKIndex3` (`IdSistemaParejaUCS`,`IdNucleoParejaUCS`,`IdParejaUCS`),
  CONSTRAINT `sectores_ibfk_1` FOREIGN KEY (`IdSistema`, `IdNucleo`) REFERENCES `nucleos` (`IdSistema`, `IdNucleo`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `sectores_ibfk_3` FOREIGN KEY (`IdSistemaParejaUCS`, `IdNucleoParejaUCS`, `IdParejaUCS`) REFERENCES `sectores` (`IdSistema`, `IdNucleo`, `IdSector`) ON DELETE NO ACTION ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `sectoresagrupacion`
--

DROP TABLE IF EXISTS `sectoresagrupacion`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `sectoresagrupacion` (
  `IdSistema` varchar(32) NOT NULL,
  `IdAgrupacion` varchar(32) NOT NULL,
  `IdSector` varchar(700) NOT NULL,
  `IdNucleo` varchar(32) NOT NULL,
  PRIMARY KEY (`IdSistema`,`IdAgrupacion`,`IdSector`,`IdNucleo`),
  KEY `Sectores_has_Agrupaciones_FKIndex1` (`IdSistema`,`IdNucleo`,`IdSector`),
  KEY `Sectores_has_Agrupaciones_FKIndex2` (`IdSistema`,`IdAgrupacion`),
  CONSTRAINT `sectoresagrupacion_ibfk_1` FOREIGN KEY (`IdSistema`, `IdNucleo`, `IdSector`) REFERENCES `sectores` (`IdSistema`, `IdNucleo`, `IdSector`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `sectoresagrupacion_ibfk_2` FOREIGN KEY (`IdSistema`, `IdAgrupacion`) REFERENCES `agrupaciones` (`IdSistema`, `IdAgrupacion`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Temporary view structure for view `sectoresentopsparainformexml`
--

DROP TABLE IF EXISTS `sectoresentopsparainformexml`;
/*!50001 DROP VIEW IF EXISTS `sectoresentopsparainformexml`*/;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
/*!50001 CREATE VIEW `sectoresentopsparainformexml` AS SELECT 
 1 AS `Idsistema`,
 1 AS `IdSectorizacion`,
 1 AS `IdToP`,
 1 AS `IdSector`,
 1 AS `IdNucleo`,
 1 AS `IdSectorOriginal`*/;
SET character_set_client = @saved_cs_client;

--
-- Table structure for table `sectoressector`
--

DROP TABLE IF EXISTS `sectoressector`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `sectoressector` (
  `IdSistema` varchar(32) NOT NULL,
  `IdNucleo` varchar(32) NOT NULL,
  `IdSector` varchar(700) NOT NULL,
  `IdSectorOriginal` varchar(700) NOT NULL,
  `EsDominante` tinyint(1) DEFAULT '0',
  PRIMARY KEY (`IdSistema`,`IdNucleo`,`IdSector`,`IdSectorOriginal`),
  KEY `SectoresSector_FKIndex1` (`IdSistema`,`IdNucleo`,`IdSector`),
  KEY `SectoresSector_FKIndex2` (`IdSectorOriginal`),
  CONSTRAINT `sectoressector_ibfk_1` FOREIGN KEY (`IdSistema`, `IdNucleo`, `IdSector`) REFERENCES `sectores` (`IdSistema`, `IdNucleo`, `IdSector`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `sectoressectorizacion`
--

DROP TABLE IF EXISTS `sectoressectorizacion`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `sectoressectorizacion` (
  `IdSistema` varchar(32) NOT NULL,
  `IdSectorizacion` varchar(32) NOT NULL,
  `IdNucleo` varchar(32) NOT NULL,
  `IdSector` varchar(700) NOT NULL,
  `IdTOP` varchar(32) NOT NULL,
  PRIMARY KEY (`IdSistema`,`IdSectorizacion`,`IdNucleo`,`IdSector`,`IdTOP`),
  KEY `SectoresSectorizacion_FKIndex1` (`IdSistema`,`IdSectorizacion`),
  KEY `SectoresSectorizacion_FKIndex2` (`IdSistema`,`IdNucleo`,`IdSector`),
  KEY `SectoresSectorizacion_FKIndex3` (`IdSistema`,`IdTOP`),
  CONSTRAINT `sectoressectorizacion_ibfk_1` FOREIGN KEY (`IdSistema`, `IdSectorizacion`) REFERENCES `sectorizaciones` (`IdSistema`, `IdSectorizacion`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `sectoressectorizacion_ibfk_2` FOREIGN KEY (`IdSistema`, `IdNucleo`, `IdSector`) REFERENCES `sectores` (`IdSistema`, `IdNucleo`, `IdSector`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `sectoressectorizacion_ibfk_3` FOREIGN KEY (`IdSistema`, `IdTOP`) REFERENCES `top` (`IdSistema`, `IdTOP`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `sectorizaciones`
--

DROP TABLE IF EXISTS `sectorizaciones`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `sectorizaciones` (
  `IdSistema` varchar(32) NOT NULL,
  `IdSectorizacion` varchar(32) NOT NULL,
  `Activa` tinyint(1) DEFAULT NULL,
  `FechaActivacion` datetime DEFAULT NULL,
  PRIMARY KEY (`IdSistema`,`IdSectorizacion`),
  KEY `Sectorizaciones_FKIndex1` (`IdSistema`),
  CONSTRAINT `sectorizaciones_ibfk_1` FOREIGN KEY (`IdSistema`) REFERENCES `sistema` (`IdSistema`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `sistema`
--

DROP TABLE IF EXISTS `sistema`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `sistema` (
  `IdSistema` varchar(32) NOT NULL,
  `TiempoPTT` int(10) unsigned DEFAULT NULL,
  `TiempoSinJack1` int(10) unsigned DEFAULT NULL,
  `TiempoSinJack2` int(10) unsigned DEFAULT NULL,
  `TamLiteralEnlExt` int(10) unsigned DEFAULT '32',
  `TamLiteralDA` int(10) unsigned DEFAULT '32',
  `TamLiteralIA` int(10) unsigned DEFAULT '32',
  `TamLiteralAG` int(10) unsigned DEFAULT '32',
  `TamLiteralEmplazamiento` int(10) unsigned DEFAULT '32',
  `VersionIP` int(1) unsigned DEFAULT NULL,
  `GrupoMulticastConfiguracion` varchar(15) DEFAULT NULL,
  `PuertoMulticastConfiguracion` int(5) unsigned DEFAULT '1000',
  `EstadoScvA` int(1) unsigned DEFAULT '0',
  `EstadoScvB` int(1) unsigned DEFAULT '0',
  `NumLlamadasEnIDA` int(10) unsigned DEFAULT '10',
  `KeepAliveMultiplier` int(10) unsigned DEFAULT '10',
  `KeepAlivePeriod` int(10) unsigned DEFAULT '200',
  `NumPagDestinosInt` int(10) unsigned DEFAULT NULL,
  `NumDestinosInternosPag` int(10) unsigned DEFAULT NULL,
  `NumPagFreq` int(10) unsigned DEFAULT NULL,
  `NumFreqPagina` int(10) unsigned DEFAULT NULL,
  `NumLlamadasEntrantesIDA` int(10) unsigned DEFAULT '5',
  `NumEnlacesAI` int(10) unsigned DEFAULT '18',
  `NumMaxSectores` int(10) unsigned DEFAULT '32',
  PRIMARY KEY (`IdSistema`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER `new_cd40`.`AltaSistema`
AFTER INSERT ON `new_cd40`.`sistema`
FOR EACH ROW
BEGIN
    INSERT INTO Prefijos VALUES (new.IdSistema,0);
    INSERT INTO Prefijos VALUES (new.IdSistema,1);
    INSERT INTO Prefijos VALUES (new.IdSistema,2);
    INSERT INTO Prefijos VALUES (new.IdSistema,3);
    INSERT INTO Prefijos VALUES (new.IdSistema,4);
    INSERT INTO Prefijos VALUES (new.IdSistema,5);
    INSERT INTO Prefijos VALUES (new.IdSistema,6);
    INSERT INTO Prefijos VALUES (new.IdSistema,7);
    INSERT INTO Prefijos VALUES (new.IdSistema,8);
    INSERT INTO Prefijos VALUES (new.IdSistema,9);
    INSERT INTO Prefijos VALUES (new.IdSistema,10);
    INSERT INTO Prefijos VALUES (new.IdSistema,11);
    INSERT INTO Prefijos VALUES (new.IdSistema,12);
    INSERT INTO Prefijos VALUES (new.IdSistema,13);
    INSERT INTO Prefijos VALUES (new.IdSistema,14);
    INSERT INTO Prefijos VALUES (new.IdSistema,15);
    INSERT INTO Prefijos VALUES (new.IdSistema,16);
    INSERT INTO Prefijos VALUES (new.IdSistema,17);
    INSERT INTO Prefijos VALUES (new.IdSistema,18);
    INSERT INTO Prefijos VALUES (new.IdSistema,19);
    INSERT INTO Prefijos VALUES (new.IdSistema,20);
    INSERT INTO Prefijos VALUES (new.IdSistema,21);
    INSERT INTO Prefijos VALUES (new.IdSistema,22);
    INSERT INTO Prefijos VALUES (new.IdSistema,23);
    INSERT INTO Prefijos VALUES (new.IdSistema,24);
    INSERT INTO Prefijos VALUES (new.IdSistema,25);
    INSERT INTO Prefijos VALUES (new.IdSistema,26);
    INSERT INTO Prefijos VALUES (new.IdSistema,27);
    INSERT INTO Prefijos VALUES (new.IdSistema,28);
    INSERT INTO Prefijos VALUES (new.IdSistema,29);
    INSERT INTO Prefijos VALUES (new.IdSistema,30);
    INSERT INTO Prefijos VALUES (new.IdSistema,31);
    INSERT INTO Prefijos VALUES (new.IdSistema,32);
		
	INSERT INTO Redes VALUES (new.IdSistema,"ATS",3) ;    

	UPDATE ParametrosSector SET NumLlamadasEntrantesIDA=new.NumLlamadasEntrantesIDA,
                                NumLlamadasEnIDA=new.NumLlamadasEnIda,
                                NumFreqPagina=new.NumFreqPagina,
                                NumPagFreq=new.NumPagFreq,
                                NumDestinosInternosPag=new.NumDestinosInternosPag,
                                NumPagDestinosInt=new.NumPagDestinosInt,
								KeepAliveMultiplier=new.KeepAliveMultiplier,
								KeepAlivePeriod=new.KeepAlivePeriod,
								NumEnlacesAI=new.NumEnlacesAI
								 WHERE IdSistema=new.IdSistema;

END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER `new_cd40`.`sistema_AUPD`
AFTER UPDATE ON `new_cd40`.`sistema`
FOR EACH ROW
UPDATE ParametrosSector SET NumLlamadasEntrantesIDA=new.NumLlamadasEntrantesIDA,
                                NumLlamadasEnIDA=new.NumLlamadasEnIda,
                                NumFreqPagina=new.NumFreqPagina,
                                NumPagFreq=new.NumPagFreq,
                                NumDestinosInternosPag=new.NumDestinosInternosPag,
                                NumPagDestinosInt=new.NumPagDestinosInt,
								KeepAliveMultiplier=new.KeepAliveMultiplier,
								KeepAlivePeriod=new.KeepAlivePeriod,
								NumEnlacesAI=new.NumEnlacesAI
								 WHERE IdSistema=new.IdSistema */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;

--
-- Table structure for table `tabla_bss`
--

DROP TABLE IF EXISTS `tabla_bss`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tabla_bss` (
  `idtabla_bss` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(32) DEFAULT NULL,
  `description` varchar(128) DEFAULT NULL,
  `UsuarioCreacion` varchar(32) DEFAULT NULL,
  `FechaCreacion` datetime DEFAULT CURRENT_TIMESTAMP,
  `UsuarioModificacion` varchar(32) DEFAULT NULL,
  `FechaModificacion` datetime DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`idtabla_bss`)
) ENGINE=InnoDB AUTO_INCREMENT=8 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `tablasmodificadas`
--

DROP TABLE IF EXISTS `tablasmodificadas`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tablasmodificadas` (
  `IdTabla` varchar(32) NOT NULL,
  PRIMARY KEY (`IdTabla`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `teclassector`
--

DROP TABLE IF EXISTS `teclassector`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `teclassector` (
  `IdSistema` varchar(32) NOT NULL,
  `IdNucleo` varchar(32) NOT NULL,
  `IdSector` varchar(700) NOT NULL,
  `TransConConsultaPrev` tinyint(1) DEFAULT '1',
  `TransDirecta` tinyint(1) DEFAULT '0',
  `Conferencia` tinyint(1) DEFAULT '0',
  `Escucha` tinyint(1) DEFAULT '0',
  `Retener` tinyint(1) DEFAULT '1',
  `Captura` tinyint(1) DEFAULT '0',
  `Redireccion` tinyint(1) DEFAULT '0',
  `RepeticionUltLlamada` tinyint(1) DEFAULT '1',
  `RellamadaAut` tinyint(1) DEFAULT '0',
  `TeclaPrioridad` tinyint(1) DEFAULT '1',
  `Tecla55mas1` tinyint(1) DEFAULT '0',
  `Monitoring` tinyint(1) DEFAULT '1',
  `CoordinadorTF` tinyint(1) DEFAULT '0',
  `CoordinadorRD` tinyint(1) DEFAULT '0',
  `IntegracionRDTF` tinyint(1) DEFAULT '0',
  `LlamadaSelectiva` tinyint(1) DEFAULT '0',
  `GrupoBSS` tinyint(1) DEFAULT '1',
  `LTT` tinyint(1) DEFAULT '1',
  `SayAgain` tinyint(1) DEFAULT '1',
  `InhabilitacionRedirec` tinyint(1) DEFAULT '0',
  `Glp` tinyint(1) DEFAULT '0',
  PRIMARY KEY (`IdSistema`,`IdNucleo`,`IdSector`),
  KEY `TeclasSector_FKIndex1` (`IdSistema`,`IdNucleo`,`IdSector`),
  CONSTRAINT `teclassector_ibfk_1` FOREIGN KEY (`IdSistema`, `IdNucleo`, `IdSector`) REFERENCES `sectores` (`IdSistema`, `IdNucleo`, `IdSector`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `tifx`
--

DROP TABLE IF EXISTS `tifx`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tifx` (
  `IdSistema` varchar(32) NOT NULL,
  `IdTIFX` varchar(32) NOT NULL,
  `ModoArranque` varchar(1) DEFAULT NULL,
  `ModoSincronizacion` int(1) unsigned DEFAULT NULL,
  `Master` varchar(32) DEFAULT '0.0.0.0:0',
  `SNMPPortLocal` int(10) unsigned DEFAULT '161',
  `SNMPPortRemoto` int(10) unsigned DEFAULT '161',
  `SNMPTraps` int(10) unsigned DEFAULT '162',
  `SIPPortLocal` int(10) unsigned DEFAULT '5060',
  `TimeSupervision` int(10) unsigned DEFAULT '0',
  `IpRed1` varchar(60) DEFAULT NULL,
  `IpRed2` varchar(60) DEFAULT NULL,
  `Grabador1` varchar(45) DEFAULT NULL,
  `Grabador2` varchar(45) DEFAULT NULL,
  `iSupervLanGW` tinyint(1) NOT NULL DEFAULT '0' COMMENT 'Incorpora a la supervision ethernet de la pasarela la conexión a la Puerta de enlace. 0: no supervisa conexion GW; 1: supervisa GW.',
  `itmmaxSupervLanGW` int(1) NOT NULL DEFAULT '1' COMMENT 'Si supervisionLanGW=1,  tiempo maximo (en sg) de espera conexion GW. Si vence es que no hay ping a la puerta de enlace. Este tiempo tiene que ser menor de 5 sg.\n',
  PRIMARY KEY (`IdSistema`,`IdTIFX`),
  KEY `TIFX_FKIndex1` (`IdSistema`),
  CONSTRAINT `tifx_ibfk_1` FOREIGN KEY (`IdSistema`) REFERENCES `sistema` (`IdSistema`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `top`
--

DROP TABLE IF EXISTS `top`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `top` (
  `IdSistema` varchar(32) NOT NULL,
  `IdTOP` varchar(32) NOT NULL,
  `PosicionSacta` int(10) unsigned DEFAULT NULL,
  `ModoArranque` varchar(1) DEFAULT NULL,
  `IpRed1` varchar(60) DEFAULT NULL,
  `IpRed2` varchar(60) DEFAULT NULL,
  `ConexionJacksEjecutivo` tinyint(1) DEFAULT '1',
  `ConexionJacksAyudante` tinyint(1) DEFAULT '1',
  `NumAltavoces` smallint(1) DEFAULT '8',
  `Grabador1` varchar(45) DEFAULT NULL,
  `Grabador2` varchar(45) DEFAULT NULL,
  `IdDependenciaATS` varchar(32) DEFAULT NULL,
  PRIMARY KEY (`IdSistema`,`IdTOP`),
  KEY `TOP_FKIndex1` (`IdSistema`),
  CONSTRAINT `top_ibfk_1` FOREIGN KEY (`IdSistema`) REFERENCES `sistema` (`IdSistema`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `troncales`
--

DROP TABLE IF EXISTS `troncales`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `troncales` (
  `IdSistema` varchar(32) NOT NULL,
  `IdTroncal` varchar(32) NOT NULL,
  `NumTest` varchar(16) DEFAULT NULL,
  PRIMARY KEY (`IdSistema`,`IdTroncal`),
  KEY `Troncales_FKIndex1` (`IdSistema`),
  CONSTRAINT `troncales_ibfk_1` FOREIGN KEY (`IdSistema`) REFERENCES `sistema` (`IdSistema`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER `new_cd40`.`TRONCALDELETE`
BEFORE DELETE ON `new_cd40`.`troncales`
FOR EACH ROW
UPDATE RECURSOSTF SET IdTroncal=NULL 
		WHERE IdTroncal=old.IdTroncal AND IdSistema=old.IdSistema */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;

--
-- Table structure for table `troncalesruta`
--

DROP TABLE IF EXISTS `troncalesruta`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `troncalesruta` (
  `IdSistema` varchar(32) NOT NULL,
  `IdTroncal` varchar(32) NOT NULL,
  `Central` varchar(32) NOT NULL,
  `IdRuta` varchar(32) NOT NULL,
  PRIMARY KEY (`IdSistema`,`IdTroncal`,`Central`,`IdRuta`),
  KEY `TroncalesRuta_FKIndex1` (`IdSistema`,`IdTroncal`),
  KEY `TroncalesRuta_FKIndex2` (`IdSistema`,`IdRuta`,`Central`),
  CONSTRAINT `troncalesruta_ibfk_1` FOREIGN KEY (`IdSistema`, `IdTroncal`) REFERENCES `troncales` (`IdSistema`, `IdTroncal`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `troncalesruta_ibfk_2` FOREIGN KEY (`IdSistema`, `IdRuta`, `Central`) REFERENCES `rutas` (`IdSistema`, `IdRuta`, `Central`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `usuariosabonados`
--

DROP TABLE IF EXISTS `usuariosabonados`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `usuariosabonados` (
  `IdSistema` varchar(32) NOT NULL,
  `IdPrefijo` int(10) unsigned NOT NULL,
  `IdNucleo` varchar(32) NOT NULL,
  `IdSector` varchar(700) NOT NULL,
  `IdAbonado` varchar(32) NOT NULL,
  PRIMARY KEY (`IdSistema`,`IdPrefijo`,`IdNucleo`,`IdSector`,`IdAbonado`),
  KEY `UsuariosAbonados_FKIndex2` (`IdSistema`,`IdPrefijo`),
  KEY `UsuariosAbonados_FKIndex3` (`IdSistema`,`IdNucleo`,`IdSector`),
  KEY `UsuariosAbonados_FKIndex4` (`IdSector`),
  CONSTRAINT `usuariosabonados_ibfk_2` FOREIGN KEY (`IdSistema`, `IdPrefijo`) REFERENCES `redes` (`IdSistema`, `IdPrefijo`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `usuariosabonados_ibfk_3` FOREIGN KEY (`IdSistema`, `IdNucleo`, `IdSector`) REFERENCES `sectores` (`IdSistema`, `IdNucleo`, `IdSector`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `valores_tabla`
--

DROP TABLE IF EXISTS `valores_tabla`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `valores_tabla` (
  `idvalores_tabla` int(11) NOT NULL AUTO_INCREMENT,
  `tabla_bss_idtabla_bss` int(11) NOT NULL,
  `valor_prop` int(11) NOT NULL,
  `valor_rssi` int(11) NOT NULL,
  PRIMARY KEY (`idvalores_tabla`,`tabla_bss_idtabla_bss`),
  KEY `fk_valores_tabla_tabla_bss1_idx` (`tabla_bss_idtabla_bss`),
  CONSTRAINT `fk_valores_tabla_tabla_bss1` FOREIGN KEY (`tabla_bss_idtabla_bss`) REFERENCES `tabla_bss` (`idtabla_bss`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=99 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Temporary view structure for view `viewdestinostelefonia`
--

DROP TABLE IF EXISTS `viewdestinostelefonia`;
/*!50001 DROP VIEW IF EXISTS `viewdestinostelefonia`*/;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
/*!50001 CREATE VIEW `viewdestinostelefonia` AS SELECT 
 1 AS `IdSistema`,
 1 AS `IdDestino`,
 1 AS `TipoDestino`,
 1 AS `IdGrupo`,
 1 AS `IdPrefijo`,
 1 AS `IdAbonado`,
 1 AS `IdRed`*/;
SET character_set_client = @saved_cs_client;

--
-- Temporary view structure for view `viewequiposexternos`
--

DROP TABLE IF EXISTS `viewequiposexternos`;
/*!50001 DROP VIEW IF EXISTS `viewequiposexternos`*/;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
/*!50001 CREATE VIEW `viewequiposexternos` AS SELECT 
 1 AS `idEquipos`,
 1 AS `IpRed1`,
 1 AS `tipoEquipo`,
 1 AS `modeloequipo`,
 1 AS `tiporadio`*/;
SET character_set_client = @saved_cs_client;

--
-- Temporary view structure for view `viewincidenciasmasalarma`
--

DROP TABLE IF EXISTS `viewincidenciasmasalarma`;
/*!50001 DROP VIEW IF EXISTS `viewincidenciasmasalarma`*/;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
/*!50001 CREATE VIEW `viewincidenciasmasalarma` AS SELECT 
 1 AS `IdSistema`,
 1 AS `IdIncidencia`,
 1 AS `Incidencia`,
 1 AS `alarma`*/;
SET character_set_client = @saved_cs_client;

--
-- Temporary view structure for view `viewincidenciasmasalarma_ingles`
--

DROP TABLE IF EXISTS `viewincidenciasmasalarma_ingles`;
/*!50001 DROP VIEW IF EXISTS `viewincidenciasmasalarma_ingles`*/;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
/*!50001 CREATE VIEW `viewincidenciasmasalarma_ingles` AS SELECT 
 1 AS `IdSistema`,
 1 AS `IdIncidencia`,
 1 AS `Incidencia`,
 1 AS `alarma`*/;
SET character_set_client = @saved_cs_client;

--
-- Temporary view structure for view `viewrecursosimplicadosrutas`
--

DROP TABLE IF EXISTS `viewrecursosimplicadosrutas`;
/*!50001 DROP VIEW IF EXISTS `viewrecursosimplicadosrutas`*/;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
/*!50001 CREATE VIEW `viewrecursosimplicadosrutas` AS SELECT 
 1 AS `idsistema`,
 1 AS `idrecurso`,
 1 AS `iddestino`,
 1 AS `idred`,
 1 AS `idtroncal`,
 1 AS `idruta`,
 1 AS `tipo`*/;
SET character_set_client = @saved_cs_client;

--
-- Temporary view structure for view `viewsectoresenagrupacion`
--

DROP TABLE IF EXISTS `viewsectoresenagrupacion`;
/*!50001 DROP VIEW IF EXISTS `viewsectoresenagrupacion`*/;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
/*!50001 CREATE VIEW `viewsectoresenagrupacion` AS SELECT 
 1 AS `contador`,
 1 AS `IdAgrupacion`*/;
SET character_set_client = @saved_cs_client;

--
-- Temporary view structure for view `viewsectoresentops`
--

DROP TABLE IF EXISTS `viewsectoresentops`;
/*!50001 DROP VIEW IF EXISTS `viewsectoresentops`*/;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
/*!50001 CREATE VIEW `viewsectoresentops` AS SELECT 
 1 AS `IdSistema`,
 1 AS `idsectorizacion`,
 1 AS `IdTOP`,
 1 AS `idsector`,
 1 AS `idnucleo`,
 1 AS `tipo`*/;
SET character_set_client = @saved_cs_client;

--
-- Table structure for table `zonas`
--

DROP TABLE IF EXISTS `zonas`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `zonas` (
  `idZonas` int(11) NOT NULL AUTO_INCREMENT,
  `IdSistema` varchar(45) NOT NULL,
  `Nombre` varchar(45) NOT NULL,
  PRIMARY KEY (`idZonas`),
  UNIQUE KEY `idx_zonas_unique_name` (`IdSistema`,`Nombre`)
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=utf8 COMMENT='Zonas Radio';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping events for database 'new_cd40'
--

--
-- Dumping routines for database 'new_cd40'
--
/*!50003 DROP PROCEDURE IF EXISTS `ActualizaNombreSector` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `ActualizaNombreSector`(in id_sistema varchar(32), in id_nucleo varchar(32), in id_sector varchar(700), in id_nuevo_nombre_sector varchar(700))
BEGIN
	UPDATE Sectores 
		SET IdSector=id_nuevo_nombre_sector
		WHERE IdSistema=id_sistema AND
					IdNucleo=id_nucleo AND
					IdSector=id_sector AND
					SectorSimple=FALSE;
		REPLACE INTO TablasModificadas (IdTabla)
				VALUES ('Sectores');
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `ActualizaSectoresSectorizacion` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `ActualizaSectoresSectorizacion`(in id_sistema varchar(32), in id_nucleo varchar(32), in id_grupo varchar(700), in id_agrupacion varchar(700))
BEGIN
	DECLARE id char(32);
	
	SELECT DATE_FORMAT(FechaActivacion,"%d/%m/%Y %H:%i:%s") INTO id FROM sectorizaciones WHERE IdSistema=id_sistema AND Activa=true;
	
	UPDATE SectoresSectorizacion
		SET IdSector=id_agrupacion
		WHERE IdSistema=id_sistema AND
					IdNucleo=id_nucleo AND
					IdSector=id_grupo AND
					IdSectorizacion<>id;
	REPLACE INTO TablasModificadas (IdTabla)
				VALUES ('SectoresSectorizacion');
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `AgrupacionDeLosSectores` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `AgrupacionDeLosSectores`(in cuantos int, in lista_usuarios varchar(700))
BEGIN
	DECLARE lista VARCHAR(700);
	SET @lista=lista_usuarios;
	SELECT sa.IdAgrupacion,@lista,lista FROM SectoresAgrupacion sa, viewSectoresEnAgrupacion cuenta
	WHERE cuenta.contador=cuantos AND
				sa.IdAgrupacion=cuenta.IdAgrupacion	AND
				sa.IdSector in (@lista);
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `AsignacionRecursosDeUnaRed` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `AsignacionRecursosDeUnaRed`(in id_sistema char(32), in id_red char(32))
BEGIN
	IF (id_red is not null) THEN
      SELECT 'P' AS TIPO,rTf.IdRed, r.IdRecurso, r.IdTifx AS NOMBRE, 
        r.SlotPasarela AS SlotPasarela, r.NumDispositivoSlot AS NumDispositivoSlot
      FROM new_cd40_trans.RecursosTf rTf, new_cd40_trans.Recursos r
      WHERE rTf.IdSistema=id_sistema AND
            rTf.IdRed=id_red AND
            r.IdSistema=rTf.IdSistema AND
            r.IdRecurso=rTf.IdRecurso AND r.IdTifx IS NOT NULL
      UNION
      SELECT 'E' AS TIPO,rTf.IdRed, r.IdRecurso, r.Idequipos AS NOMBRE, 
            NULL AS SlotPasarela, NULL AS NumDispositivoSlot
          FROM new_cd40_trans.RecursosTf rTf, new_cd40_trans.Recursos r
          WHERE rTf.IdSistema=id_sistema AND
                rTf.IdRed=id_red AND
                r.IdSistema=rTf.IdSistema AND
                r.IdRecurso=rTf.IdRecurso AND r.Idequipos IS NOT NULL 
      ORDER BY TIPO,NOMBRE,SlotPasarela, NumDispositivoSlot,IdRecurso;  
	ELSE
      -- Todas las redes de telefonia
       SELECT    CASE WHEN r.IdTifx IS NOT NULL THEN 'P' ELSE CASE WHEN r.Idequipos IS NOT NULL THEN 'E' ELSE NULL END END AS TIPO,
                rd.IdRed,
                r.IdRecurso, 
                CASE WHEN r.IdTifx IS NOT NULL  THEN r.IdTifx ELSE  r.Idequipos END AS NOMBRE,
                CASE WHEN r.IdTifx IS NOT NULL  THEN r.SlotPasarela ELSE  NULL END AS SlotPasarela,
                CASE WHEN r.IdTifx IS NOT NULL  THEN r.NumDispositivoSlot ELSE   NULL END AS NumDispositivoSlot
        FROM Redes rd
        LEFT JOIN RecursosTf rTf ON rd.IdSistema=rTf.IdSistema AND rd.IdRed=rTf.IdRed          
        LEFT JOIN Recursos r     ON rTf.IdSistema=r.IdSistema  AND rTf.IdRecurso=r.IdRecurso AND rTf.TipoRecurso=r.TipoRecurso
        WHERE rd.IdSistema=id_sistema
        ORDER BY TIPO,NOMBRE,SlotPasarela, NumDispositivoSlot,IdRecurso;  
	END IF;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `AsignacionUsuariosATops` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `AsignacionUsuariosATops`(in id_Sistema char(32))
BEGIN
SELECT c.IdSectorOriginal AS IdSector,a.IdTop,t.Grabador1,e1.IpRed1 as IpGrabador1, 
			t.Grabador2,e2.IpRed1 as IpGrabador2, e1.SipPort as RtspPort 
		FROM SectoresSector c
		INNER JOIN sectorizaciones b ON b.IdSistema = c.IdSistema AND b.Activa=true
		INNER JOIN sectoressectorizacion a ON a.IdSistema=b.IdSistema AND a.IdSectorizacion=b.IdSectorizacion
		INNER JOIN top t ON t.IdTop=a.IdTop
		LEFT JOIN equiposeu e1 ON e1.IdEquipos=t.grabador1
		LEFT JOIN equiposeu e2 ON e2.idEquipos=t.grabador2
		WHERE 	c.IdSistema=id_Sistema AND
				c.IdNucleo=a.IdNucleo AND
				c.IdSector=a.IdSector;     
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `CheckIdentificadorAsignado` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `CheckIdentificadorAsignado`(in Tipo char(32),in pid_sistema char(32), in id_identificador char(32), out iAsignado int)
BEGIN
  -- Devuelve 1 si el identificador del tipo, que se le pasa como parámetro, existe en el sistema, 0 si no existe y 
  -- -1 si se ha producido error. 
  -- El tipo puede ser: EF (Pasarelas,Tops, Equipos Externos y Encaminamientos).
  --                    R (Recursos Radio y de Telefonía)
  --                    D (Destinos Radio y de Telefonía), 
  --                    NA (Emplazamientos y Zonas) 
  DECLARE iCuenta INT;

  SET iCuenta= -1;
  
  CASE 
    WHEN (TIPO='EF') THEN     
       -- Se comprueba si el identificador está asignado a una pasarela PASARELA,TOP, EQUIPOSEU o encaminamiento
       -- Los encaminamientos que son SCV IP tienen un registro en la tabla de equiposeu con las direcciones IP
       -- de los servidores proxy y de presencia
      SELECT COUNT(1) INTO iCuenta FROM (
                            SELECT IdSistema AS IdSistema ,IdTIFX as ID,'P' AS TIPO FROM tifx
                            UNION
                            SELECT IdSistema AS IdSistema,IdEquipos AS ID ,'E' AS TIPO FROM equiposeu
                            UNION 
                            SELECT IdSistema AS IdSistema,Central AS ID ,'C' AS TIPO FROM encaminamientos
                            UNION 
                            SELECT IdSistema AS IdSistema,IdTop AS ID,'T' AS TIPO FROM top) A
      WHERE A.idSistema= pid_sistema and A.ID=id_identificador;
    WHEN (TIPO='R') THEN
      -- Se comprueba si el identificador de recursos de telefonía o radio está asignado a algun recurso
      SELECT COUNT(1) INTO iCuenta 
      FROM recursos 
      WHERE idSistema=pid_sistema and idRecurso=id_identificador;

    WHEN (TIPO='D') THEN
      -- Se comprueba si el Identificador del destino radio o de telefonía está libre
      SELECT  COUNT(1) INTO iCuenta
      FROM destinostelefonia A 
         JOIN destinosexternos B ON A.IdSistema=B.IdSistema AND A.IdDestino=B.IdDestino AND 
                    A.TipoDestino=B.TipoDestino AND A.IdPrefijo=B.IdPrefijo
         LEFT JOIN  redes R ON A.IdSistema=R.IdSistema AND A.IdPrefijo=R.IdPrefijo
      WHERE A.IdSistema=pid_sistema and A.IdDestino=id_identificador;
	  
      IF iCuenta=0 THEN
        -- Se comprueba si el identificador existe como DestinoRadio
        SELECT COUNT(1) INTO iCuenta
        FROM destinosradio
        WHERE IdSistema=pid_sistema and IdDestino=id_identificador;
      END IF;  

    WHEN (TIPO='NA') THEN
      -- Identificadores no agrupados: EMPLAZAMIENTOS, ZONAS
      SELECT COUNT(1) INTO iCuenta
      FROM emplazamientos
      WHERE IdSistema=pid_sistema and IdEmplazamiento=id_identificador;

      IF iCuenta=0 THEN
      -- Se comprueba si el identificador existe como zonas
       SELECT COUNT(1) INTO iCuenta
       FROM  zonas
       WHERE IdSistema=pid_sistema AND Nombre=id_identificador;
      END IF;	
    ELSE
      SET iCuenta= -1;
  END CASE; 

  IF iCuenta>0 THEN
      SET iAsignado=1;      
  ELSE
      SET iAsignado=iCuenta;
  END IF;

END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `CreaPosicionesActiva` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `CreaPosicionesActiva`(in id_sistema char(32), in id_sectorizacion char(32), in id_activa char(32))
BEGIN
		INSERT into radio
			SELECT IdSistema,id_activa as IdSectorizacion,IdNucleo,IdSector,PosHMI,IdDestino,Prioridad,PrioridadSIP,ModoOperacion,Cascos,Literal,SupervisionPortadora FROM Radio
				WHERE IdSectorizacion=id_sectorizacion AND
							IdSistema=id_sistema;
		REPLACE INTO TablasModificadas (IdTabla)
				VALUES ('radio');
	
		INSERT into Internos
			SELECT IdSistema,id_activa as IdSectorizacion,IdNucleo,IdSector,IdDestino,IdPrefijo,TipoAcceso,PosHMI,Prioridad,OrigenR2,PrioridadSIP,Literal FROM Internos
				WHERE IdSectorizacion=id_sectorizacion AND
							IdSistema=id_sistema;
		REPLACE INTO TablasModificadas (IdTabla)
				VALUES ('Internos');

		INSERT into Externos
			SELECT IdSistema,id_activa as IdSectorizacion,IdNucleo,IdSector,IdDestino,IdPrefijo,TipoAcceso,PosHMI,Prioridad,OrigenR2,PrioridadSIP,Literal,Grupo FROM Externos
				WHERE IdSectorizacion=id_sectorizacion AND
							IdSistema=id_sistema;
		REPLACE INTO TablasModificadas (IdTabla)
				VALUES ('Externos');
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `CrearFicheroBackup` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `CrearFicheroBackup`(in id_sistema char(32), in tabla text, in fichero text,in fDesde datetime, in fHasta datetime)
BEGIN
DECLARE lista CHAR(55);
set @SQL = concat("SELECT *",
			"INTO OUTFILE '", fichero, "'", 
      " FIELDS TERMINATED BY ',' OPTIONALLY ENCLOSED BY '\"' LINES TERMINATED BY '\\n'",
      " FROM ", tabla,
      " WHERE IdSistema='", id_sistema,
			"' AND DATE_FORMAT(FechaHora,'%Y/%m/%d')>='",DATE_FORMAT(fDesde,'%Y/%m/%d'), 
			"' AND DATE_FORMAT(FechaHora,'%Y/%m/%d')< '",DATE_FORMAT(fHasta,'%Y/%m/%d'),
			"';"); 
PREPARE stmt FROM @SQL; 
EXECUTE stmt; 
set @SQL_DELETE = concat("DELETE FROM ", tabla,
      " WHERE IdSistema='", id_sistema,
			"' AND DATE_FORMAT(FechaHora,'%Y/%m/%d')>='",DATE_FORMAT(fDesde,'%Y/%m/%d'), 
			"' AND DATE_FORMAT(FechaHora,'%Y/%m/%d')< '",DATE_FORMAT(fHasta,'%Y/%m/%d'),
			"';"); 
PREPARE stmt_delete FROM @SQL_DELETE; 
EXECUTE stmt_delete; 
	
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `CreaSectoresActiva` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `CreaSectoresActiva`(in id_sistema char(32), in id_sectorizacion char(32), in id_activa char(32))
BEGIN
	INSERT into SectoresSectorizacion
	SELECT IdSistema,id_activa as IdSectorizacion,IdNucleo,IdSector,IdTop FROM SectoresSectorizacion
		WHERE IdSectorizacion=id_sectorizacion AND
					IdSistema=id_sistema;
					
	REPLACE INTO TablasModificadas (IdTabla)
				VALUES ('SectoresSectorizacion');
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `CreaSectoresActivaConexionSacta` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `CreaSectoresActivaConexionSacta`(in id_sistema char(32), in id_sectorizacion char(32), in id_activa char(32))
BEGIN
	INSERT into SectoresSectorizacion
		SELECT ss.IdSistema,id_activa as IdSectorizacion,ss.IdNucleo,ss.IdSector,ss.IdTOP FROM SectoresSectorizacion ss, Sectores s
		WHERE ss.IdSistema=id_sistema AND
					(ss.IdSectorizacion='SACTA' OR
						(ss.IdSectorizacion=id_sectorizacion AND 
							s.Tipo='M' AND	
							ss.IdTOP NOT IN (SELECT idtop 
																	FROM sectoressectorizacion 
																	WHERE idsistema=id_sistema AND idsectorizacion='SACTA'))) AND
					ss.IdSector=s.IdSector;
	REPLACE INTO TablasModificadas (IdTabla)
				VALUES ('SectoresSectorizacion');
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `CuantasTeclasConPrioridadUno` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `CuantasTeclasConPrioridadUno`(in id_sistema varchar(32), in id_nucleo varchar(32), out cuantas int)
BEGIN
  DECLARE iNumTeclasPrioridad int default 0;
  SET cuantas=0;
	
  -- Obtiene el número de teclas con prioridad 1 asignadas al panel de linea caliente para un sistema y nucleo
  -- Las teclas asignadas al panel de línea caliente pueden corresponder a:
  --   Destinos internos (Idprefijo=0)
  --   Destinos externos: Líneas calientes LCEN (IdPrefijo=1 y TipoAcceso='IA') y 
  --                      Destinos de telefonía ATS con comunicación IP (IdPrefijo=3 y TipoAcceso='IA')
  --  Un destino ATS se puede asignar a más de un sector, por lo que esté tipo de destino sólo se debe contar una vez
  --  aunque está asignado a varios sectores  
  
  SELECT SUM(cuenta) 
  INTO iNumTeclasPrioridad 
  FROM (SELECT COUNT(1) cuenta FROM DestinosInternosSector
        WHERE IdSistema=id_sistema AND 
              IdNucleo=id_nucleo AND
              IdPrefijo=0 AND
              Prioridad=1
        UNION ALL      
        SELECT COUNT(1) cuenta 
        FROM (SELECT 1 FROM DestinosExternosSector
              WHERE IdSistema=id_sistema AND 
                    IdNucleo=id_nucleo AND
                    (IdPrefijo=1 OR IdPrefijo=3) AND TipoDestino=1 AND TipoAcceso='IA' AND
                    Prioridad=1
              GROUP BY IdDestino) F
        ) B;          

  SET cuantas=iNumTeclasPrioridad;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `DestinosExternosAsignadosASector` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `DestinosExternosAsignadosASector`(in id_sistema char(32), in id_sector varchar(700), in telefonia BOOL)
BEGIN
	IF telefonia=true THEN
    -- Destinos externos asignados en el panel de telefonia
		IF id_sector IS NOT NULL THEN
			SELECT * FROM DestinosExternosSector
			WHERE IdSistema=id_sistema AND
						IdSector=id_sector AND
						IdPrefijo<>1 AND TipoAcceso='DA';
		ELSE
			SELECT * FROM DestinosExternosSector
			WHERE IdSistema=id_sistema AND
						IdPrefijo<>1 AND TipoAcceso='DA';
		END IF;
	ELSE
    -- Destinos externos asignados en el panel de linea caliente
    -- Sólo se pueden configurar Destinos ATS en el panel de línea caliente
    -- El destino ATS, puede tener asociado un Destino LCEN (prefijo=1)
		IF id_sector IS NOT NULL THEN
			SELECT * FROM DestinosExternosSector
			WHERE IdSistema=id_sistema AND
						IdSector=id_sector AND
						IdPrefijo=3 AND TipoAcceso='IA';
		ELSE
			SELECT * FROM DestinosExternosSector
			WHERE IdSistema=id_sistema AND
						IdPrefijo=3 AND TipoAcceso='IA';
		END IF;
	END IF;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `DestinosInstantaneosSinAsignarAEnlaces` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `DestinosInstantaneosSinAsignarAEnlaces`(in id_sistema char(32))
BEGIN
	SELECT * FROM Destinos
	WHERE IdDestino not in (SELECT IdDestino FROM EnlacesLCEN 
														WHERE IdSistema=id_sistema AND IdPrefijo in ("0","1")) AND
				IdSistema=id_sistema AND
				IdPrefijo in ("0","1");
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `DestinosInternosAsignadosASector` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `DestinosInternosAsignadosASector`(in id_sistema char(32), in id_sector varchar(700), in telefonia BOOL)
BEGIN
	IF telefonia=true THEN
		IF id_sector IS NOT NULL THEN
			SELECT * FROM DestinosInternosSector
			WHERE IdSistema=id_sistema AND
						IdSector=id_sector AND
						IdPrefijo=2;
		ELSE
			SELECT * FROM DestinosInternosSector
			WHERE IdSistema=id_sistema AND
						IdPrefijo=2;
		END IF;
	ELSE
		IF id_sector IS NOT NULL THEN
			SELECT * FROM DestinosInternosSector
			WHERE IdSistema=id_sistema AND
						IdSector=id_sector AND
						IdPrefijo=0;
		ELSE
			SELECT * FROM DestinosInternosSector
			WHERE IdSistema=id_sistema AND
						IdPrefijo=0;
		END IF;
	END IF;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `DestinosInternosSinAsignarAEnlaces` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `DestinosInternosSinAsignarAEnlaces`(in id_sistema char(32))
BEGIN
	SELECT * FROM Destinos
	WHERE IdDestino not in (SELECT IdDestino FROM EnlacesInternos 
														WHERE IdSistema=id_sistema AND IdPrefijo <> "1") AND
				IdSistema=id_sistema AND
				IdPrefijo <> "1";
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `DestinosLCEN_SinAsignar` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `DestinosLCEN_SinAsignar`(in id_sistema char(32))
BEGIN
  -- Se obtiene los destinos de LC que no están asignados en el sistema
  -- Un destino LCEN sólo se puede asignar a un único sector
  (SELECT a.* FROM destinosTelefonia a
   WHERE  a.IdSistema=id_sistema AND
		    a.IdPrefijo=1 AND
		    a.IdDestino NOT IN (SELECT IdDestinoLCEN FROM DestinosExternosSector D
								WHERE D.IdSistema=a.IdSistema AND D.TipoAcceso='IA' AND
                                      D.IdPrefijoDestinoLCEN=a.IdPrefijo));
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `DestinosLineaCalienteSinAsignarAlSector` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `DestinosLineaCalienteSinAsignarAlSector`(in id_sistema char(32), in id_usuario varchar(700), in id_nucleo char(32))
BEGIN
   -- Se obtienen los destinos externos de telefonía para el panel de línea caliente que son internos (prefijo=0) o ATS (prefijo=3)
  -- Los destinos ATS se pueden asignar a varios sectores.
  -- Se obtiene los destinos ATS con IdAbonado dentro del rango de algún encaminamiento
  	(SELECT a.* FROM destinosTelefonia a, destinosInternos b, Sectores c
			WHERE a.IdSistema=id_sistema AND
					a.IdPrefijo=0 AND
					a.IdSistema=b.IdSistema AND
					a.IdPrefijo=b.IdPrefijo AND
					a.IdDestino=b.IdDestino AND
					b.IdSector<>id_usuario AND
					b.IdNucleo=id_nucleo AND
					c.IdSector=b.IdSector AND
					c.SectorSimple=true AND
					b.IdDestino not in (SELECT IdDestino FROM DestinosInternosSector
                                                             WHERE IdSistema=id_sistema AND
                                                                   IdSector=id_usuario AND
                                                                   IdPrefijo=0) ORDER BY a.IdDestino)
	  UNION -- Se obtiene los destinos ATS con IdAbonado dentro del rango de algún encaminamiento
    (SELECT a.* from destinosTelefonia a, destinosexternos D 
     WHERE   a.IdSistema=D.IdSistema AND
             a.TipoDestino=D.TipoDestino AND
             a.IdPrefijo=D.IdPrefijo AND
             a.IdDestino=D.IdDestino AND
             a.IdSistema=id_sistema AND
             a.IdPrefijo=3 AND
             a.TipoDestino=1 AND
             EXISTS (SELECT 1 from encaminamientos E 
                     LEFT JOIN rangos as R ON  R.IdSistema=E.IdSistema AND R.Central = E.Central
                     WHERE E.IdSistema=D.IdSistema AND D.IdAbonado BETWEEN R.inicial and R.final) AND
             a.IdDestino NOT IN (SELECT IdDestino FROM DestinosExternosSector D
                                 WHERE  D.IdSistema=a.IdSistema AND D.IdPrefijo=a.IdPrefijo AND
                                        D.IdNucleo=id_nucleo AND D.IdSector=id_usuario AND D.TipoAcceso='IA') ORDER BY a.IdDestino);
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `DestinosPorGruposTelefonia` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `DestinosPorGruposTelefonia`(in id_sistema varchar(32))
BEGIN
	SELECT g.idgrupo,d.IdDestino
	FROM GruposTelefonia g, destinostelefonia d
	WHERE g.IdSistema=id_sistema AND
				d.IdSistema = g.IdSistema AND
				d.IdGrupo=g.IdGrupo
UNION
	SELECT idgrupo,null 
		FROM grupostelefonia 
		WHERE IdSistema=id_sistema AND 
					IdGrupo NOT IN (SELECT IdGrupo FROM destinostelefonia WHERE idSistema=id_sistema AND IdGrupo is not null);
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `DestinosRadioAsignadosASector` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `DestinosRadioAsignadosASector`(in id_sistema char(32), in id_usuario varchar(700), in pagina int, in frecPorPagina int)
BEGIN
	SELECT * FROM DestinosRadioSector
				WHERE Idsistema=id_sistema AND
							IdSector=id_usuario AND
							(((PosHMI-1) div frecPorPagina) = pagina);
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `DestinosRadioSectorizados` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `DestinosRadioSectorizados`(in id_sistema char(32), in id_sector varchar(700), in id_sectorizacion char(32))
BEGIN
		SELECT * FROM radio
					WHERE IdSistema=id_sistema AND
								IdSectorizacion=id_sectorizacion AND
								IdSector=id_sector;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `DestinosRadioSectorizadosParaXML` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `DestinosRadioSectorizadosParaXML`(in id_sistema char(32), in id_sectorizacion char(32))
BEGIN
	IF (id_sectorizacion IS NOT NULL) THEN
		SELECT * FROM (SELECT * FROM radio
			WHERE IdSistema=id_sistema AND
						IdSectorizacion=id_sectorizacion) AS A ORDER BY A.IdSector,A.PosHMI;
	ELSE
		SELECT * FROM (SELECT * FROM DestinosRadioSector
			WHERE IdSistema=id_sistema) AS A ORDER BY A.IdSector,A.PosHMI;
	END IF;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `DestinosRadioSinAsignarALaPaginaDelSector` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `DestinosRadioSinAsignarALaPaginaDelSector`(in id_sistema char(32), in id_usuario varchar(700), in pagina int, in frecPorPagina int)
BEGIN
	SELECT IdDestino FROM DestinosRadio DR
	WHERE idSistema=id_sistema AND
				0 < (SELECT COUNT(*) FROM RecursosRadio RR WHERE RR.IdDestino=DR.IdDestino) AND
				IdDestino not in (select IdDestino from DestinosRadioSector
														where Idsistema=id_sistema AND
																	IdSector=id_usuario AND
																	(((PosHMI-1) div frecPorPagina) = pagina));
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `DestinosTelefoniaMultiDestino` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `DestinosTelefoniaMultiDestino`(in id_sistema char(32), in id_nucleo char(32))
BEGIN
		(select a.* from destinosTelefonia a, destinosInternos b
where a.IdSistema='departamento' AND
a.IdPrefijo=2 AND 
a.IdSistema=b.IdSistema AND
a.IdPrefijo=b.IdPrefijo AND
a.IdDestino=b.IdDestino AND

b.IdNucleo='TWR-N' AND
b.IdDestino in
(SELECT s.IdSector FROM Sectores s
WHERE s.IdSistema='departamento' AND
s.SectorSimple)
ORDER BY a.IdDestino)

UNION

(select a.* from destinosTelefonia a
where a.IdSistema='departamento' AND
a.TipoDestino=1 AND 
a.IdPrefijo<>1 AND
a.IdDestino IN (SELECT IdDestino FROM DestinosExternosSector
					WHERE IdSistema='departamento' AND				    
				    TipoAcceso='DA' AND 
				    a.IdPrefijo<>1)
ORDER BY a.IdDestino);
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `DestinosTelefoniaSectorizados` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `DestinosTelefoniaSectorizados`(in id_sistema char(32), in id_sector varchar(700), in id_sectorizacion char(32), in internos BOOL)
BEGIN
	IF internos=true THEN
		SELECT * FROM internos
					WHERE IdSistema=id_sistema AND
								IdSectorizacion=id_sectorizacion AND
								IdSector=id_sector;
	ELSE
			SELECT * FROM externos
					WHERE IdSistema=id_sistema AND
								IdSectorizacion=id_sectorizacion AND
								IdSector=id_sector;
	END IF;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `DestinosTelefoniaSectorizadosParaXML` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `DestinosTelefoniaSectorizadosParaXML`(in id_sistema char(32), in id_sectorizacion char(32), in lc BOOL)
BEGIN
	IF (id_sectorizacion IS NOT NULL) THEN
		IF lc=true THEN
			SELECT * FROM ((SELECT *,null as 'Grupo' FROM internos
								WHERE IdSistema=id_sistema AND
									IdSectorizacion=id_sectorizacion AND
									IdPrefijo=0)
			UNION ALL
			(SELECT * FROM externos
					WHERE IdSistema=id_sistema AND
								IdSectorizacion=id_sectorizacion AND
								IdPrefijo=1)) as A ORDER BY IdSector,PosHMI;
		ELSE
			SELECT * FROM ((SELECT *,null as 'Grupo' FROM internos
					WHERE IdSistema=id_sistema AND
								IdSectorizacion=id_sectorizacion AND
								IdPrefijo=2)
			UNION
			(SELECT * FROM externos
					WHERE IdSistema=id_sistema AND
								IdSectorizacion=id_sectorizacion AND
								IdPrefijo<>1)) as A ORDER BY IdSector,PosHMI;
		END IF;			
	ELSE	
		IF lc=true THEN
			SELECT * FROM ((SELECT * FROM DestinosInternosSector
					WHERE IdSistema=id_sistema AND
								IdPrefijo=0)
			UNION
			(SELECT d.IdSistema,d.IdDestino,d.TipoDestino,d.IdNucleo,d.IdSector,d.IdPrefijo,d.PosHMI,d.Prioridad,d.OrigenR2,d.PrioridadSIP,d.TipoAcceso,d.Literal
					FROM DestinosExternosSector d
					WHERE IdSistema=id_sistema AND
								IdPrefijo=1)) as A ORDER BY IdSector,PosHMI;
		ELSE
			SELECT * FROM ((SELECT * FROM DestinosInternosSector
					WHERE IdSistema=id_sistema AND
								IdPrefijo=2)
			UNION
			(SELECT d.IdSistema,d.IdDestino,d.TipoDestino,d.IdNucleo,d.IdSector,d.IdPrefijo,d.PosHMI,d.Prioridad,d.OrigenR2,d.PrioridadSIP,d.TipoAcceso,d.Literal
					FROM DestinosExternosSector d
					WHERE IdSistema=id_sistema AND
								IdPrefijo<>1)) as A ORDER BY IdSector,PosHMI;
		END IF;			
	END IF;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `DestinosTelefoniaSinAsignarAlSector` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `DestinosTelefoniaSinAsignarAlSector`(in id_sistema char(32), in id_usuario varchar(700), in id_nucleo char(32))
BEGIN
		(select a.* from destinosTelefonia a, destinosInternos b
			where a.IdSistema=id_sistema AND
				a.IdPrefijo=2 AND 
				a.IdSistema=b.IdSistema AND
				a.IdPrefijo=b.IdPrefijo AND
				a.IdDestino=b.IdDestino AND
				b.IdSector<>id_usuario AND
				b.IdNucleo=id_nucleo AND
				b.IdDestino in (SELECT s.IdSector FROM Sectores s
														WHERE s.IdSistema=id_sistema AND
																	s.SectorSimple) AND
				b.IdDestino not in (SELECT IdDestino FROM DestinosInternosSector
														WHERE IdSistema=id_sistema AND
																	IdSector=id_usuario AND
																	IdPrefijo=2)
				ORDER BY a.IdDestino)
UNION
(select a.* from destinosTelefonia a
	where a.IdSistema=id_sistema AND
				a.TipoDestino=1 AND 
				a.IdPrefijo<>1 AND
				a.IdDestino not in (SELECT IdDestino FROM DestinosExternosSector
														WHERE IdSistema=id_sistema AND
															  IdSector=id_usuario AND
                                                              TipoAcceso='DA' AND 
                                                              a.IdPrefijo<>1)
				ORDER BY a.IdDestino);
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `EliminaActiva` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `EliminaActiva`(in id_sistema char(32))
BEGIN
	DECLARE id char(32);
	
	SELECT FechaActivacion INTO id FROM sectorizaciones WHERE IdSistema=id_sistema AND Activa=true;
	
	DELETE FROM Sectorizaciones
		WHERE NOT Activa AND
					IdSistema=id_sistema AND
					FechaActivacion=id;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `ExisteIP` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `ExisteIP`(in id_sistema varchar(32), in ip varchar(15), in id_hw varchar(32))
BEGIN
	select 	(select count(*) from Top where IdSistema=id_sistema AND IdTOP!=id_hw AND (IpRed1=ip OR IpRed2=ip)) +
					(select count(*) from Tifx where IdSistema=id_sistema AND IdTifx!=id_hw AND (IpRed1=ip OR IpRed2=ip)) +
					(select count(*) from EquiposEu where IdSistema=id_sistema AND idEquipos!=id_hw AND (IpRed1=ip OR IpRed2=ip));
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `FinalizaTransaccion` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `FinalizaTransaccion`()
BEGIN
		start transaction;
		
		insert ignore into cd40.sistema select * from cd40_trans.sistema;
		insert ignore into cd40.abonados select * from cd40_trans.abonados;
		
		commit;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `GeneraRegistroBackup` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `GeneraRegistroBackup`(in id_sistema char(32), in tipo_backup int, in fDesde datetime, in fHasta datetime, in recurso text)
BEGIN
	INSERT INTO registrobackup (
	   IdSistema
	  ,TipoBackup
	  ,FechaInicio
	  ,FechaFin
	  ,RecursoDestino
	) VALUES (
	   id_sistema 															
	  ,tipo_backup 															
	  ,DATE_FORMAT(fDesde,'%Y/%m/%d %H:%i:%s') 	
	  ,DATE_FORMAT(fHasta,'%Y/%m/%d %H:%i:%s') 	
	  ,recurso 																	
	);
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `GestionaFS` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `GestionaFS`(in id_sistema char(32), in id_nucleo char(32), in id_sector char(32), in SeleccionadoFS bool)
BEGIN
IF(SeleccionadoFS = true) THEN
	UPDATE Sectores 
		SET SeleccionadoFS=false;
		REPLACE INTO TablasModificadas (IdTabla)
				VALUES ('Sectores');
                END IF;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `GetEstadoCluster` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `GetEstadoCluster`()
BEGIN
	SELECT * FROM estadocluster;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `GetEventosRadio` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `GetEventosRadio`(in id_sistema varchar(32), in desde date, in hasta date)
BEGIN
	SELECT * FROM EventosRadio	
	WHERE IdSistema=id_sistema AND
				DATE_FORMAT(FechaHora,"%Y/%m/%d") >= desde AND
				DATE_FORMAT(FechaHora,"%Y/%m/%d") < hasta
	ORDER BY FechaHora;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `GetEventosTelefonia` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `GetEventosTelefonia`(in id_sistema varchar(32), in desde date, in hasta date)
BEGIN
	SELECT * FROM EventosTelefonia	
	WHERE IdSistema=id_sistema AND
				DATE_FORMAT(FechaHora,"%Y/%m/%d") >= desde AND
				DATE_FORMAT(FechaHora,"%Y/%m/%d") < hasta
	ORDER BY FechaHora;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `GetFunciones` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `GetFunciones`(in id_sistema varchar(32), in _tipo int)
BEGIN
	SELECT * FROM funciones
	WHERE IdSistema=id_sistema AND
				Tipo=_tipo;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `GetHistoricos` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `GetHistoricos`(in id_sistema varchar(32),in num_scv int, in lista_incidencias text,in tipo_hw int, in id_hw varchar(32), in desde datetime, in hasta datetime)
BEGIN
	declare i int;
	declare pos int;
	declare cadena text;
	set i=LOCATE(",",lista_incidencias);
	set pos=1;
	create temporary table if not exists t SELECT hi.*, i.Descripcion AS Descriptor FROM HistoricoIncidencias hi, incidencias i where false;
	IF (lista_incidencias<>"") THEN												
		IF (id_hw="") THEN
			IF (tipo_hw=-1) THEN																		
				WHILE i>0 DO
					set cadena=substring(lista_incidencias,pos,i-pos);
					set pos=i+1;
					set i=LOCATE(",",lista_incidencias,pos);
		
					INSERT INTO t 
					SELECT hi.*, i.Descripcion FROM HistoricoIncidencias hi, incidencias i
						WHERE hi.IdSistema=id_sistema AND
							hi.IdIncidencia=cadena AND 
							hi.FechaHora >= desde AND
							hi.FechaHora < hasta AND 
							i.IdIncidencia=hi.IdIncidencia;
					END WHILE;
			ELSE
				WHILE i>0 DO
					set cadena=substring(lista_incidencias,pos,i-pos);
					set pos=i+1;
					set i=LOCATE(",",lista_incidencias,pos);
		
					INSERT INTO t 
					SELECT hi.*, i.Descripcion FROM HistoricoIncidencias hi, incidencias i
						WHERE hi.IdSistema=id_sistema AND
							hi.TipoHw = tipo_hw AND
							hi.IdIncidencia=cadena AND 
							hi.FechaHora >= desde AND
							hi.FechaHora < hasta AND 
							i.IdIncidencia=hi.IdIncidencia;
					END WHILE;
			END IF;
		ELSE
			IF (tipo_hw=-1) THEN																		
				WHILE i>0 DO
					set cadena=substring(lista_incidencias,pos,i-pos);
					set pos=i+1;
					set i=LOCATE(",",lista_incidencias,pos);
		
					INSERT INTO t 
					SELECT hi.*, i.Descripcion FROM HistoricoIncidencias hi, incidencias i
						WHERE hi.IdSistema=id_sistema AND
							hi.IdHw=id_hw AND 
							hi.IdIncidencia=cadena AND 
							hi.FechaHora >= desde AND
							hi.FechaHora < hasta AND 
							i.IdIncidencia=hi.IdIncidencia;
					END WHILE;
			ELSE
				WHILE i>0 DO
					set cadena=substring(lista_incidencias,pos,i-pos);
					set pos=i+1;
					set i=LOCATE(",",lista_incidencias,pos);
		
					INSERT INTO t 
					SELECT hi.*, i.Descripcion FROM HistoricoIncidencias hi, incidencias i
						WHERE hi.IdSistema=id_sistema AND
							hi.IdHw=id_hw AND 
							hi.TipoHw=tipo_hw AND 
							hi.IdIncidencia=cadena AND 
							hi.FechaHora >= desde AND
							hi.FechaHora < hasta AND 
							i.IdIncidencia=hi.IdIncidencia;
					END WHILE;
			END IF;
		END IF;
	ELSE
		IF (id_hw="") THEN
			IF (tipo_hw=-1) THEN																		
				INSERT INTO t 
					SELECT hi.*,i.Descripcion FROM HistoricoIncidencias hi, incidencias i WHERE hi.IdSistema=id_sistema AND 
  							hi.FechaHora >= desde AND
	  						hi.FechaHora < hasta AND 
								i.IdIncidencia=hi.IdIncidencia;
			ELSE
				INSERT INTO t 
					SELECT hi.*, i.Descripcion FROM HistoricoIncidencias  hi, incidencias i
						WHERE hi.IdSistema=id_sistema AND
									hi.TipoHw=tipo_hw AND 
    							hi.FechaHora >= desde AND
    							hi.FechaHora < hasta AND 
									i.IdIncidencia=hi.IdIncidencia;
			END IF;
		ELSE
			IF (tipo_hw=-1) THEN																		
				INSERT INTO t 
					SELECT hi.*, i.Descripcion FROM HistoricoIncidencias hi, incidencias i
						WHERE hi.IdSistema=id_sistema AND
							hi.IdHw=id_hw AND 
							hi.FechaHora >= desde AND
							hi.FechaHora < hasta AND 
							i.IdIncidencia=hi.IdIncidencia;
			ELSE
				INSERT INTO t 
					SELECT hi.*, i.Descripcion FROM HistoricoIncidencias hi, incidencias i
						WHERE hi.IdSistema=id_sistema AND
							hi.TipoHw=tipo_hw AND
							hi.IdHw=id_hw AND 
							hi.FechaHora >= desde AND
							hi.FechaHora < hasta AND 
							i.IdIncidencia=hi.IdIncidencia;
			END IF;
		END IF;
	END IF;
	IF (num_scv=0) THEN
		SELECT * FROM t				
			ORDER BY -FechaHora;
	ELSE
		SELECT * FROM t				
			WHERE Scv=Num_scv
			ORDER BY -FechaHora;
	END IF;
		
	DROP TABLE t;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `GetMtbf` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `GetMtbf`(in id_sistema varchar(32), in desde date, in hasta date)
BEGIN
	SELECT COUNT(*) AS mtbf 
		FROM historicoincidencias 
		WHERE DATE_FORMAT(FechaHora,"%Y/%m/%d")>=desde AND 
				DATE_FORMAT(FechaHora,"%Y/%m/%d")<hasta AND
				idIncidencia in (select idIncidencia from incidencias i where i.GeneraError=1 AND i.IdIncidenciaCorrectora is not null);
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `GetMtbfComponents` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `GetMtbfComponents`(in id_sistema varchar(32), in desde date, in hasta date, in components varchar(125))
BEGIN
	SELECT COUNT(*) AS mtbf 
		FROM historicoincidencias 
		WHERE DATE_FORMAT(FechaHora,"%Y/%m/%d")>=desde AND 
				DATE_FORMAT(FechaHora,"%Y/%m/%d")<hasta AND
                FIND_IN_SET(IdHw,components) > 0 AND
				idIncidencia in (select idIncidencia from incidencias i where i.GeneraError=1 AND i.IdIncidenciaCorrectora is not null);
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `GetMtbfEvents` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `GetMtbfEvents`(in id_sistema varchar(32), in desde date, in hasta date, in evento int)
BEGIN
	SELECT COUNT(*) AS mtbf 
		FROM historicoincidencias 
		WHERE DATE_FORMAT(FechaHora,"%Y/%m/%d")>=desde AND 
				DATE_FORMAT(FechaHora,"%Y/%m/%d")<hasta AND
                idIncidencia = evento;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `GetRate` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `GetRate`(in id_sistema varchar(32), in desde date, in hasta date)
BEGIN
SELECT
(SELECT COUNT(*) AS fallos 
	FROM historicoincidencias 
    WHERE DATE_FORMAT(FechaHora,"%Y/%m/%d")>=desde AND 
			DATE_FORMAT(FechaHora,"%Y/%m/%d")<hasta AND
            idIncidencia in (select idIncidencia from incidencias i where i.GeneraError=1 AND i.IdIncidenciaCorrectora is not null))
/
(SELECT COUNT(*) AS total
	FROM historicoincidencias 
    WHERE DATE_FORMAT(FechaHora,"%Y/%m/%d")>=desde AND 
			DATE_FORMAT(FechaHora,"%Y/%m/%d")<hasta) * 100 AS rate;

END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `GetRateComponents` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `GetRateComponents`(in id_sistema varchar(32), in desde date, in hasta date, in components text)
BEGIN
SELECT
(SELECT COUNT(*) AS fallos 
	FROM historicoincidencias 
    WHERE DATE_FORMAT(FechaHora,"%Y/%m/%d")>=desde AND 
			DATE_FORMAT(FechaHora,"%Y/%m/%d")<hasta  AND
			FIND_IN_SET(IdHw,components) > 0 AND
            idIncidencia in (select idIncidencia from incidencias i where i.GeneraError=1 AND i.IdIncidenciaCorrectora is not null))
/
(SELECT COUNT(*) AS total
	FROM historicoincidencias 
    WHERE DATE_FORMAT(FechaHora,"%Y/%m/%d")>=desde  AND
			FIND_IN_SET(IdHw,components) > 0 AND
			DATE_FORMAT(FechaHora,"%Y/%m/%d")<hasta) * 100 AS rate;

END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `GetRateEvents` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `GetRateEvents`(in id_sistema varchar(32), in desde date, in hasta date, in evento int)
BEGIN
	SELECT
	(SELECT COUNT(*) AS fallos 
		FROM historicoincidencias 
		WHERE DATE_FORMAT(FechaHora,"%Y/%m/%d")>=desde AND 
				DATE_FORMAT(FechaHora,"%Y/%m/%d")<hasta  AND
				idIncidencia = evento)
	/
	(SELECT COUNT(*) AS total
		FROM historicoincidencias 
		WHERE   idIncidencia in (select idIncidencia from incidencias i where i.GeneraError=1 AND i.IdIncidenciaCorrectora is not null) AND
				DATE_FORMAT(FechaHora,"%Y/%m/%d")>=desde  AND
				DATE_FORMAT(FechaHora,"%Y/%m/%d")<hasta) * 100 AS rate;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `GetUnHistorico` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `GetUnHistorico`(in id_sistema varchar(32),in lista_incidencias text,in tipo_hw int, in id_hw varchar(32), in desde date, in hasta date)
BEGIN
	declare i int;
	declare pos int;
	declare cadena text;
	set i=LOCATE(",",lista_incidencias);
	set pos=1;
	create temporary table if not exists t SELECT hi.*, i.Descripcion AS Descriptor FROM HistoricoIncidencias hi, incidencias i where false;
	IF (lista_incidencias<>"") THEN												
		IF (id_hw="") THEN
			IF (tipo_hw=-1) THEN																		
				WHILE i>0 DO
					set cadena=substring(lista_incidencias,pos,i-pos);
					set pos=i+1;
					set i=LOCATE(",",lista_incidencias,pos);
		
					INSERT INTO t 
					SELECT hi.*, i.Descripcion FROM HistoricoIncidencias hi, incidencias i
						WHERE hi.IdSistema=id_sistema AND
							hi.IdIncidencia=cadena AND 
							DATE_FORMAT(hi.FechaHora,"%Y/%m/%d") >= desde AND
							DATE_FORMAT(hi.FechaHora,"%Y/%m/%d") < hasta AND
							i.IdIncidencia=hi.IdIncidencia
						LIMIT 1;
					END WHILE;
			ELSE
				WHILE i>0 DO
					set cadena=substring(lista_incidencias,pos,i-pos);
					set pos=i+1;
					set i=LOCATE(",",lista_incidencias,pos);
		
					INSERT INTO t 
					SELECT hi.*, i.Descripcion FROM HistoricoIncidencias hi, incidencias i
						WHERE hi.IdSistema=id_sistema AND
							hi.TipoHw = tipo_hw AND
							hi.IdIncidencia=cadena AND 
							DATE_FORMAT(hi.FechaHora,"%Y/%m/%d") >= desde AND
							DATE_FORMAT(hi.FechaHora,"%Y/%m/%d") < hasta AND
							i.IdIncidencia=hi.IdIncidencia
						LIMIT 1;
					END WHILE;
			END IF;
		ELSE
			IF (tipo_hw=-1) THEN																		
				WHILE i>0 DO
					set cadena=substring(lista_incidencias,pos,i-pos);
					set pos=i+1;
					set i=LOCATE(",",lista_incidencias,pos);
		
					INSERT INTO t 
					SELECT hi.*, i.Descripcion FROM HistoricoIncidencias hi, incidencias i
						WHERE hi.IdSistema=id_sistema AND
							hi.IdHw=id_hw AND 
							hi.IdIncidencia=cadena AND 
							DATE_FORMAT(hi.FechaHora,"%Y/%m/%d") >= desde AND
							DATE_FORMAT(hi.FechaHora,"%Y/%m/%d") < hasta AND
							i.IdIncidencia=hi.IdIncidencia
						LIMIT 1;
					END WHILE;
			ELSE
				WHILE i>0 DO
					set cadena=substring(lista_incidencias,pos,i-pos);
					set pos=i+1;
					set i=LOCATE(",",lista_incidencias,pos);
		
					INSERT INTO t 
					SELECT hi.*, i.Descripcion FROM HistoricoIncidencias hi, incidencias i
						WHERE hi.IdSistema=id_sistema AND
							hi.IdHw=id_hw AND 
							hi.TipoHw=tipo_hw AND 
							hi.IdIncidencia=cadena AND 
							DATE_FORMAT(hi.FechaHora,"%Y/%m/%d") >= desde AND
							DATE_FORMAT(hi.FechaHora,"%Y/%m/%d") < hasta AND
							i.IdIncidencia=hi.IdIncidencia
						LIMIT 1;
					END WHILE;
			END IF;
		END IF;
	ELSE
		IF (id_hw="") THEN
			IF (tipo_hw=-1) THEN																		
				INSERT INTO t 
					SELECT hi.*,i.Descripcion FROM HistoricoIncidencias hi, incidencias i WHERE hi.IdSistema=id_sistema AND 
								DATE_FORMAT(hi.FechaHora,"%Y/%m/%d") >= desde AND
								DATE_FORMAT(hi.FechaHora,"%Y/%m/%d") < hasta AND
								i.IdIncidencia=hi.IdIncidencia
						LIMIT 1;
			ELSE
				INSERT INTO t 
					SELECT hi.*, i.Descripcion FROM HistoricoIncidencias  hi, incidencias i
						WHERE hi.IdSistema=id_sistema AND
									hi.TipoHw=tipo_hw AND 
									DATE_FORMAT(hi.FechaHora,"%Y/%m/%d") >= desde AND
									DATE_FORMAT(hi.FechaHora,"%Y/%m/%d") < hasta AND
								i.IdIncidencia=hi.IdIncidencia
						LIMIT 1;
			END IF;
		ELSE
			IF (tipo_hw=-1) THEN																		
				INSERT INTO t 
					SELECT hi.*, i.Descripcion FROM HistoricoIncidencias hi, incidencias i
						WHERE hi.IdSistema=id_sistema AND
							hi.IdHw=id_hw AND 
							DATE_FORMAT(hi.FechaHora,"%Y/%m/%d") >= desde AND
							DATE_FORMAT(hi.FechaHora,"%Y/%m/%d") < hasta AND
							i.IdIncidencia=hi.IdIncidencia
						LIMIT 1;
			ELSE
				INSERT INTO t 
					SELECT hi.*, i.Descripcion FROM HistoricoIncidencias hi, incidencias i
						WHERE hi.IdSistema=id_sistema AND
							hi.TipoHw=tipo_hw AND
							hi.IdHw=id_hw AND 
							DATE_FORMAT(hi.FechaHora,"%Y/%m/%d") >= desde AND
							DATE_FORMAT(hi.FechaHora,"%Y/%m/%d") < hasta AND
							i.IdIncidencia=hi.IdIncidencia
						LIMIT 1;
			END IF;
		END IF;
	END IF;
	SELECT * FROM t				
		ORDER BY -FechaHora LIMIT 1;
		
	DROP TABLE t;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `IdManttoLibre` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `IdManttoLibre`(in id_Sistema varchar(32), out id_Sacta int)
BEGIN
	DECLARE encontrado bool default false;
	DECLARE nSacta int default 10000;
	DECLARE id int;
	
	set id_Sacta=0;
	
	WHILE ((not encontrado) AND (nSacta < 20000)) DO
		SELECT COUNT(*) INTO id FROM sectores WHERE IdSistema=id_sistema AND NumSacta=nSacta;
		IF (id=0) THEN
			SET encontrado=true;
			SET id_Sacta=nSacta;
		ELSE
			SET nSacta=nSacta+1;
		END IF;
	END WHILE;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `IniciaTransaccion` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `IniciaTransaccion`()
BEGIN
		create table if not exists cd40_trans.sistema like cd40.sistema;
		create table if not exists cd40_trans.abonados like cd40.abonados;
		
		start transaction;
		
		delete from cd40_trans.sistema;
		delete from cd40_trans.abonados;
		
		insert into cd40_trans.sistema select * from cd40.sistema;
		insert into cd40_trans.abonados select * from cd40.abonados;
		
		commit;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `ListaRecursosDestino` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `ListaRecursosDestino`(in id_sistema varchar(32), in id_destino varchar(700), in id_prefijo int, in id_dependencia varchar(32))
BEGIN
	IF (id_prefijo=1) THEN
    IF (id_dependencia='') THEN
  		SELECT rLC.*,r.Interface FROM recursoslcen rLC, recursos r
  		WHERE rLC.IdSistema=id_sistema AND
  					rLC.IdDestino=id_destino AND
  					rLC.IdPrefijo=id_prefijo AND
  					r.IdSistema=rLC.IdSistema AND
  					r.IdRecurso=rLC.IdRecurso AND
  					r.TipoRecurso=rLC.TipoRecurso;
    ELSE
    SELECT rLC.*,r.Interface,gr.IdGrupo FROM recursoslcen rLC, recursos r, destinostelefonia gr
		WHERE rLC.IdSistema=id_sistema AND
          gr.IdGrupo=id_dependencia AND
					rLC.IdDestino=gr.IdDestino AND
					rLC.IdPrefijo=1 AND
					r.IdSistema=rLC.IdSistema AND
					r.IdRecurso=rLC.IdRecurso AND
					r.TipoRecurso=rLC.TipoRecurso;
    END IF;
	ELSE
		SELECT rTf.*,r.Interface FROM recursostf rTf, recursos r
		WHERE rTf.IdSistema=id_sistema AND
					rTf.IdDestino=id_destino AND
					rTf.IdPrefijo=id_prefijo AND
					r.IdSistema=rTf.IdSistema AND
					r.IdRecurso=rTf.IdRecurso AND
					r.TipoRecurso=rTf.TipoRecurso;
	END IF;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `ListaRecursosLCEN_DestinoATS` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `ListaRecursosLCEN_DestinoATS`(IN id_sistema varchar(32),IN id_sectorizacion varchar(32),IN id_Nucleo varchar(32),IN id_Sector varchar(700), IN id_Grupo varchar(80))
BEGIN
  -- Devuelve la lista de recursos LCEN (Destinos LCEN con idPrefijo=1) asignados a un destino ATS en el panel de línea caliente
  -- (TipoAcceso='IA') del sector idSector en la sectorización activa o en la sectorización id_sectorización
  -- Cuando un destino ATS tiene asociado destinos LCEN, los destinos tienen configurado el mismo grupo.
  
  IF (id_sistema is not null AND id_Grupo is not null AND id_Sector is not null AND id_Nucleo is not null) then
  
    IF (id_sectorizacion is not null) then
      SELECT rLC.*,r.Interface,E.grupo 
			FROM recursoslcen rLC
			JOIN recursos R ON R.IdSistema=rLC.IdSistema AND 
							           R.IdRecurso=rLC.IdRecurso AND 
							           R.TipoRecurso=rLC.TipoRecurso 
			JOIN externos E ON E.IdSistema=rLC.IdSistema AND E.IdDestino=rLC.IdDestino  AND 
							   E.Idprefijo=rLC.IdPrefijo AND 
                               E.IdSector=id_Sector AND
                               E.IdSectorizacion=id_sectorizacion AND
                               E.IdNucleo=id_Nucleo AND
                               E.tipoAcceso='IA' 
			WHERE 
				 rLC.IdSistema=id_sistema AND 
				 rLC.IdPrefijo=1 AND
				 E.grupo=id_Grupo;
    ELSE
      SELECT rLC.*,r.Interface,E.grupo 
        FROM recursoslcen rLC
        JOIN recursos R ON R.IdSistema=rLC.IdSistema AND 
                   R.IdRecurso=rLC.IdRecurso AND 
                   R.TipoRecurso=rLC.TipoRecurso 
        JOIN externos E ON E.IdSistema=rLC.IdSistema AND E.IdDestino=rLC.IdDestino  AND 
                           E.Idprefijo=rLC.IdPrefijo AND E.tipoAcceso='IA' AND 
                           E.IdSector=id_Sector AND E.IdNucleo=id_Nucleo AND
                           EXISTS (SELECT 1 FROM sectorizaciones st 
                                         WHERE st.Activa=TRUE AND 
                                               st.IdSistema=e.IdSistema AND 
                                               E.IdSectorizacion=DATE_FORMAT(st.FECHAACTIVACION,'%e/%m/%Y %H:%i:%S'))
        WHERE 
           rLC.IdSistema=id_sistema AND 
           rLC.IdPrefijo=1 AND
           E.grupo=id_Grupo;
    END IF;
  END IF;
  
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `ListaRutas` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `ListaRutas`(in id_Sistema char(32), in id_Central char(32))
BEGIN
	select IdRuta as IdRuta,Tipo as Tipo from Rutas 
		where IdSistema=id_Sistema AND
				Central=id_Central
		ORDER BY Orden;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `LoginTop` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `LoginTop`(in id_Sistema char(32),in id_Hw char(32), out id_Usuario char(32), out modo_arranque char(1))
BEGIN
	SELECT b.idsector,c.ModoArranque  INTO id_Usuario, modo_arranque
	FROM sectorizaciones a, SectoresSectorizacion b, top c
	WHERE a.idSistema=id_Sistema AND
				c.IdSistema=id_Sistema AND
				a.Activa=true AND
				b.idSistema=id_Sistema AND
				a.idSectorizacion=b.idSectorizacion AND
				b.idTOP=id_Hw AND
				b.IdTop=c.IdTOP;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `MultiDestinosAsignadosAlSector` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `MultiDestinosAsignadosAlSector`(in id_sistema char(32), in id_sector varchar(700))
BEGIN
	SELECT * FROM multidestinos
	WHERE IdSistema=id_sistema AND
			IdSector=id_sector AND
			PosHMI<>0;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `NivelesIntrusion` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `NivelesIntrusion`(in id_sistema char(32), in id_usuario varchar(700))
BEGIN
	SELECT n.* FROM Niveles n, SectoresSector ss
	WHERE ss.IdSistema=id_sistema AND
				ss.IdSector=id_usuario AND
				n.IdSistema=id_sistema AND
				n.IdSector=ss.IdSectorOriginal AND
				ss.EsDominante;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `NivelesIntrusionParaXML` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `NivelesIntrusionParaXML`(in id_sistema varchar(32), in id_sectorizacion varchar(32))
BEGIN
	IF (id_sectorizacion IS NOT NULL) THEN
		SELECT c.IdSector, n.* FROM Niveles n, SectoresSector ss, sectoressectorizacion c
			WHERE ss.IdSistema=id_sistema AND
				c.IdSectorizacion=id_sectorizacion AND
				c.IdSistema=ss.IdSistema AND
				n.IdSistema=ss.IdSistema AND
				n.IdSector=ss.IdSectorOriginal AND
				c.IdSector=ss.IdSector AND
				ss.EsDominante;
	ELSE
		SELECT n.* FROM Niveles n, Sectores s
			WHERE	s.IdSistema=id_sistema AND
						s.SectorSimple AND
						n.IdSistema=s.IdSistema AND
						n.IdSector=s.IdSector AND
						n.IdNucleo=s.IdNucleo;
	END IF;	
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `NumerosAbonadoExternos` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `NumerosAbonadoExternos`(in idSistema char(32), in idUsuario char(32))
BEGIN
	SELECT a.*
		FROM UsuariosAbonados a, Sectorizaciones b
		WHERE b.idSistema=idSistema AND
					b.Activa=true AND
					a.idSectorizacion=b.idSectorizacion AND
					a.idSistema=b.idSistema AND
					a.idSector=idUsuario AND
					a.idPrefijos<>1;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `NumerosAbonadoInternos` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `NumerosAbonadoInternos`(in id_Sistema char(32),in id_Usuario char (32))
BEGIN
	SELECT *
		FROM UsuariosAbonados
		WHERE IdSistema=id_Sistema AND
					IdSector=id_Usuario;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `NumerosAbonadosSectorAgrupado` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `NumerosAbonadosSectorAgrupado`(in id_sistema char(32), in id_sector_agrupado varchar(700))
BEGIN
	SELECT UA.* FROM UsuariosAbonados ua, SectoresAgrupacion ss
	WHERE ss.IdSistema=id_sistema AND
				ua.IdSistema=ss.IdSistema AND
				ss.IdAgrupacion=id_sector_agrupado AND
				ss.IdSector=ua.IdSector;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `NumerosAbonadosSectorNoAgrupado` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `NumerosAbonadosSectorNoAgrupado`(in id_sistema char(32), in id_nucleo char(32), in id_sector_agrupado varchar(700))
BEGIN
	SELECT UA.* FROM UsuariosAbonados ua, SectoresSector ss
	WHERE ss.IdSistema=id_sistema AND	ss.IdNucleo=id_nucleo AND
				ua.IdSistema=ss.IdSistema AND ua.IdNucleo=ss.IdNucleo AND
				ss.IdSector=id_sector_agrupado AND
				ss.IdSectorOriginal=ua.IdSector;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `ParametrosSectorAgrupado` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `ParametrosSectorAgrupado`(in id_sistema char(32), in id_sector_agrupado char(32))
BEGIN
		SELECT ps.* FROM SectoresAgrupacion ss, ParametrosSector ps
		WHERE ss.IdSistema=id_sistema AND
					ps.IdSistema=ss.IdSistema AND
					ss.IdAgrupacion=id_sector_agrupado AND
					ss.IdSector=ps.IdSector;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `ParametrosSectores` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `ParametrosSectores`(in id_sistema varchar(32), in lista_nucleos varchar(255), in lista_sectores varchar(700))
BEGIN
	declare i int;
	declare pos int;
	declare nucleo text;
	declare j int;
	declare posj int;
	declare sector text;
	set i=LOCATE(",",lista_nucleos);
	set pos=1;
	create temporary table if not exists ps SELECT * FROM ParametrosSector where false;
	WHILE i>0 DO
		set j=LOCATE(",",lista_sectores);
		set posj=1;
		set nucleo=substring(lista_nucleos,pos,i-pos);
		set pos=i+1;
		set i=LOCATE(",",lista_nucleos,pos);
		WHILE j>0 DO
			set sector=substring(lista_sectores,posj,j-posj);
			set posj=j+1;
			set j=LOCATE(",",lista_sectores,posj);
			INSERT INTO ps
			SELECT * FROM ParametrosSector
				WHERE IdSistema=id_sistema AND
							IdNucleo=nucleo AND
							IdSector=sector;
					
			END WHILE;
		END WHILE;
		
		SELECT * FROM ps;
		DROP TABLE ps;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `ParametrosSectoresParaXML` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `ParametrosSectoresParaXML`(in id_sistema varchar(32), in id_sectorizacion varchar(32))
BEGIN
	IF (id_sectorizacion IS NOT NULL) THEN
		SELECT c.IdSector, n.* FROM ParametrosSector n, SectoresSector ss, sectoressectorizacion c
		WHERE ss.IdSistema=id_sistema AND
				c.IdSectorizacion=id_sectorizacion AND
				c.IdSistema=ss.IdSistema AND
				n.IdSistema=ss.IdSistema AND
				n.IdSector=ss.IdSectorOriginal AND
				c.IdSector=ss.IdSector AND
				ss.EsDominante;
	ELSE
		SELECT n.* FROM ParametrosSector n, Sectores s
			WHERE	s.IdSistema=id_sistema AND
						s.SectorSimple AND
						n.IdSistema=s.IdSistema AND
						n.IdSector=s.IdSector AND
						n.IdNucleo=s.IdNucleo;
	END IF;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `ParametrosSectorNoAgrupado` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `ParametrosSectorNoAgrupado`(in id_sistema char(32), in id_sector_agrupado varchar(700))
BEGIN
	SELECT ps.* FROM SectoresSector ss, ParametrosSector ps
		WHERE ss.IdSistema=id_sistema AND
					ps.IdSistema=ss.IdSistema AND
					ss.IdSector=id_sector_agrupado AND
					ss.IdSectorOriginal=ps.IdSector;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `PermisosRedes` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `PermisosRedes`(in idSistema char(32), in idUsuario varchar(700))
BEGIN
    -- Obtiene los permisos de las redes con prefijo >3
	SELECT r.Idred, pr.Llamar, pr.Recibir,pr.IdRecursoPref,pr.TipoRecursoPref
    FROM redes r
    LEFT OUTER JOIN permisosredes pr ON
        r.IdSistema = pr.IdSistema AND
        r.IdRed = pr.IdRed   AND
        r.IdSistema = idSistema AND 
        pr.IdSector = idUsuario
     WHERE r.IdPrefijo>3;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `PlanDireccionamientoIP` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `PlanDireccionamientoIP`(in id_Sistema char(32))
BEGIN
 -- Obtiene el plan de direccionamiento IP.
 -- Si el equipo externo de telefonía pertenece a una central IP (tipoEquipo=3 y Min=Max=-1) ,EsCentralIP toma el valor true
 -- Si el equipo externo de telefonía no pertenece a una central Ip (tipoEquipo=3 y Min!=-1 y  Max!=-1) , se devuelven las direcciones IP 
 -- con el puerto sip configurado en el equipo externo IpRed1:PuertoSip, IpRex2:PuertoSip.
 
 -- Devuelve el identificador, tipoequipo, dirección IP 1, dirección IP2, Es Central telefonía interna (interno),
 -- valor máximo y mínimo del rango si se trata de una central, EsCentralIP=true si el equipo externo pertenece   
 -- a una central (encaminamient) IP
 -- Si el equipo esterno es central IP, puede tener configurados los campos IpRed3, SrvPresenciaIpRed1,
 -- SrvPresenciaIpRed2, SrvPresenciaIpRed3
 
	(SELECT IdTop as Id,0 as TipoEH, IpRed1, IpRed2, 0 as Interno, 0 as Min, 0 as Max, 0 as EsCentralIP,null as IpRed3,
            null as SrvPresenciaIpRed1, null as SrvPresenciaIpRed2, null as SrvPresenciaIpRed3
		FROM TOP
		WHERE IdSistema=id_Sistema)
	UNION
	(SELECT t.IdTifX as Id,1 as TipoEH , g.IpRed, t.IpRed2, 0 as Interno, 0 as Min, 0 as Max,0 as EsCentralIP,null as IpRed3,
        null as SrvPresenciaIpRed1, null as SrvPresenciaIpRed2, null as SrvPresenciaIpRed3
		FROM TIFX t, GWACTIVAS g
		WHERE t.IdSistema=id_Sistema AND
                      g.IdSistema = t.IdSistema AND
                      g.IdTifx = t.IdTIFX)
	UNION
	(SELECT IdEquipos as Id,TipoEquipo as TipoEH , 
          (CASE WHEN eu.TipoEquipo=3 and eu.Min!= -1 and eu.Max!=-1 AND eu.IpRed1 IS NOT NULL AND eu.SipPort IS NOT NULL THEN CONCAT(IpRed1,':',eu.SipPort) ELSE IpRed1 END) as IpRed1,
		      (CASE WHEN eu.TipoEquipo=3 and eu.Min!= -1 and eu.Max!=-1 AND eu.IpRed2 IS NOT NULL AND eu.SipPort IS NOT NULL THEN CONCAT(IpRed2,':',eu.SipPort) ELSE IpRed2 END) as IpRed2,
          Interno, r.Inicial as Min, r.Final as Max,
          (CASE WHEN eu.TipoEquipo=3 and eu.Min= -1 and eu.Max=-1 THEN true ELSE false END) as EsCentralIP,
          IpRed3,SrvPresenciaIpRed1,SrvPresenciaIpRed2,SrvPresenciaIpRed3
		FROM EQUIPOSEU eu
        LEFT JOIN RANGOS r ON  r.IdSistema = eu.IdSistema AND r.Central = eu.IdEquipos
		WHERE eu.IdSistema=id_Sistema);
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `PlanDireccionamientoSIP` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `PlanDireccionamientoSIP`(in id_Sistema char(32))
BEGIN
-- Obtiene el plan de direccionamiento SIP
-- Los registros deben ir ordenados por IdAgrupación (c.IdSector) y IdSector (ss.IdSectorOriginal)
-- Para que el Web service GetPlanDireccionamientoSIP muestre dentro del mismo sector
-- los número de abonados a los que atiende
SELECT a.IdPrefijo,a.IdAbonado,ss.IdSectorOriginal as IdSector,c.IdSector as IdAgrupacion
	FROM Sectorizaciones b, SectoresSectorizacion c
	inner join SectoresSector ss ON ss.IdSector= c.IdSector
	inner join UsuariosAbonados a ON a.IdSector=IdSectorOriginal
	WHERE b.IdSistema=id_Sistema AND
						b.Activa=true AND
						b.IdSectorizacion=c.IdSectorizacion
	ORDER BY  c.IdSector,ss.IdSectorOriginal,a.idPrefijo ASC, a.idAbonado ASC;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `PosicionOcupadaEnSector` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `PosicionOcupadaEnSector`(in id_sistema char(32), in id_nucleo char(32), in id_sector varchar(700), in tipo_acceso char(32), in posicion int, out existe boolean)
BEGIN
	DECLARE existeInterno,existeExterno int;
	
	SELECT count(*) INTO existeInterno
		FROM DestinosInternosSector dis
		WHERE dis.IdSistema=id_sistema AND
					dis.IdNucleo=id_nucleo AND
					dis.IdSector=id_sector AND
					dis.TipoAcceso=tipo_acceso AND
					dis.PosHMI=posicion;
				
	SELECT count(*) INTO existeExterno
		FROM DestinosExternosSector dis
		WHERE dis.IdSistema=id_sistema AND
					dis.IdNucleo=id_nucleo AND
					dis.IdSector=id_sector AND
					dis.TipoAcceso=tipo_acceso AND
					dis.PosHMI=posicion;				
					
	SET existe=((existeInterno>0) OR (existeExterno>0));
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `PrefijosSinAsignarARedes` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `PrefijosSinAsignarARedes`(in id_Sistema char(32))
BEGIN
	select IdPrefijo from Prefijos
	where IdSistema=id_Sistema AND
				IdPrefijo > 3 AND IdPrefijo < 32 AND
				IdPrefijo not in (select IdPrefijo from Redes where IdSistema=id_Sistema);
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `PrimeraPosicionLibre` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `PrimeraPosicionLibre`(in id_sistema char(32), in id_nucleo char(32), in id_sector varchar(700), in tipo_acceso char(2), out hueco int)
BEGIN
	DECLARE i int;
	DECLARE huecoInterno, huecoExterno int;
	DECLARE encontrado boolean;
	SET i=0;
	SET encontrado=false;
	SET huecoInterno=0;
	SET huecoExterno=0;
	WHILE (NOT encontrado) AND (i<56) DO
		SET i=i+1;
		SELECT COUNT(*) INTO huecoInterno
			FROM DestinosInternosSector d
			WHERE d.IdSistema=id_sistema AND
						d.IdNucleo=id_nucleo AND
						d.IdSector=id_sector AND
						d.TipoAcceso=tipo_acceso AND
						d.PosHMI=i;
						
		SELECT COUNT(*) INTO huecoExterno
			FROM DestinosExternosSector d
			WHERE d.IdSistema=id_sistema AND
						d.IdNucleo=id_nucleo AND
						d.IdSector=id_sector AND
						d.TipoAcceso=tipo_acceso AND
						d.PosHMI=i;
						
		SET encontrado=((huecoInterno=0) AND (huecoExterno=0));
	END WHILE;
	SET hueco=i;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `RangosConIdRed` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `RangosConIdRed`(in id_sistema varchar(32), in id_central varchar(32))
BEGIN
	IF (id_central is not null) THEN
		SELECT r.*, red.IdRed
		FROM 	Rangos r
        LEFT JOIN Redes red ON red.IdSistema=r.IdSistema AND r.IdPrefijo = red.IdPrefijo
		WHERE	r.IdSistema=id_sistema AND
					r.Central=id_central;
	ELSE
		SELECT r.*, red.IdRed
		FROM 	Rangos r
        LEFT JOIN Redes red ON red.IdSistema=r.IdSistema AND r.IdPrefijo = red.IdPrefijo
		WHERE	r.IdSistema=id_sistema;
	END IF;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `RecursosLCENSinAsignarAEnlaces` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `RecursosLCENSinAsignarAEnlaces`(in id_sistema char(32))
BEGIN
		SELECT * FROM RecursosLCEN
		WHERE IdSistema=id_sistema AND
					IdDestino is null;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `RecursosLcParaInformeXML` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `RecursosLcParaInformeXML`(in id_sistema varchar(32))
BEGIN
	SELECT r.*, rt.*,pr.*
	FROM recursos r, recursoslcen rt, parametrosRecurso pr
	WHERE r.IdSistema=id_sistema AND
				rt.IdSistema = r.IdSistema AND rt.IdRecurso = r.IdRecurso AND rt.TipoRecurso = r.TipoRecurso AND
				pr.IdSistema = r.IdSistema AND pr.IdRecurso = r.IdRecurso AND pr.TipoRecurso = r.TipoRecurso;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `RecursosPorEmplazamientoParaXML` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `RecursosPorEmplazamientoParaXML`(in id_sistema varchar(32))
BEGIN
		(SELECT t.IdEmplazamiento,rTf.IdRecurso,r.IdTifx, r.SlotPasarela, r.NumDispositivoSlot
			FROM emplazamientos t, recursosradio rTf, recursos r
			WHERE t.IdSistema=id_sistema AND
				rTf.IdSistema=t.IdSistema AND
				rTf.IdEmplazamiento=t.IdEmplazamiento AND
				r.IdSistema=rTf.IdSistema AND
				r.IdRecurso=rTf.IdRecurso)
	UNION
		(SELECT t.IdEmplazamiento,null,null,null,null
			FROM emplazamientos t
			WHERE t.IdSistema=id_sistema AND
					t.IdEmplazamiento NOT IN (SELECT DISTINCT(IdEmplazamiento) FROM recursosradio WHERE IdSistema=id_sistema AND IdEmplazamiento IS NOT NULL));
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `RecursosPorTroncalParaXML` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `RecursosPorTroncalParaXML`(in id_sistema varchar(32))
BEGIN
		(SELECT t.IdTroncal,t.NumTest,rTf.IdRecurso,r.IdTifx, r.SlotPasarela, r.NumDispositivoSlot
			FROM troncales t, recursostf rTf, recursos r
			WHERE t.IdSistema=id_sistema AND
				rTf.IdSistema=t.IdSistema AND
				rTf.IdTroncal=t.IdTroncal AND
				r.IdSistema=rTf.IdSistema AND
				r.IdRecurso=rTf.IdRecurso)
	UNION
		(SELECT t.IdTroncal,t.NumTest,null,null,null,null
			FROM troncales t
			WHERE t.IdSistema=id_sistema AND
					t.IdTroncal NOT IN (SELECT DISTINCT(IdTroncal) FROM recursostf WHERE IdSistema=id_sistema AND IdTroncal IS NOT NULL));
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `RecursosRadioSinAsignarAEnlaces` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `RecursosRadioSinAsignarAEnlaces`(in id_sistema char(32), in id_site char(32))
BEGIN
	IF (id_site is not null) THEN
		SELECT RR.IdSistema, RR.IdRecurso, R.Tipo FROM RecursosRadio RR, Recursos R
			WHERE RR.IdSistema=id_sistema AND
				  RR.IdDestino is null AND 
				  RR.IdRecurso = R.IdRecurso AND
				  RR.IdEmplazamiento = id_site;
	ELSE
		SELECT RR.IdSistema, RR.IdRecurso, R.Tipo FROM RecursosRadio RR, Recursos R
			WHERE RR.IdSistema=id_sistema AND
				  RR.IdDestino is null AND 
				  RR.IdRecurso = R.IdRecurso;
	END IF;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `RecursosRdParaInformeXML` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `RecursosRdParaInformeXML`(in id_sistema varchar(32))
BEGIN
	SELECT r.*, rt.*,pr.*
	FROM recursos r, recursosradio rt, parametrosRecurso pr
	WHERE r.IdSistema=id_sistema AND
				rt.IdSistema = r.IdSistema AND rt.IdRecurso = r.IdRecurso AND rt.TipoRecurso = r.TipoRecurso AND
				pr.IdSistema = r.IdSistema AND pr.IdRecurso = r.IdRecurso AND pr.TipoRecurso = r.TipoRecurso;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `RecursosSinAsignar` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `RecursosSinAsignar`(in id char(32),in id_Sistema char(32))
BEGIN
	select * from recursos
	where IdSistema=id_Sistema && @id is null;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `RecursosTfParaInformeXML` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `RecursosTfParaInformeXML`(in id_sistema varchar(32))
BEGIN
	SELECT r.*, pr.*, rt.IdPrefijo,rt.TipoDestino,rt.IdDestino,rt.IdTroncal,rt.IdRed,rt.Lado,rt.Modo
	FROM recursos r, recursostf rt, parametrosRecurso pr
	WHERE r.IdSistema=id_sistema AND
				rt.IdSistema = r.IdSistema AND rt.IdRecurso = r.IdRecurso AND rt.TipoRecurso = r.TipoRecurso AND
				pr.IdSistema = r.IdSistema AND pr.IdRecurso = r.IdRecurso AND pr.TipoRecurso = r.TipoRecurso;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `RecursosTFSinAsignarAEnlaces` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `RecursosTFSinAsignarAEnlaces`(in id_sistema char(32))
BEGIN
		SELECT rtf.* FROM RecursosTF rtf, recursos r
		WHERE rtf.IdSistema=id_sistema AND
					IdDestino is null AND
					r.IdSistema=id_sistema AND
					r.IdRecurso=rtf.IdRecurso AND
					r.TipoRecurso=rtf.TipoRecurso AND
					r.Interface in (2,3,13);			
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `RedesUsuariosAbonados` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `RedesUsuariosAbonados`(in id_Sistema char(32), in id_Nucleo char(32), in id_Sector varchar(700))
BEGIN
	SELECT red.IdRed, red.IdPrefijo, abonado.IdAbonado 
	FROM Redes red, UsuariosAbonados abonado
	WHERE abonado.IdSistema=id_Sistema AND
				abonado.IdNucleo=id_Nucleo AND
				abonado.IdSector=id_Sector AND
				red.IdSistema=abonado.IdSistema AND
				red.IdPrefijo=abonado.IdPrefijo;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `RedesUsuariosAbonadosParaXML` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `RedesUsuariosAbonadosParaXML`(in id_sistema varchar(32), in id_sectorizacion varchar(32))
BEGIN
			SELECT abonado.IdNucleo, c.IdSector,c.IdSectorOriginal, red.IdRed, red.IdPrefijo, abonado.IdAbonado 
			FROM Redes red, UsuariosAbonados abonado, sectoressector c
			WHERE abonado.IdSistema=id_sistema AND
						abonado.IdSistema=c.IdSistema AND
						abonado.IdNucleo=c.IdNucleo AND
						abonado.IdSector=c.IdSectorOriginal AND
						red.IdSistema=abonado.IdSistema AND
						red.IdPrefijo=abonado.IdPrefijo;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `SectorConNumeroAbonado` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `SectorConNumeroAbonado`(in id_sistema char(32), in id_nucleo char(32), in id_prefijo int, in id_abonado char(32))
BEGIN
	SELECT s.IdSector FROM UsuariosAbonados ua, Sectores s
		WHERE ua.IdSistema=id_sistema AND
					ua.IdNucleo=id_nucleo AND
					ua.IdPrefijo=id_prefijo AND
					ua.IdAbonado=id_abonado AND
					s.IdSistema=ua.IdSistema AND
					s.IdNucleo=ua.IdNucleo AND
					s.SectorSimple AND
					ua.IdSector=s.IdSector;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `SectoresCompuestosPorSectorOriginal` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `SectoresCompuestosPorSectorOriginal`(in id_sistema char(32), in id_sector_original varchar(700))
BEGIN
	SELECT s.IdSector from SectoresSector ss, Sectores s
	where ss.IdSistema=id_sistema AND
				ss.IdSectorOriginal=id_sector_original AND
				ss.IdSectorOriginal!=ss.IdSector AND
				s.IdSector=ss.IdSector;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `SectoresFueraDeAgrupacion` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `SectoresFueraDeAgrupacion`(in id_sistema varchar(32), in id_agrupacion varchar(32))
BEGIN
	IF id_agrupacion IS NOT NULL THEN
		SELECT s.IdSector FROM Sectores s
			WHERE s.IdSistema=id_sistema AND
						s.SectorSimple=true AND
						s.IdSector NOT IN (SELECT IdSector FROM	SectoresAgrupacion	
																	WHERE IdSistema=id_sistema AND
																				IdAgrupacion=id_agrupacion);
	ELSE
		SELECT s.IdSector FROM Sectores s
			WHERE s.IdSistema=id_sistema AND
						s.SectorSimple=true;
	END IF;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `SectoresManttoEnActiva` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `SectoresManttoEnActiva`(in id_sistema char(32))
BEGIN
	SELECT  s.NumSacta, t.PosicionSacta
	FROM Sectores s, sectorizaciones sct, sectoressectorizacion ss, top t
	WHERE sct.IdSistema=id_sistema AND
				sct.Activa AND
				ss.IdSistema = sct.IdSistema AND ss.IdSectorizacion = sct.IdSectorizacion AND
				ss.IdSistema = s.IdSistema AND ss.IdNucleo = s.IdNucleo AND ss.IdSector = s.IdSector AND s.SectorSimple AND s.Tipo='M' AND
				ss.IdTOP=t.IdTOP;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `SectoresManttoEnTop` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `SectoresManttoEnTop`(in id_sistema varchar(32),in id_sectorizacion varchar(32), in id_top varchar(32))
BEGIN
	select s.* from sectores s, sectoressectorizacion ss, sectoressector sct
	where ss.IdSistema=id_sistema AND
				ss.IdSistema=sct.IdSistema AND
				s.IdSistema=sct.IdSistema AND
				ss.IdSectorizacion=id_sectorizacion AND
				ss.IdTOP=id_top AND
				ss.IdSector=sct.IdSector AND
				sct.IdSectorOriginal=s.IdSector AND
				s.SectorSimple AND
				s.Tipo='M';
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `SectoresNumSactaSorted` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `SectoresNumSactaSorted`(in id_sistema char(32), in id_nucleo char(32), in lista_sectores text)
BEGIN
	DECLARE lista text;
	SET @lista=lista_sectores;
	SELECT IdSector from Sectores
	where IdSistema=id_sistema AND IdNucleo=id_nucleo AND
		IdSector in (@lista)
	ORDER BY NumSacta;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `SectoresReales` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `SectoresReales`(in id_sistema char(32), in id_sectorizacion char(32), out cuantos int)
BEGIN
	SELECT count(*) into cuantos FROM Sectores 
	WHERE IdSistema=id_sistema AND
				SectorSimple AND
				Tipo='R' AND
				IdNucleo IN (SELECT DISTINCT IdNucleo FROM SectoresSectorizacion WHERE IdSistema=id_sistema AND IdSectorizacion=id_sectorizacion) AND 
				IdSector NOT IN (SELECT IdSectorOriginal FROM SectoresSector ss,SectoresSectorizacion sz
													WHERE sz.IdSectorizacion=id_sectorizacion AND
																sz.IdSistema=id_sistema AND
																ss.IdSistema=sz.IdSistema AND
																ss.IdSector=sz.IdSector);
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `SectoresSinAsignarASectorizacion` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `SectoresSinAsignarASectorizacion`(in id_sistema char(32),in id_sectorizacion char(32), in id_nucleo char(32))
BEGIN
  IF id_sectorizacion!='SACTA' THEN
		select a.IdSector, a.Tipo from Sectores a
		where a.IdSistema=id_sistema AND
					a.IdNucleo=id_nucleo AND
					a.SectorSimple=true AND
					a.IdSector not in (select ss.IdSectorOriginal FROM sectoresSectorizacion ssz, sectoresSector ss
																			WHERE ssz.IdSistema=id_sistema AND
																						ssz.IdNucleo=id_nucleo AND
																						ssz.IdSectorizacion=id_sectorizacion AND
																						ss.IdSistema=ssz.IdSistema AND ss.IdNucleo=ssz.IdNucleo AND 
																						ss.IdSector=ssz.IdSector)
	     ORDER BY A.IdSector,A.Tipo;
    ELSE
	   select a.IdSector, a.Tipo from Sectores a
		where a.IdSistema=id_sistema AND
					a.IdNucleo=id_nucleo AND
					a.SectorSimple=true AND
					a.IdSector not in (select ss.IdSectorOriginal FROM sectoresSectorizacion ssz, sectoresSector ss
																			WHERE ssz.IdSistema=id_sistema AND
																						ssz.IdNucleo=id_nucleo AND
																						ssz.IdSectorizacion=id_sectorizacion AND
																						ss.IdSistema=ssz.IdSistema AND ss.IdNucleo=ssz.IdNucleo AND 
																						ss.IdSector=ssz.IdSector) AND A.Tipo='M'
	     ORDER BY A.IdSector,A.Tipo;
    END IF;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `SlotsLibresEnTifx` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `SlotsLibresEnTifx`(in id_sistema char(32), in cuantos int)
BEGIN
	CASE cuantos
		WHEN 8 THEN
				SELECT l.Id FROM (
									select b.idtifx as Id,16-sum(a.2bd) as libres 
																								from (
																											(select idtifx as id,count(*)*2 as 2BD from recursos 
																													where IdSistema=id_sistema AND
																																interface=8 
																																group by idtifx) UNION
																										 	(select idtifx as id,count(*)*8 as 2BD from recursos 
																													where IdSistema=id_sistema AND
																																interface=7 
																																group by idtifx) UNION
																											(select idtifx as id,count(*)*16 as 2BD from recursos 
																													where IdSistema=id_sistema AND
																																interface=9
																																group by idtifx) UNION
																											(select idtifx as id,count(*) as 2BD from recursos 
																													where IdSistema=id_sistema AND
																																interface in (0,1,2,3,4,5,6)
																																group by idtifx) UNION
																											(select idtifx as id,0 as 2BD from TIFX 
																													where IdSistema=id_sistema AND
																																IdTifx NOT IN (SELECT DISTINCT(IdTifx) FROM Recursos WHERE IdSistema=id_sistema))
																											) a, tifx b
													where b.IdSistema=id_sistema AND
							  								a.id=b.IdTIFX
													group by a.id
								) l
				WHERE l.libres>=2;
			WHEN 7 THEN
				SELECT l.Id FROM (
									select b.idtifx as Id,16-sum(a.2bd) as libres 
																								from (
																											(select idtifx as id,((count(*)DIV 2)*4) + ((count(*)%2)*4)as 2BD from recursos 
																													where IdSistema=id_sistema AND
																																interface=8 
																																group by idtifx) UNION
																										 	(select idtifx as id,count(*)*8 as 2BD from recursos 
																													where IdSistema=id_sistema AND
																																interface=7 
																																group by idtifx) UNION
																											(select idtifx as id,count(*)*16 as 2BD from recursos 
																													where IdSistema=id_sistema AND
																																interface=9
																																group by idtifx) UNION
																											(select idtifx as id,count(*) as 2BD from recursos 
																													where IdSistema=id_sistema AND
																																interface in (0,1,2,3,4,5,6)
																																group by idtifx) UNION
																											(select idtifx as id,0 as 2BD from TIFX 
																													where IdSistema=id_sistema AND
																																IdTifx NOT IN (SELECT DISTINCT(IdTifx) FROM Recursos WHERE IdSistema=id_sistema))
																											) a, tifx b
													where b.IdSistema=id_sistema AND
							  								a.id=b.IdTIFX
													group by a.id
								) l
				WHERE l.libres>=8;
			WHEN 9 THEN
				SELECT l.Id FROM (
									select b.idtifx as Id,16-sum(a.2bd) as libres 
																								from (
																											(select idtifx as id,((count(*)DIV 2)*4) + ((count(*)%2)*4)as 2BD from recursos 
																													where IdSistema=id_sistema AND
																																interface=8 
																																group by idtifx) UNION
																										 	(select idtifx as id,count(*)*8 as 2BD from recursos 
																													where IdSistema=id_sistema AND
																																interface=7 
																																group by idtifx) UNION
																											(select idtifx as id,count(*)*16 as 2BD from recursos 
																													where IdSistema=id_sistema AND
																																interface=9
																																group by idtifx) UNION
																											(select idtifx as id,count(*) as 2BD from recursos 
																													where IdSistema=id_sistema AND
																																interface in (0,1,2,3,4,5,6)
																																group by idtifx) UNION
																											(select idtifx as id,0 as 2BD from TIFX 
																													where IdSistema=id_sistema AND
																																IdTifx NOT IN (SELECT DISTINCT(IdTifx) FROM Recursos WHERE IdSistema=id_sistema))
																											) a, tifx b
													where b.IdSistema=id_sistema AND
							  								a.id=b.IdTIFX
													group by a.id
								) l
				WHERE l.libres>=16;
			ELSE
				SELECT l.Id FROM (
									select b.idtifx as Id,16-sum(a.2bd) as libres 
																								from (
																											(select idtifx as id,((count(*)DIV 2)*4) + ((count(*)%2)*4)as 2BD from recursos 
																													where IdSistema=id_sistema AND
																																interface=8 
																																group by idtifx) UNION
																										 	(select idtifx as id,count(*)*8 as 2BD from recursos 
																													where IdSistema=id_sistema AND
																																interface=7 
																																group by idtifx) UNION
																											(select idtifx as id,count(*)*16 as 2BD from recursos 
																													where IdSistema=id_sistema AND
																																interface=9
																																group by idtifx) UNION
																											(select idtifx as id,count(*) as 2BD from recursos 
																													where IdSistema=id_sistema AND
																																interface in (0,1,2,3,4,5,6)
																																group by idtifx) UNION
																											(select idtifx as id,0 as 2BD from TIFX 
																													where IdSistema=id_sistema AND
																																IdTifx NOT IN (SELECT DISTINCT(IdTifx) FROM Recursos WHERE IdSistema=id_sistema))
																											) a, tifx b
													where b.IdSistema=id_sistema AND
							  								a.id=b.IdTIFX
													group by a.id
								) l
				WHERE l.libres>=1;
	END CASE;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `TeclasSector` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `TeclasSector`(in id_sistema char(32), in id_sector varchar(700))
BEGIN
	SELECT ts.* FROM TeclasSector ts
	WHERE 	ts.IdSistema=id_sistema AND
			ts.IdSector=id_sector;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `TeclasSectorParaXML` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `TeclasSectorParaXML`(in id_sistema varchar(32), in id_sectorizacion varchar(32))
BEGIN
	IF (id_sectorizacion IS NOT NULL) THEN
		SELECT c.IdSector,ts.* FROM TeclasSector ts, sectoressectorizacion c
			WHERE 		ts.IdSistema=id_sistema AND
						c.IdSectorizacion=id_sectorizacion AND
						c.IdSistema=ts.IdSistema AND
						ts.IdSector=c.IdSector;
	ELSE
		SELECT ts.* FROM TeclasSector ts, Sectores s
			WHERE	s.IdSistema=id_sistema AND
						s.SectorSimple AND
						ts.IdSistema=s.IdSistema AND
						ts.IdSector=s.IdSector AND
						ts.IdNucleo=s.IdNucleo;
	END IF;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `TroncalesSinAsignarARutas` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `TroncalesSinAsignarARutas`(in id_Sistema char(32),in id_Central char(32), in id_Ruta char(32))
BEGIN
	IF (id_Ruta="") THEN

			SELECT *
			FROM Troncales
			WHERE IdSistema=id_Sistema AND
							IdTroncal NOT IN (SELECT IdTroncal
											  FROM TroncalesRuta Tr,Rutas r
                                              WHERE tr.IdSistema=id_Sistema AND
												    r.IdSistema=Tr.IdSistema AND
                                                    r.Central=Tr.Central AND
                                                    r.IdRuta=Tr.IdRuta AND
                                                    (r.Tipo='D' OR (r.Tipo='A' AND tr.Central=id_Central)));
	ELSE
			
			SELECT * 
			FROM Troncales 
			WHERE IdSistema=id_Sistema AND 
							IdTroncal NOT IN (SELECT IdTroncal 
											  FROM TroncalesRuta
											  WHERE IdSistema=id_Sistema AND Central=id_Central);
	END IF;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `UsuariosImplicadosEnRecurso` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `UsuariosImplicadosEnRecurso`(in id_sistema char(32), in id_destino varchar(700), in id_Prefijo int)
BEGIN
  -- El procedimiento devuelve los sectores en los que el destino está configurado
  -- en la sectorización activa.

  -- Si el destino no es LCEN y el recurso se encuentra definido dentro de varios sectores que forman una agrupación, 
  -- el procedimiento devuelve todos esos sectores. En la función que invoca a este procedimiento, 
  -- se hará un filtrado para que en este caso sólo se devuelva el sector dominante de la agrupación. 

	IF (id_Prefijo is not null AND id_Prefijo=1) THEN
      -- Si el destino es LCEN (id_Prefijo=1), la asociación viene configurada en los campos IdDestinoLen de la tabla destinosexternosSector
      -- Un destino LCEN sólo puede estar asociado a un destino ATS (D.IdPrefijo=3) en el sistema
		  SELECT D.IdSector    IDSECTOR,
			   E.idsector    SECTORAGRUPADO,
			   C.ESDOMINANTE ES_DOMINANTE
		  FROM Externos E, SectoresSector C,DestinosExternosSector D
		  WHERE 
			 E.IdSistema=C.IdSistema AND
			 E.IdNucleo=C.IdNucleo   AND
			 E.IdSector=C.IdSector   AND   
			 C.IdSistema=D.IdSistema AND   
			 C.IDNUCLEO=D.IdNucleo AND   
			 C.IdSectorOriginal=D.IdSector AND
             E.TipoAcceso=D.TipoAcceso AND       
			 E.idprefijo=D.idPrefijoDestinoLCEN AND 			 
             E.IdDestino=D.IdDestinoLCEN AND 
			 D.IdSistema=id_sistema  AND
             D.idprefijo= 3 AND 
             D.idPrefijoDestinoLCEN=id_Prefijo AND
			 D.IdDestinoLCEN=id_destino AND
			 EXISTS(SELECT 1 FROM Sectorizaciones  ST
					WHERE ST.IdSistema=E.IdSistema AND ST.Activa=true and ST.IdSectorizacion=E.IdSectorizacion)
		  GROUP BY E.IdSector,D.idSector,C.ESDOMINANTE
		  ORDER BY E.idSector ASC, C.Esdominante DESC,D.idSector;
	ELSE  
		SELECT D.IdSector    IDSECTOR,
			   E.idsector    SECTORAGRUPADO,
			   C.ESDOMINANTE ES_DOMINANTE
		  FROM Externos E, SectoresSector C,DestinosExternosSector D
		  WHERE 
			 E.IdSistema=C.IdSistema AND
			 E.IdNucleo=C.IdNucleo   AND
			 E.IdSector=C.IdSector   AND   
			 C.IdSistema=D.IdSistema AND   
			 C.IDNUCLEO=D.IdNucleo AND   
			 C.IdSectorOriginal=D.IdSector AND
			 E.idprefijo=D.idprefijo AND 
			 E.TipoAcceso=D.TipoAcceso AND
			 D.IdSistema=id_sistema  AND
			 D.IdDestino=id_destino AND
			 EXISTS(SELECT 1 FROM Sectorizaciones  ST
					WHERE ST.IdSistema=E.IdSistema AND ST.Activa=true and ST.IdSectorizacion=E.IdSectorizacion) AND
			((E.literal!=E.grupo and E.idprefijo=D.idprefijo  AND E.IdDestino=D.IdDestino)
			  OR
			 (E.literal=E.grupo AND 
			  E.Grupo IN (SELECT dt.IdGrupo FROM destinosTelefonia dt 
						  WHERE dt.IdSistema = D.IdSistema AND dt.IdDestino = D.IdDestino)))
		  GROUP BY E.IdSector,D.idSector,C.ESDOMINANTE
		  ORDER BY E.idSector ASC, C.Esdominante DESC,D.idSector;
	END IF;                
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `VersionConfiguracion` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `VersionConfiguracion`(
  IN   `id_sistema`  varchar(32),
  OUT  `versionCfg`  text
)
BEGIN
	SELECT DATE_FORMAT(FechaActivacion,"%d/%m/%Y %H:%i:%s") INTO versionCfg FROM Sectorizaciones 
			WHERE Activa=true AND
						IdSistema=id_sistema;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;

--
-- Current Database: `new_cd40_trans`
--

CREATE DATABASE /*!32312 IF NOT EXISTS*/ `new_cd40_trans` /*!40100 DEFAULT CHARACTER SET utf8 */;

USE `new_cd40_trans`;

--
-- Table structure for table `abonados`
--

DROP TABLE IF EXISTS `abonados`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `abonados` (
  `IdSistema` varchar(32) NOT NULL,
  `IdAbonado` varchar(32) NOT NULL,
  PRIMARY KEY (`IdSistema`,`IdAbonado`),
  KEY `ABONADOS_FKIndex1` (`IdSistema`),
  CONSTRAINT `abonados_ibfk_1` FOREIGN KEY (`IdSistema`) REFERENCES `sistema` (`IdSistema`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `agenda`
--

DROP TABLE IF EXISTS `agenda`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `agenda` (
  `IdSistema` varchar(32) NOT NULL,
  `IdNucleo` varchar(32) NOT NULL,
  `IdSector` varchar(700) NOT NULL,
  `Prefijo` int(2) unsigned NOT NULL DEFAULT '0',
  `Numero` varchar(20) NOT NULL DEFAULT '',
  `Nombre` varchar(32) DEFAULT NULL,
  PRIMARY KEY (`IdSistema`,`IdNucleo`,`IdSector`,`Prefijo`,`Numero`),
  KEY `FK_AGENDA_REDES_idx` (`IdSistema`,`Prefijo`),
  CONSTRAINT `FK_AGENDA_REDES` FOREIGN KEY (`IdSistema`, `Prefijo`) REFERENCES `redes` (`IdSistema`, `IdPrefijo`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `FK_SECTORES` FOREIGN KEY (`IdSistema`, `IdNucleo`, `IdSector`) REFERENCES `sectores` (`IdSistema`, `IdNucleo`, `IdSector`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `agrupaciones`
--

DROP TABLE IF EXISTS `agrupaciones`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `agrupaciones` (
  `IdSistema` varchar(32) NOT NULL,
  `IdAgrupacion` varchar(32) NOT NULL,
  PRIMARY KEY (`IdSistema`,`IdAgrupacion`),
  KEY `Agrupaciones_FKIndex1` (`IdSistema`),
  CONSTRAINT `agrupaciones_ibfk_1` FOREIGN KEY (`IdSistema`) REFERENCES `sistema` (`IdSistema`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `alarmas`
--

DROP TABLE IF EXISTS `alarmas`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `alarmas` (
  `IdSistema` varchar(32) NOT NULL,
  `IdIncidencia` int(10) unsigned NOT NULL,
  `Alarma` tinyint(1) NOT NULL,
  PRIMARY KEY (`IdSistema`,`IdIncidencia`),
  KEY `Alarmas_FKIndex1` (`IdSistema`),
  KEY `Alarmas_FKIndex2` (`IdIncidencia`),
  CONSTRAINT `alarmas_ibfk_1` FOREIGN KEY (`IdSistema`) REFERENCES `sistema` (`IdSistema`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `alarmas_ibfk_2` FOREIGN KEY (`IdIncidencia`) REFERENCES `incidencias` (`IdIncidencia`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `altavoces`
--

DROP TABLE IF EXISTS `altavoces`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `altavoces` (
  `IdSistema` varchar(32) NOT NULL,
  `IdDestino` varchar(700) NOT NULL,
  `TipoDestino` int(10) unsigned NOT NULL,
  `IdNucleo` varchar(32) NOT NULL,
  `IdSector` varchar(700) NOT NULL,
  `PosHMI` int(10) unsigned NOT NULL,
  `NumAltavoz` int(10) unsigned NOT NULL,
  `Estado` varchar(1) DEFAULT NULL,
  PRIMARY KEY (`IdSistema`,`IdDestino`,`TipoDestino`,`IdNucleo`,`IdSector`,`PosHMI`,`NumAltavoz`),
  KEY `Altavoces_FKIndex1` (`IdSistema`,`IdDestino`,`TipoDestino`,`IdNucleo`,`IdSector`,`PosHMI`),
  CONSTRAINT `altavoces_ibfk_1` FOREIGN KEY (`IdSistema`, `IdDestino`, `TipoDestino`, `IdNucleo`, `IdSector`, `PosHMI`) REFERENCES `destinosradiosector` (`IdSistema`, `IdDestino`, `TipoDestino`, `IdNucleo`, `IdSector`, `PosHMI`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `controlbackup`
--

DROP TABLE IF EXISTS `controlbackup`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `controlbackup` (
  `IdSistema` varchar(32) NOT NULL,
  `TipoBackup` int(11) NOT NULL,
  `Profundidad` int(11) unsigned NOT NULL,
  `Recurso` varchar(128) NOT NULL,
  PRIMARY KEY (`IdSistema`,`TipoBackup`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `dependenciasats`
--

DROP TABLE IF EXISTS `dependenciasats`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `dependenciasats` (
  `IdDependenciaATS` varchar(32) NOT NULL,
  `IdSistema` varchar(32) NOT NULL,
  PRIMARY KEY (`IdDependenciaATS`,`IdSistema`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `destinos`
--

DROP TABLE IF EXISTS `destinos`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `destinos` (
  `IdSistema` varchar(32) NOT NULL,
  `IdDestino` varchar(700) NOT NULL,
  `TipoDestino` int(10) unsigned NOT NULL COMMENT '0: Destino Radio\n1: Destino de telefonía\n2: Destino de telefonía interno\n',
  PRIMARY KEY (`IdSistema`,`IdDestino`,`TipoDestino`),
  KEY `Destinos_FKIndex1` (`IdSistema`),
  CONSTRAINT `destinos_ibfk_1` FOREIGN KEY (`IdSistema`) REFERENCES `sistema` (`IdSistema`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER `new_cd40_trans`.`InsertDestino`
AFTER INSERT ON `new_cd40_trans`.`destinos`
FOR EACH ROW
BEGIN
    REPLACE INTO TablasModificadas (IdTabla) VALUES ('Destinos');
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER `new_cd40_trans`.`UpdateDestino`
AFTER UPDATE ON `new_cd40_trans`.`destinos`
FOR EACH ROW
BEGIN
    REPLACE INTO TablasModificadas (IdTabla) VALUES ('Destinos');
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER `new_cd40_trans`.`DeleteDestino`
BEFORE DELETE ON `new_cd40_trans`.`destinos`
FOR EACH ROW
BEGIN
  DECLARE iExiste INT(1);
  
	REPLACE INTO TablasModificadas (IdTabla) VALUES ('Destinos') ;
  IF old.TipoDestino=0 THEN
    REPLACE INTO TablasModificadas (IdTabla) VALUES ('DestinosRadio') ;
    REPLACE INTO TablasModificadas (IdTabla) VALUES ('DestinosRadioSector') ;
	ELSE
		REPLACE INTO TablasModificadas (IdTabla) VALUES ('DestinosTelefonia') ;
    REPLACE INTO TablasModificadas (IdTabla) VALUES ('DestinosInternos') ;
    REPLACE INTO TablasModificadas (IdTabla) VALUES ('DestinosExternos') ;
    REPLACE INTO TablasModificadas (IdTabla) VALUES ('DestinosInternosSector') ;
    REPLACE INTO TablasModificadas (IdTabla) VALUES ('DestinosExternosSector') ;
	END IF;
  
	IF old.tipoDestino=1 THEN  
      -- Se comprueba si algun sector tiene asociado el destino LCEN
      -- Si es asi, se elimina la asociacion antes de eliminarlo
 	 		SELECT COUNT(1) INTO iExiste
						FROM DestinosExternosSector
						WHERE IdSistema=old.IdSistema AND TipoDestino=old.TipoDestino AND 
							    IdPrefijoDestinoLCEN=1 AND IdDestinoLCEN=old.IdDestino;
			IF iExiste>0 THEN
        UPDATE DestinosExternosSector
        SET IdPrefijoDestinoLCEN=NULL, IdDestinoLCEN=NULL
        WHERE IdSistema=old.IdSistema AND TipoDestino=old.TipoDestino AND 
              IdPrefijoDestinoLCEN=1 AND IdDestinoLCEN=old.IdDestino;
			END IF ;
  END IF;  
  
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;

--
-- Temporary view structure for view `destinosenequipoexterno`
--

DROP TABLE IF EXISTS `destinosenequipoexterno`;
/*!50001 DROP VIEW IF EXISTS `destinosenequipoexterno`*/;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
/*!50001 CREATE VIEW `destinosenequipoexterno` AS SELECT 
 1 AS `IdDestino`,
 1 AS `IpRed1`,
 1 AS `IpRed2`,
 1 AS `idEquipos`*/;
SET character_set_client = @saved_cs_client;

--
-- Temporary view structure for view `destinosenequipoexternoparaoptions`
--

DROP TABLE IF EXISTS `destinosenequipoexternoparaoptions`;
/*!50001 DROP VIEW IF EXISTS `destinosenequipoexternoparaoptions`*/;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
/*!50001 CREATE VIEW `destinosenequipoexternoparaoptions` AS SELECT 
 1 AS `idEquipos`,
 1 AS `IpRed1`,
 1 AS `IpRed2`,
 1 AS `SipPort`,
 1 AS `TipoEquipo`,
 1 AS `modeloequipo`,
 1 AS `tiporadio`,
 1 AS `IdRecurso`,
 1 AS `IdDestino`*/;
SET character_set_client = @saved_cs_client;

--
-- Table structure for table `destinosexternos`
--

DROP TABLE IF EXISTS `destinosexternos`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `destinosexternos` (
  `IdSistema` varchar(32) NOT NULL,
  `IdDestino` varchar(700) NOT NULL,
  `TipoDestino` int(10) unsigned NOT NULL,
  `IdPrefijo` int(10) unsigned NOT NULL,
  `IdAbonado` varchar(32) DEFAULT NULL,
  PRIMARY KEY (`IdSistema`,`IdDestino`,`TipoDestino`,`IdPrefijo`),
  KEY `Table_44_FKIndex1` (`IdSistema`,`IdDestino`,`TipoDestino`,`IdPrefijo`),
  CONSTRAINT `Table_44_FKIndex1` FOREIGN KEY (`IdSistema`, `IdDestino`, `TipoDestino`, `IdPrefijo`) REFERENCES `destinostelefonia` (`IdSistema`, `IdDestino`, `TipoDestino`, `IdPrefijo`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER `new_cd40_trans`.`AltaDestinosExternos`
BEFORE INSERT ON `new_cd40_trans`.`destinosexternos`
FOR EACH ROW
BEGIN
  DECLARE numAbonados INT(1);
 	IF new.IdAbonado IS NOT null THEN
		SELECT COUNT(*) INTO numAbonados
				FROM Abonados
				WHERE IdSistema=new.IdSistema AND
							IdAbonado=new.IdAbonado;
		IF numAbonados=0 THEN
			INSERT INTO Abonados SET IdSistema=new.IdSistema,
																IdAbonado=new.IdAbonado;
		END IF;
	END IF ;
	INSERT INTO DestinosTelefonia (IdSistema,IdDestino,TipoDestino,IdPrefijo,IdGrupo) 
		VALUES (new.IdSistema,new.IdDestino,new.TipoDestino,new.IdPrefijo,null);
  REPLACE INTO TablasModificadas (IdTabla) VALUES ('DestinosExternos');
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER `new_cd40_trans`.`UpdateDestinosExternos`
AFTER UPDATE ON `new_cd40_trans`.`destinosexternos`
FOR EACH ROW
BEGIN
  DECLARE numAbonados INT(1);
 	IF new.IdAbonado IS NOT null THEN
	 	SELECT COUNT(*) INTO numAbonados
				FROM Abonados
				WHERE IdSistema=new.IdSistema AND
							IdAbonado=new.IdAbonado;
		IF numAbonados=0 THEN
			INSERT INTO Abonados SET IdSistema=new.IdSistema,
															IdAbonado=new.IdAbonado;
		END IF;
	END IF ;
  REPLACE INTO TablasModificadas (IdTabla) VALUES ('DestinosExternos');
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER `new_cd40_trans`.`DeleteDestinosExternos`
BEFORE DELETE ON `new_cd40_trans`.`destinosexternos`
FOR EACH ROW
BEGIN
	IF old.IdPrefijo=1 THEN
		UPDATE RECURSOSLCEN SET IdDestino=NULL,TipoDestino=NULL,IdPrefijo=NULL
				WHERE IdSistema = old.IdSistema AND IdDestino=old.IdDestino AND TipoDestino=old.TipoDestino AND IdPrefijo=old.IdPrefijo;
	END IF;
	
	IF old.IdPrefijo>2 THEN
		UPDATE RECURSOSTF SET IdDestino=NULL,TipoDestino=NULL,IdPrefijo=NULL 
				WHERE IdSistema = old.IdSistema AND IdDestino=old.IdDestino AND TipoDestino=old.TipoDestino AND IdPrefijo=old.IdPrefijo;
	END IF;
	REPLACE INTO TablasModificadas (IdTabla) VALUES ('DestinosExternos');
  REPLACE INTO TablasModificadas (IdTabla) VALUES ('DestinosExternosSector');
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;

--
-- Table structure for table `destinosexternossector`
--

DROP TABLE IF EXISTS `destinosexternossector`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `destinosexternossector` (
  `IdSistema` varchar(32) NOT NULL,
  `IdNucleo` varchar(32) NOT NULL,
  `IdSector` varchar(700) NOT NULL,
  `IdDestino` varchar(700) NOT NULL,
  `TipoDestino` int(10) unsigned NOT NULL,
  `IdPrefijo` int(10) unsigned NOT NULL,
  `PosHMI` int(10) unsigned DEFAULT NULL,
  `Prioridad` int(10) unsigned DEFAULT NULL,
  `OrigenR2` varchar(32) DEFAULT NULL,
  `PrioridadSIP` int(10) unsigned DEFAULT NULL,
  `TipoAcceso` varchar(2) NOT NULL COMMENT 'Tipo de Acceso: DA (Panel de telefonía), IA (Panel de Linea caliente)',
  `Literal` varchar(32) DEFAULT NULL,
  `IdPrefijoDestinoLCEN` int(10) unsigned DEFAULT NULL COMMENT 'Para Destinos ATS asignados al panel de línea caliente (TipoAcceso=IA): Prefijo del destino LCEN asignado al Destino ATS. Tomará el valor 1',
  `IdDestinoLCEN` varchar(32) DEFAULT NULL COMMENT 'Para Destinos ATS asignados al panel de línea caliente (TipoAcceso=IA): Identificador del destino LCEN asignado al Destino ATS.',
  PRIMARY KEY (`IdSistema`,`IdNucleo`,`IdSector`,`IdDestino`,`TipoDestino`,`IdPrefijo`,`TipoAcceso`),
  KEY `Table_45_FKIndex1` (`IdSistema`,`IdDestino`,`TipoDestino`,`IdPrefijo`),
  KEY `Table_45_FKIndex2` (`IdSistema`,`IdNucleo`,`IdSector`),
  KEY `DestinosExternosSector_idDestinoLcen_fk_idx` (`IdSistema`,`IdDestinoLCEN`,`TipoDestino`,`IdPrefijoDestinoLCEN`),
  KEY `DestinosExternosSector_idDestinoLcen_idx` (`IdSistema`,`TipoAcceso`,`IdPrefijoDestinoLCEN`),
  KEY `DestinosExternosSector_PosHMI_idx` (`IdSistema`,`IdNucleo`,`PosHMI`,`IdPrefijo`,`TipoAcceso`),
  CONSTRAINT `destinosexternossector_ibfk_1` FOREIGN KEY (`IdSistema`, `IdDestino`, `TipoDestino`, `IdPrefijo`) REFERENCES `destinosexternos` (`IdSistema`, `IdDestino`, `TipoDestino`, `IdPrefijo`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `destinosexternossector_ibfk_2` FOREIGN KEY (`IdSistema`, `IdNucleo`, `IdSector`) REFERENCES `sectores` (`IdSistema`, `IdNucleo`, `IdSector`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER `new_cd40_trans`.`AltaDestinoExternoSector`
AFTER INSERT ON `new_cd40_trans`.`destinosexternossector`
FOR EACH ROW
BEGIN
  REPLACE INTO TablasModificadas (IdTabla) VALUES ('DestinosExternosSector');
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER `new_cd40_trans`.`UpdateDestinoExternoSector`
AFTER UPDATE ON `new_cd40_trans`.`destinosexternossector`
FOR EACH ROW
BEGIN
  REPLACE INTO TablasModificadas (IdTabla) VALUES ('DestinosExternosSector');
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER `new_cd40_trans`.`DeleteDestinoExternoSector`
BEFORE DELETE ON `new_cd40_trans`.`destinosexternossector`
FOR EACH ROW
BEGIN
  REPLACE INTO TablasModificadas (IdTabla) VALUES ('DestinosExternosSector');
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;

--
-- Table structure for table `destinosinternos`
--

DROP TABLE IF EXISTS `destinosinternos`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `destinosinternos` (
  `IdSistema` varchar(32) NOT NULL,
  `IdDestino` varchar(700) NOT NULL,
  `TipoDestino` int(10) unsigned NOT NULL,
  `IdPrefijo` int(10) unsigned NOT NULL,
  `IdSector` varchar(700) NOT NULL,
  `IdNucleo` varchar(32) NOT NULL,
  PRIMARY KEY (`IdSistema`,`IdDestino`,`TipoDestino`,`IdPrefijo`,`IdSector`,`IdNucleo`),
  KEY `Table_42_FKIndex1` (`IdSistema`,`IdDestino`,`TipoDestino`,`IdPrefijo`),
  KEY `Table_42_FKIndex2` (`IdSistema`,`IdNucleo`,`IdSector`),
  CONSTRAINT `destinosinternos_ibfk_1` FOREIGN KEY (`IdSistema`, `IdDestino`, `TipoDestino`, `IdPrefijo`) REFERENCES `destinostelefonia` (`IdSistema`, `IdDestino`, `TipoDestino`, `IdPrefijo`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `destinosinternos_ibfk_2` FOREIGN KEY (`IdSistema`, `IdNucleo`, `IdSector`) REFERENCES `sectores` (`IdSistema`, `IdNucleo`, `IdSector`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER `new_cd40_trans`.`AltaDestinoInterno`
BEFORE INSERT ON `new_cd40_trans`.`destinosinternos`
FOR EACH ROW
BEGIN
    REPLACE INTO DestinosTelefonia SET IdSistema=new.IdSistema,
														 					IdDestino=new.IdDestino,
														 					TipoDestino=new.TipoDestino,
																			IdPrefijo=new.IdPrefijo;
    REPLACE INTO TablasModificadas (IdTabla) VALUES ('DestinosInternos');
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER `new_cd40_trans`.`UpdateDestinoInterno`
AFTER UPDATE ON `new_cd40_trans`.`destinosinternos`
FOR EACH ROW
BEGIN
    REPLACE INTO TablasModificadas (IdTabla) VALUES ('DestinosInternos');
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER `new_cd40_trans`.`DeleteDestinoInterno`
BEFORE DELETE ON `new_cd40_trans`.`destinosinternos`
FOR EACH ROW
BEGIN
    REPLACE INTO TablasModificadas (IdTabla) VALUES ('DestinosInternos');
    REPLACE INTO TablasModificadas (IdTabla) VALUES ('DestinosInternosSector');
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;

--
-- Table structure for table `destinosinternossector`
--

DROP TABLE IF EXISTS `destinosinternossector`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `destinosinternossector` (
  `IdSistema` varchar(32) NOT NULL,
  `IdDestino` varchar(32) NOT NULL,
  `TipoDestino` int(10) unsigned NOT NULL,
  `IdNucleo` varchar(32) NOT NULL,
  `IdSector` varchar(700) NOT NULL,
  `IdPrefijo` int(10) unsigned NOT NULL,
  `PosHMI` int(10) unsigned NOT NULL,
  `Prioridad` int(1) unsigned DEFAULT NULL,
  `OrigenR2` varchar(32) DEFAULT NULL,
  `PrioridadSIP` int(1) unsigned DEFAULT NULL,
  `TipoAcceso` varchar(2) NOT NULL,
  `Literal` varchar(32) DEFAULT NULL,
  PRIMARY KEY (`IdSistema`,`IdDestino`,`TipoDestino`,`IdNucleo`,`IdSector`,`IdPrefijo`),
  KEY `Table_43_FKIndex1` (`IdSistema`,`IdDestino`,`TipoDestino`,`IdPrefijo`),
  KEY `Table_43_FKIndex2` (`IdSistema`,`IdNucleo`,`IdSector`),
  CONSTRAINT `destinosinternossector_ibfk_1` FOREIGN KEY (`IdSistema`, `IdDestino`, `TipoDestino`, `IdPrefijo`) REFERENCES `destinosinternos` (`IdSistema`, `IdDestino`, `TipoDestino`, `IdPrefijo`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `destinosinternossector_ibfk_2` FOREIGN KEY (`IdSistema`, `IdNucleo`, `IdSector`) REFERENCES `sectores` (`IdSistema`, `IdNucleo`, `IdSector`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER `new_cd40_trans`.`AltaDestinoInternoSector`
AFTER INSERT ON `new_cd40_trans`.`destinosinternossector`
FOR EACH ROW
BEGIN
  REPLACE INTO TablasModificadas (IdTabla) VALUES ('DestinosInternosSector');
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER `new_cd40_trans`.`UpdateDestinoInternoSector`
AFTER UPDATE ON `new_cd40_trans`.`destinosinternossector`
FOR EACH ROW
BEGIN
  REPLACE INTO TablasModificadas (IdTabla) VALUES ('DestinosInternosSector');
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER `new_cd40_trans`.`DeleteDestinosInternosSector`
BEFORE DELETE ON `new_cd40_trans`.`destinosinternossector`
FOR EACH ROW
BEGIN
  REPLACE INTO TablasModificadas (IdTabla) VALUES ('DestinosInternosSector');
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;

--
-- Table structure for table `destinosradio`
--

DROP TABLE IF EXISTS `destinosradio`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `destinosradio` (
  `fk_metodosbss` int(11) NOT NULL AUTO_INCREMENT,
  `IdSistema` varchar(32) NOT NULL,
  `IdDestino` varchar(700) NOT NULL,
  `TipoDestino` int(10) unsigned NOT NULL COMMENT '0: Destino Radio\n\n',
  `zonas_idZonas` int(11) DEFAULT NULL COMMENT 'Zona Tx por defecto',
  `TipoFrec` int(1) unsigned DEFAULT NULL COMMENT 'Tipo Frecuencia: 0 --> Modo Normal tipo VHF, 1-->Modo Normal frecuencia UHF, 2 Modo Normal frecuencia HF, 4 --> Modo 1+1, 5 --> Modo FD, 6 --> Modo EM',
  `ExclusividadTXRX` tinyint(1) DEFAULT NULL,
  `Frecuencia` int(8) DEFAULT '0',
  `MetodoCalculoClimax` int(1) DEFAULT '0' COMMENT '0: Relativo\n1: Absoluto',
  `VentanaSeleccionBss` int(11) DEFAULT '50' COMMENT '50..2000 msg.\n',
  `SincronizaGrupoClimax` tinyint(1) DEFAULT '0',
  `AudioPrimerSqBss` tinyint(1) DEFAULT '1',
  `FrecuenciaNoDesasignable` tinyint(1) DEFAULT '0',
  `VentanaReposoZonaTxDefecto` int(11) DEFAULT NULL COMMENT 'Ventana de Reposo para Zona TX por Defecto (Para emplazamientos múltiples, ahora no se utilzará)',
  `PrioridadSesionSip` int(11) DEFAULT '2' COMMENT '2: Normal\n0: Emergencia',
  `MetodosBssOfrecidos` int(1) DEFAULT '0' COMMENT '0: Ninguno\n1: RSSI\n2: RSSI + NUCLEO\n3: CENTRAL',
  `CldSupervisionTime` int(11) DEFAULT '1' COMMENT '1..60 sg.\n',
  `CnfModoDestino` varchar(1) DEFAULT NULL COMMENT 'Modo del destino: 0 (Normal), 1 (1+1),  2 (FD), 3 (EM)\nSólo se usa en la Web de configuración',
  `CnfTipoFrecuencia` varchar(1) DEFAULT NULL COMMENT 'Tipo de Frecuencia: 0 --> VHF, 1  --> UHF, 2 --> HF\nSólo se usa en la Web de configuración',
  `ModoTransmision` enum('R','C','M') DEFAULT NULL COMMENT '''Modo de Transmision para el modo destino FD(CnfModoDestino=2): R (Último Receptor), C (Climax), M (Manual). Para otro, modo de destino tomará el valor null.''',
  `EmplazamientoDefecto` varchar(32) DEFAULT NULL COMMENT 'Indica, para el caso de frecuencias desplazadas, el emplazamiento por defecto. Sino se deja a NULL para no devolverlo.',
  `TiempoVueltaADefecto` int(11) DEFAULT NULL COMMENT 'Tiempo en segundos en el que el que la frecuencia vuelve al emplazamiento por defecto. Si es cero la frecuencia nunca vuelve al emplazamiento por defecto.',
  `PorcentajeRSSI` int(1) DEFAULT NULL COMMENT 'Porcentaje de cálculo para el método BSS RSSI. \n0: No aplica\n1-9 indica el porcentaje',
  `ConRedundancia` enum('0','1') DEFAULT '0' COMMENT 'Indicador de si el destino es 1+1: 1 (si), 0 (No)',
  PRIMARY KEY (`fk_metodosbss`,`IdSistema`,`IdDestino`,`TipoDestino`),
  KEY `DestinosRadio_FKIndex1` (`IdSistema`,`IdDestino`,`TipoDestino`),
  KEY `fk_destinosradio_zonas1_idx` (`zonas_idZonas`),
  KEY `destinoRadio_emplazamiento_fk_idx` (`IdSistema`,`EmplazamientoDefecto`),
  CONSTRAINT `destinoRadio_emplazamiento_fk` FOREIGN KEY (`IdSistema`, `EmplazamientoDefecto`) REFERENCES `emplazamientos` (`IdSistema`, `IdEmplazamiento`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `destinosradio_ibfk_1` FOREIGN KEY (`IdSistema`, `IdDestino`, `TipoDestino`) REFERENCES `destinos` (`IdSistema`, `IdDestino`, `TipoDestino`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `fk_destinosradio_zonas1` FOREIGN KEY (`zonas_idZonas`) REFERENCES `zonas` (`idZonas`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB AUTO_INCREMENT=187 DEFAULT CHARSET=latin1 COMMENT='Destinos Radio';
/*!40101 SET character_set_client = @saved_cs_client */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER `new_cd40_trans`.`AltaDestinoRadio`
BEFORE INSERT ON `new_cd40_trans`.`destinosradio`
FOR EACH ROW
BEGIN
  		DECLARE numDestinos INT(1);
 	 		SELECT COUNT(*) INTO numDestinos
						FROM Destinos
						WHERE IdSistema=new.IdSistema AND
							IdDestino=new.IdDestino AND
							TipoDestino=new.TipoDestino;
			IF numDestinos=0 THEN
					INSERT INTO Destinos SET IdSistema=new.IdSistema,
																	 IdDestino=new.IdDestino,
																	 TipoDestino=new.TipoDestino;
			END IF;
    REPLACE INTO TablasModificadas (IdTabla) VALUES ('DestinosRadio');
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER `new_cd40_trans`.`UpdateDestinoRadio`
AFTER UPDATE ON `new_cd40_trans`.`destinosradio`
FOR EACH ROW
BEGIN
    REPLACE INTO TablasModificadas (IdTabla) VALUES ('DestinosRadio');
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER `new_cd40_trans`.`DESTRADIODELETE`
BEFORE DELETE ON `new_cd40_trans`.`destinosradio`
FOR EACH ROW
BEGIN
IF old.TipoDestino=0 THEN
	UPDATE RECURSOSRADIO SET IdDestino=NULL,TipoDestino=NULL 
  				WHERE IdSistema = old.IdSistema AND IdDestino=old.IdDestino AND TipoDestino=old.TipoDestino;
END IF ;
    REPLACE INTO TablasModificadas (IdTabla) VALUES ('DestinosRadio');
    REPLACE INTO TablasModificadas (IdTabla) VALUES ('DestinosRadioSector') ;
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;

--
-- Table structure for table `destinosradiosector`
--

DROP TABLE IF EXISTS `destinosradiosector`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `destinosradiosector` (
  `IdSistema` varchar(32) NOT NULL,
  `IdDestino` varchar(700) NOT NULL,
  `TipoDestino` int(10) unsigned NOT NULL,
  `IdNucleo` varchar(32) NOT NULL,
  `IdSector` varchar(700) NOT NULL,
  `PosHMI` int(10) unsigned NOT NULL,
  `Prioridad` int(1) unsigned DEFAULT NULL,
  `PrioridadSIP` int(1) unsigned DEFAULT NULL,
  `ModoOperacion` varchar(1) DEFAULT NULL,
  `Cascos` varchar(1) DEFAULT NULL,
  `Literal` varchar(32) DEFAULT NULL,
  `SupervisionPortadora` tinyint(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`IdSistema`,`IdDestino`,`TipoDestino`,`IdNucleo`,`IdSector`,`PosHMI`),
  KEY `DestinosRadioSector_FKIndex1` (`IdSistema`,`IdDestino`,`TipoDestino`),
  KEY `DestinosRadioSector_FKIndex2` (`IdSistema`,`IdNucleo`,`IdSector`),
  CONSTRAINT `destinosradiosector_ibfk_1` FOREIGN KEY (`IdSistema`, `IdDestino`, `TipoDestino`) REFERENCES `destinosradio` (`IdSistema`, `IdDestino`, `TipoDestino`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `destinosradiosector_ibfk_2` FOREIGN KEY (`IdSistema`, `IdNucleo`, `IdSector`) REFERENCES `sectores` (`IdSistema`, `IdNucleo`, `IdSector`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER `new_cd40_trans`.`AltaDestinoRadioSector`
AFTER INSERT ON `new_cd40_trans`.`destinosradiosector`
FOR EACH ROW
BEGIN
  REPLACE INTO TablasModificadas (IdTabla) VALUES ('DestinosRadioSector');
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER `new_cd40_trans`.`UpdateDestinoRadioSector`
AFTER UPDATE ON `new_cd40_trans`.`destinosradiosector`
FOR EACH ROW
BEGIN
  REPLACE INTO TablasModificadas (IdTabla) VALUES ('DestinosRadioSector');
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER `new_cd40_trans`.`DeleteDestinoRadioSector`
BEFORE DELETE ON `new_cd40_trans`.`destinosradiosector`
FOR EACH ROW
BEGIN
  REPLACE INTO TablasModificadas (IdTabla) VALUES ('DestinosRadioSector');
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;

--
-- Table structure for table `destinostelefonia`
--

DROP TABLE IF EXISTS `destinostelefonia`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `destinostelefonia` (
  `IdSistema` varchar(32) NOT NULL,
  `IdDestino` varchar(700) NOT NULL,
  `TipoDestino` int(10) unsigned NOT NULL,
  `IdPrefijo` int(10) unsigned NOT NULL,
  `IdGrupo` varchar(32) DEFAULT NULL,
  PRIMARY KEY (`IdSistema`,`IdDestino`,`TipoDestino`,`IdPrefijo`),
  KEY `DestinosTelefonia_FKIndex1` (`IdSistema`,`IdDestino`,`TipoDestino`),
  KEY `DestinosTelefonia_FKIndex2` (`IdSistema`,`IdGrupo`),
  KEY `DestinosTelefonia_FKIndex3` (`IdSistema`,`IdPrefijo`),
  CONSTRAINT `destinostelefonia_ibfk_1` FOREIGN KEY (`IdSistema`, `IdDestino`, `TipoDestino`) REFERENCES `destinos` (`IdSistema`, `IdDestino`, `TipoDestino`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `destinostelefonia_ibfk_2` FOREIGN KEY (`IdSistema`, `IdGrupo`) REFERENCES `grupostelefonia` (`IdSistema`, `IdGrupo`) ON DELETE NO ACTION ON UPDATE CASCADE,
  CONSTRAINT `destinostelefonia_ibfk_3` FOREIGN KEY (`IdSistema`, `IdPrefijo`) REFERENCES `prefijos` (`IdSistema`, `IdPrefijo`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER `new_cd40_trans`.`AltaDestinosTelefonia`
BEFORE INSERT ON `new_cd40_trans`.`destinostelefonia`
FOR EACH ROW
BEGIN
 			DECLARE numDestinos INT(1);
 	 		SELECT COUNT(*) INTO numDestinos
						FROM Destinos
						WHERE IdSistema=new.IdSistema AND
							IdDestino=new.IdDestino AND
							TipoDestino=new.TipoDestino;
			IF numDestinos=0 THEN
					INSERT INTO Destinos SET IdSistema=new.IdSistema,
																	 IdDestino=new.IdDestino,
																	 TipoDestino=new.TipoDestino;
			END IF ;
    REPLACE INTO TablasModificadas (IdTabla) VALUES ('DestinosTelefonia');
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER `new_cd40_trans`.`UpdateDestinoTelefonia`
AFTER UPDATE ON `new_cd40_trans`.`destinostelefonia`
FOR EACH ROW
BEGIN
    REPLACE INTO TablasModificadas (IdTabla) VALUES ('DestinosTelefonia');
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER `new_cd40_trans`.`DeleteDestinoTelefonia`
BEFORE DELETE ON `new_cd40_trans`.`destinostelefonia`
FOR EACH ROW
BEGIN
    REPLACE INTO TablasModificadas (IdTabla) VALUES ('DestinosTelefonia') ;
    REPLACE INTO TablasModificadas (IdTabla) VALUES ('DestinosInternos') ;
    REPLACE INTO TablasModificadas (IdTabla) VALUES ('DestinosExternos') ;
    REPLACE INTO TablasModificadas (IdTabla) VALUES ('DestinosInternosSector') ;
    REPLACE INTO TablasModificadas (IdTabla) VALUES ('DestinosExternosSector') ;
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;

--
-- Table structure for table `emplazamientos`
--

DROP TABLE IF EXISTS `emplazamientos`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `emplazamientos` (
  `IdSistema` varchar(32) NOT NULL,
  `IdEmplazamiento` varchar(32) NOT NULL,
  PRIMARY KEY (`IdSistema`,`IdEmplazamiento`),
  KEY `Emplazamientos_FKIndex1` (`IdSistema`),
  CONSTRAINT `emplazamientos_ibfk_1` FOREIGN KEY (`IdSistema`) REFERENCES `sistema` (`IdSistema`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER `new_cd40_trans`.`PRDELETE`
BEFORE DELETE ON `new_cd40_trans`.`emplazamientos`
FOR EACH ROW
UPDATE RECURSOSRADIO SET IdEmplazamiento=NULL
			WHERE IdSistema = old.IdSistema AND IdEmplazamiento=old.IdEmplazamiento */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;

--
-- Table structure for table `encaminamientos`
--

DROP TABLE IF EXISTS `encaminamientos`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `encaminamientos` (
  `IdSistema` varchar(32) NOT NULL,
  `Central` varchar(32) NOT NULL,
  `CentralPropia` tinyint(1) DEFAULT '0',
  `Throwswitching` tinyint(1) DEFAULT '0',
  `NumTest` varchar(6) DEFAULT NULL,
  `CentralIp` tinyint(1) DEFAULT '0',
  PRIMARY KEY (`IdSistema`,`Central`),
  KEY `Encaminamientos_FKIndex1` (`IdSistema`),
  CONSTRAINT `encaminamientos_ibfk_1` FOREIGN KEY (`IdSistema`) REFERENCES `sistema` (`IdSistema`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `equiposeu`
--

DROP TABLE IF EXISTS `equiposeu`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `equiposeu` (
  `IdSistema` varchar(32) NOT NULL,
  `idEquipos` varchar(32) NOT NULL,
  `IpRed1` varchar(60) DEFAULT NULL COMMENT 'Si el equipo externo está asociado a una CentralIP (TipoEquipo=3 y Min=Max= -1), almacena la Dirección IP y puerto del primer proxy configurado. El valor almacenara la dirección en el formato XXX.XXX.XXX.XXX:YYYY donde YYYY es el puerto. Si es un equipo externo de otro tipo, almacena sólo la dirección IP 1  del equipo. El puerto se almacena en el campo SipPort',
  `IpRed2` varchar(60) DEFAULT NULL COMMENT 'Si el equipo externo está asociado a una CentralIP (TipoEquipo=3 y Min=Max= -1), almacena la Dirección IP y puerto del segundo proxy configurado. El valor almacenara la dirección en el formato XXX.XXX.XXX.XXX:YYYY donde YYYY es el puerto. Si es un equipo externo de otro tipo, almacena sólo la dirección IP 2  del equipo. El puerto se almacena en el campo SipPort',
  `TipoEquipo` int(10) unsigned DEFAULT NULL,
  `Interno` tinyint(1) DEFAULT '0',
  `Min` int(11) DEFAULT '0',
  `Max` int(11) DEFAULT '0',
  `SipPort` int(6) DEFAULT '5060',
  `IpRed3` varchar(60) DEFAULT NULL COMMENT 'Si el equipo externo está asociado a una CentralIP (TipoEquipo=3 y Min=Max= -1), almacena la Dirección IP y puerto del tercer proxy configurado. El valor almacenara la dirección en el formato XXX.XXX.XXX.XXX:YYYY donde YYYY es el puerto.Si es de otro tipo,  este campo tomará el valor null',
  `SrvPresenciaIpRed1` varchar(60) DEFAULT NULL COMMENT 'Si el equipo externo está asociado a una CentralIP (TipoEquipo=3 y Min=Max= -1), almacena la Dirección IP y puerto del primer servidor de presencia configurado. El valor almacenara la dirección en el formato XXX.XXX.XXX.XXX:YYYY donde YYYY es el puerto. Si es un equipo externo de otro tipo, tomará el valor null.',
  `SrvPresenciaIpRed2` varchar(60) DEFAULT NULL COMMENT 'Si el equipo externo está asociado a una CentralIP (TipoEquipo=3 y Min=Max= -1), almacena la Dirección IP y puerto del segundo servidor de presencia configurado. El valor almacenara la dirección en el formato XXX.XXX.XXX.XXX:YYYY donde YYYY es el puerto. Si es un equipo externo de otro tipo, tomará el valor null.',
  `SrvPresenciaIpRed3` varchar(60) DEFAULT NULL COMMENT 'Si el equipo externo está asociado a una CentralIP (TipoEquipo=3 y Min=Max= -1), almacena la Dirección IP y puerto del tercer servidor de presencia configurado. El valor almacenara la dirección en el formato XXX.XXX.XXX.XXX:YYYY donde YYYY es el puerto. Si es un equipo externo de otro tipo, tomará el valor null.',
  PRIMARY KEY (`IdSistema`,`idEquipos`),
  KEY `EquiposEU_FKIndex1` (`IdSistema`),
  CONSTRAINT `equiposeu_ibfk_1` FOREIGN KEY (`IdSistema`) REFERENCES `sistema` (`IdSistema`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `estadoaltavoces`
--

DROP TABLE IF EXISTS `estadoaltavoces`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `estadoaltavoces` (
  `IdDestino` varchar(700) NOT NULL,
  `PosHMI` int(10) unsigned NOT NULL,
  `IdSector` varchar(700) NOT NULL,
  `IdNucleo` varchar(32) NOT NULL,
  `IdSectorizacion` varchar(32) NOT NULL,
  `IdSistema` varchar(32) NOT NULL,
  `NumAltavoz` int(10) unsigned NOT NULL,
  `Estado` varchar(1) DEFAULT NULL,
  PRIMARY KEY (`IdDestino`,`PosHMI`,`IdSector`,`IdNucleo`,`IdSectorizacion`,`IdSistema`,`NumAltavoz`),
  KEY `Altavoz_FKIndex1` (`IdSistema`,`IdSectorizacion`,`IdNucleo`,`IdSector`,`PosHMI`,`IdDestino`),
  CONSTRAINT `estadoaltavoces_ibfk_1` FOREIGN KEY (`IdSistema`, `IdSectorizacion`, `IdNucleo`, `IdSector`, `PosHMI`, `IdDestino`) REFERENCES `radio` (`IdSistema`, `IdSectorizacion`, `IdNucleo`, `IdSector`, `PosHMI`, `IdDestino`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `estadocluster`
--

DROP TABLE IF EXISTS `estadocluster`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `estadocluster` (
  `Name` varchar(32) NOT NULL,
  `Presencia` tinyint(1) DEFAULT '0',
  `Estado` int(11) DEFAULT '0',
  PRIMARY KEY (`Name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `estadorecursos`
--

DROP TABLE IF EXISTS `estadorecursos`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `estadorecursos` (
  `IdDestino` varchar(32) NOT NULL,
  `PosHMI` int(10) unsigned NOT NULL,
  `IdSector` varchar(700) NOT NULL,
  `IdNucleo` varchar(32) NOT NULL,
  `IdSectorizacion` varchar(32) NOT NULL,
  `IdSistema` varchar(32) NOT NULL,
  `IdRecurso` varchar(32) NOT NULL,
  `Estado` varchar(1) DEFAULT NULL,
  PRIMARY KEY (`IdDestino`,`PosHMI`,`IdSector`,`IdNucleo`,`IdSectorizacion`,`IdSistema`,`IdRecurso`),
  KEY `Recurso_FKIndex1` (`IdSistema`,`IdSectorizacion`,`IdNucleo`,`IdSector`,`PosHMI`,`IdDestino`),
  CONSTRAINT `estadorecursos_ibfk_1` FOREIGN KEY (`IdSistema`, `IdSectorizacion`, `IdNucleo`, `IdSector`, `PosHMI`, `IdDestino`) REFERENCES `radio` (`IdSistema`, `IdSectorizacion`, `IdNucleo`, `IdSector`, `PosHMI`, `IdDestino`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `estadosrecursos`
--

DROP TABLE IF EXISTS `estadosrecursos`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `estadosrecursos` (
  `IdSistema` varchar(32) NOT NULL,
  `IdSector` varchar(700) NOT NULL,
  `IdNucleo` varchar(32) NOT NULL,
  `PosHMI` int(10) unsigned NOT NULL,
  `IdRecurso` varchar(32) NOT NULL,
  `TipoRecurso` int(1) unsigned NOT NULL,
  `IdDestino` varchar(700) NOT NULL,
  `TipoDestino` int(10) unsigned NOT NULL,
  `Estado` varchar(1) DEFAULT NULL,
  PRIMARY KEY (`IdSistema`,`IdSector`,`IdNucleo`,`PosHMI`,`IdRecurso`,`TipoRecurso`,`IdDestino`,`TipoDestino`),
  KEY `Table_44_FKIndex1` (`IdSistema`,`IdRecurso`,`TipoRecurso`),
  KEY `Table_44_FKIndex2` (`IdSistema`,`IdDestino`,`TipoDestino`,`IdNucleo`,`IdSector`,`PosHMI`),
  CONSTRAINT `estadosrecursos_ibfk_1` FOREIGN KEY (`IdSistema`, `IdRecurso`, `TipoRecurso`) REFERENCES `recursosradio` (`IdSistema`, `IdRecurso`, `TipoRecurso`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `estadosrecursos_ibfk_2` FOREIGN KEY (`IdSistema`, `IdDestino`, `TipoDestino`, `IdNucleo`, `IdSector`, `PosHMI`) REFERENCES `destinosradiosector` (`IdSistema`, `IdDestino`, `TipoDestino`, `IdNucleo`, `IdSector`, `PosHMI`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `externos`
--

DROP TABLE IF EXISTS `externos`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `externos` (
  `IdSistema` varchar(32) NOT NULL,
  `IdSectorizacion` varchar(32) NOT NULL,
  `IdNucleo` varchar(32) NOT NULL,
  `IdSector` varchar(700) NOT NULL,
  `IdDestino` varchar(700) NOT NULL,
  `IdPrefijo` int(10) unsigned NOT NULL,
  `TipoAcceso` varchar(2) NOT NULL,
  `PosHMI` int(10) unsigned DEFAULT NULL,
  `Prioridad` int(10) unsigned DEFAULT NULL,
  `OrigenR2` varchar(32) DEFAULT NULL,
  `PrioridadSIP` int(10) unsigned DEFAULT NULL,
  `Literal` varchar(32) DEFAULT NULL,
  `Grupo` varchar(80) DEFAULT '""',
  PRIMARY KEY (`IdSistema`,`IdSectorizacion`,`IdNucleo`,`IdSector`,`IdDestino`,`IdPrefijo`,`TipoAcceso`),
  KEY `Table_41_FKIndex1` (`IdSistema`,`IdSectorizacion`,`IdNucleo`,`IdSector`),
  CONSTRAINT `externos_ibfk_1` FOREIGN KEY (`IdSistema`, `IdSectorizacion`, `IdNucleo`, `IdSector`) REFERENCES `sectoressectorizacion` (`IdSistema`, `IdSectorizacion`, `IdNucleo`, `IdSector`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `funciones`
--

DROP TABLE IF EXISTS `funciones`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `funciones` (
  `IdSistema` varchar(32) NOT NULL,
  `Clave` int(11) NOT NULL,
  `Tipo` int(11) NOT NULL COMMENT '0: Radio; 1: Telefonía',
  `Funcion` varchar(32) NOT NULL,
  PRIMARY KEY (`IdSistema`,`Clave`),
  CONSTRAINT `FK_Sistema_Funciones` FOREIGN KEY (`IdSistema`) REFERENCES `sistema` (`IdSistema`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `grupostelefonia`
--

DROP TABLE IF EXISTS `grupostelefonia`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `grupostelefonia` (
  `IdSistema` varchar(32) NOT NULL,
  `IdGrupo` varchar(32) NOT NULL,
  PRIMARY KEY (`IdSistema`,`IdGrupo`),
  KEY `GruposTelefonia_FKIndex1` (`IdSistema`),
  CONSTRAINT `grupostelefonia_ibfk_1` FOREIGN KEY (`IdSistema`) REFERENCES `sistema` (`IdSistema`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER `new_cd40_trans`.`DeleteGrupoTelefonia`
BEFORE DELETE ON `new_cd40_trans`.`grupostelefonia`
FOR EACH ROW
BEGIN
  UPDATE DestinosTelefonia 
    SET IdGrupo=null 
    WHERE IdSistema=old.IdSistema AND IdGrupo=old.IdGrupo;
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;

--
-- Table structure for table `gwactivas`
--

DROP TABLE IF EXISTS `gwactivas`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `gwactivas` (
  `IdSistema` varchar(32) NOT NULL,
  `IdTifx` varchar(32) NOT NULL,
  `IpRed` varchar(60) NOT NULL,
  PRIMARY KEY (`IdSistema`,`IdTifx`),
  CONSTRAINT `FK_Tifx` FOREIGN KEY (`IdSistema`, `IdTifx`) REFERENCES `tifx` (`IdSistema`, `IdTIFX`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `hfparams`
--

DROP TABLE IF EXISTS `hfparams`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `hfparams` (
  `IdSistema` varchar(32) NOT NULL COMMENT 'Identificador del sistema',
  `IdRecurso` varchar(32) NOT NULL COMMENT 'Identificador del recurso radio',
  `IpGestor` varchar(15) DEFAULT NULL,
  `Oid` varchar(45) DEFAULT NULL,
  `Frecuencia` varchar(10) DEFAULT '',
  `TipoEquipo` int(11) DEFAULT '1' COMMENT '0: Receptor\n1: Transmisor\n2: Transceptor',
  `TipoCanal` int(11) DEFAULT '0' COMMENT '0: Monocanal\n1: Multicanal\n2: Otros',
  `TipoFrecuencia` int(11) DEFAULT '1' COMMENT '0: HF\n1: VHF\n2: UHF',
  `TipoModo` int(11) DEFAULT '0' COMMENT '0: Principal\n1: Reserva\n2: Ambos',
  `PrioridadEquipo` int(11) DEFAULT '0' COMMENT '0: Baja\n1: Normal\n2: Alta',
  `Puerto` int(10) DEFAULT '0',
  `Offset` int(10) DEFAULT '0',
  `Canalizacion` int(10) DEFAULT '0',
  `Modulacion` int(10) DEFAULT '0',
  `Potencia` int(10) DEFAULT '0',
  `FormatoFrecuenciaPrincipal` int(10) DEFAULT '0',
  `ModeloEquipo` int(11) DEFAULT '1000',
  PRIMARY KEY (`IdSistema`,`IdRecurso`),
  CONSTRAINT `hfparams_ibfk_1` FOREIGN KEY (`IdSistema`, `IdRecurso`) REFERENCES `recursosradio` (`IdSistema`, `IdRecurso`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COMMENT='Parámetros de recursos radio HF y M+N';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `hfrangofrecuencias`
--

DROP TABLE IF EXISTS `hfrangofrecuencias`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `hfrangofrecuencias` (
  `IdSistema` varchar(32) NOT NULL,
  `IdRecurso` varchar(32) NOT NULL,
  `Min` varchar(15) DEFAULT NULL,
  `Max` varchar(15) DEFAULT NULL,
  KEY `hfrangofrecuencias_ibfk_1` (`IdSistema`,`IdRecurso`),
  CONSTRAINT `hfrangofrecuencias_ibfk_1` FOREIGN KEY (`IdSistema`, `IdRecurso`) REFERENCES `hfparams` (`IdSistema`, `IdRecurso`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `historicoincidencias`
--

DROP TABLE IF EXISTS `historicoincidencias`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `historicoincidencias` (
  `IdSistema` varchar(32) NOT NULL,
  `Scv` int(1) NOT NULL DEFAULT '0',
  `IdHw` varchar(32) NOT NULL,
  `TipoHw` int(1) unsigned NOT NULL,
  `IdIncidencia` int(10) unsigned NOT NULL,
  `FechaHora` datetime NOT NULL,
  `Reconocida` datetime DEFAULT NULL,
  `Descripcion` varchar(200) DEFAULT NULL,
  `Usuario` varchar(32) DEFAULT NULL,
  KEY `HistoricoIncidencias_FKIndex1` (`IdIncidencia`),
  KEY `HistoricoIncidencias_FKIndex2` (`IdSistema`),
  KEY `HistoricoIncidenciasIndex` (`IdSistema`,`IdHw`,`TipoHw`,`IdIncidencia`,`FechaHora`),
  KEY `HistoricoIncidenciasFecha_Idx` (`FechaHora`),
  CONSTRAINT `historicoincidencias_ibfk_1` FOREIGN KEY (`IdIncidencia`) REFERENCES `incidencias` (`IdIncidencia`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `historicoincidencias_ibfk_2` FOREIGN KEY (`IdSistema`) REFERENCES `sistema` (`IdSistema`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `incidencias`
--

DROP TABLE IF EXISTS `incidencias`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `incidencias` (
  `IdIncidencia` int(10) unsigned NOT NULL,
  `IdIncidenciaCorrectora` int(10) unsigned DEFAULT NULL,
  `Incidencia` varchar(180) DEFAULT NULL,
  `Descripcion` varchar(180) DEFAULT NULL,
  `GeneraError` tinyint(1) DEFAULT NULL,
  `OID` varchar(32) DEFAULT NULL,
  `trep` int(4) NOT NULL DEFAULT '0',
  PRIMARY KEY (`IdIncidencia`),
  KEY `Incidencias_FKIndex1` (`IdIncidenciaCorrectora`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `incidencias_frances`
--

DROP TABLE IF EXISTS `incidencias_frances`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `incidencias_frances` (
  `IdIncidencia` int(10) unsigned NOT NULL,
  `IdIncidenciaCorrectora` int(10) unsigned DEFAULT NULL,
  `Incidencia` varchar(180) DEFAULT NULL,
  `Descripcion` varchar(180) DEFAULT NULL,
  `GeneraError` tinyint(1) DEFAULT NULL,
  `OID` varchar(32) DEFAULT NULL,
  `trep` int(4) NOT NULL DEFAULT '0',
  PRIMARY KEY (`IdIncidencia`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `incidencias_ingles`
--

DROP TABLE IF EXISTS `incidencias_ingles`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `incidencias_ingles` (
  `IdIncidencia` int(10) unsigned NOT NULL,
  `IdIncidenciaCorrectora` int(10) unsigned DEFAULT NULL,
  `Incidencia` varchar(180) DEFAULT NULL,
  `Descripcion` varchar(180) DEFAULT NULL,
  `GeneraError` tinyint(1) DEFAULT NULL,
  `OID` varchar(32) DEFAULT NULL,
  `trep` int(4) NOT NULL DEFAULT '0',
  PRIMARY KEY (`IdIncidencia`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `internos`
--

DROP TABLE IF EXISTS `internos`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `internos` (
  `IdSistema` varchar(32) NOT NULL,
  `IdSectorizacion` varchar(32) NOT NULL,
  `IdNucleo` varchar(32) NOT NULL,
  `IdSector` varchar(700) NOT NULL,
  `IdDestino` varchar(700) NOT NULL,
  `IdPrefijo` int(10) unsigned NOT NULL,
  `TipoAcceso` varchar(2) NOT NULL,
  `PosHMI` int(10) unsigned DEFAULT NULL,
  `Prioridad` int(10) unsigned DEFAULT NULL,
  `OrigenR2` varchar(32) DEFAULT NULL,
  `PrioridadSIP` int(10) unsigned DEFAULT NULL,
  `Literal` varchar(32) DEFAULT NULL,
  PRIMARY KEY (`IdSistema`,`IdSectorizacion`,`IdNucleo`,`IdSector`,`IdDestino`,`IdPrefijo`,`TipoAcceso`),
  KEY `Table_40_FKIndex1` (`IdSistema`,`IdSectorizacion`,`IdNucleo`,`IdSector`),
  CONSTRAINT `internos_ibfk_1` FOREIGN KEY (`IdSistema`, `IdSectorizacion`, `IdNucleo`, `IdSector`) REFERENCES `sectoressectorizacion` (`IdSistema`, `IdSectorizacion`, `IdNucleo`, `IdSector`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `llamadas_entrantes_sector`
--

DROP TABLE IF EXISTS `llamadas_entrantes_sector`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `llamadas_entrantes_sector` (
  `IdSistema` varchar(32) NOT NULL COMMENT 'Identificador del sistema al que pertenece el sector',
  `IdNucleo` varchar(32) NOT NULL COMMENT 'Identificador del núcleo al que pertenece el sector',
  `IdSector` varchar(700) NOT NULL COMMENT 'Identificador del sector',
  `IdRed` varchar(32) NOT NULL COMMENT 'Identificador único de la red',
  `Tipo` enum('A','R','T') NOT NULL COMMENT 'Tipo: A (Abonado), R (Rango), T (Todas)',
  `NumAbonadoInicial` varchar(45) NOT NULL COMMENT 'Si tipo es A, almacena el número del abonado al que se le permite realizar llamadas.\nSi  tipo es R, almacena el número de abonado inicial del rango al que se le permite realizar llamadas.',
  `NumAbonadoFinal` varchar(45) DEFAULT NULL COMMENT 'Si  tipo es R, almacena el número de abonado final del rango al que se le permite realizar llamadas.\nSi tipo es A, estará vacío',
  `FechaGbd` datetime DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP COMMENT 'Fecha de inserción o modificación del registro en la BD',
  PRIMARY KEY (`IdSistema`,`IdNucleo`,`IdSector`,`IdRed`,`Tipo`,`NumAbonadoInicial`),
  UNIQUE KEY `Llam_Ent_Sector_Rango_idx` (`IdSistema`,`IdNucleo`,`IdSector`,`IdRed`,`Tipo`,`NumAbonadoInicial`,`NumAbonadoFinal`),
  KEY `Llam_Ent_Sector_Red_FK_idx` (`IdSistema`,`IdRed`),
  CONSTRAINT `Llam_Ent_Sector_Red_FK` FOREIGN KEY (`IdSistema`, `IdRed`) REFERENCES `redes` (`IdSistema`, `IdRed`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `Llam_Ent_Sector_Sectores_FK` FOREIGN KEY (`IdSistema`, `IdNucleo`, `IdSector`) REFERENCES `sectores` (`IdSistema`, `IdNucleo`, `IdSector`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COMMENT='Lista blanca de abonados o rango de abonados de cada red con prefijo >= 4 a los que un sector podrá efectuar llamadas ';
/*!40101 SET character_set_client = @saved_cs_client */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER `new_cd40_trans`.`llamadas_entrantes_sector_AF_INSERT`
AFTER INSERT ON `new_cd40_trans`.`llamadas_entrantes_sector`
FOR EACH ROW
BEGIN
  
  IF (NEW.Tipo='R') THEN
      IF (NEW.NumAbonadoFinal IS NULL) THEN
        SIGNAL SQLSTATE VALUE '02000'
        SET MESSAGE_TEXT = 'Debe informar el número de abonado inicial y final del rango de abonados';
      ELSE 
         IF (NEW.NumAbonadoFinal<=NEW.NumAbonadoInicial) THEN
           SIGNAL SQLSTATE VALUE '02000'
           SET MESSAGE_TEXT = 'El número de abonado final debe ser mayor que el número de abonado inicial';
         END IF;
      END IF;
	ElSE
      IF (NEW.Tipo='A' AND NEW.NumAbonadoFinal IS NOT NULL) THEN
        SIGNAL SQLSTATE VALUE '02000'
        SET MESSAGE_TEXT = 'Para el tipo=A sólo debe informar el número de abonado inicial';
	    END IF;
	END IF;

END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER `new_cd40_trans`.`llamadas_entrantes_sector_AF_UPDATE`
AFTER UPDATE ON `new_cd40_trans`.`llamadas_entrantes_sector`
FOR EACH ROW
BEGIN
   
  IF (NEW.TIPO<>OLD.TIPO OR NEW.NumAbonadoFinal<>OLD.NumAbonadoFinal OR NEW.NumAbonadoInicial<>OLD.NumAbonadoInicial) THEN
	   
	  IF (NEW.Tipo='R') THEN
		  IF (NEW.NumAbonadoFinal IS NULL) THEN
			SIGNAL SQLSTATE VALUE '02000'
			SET MESSAGE_TEXT = 'Debe informar el número de abonado inicial y final del rango de abonados';
		  ELSE 
			IF (NEW.NumAbonadoFinal<=NEW.NumAbonadoInicial) THEN
			  SIGNAL SQLSTATE VALUE '02000'
			  SET MESSAGE_TEXT = 'El número de abonado final debe ser mayor que el número de abonado inicial';
		    END IF;
          END IF;
	  ElSE
		  IF (NEW.Tipo='A' AND NEW.NumAbonadoFinal IS NOT NULL) THEN
          SIGNAL SQLSTATE VALUE '02000'
          SET MESSAGE_TEXT = 'Para el tipo=A sólo debe informar el número de abonado inicial';
		  END IF;
	  END IF;
  END IF;
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;

--
-- Table structure for table `llamadas_salientes_sector`
--

DROP TABLE IF EXISTS `llamadas_salientes_sector`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `llamadas_salientes_sector` (
  `IdSistema` varchar(32) NOT NULL COMMENT 'Identificador del sistema al que pertenece el sector',
  `IdNucleo` varchar(32) NOT NULL COMMENT 'Identificador del núcleo al que pertenece el sector',
  `IdSector` varchar(700) NOT NULL COMMENT 'Identificador del sector',
  `IdRed` varchar(32) NOT NULL COMMENT 'Identificador único de la red',
  `Tipo` enum('A','R','T') NOT NULL COMMENT 'Tipo: A (Abonado), R (Rango), T (Todas)',
  `NumAbonadoInicial` varchar(45) NOT NULL COMMENT 'Si tipo es A, almacena el número del abonado al que se le permite realizar llamadas.\nSi  tipo es R, almacena el número de abonado inicial del rango al que se le permite realizar llamadas.',
  `NumAbonadoFinal` varchar(45) DEFAULT NULL COMMENT 'Si  tipo es R, almacena el número de abonado final del rango al que se le permite realizar llamadas.\nSi tipo es A, estará vacío',
  `FechaGbd` datetime DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP COMMENT 'Fecha de inserción o modificación del registro en la BD',
  PRIMARY KEY (`IdSistema`,`IdNucleo`,`IdSector`,`IdRed`,`Tipo`,`NumAbonadoInicial`),
  UNIQUE KEY `Llam_Sal_Sector_Rango_idx` (`IdSistema`,`IdNucleo`,`IdSector`,`IdRed`,`Tipo`,`NumAbonadoInicial`,`NumAbonadoFinal`),
  KEY `Llam_Sal_Sector_Red_FK_idx` (`IdSistema`,`IdRed`),
  CONSTRAINT `Llam_Sal_Sector_Red_FK` FOREIGN KEY (`IdSistema`, `IdRed`) REFERENCES `redes` (`IdSistema`, `IdRed`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `Llam_Sal_Sector_Sectores_FK` FOREIGN KEY (`IdSistema`, `IdNucleo`, `IdSector`) REFERENCES `sectores` (`IdSistema`, `IdNucleo`, `IdSector`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COMMENT='Lista blanca de abonados o rango de abonados de cada red con prefijo >= 4 a los que un sector podrá efectuar llamadas ';
/*!40101 SET character_set_client = @saved_cs_client */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER `new_cd40_trans`.`llamadas_salientes_sector_AF_INSERT`
AFTER INSERT ON `new_cd40_trans`.`llamadas_salientes_sector`
FOR EACH ROW
BEGIN
  
  IF (NEW.Tipo='R') THEN
      IF (NEW.NumAbonadoFinal IS NULL) THEN
        SIGNAL SQLSTATE VALUE '02000'
        SET MESSAGE_TEXT = 'Debe informar el número de abonado inicial y final del rango de abonados';
      ELSE 
         IF (NEW.NumAbonadoFinal<=NEW.NumAbonadoInicial) THEN
           SIGNAL SQLSTATE VALUE '02000'
           SET MESSAGE_TEXT = 'El número de abonado final debe ser mayor que el número de abonado inicial';
         END IF;
      END IF;
	ElSE
      IF (NEW.Tipo='A' AND NEW.NumAbonadoFinal IS NOT NULL) THEN
        SIGNAL SQLSTATE VALUE '02000'
        SET MESSAGE_TEXT = 'Para el tipo=A sólo debe informar el número de abonado inicial';
	    END IF;
	END IF;

END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER `new_cd40_trans`.`llamadas_salientes_sector_AF_UPDATE`
AFTER UPDATE ON `new_cd40_trans`.`llamadas_salientes_sector`
FOR EACH ROW
BEGIN
   
  IF (NEW.TIPO<>OLD.TIPO OR NEW.NumAbonadoFinal<>OLD.NumAbonadoFinal OR NEW.NumAbonadoInicial<>OLD.NumAbonadoInicial) THEN
	   
	  IF (NEW.Tipo='R') THEN
		  IF (NEW.NumAbonadoFinal IS NULL) THEN
			SIGNAL SQLSTATE VALUE '02000'
			SET MESSAGE_TEXT = 'Debe informar el número de abonado inicial y final del rango de abonados';
		  ELSE 
			IF (NEW.NumAbonadoFinal<=NEW.NumAbonadoInicial) THEN
			  SIGNAL SQLSTATE VALUE '02000'
			  SET MESSAGE_TEXT = 'El número de abonado final debe ser mayor que el número de abonado inicial';
		    END IF;
          END IF;
	  ElSE
		  IF (NEW.Tipo='A' AND NEW.NumAbonadoFinal IS NOT NULL) THEN
          SIGNAL SQLSTATE VALUE '02000'
          SET MESSAGE_TEXT = 'Para el tipo=A sólo debe informar el número de abonado inicial';
		  END IF;
	  END IF;
  END IF;
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;

--
-- Table structure for table `metodos_bss`
--

DROP TABLE IF EXISTS `metodos_bss`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `metodos_bss` (
  `idmetodos_bss` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`idmetodos_bss`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `metodosbss_destinosradio`
--

DROP TABLE IF EXISTS `metodosbss_destinosradio`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `metodosbss_destinosradio` (
  `idmetodosbss_destinosradio` int(11) NOT NULL AUTO_INCREMENT,
  `metodos_bss_idmetodos_bss` int(11) NOT NULL,
  `metodosbss_destinosradio_fk` int(11) NOT NULL,
  PRIMARY KEY (`idmetodosbss_destinosradio`,`metodos_bss_idmetodos_bss`,`metodosbss_destinosradio_fk`),
  KEY `fk_metodosbss_destinosradio_metodos_bss1_idx` (`metodos_bss_idmetodos_bss`),
  KEY `fk_metodosbss_destinosradio_fk_idx` (`metodosbss_destinosradio_fk`),
  CONSTRAINT `fk_metodosbss_destinosradio_fk` FOREIGN KEY (`metodosbss_destinosradio_fk`) REFERENCES `destinosradio` (`fk_metodosbss`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `fk_metodosbss_destinosradio_metodos_bss1` FOREIGN KEY (`metodos_bss_idmetodos_bss`) REFERENCES `metodos_bss` (`idmetodos_bss`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `multidestinos`
--

DROP TABLE IF EXISTS `multidestinos`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `multidestinos` (
  `IdKey` int(11) NOT NULL AUTO_INCREMENT COMMENT 'Clave Primaria',
  `IdSistema` varchar(32) DEFAULT NULL COMMENT 'Id del Sistema con el que estamos trabajando.',
  `IdNucleo` varchar(32) DEFAULT NULL COMMENT 'Id del Núcleo en la que se encuentra el elemento del Multi Destino.',
  `IdSector` varchar(700) DEFAULT NULL COMMENT 'Id del Sector en el que se encuentra configurado el elemento del Multi Destino.',
  `IdMultiDestino` varchar(32) NOT NULL COMMENT 'Identificador para representar al Multi Destino.',
  `Dominio` varchar(32) DEFAULT 'PROPIO' COMMENT 'Representa si es propio (es conocido por el SCV) o ajeno (trabaja fuera del SCV).',
  `IdSectorizacion` varchar(32) DEFAULT NULL COMMENT 'Id de la Sectorización en la que se encuentra el elemento del  Multi Destino.',
  PRIMARY KEY (`IdKey`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COMMENT='Tabla para gestionar los Multi Destinos (También llamados Grupos.)';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `multidestinos_elementos`
--

DROP TABLE IF EXISTS `multidestinos_elementos`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `multidestinos_elementos` (
  `IdKey` int(11) NOT NULL AUTO_INCREMENT COMMENT 'Clave Primaria.',
  `IdMultiDestino` int(11) NOT NULL COMMENT 'Clave ajena a multidestino.',
  `IdDestino` varchar(700) NOT NULL COMMENT 'Identificador del destino para el Multi Destino.',
  `IdPrefijo` int(10) DEFAULT NULL COMMENT 'Identifica el tipo de Destino que se está añadiendo: 1=LC - 3=ATS - 4=RTB - 5=RTB2 - 7=redMXONE - 0=Interno.',
  PRIMARY KEY (`IdKey`),
  KEY `FK_multi_destinos_idx` (`IdMultiDestino`),
  CONSTRAINT `FK_multi_destinos` FOREIGN KEY (`IdMultiDestino`) REFERENCES `multidestinos` (`IdKey`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COMMENT='Tabla con los elementos (destinos internos o externos) que componen el grupo Multi destino.';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `niveles`
--

DROP TABLE IF EXISTS `niveles`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `niveles` (
  `IdSistema` varchar(32) NOT NULL,
  `IdNucleo` varchar(32) NOT NULL,
  `IdSector` varchar(700) NOT NULL,
  `CICL` int(10) unsigned DEFAULT '0',
  `CIPL` int(10) unsigned DEFAULT '0',
  `CPICL` int(10) unsigned DEFAULT '0',
  `CPIPL` int(10) unsigned DEFAULT '0',
  PRIMARY KEY (`IdSistema`,`IdNucleo`,`IdSector`),
  KEY `Niveles_FKIndex1` (`IdSistema`,`IdNucleo`,`IdSector`),
  CONSTRAINT `niveles_ibfk_1` FOREIGN KEY (`IdSistema`, `IdNucleo`, `IdSector`) REFERENCES `sectores` (`IdSistema`, `IdNucleo`, `IdSector`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `nucleos`
--

DROP TABLE IF EXISTS `nucleos`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `nucleos` (
  `IdSistema` varchar(32) NOT NULL,
  `IdNucleo` varchar(32) NOT NULL,
  PRIMARY KEY (`IdSistema`,`IdNucleo`),
  KEY `Nucleos_FKIndex1` (`IdSistema`),
  CONSTRAINT `nucleos_ibfk_1` FOREIGN KEY (`IdSistema`) REFERENCES `sistema` (`IdSistema`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `operadores`
--

DROP TABLE IF EXISTS `operadores`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `operadores` (
  `IdOperador` varchar(32) NOT NULL,
  `IdSistema` varchar(32) NOT NULL,
  `Clave` varchar(10) DEFAULT NULL,
  `NivelAcceso` int(1) unsigned DEFAULT NULL,
  `Nombre` varchar(32) DEFAULT NULL,
  `Apellidos` varchar(32) DEFAULT NULL,
  `Telefono` varchar(32) DEFAULT NULL,
  `FechaUltAcceso` date DEFAULT NULL,
  `Comentarios` varchar(100) DEFAULT NULL,
  PRIMARY KEY (`IdOperador`,`IdSistema`),
  KEY `Operadores_FKIndex1` (`IdSistema`),
  CONSTRAINT `operadores_ibfk_1` FOREIGN KEY (`IdSistema`) REFERENCES `sistema` (`IdSistema`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `parametrosrecurso`
--

DROP TABLE IF EXISTS `parametrosrecurso`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `parametrosrecurso` (
  `IdSistema` varchar(32) NOT NULL,
  `IdRecurso` varchar(32) NOT NULL,
  `TipoRecurso` int(1) unsigned NOT NULL,
  `GananciaAGCTX` int(1) unsigned DEFAULT '0',
  `GananciaAGCTXdBm` float DEFAULT '0',
  `GananciaAGCRX` int(1) unsigned DEFAULT '0',
  `GananciaAGCRXdBm` float DEFAULT '0',
  `SupresionSilencio` tinyint(1) DEFAULT '1',
  `TamRTP` int(10) unsigned DEFAULT '20',
  `Codec` int(1) unsigned DEFAULT NULL,
  `TipoEM` int(1) DEFAULT '0',
  `GrabacionEd137` tinyint(1) DEFAULT '1',
  `TReleaseBL` int(1) DEFAULT '3' COMMENT 'Tiempo (sg) teniendo información acústica (tonos) antes de pasar el interfaz a reposo. 0<valor<=10 . Aplica en líneas BL.',
  `iDetCallerId` tinyint(1) DEFAULT '0' COMMENT 'Habilita la detección de Caller-Id en llamadas entrantes. 0: Lineas FX0 con detección callerID deshabilitada 1: Deteccion callerId activada. Aplica a líneas de tipo AB.\n',
  `iTmCallerId` int(11) DEFAULT '3000' COMMENT 'Tiempo para detectar CallerId en unidades de 100 ms. Valor: 0< valor <= 50. Aplica a líneas de tipo AB.',
  `iDetInversionPol` tinyint(1) DEFAULT '0' COMMENT 'Tiene en cuenta la inversión de polaridad en el comportamiento de la línea de AB. 0: no tiene encuenta la inversion polaridad 1: tiene en cuenta la inversión de polaridad. Aplica a líneas de tipo AB.\n',
  `iTmLlamEntrante` int(1) DEFAULT '30' COMMENT 'Tiempo maximo llamada entrante en lineas tlf (sg). 0<valor<=60. Aplica a líneas de tipo AB, BL y BC.',
  `iTmDetFinLlamada` int(1) DEFAULT '6' COMMENT 'Tiempo para detectar fin de llamada desde que llega ringoff para lineas de AB (sg). 0<valor<=8. Aplica a líneas AB.',
  `iPeriodoSpvRing` int(2) DEFAULT '200' COMMENT 'Periodo para supervisar señal ring en llamadas entrantes (en ms). 50<=valor<=400. Aplica a líneas AB y BL.',
  `iFiltroSpvRing` int(1) DEFAULT '2' COMMENT 'número de veces que hay que supervisar para dar por válido un valor. 0<valor<=6. Aplica a líneas AB y BL.',
  `iDetDtmf` tinyint(1) DEFAULT '0' COMMENT 'Deteccion dtmf para líneas BC. 0: no detecta 1: detecta. Aplica a líneas BC.\n',
  PRIMARY KEY (`IdSistema`,`IdRecurso`,`TipoRecurso`),
  KEY `ParametrosRecurso_FKIndex1` (`IdSistema`,`IdRecurso`,`TipoRecurso`),
  CONSTRAINT `parametrosrecurso_ibfk_1` FOREIGN KEY (`IdSistema`, `IdRecurso`, `TipoRecurso`) REFERENCES `recursos` (`IdSistema`, `IdRecurso`, `TipoRecurso`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `parametrossector`
--

DROP TABLE IF EXISTS `parametrossector`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `parametrossector` (
  `IdSistema` varchar(32) NOT NULL,
  `IdNucleo` varchar(32) NOT NULL,
  `IdSector` varchar(700) NOT NULL,
  `NumLlamadasEntrantesIDA` int(10) unsigned DEFAULT '5',
  `NumLlamadasEnIDA` int(10) unsigned DEFAULT '10',
  `NumFreqPagina` int(10) unsigned DEFAULT NULL,
  `NumPagFreq` int(10) unsigned DEFAULT NULL,
  `NumDestinosInternosPag` int(10) unsigned DEFAULT NULL,
  `NumPagDestinosInt` int(10) unsigned DEFAULT NULL,
  `Intrusion` tinyint(1) DEFAULT '1',
  `Intruido` tinyint(1) DEFAULT '1',
  `KeepAlivePeriod` int(10) unsigned DEFAULT '200',
  `KeepAliveMultiplier` int(10) unsigned DEFAULT '10',
  `NumEnlacesAI` int(10) unsigned DEFAULT '18',
  `GrabacionEd137` tinyint(1) DEFAULT '1',
  PRIMARY KEY (`IdSistema`,`IdNucleo`,`IdSector`),
  KEY `ParametrosSector_FKIndex1` (`IdSistema`,`IdNucleo`,`IdSector`),
  CONSTRAINT `parametrossector_ibfk_1` FOREIGN KEY (`IdSistema`, `IdNucleo`, `IdSector`) REFERENCES `sectores` (`IdSistema`, `IdNucleo`, `IdSector`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `permisosopedepats`
--

DROP TABLE IF EXISTS `permisosopedepats`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `permisosopedepats` (
  `IdOperador` varchar(32) NOT NULL,
  `IdSistema` varchar(32) NOT NULL,
  `IdDependenciaATS` varchar(32) NOT NULL,
  PRIMARY KEY (`IdOperador`,`IdDependenciaATS`,`IdSistema`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `permisosredes`
--

DROP TABLE IF EXISTS `permisosredes`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `permisosredes` (
  `IdSistema` varchar(32) NOT NULL COMMENT 'Identificador del sistema',
  `IdRed` varchar(32) NOT NULL COMMENT 'Identificador de la red',
  `IdSector` varchar(700) NOT NULL COMMENT 'Identificador del sector',
  `IdNucleo` varchar(32) NOT NULL COMMENT 'Identificador del núcleo al que pertenece el sector',
  `Llamar` enum('true','false') DEFAULT NULL COMMENT 'Indicador de si el sector puede realizar llamadas por esa red',
  `Recibir` enum('true','false') DEFAULT NULL COMMENT 'Indicador de si  el sector puede recibir llamadas',
  `IdRecursoPref` varchar(32) DEFAULT NULL COMMENT 'Identificador del recurso de red predeterminado para encaminar las llamadas salientes',
  `TipoRecursoPref` int(1) unsigned DEFAULT NULL COMMENT 'Tipo del recurso predeterminado para encaminar las llamadas salientes',
  PRIMARY KEY (`IdSistema`,`IdRed`,`IdSector`,`IdNucleo`),
  KEY `PermisosRedes_FKIndex1` (`IdSistema`,`IdRed`),
  KEY `PermisosRedes_FKIndex2` (`IdSistema`,`IdNucleo`,`IdSector`),
  KEY `permisosRedes_recurso_pref_idx` (`IdSistema`,`IdRecursoPref`,`TipoRecursoPref`),
  CONSTRAINT `permisosredes_ibfk_1` FOREIGN KEY (`IdSistema`, `IdRed`) REFERENCES `redes` (`IdSistema`, `IdRed`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `permisosredes_ibfk_2` FOREIGN KEY (`IdSistema`, `IdNucleo`, `IdSector`) REFERENCES `sectores` (`IdSistema`, `IdNucleo`, `IdSector`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `permisosRedes_recurso_pref_fk` FOREIGN KEY (`IdSistema`, `IdRecursoPref`, `TipoRecursoPref`) REFERENCES `recursostf` (`IdSistema`, `IdRecurso`, `TipoRecurso`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COMMENT='Configura los permisos de cada sector en cada red';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `prefijos`
--

DROP TABLE IF EXISTS `prefijos`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `prefijos` (
  `IdSistema` varchar(32) NOT NULL,
  `IdPrefijo` int(10) unsigned NOT NULL,
  PRIMARY KEY (`IdSistema`,`IdPrefijo`),
  KEY `Prefijos_FKIndex1` (`IdSistema`),
  CONSTRAINT `fk_prefijos_sistema` FOREIGN KEY (`IdSistema`) REFERENCES `sistema` (`IdSistema`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `radio`
--

DROP TABLE IF EXISTS `radio`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `radio` (
  `IdSistema` varchar(32) NOT NULL,
  `IdSectorizacion` varchar(32) NOT NULL,
  `IdNucleo` varchar(32) NOT NULL,
  `IdSector` varchar(700) NOT NULL,
  `PosHMI` int(10) unsigned NOT NULL,
  `IdDestino` varchar(700) NOT NULL,
  `Prioridad` int(1) unsigned DEFAULT NULL,
  `PrioridadSIP` int(1) unsigned DEFAULT NULL,
  `ModoOperacion` varchar(1) DEFAULT NULL,
  `Cascos` varchar(1) DEFAULT NULL,
  `Literal` varchar(32) DEFAULT NULL,
  `SupervisionPortadora` tinyint(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`IdSistema`,`IdSectorizacion`,`IdNucleo`,`IdSector`,`PosHMI`,`IdDestino`),
  KEY `Table_42_FKIndex1` (`IdSistema`,`IdSectorizacion`,`IdNucleo`,`IdSector`),
  CONSTRAINT `radio_ibfk_1` FOREIGN KEY (`IdSistema`, `IdSectorizacion`, `IdNucleo`, `IdSector`) REFERENCES `sectoressectorizacion` (`IdSistema`, `IdSectorizacion`, `IdNucleo`, `IdSector`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `radio_param`
--

DROP TABLE IF EXISTS `radio_param`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `radio_param` (
  `idradio_param` int(11) NOT NULL AUTO_INCREMENT,
  `GrsDelay` int(11) DEFAULT '0' COMMENT '0..200 msg\n',
  `OffSetFrequency` int(11) DEFAULT NULL,
  `EnableEventPttSq` tinyint(1) DEFAULT '0',
  `metodos_bss_idmetodos_bss` int(11) NOT NULL,
  PRIMARY KEY (`idradio_param`),
  KEY `fk_radio_param_metodos_bss1_idx` (`metodos_bss_idmetodos_bss`),
  CONSTRAINT `fk_radio_param_metodos_bss1` FOREIGN KEY (`metodos_bss_idmetodos_bss`) REFERENCES `metodos_bss` (`idmetodos_bss`) ON DELETE NO ACTION ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=295 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `rangos`
--

DROP TABLE IF EXISTS `rangos`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `rangos` (
  `IdSistema` varchar(32) NOT NULL,
  `Central` varchar(32) NOT NULL,
  `Tipo` varchar(1) NOT NULL,
  `Inicial` varchar(6) NOT NULL,
  `IdPrefijo` int(10) unsigned NOT NULL,
  `IdAbonado` varchar(32) NOT NULL,
  `Final` varchar(6) DEFAULT NULL,
  PRIMARY KEY (`IdSistema`,`Tipo`,`Central`,`Inicial`),
  KEY `Rangos_FKIndex2` (`IdSistema`,`IdPrefijo`),
  KEY `Rangos_FKIndex4` (`IdSistema`,`Central`),
  CONSTRAINT `FK_Encaminamientos` FOREIGN KEY (`IdSistema`, `Central`) REFERENCES `encaminamientos` (`IdSistema`, `Central`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `recursos`
--

DROP TABLE IF EXISTS `recursos`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `recursos` (
  `IdSistema` varchar(32) NOT NULL,
  `IdRecurso` varchar(32) NOT NULL,
  `TipoRecurso` int(1) unsigned NOT NULL DEFAULT '1',
  `idEquipos` varchar(32) DEFAULT NULL,
  `IdTIFX` varchar(32) DEFAULT NULL,
  `Tipo` int(1) unsigned NOT NULL DEFAULT '0',
  `Interface` int(2) unsigned DEFAULT '0',
  `SlotPasarela` int(1) DEFAULT '-1',
  `NumDispositivoSlot` int(1) DEFAULT '-1',
  `ServidorSIP` varchar(32) DEFAULT '0.0.0.0:5060',
  `Diffserv` tinyint(1) DEFAULT '0',
  PRIMARY KEY (`IdSistema`,`IdRecurso`,`TipoRecurso`),
  KEY `RECURSOS_FKIndex1` (`IdSistema`),
  KEY `RECURSOS_FKIndex2` (`IdSistema`,`IdTIFX`),
  KEY `RECURSOS_FKIndex3` (`IdSistema`,`idEquipos`),
  CONSTRAINT `recursos_ibfk_1` FOREIGN KEY (`IdSistema`) REFERENCES `sistema` (`IdSistema`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `recursos_ibfk_2` FOREIGN KEY (`IdSistema`, `IdTIFX`) REFERENCES `tifx` (`IdSistema`, `IdTIFX`) ON DELETE NO ACTION ON UPDATE CASCADE,
  CONSTRAINT `recursos_ibfk_3` FOREIGN KEY (`IdSistema`, `idEquipos`) REFERENCES `equiposeu` (`IdSistema`, `idEquipos`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `recursoslcen`
--

DROP TABLE IF EXISTS `recursoslcen`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `recursoslcen` (
  `IdSistema` varchar(32) NOT NULL,
  `IdRecurso` varchar(32) NOT NULL,
  `TipoRecurso` int(1) unsigned NOT NULL,
  `IdPrefijo` int(10) unsigned DEFAULT NULL,
  `IdDestino` varchar(700) DEFAULT NULL,
  `TipoDestino` int(10) unsigned DEFAULT NULL,
  `T1` int(10) unsigned DEFAULT '20',
  `T1Max` int(10) unsigned DEFAULT '100',
  `T2` int(10) unsigned DEFAULT '20',
  `T2Max` int(10) unsigned DEFAULT '100',
  `T3` int(10) unsigned DEFAULT '40',
  `T4` int(10) unsigned DEFAULT '40',
  `T4Max` int(10) unsigned DEFAULT '160',
  `T5` int(10) unsigned DEFAULT '60',
  `T5Max` int(10) unsigned DEFAULT '100',
  `T6` int(10) unsigned DEFAULT '5000',
  `T6Max` int(10) unsigned DEFAULT '6000',
  `T8` int(10) unsigned DEFAULT '80',
  `T8Max` int(10) unsigned DEFAULT '150',
  `T9` int(10) unsigned DEFAULT '40',
  `T9Max` int(10) unsigned DEFAULT '60',
  `T10` int(10) unsigned DEFAULT '20',
  `T10Max` int(10) unsigned DEFAULT '130',
  `T11` int(10) unsigned DEFAULT '20',
  `T11Max` int(10) unsigned DEFAULT '130',
  `T12` int(10) unsigned DEFAULT '200',
  `FrqTonoSQ` int(10) unsigned DEFAULT '2280',
  `UmbralTonoSQ` int(11) DEFAULT '-35',
  `FrqTonoPTT` int(10) unsigned DEFAULT '2280',
  `UmbralTonoPTT` int(11) DEFAULT '-10',
  `RefrescoEstados` int(10) unsigned DEFAULT '200',
  `Timeout` int(10) unsigned DEFAULT '600',
  `LongRafagas` int(10) unsigned DEFAULT '6',
  `isuperv_options` tinyint(1) DEFAULT '0' COMMENT 'Supervisa colateral IP. Valor: 0: no supervisa; 1: supervisa. Aplica en líneas LC.',
  `itm_superv_options` int(1) unsigned DEFAULT '10' COMMENT 'Si SupervisionOptions=1, tiempo entre options cuando el colateral está presente (en sg). Valor: 0<valor<20. Aplica en líneas LC.\n',
  PRIMARY KEY (`IdSistema`,`IdRecurso`,`TipoRecurso`),
  KEY `RecursosLCEN_FKIndex1` (`IdSistema`,`IdRecurso`,`TipoRecurso`),
  KEY `RecursosLCEN_FKIndex2` (`IdSistema`,`IdDestino`,`TipoDestino`,`IdPrefijo`),
  CONSTRAINT `recursoslcen_ibfk_1` FOREIGN KEY (`IdSistema`, `IdRecurso`, `TipoRecurso`) REFERENCES `recursos` (`IdSistema`, `IdRecurso`, `TipoRecurso`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `recursosradio`
--

DROP TABLE IF EXISTS `recursosradio`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `recursosradio` (
  `IdSistema` varchar(32) NOT NULL,
  `IdRecurso` varchar(32) NOT NULL,
  `TipoRecurso` int(1) unsigned NOT NULL,
  `zonas_idZonas` int(11) NOT NULL,
  `radio_param_idradio_param` int(11) NOT NULL,
  `tabla_bss_idtabla_bss` int(11) DEFAULT NULL,
  `TipoDestino` int(10) unsigned DEFAULT NULL,
  `IdDestino` varchar(700) DEFAULT NULL,
  `IdEmplazamiento` varchar(32) NOT NULL,
  `EM` tinyint(1) DEFAULT NULL,
  `SQ` varchar(1) DEFAULT NULL,
  `PTT` varchar(1) DEFAULT NULL,
  `FrqTonoE` int(10) unsigned DEFAULT NULL,
  `UmbralTonoE` int(11) DEFAULT NULL,
  `FrqTonoM` int(10) unsigned DEFAULT NULL,
  `UmbralTonoM` int(11) DEFAULT NULL,
  `FrqTonoSQ` int(10) unsigned DEFAULT NULL,
  `UmbralTonoSQ` int(11) DEFAULT NULL,
  `FrqTonoPTT` int(10) unsigned DEFAULT NULL,
  `UmbralTonoPTT` int(11) DEFAULT NULL,
  `BSS` tinyint(1) DEFAULT NULL,
  `NTZ` tinyint(1) DEFAULT NULL,
  `TipoNTZ` int(1) unsigned DEFAULT '0',
  `Cifrado` tinyint(1) DEFAULT NULL,
  `SupervPortadoraTX` tinyint(1) DEFAULT NULL,
  `SupervModuladoraTX` tinyint(1) DEFAULT NULL,
  `ModoConfPTT` int(1) unsigned DEFAULT NULL,
  `RepSQyBSS` int(10) unsigned DEFAULT '1',
  `DesactivacionSQ` int(10) unsigned DEFAULT '1',
  `TimeoutPTT` int(10) unsigned DEFAULT '200',
  `UmbralVAD` int(11) DEFAULT '-33',
  `TiempoPTT` int(10) unsigned DEFAULT '120',
  `NumFlujosAudio` int(1) unsigned DEFAULT '1',
  `KeepAlivePeriod` int(10) unsigned DEFAULT '200',
  `KeepAliveMultiplier` int(10) unsigned DEFAULT '10',
  `RedundanciaRol` enum('P','R') DEFAULT NULL COMMENT 'Rol del recurso si está asociado a un destino Radio con redundancia (1+1): P (Principal), R (Reserva). En otro caso, tomará el valor null.',
  `RedundanciaIdPareja` varchar(70) DEFAULT NULL COMMENT 'Identificador único del grupo de redundancia. Se compone como IdRecursoPrincipal#idRecursoRerva',
  PRIMARY KEY (`IdSistema`,`IdRecurso`,`TipoRecurso`),
  KEY `RecursosRadio_FKIndex1` (`IdSistema`,`IdRecurso`,`TipoRecurso`),
  KEY `RecursosRadio_FKIndex2` (`IdSistema`,`IdEmplazamiento`),
  KEY `RecursosRadio_FKIndex3` (`IdSistema`,`IdDestino`,`TipoDestino`),
  KEY `fk_recursosradio_zonas1_idx` (`zonas_idZonas`),
  KEY `fk_recursosradio_radio_param1_idx` (`radio_param_idradio_param`),
  KEY `fk_recursosradio_tabla_bss1_idx` (`tabla_bss_idtabla_bss`),
  CONSTRAINT `fk_recursosradio_radio_param1` FOREIGN KEY (`radio_param_idradio_param`) REFERENCES `radio_param` (`idradio_param`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `fk_recursosradio_tabla_bss1` FOREIGN KEY (`tabla_bss_idtabla_bss`) REFERENCES `tabla_bss` (`idtabla_bss`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `fk_recursosradio_zonas1` FOREIGN KEY (`zonas_idZonas`) REFERENCES `zonas` (`idZonas`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `recursosradio_ibfk_1` FOREIGN KEY (`IdSistema`, `IdRecurso`, `TipoRecurso`) REFERENCES `recursos` (`IdSistema`, `IdRecurso`, `TipoRecurso`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `recursosradio_ibfk_2` FOREIGN KEY (`IdSistema`, `IdEmplazamiento`) REFERENCES `emplazamientos` (`IdSistema`, `IdEmplazamiento`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `recursosradio_ibfk_3` FOREIGN KEY (`IdSistema`, `IdDestino`, `TipoDestino`) REFERENCES `destinosradio` (`IdSistema`, `IdDestino`, `TipoDestino`) ON DELETE NO ACTION ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `recursostf`
--

DROP TABLE IF EXISTS `recursostf`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `recursostf` (
  `IdSistema` varchar(32) NOT NULL,
  `IdRecurso` varchar(32) NOT NULL,
  `TipoRecurso` int(1) unsigned NOT NULL,
  `IdPrefijo` int(10) unsigned DEFAULT NULL,
  `TipoDestino` int(10) unsigned DEFAULT NULL,
  `IdDestino` varchar(700) DEFAULT NULL,
  `IdTroncal` varchar(32) DEFAULT NULL,
  `IdRed` varchar(32) DEFAULT NULL,
  `Lado` varchar(5) NOT NULL DEFAULT 'A',
  `Modo` varchar(10) DEFAULT NULL,
  PRIMARY KEY (`IdSistema`,`IdRecurso`,`TipoRecurso`),
  KEY `RecursosTF_FKIndex1` (`IdSistema`,`IdRecurso`,`TipoRecurso`),
  KEY `RecursosTF_FKIndex2` (`IdSistema`,`IdRed`),
  KEY `RecursosTF_FKIndex3` (`IdSistema`,`IdTroncal`),
  KEY `RecursosTF_FKIndex4` (`IdSistema`,`IdDestino`,`TipoDestino`,`IdPrefijo`),
  CONSTRAINT `recursostf_ibfk_1` FOREIGN KEY (`IdSistema`, `IdRecurso`, `TipoRecurso`) REFERENCES `recursos` (`IdSistema`, `IdRecurso`, `TipoRecurso`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `recursostf_ibfk_2` FOREIGN KEY (`IdSistema`, `IdRed`) REFERENCES `redes` (`IdSistema`, `IdRed`) ON DELETE NO ACTION ON UPDATE CASCADE,
  CONSTRAINT `recursostf_ibfk_3` FOREIGN KEY (`IdSistema`, `IdTroncal`) REFERENCES `troncales` (`IdSistema`, `IdTroncal`) ON DELETE NO ACTION ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `redes`
--

DROP TABLE IF EXISTS `redes`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `redes` (
  `IdSistema` varchar(32) NOT NULL,
  `IdRed` varchar(32) NOT NULL,
  `IdPrefijo` int(10) unsigned NOT NULL,
  PRIMARY KEY (`IdSistema`,`IdRed`),
  UNIQUE KEY `Redes_Unique_Pref_idx` (`IdSistema`,`IdPrefijo`),
  KEY `REDES_FKIndex1` (`IdSistema`),
  CONSTRAINT `redes_ibfk_1` FOREIGN KEY (`IdSistema`) REFERENCES `sistema` (`IdSistema`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `redes_ibfk_2` FOREIGN KEY (`IdSistema`, `IdPrefijo`) REFERENCES `prefijos` (`IdSistema`, `IdPrefijo`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER `new_cd40_trans`.`REDDELETE`
BEFORE DELETE ON `new_cd40_trans`.`redes`
FOR EACH ROW
BEGIN
UPDATE RECURSOSTF SET IdRed=NULL 
	WHERE IdRed=old.IdRed AND IdSistema=old.IdSistema;
DELETE FROM destinostelefonia WHERE
  IdSistema=old.IdSistema AND
  TipoDestino=1 AND
  IdPrefijo=old.IdPrefijo;
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;

--
-- Table structure for table `rutas`
--

DROP TABLE IF EXISTS `rutas`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `rutas` (
  `IdSistema` varchar(32) NOT NULL,
  `IdRuta` varchar(32) NOT NULL,
  `Central` varchar(32) NOT NULL,
  `Tipo` varchar(1) DEFAULT NULL,
  `Orden` int(11) DEFAULT NULL,
  PRIMARY KEY (`IdSistema`,`IdRuta`,`Central`),
  KEY `Rutas_FKIndex1` (`IdSistema`,`Central`),
  CONSTRAINT `rutas_ibfk_1` FOREIGN KEY (`IdSistema`, `Central`) REFERENCES `encaminamientos` (`IdSistema`, `Central`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `sectores`
--

DROP TABLE IF EXISTS `sectores`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `sectores` (
  `IdSistema` varchar(32) NOT NULL,
  `IdNucleo` varchar(32) NOT NULL,
  `IdSector` varchar(700) NOT NULL,
  `IdSistemaParejaUCS` varchar(32) DEFAULT NULL,
  `IdNucleoParejaUCS` varchar(32) DEFAULT NULL,
  `IdParejaUCS` varchar(32) DEFAULT NULL,
  `SectorSimple` tinyint(1) DEFAULT '1',
  `Tipo` varchar(1) DEFAULT 'R',
  `TipoPosicion` varchar(1) DEFAULT 'C',
  `PrioridadR2` int(1) unsigned DEFAULT '4',
  `TipoHMI` int(10) unsigned DEFAULT NULL,
  `NumSacta` int(10) unsigned DEFAULT NULL,
  `SeleccionadoFS` tinyint(1) DEFAULT '0' COMMENT 'Modificación para marca de sector seleccionado modelo Fuera Sectorización',
  `IdDependenciaATS` varchar(32) DEFAULT NULL,
  PRIMARY KEY (`IdSistema`,`IdNucleo`,`IdSector`),
  KEY `Sectores_FKIndex1` (`IdSistema`,`IdNucleo`),
  KEY `Sectores_FKIndex3` (`IdSistemaParejaUCS`,`IdNucleoParejaUCS`,`IdParejaUCS`),
  CONSTRAINT `sectores_ibfk_1` FOREIGN KEY (`IdSistema`, `IdNucleo`) REFERENCES `nucleos` (`IdSistema`, `IdNucleo`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `sectores_ibfk_3` FOREIGN KEY (`IdSistemaParejaUCS`, `IdNucleoParejaUCS`, `IdParejaUCS`) REFERENCES `sectores` (`IdSistema`, `IdNucleo`, `IdSector`) ON DELETE NO ACTION ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER `new_cd40_trans`.`AltaSector`
AFTER INSERT ON `new_cd40_trans`.`sectores`
FOR EACH ROW
BEGIN
    INSERT INTO DestinosInternos SET IdSistema=new.IdSistema,
																			IdDestino=new.IdSector,
																			TipoDestino=2,
																			IdPrefijo=0,
																			IdSector=new.IdSector,
																			IdNucleo=new.IdNucleo;
    INSERT INTO DestinosInternos SET IdSistema=new.IdSistema,
																			IdDestino=new.IdSector,
																			TipoDestino=2,
																			IdPrefijo=2,
																			IdSector=new.IdSector,
																			IdNucleo=new.IdNucleo ;
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER `new_cd40_trans`.`ActualizaSector`
AFTER UPDATE ON `new_cd40_trans`.`sectores`
FOR EACH ROW
BEGIN
    UPDATE Destinos 
			SET IdDestino=new.IdSector
			WHERE IdSistema=new.IdSistema AND
													IdDestino=old.IdSector AND
													TipoDestino=2 ;
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER `new_cd40_trans`.`BajaSector`
BEFORE DELETE ON `new_cd40_trans`.`sectores`
FOR EACH ROW
BEGIN
    DELETE FROM Destinos WHERE IdSistema=old.IdSistema AND
																IdDestino=old.IdSector AND
																TipoDestino=2 ;
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;

--
-- Table structure for table `sectoresagrupacion`
--

DROP TABLE IF EXISTS `sectoresagrupacion`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `sectoresagrupacion` (
  `IdSistema` varchar(32) NOT NULL,
  `IdAgrupacion` varchar(32) NOT NULL,
  `IdSector` varchar(700) NOT NULL,
  `IdNucleo` varchar(32) NOT NULL,
  PRIMARY KEY (`IdSistema`,`IdAgrupacion`,`IdSector`,`IdNucleo`),
  KEY `Sectores_has_Agrupaciones_FKIndex1` (`IdSistema`,`IdNucleo`,`IdSector`),
  KEY `Sectores_has_Agrupaciones_FKIndex2` (`IdSistema`,`IdAgrupacion`),
  CONSTRAINT `sectoresagrupacion_ibfk_1` FOREIGN KEY (`IdSistema`, `IdNucleo`, `IdSector`) REFERENCES `sectores` (`IdSistema`, `IdNucleo`, `IdSector`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `sectoresagrupacion_ibfk_2` FOREIGN KEY (`IdSistema`, `IdAgrupacion`) REFERENCES `agrupaciones` (`IdSistema`, `IdAgrupacion`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Temporary view structure for view `sectoresentopsparainformexml`
--

DROP TABLE IF EXISTS `sectoresentopsparainformexml`;
/*!50001 DROP VIEW IF EXISTS `sectoresentopsparainformexml`*/;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
/*!50001 CREATE VIEW `sectoresentopsparainformexml` AS SELECT 
 1 AS `Idsistema`,
 1 AS `IdSectorizacion`,
 1 AS `IdToP`,
 1 AS `IdSector`,
 1 AS `IdNucleo`,
 1 AS `IdSectorOriginal`*/;
SET character_set_client = @saved_cs_client;

--
-- Table structure for table `sectoressector`
--

DROP TABLE IF EXISTS `sectoressector`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `sectoressector` (
  `IdSistema` varchar(32) NOT NULL,
  `IdNucleo` varchar(32) NOT NULL,
  `IdSector` varchar(700) NOT NULL,
  `IdSectorOriginal` varchar(700) NOT NULL,
  `EsDominante` tinyint(1) DEFAULT '0',
  PRIMARY KEY (`IdSistema`,`IdNucleo`,`IdSector`,`IdSectorOriginal`),
  KEY `SectoresSector_FKIndex1` (`IdSistema`,`IdNucleo`,`IdSector`),
  KEY `SectoresSector_FKIndex2` (`IdSectorOriginal`),
  CONSTRAINT `sectoressector_ibfk_1` FOREIGN KEY (`IdSistema`, `IdNucleo`, `IdSector`) REFERENCES `sectores` (`IdSistema`, `IdNucleo`, `IdSector`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `sectoressectorizacion`
--

DROP TABLE IF EXISTS `sectoressectorizacion`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `sectoressectorizacion` (
  `IdSistema` varchar(32) NOT NULL,
  `IdSectorizacion` varchar(32) NOT NULL,
  `IdNucleo` varchar(32) NOT NULL,
  `IdSector` varchar(700) NOT NULL,
  `IdTOP` varchar(32) NOT NULL,
  PRIMARY KEY (`IdSistema`,`IdSectorizacion`,`IdNucleo`,`IdSector`,`IdTOP`),
  KEY `SectoresSectorizacion_FKIndex1` (`IdSistema`,`IdSectorizacion`),
  KEY `SectoresSectorizacion_FKIndex2` (`IdSistema`,`IdNucleo`,`IdSector`),
  KEY `SectoresSectorizacion_FKIndex3` (`IdSistema`,`IdTOP`),
  CONSTRAINT `sectoressectorizacion_ibfk_1` FOREIGN KEY (`IdSistema`, `IdSectorizacion`) REFERENCES `sectorizaciones` (`IdSistema`, `IdSectorizacion`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `sectoressectorizacion_ibfk_2` FOREIGN KEY (`IdSistema`, `IdNucleo`, `IdSector`) REFERENCES `sectores` (`IdSistema`, `IdNucleo`, `IdSector`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `sectoressectorizacion_ibfk_3` FOREIGN KEY (`IdSistema`, `IdTOP`) REFERENCES `top` (`IdSistema`, `IdTOP`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `sectorizaciones`
--

DROP TABLE IF EXISTS `sectorizaciones`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `sectorizaciones` (
  `IdSistema` varchar(32) NOT NULL,
  `IdSectorizacion` varchar(32) NOT NULL,
  `Activa` tinyint(1) DEFAULT NULL,
  `FechaActivacion` datetime DEFAULT NULL,
  PRIMARY KEY (`IdSistema`,`IdSectorizacion`),
  KEY `Sectorizaciones_FKIndex1` (`IdSistema`),
  CONSTRAINT `sectorizaciones_ibfk_1` FOREIGN KEY (`IdSistema`) REFERENCES `sistema` (`IdSistema`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `sistema`
--

DROP TABLE IF EXISTS `sistema`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `sistema` (
  `IdSistema` varchar(32) NOT NULL,
  `TiempoPTT` int(10) unsigned DEFAULT NULL,
  `TiempoSinJack1` int(10) unsigned DEFAULT NULL,
  `TiempoSinJack2` int(10) unsigned DEFAULT NULL,
  `TamLiteralEnlExt` int(10) unsigned DEFAULT '32',
  `TamLiteralDA` int(10) unsigned DEFAULT '32',
  `TamLiteralIA` int(10) unsigned DEFAULT '32',
  `TamLiteralAG` int(10) unsigned DEFAULT '32',
  `TamLiteralEmplazamiento` int(10) unsigned DEFAULT '32',
  `VersionIP` int(1) unsigned DEFAULT NULL,
  `GrupoMulticastConfiguracion` varchar(15) DEFAULT NULL,
  `PuertoMulticastConfiguracion` int(5) unsigned DEFAULT '1000',
  `EstadoScvA` int(1) unsigned DEFAULT '0',
  `EstadoScvB` int(1) unsigned DEFAULT '0',
  `NumLlamadasEnIDA` int(10) unsigned DEFAULT '10',
  `KeepAliveMultiplier` int(10) unsigned DEFAULT '10',
  `KeepAlivePeriod` int(10) unsigned DEFAULT '200',
  `NumPagDestinosInt` int(10) unsigned DEFAULT NULL,
  `NumDestinosInternosPag` int(10) unsigned DEFAULT NULL,
  `NumPagFreq` int(10) unsigned DEFAULT NULL,
  `NumFreqPagina` int(10) unsigned DEFAULT NULL,
  `NumLlamadasEntrantesIDA` int(10) unsigned DEFAULT '5',
  `NumEnlacesAI` int(10) unsigned DEFAULT '18',
  `NumMaxSectores` int(10) unsigned DEFAULT '32',
  PRIMARY KEY (`IdSistema`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER `new_cd40_trans`.`AltaSistema`
AFTER INSERT ON `new_cd40_trans`.`sistema`
FOR EACH ROW
BEGIN
    INSERT INTO Prefijos VALUES (new.IdSistema,0);
    INSERT INTO Prefijos VALUES (new.IdSistema,1);
    INSERT INTO Prefijos VALUES (new.IdSistema,2);
    INSERT INTO Prefijos VALUES (new.IdSistema,3);
    INSERT INTO Prefijos VALUES (new.IdSistema,4);
    INSERT INTO Prefijos VALUES (new.IdSistema,5);
    INSERT INTO Prefijos VALUES (new.IdSistema,6);
    INSERT INTO Prefijos VALUES (new.IdSistema,7);
    INSERT INTO Prefijos VALUES (new.IdSistema,8);
    INSERT INTO Prefijos VALUES (new.IdSistema,9);
    INSERT INTO Prefijos VALUES (new.IdSistema,10);
    INSERT INTO Prefijos VALUES (new.IdSistema,11);
    INSERT INTO Prefijos VALUES (new.IdSistema,12);
    INSERT INTO Prefijos VALUES (new.IdSistema,13);
    INSERT INTO Prefijos VALUES (new.IdSistema,14);
    INSERT INTO Prefijos VALUES (new.IdSistema,15);
    INSERT INTO Prefijos VALUES (new.IdSistema,16);
    INSERT INTO Prefijos VALUES (new.IdSistema,17);
    INSERT INTO Prefijos VALUES (new.IdSistema,18);
    INSERT INTO Prefijos VALUES (new.IdSistema,19);
    INSERT INTO Prefijos VALUES (new.IdSistema,20);
    INSERT INTO Prefijos VALUES (new.IdSistema,21);
    INSERT INTO Prefijos VALUES (new.IdSistema,22);
    INSERT INTO Prefijos VALUES (new.IdSistema,23);
    INSERT INTO Prefijos VALUES (new.IdSistema,24);
    INSERT INTO Prefijos VALUES (new.IdSistema,25);
    INSERT INTO Prefijos VALUES (new.IdSistema,26);
    INSERT INTO Prefijos VALUES (new.IdSistema,27);
    INSERT INTO Prefijos VALUES (new.IdSistema,28);
    INSERT INTO Prefijos VALUES (new.IdSistema,29);
    INSERT INTO Prefijos VALUES (new.IdSistema,30);
    INSERT INTO Prefijos VALUES (new.IdSistema,31);
    INSERT INTO Prefijos VALUES (new.IdSistema,32);
		
	INSERT INTO Redes VALUES (new.IdSistema,"ATS",3) ;    

	UPDATE ParametrosSector SET NumLlamadasEntrantesIDA=new.NumLlamadasEntrantesIDA,
                                NumLlamadasEnIDA=new.NumLlamadasEnIda,
                                NumFreqPagina=new.NumFreqPagina,
                                NumPagFreq=new.NumPagFreq,
                                NumDestinosInternosPag=new.NumDestinosInternosPag,
                                NumPagDestinosInt=new.NumPagDestinosInt,
								KeepAliveMultiplier=new.KeepAliveMultiplier,
								KeepAlivePeriod=new.KeepAlivePeriod,
								NumEnlacesAI=new.NumEnlacesAI
								 WHERE IdSistema=new.IdSistema;

END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER `new_cd40_trans`.`sistema_AUPD`
AFTER UPDATE ON `new_cd40_trans`.`sistema`
FOR EACH ROW
UPDATE ParametrosSector SET NumLlamadasEntrantesIDA=new.NumLlamadasEntrantesIDA,
                                NumLlamadasEnIDA=new.NumLlamadasEnIda,
                                NumFreqPagina=new.NumFreqPagina,
                                NumPagFreq=new.NumPagFreq,
                                NumDestinosInternosPag=new.NumDestinosInternosPag,
                                NumPagDestinosInt=new.NumPagDestinosInt,
								KeepAliveMultiplier=new.KeepAliveMultiplier,
								KeepAlivePeriod=new.KeepAlivePeriod,
								NumEnlacesAI=new.NumEnlacesAI
								 WHERE IdSistema=new.IdSistema */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;

--
-- Table structure for table `tabla_bss`
--

DROP TABLE IF EXISTS `tabla_bss`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tabla_bss` (
  `idtabla_bss` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(32) DEFAULT NULL,
  `description` varchar(128) DEFAULT NULL,
  `UsuarioCreacion` varchar(32) DEFAULT NULL,
  `FechaCreacion` datetime DEFAULT CURRENT_TIMESTAMP,
  `UsuarioModificacion` varchar(32) DEFAULT NULL,
  `FechaModificacion` datetime DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`idtabla_bss`)
) ENGINE=InnoDB AUTO_INCREMENT=8 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `tablasmodificadas`
--

DROP TABLE IF EXISTS `tablasmodificadas`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tablasmodificadas` (
  `IdTabla` varchar(32) NOT NULL,
  PRIMARY KEY (`IdTabla`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `teclassector`
--

DROP TABLE IF EXISTS `teclassector`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `teclassector` (
  `IdSistema` varchar(32) NOT NULL,
  `IdNucleo` varchar(32) NOT NULL,
  `IdSector` varchar(700) NOT NULL,
  `TransConConsultaPrev` tinyint(1) DEFAULT '1',
  `TransDirecta` tinyint(1) DEFAULT '0',
  `Conferencia` tinyint(1) DEFAULT '0',
  `Escucha` tinyint(1) DEFAULT '0',
  `Retener` tinyint(1) DEFAULT '1',
  `Captura` tinyint(1) DEFAULT '0',
  `Redireccion` tinyint(1) DEFAULT '0',
  `RepeticionUltLlamada` tinyint(1) DEFAULT '1',
  `RellamadaAut` tinyint(1) DEFAULT '0',
  `TeclaPrioridad` tinyint(1) DEFAULT '1',
  `Tecla55mas1` tinyint(1) DEFAULT '0',
  `Monitoring` tinyint(1) DEFAULT '1',
  `CoordinadorTF` tinyint(1) DEFAULT '0',
  `CoordinadorRD` tinyint(1) DEFAULT '0',
  `IntegracionRDTF` tinyint(1) DEFAULT '0',
  `LlamadaSelectiva` tinyint(1) DEFAULT '0',
  `GrupoBSS` tinyint(1) DEFAULT '1',
  `LTT` tinyint(1) DEFAULT '1',
  `SayAgain` tinyint(1) DEFAULT '1',
  `InhabilitacionRedirec` tinyint(1) DEFAULT '0',
  `Glp` tinyint(1) DEFAULT '0',
  PRIMARY KEY (`IdSistema`,`IdNucleo`,`IdSector`),
  KEY `TeclasSector_FKIndex1` (`IdSistema`,`IdNucleo`,`IdSector`),
  CONSTRAINT `teclassector_ibfk_1` FOREIGN KEY (`IdSistema`, `IdNucleo`, `IdSector`) REFERENCES `sectores` (`IdSistema`, `IdNucleo`, `IdSector`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `tifx`
--

DROP TABLE IF EXISTS `tifx`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tifx` (
  `IdSistema` varchar(32) NOT NULL,
  `IdTIFX` varchar(32) NOT NULL,
  `ModoArranque` varchar(1) DEFAULT NULL,
  `ModoSincronizacion` int(1) unsigned DEFAULT NULL,
  `Master` varchar(32) DEFAULT '0.0.0.0:0',
  `SNMPPortLocal` int(10) unsigned DEFAULT '161',
  `SNMPPortRemoto` int(10) unsigned DEFAULT '161',
  `SNMPTraps` int(10) unsigned DEFAULT '162',
  `SIPPortLocal` int(10) unsigned DEFAULT '5060',
  `TimeSupervision` int(10) unsigned DEFAULT '0',
  `IpRed1` varchar(60) DEFAULT NULL,
  `IpRed2` varchar(60) DEFAULT NULL,
  `Grabador1` varchar(45) DEFAULT NULL,
  `Grabador2` varchar(45) DEFAULT NULL,
  `iSupervLanGW` tinyint(1) NOT NULL DEFAULT '0' COMMENT 'Incorpora a la supervision ethernet de la pasarela la conexión a la Puerta de enlace. 0: no supervisa conexion GW; 1: supervisa GW.',
  `itmmaxSupervLanGW` int(1) NOT NULL DEFAULT '1' COMMENT 'Si supervisionLanGW=1,  tiempo maximo (en sg) de espera conexion GW. Si vence es que no hay ping a la puerta de enlace. Este tiempo tiene que ser menor de 5 sg.\n',
  PRIMARY KEY (`IdSistema`,`IdTIFX`),
  KEY `TIFX_FKIndex1` (`IdSistema`),
  CONSTRAINT `tifx_ibfk_1` FOREIGN KEY (`IdSistema`) REFERENCES `sistema` (`IdSistema`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER `new_cd40_trans`.`TIFXDELETE`
BEFORE DELETE ON `new_cd40_trans`.`tifx`
FOR EACH ROW
UPDATE RECURSOS SET IdTIFX=NULL 
		WHERE IdSistema = old.IdSistema AND IdTIFX= old.IdTIFX */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;

--
-- Table structure for table `top`
--

DROP TABLE IF EXISTS `top`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `top` (
  `IdSistema` varchar(32) NOT NULL,
  `IdTOP` varchar(32) NOT NULL,
  `PosicionSacta` int(10) unsigned DEFAULT NULL,
  `ModoArranque` varchar(1) DEFAULT NULL,
  `IpRed1` varchar(60) DEFAULT NULL,
  `IpRed2` varchar(60) DEFAULT NULL,
  `ConexionJacksEjecutivo` tinyint(1) DEFAULT '1',
  `ConexionJacksAyudante` tinyint(1) DEFAULT '1',
  `NumAltavoces` smallint(1) DEFAULT '8',
  `Grabador1` varchar(45) DEFAULT NULL,
  `Grabador2` varchar(45) DEFAULT NULL,
  `IdDependenciaATS` varchar(32) DEFAULT NULL,
  PRIMARY KEY (`IdSistema`,`IdTOP`),
  KEY `TOP_FKIndex1` (`IdSistema`),
  CONSTRAINT `top_ibfk_1` FOREIGN KEY (`IdSistema`) REFERENCES `sistema` (`IdSistema`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `troncales`
--

DROP TABLE IF EXISTS `troncales`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `troncales` (
  `IdSistema` varchar(32) NOT NULL,
  `IdTroncal` varchar(32) NOT NULL,
  `NumTest` varchar(16) DEFAULT NULL,
  PRIMARY KEY (`IdSistema`,`IdTroncal`),
  KEY `Troncales_FKIndex1` (`IdSistema`),
  CONSTRAINT `troncales_ibfk_1` FOREIGN KEY (`IdSistema`) REFERENCES `sistema` (`IdSistema`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER `new_cd40_trans`.`TRONCALDELETE`
BEFORE DELETE ON `new_cd40_trans`.`troncales`
FOR EACH ROW
UPDATE RECURSOSTF SET IdTroncal=NULL 
		WHERE IdTroncal=old.IdTroncal AND IdSistema=old.IdSistema */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;

--
-- Table structure for table `troncalesruta`
--

DROP TABLE IF EXISTS `troncalesruta`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `troncalesruta` (
  `IdSistema` varchar(32) NOT NULL,
  `IdTroncal` varchar(32) NOT NULL,
  `Central` varchar(32) NOT NULL,
  `IdRuta` varchar(32) NOT NULL,
  PRIMARY KEY (`IdSistema`,`IdTroncal`,`Central`,`IdRuta`),
  KEY `TroncalesRuta_FKIndex1` (`IdSistema`,`IdTroncal`),
  KEY `TroncalesRuta_FKIndex2` (`IdSistema`,`IdRuta`,`Central`),
  CONSTRAINT `troncalesruta_ibfk_1` FOREIGN KEY (`IdSistema`, `IdTroncal`) REFERENCES `troncales` (`IdSistema`, `IdTroncal`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `troncalesruta_ibfk_2` FOREIGN KEY (`IdSistema`, `IdRuta`, `Central`) REFERENCES `rutas` (`IdSistema`, `IdRuta`, `Central`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `usuariosabonados`
--

DROP TABLE IF EXISTS `usuariosabonados`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `usuariosabonados` (
  `IdSistema` varchar(32) NOT NULL,
  `IdPrefijo` int(10) unsigned NOT NULL,
  `IdNucleo` varchar(32) NOT NULL,
  `IdSector` varchar(700) NOT NULL,
  `IdAbonado` varchar(32) NOT NULL,
  PRIMARY KEY (`IdSistema`,`IdPrefijo`,`IdNucleo`,`IdSector`,`IdAbonado`),
  KEY `UsuariosAbonados_FKIndex2` (`IdSistema`,`IdPrefijo`),
  KEY `UsuariosAbonados_FKIndex3` (`IdSistema`,`IdNucleo`,`IdSector`),
  KEY `UsuariosAbonados_FKIndex4` (`IdSector`),
  CONSTRAINT `usuariosabonados_ibfk_2` FOREIGN KEY (`IdSistema`, `IdPrefijo`) REFERENCES `redes` (`IdSistema`, `IdPrefijo`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `usuariosabonados_ibfk_3` FOREIGN KEY (`IdSistema`, `IdNucleo`, `IdSector`) REFERENCES `sectores` (`IdSistema`, `IdNucleo`, `IdSector`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `valores_tabla`
--

DROP TABLE IF EXISTS `valores_tabla`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `valores_tabla` (
  `idvalores_tabla` int(11) NOT NULL AUTO_INCREMENT,
  `tabla_bss_idtabla_bss` int(11) NOT NULL,
  `valor_prop` int(11) NOT NULL,
  `valor_rssi` int(11) NOT NULL,
  PRIMARY KEY (`idvalores_tabla`,`tabla_bss_idtabla_bss`),
  KEY `fk_valores_tabla_tabla_bss1_idx` (`tabla_bss_idtabla_bss`),
  CONSTRAINT `fk_valores_tabla_tabla_bss1` FOREIGN KEY (`tabla_bss_idtabla_bss`) REFERENCES `tabla_bss` (`idtabla_bss`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=99 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Temporary view structure for view `viewdestinostelefonia`
--

DROP TABLE IF EXISTS `viewdestinostelefonia`;
/*!50001 DROP VIEW IF EXISTS `viewdestinostelefonia`*/;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
/*!50001 CREATE VIEW `viewdestinostelefonia` AS SELECT 
 1 AS `IdSistema`,
 1 AS `IdDestino`,
 1 AS `TipoDestino`,
 1 AS `IdGrupo`,
 1 AS `IdPrefijo`,
 1 AS `IdAbonado`,
 1 AS `IdRed`*/;
SET character_set_client = @saved_cs_client;

--
-- Temporary view structure for view `viewequiposexternos`
--

DROP TABLE IF EXISTS `viewequiposexternos`;
/*!50001 DROP VIEW IF EXISTS `viewequiposexternos`*/;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
/*!50001 CREATE VIEW `viewequiposexternos` AS SELECT 
 1 AS `idEquipos`,
 1 AS `IpRed1`,
 1 AS `tipoEquipo`,
 1 AS `modeloequipo`,
 1 AS `tiporadio`*/;
SET character_set_client = @saved_cs_client;

--
-- Temporary view structure for view `viewincidenciasmasalarma`
--

DROP TABLE IF EXISTS `viewincidenciasmasalarma`;
/*!50001 DROP VIEW IF EXISTS `viewincidenciasmasalarma`*/;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
/*!50001 CREATE VIEW `viewincidenciasmasalarma` AS SELECT 
 1 AS `IdSistema`,
 1 AS `IdIncidencia`,
 1 AS `Incidencia`,
 1 AS `alarma`*/;
SET character_set_client = @saved_cs_client;

--
-- Temporary view structure for view `viewincidenciasmasalarma_ingles`
--

DROP TABLE IF EXISTS `viewincidenciasmasalarma_ingles`;
/*!50001 DROP VIEW IF EXISTS `viewincidenciasmasalarma_ingles`*/;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
/*!50001 CREATE VIEW `viewincidenciasmasalarma_ingles` AS SELECT 
 1 AS `IdSistema`,
 1 AS `IdIncidencia`,
 1 AS `Incidencia`,
 1 AS `alarma`*/;
SET character_set_client = @saved_cs_client;

--
-- Temporary view structure for view `viewrecursosimplicadosrutas`
--

DROP TABLE IF EXISTS `viewrecursosimplicadosrutas`;
/*!50001 DROP VIEW IF EXISTS `viewrecursosimplicadosrutas`*/;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
/*!50001 CREATE VIEW `viewrecursosimplicadosrutas` AS SELECT 
 1 AS `idsistema`,
 1 AS `idrecurso`,
 1 AS `iddestino`,
 1 AS `idred`,
 1 AS `idtroncal`,
 1 AS `idruta`,
 1 AS `tipo`*/;
SET character_set_client = @saved_cs_client;

--
-- Temporary view structure for view `viewsectoresenagrupacion`
--

DROP TABLE IF EXISTS `viewsectoresenagrupacion`;
/*!50001 DROP VIEW IF EXISTS `viewsectoresenagrupacion`*/;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
/*!50001 CREATE VIEW `viewsectoresenagrupacion` AS SELECT 
 1 AS `contador`,
 1 AS `IdAgrupacion`*/;
SET character_set_client = @saved_cs_client;

--
-- Temporary view structure for view `viewsectoresentops`
--

DROP TABLE IF EXISTS `viewsectoresentops`;
/*!50001 DROP VIEW IF EXISTS `viewsectoresentops`*/;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
/*!50001 CREATE VIEW `viewsectoresentops` AS SELECT 
 1 AS `IdSistema`,
 1 AS `idsectorizacion`,
 1 AS `IdTOP`,
 1 AS `idsector`,
 1 AS `idnucleo`,
 1 AS `tipo`*/;
SET character_set_client = @saved_cs_client;

--
-- Table structure for table `zonas`
--

DROP TABLE IF EXISTS `zonas`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `zonas` (
  `idZonas` int(11) NOT NULL AUTO_INCREMENT,
  `IdSistema` varchar(45) NOT NULL,
  `Nombre` varchar(45) NOT NULL,
  PRIMARY KEY (`idZonas`),
  UNIQUE KEY `idx_zonas_unique_name` (`IdSistema`,`Nombre`)
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=utf8 COMMENT='Zonas Radio';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping events for database 'new_cd40_trans'
--

--
-- Dumping routines for database 'new_cd40_trans'
--
/*!50003 DROP PROCEDURE IF EXISTS `ActualizaNombreSector` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `ActualizaNombreSector`(in id_sistema varchar(32), in id_nucleo varchar(32), in id_sector varchar(700), in id_nuevo_nombre_sector varchar(700))
BEGIN
	UPDATE Sectores 
		SET IdSector=id_nuevo_nombre_sector
		WHERE IdSistema=id_sistema AND
					IdNucleo=id_nucleo AND
					IdSector=id_sector AND
					SectorSimple=FALSE;
		REPLACE INTO TablasModificadas (IdTabla)
				VALUES ('Sectores');
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `ActualizaSectoresSectorizacion` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `ActualizaSectoresSectorizacion`(in id_sistema varchar(32), in id_nucleo varchar(32), in id_grupo varchar(700), in id_agrupacion varchar(700))
BEGIN
	DECLARE id char(32);
	
	SELECT DATE_FORMAT(FechaActivacion,"%d/%m/%Y %H:%i:%s") INTO id FROM sectorizaciones WHERE IdSistema=id_sistema AND Activa=true;
	
	UPDATE SectoresSectorizacion
		SET IdSector=id_agrupacion
		WHERE IdSistema=id_sistema AND
					IdNucleo=id_nucleo AND
					IdSector=id_grupo AND
					IdSectorizacion<>id;
	REPLACE INTO TablasModificadas (IdTabla)
				VALUES ('SectoresSectorizacion');
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `AgrupacionDeLosSectores` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `AgrupacionDeLosSectores`(in cuantos int, in lista_usuarios varchar(700))
BEGIN
	DECLARE lista VARCHAR(700);
	SET @lista=lista_usuarios;
	SELECT sa.IdAgrupacion,@lista,lista FROM SectoresAgrupacion sa, viewSectoresEnAgrupacion cuenta
	WHERE cuenta.contador=cuantos AND
				sa.IdAgrupacion=cuenta.IdAgrupacion	AND
				sa.IdSector in (@lista);
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `AsignacionRecursosDeUnaRed` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `AsignacionRecursosDeUnaRed`(in id_sistema char(32), in id_red char(32))
BEGIN
	IF (id_red is not null) THEN
      SELECT 'P' AS TIPO,rTf.IdRed, r.IdRecurso, r.IdTifx AS NOMBRE, 
        r.SlotPasarela AS SlotPasarela, r.NumDispositivoSlot AS NumDispositivoSlot
      FROM new_cd40_trans.RecursosTf rTf, new_cd40_trans.Recursos r
      WHERE rTf.IdSistema=id_sistema AND
            rTf.IdRed=id_red AND
            r.IdSistema=rTf.IdSistema AND
            r.IdRecurso=rTf.IdRecurso AND r.IdTifx IS NOT NULL
      UNION
      SELECT 'E' AS TIPO,rTf.IdRed, r.IdRecurso, r.Idequipos AS NOMBRE, 
            NULL AS SlotPasarela, NULL AS NumDispositivoSlot
          FROM new_cd40_trans.RecursosTf rTf, new_cd40_trans.Recursos r
          WHERE rTf.IdSistema=id_sistema AND
                rTf.IdRed=id_red AND
                r.IdSistema=rTf.IdSistema AND
                r.IdRecurso=rTf.IdRecurso AND r.Idequipos IS NOT NULL 
      ORDER BY TIPO,NOMBRE,SlotPasarela, NumDispositivoSlot,IdRecurso;  
	ELSE
      -- Todas las redes de telefonia
       SELECT    CASE WHEN r.IdTifx IS NOT NULL THEN 'P' ELSE CASE WHEN r.Idequipos IS NOT NULL THEN 'E' ELSE NULL END END AS TIPO,
                rd.IdRed,
                r.IdRecurso, 
                CASE WHEN r.IdTifx IS NOT NULL  THEN r.IdTifx ELSE  r.Idequipos END AS NOMBRE,
                CASE WHEN r.IdTifx IS NOT NULL  THEN r.SlotPasarela ELSE  NULL END AS SlotPasarela,
                CASE WHEN r.IdTifx IS NOT NULL  THEN r.NumDispositivoSlot ELSE   NULL END AS NumDispositivoSlot
        FROM Redes rd
        LEFT JOIN RecursosTf rTf ON rd.IdSistema=rTf.IdSistema AND rd.IdRed=rTf.IdRed          
        LEFT JOIN Recursos r     ON rTf.IdSistema=r.IdSistema  AND rTf.IdRecurso=r.IdRecurso AND rTf.TipoRecurso=r.TipoRecurso
        WHERE rd.IdSistema=id_sistema
        ORDER BY TIPO,NOMBRE,SlotPasarela, NumDispositivoSlot,IdRecurso;  
	END IF;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `AsignacionUsuariosATops` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `AsignacionUsuariosATops`(in id_Sistema char(32))
BEGIN
SELECT c.IdSectorOriginal AS IdSector,a.IdTop,t.Grabador1,e1.IpRed1 as IpGrabador1, 
			t.Grabador2,e2.IpRed1 as IpGrabador2, e1.SipPort as RtspPort 
		FROM SectoresSector c
		INNER JOIN sectorizaciones b ON b.IdSistema = c.IdSistema AND b.Activa=true
		INNER JOIN sectoressectorizacion a ON a.IdSistema=b.IdSistema AND a.IdSectorizacion=b.IdSectorizacion
		INNER JOIN top t ON t.IdTop=a.IdTop
		LEFT JOIN equiposeu e1 ON e1.IdEquipos=t.grabador1
		LEFT JOIN equiposeu e2 ON e2.idEquipos=t.grabador2
		WHERE 	c.IdSistema=id_Sistema AND
				c.IdNucleo=a.IdNucleo AND
				c.IdSector=a.IdSector;     
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `CheckIdentificadorAsignado` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `CheckIdentificadorAsignado`(in Tipo char(32),in pid_sistema char(32), in id_identificador char(32), out iAsignado int)
BEGIN
  -- Devuelve 1 si el identificador del tipo, que se le pasa como parámetro, existe en el sistema, 0 si no existe y 
  -- -1 si se ha producido error. 
  -- El tipo puede ser: EF (Pasarelas,Tops, Equipos Externos y Encaminamientos).
  --                    R (Recursos Radio y de Telefonía)
  --                    D (Destinos Radio y de Telefonía), 
  --                    NA (Emplazamientos y Zonas) 
  DECLARE iCuenta INT;

  SET iCuenta= -1;
  
  CASE 
    WHEN (TIPO='EF') THEN     
       -- Se comprueba si el identificador está asignado a una pasarela PASARELA,TOP, EQUIPOSEU o encaminamiento
       -- Los encaminamientos que son SCV IP tienen un registro en la tabla de equiposeu con las direcciones IP
       -- de los servidores proxy y de presencia
      SELECT COUNT(1) INTO iCuenta FROM (
                            SELECT IdSistema AS IdSistema ,IdTIFX as ID,'P' AS TIPO FROM tifx
                            UNION
                            SELECT IdSistema AS IdSistema,IdEquipos AS ID ,'E' AS TIPO FROM equiposeu
                            UNION 
                            SELECT IdSistema AS IdSistema,Central AS ID ,'C' AS TIPO FROM encaminamientos
                            UNION 
                            SELECT IdSistema AS IdSistema,IdTop AS ID,'T' AS TIPO FROM top) A
      WHERE A.idSistema= pid_sistema and A.ID=id_identificador;
    WHEN (TIPO='R') THEN
      -- Se comprueba si el identificador de recursos de telefonía o radio está asignado a algun recurso
      SELECT COUNT(1) INTO iCuenta 
      FROM recursos 
      WHERE idSistema=pid_sistema and idRecurso=id_identificador;

    WHEN (TIPO='D') THEN
      -- Se comprueba si el Identificador del destino radio o de telefonía está libre
      SELECT  COUNT(1) INTO iCuenta
      FROM destinostelefonia A 
         JOIN destinosexternos B ON A.IdSistema=B.IdSistema AND A.IdDestino=B.IdDestino AND 
                    A.TipoDestino=B.TipoDestino AND A.IdPrefijo=B.IdPrefijo
         LEFT JOIN  redes R ON A.IdSistema=R.IdSistema AND A.IdPrefijo=R.IdPrefijo
      WHERE A.IdSistema=pid_sistema and A.IdDestino=id_identificador;
	  
      IF iCuenta=0 THEN
        -- Se comprueba si el identificador existe como DestinoRadio
        SELECT COUNT(1) INTO iCuenta
        FROM destinosradio
        WHERE IdSistema=pid_sistema and IdDestino=id_identificador;
      END IF;  

    WHEN (TIPO='NA') THEN
      -- Identificadores no agrupados: EMPLAZAMIENTOS, ZONAS
      SELECT COUNT(1) INTO iCuenta
      FROM emplazamientos
      WHERE IdSistema=pid_sistema and IdEmplazamiento=id_identificador;

      IF iCuenta=0 THEN
      -- Se comprueba si el identificador existe como zonas
       SELECT COUNT(1) INTO iCuenta
       FROM  zonas
       WHERE IdSistema=pid_sistema AND Nombre=id_identificador;
      END IF;	
    ELSE
      SET iCuenta= -1;
  END CASE; 

  IF iCuenta>0 THEN
      SET iAsignado=1;      
  ELSE
      SET iAsignado=iCuenta;
  END IF;

END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `CreaPosicionesActiva` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `CreaPosicionesActiva`(in id_sistema char(32), in id_sectorizacion char(32), in id_activa char(32))
BEGIN
		INSERT into radio
			SELECT IdSistema,id_activa as IdSectorizacion,IdNucleo,IdSector,PosHMI,IdDestino,Prioridad,PrioridadSIP,ModoOperacion,Cascos,Literal,SupervisionPortadora FROM Radio
				WHERE IdSectorizacion=id_sectorizacion AND
							IdSistema=id_sistema;
		REPLACE INTO TablasModificadas (IdTabla)
				VALUES ('radio');
	
		INSERT into Internos
			SELECT IdSistema,id_activa as IdSectorizacion,IdNucleo,IdSector,IdDestino,IdPrefijo,TipoAcceso,PosHMI,Prioridad,OrigenR2,PrioridadSIP,Literal FROM Internos
				WHERE IdSectorizacion=id_sectorizacion AND
							IdSistema=id_sistema;
		REPLACE INTO TablasModificadas (IdTabla)
				VALUES ('Internos');

		INSERT into Externos
			SELECT IdSistema,id_activa as IdSectorizacion,IdNucleo,IdSector,IdDestino,IdPrefijo,TipoAcceso,PosHMI,Prioridad,OrigenR2,PrioridadSIP,Literal,Grupo FROM Externos
				WHERE IdSectorizacion=id_sectorizacion AND
							IdSistema=id_sistema;
		REPLACE INTO TablasModificadas (IdTabla)
				VALUES ('Externos');
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `CrearFicheroBackup` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `CrearFicheroBackup`(in id_sistema char(32), in tabla text, in fichero text,in fDesde datetime, in fHasta datetime)
BEGIN
DECLARE lista CHAR(55);
set @SQL = concat("SELECT *",
			"INTO OUTFILE '", fichero, "'", 
      " FIELDS TERMINATED BY ',' OPTIONALLY ENCLOSED BY '\"' LINES TERMINATED BY '\\n'",
      " FROM ", tabla,
      " WHERE IdSistema='", id_sistema,
			"' AND DATE_FORMAT(FechaHora,'%Y/%m/%d')>='",DATE_FORMAT(fDesde,'%Y/%m/%d'), 
			"' AND DATE_FORMAT(FechaHora,'%Y/%m/%d')< '",DATE_FORMAT(fHasta,'%Y/%m/%d'),
			"';"); 
PREPARE stmt FROM @SQL; 
EXECUTE stmt; 
set @SQL_DELETE = concat("DELETE FROM ", tabla,
      " WHERE IdSistema='", id_sistema,
			"' AND DATE_FORMAT(FechaHora,'%Y/%m/%d')>='",DATE_FORMAT(fDesde,'%Y/%m/%d'), 
			"' AND DATE_FORMAT(FechaHora,'%Y/%m/%d')< '",DATE_FORMAT(fHasta,'%Y/%m/%d'),
			"';"); 
PREPARE stmt_delete FROM @SQL_DELETE; 
EXECUTE stmt_delete; 
	
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `CreaSectoresActiva` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `CreaSectoresActiva`(in id_sistema char(32), in id_sectorizacion char(32), in id_activa char(32))
BEGIN
	INSERT into SectoresSectorizacion
	SELECT IdSistema,id_activa as IdSectorizacion,IdNucleo,IdSector,IdTop FROM SectoresSectorizacion
		WHERE IdSectorizacion=id_sectorizacion AND
					IdSistema=id_sistema;
					
	REPLACE INTO TablasModificadas (IdTabla)
				VALUES ('SectoresSectorizacion');
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `CreaSectoresActivaConexionSacta` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `CreaSectoresActivaConexionSacta`(in id_sistema char(32), in id_sectorizacion char(32), in id_activa char(32))
BEGIN
	INSERT into SectoresSectorizacion
		SELECT ss.IdSistema,id_activa as IdSectorizacion,ss.IdNucleo,ss.IdSector,ss.IdTOP FROM SectoresSectorizacion ss, Sectores s
		WHERE ss.IdSistema=id_sistema AND
					(ss.IdSectorizacion='SACTA' OR
						(ss.IdSectorizacion=id_sectorizacion AND 
							s.Tipo='M' AND	
							ss.IdTOP NOT IN (SELECT idtop 
																	FROM sectoressectorizacion 
																	WHERE idsistema=id_sistema AND idsectorizacion='SACTA'))) AND
					ss.IdSector=s.IdSector;
	REPLACE INTO TablasModificadas (IdTabla)
				VALUES ('SectoresSectorizacion');
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `CuantasTeclasConPrioridadUno` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `CuantasTeclasConPrioridadUno`(in id_sistema varchar(32), in id_nucleo varchar(32), out cuantas int)
BEGIN
  DECLARE iNumTeclasPrioridad int default 0;
  SET cuantas=0;
	
  -- Obtiene el número de teclas con prioridad 1 asignadas al panel de linea caliente para un sistema y nucleo
  -- Las teclas asignadas al panel de línea caliente pueden corresponder a:
  --   Destinos internos (Idprefijo=0)
  --   Destinos externos: Líneas calientes LCEN (IdPrefijo=1 y TipoAcceso='IA') y 
  --                      Destinos de telefonía ATS con comunicación IP (IdPrefijo=3 y TipoAcceso='IA')
  --  Un destino ATS se puede asignar a más de un sector, por lo que esté tipo de destino sólo se debe contar una vez
  --  aunque está asignado a varios sectores  
  
  SELECT SUM(cuenta) 
  INTO iNumTeclasPrioridad 
  FROM (SELECT COUNT(1) cuenta FROM DestinosInternosSector
        WHERE IdSistema=id_sistema AND 
              IdNucleo=id_nucleo AND
              IdPrefijo=0 AND
              Prioridad=1
        UNION ALL      
        SELECT COUNT(1) cuenta 
        FROM (SELECT 1 FROM DestinosExternosSector
              WHERE IdSistema=id_sistema AND 
                    IdNucleo=id_nucleo AND
                    (IdPrefijo=1 OR IdPrefijo=3) AND TipoDestino=1 AND TipoAcceso='IA' AND
                    Prioridad=1
              GROUP BY IdDestino) F
        ) B;          

  SET cuantas=iNumTeclasPrioridad;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `DestinosExternosAsignadosASector` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `DestinosExternosAsignadosASector`(in id_sistema char(32), in id_sector varchar(700), in telefonia BOOL)
BEGIN
	IF telefonia=true THEN
    -- Destinos externos asignados en el panel de telefonia
		IF id_sector IS NOT NULL THEN
			SELECT * FROM DestinosExternosSector
			WHERE IdSistema=id_sistema AND
						IdSector=id_sector AND
						IdPrefijo<>1 AND TipoAcceso='DA';
		ELSE
			SELECT * FROM DestinosExternosSector
			WHERE IdSistema=id_sistema AND
						IdPrefijo<>1 AND TipoAcceso='DA';
		END IF;
	ELSE
    -- Destinos externos asignados en el panel de linea caliente
    -- Sólo se pueden configurar Destinos ATS en el panel de línea caliente
    -- El destino ATS, puede tener asociado un Destino LCEN (prefijo=1)
		IF id_sector IS NOT NULL THEN
			SELECT * FROM DestinosExternosSector
			WHERE IdSistema=id_sistema AND
						IdSector=id_sector AND
						IdPrefijo=3 AND TipoAcceso='IA';
		ELSE
			SELECT * FROM DestinosExternosSector
			WHERE IdSistema=id_sistema AND
						IdPrefijo=3 AND TipoAcceso='IA';
		END IF;
	END IF;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `DestinosInstantaneosSinAsignarAEnlaces` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `DestinosInstantaneosSinAsignarAEnlaces`(in id_sistema char(32))
BEGIN
	SELECT * FROM Destinos
	WHERE IdDestino not in (SELECT IdDestino FROM EnlacesLCEN 
														WHERE IdSistema=id_sistema AND IdPrefijo in ("0","1")) AND
				IdSistema=id_sistema AND
				IdPrefijo in ("0","1");
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `DestinosInternosAsignadosASector` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `DestinosInternosAsignadosASector`(in id_sistema char(32), in id_sector varchar(700), in telefonia BOOL)
BEGIN
	IF telefonia=true THEN
		IF id_sector IS NOT NULL THEN
			SELECT * FROM DestinosInternosSector
			WHERE IdSistema=id_sistema AND
						IdSector=id_sector AND
						IdPrefijo=2;
		ELSE
			SELECT * FROM DestinosInternosSector
			WHERE IdSistema=id_sistema AND
						IdPrefijo=2;
		END IF;
	ELSE
		IF id_sector IS NOT NULL THEN
			SELECT * FROM DestinosInternosSector
			WHERE IdSistema=id_sistema AND
						IdSector=id_sector AND
						IdPrefijo=0;
		ELSE
			SELECT * FROM DestinosInternosSector
			WHERE IdSistema=id_sistema AND
						IdPrefijo=0;
		END IF;
	END IF;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `DestinosInternosSinAsignarAEnlaces` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `DestinosInternosSinAsignarAEnlaces`(in id_sistema char(32))
BEGIN
	SELECT * FROM Destinos
	WHERE IdDestino not in (SELECT IdDestino FROM EnlacesInternos 
														WHERE IdSistema=id_sistema AND IdPrefijo <> "1") AND
				IdSistema=id_sistema AND
				IdPrefijo <> "1";
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `DestinosLCEN_SinAsignar` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `DestinosLCEN_SinAsignar`(in id_sistema char(32))
BEGIN
  -- Se obtiene los destinos de LC que no están asignados en el sistema
  -- Un destino LCEN sólo se puede asignar a un único sector
  (SELECT a.* FROM destinosTelefonia a
   WHERE  a.IdSistema=id_sistema AND
		    a.IdPrefijo=1 AND
		    a.IdDestino NOT IN (SELECT IdDestinoLCEN FROM DestinosExternosSector D
								WHERE D.IdSistema=a.IdSistema AND D.TipoAcceso='IA' AND
                                      D.IdPrefijoDestinoLCEN=a.IdPrefijo));
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `DestinosLineaCalienteSinAsignarAlSector` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `DestinosLineaCalienteSinAsignarAlSector`(in id_sistema char(32), in id_usuario varchar(700), in id_nucleo char(32))
BEGIN
   -- Se obtienen los destinos externos de telefonía para el panel de línea caliente que son internos (prefijo=0) o ATS (prefijo=3)
  -- Los destinos ATS se pueden asignar a varios sectores.
  -- Se obtiene los destinos ATS con IdAbonado dentro del rango de algún encaminamiento
  	(SELECT a.* FROM destinosTelefonia a, destinosInternos b, Sectores c
			WHERE a.IdSistema=id_sistema AND
					a.IdPrefijo=0 AND
					a.IdSistema=b.IdSistema AND
					a.IdPrefijo=b.IdPrefijo AND
					a.IdDestino=b.IdDestino AND
					b.IdSector<>id_usuario AND
					b.IdNucleo=id_nucleo AND
					c.IdSector=b.IdSector AND
					c.SectorSimple=true AND
					b.IdDestino not in (SELECT IdDestino FROM DestinosInternosSector
                                                             WHERE IdSistema=id_sistema AND
                                                                   IdSector=id_usuario AND
                                                                   IdPrefijo=0) ORDER BY a.IdDestino)
	  UNION -- Se obtiene los destinos ATS con IdAbonado dentro del rango de algún encaminamiento
    (SELECT a.* from destinosTelefonia a, destinosexternos D 
     WHERE   a.IdSistema=D.IdSistema AND
             a.TipoDestino=D.TipoDestino AND
             a.IdPrefijo=D.IdPrefijo AND
             a.IdDestino=D.IdDestino AND
             a.IdSistema=id_sistema AND
             a.IdPrefijo=3 AND
             a.TipoDestino=1 AND
             EXISTS (SELECT 1 from encaminamientos E 
                     LEFT JOIN rangos as R ON  R.IdSistema=E.IdSistema AND R.Central = E.Central
                     WHERE E.IdSistema=D.IdSistema AND D.IdAbonado BETWEEN R.inicial and R.final) AND
             a.IdDestino NOT IN (SELECT IdDestino FROM DestinosExternosSector D
                                 WHERE  D.IdSistema=a.IdSistema AND D.IdPrefijo=a.IdPrefijo AND
                                        D.IdNucleo=id_nucleo AND D.IdSector=id_usuario AND D.TipoAcceso='IA') ORDER BY a.IdDestino);
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `DestinosPorGruposTelefonia` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `DestinosPorGruposTelefonia`(in id_sistema varchar(32))
BEGIN
	SELECT g.idgrupo,d.IdDestino
	FROM GruposTelefonia g, destinostelefonia d
	WHERE g.IdSistema=id_sistema AND
				d.IdSistema = g.IdSistema AND
				d.IdGrupo=g.IdGrupo
UNION
	SELECT idgrupo,null 
		FROM grupostelefonia 
		WHERE IdSistema=id_sistema AND 
					IdGrupo NOT IN (SELECT IdGrupo FROM destinostelefonia WHERE idSistema=id_sistema AND IdGrupo is not null);
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `DestinosRadioAsignadosASector` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `DestinosRadioAsignadosASector`(in id_sistema char(32), in id_usuario varchar(700), in pagina int, in frecPorPagina int)
BEGIN
	SELECT * FROM DestinosRadioSector
				WHERE Idsistema=id_sistema AND
							IdSector=id_usuario AND
							(((PosHMI-1) div frecPorPagina) = pagina);
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `DestinosRadioSectorizados` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `DestinosRadioSectorizados`(in id_sistema char(32), in id_sector varchar(700), in id_sectorizacion char(32))
BEGIN
		SELECT * FROM radio
					WHERE IdSistema=id_sistema AND
								IdSectorizacion=id_sectorizacion AND
								IdSector=id_sector;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `DestinosRadioSectorizadosParaXML` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `DestinosRadioSectorizadosParaXML`(in id_sistema char(32), in id_sectorizacion char(32))
BEGIN
	IF (id_sectorizacion IS NOT NULL) THEN
		SELECT * FROM (SELECT * FROM radio
			WHERE IdSistema=id_sistema AND
						IdSectorizacion=id_sectorizacion) AS A ORDER BY A.IdSector,A.PosHMI;
	ELSE
		SELECT * FROM (SELECT * FROM DestinosRadioSector
			WHERE IdSistema=id_sistema) AS A ORDER BY A.IdSector,A.PosHMI;
	END IF;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `DestinosRadioSinAsignarALaPaginaDelSector` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `DestinosRadioSinAsignarALaPaginaDelSector`(in id_sistema char(32), in id_usuario varchar(700), in pagina int, in frecPorPagina int)
BEGIN
	SELECT IdDestino FROM DestinosRadio DR
	WHERE idSistema=id_sistema AND
				0 < (SELECT COUNT(*) FROM RecursosRadio RR WHERE RR.IdDestino=DR.IdDestino) AND
				IdDestino not in (select IdDestino from DestinosRadioSector
														where Idsistema=id_sistema AND
																	IdSector=id_usuario AND
																	(((PosHMI-1) div frecPorPagina) = pagina));
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `DestinosTelefoniaMultiDestino` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `DestinosTelefoniaMultiDestino`(in id_sistema char(32), in id_nucleo char(32))
BEGIN
(SELECT a.*, c.IdAbonado 
FROM destinosTelefonia a, destinosInternos b
LEFT JOIN usuariosabonados c ON b.IdSistema=c.IdSistema 
AND b.IdNucleo=c.IdNucleo 
AND b.IdSector=c.IdSector 
WHERE a.IdSistema=id_sistema AND
a.IdPrefijo=2 AND 
a.IdSistema=b.IdSistema AND
a.IdPrefijo=b.IdPrefijo AND
a.IdDestino=b.IdDestino AND
b.IdNucleo=id_nucleo AND
b.IdDestino in
	(SELECT s.IdSector FROM Sectores s
	WHERE s.IdSistema=id_sistema AND
	s.SectorSimple)
ORDER BY a.IdDestino)
UNION
(SELECT a.*, c.IdAbonado
FROM destinosTelefonia a
LEFT JOIN destinosexternos c ON a.IdSistema=c.IdSistema
AND a.IdDestino = c.IdDestino
AND a.TipoDestino = c.TipoDestino
AND a.IdPrefijo = c.IdPrefijo
WHERE a.IdSistema=id_sistema AND
a.TipoDestino=1 AND 
a.IdPrefijo<>1 AND
a.IdPrefijo<>20 
ORDER BY a.IdDestino);
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `DestinosTelefoniaMultiDestinoAjeno` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `DestinosTelefoniaMultiDestinoAjeno`(in id_sistema char(32), in id_nucleo char(32))
BEGIN
		(select a.* from destinosTelefonia a, destinosInternos b
where a.IdSistema=id_sistema AND
a.IdPrefijo<>20 AND
(a.IdPrefijo=32 OR a.IdPrefijo=3) AND 
a.IdSistema=b.IdSistema AND
a.IdPrefijo=b.IdPrefijo AND
a.IdDestino=b.IdDestino AND

b.IdNucleo=id_nucleo AND
b.IdDestino in
(SELECT s.IdSector FROM Sectores s
WHERE s.IdSistema=id_sistema AND
s.SectorSimple)
ORDER BY a.IdDestino)

UNION

(select a.* from destinosTelefonia a
where a.IdSistema=id_sistema AND
a.TipoDestino=1 AND 
a.IdPrefijo<>1 AND
(a.IdPrefijo=32 OR a.IdPrefijo=3) AND
a.IdPrefijo<>20
ORDER BY a.IdDestino);
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `DestinosTelefoniaSectorizados` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `DestinosTelefoniaSectorizados`(in id_sistema char(32), in id_sector varchar(700), in id_sectorizacion char(32), in internos BOOL)
BEGIN
	IF internos=true THEN
		SELECT * FROM internos
					WHERE IdSistema=id_sistema AND
								IdSectorizacion=id_sectorizacion AND
								IdSector=id_sector;
	ELSE
			SELECT * FROM externos
					WHERE IdSistema=id_sistema AND
								IdSectorizacion=id_sectorizacion AND
								IdSector=id_sector;
	END IF;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `DestinosTelefoniaSectorizadosParaXML` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `DestinosTelefoniaSectorizadosParaXML`(in id_sistema char(32), in id_sectorizacion char(32), in lc BOOL)
BEGIN
	IF (id_sectorizacion IS NOT NULL) THEN
		IF lc=true THEN
			SELECT * FROM ((SELECT *,null as 'Grupo' FROM internos
								WHERE IdSistema=id_sistema AND
									IdSectorizacion=id_sectorizacion AND
									IdPrefijo=0)
			UNION ALL
			(SELECT * FROM externos
					WHERE IdSistema=id_sistema AND
								IdSectorizacion=id_sectorizacion AND
								IdPrefijo=1)) as A ORDER BY IdSector,PosHMI;
		ELSE
			SELECT * FROM ((SELECT *,null as 'Grupo' FROM internos
					WHERE IdSistema=id_sistema AND
								IdSectorizacion=id_sectorizacion AND
								IdPrefijo=2)
			UNION
			(SELECT * FROM externos
					WHERE IdSistema=id_sistema AND
								IdSectorizacion=id_sectorizacion AND
								IdPrefijo<>1)) as A ORDER BY IdSector,PosHMI;
		END IF;			
	ELSE	
		IF lc=true THEN
			SELECT * FROM ((SELECT * FROM DestinosInternosSector
					WHERE IdSistema=id_sistema AND
								IdPrefijo=0)
			UNION
			(SELECT d.IdSistema,d.IdDestino,d.TipoDestino,d.IdNucleo,d.IdSector,d.IdPrefijo,d.PosHMI,d.Prioridad,d.OrigenR2,d.PrioridadSIP,d.TipoAcceso,d.Literal
					FROM DestinosExternosSector d
					WHERE IdSistema=id_sistema AND
								IdPrefijo=1)) as A ORDER BY IdSector,PosHMI;
		ELSE
			SELECT * FROM ((SELECT * FROM DestinosInternosSector
					WHERE IdSistema=id_sistema AND
								IdPrefijo=2)
			UNION
			(SELECT d.IdSistema,d.IdDestino,d.TipoDestino,d.IdNucleo,d.IdSector,d.IdPrefijo,d.PosHMI,d.Prioridad,d.OrigenR2,d.PrioridadSIP,d.TipoAcceso,d.Literal
					FROM DestinosExternosSector d
					WHERE IdSistema=id_sistema AND
								IdPrefijo<>1)) as A ORDER BY IdSector,PosHMI;
		END IF;			
	END IF;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `DestinosTelefoniaSinAsignarAlSector` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `DestinosTelefoniaSinAsignarAlSector`(in id_sistema char(32), in id_usuario varchar(700), in id_nucleo char(32))
BEGIN
		(select a.* from destinosTelefonia a, destinosInternos b
			where a.IdSistema=id_sistema AND
				a.IdPrefijo=2 AND 
				a.IdSistema=b.IdSistema AND
				a.IdPrefijo=b.IdPrefijo AND
				a.IdDestino=b.IdDestino AND
				b.IdSector<>id_usuario AND
				b.IdNucleo=id_nucleo AND
				b.IdDestino in (SELECT s.IdSector FROM Sectores s
														WHERE s.IdSistema=id_sistema AND
																	s.SectorSimple) AND
				b.IdDestino not in (SELECT IdDestino FROM DestinosInternosSector
														WHERE IdSistema=id_sistema AND
																	IdSector=id_usuario AND
																	IdPrefijo=2)
				ORDER BY a.IdDestino)
UNION
(select a.* from destinosTelefonia a
	where a.IdSistema=id_sistema AND
				a.TipoDestino=1 AND 
				a.IdPrefijo<>1 AND
                a.IdPrefijo<>20 AND
				a.IdDestino not in (SELECT IdDestino FROM DestinosExternosSector
														WHERE IdSistema=id_sistema AND
															  IdSector=id_usuario AND
                                                              TipoAcceso='DA' AND 
                                                              a.IdPrefijo<>1)
				ORDER BY a.IdDestino);
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `EliminaActiva` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `EliminaActiva`(in id_sistema char(32))
BEGIN
	DECLARE id char(32);
	
	SELECT FechaActivacion INTO id FROM sectorizaciones WHERE IdSistema=id_sistema AND Activa=true;
	
	DELETE FROM Sectorizaciones
		WHERE NOT Activa AND
					IdSistema=id_sistema AND
					FechaActivacion=id;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `ExisteIP` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `ExisteIP`(in id_sistema varchar(32), in ip varchar(15), in id_hw varchar(32))
BEGIN
	select 	(select count(*) from Top where IdSistema=id_sistema AND IdTOP!=id_hw AND (IpRed1=ip OR IpRed2=ip)) +
					(select count(*) from Tifx where IdSistema=id_sistema AND IdTifx!=id_hw AND (IpRed1=ip OR IpRed2=ip)) +
					(select count(*) from EquiposEu where IdSistema=id_sistema AND idEquipos!=id_hw AND (IpRed1=ip OR IpRed2=ip));
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `FinalizaTransaccion` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `FinalizaTransaccion`()
BEGIN
		start transaction;
		
		insert ignore into cd40.sistema select * from cd40_trans.sistema;
		insert ignore into cd40.abonados select * from cd40_trans.abonados;
		
		commit;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `GeneraRegistroBackup` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `GeneraRegistroBackup`(in id_sistema char(32), in tipo_backup int, in fDesde datetime, in fHasta datetime, in recurso text)
BEGIN
	INSERT INTO registrobackup (
	   IdSistema
	  ,TipoBackup
	  ,FechaInicio
	  ,FechaFin
	  ,RecursoDestino
	) VALUES (
	   id_sistema 															
	  ,tipo_backup 															
	  ,DATE_FORMAT(fDesde,'%Y/%m/%d %H:%i:%s') 	
	  ,DATE_FORMAT(fHasta,'%Y/%m/%d %H:%i:%s') 	
	  ,recurso 																	
	);
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `GestionaFS` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `GestionaFS`(in id_sistema char(32), in id_nucleo char(32), in id_sector char(32), in SeleccionadoFS bool)
BEGIN
IF(SeleccionadoFS = true) THEN
	UPDATE Sectores 
		SET SeleccionadoFS=false;
		REPLACE INTO TablasModificadas (IdTabla)
				VALUES ('Sectores');
                END IF;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `GetEstadoCluster` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `GetEstadoCluster`()
BEGIN
	SELECT * FROM estadocluster;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `GetEventosRadio` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `GetEventosRadio`(in id_sistema varchar(32), in desde date, in hasta date)
BEGIN
	SELECT * FROM EventosRadio	
	WHERE IdSistema=id_sistema AND
				DATE_FORMAT(FechaHora,"%Y/%m/%d") >= desde AND
				DATE_FORMAT(FechaHora,"%Y/%m/%d") < hasta
	ORDER BY FechaHora;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `GetEventosTelefonia` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `GetEventosTelefonia`(in id_sistema varchar(32), in desde date, in hasta date)
BEGIN
	SELECT * FROM EventosTelefonia	
	WHERE IdSistema=id_sistema AND
				DATE_FORMAT(FechaHora,"%Y/%m/%d") >= desde AND
				DATE_FORMAT(FechaHora,"%Y/%m/%d") < hasta
	ORDER BY FechaHora;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `GetFunciones` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `GetFunciones`(in id_sistema varchar(32), in _tipo int)
BEGIN
	SELECT * FROM funciones
	WHERE IdSistema=id_sistema AND
				Tipo=_tipo;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `GetHistoricos` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `GetHistoricos`(in id_sistema varchar(32),in num_scv int, in lista_incidencias text,in tipo_hw int, in id_hw varchar(32), in desde datetime, in hasta datetime)
BEGIN
	declare i int;
	declare pos int;
	declare cadena text;
	set i=LOCATE(",",lista_incidencias);
	set pos=1;
	create temporary table if not exists t SELECT hi.*, i.Descripcion AS Descriptor FROM HistoricoIncidencias hi, incidencias i where false;
	IF (lista_incidencias<>"") THEN												
		IF (id_hw="") THEN
			IF (tipo_hw=-1) THEN																		
				WHILE i>0 DO
					set cadena=substring(lista_incidencias,pos,i-pos);
					set pos=i+1;
					set i=LOCATE(",",lista_incidencias,pos);
		
					INSERT INTO t 
					SELECT hi.*, i.Descripcion FROM HistoricoIncidencias hi, incidencias i
						WHERE hi.IdSistema=id_sistema AND
							hi.IdIncidencia=cadena AND 
							hi.FechaHora >= desde AND
							hi.FechaHora < hasta AND 
							i.IdIncidencia=hi.IdIncidencia;
					END WHILE;
			ELSE
				WHILE i>0 DO
					set cadena=substring(lista_incidencias,pos,i-pos);
					set pos=i+1;
					set i=LOCATE(",",lista_incidencias,pos);
		
					INSERT INTO t 
					SELECT hi.*, i.Descripcion FROM HistoricoIncidencias hi, incidencias i
						WHERE hi.IdSistema=id_sistema AND
							hi.TipoHw = tipo_hw AND
							hi.IdIncidencia=cadena AND 
							hi.FechaHora >= desde AND
							hi.FechaHora < hasta AND 
							i.IdIncidencia=hi.IdIncidencia;
					END WHILE;
			END IF;
		ELSE
			IF (tipo_hw=-1) THEN																		
				WHILE i>0 DO
					set cadena=substring(lista_incidencias,pos,i-pos);
					set pos=i+1;
					set i=LOCATE(",",lista_incidencias,pos);
		
					INSERT INTO t 
					SELECT hi.*, i.Descripcion FROM HistoricoIncidencias hi, incidencias i
						WHERE hi.IdSistema=id_sistema AND
							hi.IdHw=id_hw AND 
							hi.IdIncidencia=cadena AND 
							hi.FechaHora >= desde AND
							hi.FechaHora < hasta AND 
							i.IdIncidencia=hi.IdIncidencia;
					END WHILE;
			ELSE
				WHILE i>0 DO
					set cadena=substring(lista_incidencias,pos,i-pos);
					set pos=i+1;
					set i=LOCATE(",",lista_incidencias,pos);
		
					INSERT INTO t 
					SELECT hi.*, i.Descripcion FROM HistoricoIncidencias hi, incidencias i
						WHERE hi.IdSistema=id_sistema AND
							hi.IdHw=id_hw AND 
							hi.TipoHw=tipo_hw AND 
							hi.IdIncidencia=cadena AND 
							hi.FechaHora >= desde AND
							hi.FechaHora < hasta AND 
							i.IdIncidencia=hi.IdIncidencia;
					END WHILE;
			END IF;
		END IF;
	ELSE
		IF (id_hw="") THEN
			IF (tipo_hw=-1) THEN																		
				INSERT INTO t 
					SELECT hi.*,i.Descripcion FROM HistoricoIncidencias hi, incidencias i WHERE hi.IdSistema=id_sistema AND 
  							hi.FechaHora >= desde AND
	  						hi.FechaHora < hasta AND 
								i.IdIncidencia=hi.IdIncidencia;
			ELSE
				INSERT INTO t 
					SELECT hi.*, i.Descripcion FROM HistoricoIncidencias  hi, incidencias i
						WHERE hi.IdSistema=id_sistema AND
									hi.TipoHw=tipo_hw AND 
    							hi.FechaHora >= desde AND
    							hi.FechaHora < hasta AND 
									i.IdIncidencia=hi.IdIncidencia;
			END IF;
		ELSE
			IF (tipo_hw=-1) THEN																		
				INSERT INTO t 
					SELECT hi.*, i.Descripcion FROM HistoricoIncidencias hi, incidencias i
						WHERE hi.IdSistema=id_sistema AND
							hi.IdHw=id_hw AND 
							hi.FechaHora >= desde AND
							hi.FechaHora < hasta AND 
							i.IdIncidencia=hi.IdIncidencia;
			ELSE
				INSERT INTO t 
					SELECT hi.*, i.Descripcion FROM HistoricoIncidencias hi, incidencias i
						WHERE hi.IdSistema=id_sistema AND
							hi.TipoHw=tipo_hw AND
							hi.IdHw=id_hw AND 
							hi.FechaHora >= desde AND
							hi.FechaHora < hasta AND 
							i.IdIncidencia=hi.IdIncidencia;
			END IF;
		END IF;
	END IF;
	IF (num_scv=0) THEN
		SELECT * FROM t				
			ORDER BY -FechaHora;
	ELSE
		SELECT * FROM t				
			WHERE Scv=Num_scv
			ORDER BY -FechaHora;
	END IF;
		
	DROP TABLE t;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `GetMtbf` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `GetMtbf`(in id_sistema varchar(32), in desde date, in hasta date)
BEGIN
	SELECT COUNT(*) AS mtbf 
		FROM historicoincidencias 
		WHERE DATE_FORMAT(FechaHora,"%Y/%m/%d")>=desde AND 
				DATE_FORMAT(FechaHora,"%Y/%m/%d")<hasta AND
				idIncidencia in (select idIncidencia from incidencias i where i.GeneraError=1 AND i.IdIncidenciaCorrectora is not null);
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `GetMtbfComponents` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `GetMtbfComponents`(in id_sistema varchar(32), in desde date, in hasta date, in components varchar(125))
BEGIN
	SELECT COUNT(*) AS mtbf 
		FROM historicoincidencias 
		WHERE DATE_FORMAT(FechaHora,"%Y/%m/%d")>=desde AND 
				DATE_FORMAT(FechaHora,"%Y/%m/%d")<hasta AND
                FIND_IN_SET(IdHw,components) > 0 AND
				idIncidencia in (select idIncidencia from incidencias i where i.GeneraError=1 AND i.IdIncidenciaCorrectora is not null);
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `GetMtbfEvents` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `GetMtbfEvents`(in id_sistema varchar(32), in desde date, in hasta date, in evento int)
BEGIN
	SELECT COUNT(*) AS mtbf 
		FROM historicoincidencias 
		WHERE DATE_FORMAT(FechaHora,"%Y/%m/%d")>=desde AND 
				DATE_FORMAT(FechaHora,"%Y/%m/%d")<hasta AND
                idIncidencia = evento;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `GetRate` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `GetRate`(in id_sistema varchar(32), in desde date, in hasta date)
BEGIN
SELECT
(SELECT COUNT(*) AS fallos 
	FROM historicoincidencias 
    WHERE DATE_FORMAT(FechaHora,"%Y/%m/%d")>=desde AND 
			DATE_FORMAT(FechaHora,"%Y/%m/%d")<hasta AND
            idIncidencia in (select idIncidencia from incidencias i where i.GeneraError=1 AND i.IdIncidenciaCorrectora is not null))
/
(SELECT COUNT(*) AS total
	FROM historicoincidencias 
    WHERE DATE_FORMAT(FechaHora,"%Y/%m/%d")>=desde AND 
			DATE_FORMAT(FechaHora,"%Y/%m/%d")<hasta) * 100 AS rate;

END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `GetRateComponents` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `GetRateComponents`(in id_sistema varchar(32), in desde date, in hasta date, in components text)
BEGIN
SELECT
(SELECT COUNT(*) AS fallos 
	FROM historicoincidencias 
    WHERE DATE_FORMAT(FechaHora,"%Y/%m/%d")>=desde AND 
			DATE_FORMAT(FechaHora,"%Y/%m/%d")<hasta  AND
			FIND_IN_SET(IdHw,components) > 0 AND
            idIncidencia in (select idIncidencia from incidencias i where i.GeneraError=1 AND i.IdIncidenciaCorrectora is not null))
/
(SELECT COUNT(*) AS total
	FROM historicoincidencias 
    WHERE DATE_FORMAT(FechaHora,"%Y/%m/%d")>=desde  AND
			FIND_IN_SET(IdHw,components) > 0 AND
			DATE_FORMAT(FechaHora,"%Y/%m/%d")<hasta) * 100 AS rate;

END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `GetRateEvents` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `GetRateEvents`(in id_sistema varchar(32), in desde date, in hasta date, in evento int)
BEGIN
	SELECT
	(SELECT COUNT(*) AS fallos 
		FROM historicoincidencias 
		WHERE DATE_FORMAT(FechaHora,"%Y/%m/%d")>=desde AND 
				DATE_FORMAT(FechaHora,"%Y/%m/%d")<hasta  AND
				idIncidencia = evento)
	/
	(SELECT COUNT(*) AS total
		FROM historicoincidencias 
		WHERE   idIncidencia in (select idIncidencia from incidencias i where i.GeneraError=1 AND i.IdIncidenciaCorrectora is not null) AND
				DATE_FORMAT(FechaHora,"%Y/%m/%d")>=desde  AND
				DATE_FORMAT(FechaHora,"%Y/%m/%d")<hasta) * 100 AS rate;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `GetUnHistorico` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `GetUnHistorico`(in id_sistema varchar(32),in lista_incidencias text,in tipo_hw int, in id_hw varchar(32), in desde date, in hasta date)
BEGIN
	declare i int;
	declare pos int;
	declare cadena text;
	set i=LOCATE(",",lista_incidencias);
	set pos=1;
	create temporary table if not exists t SELECT hi.*, i.Descripcion AS Descriptor FROM HistoricoIncidencias hi, incidencias i where false;
	IF (lista_incidencias<>"") THEN												
		IF (id_hw="") THEN
			IF (tipo_hw=-1) THEN																		
				WHILE i>0 DO
					set cadena=substring(lista_incidencias,pos,i-pos);
					set pos=i+1;
					set i=LOCATE(",",lista_incidencias,pos);
		
					INSERT INTO t 
					SELECT hi.*, i.Descripcion FROM HistoricoIncidencias hi, incidencias i
						WHERE hi.IdSistema=id_sistema AND
							hi.IdIncidencia=cadena AND 
							DATE_FORMAT(hi.FechaHora,"%Y/%m/%d") >= desde AND
							DATE_FORMAT(hi.FechaHora,"%Y/%m/%d") < hasta AND
							i.IdIncidencia=hi.IdIncidencia
						LIMIT 1;
					END WHILE;
			ELSE
				WHILE i>0 DO
					set cadena=substring(lista_incidencias,pos,i-pos);
					set pos=i+1;
					set i=LOCATE(",",lista_incidencias,pos);
		
					INSERT INTO t 
					SELECT hi.*, i.Descripcion FROM HistoricoIncidencias hi, incidencias i
						WHERE hi.IdSistema=id_sistema AND
							hi.TipoHw = tipo_hw AND
							hi.IdIncidencia=cadena AND 
							DATE_FORMAT(hi.FechaHora,"%Y/%m/%d") >= desde AND
							DATE_FORMAT(hi.FechaHora,"%Y/%m/%d") < hasta AND
							i.IdIncidencia=hi.IdIncidencia
						LIMIT 1;
					END WHILE;
			END IF;
		ELSE
			IF (tipo_hw=-1) THEN																		
				WHILE i>0 DO
					set cadena=substring(lista_incidencias,pos,i-pos);
					set pos=i+1;
					set i=LOCATE(",",lista_incidencias,pos);
		
					INSERT INTO t 
					SELECT hi.*, i.Descripcion FROM HistoricoIncidencias hi, incidencias i
						WHERE hi.IdSistema=id_sistema AND
							hi.IdHw=id_hw AND 
							hi.IdIncidencia=cadena AND 
							DATE_FORMAT(hi.FechaHora,"%Y/%m/%d") >= desde AND
							DATE_FORMAT(hi.FechaHora,"%Y/%m/%d") < hasta AND
							i.IdIncidencia=hi.IdIncidencia
						LIMIT 1;
					END WHILE;
			ELSE
				WHILE i>0 DO
					set cadena=substring(lista_incidencias,pos,i-pos);
					set pos=i+1;
					set i=LOCATE(",",lista_incidencias,pos);
		
					INSERT INTO t 
					SELECT hi.*, i.Descripcion FROM HistoricoIncidencias hi, incidencias i
						WHERE hi.IdSistema=id_sistema AND
							hi.IdHw=id_hw AND 
							hi.TipoHw=tipo_hw AND 
							hi.IdIncidencia=cadena AND 
							DATE_FORMAT(hi.FechaHora,"%Y/%m/%d") >= desde AND
							DATE_FORMAT(hi.FechaHora,"%Y/%m/%d") < hasta AND
							i.IdIncidencia=hi.IdIncidencia
						LIMIT 1;
					END WHILE;
			END IF;
		END IF;
	ELSE
		IF (id_hw="") THEN
			IF (tipo_hw=-1) THEN																		
				INSERT INTO t 
					SELECT hi.*,i.Descripcion FROM HistoricoIncidencias hi, incidencias i WHERE hi.IdSistema=id_sistema AND 
								DATE_FORMAT(hi.FechaHora,"%Y/%m/%d") >= desde AND
								DATE_FORMAT(hi.FechaHora,"%Y/%m/%d") < hasta AND
								i.IdIncidencia=hi.IdIncidencia
						LIMIT 1;
			ELSE
				INSERT INTO t 
					SELECT hi.*, i.Descripcion FROM HistoricoIncidencias  hi, incidencias i
						WHERE hi.IdSistema=id_sistema AND
									hi.TipoHw=tipo_hw AND 
									DATE_FORMAT(hi.FechaHora,"%Y/%m/%d") >= desde AND
									DATE_FORMAT(hi.FechaHora,"%Y/%m/%d") < hasta AND
								i.IdIncidencia=hi.IdIncidencia
						LIMIT 1;
			END IF;
		ELSE
			IF (tipo_hw=-1) THEN																		
				INSERT INTO t 
					SELECT hi.*, i.Descripcion FROM HistoricoIncidencias hi, incidencias i
						WHERE hi.IdSistema=id_sistema AND
							hi.IdHw=id_hw AND 
							DATE_FORMAT(hi.FechaHora,"%Y/%m/%d") >= desde AND
							DATE_FORMAT(hi.FechaHora,"%Y/%m/%d") < hasta AND
							i.IdIncidencia=hi.IdIncidencia
						LIMIT 1;
			ELSE
				INSERT INTO t 
					SELECT hi.*, i.Descripcion FROM HistoricoIncidencias hi, incidencias i
						WHERE hi.IdSistema=id_sistema AND
							hi.TipoHw=tipo_hw AND
							hi.IdHw=id_hw AND 
							DATE_FORMAT(hi.FechaHora,"%Y/%m/%d") >= desde AND
							DATE_FORMAT(hi.FechaHora,"%Y/%m/%d") < hasta AND
							i.IdIncidencia=hi.IdIncidencia
						LIMIT 1;
			END IF;
		END IF;
	END IF;
	SELECT * FROM t				
		ORDER BY -FechaHora LIMIT 1;
		
	DROP TABLE t;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `IdManttoLibre` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `IdManttoLibre`(in id_Sistema varchar(32), out id_Sacta int)
BEGIN
	DECLARE encontrado bool default false;
	DECLARE nSacta int default 10000;
	DECLARE id int;
	
	set id_Sacta=0;
	
	WHILE ((not encontrado) AND (nSacta < 20000)) DO
		SELECT COUNT(*) INTO id FROM sectores WHERE IdSistema=id_sistema AND NumSacta=nSacta;
		IF (id=0) THEN
			SET encontrado=true;
			SET id_Sacta=nSacta;
		ELSE
			SET nSacta=nSacta+1;
		END IF;
	END WHILE;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `IniciaTransaccion` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `IniciaTransaccion`()
BEGIN
		create table if not exists cd40_trans.sistema like cd40.sistema;
		create table if not exists cd40_trans.abonados like cd40.abonados;
		
		start transaction;
		
		delete from cd40_trans.sistema;
		delete from cd40_trans.abonados;
		
		insert into cd40_trans.sistema select * from cd40.sistema;
		insert into cd40_trans.abonados select * from cd40.abonados;
		
		commit;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `ListaRecursosDestino` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `ListaRecursosDestino`(in id_sistema varchar(32), in id_destino varchar(700), in id_prefijo int, in id_dependencia varchar(32))
BEGIN
	IF (id_prefijo=1) THEN
    IF (id_dependencia='') THEN
  		SELECT rLC.*,r.Interface FROM recursoslcen rLC, recursos r
  		WHERE rLC.IdSistema=id_sistema AND
  					rLC.IdDestino=id_destino AND
  					rLC.IdPrefijo=id_prefijo AND
  					r.IdSistema=rLC.IdSistema AND
  					r.IdRecurso=rLC.IdRecurso AND
  					r.TipoRecurso=rLC.TipoRecurso;
    ELSE
    SELECT rLC.*,r.Interface,gr.IdGrupo FROM recursoslcen rLC, recursos r, destinostelefonia gr
		WHERE rLC.IdSistema=id_sistema AND
          gr.IdGrupo=id_dependencia AND
					rLC.IdDestino=gr.IdDestino AND
					rLC.IdPrefijo=1 AND
					r.IdSistema=rLC.IdSistema AND
					r.IdRecurso=rLC.IdRecurso AND
					r.TipoRecurso=rLC.TipoRecurso;
    END IF;
	ELSE
		SELECT rTf.*,r.Interface FROM recursostf rTf, recursos r
		WHERE rTf.IdSistema=id_sistema AND
					rTf.IdDestino=id_destino AND
					rTf.IdPrefijo=id_prefijo AND
					r.IdSistema=rTf.IdSistema AND
					r.IdRecurso=rTf.IdRecurso AND
					r.TipoRecurso=rTf.TipoRecurso;
	END IF;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `ListaRecursosLCEN_DestinoATS` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `ListaRecursosLCEN_DestinoATS`(IN id_sistema varchar(32),IN id_sectorizacion varchar(32),IN id_Nucleo varchar(32),IN id_Sector varchar(700), IN id_Grupo varchar(80))
BEGIN
  -- Devuelve la lista de recursos LCEN (Destinos LCEN con idPrefijo=1) asignados a un destino ATS en el panel de línea caliente
  -- (TipoAcceso='IA') del sector idSector en la sectorización activa o en la sectorización id_sectorización
  -- Cuando un destino ATS tiene asociado destinos LCEN, los destinos tienen configurado el mismo grupo.
  
  IF (id_sistema is not null AND id_Grupo is not null AND id_Sector is not null AND id_Nucleo is not null) then
  
    IF (id_sectorizacion is not null) then
      SELECT rLC.*,r.Interface,E.grupo 
			FROM recursoslcen rLC
			JOIN recursos R ON R.IdSistema=rLC.IdSistema AND 
							           R.IdRecurso=rLC.IdRecurso AND 
							           R.TipoRecurso=rLC.TipoRecurso 
			JOIN externos E ON E.IdSistema=rLC.IdSistema AND E.IdDestino=rLC.IdDestino  AND 
							   E.Idprefijo=rLC.IdPrefijo AND 
                               E.IdSector=id_Sector AND
                               E.IdSectorizacion=id_sectorizacion AND
                               E.IdNucleo=id_Nucleo AND
                               E.tipoAcceso='IA' 
			WHERE 
				 rLC.IdSistema=id_sistema AND 
				 rLC.IdPrefijo=1 AND
				 E.grupo=id_Grupo;
    ELSE
      SELECT rLC.*,r.Interface,E.grupo 
        FROM recursoslcen rLC
        JOIN recursos R ON R.IdSistema=rLC.IdSistema AND 
                   R.IdRecurso=rLC.IdRecurso AND 
                   R.TipoRecurso=rLC.TipoRecurso 
        JOIN externos E ON E.IdSistema=rLC.IdSistema AND E.IdDestino=rLC.IdDestino  AND 
                           E.Idprefijo=rLC.IdPrefijo AND E.tipoAcceso='IA' AND 
                           E.IdSector=id_Sector AND E.IdNucleo=id_Nucleo AND
                           EXISTS (SELECT 1 FROM sectorizaciones st 
                                         WHERE st.Activa=TRUE AND 
                                               st.IdSistema=e.IdSistema AND 
                                               E.IdSectorizacion=DATE_FORMAT(st.FECHAACTIVACION,'%e/%m/%Y %H:%i:%S'))
        WHERE 
           rLC.IdSistema=id_sistema AND 
           rLC.IdPrefijo=1 AND
           E.grupo=id_Grupo;
    END IF;
  END IF;
  
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `ListaRutas` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `ListaRutas`(in id_Sistema char(32), in id_Central char(32))
BEGIN
	select IdRuta as IdRuta,Tipo as Tipo from Rutas 
		where IdSistema=id_Sistema AND
				Central=id_Central
		ORDER BY Orden;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `LoginTop` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `LoginTop`(in id_Sistema char(32),in id_Hw char(32), out id_Usuario char(32), out modo_arranque char(1))
BEGIN
	SELECT b.idsector,c.ModoArranque  INTO id_Usuario, modo_arranque
	FROM sectorizaciones a, SectoresSectorizacion b, top c
	WHERE a.idSistema=id_Sistema AND
				c.IdSistema=id_Sistema AND
				a.Activa=true AND
				b.idSistema=id_Sistema AND
				a.idSectorizacion=b.idSectorizacion AND
				b.idTOP=id_Hw AND
				b.IdTop=c.IdTOP;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `MultiDestinosAsignadosAlSector` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `MultiDestinosAsignadosAlSector`(in id_sistema char(32), in id_sector varchar(700))
BEGIN
	SELECT *
	FROM destinosexternossector
	WHERE IdSistema=id_sistema AND
	IdSector = id_sector AND
	TipoAcceso = 'MD';
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `MultiDestinosSinAsignarAlSector` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `MultiDestinosSinAsignarAlSector`(in id_sistema char(32), in id_usuario char(32), in id_nucleo char(32))
BEGIN
	SELECT *
	FROM multidestinos
	WHERE IdSistema LIKE id_sistema
    AND IdNucleo LIKE id_nucleo
	AND IdKey NOT IN
		(SELECT IdMultiDestino
		FROM multidestinos_elementos
		WHERE IdDestino LIKE id_usuario)
	AND IdKey NOT IN
		(SELECT md.IdKey
		FROM multidestinos md, destinosexternossector des
		WHERE md.IdSistema = des.IdSistema AND
		md.IdNucleo = des.IdNucleo AND
		md.IdMultiDestino = IdDestino AND 
		des.IdSector LIKE id_usuario)
	
UNION 
SELECT *
FROM multidestinos
WHERE IdSistema LIKE id_sistema
AND IdNucleo LIKE id_nucleo
AND IdMultiDestino NOT IN
	(SELECT IdDestino
	FROM destinosexternossector
	WHERE IdSistema = id_sistema
	AND IdNucleo = id_nucleo
	AND IdSector = id_usuario)
ORDER BY IdMultiDestino;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `NivelesIntrusion` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `NivelesIntrusion`(in id_sistema char(32), in id_usuario varchar(700))
BEGIN
	SELECT n.* FROM Niveles n, SectoresSector ss
	WHERE ss.IdSistema=id_sistema AND
				ss.IdSector=id_usuario AND
				n.IdSistema=id_sistema AND
				n.IdSector=ss.IdSectorOriginal AND
				ss.EsDominante;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `NivelesIntrusionParaXML` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `NivelesIntrusionParaXML`(in id_sistema varchar(32), in id_sectorizacion varchar(32))
BEGIN
	IF (id_sectorizacion IS NOT NULL) THEN
		SELECT c.IdSector, n.* FROM Niveles n, SectoresSector ss, sectoressectorizacion c
			WHERE ss.IdSistema=id_sistema AND
				c.IdSectorizacion=id_sectorizacion AND
				c.IdSistema=ss.IdSistema AND
				n.IdSistema=ss.IdSistema AND
				n.IdSector=ss.IdSectorOriginal AND
				c.IdSector=ss.IdSector AND
				ss.EsDominante;
	ELSE
		SELECT n.* FROM Niveles n, Sectores s
			WHERE	s.IdSistema=id_sistema AND
						s.SectorSimple AND
						n.IdSistema=s.IdSistema AND
						n.IdSector=s.IdSector AND
						n.IdNucleo=s.IdNucleo;
	END IF;	
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `NumerosAbonadoExternos` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `NumerosAbonadoExternos`(in idSistema char(32), in idUsuario char(32))
BEGIN
	SELECT a.*
		FROM UsuariosAbonados a, Sectorizaciones b
		WHERE b.idSistema=idSistema AND
					b.Activa=true AND
					a.idSectorizacion=b.idSectorizacion AND
					a.idSistema=b.idSistema AND
					a.idSector=idUsuario AND
					a.idPrefijos<>1;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `NumerosAbonadoInternos` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `NumerosAbonadoInternos`(in id_Sistema char(32),in id_Usuario char (32))
BEGIN
	SELECT *
		FROM UsuariosAbonados
		WHERE IdSistema=id_Sistema AND
					IdSector=id_Usuario;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `NumerosAbonadosSectorAgrupado` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `NumerosAbonadosSectorAgrupado`(in id_sistema char(32), in id_sector_agrupado varchar(700))
BEGIN
	SELECT UA.* FROM UsuariosAbonados ua, SectoresAgrupacion ss
	WHERE ss.IdSistema=id_sistema AND
				ua.IdSistema=ss.IdSistema AND
				ss.IdAgrupacion=id_sector_agrupado AND
				ss.IdSector=ua.IdSector;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `NumerosAbonadosSectorNoAgrupado` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `NumerosAbonadosSectorNoAgrupado`(in id_sistema char(32), in id_nucleo char(32), in id_sector_agrupado varchar(700))
BEGIN
	SELECT UA.* FROM UsuariosAbonados ua, SectoresSector ss
	WHERE ss.IdSistema=id_sistema AND	ss.IdNucleo=id_nucleo AND
				ua.IdSistema=ss.IdSistema AND ua.IdNucleo=ss.IdNucleo AND
				ss.IdSector=id_sector_agrupado AND
				ss.IdSectorOriginal=ua.IdSector;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `ParametrosSectorAgrupado` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `ParametrosSectorAgrupado`(in id_sistema char(32), in id_sector_agrupado char(32))
BEGIN
		SELECT ps.* FROM SectoresAgrupacion ss, ParametrosSector ps
		WHERE ss.IdSistema=id_sistema AND
					ps.IdSistema=ss.IdSistema AND
					ss.IdAgrupacion=id_sector_agrupado AND
					ss.IdSector=ps.IdSector;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `ParametrosSectores` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `ParametrosSectores`(in id_sistema varchar(32), in lista_nucleos varchar(255), in lista_sectores varchar(700))
BEGIN
	declare i int;
	declare pos int;
	declare nucleo text;
	declare j int;
	declare posj int;
	declare sector text;
	set i=LOCATE(",",lista_nucleos);
	set pos=1;
	create temporary table if not exists ps SELECT * FROM ParametrosSector where false;
	WHILE i>0 DO
		set j=LOCATE(",",lista_sectores);
		set posj=1;
		set nucleo=substring(lista_nucleos,pos,i-pos);
		set pos=i+1;
		set i=LOCATE(",",lista_nucleos,pos);
		WHILE j>0 DO
			set sector=substring(lista_sectores,posj,j-posj);
			set posj=j+1;
			set j=LOCATE(",",lista_sectores,posj);
			INSERT INTO ps
			SELECT * FROM ParametrosSector
				WHERE IdSistema=id_sistema AND
							IdNucleo=nucleo AND
							IdSector=sector;
					
			END WHILE;
		END WHILE;
		
		SELECT * FROM ps;
		DROP TABLE ps;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `ParametrosSectoresParaXML` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `ParametrosSectoresParaXML`(in id_sistema varchar(32), in id_sectorizacion varchar(32))
BEGIN
	IF (id_sectorizacion IS NOT NULL) THEN
		SELECT c.IdSector, n.* FROM ParametrosSector n, SectoresSector ss, sectoressectorizacion c
		WHERE ss.IdSistema=id_sistema AND
				c.IdSectorizacion=id_sectorizacion AND
				c.IdSistema=ss.IdSistema AND
				n.IdSistema=ss.IdSistema AND
				n.IdSector=ss.IdSectorOriginal AND
				c.IdSector=ss.IdSector AND
				ss.EsDominante;
	ELSE
		SELECT n.* FROM ParametrosSector n, Sectores s
			WHERE	s.IdSistema=id_sistema AND
						s.SectorSimple AND
						n.IdSistema=s.IdSistema AND
						n.IdSector=s.IdSector AND
						n.IdNucleo=s.IdNucleo;
	END IF;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `ParametrosSectorNoAgrupado` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `ParametrosSectorNoAgrupado`(in id_sistema char(32), in id_sector_agrupado varchar(700))
BEGIN
	SELECT ps.* FROM SectoresSector ss, ParametrosSector ps
		WHERE ss.IdSistema=id_sistema AND
					ps.IdSistema=ss.IdSistema AND
					ss.IdSector=id_sector_agrupado AND
					ss.IdSectorOriginal=ps.IdSector;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `PermisosRedes` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `PermisosRedes`(in idSistema char(32), in idUsuario varchar(700))
BEGIN
    -- Obtiene los permisos de las redes con prefijo >3
	SELECT r.Idred, pr.Llamar, pr.Recibir,pr.IdRecursoPref,pr.TipoRecursoPref
    FROM redes r
    LEFT OUTER JOIN permisosredes pr ON
        r.IdSistema = pr.IdSistema AND
        r.IdRed = pr.IdRed   AND
        r.IdSistema = idSistema AND 
        pr.IdSector = idUsuario
     WHERE r.IdPrefijo>3;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `PlanDireccionamientoIP` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `PlanDireccionamientoIP`(in id_Sistema char(32))
BEGIN
  -- Obtiene el plan de direccionamiento IP.
 -- Si el equipo externo de telefonía pertenece a una central IP (tipoEquipo=3 y Min=Max=-1) ,EsCentralIP toma el valor true
 -- Si el equipo externo de telefonía no pertenece a una central Ip (tipoEquipo=3 y Min!=-1 y  Max!=-1) , se devuelven las direcciones IP 
 -- con el puerto sip configurado en el equipo externo IpRed1:PuertoSip, IpRex2:PuertoSip.
 
 -- Devuelve el identificador, tipoequipo, dirección IP 1, dirección IP2, Es Central telefonía interna (interno),
 -- valor máximo y mínimo del rango si se trata de una central, EsCentralIP=true si el equipo externo pertenece   
 -- a una central (encaminamient) IP
 -- Si el equipo esterno es central IP, puede tener configurados los campos IpRed3, SrvPresenciaIpRed1,
 -- SrvPresenciaIpRed2, SrvPresenciaIpRed3
 
	(SELECT IdTop as Id,0 as TipoEH, IpRed1, IpRed2, 0 as Interno, 0 as Min, 0 as Max, 0 as EsCentralIP,null as IpRed3,
            null as SrvPresenciaIpRed1, null as SrvPresenciaIpRed2, null as SrvPresenciaIpRed3
		FROM TOP
		WHERE IdSistema=id_Sistema)
	UNION
	(SELECT t.IdTifX as Id,1 as TipoEH , g.IpRed, t.IpRed2, 0 as Interno, 0 as Min, 0 as Max,0 as EsCentralIP,null as IpRed3,
        null as SrvPresenciaIpRed1, null as SrvPresenciaIpRed2, null as SrvPresenciaIpRed3
		FROM TIFX t, GWACTIVAS g
		WHERE t.IdSistema=id_Sistema AND
                      g.IdSistema = t.IdSistema AND
                      g.IdTifx = t.IdTIFX)
	UNION
	(SELECT IdEquipos as Id,TipoEquipo as TipoEH , 
          (CASE WHEN eu.TipoEquipo=3 and eu.Min!= -1 and eu.Max!=-1 AND eu.IpRed1 IS NOT NULL AND eu.SipPort IS NOT NULL THEN CONCAT(IpRed1,':',eu.SipPort) ELSE IpRed1 END) as IpRed1,
		      (CASE WHEN eu.TipoEquipo=3 and eu.Min!= -1 and eu.Max!=-1 AND eu.IpRed2 IS NOT NULL AND eu.SipPort IS NOT NULL THEN CONCAT(IpRed2,':',eu.SipPort) ELSE IpRed2 END) as IpRed2,
          Interno, r.Inicial as Min, r.Final as Max,
          (CASE WHEN eu.TipoEquipo=3 and eu.Min= -1 and eu.Max=-1 THEN true ELSE false END) as EsCentralIP,
          IpRed3,SrvPresenciaIpRed1,SrvPresenciaIpRed2,SrvPresenciaIpRed3
		FROM EQUIPOSEU eu
        LEFT JOIN RANGOS r ON  r.IdSistema = eu.IdSistema AND r.Central = eu.IdEquipos
		WHERE eu.IdSistema=id_Sistema);
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `PlanDireccionamientoSIP` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `PlanDireccionamientoSIP`(in id_Sistema char(32))
BEGIN
-- Obtiene el plan de direccionamiento SIP
-- Los registros deben ir ordenados por IdAgrupación (c.IdSector) y IdSector (ss.IdSectorOriginal)
-- Para que el Web service GetPlanDireccionamientoSIP muestre dentro del mismo sector
-- los número de abonados a los que atiende
SELECT a.IdPrefijo,a.IdAbonado,ss.IdSectorOriginal as IdSector,c.IdSector as IdAgrupacion
	FROM Sectorizaciones b, SectoresSectorizacion c
	inner join SectoresSector ss ON ss.IdSector= c.IdSector
	inner join UsuariosAbonados a ON a.IdSector=IdSectorOriginal
	WHERE b.IdSistema=id_Sistema AND
						b.Activa=true AND
						b.IdSectorizacion=c.IdSectorizacion
	ORDER BY  c.IdSector,ss.IdSectorOriginal,a.idPrefijo ASC, a.idAbonado ASC;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `PosicionOcupadaEnSector` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `PosicionOcupadaEnSector`(in id_sistema char(32), in id_nucleo char(32), in id_sector varchar(700), in tipo_acceso char(32), in posicion int, out existe boolean)
BEGIN
	DECLARE existeInterno,existeExterno int;
	
	SELECT count(*) INTO existeInterno
		FROM DestinosInternosSector dis
		WHERE dis.IdSistema=id_sistema AND
					dis.IdNucleo=id_nucleo AND
					dis.IdSector=id_sector AND
					dis.TipoAcceso=tipo_acceso AND
					dis.PosHMI=posicion;
				
	SELECT count(*) INTO existeExterno
		FROM DestinosExternosSector dis
		WHERE dis.IdSistema=id_sistema AND
					dis.IdNucleo=id_nucleo AND
					dis.IdSector=id_sector AND
					dis.TipoAcceso=tipo_acceso AND
					dis.PosHMI=posicion;				
					
	SET existe=((existeInterno>0) OR (existeExterno>0));
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `PrefijosSinAsignarARedes` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `PrefijosSinAsignarARedes`(in id_Sistema char(32))
BEGIN
	select IdPrefijo from Prefijos
	where IdSistema=id_Sistema AND
				IdPrefijo > 3 AND IdPrefijo < 32 AND
				IdPrefijo not in (select IdPrefijo from Redes where IdSistema=id_Sistema);
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `PrimeraPosicionLibre` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `PrimeraPosicionLibre`(in id_sistema char(32), in id_nucleo char(32), in id_sector varchar(700), in tipo_acceso char(2), out hueco int)
BEGIN
	DECLARE i int;
	DECLARE huecoInterno, huecoExterno int;
	DECLARE encontrado boolean;
	SET i=0;
	SET encontrado=false;
	SET huecoInterno=0;
	SET huecoExterno=0;
	WHILE (NOT encontrado) AND (i<56) DO
		SET i=i+1;
		SELECT COUNT(*) INTO huecoInterno
			FROM DestinosInternosSector d
			WHERE d.IdSistema=id_sistema AND
						d.IdNucleo=id_nucleo AND
						d.IdSector=id_sector AND
						d.TipoAcceso=tipo_acceso AND
						d.PosHMI=i;
						
		SELECT COUNT(*) INTO huecoExterno
			FROM DestinosExternosSector d
			WHERE d.IdSistema=id_sistema AND
						d.IdNucleo=id_nucleo AND
						d.IdSector=id_sector AND
						d.TipoAcceso=tipo_acceso AND
						d.PosHMI=i;
						
		SET encontrado=((huecoInterno=0) AND (huecoExterno=0));
	END WHILE;
	SET hueco=i;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `RangosConIdRed` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `RangosConIdRed`(in id_sistema varchar(32), in id_central varchar(32))
BEGIN
	IF (id_central is not null) THEN
		SELECT r.*, red.IdRed
		FROM 	Rangos r
        LEFT JOIN Redes red ON red.IdSistema=r.IdSistema AND r.IdPrefijo = red.IdPrefijo
		WHERE	r.IdSistema=id_sistema AND
					r.Central=id_central;
	ELSE
		SELECT r.*, red.IdRed
		FROM 	Rangos r
        LEFT JOIN Redes red ON red.IdSistema=r.IdSistema AND r.IdPrefijo = red.IdPrefijo
		WHERE	r.IdSistema=id_sistema;
	END IF;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `RecursosLCENSinAsignarAEnlaces` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `RecursosLCENSinAsignarAEnlaces`(in id_sistema char(32))
BEGIN
		SELECT * FROM RecursosLCEN
		WHERE IdSistema=id_sistema AND
					IdDestino is null;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `RecursosLcParaInformeXML` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `RecursosLcParaInformeXML`(in id_sistema varchar(32))
BEGIN
	SELECT r.*, rt.*,pr.*
	FROM recursos r, recursoslcen rt, parametrosRecurso pr
	WHERE r.IdSistema=id_sistema AND
				rt.IdSistema = r.IdSistema AND rt.IdRecurso = r.IdRecurso AND rt.TipoRecurso = r.TipoRecurso AND
				pr.IdSistema = r.IdSistema AND pr.IdRecurso = r.IdRecurso AND pr.TipoRecurso = r.TipoRecurso;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `RecursosPorEmplazamientoParaXML` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `RecursosPorEmplazamientoParaXML`(in id_sistema varchar(32))
BEGIN
		(SELECT t.IdEmplazamiento,rTf.IdRecurso,r.IdTifx, r.SlotPasarela, r.NumDispositivoSlot
			FROM emplazamientos t, recursosradio rTf, recursos r
			WHERE t.IdSistema=id_sistema AND
				rTf.IdSistema=t.IdSistema AND
				rTf.IdEmplazamiento=t.IdEmplazamiento AND
				r.IdSistema=rTf.IdSistema AND
				r.IdRecurso=rTf.IdRecurso)
	UNION
		(SELECT t.IdEmplazamiento,null,null,null,null
			FROM emplazamientos t
			WHERE t.IdSistema=id_sistema AND
					t.IdEmplazamiento NOT IN (SELECT DISTINCT(IdEmplazamiento) FROM recursosradio WHERE IdSistema=id_sistema AND IdEmplazamiento IS NOT NULL));
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `RecursosPorTroncalParaXML` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `RecursosPorTroncalParaXML`(in id_sistema varchar(32))
BEGIN
		(SELECT t.IdTroncal,t.NumTest,rTf.IdRecurso,r.IdTifx, r.SlotPasarela, r.NumDispositivoSlot
			FROM troncales t, recursostf rTf, recursos r
			WHERE t.IdSistema=id_sistema AND
				rTf.IdSistema=t.IdSistema AND
				rTf.IdTroncal=t.IdTroncal AND
				r.IdSistema=rTf.IdSistema AND
				r.IdRecurso=rTf.IdRecurso)
	UNION
		(SELECT t.IdTroncal,t.NumTest,null,null,null,null
			FROM troncales t
			WHERE t.IdSistema=id_sistema AND
					t.IdTroncal NOT IN (SELECT DISTINCT(IdTroncal) FROM recursostf WHERE IdSistema=id_sistema AND IdTroncal IS NOT NULL));
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `RecursosRadioSinAsignarAEnlaces` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `RecursosRadioSinAsignarAEnlaces`(in id_sistema char(32), in id_site char(32))
BEGIN
	IF (id_site is not null) THEN
		SELECT RR.IdSistema, RR.IdRecurso, R.Tipo FROM RecursosRadio RR, Recursos R
			WHERE RR.IdSistema=id_sistema AND
				  RR.IdDestino is null AND 
				  RR.IdRecurso = R.IdRecurso AND
				  RR.IdEmplazamiento = id_site;
	ELSE
		SELECT RR.IdSistema, RR.IdRecurso, R.Tipo FROM RecursosRadio RR, Recursos R
			WHERE RR.IdSistema=id_sistema AND
				  RR.IdDestino is null AND 
				  RR.IdRecurso = R.IdRecurso;
	END IF;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `RecursosRdParaInformeXML` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `RecursosRdParaInformeXML`(in id_sistema varchar(32))
BEGIN
	SELECT r.*, rt.*,pr.*
	FROM recursos r, recursosradio rt, parametrosRecurso pr
	WHERE r.IdSistema=id_sistema AND
				rt.IdSistema = r.IdSistema AND rt.IdRecurso = r.IdRecurso AND rt.TipoRecurso = r.TipoRecurso AND
				pr.IdSistema = r.IdSistema AND pr.IdRecurso = r.IdRecurso AND pr.TipoRecurso = r.TipoRecurso;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `RecursosSinAsignar` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `RecursosSinAsignar`(in id char(32),in id_Sistema char(32))
BEGIN
	select * from recursos
	where IdSistema=id_Sistema && @id is null;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `RecursosTfParaInformeXML` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `RecursosTfParaInformeXML`(in id_sistema varchar(32))
BEGIN
	SELECT r.*, pr.*, rt.IdPrefijo,rt.TipoDestino,rt.IdDestino,rt.IdTroncal,rt.IdRed,rt.Lado,rt.Modo
	FROM recursos r, recursostf rt, parametrosRecurso pr
	WHERE r.IdSistema=id_sistema AND
				rt.IdSistema = r.IdSistema AND rt.IdRecurso = r.IdRecurso AND rt.TipoRecurso = r.TipoRecurso AND
				pr.IdSistema = r.IdSistema AND pr.IdRecurso = r.IdRecurso AND pr.TipoRecurso = r.TipoRecurso;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `RecursosTFSinAsignarAEnlaces` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `RecursosTFSinAsignarAEnlaces`(in id_sistema char(32))
BEGIN
		SELECT rtf.* FROM RecursosTF rtf, recursos r
		WHERE rtf.IdSistema=id_sistema AND
					IdDestino is null AND
					r.IdSistema=id_sistema AND
					r.IdRecurso=rtf.IdRecurso AND
					r.TipoRecurso=rtf.TipoRecurso AND
					r.Interface in (2,3,13);			
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `RedesUsuariosAbonados` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `RedesUsuariosAbonados`(in id_Sistema char(32), in id_Nucleo char(32), in id_Sector varchar(700))
BEGIN
	SELECT red.IdRed, red.IdPrefijo, abonado.IdAbonado 
	FROM Redes red, UsuariosAbonados abonado
	WHERE abonado.IdSistema=id_Sistema AND
				abonado.IdNucleo=id_Nucleo AND
				abonado.IdSector=id_Sector AND
				red.IdSistema=abonado.IdSistema AND
				red.IdPrefijo=abonado.IdPrefijo;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `RedesUsuariosAbonadosParaXML` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `RedesUsuariosAbonadosParaXML`(in id_sistema varchar(32), in id_sectorizacion varchar(32))
BEGIN
			SELECT abonado.IdNucleo, c.IdSector,c.IdSectorOriginal, red.IdRed, red.IdPrefijo, abonado.IdAbonado 
			FROM Redes red, UsuariosAbonados abonado, sectoressector c
			WHERE abonado.IdSistema=id_sistema AND
						abonado.IdSistema=c.IdSistema AND
						abonado.IdNucleo=c.IdNucleo AND
						abonado.IdSector=c.IdSectorOriginal AND
						red.IdSistema=abonado.IdSistema AND
						red.IdPrefijo=abonado.IdPrefijo;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `SectorConNumeroAbonado` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `SectorConNumeroAbonado`(in id_sistema char(32), in id_nucleo char(32), in id_prefijo int, in id_abonado char(32))
BEGIN
	SELECT s.IdSector FROM UsuariosAbonados ua, Sectores s
		WHERE ua.IdSistema=id_sistema AND
					ua.IdNucleo=id_nucleo AND
					ua.IdPrefijo=id_prefijo AND
					ua.IdAbonado=id_abonado AND
					s.IdSistema=ua.IdSistema AND
					s.IdNucleo=ua.IdNucleo AND
					s.SectorSimple AND
					ua.IdSector=s.IdSector;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `SectoresCompuestosPorSectorOriginal` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `SectoresCompuestosPorSectorOriginal`(in id_sistema char(32), in id_sector_original varchar(700))
BEGIN
	SELECT s.IdSector from SectoresSector ss, Sectores s
	where ss.IdSistema=id_sistema AND
				ss.IdSectorOriginal=id_sector_original AND
				ss.IdSectorOriginal!=ss.IdSector AND
				s.IdSector=ss.IdSector;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `SectoresFueraDeAgrupacion` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `SectoresFueraDeAgrupacion`(in id_sistema varchar(32), in id_agrupacion varchar(32))
BEGIN
	IF id_agrupacion IS NOT NULL THEN
		SELECT s.IdSector FROM Sectores s
			WHERE s.IdSistema=id_sistema AND
						s.SectorSimple=true AND
						s.IdSector NOT IN (SELECT IdSector FROM	SectoresAgrupacion	
																	WHERE IdSistema=id_sistema AND
																				IdAgrupacion=id_agrupacion);
	ELSE
		SELECT s.IdSector FROM Sectores s
			WHERE s.IdSistema=id_sistema AND
						s.SectorSimple=true;
	END IF;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `SectoresManttoEnActiva` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `SectoresManttoEnActiva`(in id_sistema char(32))
BEGIN
	SELECT  s.NumSacta, t.PosicionSacta
	FROM Sectores s, sectorizaciones sct, sectoressectorizacion ss, top t
	WHERE sct.IdSistema=id_sistema AND
				sct.Activa AND
				ss.IdSistema = sct.IdSistema AND ss.IdSectorizacion = sct.IdSectorizacion AND
				ss.IdSistema = s.IdSistema AND ss.IdNucleo = s.IdNucleo AND ss.IdSector = s.IdSector AND s.SectorSimple AND s.Tipo='M' AND
				ss.IdTOP=t.IdTOP;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `SectoresManttoEnTop` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `SectoresManttoEnTop`(in id_sistema varchar(32),in id_sectorizacion varchar(32), in id_top varchar(32))
BEGIN
	select s.* from sectores s, sectoressectorizacion ss, sectoressector sct
	where ss.IdSistema=id_sistema AND
				ss.IdSistema=sct.IdSistema AND
				s.IdSistema=sct.IdSistema AND
				ss.IdSectorizacion=id_sectorizacion AND
				ss.IdTOP=id_top AND
				ss.IdSector=sct.IdSector AND
				sct.IdSectorOriginal=s.IdSector AND
				s.SectorSimple AND
				s.Tipo='M';
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `SectoresNumSactaSorted` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `SectoresNumSactaSorted`(in id_sistema char(32), in id_nucleo char(32), in lista_sectores text)
BEGIN
	DECLARE lista text;
	SET @lista=lista_sectores;
	SELECT IdSector from Sectores
	where IdSistema=id_sistema AND IdNucleo=id_nucleo AND
		IdSector in (@lista)
	ORDER BY NumSacta;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `SectoresReales` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `SectoresReales`(in id_sistema char(32), in id_sectorizacion char(32), out cuantos int)
BEGIN
	SELECT count(*) into cuantos FROM Sectores 
	WHERE IdSistema=id_sistema AND
				SectorSimple AND
				Tipo='R' AND
				IdNucleo IN (SELECT DISTINCT IdNucleo FROM SectoresSectorizacion WHERE IdSistema=id_sistema AND IdSectorizacion=id_sectorizacion) AND 
				IdSector NOT IN (SELECT IdSectorOriginal FROM SectoresSector ss,SectoresSectorizacion sz
													WHERE sz.IdSectorizacion=id_sectorizacion AND
																sz.IdSistema=id_sistema AND
																ss.IdSistema=sz.IdSistema AND
																ss.IdSector=sz.IdSector);
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `SectoresSinAsignarASectorizacion` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `SectoresSinAsignarASectorizacion`(in id_sistema char(32),in id_sectorizacion char(32), in id_nucleo char(32))
BEGIN
  IF id_sectorizacion!='SACTA' THEN
		select a.IdSector, a.Tipo from Sectores a
		where a.IdSistema=id_sistema AND
					a.IdNucleo=id_nucleo AND
					a.SectorSimple=true AND
					a.IdSector not in (select ss.IdSectorOriginal FROM sectoresSectorizacion ssz, sectoresSector ss
																			WHERE ssz.IdSistema=id_sistema AND
																						ssz.IdNucleo=id_nucleo AND
																						ssz.IdSectorizacion=id_sectorizacion AND
																						ss.IdSistema=ssz.IdSistema AND ss.IdNucleo=ssz.IdNucleo AND 
																						ss.IdSector=ssz.IdSector)
	     ORDER BY A.IdSector,A.Tipo;
    ELSE
	   select a.IdSector, a.Tipo from Sectores a
		where a.IdSistema=id_sistema AND
					a.IdNucleo=id_nucleo AND
					a.SectorSimple=true AND
					a.IdSector not in (select ss.IdSectorOriginal FROM sectoresSectorizacion ssz, sectoresSector ss
																			WHERE ssz.IdSistema=id_sistema AND
																						ssz.IdNucleo=id_nucleo AND
																						ssz.IdSectorizacion=id_sectorizacion AND
																						ss.IdSistema=ssz.IdSistema AND ss.IdNucleo=ssz.IdNucleo AND 
																						ss.IdSector=ssz.IdSector) AND A.Tipo='M'
	     ORDER BY A.IdSector,A.Tipo;
    END IF;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `SlotsLibresEnTifx` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `SlotsLibresEnTifx`(in id_sistema char(32), in cuantos int)
BEGIN
	CASE cuantos
		WHEN 8 THEN
				SELECT l.Id FROM (
									select b.idtifx as Id,16-sum(a.2bd) as libres 
																								from (
																											(select idtifx as id,count(*)*2 as 2BD from recursos 
																													where IdSistema=id_sistema AND
																																interface=8 
																																group by idtifx) UNION
																										 	(select idtifx as id,count(*)*8 as 2BD from recursos 
																													where IdSistema=id_sistema AND
																																interface=7 
																																group by idtifx) UNION
																											(select idtifx as id,count(*)*16 as 2BD from recursos 
																													where IdSistema=id_sistema AND
																																interface=9
																																group by idtifx) UNION
																											(select idtifx as id,count(*) as 2BD from recursos 
																													where IdSistema=id_sistema AND
																																interface in (0,1,2,3,4,5,6)
																																group by idtifx) UNION
																											(select idtifx as id,0 as 2BD from TIFX 
																													where IdSistema=id_sistema AND
																																IdTifx NOT IN (SELECT DISTINCT(IdTifx) FROM Recursos WHERE IdSistema=id_sistema))
																											) a, tifx b
													where b.IdSistema=id_sistema AND
							  								a.id=b.IdTIFX
													group by a.id
								) l
				WHERE l.libres>=2;
			WHEN 7 THEN
				SELECT l.Id FROM (
									select b.idtifx as Id,16-sum(a.2bd) as libres 
																								from (
																											(select idtifx as id,((count(*)DIV 2)*4) + ((count(*)%2)*4)as 2BD from recursos 
																													where IdSistema=id_sistema AND
																																interface=8 
																																group by idtifx) UNION
																										 	(select idtifx as id,count(*)*8 as 2BD from recursos 
																													where IdSistema=id_sistema AND
																																interface=7 
																																group by idtifx) UNION
																											(select idtifx as id,count(*)*16 as 2BD from recursos 
																													where IdSistema=id_sistema AND
																																interface=9
																																group by idtifx) UNION
																											(select idtifx as id,count(*) as 2BD from recursos 
																													where IdSistema=id_sistema AND
																																interface in (0,1,2,3,4,5,6)
																																group by idtifx) UNION
																											(select idtifx as id,0 as 2BD from TIFX 
																													where IdSistema=id_sistema AND
																																IdTifx NOT IN (SELECT DISTINCT(IdTifx) FROM Recursos WHERE IdSistema=id_sistema))
																											) a, tifx b
													where b.IdSistema=id_sistema AND
							  								a.id=b.IdTIFX
													group by a.id
								) l
				WHERE l.libres>=8;
			WHEN 9 THEN
				SELECT l.Id FROM (
									select b.idtifx as Id,16-sum(a.2bd) as libres 
																								from (
																											(select idtifx as id,((count(*)DIV 2)*4) + ((count(*)%2)*4)as 2BD from recursos 
																													where IdSistema=id_sistema AND
																																interface=8 
																																group by idtifx) UNION
																										 	(select idtifx as id,count(*)*8 as 2BD from recursos 
																													where IdSistema=id_sistema AND
																																interface=7 
																																group by idtifx) UNION
																											(select idtifx as id,count(*)*16 as 2BD from recursos 
																													where IdSistema=id_sistema AND
																																interface=9
																																group by idtifx) UNION
																											(select idtifx as id,count(*) as 2BD from recursos 
																													where IdSistema=id_sistema AND
																																interface in (0,1,2,3,4,5,6)
																																group by idtifx) UNION
																											(select idtifx as id,0 as 2BD from TIFX 
																													where IdSistema=id_sistema AND
																																IdTifx NOT IN (SELECT DISTINCT(IdTifx) FROM Recursos WHERE IdSistema=id_sistema))
																											) a, tifx b
													where b.IdSistema=id_sistema AND
							  								a.id=b.IdTIFX
													group by a.id
								) l
				WHERE l.libres>=16;
			ELSE
				SELECT l.Id FROM (
									select b.idtifx as Id,16-sum(a.2bd) as libres 
																								from (
																											(select idtifx as id,((count(*)DIV 2)*4) + ((count(*)%2)*4)as 2BD from recursos 
																													where IdSistema=id_sistema AND
																																interface=8 
																																group by idtifx) UNION
																										 	(select idtifx as id,count(*)*8 as 2BD from recursos 
																													where IdSistema=id_sistema AND
																																interface=7 
																																group by idtifx) UNION
																											(select idtifx as id,count(*)*16 as 2BD from recursos 
																													where IdSistema=id_sistema AND
																																interface=9
																																group by idtifx) UNION
																											(select idtifx as id,count(*) as 2BD from recursos 
																													where IdSistema=id_sistema AND
																																interface in (0,1,2,3,4,5,6)
																																group by idtifx) UNION
																											(select idtifx as id,0 as 2BD from TIFX 
																													where IdSistema=id_sistema AND
																																IdTifx NOT IN (SELECT DISTINCT(IdTifx) FROM Recursos WHERE IdSistema=id_sistema))
																											) a, tifx b
													where b.IdSistema=id_sistema AND
							  								a.id=b.IdTIFX
													group by a.id
								) l
				WHERE l.libres>=1;
	END CASE;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `TeclasOcupadasSectores` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `TeclasOcupadasSectores`(in id_sistema char(32))
BEGIN
	SELECT posHMI, IdSector, 'externo' AS destino FROM new_cd40_trans.destinosexternossector
         WHERE IdSistema=id_sistema
	UNION
	SELECT posHMI, IdSector, 'interno' AS destino FROM new_cd40_trans.destinosinternossector
        WHERE IdSistema=id_sistema
	ORDER BY posHMI ASC;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `TeclasSector` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `TeclasSector`(in id_sistema char(32), in id_sector varchar(700))
BEGIN
	SELECT ts.* FROM TeclasSector ts
	WHERE 	ts.IdSistema=id_sistema AND
			ts.IdSector=id_sector;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `TeclasSectorParaXML` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `TeclasSectorParaXML`(in id_sistema varchar(32), in id_sectorizacion varchar(32))
BEGIN
	IF (id_sectorizacion IS NOT NULL) THEN
		SELECT c.IdSector,ts.* FROM TeclasSector ts, sectoressectorizacion c
			WHERE 		ts.IdSistema=id_sistema AND
						c.IdSectorizacion=id_sectorizacion AND
						c.IdSistema=ts.IdSistema AND
						ts.IdSector=c.IdSector;
	ELSE
		SELECT ts.* FROM TeclasSector ts, Sectores s
			WHERE	s.IdSistema=id_sistema AND
						s.SectorSimple AND
						ts.IdSistema=s.IdSistema AND
						ts.IdSector=s.IdSector AND
						ts.IdNucleo=s.IdNucleo;
	END IF;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `TroncalesSinAsignarARutas` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `TroncalesSinAsignarARutas`(in id_Sistema char(32),in id_Central char(32), in id_Ruta char(32))
BEGIN
	IF (id_Ruta="") THEN

			SELECT *
			FROM Troncales
			WHERE IdSistema=id_Sistema AND
							IdTroncal NOT IN (SELECT IdTroncal
											  FROM TroncalesRuta Tr,Rutas r
                                              WHERE tr.IdSistema=id_Sistema AND
												    r.IdSistema=Tr.IdSistema AND
                                                    r.Central=Tr.Central AND
                                                    r.IdRuta=Tr.IdRuta AND
                                                    (r.Tipo='D' OR (r.Tipo='A' AND tr.Central=id_Central)));
	ELSE
			
			SELECT * 
			FROM Troncales 
			WHERE IdSistema=id_Sistema AND 
							IdTroncal NOT IN (SELECT IdTroncal 
											  FROM TroncalesRuta
											  WHERE IdSistema=id_Sistema AND Central=id_Central);
	END IF;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `UsuariosImplicadosEnRecurso` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `UsuariosImplicadosEnRecurso`(in id_sistema char(32), in id_destino varchar(700), in id_Prefijo int)
BEGIN
  -- El procedimiento devuelve los sectores en los que el destino está configurado
  -- en la sectorización activa.

  -- Si el destino no es LCEN y el recurso se encuentra definido dentro de varios sectores que forman una agrupación, 
  -- el procedimiento devuelve todos esos sectores. En la función que invoca a este procedimiento, 
  -- se hará un filtrado para que en este caso sólo se devuelva el sector dominante de la agrupación. 

	IF (id_Prefijo is not null AND id_Prefijo=1) THEN
      -- Si el destino es LCEN (id_Prefijo=1), la asociación viene configurada en los campos IdDestinoLen de la tabla destinosexternosSector
      -- Un destino LCEN sólo puede estar asociado a un destino ATS (D.IdPrefijo=3) en el sistema
		  SELECT D.IdSector    IDSECTOR,
			   E.idsector    SECTORAGRUPADO,
			   C.ESDOMINANTE ES_DOMINANTE
		  FROM Externos E, SectoresSector C,DestinosExternosSector D
		  WHERE 
			 E.IdSistema=C.IdSistema AND
			 E.IdNucleo=C.IdNucleo   AND
			 E.IdSector=C.IdSector   AND   
			 C.IdSistema=D.IdSistema AND   
			 C.IDNUCLEO=D.IdNucleo AND   
			 C.IdSectorOriginal=D.IdSector AND
             E.TipoAcceso=D.TipoAcceso AND       
			 E.idprefijo=D.idPrefijoDestinoLCEN AND 			 
             E.IdDestino=D.IdDestinoLCEN AND 
			 D.IdSistema=id_sistema  AND
             D.idprefijo= 3 AND 
             D.idPrefijoDestinoLCEN=id_Prefijo AND
			 D.IdDestinoLCEN=id_destino AND
			 EXISTS(SELECT 1 FROM Sectorizaciones  ST
					WHERE ST.IdSistema=E.IdSistema AND ST.Activa=true and ST.IdSectorizacion=E.IdSectorizacion)
		  GROUP BY E.IdSector,D.idSector,C.ESDOMINANTE
		  ORDER BY E.idSector ASC, C.Esdominante DESC,D.idSector;
	ELSE  
		SELECT D.IdSector    IDSECTOR,
			   E.idsector    SECTORAGRUPADO,
			   C.ESDOMINANTE ES_DOMINANTE
		  FROM Externos E, SectoresSector C,DestinosExternosSector D
		  WHERE 
			 E.IdSistema=C.IdSistema AND
			 E.IdNucleo=C.IdNucleo   AND
			 E.IdSector=C.IdSector   AND   
			 C.IdSistema=D.IdSistema AND   
			 C.IDNUCLEO=D.IdNucleo AND   
			 C.IdSectorOriginal=D.IdSector AND
			 E.idprefijo=D.idprefijo AND 
			 E.TipoAcceso=D.TipoAcceso AND
			 D.IdSistema=id_sistema  AND
			 D.IdDestino=id_destino AND
			 EXISTS(SELECT 1 FROM Sectorizaciones  ST
					WHERE ST.IdSistema=E.IdSistema AND ST.Activa=true and ST.IdSectorizacion=E.IdSectorizacion) AND
			((E.literal!=E.grupo and E.idprefijo=D.idprefijo  AND E.IdDestino=D.IdDestino)
			  OR
			 (E.literal=E.grupo AND 
			  E.Grupo IN (SELECT dt.IdGrupo FROM destinosTelefonia dt 
						  WHERE dt.IdSistema = D.IdSistema AND dt.IdDestino = D.IdDestino)))
		  GROUP BY E.IdSector,D.idSector,C.ESDOMINANTE
		  ORDER BY E.idSector ASC, C.Esdominante DESC,D.idSector;
	END IF;                
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `VersionConfiguracion` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `VersionConfiguracion`(
  IN   `id_sistema`  varchar(32),
  OUT  `versionCfg`  text
)
BEGIN
	SELECT DATE_FORMAT(FechaActivacion,"%d/%m/%Y %H:%i:%s") INTO versionCfg FROM Sectorizaciones 
			WHERE Activa=true AND
						IdSistema=id_sistema;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;

--
-- Current Database: `new_cd40`
--

USE `new_cd40`;

--
-- Final view structure for view `destinosenequipoexterno`
--

/*!50001 DROP VIEW IF EXISTS `destinosenequipoexterno`*/;
/*!50001 SET @saved_cs_client          = @@character_set_client */;
/*!50001 SET @saved_cs_results         = @@character_set_results */;
/*!50001 SET @saved_col_connection     = @@collation_connection */;
/*!50001 SET character_set_client      = utf8 */;
/*!50001 SET character_set_results     = utf8 */;
/*!50001 SET collation_connection      = utf8_general_ci */;
/*!50001 CREATE ALGORITHM=UNDEFINED */
/*!50013 DEFINER=`root`@`%` SQL SECURITY DEFINER */
/*!50001 VIEW `destinosenequipoexterno` AS select `rt`.`IdDestino` AS `IdDestino`,`e`.`IpRed1` AS `IpRed1`,`e`.`IpRed2` AS `IpRed2`,`e`.`idEquipos` AS `idEquipos` from ((`recursostf` `rt` left join `recursos` `r` on((`r`.`IdRecurso` = `rt`.`IdRecurso`))) join `equiposeu` `e` on((`r`.`idEquipos` = `e`.`idEquipos`))) where (`rt`.`IdDestino` is not null) */;
/*!50001 SET character_set_client      = @saved_cs_client */;
/*!50001 SET character_set_results     = @saved_cs_results */;
/*!50001 SET collation_connection      = @saved_col_connection */;

--
-- Final view structure for view `destinosenequipoexternoparaoptions`
--

/*!50001 DROP VIEW IF EXISTS `destinosenequipoexternoparaoptions`*/;
/*!50001 SET @saved_cs_client          = @@character_set_client */;
/*!50001 SET @saved_cs_results         = @@character_set_results */;
/*!50001 SET @saved_col_connection     = @@collation_connection */;
/*!50001 SET character_set_client      = utf8 */;
/*!50001 SET character_set_results     = utf8 */;
/*!50001 SET collation_connection      = utf8_general_ci */;
/*!50001 CREATE ALGORITHM=UNDEFINED */
/*!50013 DEFINER=`root`@`localhost` SQL SECURITY DEFINER */
/*!50001 VIEW `destinosenequipoexternoparaoptions` AS select `eu`.`idEquipos` AS `idEquipos`,`eu`.`IpRed1` AS `IpRed1`,`eu`.`IpRed2` AS `IpRed2`,`eu`.`SipPort` AS `SipPort`,`eu`.`TipoEquipo` AS `TipoEquipo`,`h`.`ModeloEquipo` AS `modeloequipo`,`r`.`Tipo` AS `tiporadio`,`r`.`IdRecurso` AS `IdRecurso`,`rr`.`IdDestino` AS `IdDestino` from (((`equiposeu` `eu` left join `recursos` `r` on((`r`.`idEquipos` = `eu`.`idEquipos`))) left join `hfparams` `h` on((`h`.`IdRecurso` = `r`.`IdRecurso`))) left join `recursosradio` `rr` on((`rr`.`IdRecurso` = `r`.`IdRecurso`))) where (`eu`.`TipoEquipo` = 2) union select `eu`.`idEquipos` AS `idEquipos`,`eu`.`IpRed1` AS `IpRed1`,`eu`.`IpRed2` AS `IpRed2`,`eu`.`SipPort` AS `SipPort`,`eu`.`TipoEquipo` AS `TipoEquipo`,0 AS `modeloequipo`,0 AS `tiporadio`,`r`.`IdRecurso` AS `IdRecurso`,`rt`.`IdDestino` AS `IdDestino` from (((`equiposeu` `eu` left join `recursos` `r` on((`r`.`idEquipos` = `eu`.`idEquipos`))) left join `hfparams` `h` on((`h`.`IdRecurso` = `r`.`IdRecurso`))) left join `recursostf` `rt` on((`rt`.`IdRecurso` = `r`.`IdRecurso`))) where (`eu`.`TipoEquipo` = 3) union select `eu`.`idEquipos` AS `idEquipos`,`eu`.`IpRed1` AS `IpRed1`,`eu`.`IpRed2` AS `IpRed2`,`eu`.`SipPort` AS `SipPort`,`eu`.`TipoEquipo` AS `TipoEquipo`,0 AS `modeloequipo`,0 AS `tiporadio`,NULL AS `IdRecurso`,NULL AS `IdDestino` from `equiposeu` `eu` where (`eu`.`TipoEquipo` = 5) */;
/*!50001 SET character_set_client      = @saved_cs_client */;
/*!50001 SET character_set_results     = @saved_cs_results */;
/*!50001 SET collation_connection      = @saved_col_connection */;

--
-- Final view structure for view `sectoresentopsparainformexml`
--

/*!50001 DROP VIEW IF EXISTS `sectoresentopsparainformexml`*/;
/*!50001 SET @saved_cs_client          = @@character_set_client */;
/*!50001 SET @saved_cs_results         = @@character_set_results */;
/*!50001 SET @saved_col_connection     = @@collation_connection */;
/*!50001 SET character_set_client      = utf8 */;
/*!50001 SET character_set_results     = utf8 */;
/*!50001 SET collation_connection      = utf8_general_ci */;
/*!50001 CREATE ALGORITHM=UNDEFINED */
/*!50013 DEFINER=`root`@`localhost` SQL SECURITY DEFINER */
/*!50001 VIEW `sectoresentopsparainformexml` AS (select `a`.`IdSistema` AS `Idsistema`,`a`.`IdSectorizacion` AS `IdSectorizacion`,`a`.`IdTOP` AS `IdToP`,`a`.`IdSector` AS `IdSector`,`c`.`IdNucleo` AS `IdNucleo`,`c`.`IdSectorOriginal` AS `IdSectorOriginal` from (`sectoressectorizacion` `a` join `sectoressector` `c`) where ((`a`.`IdSistema` = `c`.`IdSistema`) and (`c`.`IdNucleo` = `a`.`IdNucleo`) and (`c`.`IdSector` = `a`.`IdSector`)) order by `c`.`IdSectorOriginal`) union (select `b`.`IdSistema` AS `IdSistema`,`b`.`IdSectorizacion` AS `IdSectorizacion`,`a`.`IdTOP` AS `IdTOP`,NULL AS `NULL`,NULL AS `My_exp_NULL`,NULL AS `My_exp_1_NULL` from (`top` `a` join `sectorizaciones` `b`) where ((`a`.`IdSistema` = `b`.`IdSistema`) and (not((`a`.`IdSistema`,`b`.`IdSectorizacion`,`a`.`IdTOP`) in (select `sectoressectorizacion`.`IdSistema` AS `IdSistema`,`sectoressectorizacion`.`IdSectorizacion` AS `idsectorizacion`,`sectoressectorizacion`.`IdTOP` AS `IdTOP` from `sectoressectorizacion`))))) */;
/*!50001 SET character_set_client      = @saved_cs_client */;
/*!50001 SET character_set_results     = @saved_cs_results */;
/*!50001 SET collation_connection      = @saved_col_connection */;

--
-- Final view structure for view `viewdestinostelefonia`
--

/*!50001 DROP VIEW IF EXISTS `viewdestinostelefonia`*/;
/*!50001 SET @saved_cs_client          = @@character_set_client */;
/*!50001 SET @saved_cs_results         = @@character_set_results */;
/*!50001 SET @saved_col_connection     = @@collation_connection */;
/*!50001 SET character_set_client      = utf8 */;
/*!50001 SET character_set_results     = utf8 */;
/*!50001 SET collation_connection      = utf8_general_ci */;
/*!50001 CREATE ALGORITHM=UNDEFINED */
/*!50013 DEFINER=`root`@`localhost` SQL SECURITY DEFINER */
/*!50001 VIEW `viewdestinostelefonia` AS (select `a`.`IdSistema` AS `IdSistema`,`a`.`IdDestino` AS `IdDestino`,`a`.`TipoDestino` AS `TipoDestino`,`a`.`IdGrupo` AS `IdGrupo`,`a`.`IdPrefijo` AS `IdPrefijo`,`b`.`IdAbonado` AS `IdAbonado`,NULL AS `IdRed` from (`destinostelefonia` `a` join `destinosexternos` `b`) where ((`a`.`IdSistema` = `b`.`IdSistema`) and (`a`.`IdDestino` = `b`.`IdDestino`) and (`a`.`TipoDestino` = `b`.`TipoDestino`) and (`a`.`IdPrefijo` in (0,1,2,32)) and (`a`.`IdPrefijo` = `b`.`IdPrefijo`)) order by `IdDestino`) union (select `a`.`IdSistema` AS `IdSistema`,`a`.`IdDestino` AS `IdDestino`,`a`.`TipoDestino` AS `TipoDestino`,`a`.`IdGrupo` AS `IdGrupo`,`a`.`IdPrefijo` AS `IdPrefijo`,`b`.`IdAbonado` AS `IdAbonado`,`c`.`IdRed` AS `IdRed` from ((`destinostelefonia` `a` join `destinosexternos` `b`) join `redes` `c`) where ((`a`.`IdSistema` = `b`.`IdSistema`) and (`a`.`IdDestino` = `b`.`IdDestino`) and (`a`.`TipoDestino` = `b`.`TipoDestino`) and (`a`.`IdPrefijo` = `b`.`IdPrefijo`) and (`c`.`IdSistema` = `b`.`IdSistema`) and (`c`.`IdPrefijo` = `b`.`IdPrefijo`)) order by `IdDestino`) */;
/*!50001 SET character_set_client      = @saved_cs_client */;
/*!50001 SET character_set_results     = @saved_cs_results */;
/*!50001 SET collation_connection      = @saved_col_connection */;

--
-- Final view structure for view `viewequiposexternos`
--

/*!50001 DROP VIEW IF EXISTS `viewequiposexternos`*/;
/*!50001 SET @saved_cs_client          = @@character_set_client */;
/*!50001 SET @saved_cs_results         = @@character_set_results */;
/*!50001 SET @saved_col_connection     = @@collation_connection */;
/*!50001 SET character_set_client      = utf8 */;
/*!50001 SET character_set_results     = utf8 */;
/*!50001 SET collation_connection      = utf8_general_ci */;
/*!50001 CREATE ALGORITHM=UNDEFINED */
/*!50013 DEFINER=`root`@`localhost` SQL SECURITY DEFINER */
/*!50001 VIEW `viewequiposexternos` AS select `eu`.`idEquipos` AS `idEquipos`,`eu`.`IpRed1` AS `IpRed1`,`eu`.`TipoEquipo` AS `tipoEquipo`,`h`.`ModeloEquipo` AS `modeloequipo`,`r`.`Tipo` AS `tiporadio` from ((`equiposeu` `eu` left join `recursos` `r` on((`r`.`idEquipos` = `eu`.`idEquipos`))) left join `hfparams` `h` on((`h`.`IdRecurso` = `r`.`IdRecurso`))) */;
/*!50001 SET character_set_client      = @saved_cs_client */;
/*!50001 SET character_set_results     = @saved_cs_results */;
/*!50001 SET collation_connection      = @saved_col_connection */;

--
-- Final view structure for view `viewincidenciasmasalarma`
--

/*!50001 DROP VIEW IF EXISTS `viewincidenciasmasalarma`*/;
/*!50001 SET @saved_cs_client          = @@character_set_client */;
/*!50001 SET @saved_cs_results         = @@character_set_results */;
/*!50001 SET @saved_col_connection     = @@collation_connection */;
/*!50001 SET character_set_client      = utf8 */;
/*!50001 SET character_set_results     = utf8 */;
/*!50001 SET collation_connection      = utf8_general_ci */;
/*!50001 CREATE ALGORITHM=UNDEFINED */
/*!50013 DEFINER=`root`@`localhost` SQL SECURITY DEFINER */
/*!50001 VIEW `viewincidenciasmasalarma` AS select `s`.`IdSistema` AS `IdSistema`,`i`.`IdIncidencia` AS `IdIncidencia`,`i`.`Incidencia` AS `Incidencia`,(select `alarmas`.`Alarma` AS `Alarma` from `alarmas` where ((`alarmas`.`IdSistema` = `s`.`IdSistema`) and (`alarmas`.`IdIncidencia` = `i`.`IdIncidencia`))) AS `alarma` from (`incidencias` `i` join `sistema` `s`) order by `s`.`IdSistema`,`i`.`Incidencia` */;
/*!50001 SET character_set_client      = @saved_cs_client */;
/*!50001 SET character_set_results     = @saved_cs_results */;
/*!50001 SET collation_connection      = @saved_col_connection */;

--
-- Final view structure for view `viewincidenciasmasalarma_ingles`
--

/*!50001 DROP VIEW IF EXISTS `viewincidenciasmasalarma_ingles`*/;
/*!50001 SET @saved_cs_client          = @@character_set_client */;
/*!50001 SET @saved_cs_results         = @@character_set_results */;
/*!50001 SET @saved_col_connection     = @@collation_connection */;
/*!50001 SET character_set_client      = utf8 */;
/*!50001 SET character_set_results     = utf8 */;
/*!50001 SET collation_connection      = utf8_general_ci */;
/*!50001 CREATE ALGORITHM=UNDEFINED */
/*!50013 DEFINER=`root`@`localhost` SQL SECURITY DEFINER */
/*!50001 VIEW `viewincidenciasmasalarma_ingles` AS select `s`.`IdSistema` AS `IdSistema`,`i`.`IdIncidencia` AS `IdIncidencia`,`i`.`Incidencia` AS `Incidencia`,(select `alarmas`.`Alarma` AS `Alarma` from `alarmas` where ((`alarmas`.`IdSistema` = `s`.`IdSistema`) and (`alarmas`.`IdIncidencia` = `i`.`IdIncidencia`))) AS `alarma` from (`incidencias_ingles` `i` join `sistema` `s`) order by `s`.`IdSistema`,`i`.`Incidencia` */;
/*!50001 SET character_set_client      = @saved_cs_client */;
/*!50001 SET character_set_results     = @saved_cs_results */;
/*!50001 SET collation_connection      = @saved_col_connection */;

--
-- Final view structure for view `viewrecursosimplicadosrutas`
--

/*!50001 DROP VIEW IF EXISTS `viewrecursosimplicadosrutas`*/;
/*!50001 SET @saved_cs_client          = @@character_set_client */;
/*!50001 SET @saved_cs_results         = @@character_set_results */;
/*!50001 SET @saved_col_connection     = @@collation_connection */;
/*!50001 SET character_set_client      = utf8 */;
/*!50001 SET character_set_results     = utf8 */;
/*!50001 SET collation_connection      = utf8_general_ci */;
/*!50001 CREATE ALGORITHM=UNDEFINED */
/*!50013 DEFINER=`root`@`localhost` SQL SECURITY DEFINER */
/*!50001 VIEW `viewrecursosimplicadosrutas` AS (select `rt`.`IdSistema` AS `idsistema`,`rt`.`IdRecurso` AS `idrecurso`,`rt`.`IdDestino` AS `iddestino`,`rt`.`IdRed` AS `idred`,`rt`.`IdTroncal` AS `idtroncal`,`r`.`IdRuta` AS `idruta`,`r`.`Tipo` AS `tipo` from ((`recursostf` `rt` join `troncalesruta` `tr`) join `rutas` `r`) where ((`tr`.`IdSistema` = `rt`.`IdSistema`) and (`r`.`IdSistema` = `tr`.`IdSistema`) and (`rt`.`IdTroncal` is not null) and (`rt`.`IdTroncal` = `tr`.`IdTroncal`) and (`tr`.`IdRuta` = `r`.`IdRuta`)) order by `rt`.`IdSistema`,`rt`.`IdRecurso`) union (select `rt`.`IdSistema` AS `idsistema`,`rt`.`IdRecurso` AS `idrecurso`,`rt`.`IdDestino` AS `IdDestino`,`rt`.`IdRed` AS `idred`,NULL AS `idtroncal`,NULL AS `idruta`,NULL AS `tipo` from `recursostf` `rt` where isnull(`rt`.`IdTroncal`) order by `rt`.`IdSistema`,`rt`.`IdRecurso`) */;
/*!50001 SET character_set_client      = @saved_cs_client */;
/*!50001 SET character_set_results     = @saved_cs_results */;
/*!50001 SET collation_connection      = @saved_col_connection */;

--
-- Final view structure for view `viewsectoresenagrupacion`
--

/*!50001 DROP VIEW IF EXISTS `viewsectoresenagrupacion`*/;
/*!50001 SET @saved_cs_client          = @@character_set_client */;
/*!50001 SET @saved_cs_results         = @@character_set_results */;
/*!50001 SET @saved_col_connection     = @@collation_connection */;
/*!50001 SET character_set_client      = utf8 */;
/*!50001 SET character_set_results     = utf8 */;
/*!50001 SET collation_connection      = utf8_general_ci */;
/*!50001 CREATE ALGORITHM=UNDEFINED */
/*!50013 DEFINER=`root`@`localhost` SQL SECURITY DEFINER */
/*!50001 VIEW `viewsectoresenagrupacion` AS select count(0) AS `contador`,`sectoresagrupacion`.`IdAgrupacion` AS `IdAgrupacion` from `sectoresagrupacion` group by `sectoresagrupacion`.`IdAgrupacion` */;
/*!50001 SET character_set_client      = @saved_cs_client */;
/*!50001 SET character_set_results     = @saved_cs_results */;
/*!50001 SET collation_connection      = @saved_col_connection */;

--
-- Final view structure for view `viewsectoresentops`
--

/*!50001 DROP VIEW IF EXISTS `viewsectoresentops`*/;
/*!50001 SET @saved_cs_client          = @@character_set_client */;
/*!50001 SET @saved_cs_results         = @@character_set_results */;
/*!50001 SET @saved_col_connection     = @@collation_connection */;
/*!50001 SET character_set_client      = utf8 */;
/*!50001 SET character_set_results     = utf8 */;
/*!50001 SET collation_connection      = utf8_general_ci */;
/*!50001 CREATE ALGORITHM=UNDEFINED */
/*!50013 DEFINER=`root`@`localhost` SQL SECURITY DEFINER */
/*!50001 VIEW `viewsectoresentops` AS (select `sectoressectorizacion`.`IdSistema` AS `IdSistema`,`sectoressectorizacion`.`IdSectorizacion` AS `idsectorizacion`,`sectoressectorizacion`.`IdTOP` AS `IdTOP`,`sectoressectorizacion`.`IdSector` AS `idsector`,`sectoressectorizacion`.`IdNucleo` AS `idnucleo`,`s`.`Tipo` AS `tipo` from (`sectoressectorizacion` join `sectores` `s`) where ((`s`.`IdSistema` = `sectoressectorizacion`.`IdSistema`) and (`s`.`IdNucleo` = `sectoressectorizacion`.`IdNucleo`) and (`s`.`IdSector` = `sectoressectorizacion`.`IdSector`) and (`sectoressectorizacion`.`IdSistema`,`sectoressectorizacion`.`IdSectorizacion`,`sectoressectorizacion`.`IdTOP`) in (select `a`.`IdSistema` AS `IdSistema`,`b`.`IdSectorizacion` AS `idsectorizacion`,`a`.`IdTOP` AS `IdTOP` from (`top` `a` join `sectorizaciones` `b`) where (`a`.`IdSistema` = `b`.`IdSistema`)))) union (select `a`.`IdSistema` AS `IdSistema`,`b`.`IdSectorizacion` AS `idsectorizacion`,`a`.`IdTOP` AS `IdTOP`,NULL AS `NULL`,NULL AS `My_exp_NULL`,NULL AS `My_exp_1_NULL` from (`top` `a` join `sectorizaciones` `b`) where ((`a`.`IdSistema` = `b`.`IdSistema`) and (not((`a`.`IdSistema`,`b`.`IdSectorizacion`,`a`.`IdTOP`) in (select `sectoressectorizacion`.`IdSistema` AS `IdSistema`,`sectoressectorizacion`.`IdSectorizacion` AS `idsectorizacion`,`sectoressectorizacion`.`IdTOP` AS `IdTOP` from `sectoressectorizacion`))))) */;
/*!50001 SET character_set_client      = @saved_cs_client */;
/*!50001 SET character_set_results     = @saved_cs_results */;
/*!50001 SET collation_connection      = @saved_col_connection */;

--
-- Current Database: `new_cd40_trans`
--

USE `new_cd40_trans`;

--
-- Final view structure for view `destinosenequipoexterno`
--

/*!50001 DROP VIEW IF EXISTS `destinosenequipoexterno`*/;
/*!50001 SET @saved_cs_client          = @@character_set_client */;
/*!50001 SET @saved_cs_results         = @@character_set_results */;
/*!50001 SET @saved_col_connection     = @@collation_connection */;
/*!50001 SET character_set_client      = utf8 */;
/*!50001 SET character_set_results     = utf8 */;
/*!50001 SET collation_connection      = utf8_general_ci */;
/*!50001 CREATE ALGORITHM=UNDEFINED */
/*!50013 DEFINER=`root`@`%` SQL SECURITY DEFINER */
/*!50001 VIEW `destinosenequipoexterno` AS select `rt`.`IdDestino` AS `IdDestino`,`e`.`IpRed1` AS `IpRed1`,`e`.`IpRed2` AS `IpRed2`,`e`.`idEquipos` AS `idEquipos` from ((`recursostf` `rt` left join `recursos` `r` on((`r`.`IdRecurso` = `rt`.`IdRecurso`))) join `equiposeu` `e` on((`r`.`idEquipos` = `e`.`idEquipos`))) where (`rt`.`IdDestino` is not null) */;
/*!50001 SET character_set_client      = @saved_cs_client */;
/*!50001 SET character_set_results     = @saved_cs_results */;
/*!50001 SET collation_connection      = @saved_col_connection */;

--
-- Final view structure for view `destinosenequipoexternoparaoptions`
--

/*!50001 DROP VIEW IF EXISTS `destinosenequipoexternoparaoptions`*/;
/*!50001 SET @saved_cs_client          = @@character_set_client */;
/*!50001 SET @saved_cs_results         = @@character_set_results */;
/*!50001 SET @saved_col_connection     = @@collation_connection */;
/*!50001 SET character_set_client      = utf8 */;
/*!50001 SET character_set_results     = utf8 */;
/*!50001 SET collation_connection      = utf8_general_ci */;
/*!50001 CREATE ALGORITHM=UNDEFINED */
/*!50013 DEFINER=`root`@`localhost` SQL SECURITY DEFINER */
/*!50001 VIEW `destinosenequipoexternoparaoptions` AS select `eu`.`idEquipos` AS `idEquipos`,`eu`.`IpRed1` AS `IpRed1`,`eu`.`IpRed2` AS `IpRed2`,`eu`.`SipPort` AS `SipPort`,`eu`.`TipoEquipo` AS `TipoEquipo`,`h`.`ModeloEquipo` AS `modeloequipo`,`r`.`Tipo` AS `tiporadio`,`r`.`IdRecurso` AS `IdRecurso`,`rr`.`IdDestino` AS `IdDestino` from (((`equiposeu` `eu` left join `recursos` `r` on((`r`.`idEquipos` = `eu`.`idEquipos`))) left join `hfparams` `h` on((`h`.`IdRecurso` = `r`.`IdRecurso`))) left join `recursosradio` `rr` on((`rr`.`IdRecurso` = `r`.`IdRecurso`))) where (`eu`.`TipoEquipo` = 2) union select `eu`.`idEquipos` AS `idEquipos`,`eu`.`IpRed1` AS `IpRed1`,`eu`.`IpRed2` AS `IpRed2`,`eu`.`SipPort` AS `SipPort`,`eu`.`TipoEquipo` AS `TipoEquipo`,0 AS `modeloequipo`,0 AS `tiporadio`,`r`.`IdRecurso` AS `IdRecurso`,`rt`.`IdDestino` AS `IdDestino` from (((`equiposeu` `eu` left join `recursos` `r` on((`r`.`idEquipos` = `eu`.`idEquipos`))) left join `hfparams` `h` on((`h`.`IdRecurso` = `r`.`IdRecurso`))) left join `recursostf` `rt` on((`rt`.`IdRecurso` = `r`.`IdRecurso`))) where (`eu`.`TipoEquipo` = 3) union select `eu`.`idEquipos` AS `idEquipos`,`eu`.`IpRed1` AS `IpRed1`,`eu`.`IpRed2` AS `IpRed2`,`eu`.`SipPort` AS `SipPort`,`eu`.`TipoEquipo` AS `TipoEquipo`,0 AS `modeloequipo`,0 AS `tiporadio`,NULL AS `IdRecurso`,NULL AS `IdDestino` from `equiposeu` `eu` where (`eu`.`TipoEquipo` = 5) */;
/*!50001 SET character_set_client      = @saved_cs_client */;
/*!50001 SET character_set_results     = @saved_cs_results */;
/*!50001 SET collation_connection      = @saved_col_connection */;

--
-- Final view structure for view `sectoresentopsparainformexml`
--

/*!50001 DROP VIEW IF EXISTS `sectoresentopsparainformexml`*/;
/*!50001 SET @saved_cs_client          = @@character_set_client */;
/*!50001 SET @saved_cs_results         = @@character_set_results */;
/*!50001 SET @saved_col_connection     = @@collation_connection */;
/*!50001 SET character_set_client      = utf8 */;
/*!50001 SET character_set_results     = utf8 */;
/*!50001 SET collation_connection      = utf8_general_ci */;
/*!50001 CREATE ALGORITHM=UNDEFINED */
/*!50013 DEFINER=`root`@`localhost` SQL SECURITY DEFINER */
/*!50001 VIEW `sectoresentopsparainformexml` AS (select `a`.`IdSistema` AS `Idsistema`,`a`.`IdSectorizacion` AS `IdSectorizacion`,`a`.`IdTOP` AS `IdToP`,`a`.`IdSector` AS `IdSector`,`c`.`IdNucleo` AS `IdNucleo`,`c`.`IdSectorOriginal` AS `IdSectorOriginal` from (`sectoressectorizacion` `a` join `sectoressector` `c`) where ((`a`.`IdSistema` = `c`.`IdSistema`) and (`c`.`IdNucleo` = `a`.`IdNucleo`) and (`c`.`IdSector` = `a`.`IdSector`)) order by `c`.`IdSectorOriginal`) union (select `b`.`IdSistema` AS `IdSistema`,`b`.`IdSectorizacion` AS `IdSectorizacion`,`a`.`IdTOP` AS `IdTOP`,NULL AS `NULL`,NULL AS `My_exp_NULL`,NULL AS `My_exp_1_NULL` from (`top` `a` join `sectorizaciones` `b`) where ((`a`.`IdSistema` = `b`.`IdSistema`) and (not((`a`.`IdSistema`,`b`.`IdSectorizacion`,`a`.`IdTOP`) in (select `sectoressectorizacion`.`IdSistema` AS `IdSistema`,`sectoressectorizacion`.`IdSectorizacion` AS `idsectorizacion`,`sectoressectorizacion`.`IdTOP` AS `IdTOP` from `sectoressectorizacion`))))) */;
/*!50001 SET character_set_client      = @saved_cs_client */;
/*!50001 SET character_set_results     = @saved_cs_results */;
/*!50001 SET collation_connection      = @saved_col_connection */;

--
-- Final view structure for view `viewdestinostelefonia`
--

/*!50001 DROP VIEW IF EXISTS `viewdestinostelefonia`*/;
/*!50001 SET @saved_cs_client          = @@character_set_client */;
/*!50001 SET @saved_cs_results         = @@character_set_results */;
/*!50001 SET @saved_col_connection     = @@collation_connection */;
/*!50001 SET character_set_client      = utf8 */;
/*!50001 SET character_set_results     = utf8 */;
/*!50001 SET collation_connection      = utf8_general_ci */;
/*!50001 CREATE ALGORITHM=UNDEFINED */
/*!50013 DEFINER=`root`@`localhost` SQL SECURITY DEFINER */
/*!50001 VIEW `viewdestinostelefonia` AS (select `a`.`IdSistema` AS `IdSistema`,`a`.`IdDestino` AS `IdDestino`,`a`.`TipoDestino` AS `TipoDestino`,`a`.`IdGrupo` AS `IdGrupo`,`a`.`IdPrefijo` AS `IdPrefijo`,`b`.`IdAbonado` AS `IdAbonado`,NULL AS `IdRed` from (`destinostelefonia` `a` join `destinosexternos` `b`) where ((`a`.`IdSistema` = `b`.`IdSistema`) and (`a`.`IdDestino` = `b`.`IdDestino`) and (`a`.`TipoDestino` = `b`.`TipoDestino`) and (`a`.`IdPrefijo` in (0,1,2,32)) and (`a`.`IdPrefijo` = `b`.`IdPrefijo`)) order by `IdDestino`) union (select `a`.`IdSistema` AS `IdSistema`,`a`.`IdDestino` AS `IdDestino`,`a`.`TipoDestino` AS `TipoDestino`,`a`.`IdGrupo` AS `IdGrupo`,`a`.`IdPrefijo` AS `IdPrefijo`,`b`.`IdAbonado` AS `IdAbonado`,`c`.`IdRed` AS `IdRed` from ((`destinostelefonia` `a` join `destinosexternos` `b`) join `redes` `c`) where ((`a`.`IdSistema` = `b`.`IdSistema`) and (`a`.`IdDestino` = `b`.`IdDestino`) and (`a`.`TipoDestino` = `b`.`TipoDestino`) and (`a`.`IdPrefijo` = `b`.`IdPrefijo`) and (`c`.`IdSistema` = `b`.`IdSistema`) and (`c`.`IdPrefijo` = `b`.`IdPrefijo`)) order by `IdDestino`) */;
/*!50001 SET character_set_client      = @saved_cs_client */;
/*!50001 SET character_set_results     = @saved_cs_results */;
/*!50001 SET collation_connection      = @saved_col_connection */;

--
-- Final view structure for view `viewequiposexternos`
--

/*!50001 DROP VIEW IF EXISTS `viewequiposexternos`*/;
/*!50001 SET @saved_cs_client          = @@character_set_client */;
/*!50001 SET @saved_cs_results         = @@character_set_results */;
/*!50001 SET @saved_col_connection     = @@collation_connection */;
/*!50001 SET character_set_client      = utf8 */;
/*!50001 SET character_set_results     = utf8 */;
/*!50001 SET collation_connection      = utf8_general_ci */;
/*!50001 CREATE ALGORITHM=UNDEFINED */
/*!50013 DEFINER=`root`@`localhost` SQL SECURITY DEFINER */
/*!50001 VIEW `viewequiposexternos` AS select `eu`.`idEquipos` AS `idEquipos`,`eu`.`IpRed1` AS `IpRed1`,`eu`.`TipoEquipo` AS `tipoEquipo`,`h`.`ModeloEquipo` AS `modeloequipo`,`r`.`Tipo` AS `tiporadio` from ((`equiposeu` `eu` left join `recursos` `r` on((`r`.`idEquipos` = `eu`.`idEquipos`))) left join `hfparams` `h` on((`h`.`IdRecurso` = `r`.`IdRecurso`))) */;
/*!50001 SET character_set_client      = @saved_cs_client */;
/*!50001 SET character_set_results     = @saved_cs_results */;
/*!50001 SET collation_connection      = @saved_col_connection */;

--
-- Final view structure for view `viewincidenciasmasalarma`
--

/*!50001 DROP VIEW IF EXISTS `viewincidenciasmasalarma`*/;
/*!50001 SET @saved_cs_client          = @@character_set_client */;
/*!50001 SET @saved_cs_results         = @@character_set_results */;
/*!50001 SET @saved_col_connection     = @@collation_connection */;
/*!50001 SET character_set_client      = utf8 */;
/*!50001 SET character_set_results     = utf8 */;
/*!50001 SET collation_connection      = utf8_general_ci */;
/*!50001 CREATE ALGORITHM=UNDEFINED */
/*!50013 DEFINER=`root`@`localhost` SQL SECURITY DEFINER */
/*!50001 VIEW `viewincidenciasmasalarma` AS select `s`.`IdSistema` AS `IdSistema`,`i`.`IdIncidencia` AS `IdIncidencia`,`i`.`Incidencia` AS `Incidencia`,(select `alarmas`.`Alarma` AS `Alarma` from `alarmas` where ((`alarmas`.`IdSistema` = `s`.`IdSistema`) and (`alarmas`.`IdIncidencia` = `i`.`IdIncidencia`))) AS `alarma` from (`incidencias` `i` join `sistema` `s`) order by `s`.`IdSistema`,`i`.`Incidencia` */;
/*!50001 SET character_set_client      = @saved_cs_client */;
/*!50001 SET character_set_results     = @saved_cs_results */;
/*!50001 SET collation_connection      = @saved_col_connection */;

--
-- Final view structure for view `viewincidenciasmasalarma_ingles`
--

/*!50001 DROP VIEW IF EXISTS `viewincidenciasmasalarma_ingles`*/;
/*!50001 SET @saved_cs_client          = @@character_set_client */;
/*!50001 SET @saved_cs_results         = @@character_set_results */;
/*!50001 SET @saved_col_connection     = @@collation_connection */;
/*!50001 SET character_set_client      = utf8 */;
/*!50001 SET character_set_results     = utf8 */;
/*!50001 SET collation_connection      = utf8_general_ci */;
/*!50001 CREATE ALGORITHM=UNDEFINED */
/*!50013 DEFINER=`root`@`localhost` SQL SECURITY DEFINER */
/*!50001 VIEW `viewincidenciasmasalarma_ingles` AS select `s`.`IdSistema` AS `IdSistema`,`i`.`IdIncidencia` AS `IdIncidencia`,`i`.`Incidencia` AS `Incidencia`,(select `alarmas`.`Alarma` AS `Alarma` from `alarmas` where ((`alarmas`.`IdSistema` = `s`.`IdSistema`) and (`alarmas`.`IdIncidencia` = `i`.`IdIncidencia`))) AS `alarma` from (`incidencias_ingles` `i` join `sistema` `s`) order by `s`.`IdSistema`,`i`.`Incidencia` */;
/*!50001 SET character_set_client      = @saved_cs_client */;
/*!50001 SET character_set_results     = @saved_cs_results */;
/*!50001 SET collation_connection      = @saved_col_connection */;

--
-- Final view structure for view `viewrecursosimplicadosrutas`
--

/*!50001 DROP VIEW IF EXISTS `viewrecursosimplicadosrutas`*/;
/*!50001 SET @saved_cs_client          = @@character_set_client */;
/*!50001 SET @saved_cs_results         = @@character_set_results */;
/*!50001 SET @saved_col_connection     = @@collation_connection */;
/*!50001 SET character_set_client      = utf8 */;
/*!50001 SET character_set_results     = utf8 */;
/*!50001 SET collation_connection      = utf8_general_ci */;
/*!50001 CREATE ALGORITHM=UNDEFINED */
/*!50013 DEFINER=`root`@`localhost` SQL SECURITY DEFINER */
/*!50001 VIEW `viewrecursosimplicadosrutas` AS (select `rt`.`IdSistema` AS `idsistema`,`rt`.`IdRecurso` AS `idrecurso`,`rt`.`IdDestino` AS `iddestino`,`rt`.`IdRed` AS `idred`,`rt`.`IdTroncal` AS `idtroncal`,`r`.`IdRuta` AS `idruta`,`r`.`Tipo` AS `tipo` from ((`recursostf` `rt` join `troncalesruta` `tr`) join `rutas` `r`) where ((`tr`.`IdSistema` = `rt`.`IdSistema`) and (`r`.`IdSistema` = `tr`.`IdSistema`) and (`rt`.`IdTroncal` is not null) and (`rt`.`IdTroncal` = `tr`.`IdTroncal`) and (`tr`.`IdRuta` = `r`.`IdRuta`)) order by `rt`.`IdSistema`,`rt`.`IdRecurso`) union (select `rt`.`IdSistema` AS `idsistema`,`rt`.`IdRecurso` AS `idrecurso`,`rt`.`IdDestino` AS `IdDestino`,`rt`.`IdRed` AS `idred`,NULL AS `idtroncal`,NULL AS `idruta`,NULL AS `tipo` from `recursostf` `rt` where isnull(`rt`.`IdTroncal`) order by `rt`.`IdSistema`,`rt`.`IdRecurso`) */;
/*!50001 SET character_set_client      = @saved_cs_client */;
/*!50001 SET character_set_results     = @saved_cs_results */;
/*!50001 SET collation_connection      = @saved_col_connection */;

--
-- Final view structure for view `viewsectoresenagrupacion`
--

/*!50001 DROP VIEW IF EXISTS `viewsectoresenagrupacion`*/;
/*!50001 SET @saved_cs_client          = @@character_set_client */;
/*!50001 SET @saved_cs_results         = @@character_set_results */;
/*!50001 SET @saved_col_connection     = @@collation_connection */;
/*!50001 SET character_set_client      = utf8 */;
/*!50001 SET character_set_results     = utf8 */;
/*!50001 SET collation_connection      = utf8_general_ci */;
/*!50001 CREATE ALGORITHM=UNDEFINED */
/*!50013 DEFINER=`root`@`localhost` SQL SECURITY DEFINER */
/*!50001 VIEW `viewsectoresenagrupacion` AS select count(0) AS `contador`,`sectoresagrupacion`.`IdAgrupacion` AS `IdAgrupacion` from `sectoresagrupacion` group by `sectoresagrupacion`.`IdAgrupacion` */;
/*!50001 SET character_set_client      = @saved_cs_client */;
/*!50001 SET character_set_results     = @saved_cs_results */;
/*!50001 SET collation_connection      = @saved_col_connection */;

--
-- Final view structure for view `viewsectoresentops`
--

/*!50001 DROP VIEW IF EXISTS `viewsectoresentops`*/;
/*!50001 SET @saved_cs_client          = @@character_set_client */;
/*!50001 SET @saved_cs_results         = @@character_set_results */;
/*!50001 SET @saved_col_connection     = @@collation_connection */;
/*!50001 SET character_set_client      = utf8 */;
/*!50001 SET character_set_results     = utf8 */;
/*!50001 SET collation_connection      = utf8_general_ci */;
/*!50001 CREATE ALGORITHM=UNDEFINED */
/*!50013 DEFINER=`root`@`localhost` SQL SECURITY DEFINER */
/*!50001 VIEW `viewsectoresentops` AS (select `sectoressectorizacion`.`IdSistema` AS `IdSistema`,`sectoressectorizacion`.`IdSectorizacion` AS `idsectorizacion`,`sectoressectorizacion`.`IdTOP` AS `IdTOP`,`sectoressectorizacion`.`IdSector` AS `idsector`,`sectoressectorizacion`.`IdNucleo` AS `idnucleo`,`s`.`Tipo` AS `tipo` from (`sectoressectorizacion` join `sectores` `s`) where ((`s`.`IdSistema` = `sectoressectorizacion`.`IdSistema`) and (`s`.`IdNucleo` = `sectoressectorizacion`.`IdNucleo`) and (`s`.`IdSector` = `sectoressectorizacion`.`IdSector`) and (`sectoressectorizacion`.`IdSistema`,`sectoressectorizacion`.`IdSectorizacion`,`sectoressectorizacion`.`IdTOP`) in (select `a`.`IdSistema` AS `IdSistema`,`b`.`IdSectorizacion` AS `idsectorizacion`,`a`.`IdTOP` AS `IdTOP` from (`top` `a` join `sectorizaciones` `b`) where (`a`.`IdSistema` = `b`.`IdSistema`)))) union (select `a`.`IdSistema` AS `IdSistema`,`b`.`IdSectorizacion` AS `idsectorizacion`,`a`.`IdTOP` AS `IdTOP`,NULL AS `NULL`,NULL AS `My_exp_NULL`,NULL AS `My_exp_1_NULL` from (`top` `a` join `sectorizaciones` `b`) where ((`a`.`IdSistema` = `b`.`IdSistema`) and (not((`a`.`IdSistema`,`b`.`IdSectorizacion`,`a`.`IdTOP`) in (select `sectoressectorizacion`.`IdSistema` AS `IdSistema`,`sectoressectorizacion`.`IdSectorizacion` AS `idsectorizacion`,`sectoressectorizacion`.`IdTOP` AS `IdTOP` from `sectoressectorizacion`))))) */;
/*!50001 SET character_set_client      = @saved_cs_client */;
/*!50001 SET character_set_results     = @saved_cs_results */;
/*!50001 SET collation_connection      = @saved_col_connection */;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2021-05-12 11:06:50
