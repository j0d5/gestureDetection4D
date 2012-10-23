-- MySQL dump 10.13  Distrib 5.5.17, for Win32 (x86)
--
-- Host: localhost    Database: kinect
-- ------------------------------------------------------
-- Server version	5.5.17

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
-- Table structure for table `gesture`
--

DROP TABLE IF EXISTS `gesture`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `gesture` (
  `idGesture` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(45) COLLATE latin1_german2_ci NOT NULL,
  PRIMARY KEY (`idGesture`),
  UNIQUE KEY `idGesture_UNIQUE` (`idGesture`),
  UNIQUE KEY `name_UNIQUE` (`name`)
) ENGINE=InnoDB AUTO_INCREMENT=33 DEFAULT CHARSET=latin1 COLLATE=latin1_german2_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `gesture`
--

LOCK TABLES `gesture` WRITE;
/*!40000 ALTER TABLE `gesture` DISABLE KEYS */;
INSERT INTO `gesture` VALUES (30,'eight'),(27,'five'),(26,'four'),(31,'nine'),(23,'one'),(22,'POINT'),(29,'seven'),(28,'six'),(32,'ten'),(25,'three'),(24,'two'),(21,'WAVE');
/*!40000 ALTER TABLE `gesture` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `gesture2oni`
--

DROP TABLE IF EXISTS `gesture2oni`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `gesture2oni` (
  `idOniData` int(11) NOT NULL AUTO_INCREMENT,
  `gesture_idGesture` int(11) NOT NULL,
  `oniFile_idOniFile` int(11) NOT NULL,
  `startPoint_X` float DEFAULT NULL,
  `startPoint_Y` float DEFAULT NULL,
  `startPoint_Z` float DEFAULT NULL,
  PRIMARY KEY (`idOniData`,`gesture_idGesture`,`oniFile_idOniFile`),
  KEY `fk_gesture2Oni_gesture` (`gesture_idGesture`),
  KEY `fk_gesture2Oni_oniFile1` (`oniFile_idOniFile`),
  CONSTRAINT `fk_gesture2Oni_gesture` FOREIGN KEY (`gesture_idGesture`) REFERENCES `gesture` (`idGesture`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `fk_gesture2Oni_oniFile1` FOREIGN KEY (`oniFile_idOniFile`) REFERENCES `onifile` (`idOniFile`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB AUTO_INCREMENT=229 DEFAULT CHARSET=latin1 COLLATE=latin1_german2_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `gesture2oni`
--

LOCK TABLES `gesture2oni` WRITE;
/*!40000 ALTER TABLE `gesture2oni` DISABLE KEYS */;
/*!40000 ALTER TABLE `gesture2oni` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `onifile`
--

DROP TABLE IF EXISTS `onifile`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `onifile` (
  `idOniFile` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(45) COLLATE latin1_german2_ci DEFAULT NULL,
  `size` int(11) DEFAULT NULL,
  `filePath` varchar(150) COLLATE latin1_german2_ci DEFAULT NULL,
  PRIMARY KEY (`idOniFile`),
  UNIQUE KEY `name_UNIQUE` (`name`)
) ENGINE=InnoDB AUTO_INCREMENT=260 DEFAULT CHARSET=latin1 COLLATE=latin1_german2_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `onifile`
--

LOCK TABLES `onifile` WRITE;
/*!40000 ALTER TABLE `onifile` DISABLE KEYS */;
/*!40000 ALTER TABLE `onifile` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2012-01-05 15:30:08
