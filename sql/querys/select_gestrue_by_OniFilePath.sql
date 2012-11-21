SELECT name FROM gesture WHERE idGesture = (
SELECT gesture_idGesture FROM gesture2oni 
WHERE oniFile_idOniFile = (SELECT idOniFile FROM onifile WHERE name = '20121121-120457.oni' ));