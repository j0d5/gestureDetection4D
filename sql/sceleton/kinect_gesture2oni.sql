CREATE DATABASE  IF NOT EXISTS `kinect` /*!40100 DEFAULT CHARACTER SET latin1 */;
USE `kinect`;
-- MySQL dump 10.13  Distrib 5.5.16, for Win32 (x86)
--
-- Host: 127.0.0.1    Database: kinect
-- ------------------------------------------------------
-- Server version	5.5.28

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
  KEY `fk_gesture2Oni_oniFile1` (`oniFile_idOniFile`)
) ENGINE=InnoDB AUTO_INCREMENT=235 DEFAULT CHARSET=latin1 COLLATE=latin1_german2_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `gesture2oni`
--

LOCK TABLES `gesture2oni` WRITE;
/*!40000 ALTER TABLE `gesture2oni` DISABLE KEYS */;
/*!40000 ALTER TABLE `gesture2oni` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2012-11-13 17:27:32
