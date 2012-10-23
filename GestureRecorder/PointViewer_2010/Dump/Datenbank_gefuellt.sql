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
) ENGINE=InnoDB AUTO_INCREMENT=253 DEFAULT CHARSET=latin1 COLLATE=latin1_german2_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `gesture2oni`
--

LOCK TABLES `gesture2oni` WRITE;
/*!40000 ALTER TABLE `gesture2oni` DISABLE KEYS */;
INSERT INTO `gesture2oni` VALUES (231,23,262,172.297,-118.577,1190.86),(232,23,263,162.835,-99.7845,1192.62),(233,24,264,129.111,-161.458,971.768),(234,24,265,159.978,-112.443,1013.61),(235,25,266,118.292,-96.4407,940.981),(236,25,267,110.9,-117.192,933.165),(237,26,268,96.376,-87.1338,998.488),(238,26,269,113.415,-54.2546,991.948),(241,27,272,94.131,-81.4622,1142.98),(242,27,273,158.5,-102.745,1150.38),(243,28,274,152.603,-154.495,1136.99),(244,28,275,175.802,-127.193,1118.55),(245,29,276,129.097,-125.667,1075.94),(246,29,277,123.323,-92.0273,1065.84),(247,30,278,127.727,-162.682,1052.73),(248,30,279,111.413,-136.534,1052.35),(249,31,280,164.963,-169.489,1092.76),(250,31,281,159.729,-161.381,1095.9),(251,32,282,67.4729,-75.5339,1175.23),(252,32,283,57.7926,-92.2402,1180.8);
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
) ENGINE=InnoDB AUTO_INCREMENT=284 DEFAULT CHARSET=latin1 COLLATE=latin1_german2_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `onifile`
--

LOCK TABLES `onifile` WRITE;
/*!40000 ALTER TABLE `onifile` DISABLE KEYS */;
INSERT INTO `onifile` VALUES (262,'20120105-154818.oni',7108118,'./OniFiles/20120105-154818.oni'),(263,'20120105-154828.oni',8924040,'./OniFiles/20120105-154828.oni'),(264,'20120105-155225.oni',6366967,'./OniFiles/20120105-155225.oni'),(265,'20120105-155230.oni',5905431,'./OniFiles/20120105-155230.oni'),(266,'20120105-155248.oni',5257803,'./OniFiles/20120105-155248.oni'),(267,'20120105-155252.oni',6679781,'./OniFiles/20120105-155252.oni'),(268,'20120105-155303.oni',5971542,'./OniFiles/20120105-155303.oni'),(269,'20120105-155309.oni',7518041,'./OniFiles/20120105-155309.oni'),(272,'20120105-155757.oni',6367993,'./OniFiles/20120105-155757.oni'),(273,'20120105-155807.oni',6142754,'./OniFiles/20120105-155807.oni'),(274,'20120105-155820.oni',14295760,'./OniFiles/20120105-155820.oni'),(275,'20120105-155839.oni',5501337,'./OniFiles/20120105-155839.oni'),(276,'20120105-155853.oni',7440595,'./OniFiles/20120105-155853.oni'),(277,'20120105-155857.oni',3614158,'./OniFiles/20120105-155857.oni'),(278,'20120105-155908.oni',5637633,'./OniFiles/20120105-155908.oni'),(279,'20120105-155914.oni',6016291,'./OniFiles/20120105-155914.oni'),(280,'20120105-155926.oni',6613620,'./OniFiles/20120105-155926.oni'),(281,'20120105-155931.oni',6006673,'./OniFiles/20120105-155931.oni'),(282,'20120105-160115.oni',5922375,'./OniFiles/20120105-160115.oni'),(283,'20120105-160122.oni',4116281,'./OniFiles/20120105-160122.oni');
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

-- Dump completed on 2012-01-05 16:02:25
