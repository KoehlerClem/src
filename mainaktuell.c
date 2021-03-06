#include "../h/main.h"

void sensor();
short pointExists(short coordToCheck[2]);
short goHome();
void createPoint(short coord[2]);
void visitPoint();

/* Hier wird der Struct für einen Point definiert. Der Point hat x,y Coordinaten
 * in coord gespeichert, einen short Wert für die Kanten und einen bool-Wert des
 * aussagt ob der Punkt schon besucht wurde! jajaja
 */
typedef struct points {
	short coord[2];
	short arrayEdges[4];
	short pathToPointExists;
	short edgeToPointBefore;
	short visited; // 1 = true, 0 = false
} point;

point allPoints[50]; // Das Array in dem Alle Punkte gespeichert werden
short currentArrayPoint = 1; // Der Punkt an dem der letzte Punkt in dem Array erstellt wurde
short currentCoord[2] = { 0, 0 };
short currentPoint = 0; // Die aktuelle Coordinate des aktuellen Punktes
short neighbourPoint[4];

/// DO NOT DELETE THIS METHOD
/// It is called every 1ms and e.g. can be used for implementing a
/// real time counter / clock.
void user_1ms_isr_type2(void) {
}

void ecrobot_device_initialize(void) {
}

void ecrobot_device_terminate(void) {
}

void hello_world() {
	ecrobot_status_monitor("");

}
short tokenfound = 0;
int a, v, l, r;
short now = 0;
short kanten[4] = { 0, 0, 0, 0 };
void move(int v) {
	nxt_motor_set_speed(NXT_PORT_B, v, 0);
	nxt_motor_set_speed(NXT_PORT_A, v, 0);
}

void left() {
	ecrobot_status_monitor("Function: left");
	while (ecrobot_get_light_sensor(NXT_PORT_S2) <= a) {
		nxt_motor_set_speed(NXT_PORT_A, -75, 0);
		nxt_motor_set_speed(NXT_PORT_B, 75, 0);
	}
}

void right() {
	ecrobot_status_monitor("Function: right");
	while (ecrobot_get_light_sensor(NXT_PORT_S2) <= a) {
		nxt_motor_set_speed(NXT_PORT_A, 75, 0);
		nxt_motor_set_speed(NXT_PORT_B, -75, 0);
	}
}

void left2(int g) {
	ecrobot_status_monitor("Function: left2");
	nxt_motor_set_count(NXT_PORT_B, 0);
	while ((ecrobot_get_light_sensor(NXT_PORT_S2) <= a)
			&& (nxt_motor_get_count(NXT_PORT_B) <= g)) {
		nxt_motor_set_speed(NXT_PORT_A, -75, 0);
		nxt_motor_set_speed(NXT_PORT_B, 75, 0);
	}

}

void right2(int g) {
	ecrobot_status_monitor("Function: right2");
	nxt_motor_set_count(NXT_PORT_A, 0);
	while ((ecrobot_get_light_sensor(NXT_PORT_S2) <= a)
			&& (nxt_motor_get_count(NXT_PORT_A) <= g)) {
		nxt_motor_set_speed(NXT_PORT_A, 75, 0);
		nxt_motor_set_speed(NXT_PORT_B, -75, 0);
	}
}

void left3(int g) {
	ecrobot_status_monitor("Function: left3");
	nxt_motor_set_count(NXT_PORT_B, 0);
	while (nxt_motor_get_count(NXT_PORT_B) <= g) {
		nxt_motor_set_speed(NXT_PORT_A, -75, 0);
		nxt_motor_set_speed(NXT_PORT_B, 75, 0);
	}
}

void right3(int g) {
	ecrobot_status_monitor("Function: right3");
	nxt_motor_set_count(NXT_PORT_A, 0);
	while (nxt_motor_get_count(NXT_PORT_A) <= g) {
		nxt_motor_set_speed(NXT_PORT_A, 75, 0);
		nxt_motor_set_speed(NXT_PORT_B, -75, 0);
	}
}

void stopp() {
	ecrobot_status_monitor("Function: stop");
	nxt_motor_set_speed(NXT_PORT_B, 0, 0);
	nxt_motor_set_speed(NXT_PORT_A, 0, 0);
}

void token() {
	ecrobot_status_monitor("Function: token");
	tokenfound++;
	ecrobot_sound_tone(220, 1000, 50);
	nxt_motor_set_count(NXT_PORT_A, 0);
	while (nxt_motor_get_count(NXT_PORT_A) >= -100) {
		move(-75);
	}
	stopp();
	systick_wait_ms(10000);
}

void scan() {
	ecrobot_status_monitor("Function: scan");
	kanten[(now + 2) % 4] = 1; // grade auf der Linie
	nxt_motor_set_count(NXT_PORT_A, 0);

	while (nxt_motor_get_count(NXT_PORT_A) <= 180) {    //bewegt sich nach vorn
		move(75);
	}
	// Ab hier schaut er ob es eine Linie nach vorne
	if (ecrobot_get_light_sensor(NXT_PORT_S2) <= a) {    //Korekturfunktion fals er noch nicht auf der Linie steht nach dem befahren der Kreuzung
		left2(70);
		right2(140);
	}
	if (ecrobot_get_light_sensor(NXT_PORT_S2) <= a) {    //Linie nicht gefunden
		kanten[now] = 0;
		ecrobot_sound_tone(1000, 500, 30);
		v = 0;
		left3(100);
	} else {			//Linie gefunden
		kanten[now] = 1;
		ecrobot_sound_tone(100, 500, 30);
		v = 1;
	}
	//gibt es links eine Linie
	left3(50);
	left2(300);
	if (ecrobot_get_light_sensor(NXT_PORT_S2) <= a) {     //Linie nicht gefunden
		kanten[(now + 3) % 4] = 0;
		ecrobot_sound_tone(1000, 500, 30);
		l = 0;
	} else {
		kanten[(now + 3) % 4] = 1;
		ecrobot_sound_tone(100, 500, 30);
		l = 1;//sonst null
	}
	//rechts
	left3(50);
	left2(250);
	left3(50);
	left2(350);
	if (ecrobot_get_light_sensor(NXT_PORT_S2) <= a) {
		kanten[(now + 1) % 4] = 0;
		ecrobot_sound_tone(1000, 500, 30);
		r = 0;
	} else {
		kanten[(now + 1) % 4] = 1;
		ecrobot_sound_tone(100, 500, 30);
		r = 1;
	}
	if(v == 1 && l == 1 && r == 1){ ecrobot_status_monitor("GE LI RE");}
	if(v == 1 && l == 1 && r == 0){ ecrobot_status_monitor("GE LI");}
	if(v == 1 && l == 0 && r == 1){ ecrobot_status_monitor("GE RE");}
	if(v == 0 && l == 1 && r == 1){ ecrobot_status_monitor("LI RE");}
	if(v == 1 && l == 0 && r == 0){ ecrobot_status_monitor("GERADE");}
	if(v == 0 && l == 1 && r == 0){ ecrobot_status_monitor("LINKS");}
	if(v == 0 && l == 0 && r == 1){ ecrobot_status_monitor("RECHTS");}
}

void ausrichten() {
	ecrobot_status_monitor("Function: ausrichten");
	nxt_motor_set_count(NXT_PORT_A, 0);
	while (nxt_motor_get_count(NXT_PORT_A) <= 160) {
		move(75);
	}    //bewegt sich nach vor
	if (ecrobot_get_light_sensor(NXT_PORT_S2) <= a) {    //Wenn eine Linie in Fahrtrichtung ist findet er sie und stell sich auf sie
		left2(70);
		right2(140);
	}
	if (allPoints[currentPoint].arrayEdges[(now + 5) % 4] == 1) { // wenn sich recht vom Roboter eine Kante befindet soll er sie auf sie drehen
		right3(50);
		right2(250);
	} else {		// Wenn nicht dreht er sich um 150 nach rechts
		right3(150);
	}
}

void drehung(short turn) {
	ecrobot_status_monitor("Function: drehung");

	short richtung; // Die Richtung für den Roboter berechnet aus der Ausgangsrichtung und dem Bewegungsbefehl in Himmelsrichtung
	richtung = (turn - now + 4) % 4;
	switch (richtung) {

	case 0:    //geradeaus
		left3(50);
		left();
		//now gleich
		break;

	case 1: //rechts
		now = (now + 1) % 4;
		break;

	case 2:    //zurück
		right3(50);
		right();
		now = (now + 2) % 4;
		break;

	case 3: //links
		right3(50);
		right2(400);
		right3(100);
		right2(300);
		now = (now + 3) % 4;
		break;

	default:
		ecrobot_sound_tone(1000, 400, 50);
		ecrobot_sound_tone(100, 400, 50);
		ecrobot_sound_tone(1000, 400, 50);
	}
	sensor();
}

void sensor() {
	ecrobot_status_monitor("Function: sensor");
	short foundIntersection = 0;
	while (foundIntersection == 0) {
		if (ecrobot_get_touch_sensor(NXT_PORT_S3) == 1
				|| ecrobot_get_touch_sensor(NXT_PORT_S4) == 1) {
			token();
		}
		move(75);
		if (ecrobot_get_light_sensor(NXT_PORT_S2) <= a) {
			left2(70);
			right2(140);
		}
		if (ecrobot_get_light_sensor(NXT_PORT_S2) <= a) {
			foundIntersection = 1;
			left2(70);
			visitPoint();    //scan();
		}
		ecrobot_status_monitor("Function: sensor");
	}

}

// ---------------- Steak Area -----------------

/* Vergleicht zuerst die X-Coordinate des jeweils betrachteten Punktes gegen die übergebene X-Coordinate
 * und wenn wenn diese Übereistimmt vergleicht die Funktion auch die Y-Coordinate. Wenn diese auch gleich ist
 * wird true zurückgegeben.
 */
short pointExists(short coordToCheck[2]) {
	ecrobot_status_monitor("Function: pointExists");
	short i;
	for (i = 0; i < currentArrayPoint; i++) {
		if (coordToCheck[0] == allPoints[i].coord[0]) {
			if (coordToCheck[1] == allPoints[i].coord[1]) {
				return 1;
			}
		}
	}
	return 0;
}

void createPoint(short coord[2]) {
	ecrobot_status_monitor("Function: createPoint");
	if (pointExists(coord) == 0) {
		allPoints[currentArrayPoint].coord[0] = coord[0];
		allPoints[currentArrayPoint].coord[1] = coord[1];
		currentArrayPoint += 1;
	}
}

/* Es wird zuerst überprüft ob an den gegebenen Koordinaten schon ein Punk existiert.
 * Fals dies nicht der Fall ist wird ein neuer Punkt an den Koordinaten erstellt und
 * der Wert des current Arrays, also des derzeit vereachteten Punkts zurückgegeben.
 */
short getArrayAddressFromCoord(short coordToCheck[2]) {
	ecrobot_status_monitor("Function: getArrayAddressFromCoord");
	short i;
	for (i = 0; i < currentArrayPoint; i++) {
		if (coordToCheck[0] == allPoints[i].coord[0]) {
			if (coordToCheck[1] == allPoints[i].coord[1]) {
				return i;
			}
		}
	}
	return 0;
}

void updatePoints() {
	ecrobot_status_monitor("Function: updatePoints");
	short coordNorth[2] = { currentCoord[0], currentCoord[1] + 1 };
	short coordEast[2] = { currentCoord[0] + 1, currentCoord[1] };
	short coordSouth[2] = { currentCoord[0], currentCoord[1] - 1 };
	short coordWest[2] = { currentCoord[0] - 1, currentCoord[1] };

	if(allPoints[currentPoint].arrayEdges[0] == 1){
		createPoint(coordNorth);
		allPoints[getArrayAddressFromCoord(coordNorth)].arrayEdges[2] = kanten[0];
	}
	if(allPoints[currentPoint].arrayEdges[1] == 1){
		createPoint(coordEast);
			allPoints[getArrayAddressFromCoord(coordEast)].arrayEdges[3] = kanten[1];
	}
	if(allPoints[currentPoint].arrayEdges[2] == 1){
		createPoint(coordSouth);
		allPoints[getArrayAddressFromCoord(coordSouth)].arrayEdges[0] = kanten[2];
	}
	if(allPoints[currentPoint].arrayEdges[3] == 1){
		createPoint(coordWest);
		allPoints[getArrayAddressFromCoord(coordWest)].arrayEdges[1] = kanten[3];
	}
}

/* Wenn ein Punkt besucht wird, werden die Kanten des aktuellen Punktes gespeichert,
 * er als Besucht markiert und die 4 Umliegenden Punkte erstellt, fals es sie noch nicht gibt!
 */
void setNeighbourPoints(short forThisPoint) {
	ecrobot_status_monitor("Function: setNeighbourPoints");
	short coords[2] = { allPoints[forThisPoint].coord[0],
			allPoints[forThisPoint].coord[1] };
	short northPoint[2] = { coords[0], coords[1] + 1 };
	short eastPoint[2] = { coords[0] + 1, coords[1] };
	short southPoint[2] = { coords[0], coords[1] - 1 };
	short westPoint[2] = { coords[0] - 1, coords[1] };
	neighbourPoint[0] = getArrayAddressFromCoord(northPoint);
	neighbourPoint[1] = getArrayAddressFromCoord(eastPoint);
	neighbourPoint[2] = getArrayAddressFromCoord(southPoint);
	neighbourPoint[3] = getArrayAddressFromCoord(westPoint);
}

void visitPoint() {
	ecrobot_status_monitor("Function: visitPoint");
//Holt die Adresse in dem Array zu den Aktuellen Coordinaten
	currentPoint = getArrayAddressFromCoord(currentCoord);
	if (allPoints[currentPoint].visited == 0) {
		scan();
		allPoints[currentPoint].arrayEdges[0] = kanten[0];
		allPoints[currentPoint].arrayEdges[1] = kanten[1];
		allPoints[currentPoint].arrayEdges[2] = kanten[2];
		allPoints[currentPoint].arrayEdges[3] = kanten[3];
		allPoints[currentPoint].visited = 1;
		updatePoints();
	}
	else{ ausrichten();}
}

/* Setzt den Punkt auf 0,0, empfohlen für den Start der Simulation
 */

/* Checkt ob eine bewegung nach Oben möglich ist. Wenn sie möglich ist
 * wird die CurrentCoord auf die neue Coordinate gesetzt und die Funktion
 * visitPoint() aufgerufen.
 */

/* Bewegt den Roboter einen Punkt nach oben bzw erhöht den y-Wert der
 * Coordinate um 1 wenn möglich
 */
void moveNorth() {
	ecrobot_status_monitor("Function: moveNorth");
	currentCoord[1] = currentCoord[1] + 1;
// bewege auch den roboter dahin
	drehung(0);
}

/* Bewegt den Roboter einen Punkt nach rechts bzw erhöht den x-Wert der
 * Coordinate um 1 wenn möglich
 */
void moveEast() {
	ecrobot_status_monitor("Function: moveEast");
	currentCoord[0] = currentCoord[0] + 1;
	drehung(1);
}

/* Bewegt den Roboter einen Punkt nach unten bzw verringert den y-Wert der
 * Coordinate um 1 wenn möglich
 */
void moveSouth() {
	ecrobot_status_monitor("Function: moveSouth");
	currentCoord[1] = currentCoord[1] - 1;
	drehung(2);
}

/* Bewegt den Roboter einen Punkt nach links bzw verringert den x-Wert der
 * Coordinate um 1 wenn möglich
 */
void moveWest() {
	ecrobot_status_monitor("Function: moveWest");
	currentCoord[0] = currentCoord[0] - 1;
	drehung(3);
}

// ---------------- Bear Heaven -----------------

void useNewPath(short pointToVisit) {
	ecrobot_status_monitor("Function: useNewPath");
	short coords[2];

	switch (allPoints[pointToVisit].edgeToPointBefore) {
	case 0:
		coords[0] = allPoints[pointToVisit].coord[0];
		coords[1] = allPoints[pointToVisit].coord[1] - 1;
		useNewPath(getArrayAddressFromCoord(coords));
		moveNorth();
		break;
	case 1:
		coords[0] = allPoints[pointToVisit].coord[0] - 1;
		coords[1] = allPoints[pointToVisit].coord[1];
		useNewPath(getArrayAddressFromCoord(coords));
		moveEast();
		break;
	case 2:
		coords[0] = allPoints[pointToVisit].coord[0];
		coords[1] = allPoints[pointToVisit].coord[1] + 1;
		useNewPath(getArrayAddressFromCoord(coords));
		moveSouth();
		break;
	case 3:
		coords[0] = allPoints[pointToVisit].coord[0] + 1;
		coords[1] = allPoints[pointToVisit].coord[1];
		useNewPath(getArrayAddressFromCoord(coords));
		moveWest();
		break;
	case -1:
		break;
	}
}

short getNextPoint() {
	ecrobot_status_monitor("Function: getNextPoint");
	short i; //Läuft in der for-schleife von 0-3 für alle Richtungen
	short nextPointToVisit = 0; //Der nächste Punkt der noch nicht besucht ist
	short buffer1[10] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 }; // Hier sind die Punkte gespeicher die geprüft werden
	short buffer2[10] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 }; // Hier sind die nächsten Punkte die untersucht werden sollen
	short indexBuffer1 = 0; // Laufvariable die den ersten Buffer durchläuft
	short indexBuffer2; // Laufvariable für den 2. Buffer
	short checkThisPoint; //Der Punkt dessen Nachbarn als nächstes überprüft werden sollen

	for (i = 0; i < 51; i++) { // Setzt für alle Punkte zurück, dass es einen Pfad zu ihnen gibt
		allPoints[i].pathToPointExists = 0;
	}
	for (i = 0; i < 51; i++) { // Setzt für alle Punkte zurück, dass es einen Pfad zu ihnen gibt
		allPoints[i].edgeToPointBefore = -1;
	}
//allPoints[currentPoint].edgeToPointBefore = -1;
	allPoints[currentPoint].pathToPointExists = 1;
	setNeighbourPoints(currentPoint); // Sucht die Nachbarpunkte des aktuellen Punktes
	for (i = 0; i < 4; i++) { //Füllt Buffer1 für den ersten Durchlauf mit den Nachbarpunkten auf die eine Kante (i) zu dem Punkt haben
		if (allPoints[currentPoint].arrayEdges[i] == 1) {
			buffer1[indexBuffer1] = neighbourPoint[i];
			allPoints[neighbourPoint[i]].edgeToPointBefore = i; // ist der Weg zu dem Punkt die Kante i von dem jetzigem Punkt aus
			allPoints[neighbourPoint[i]].pathToPointExists = 1;
			if (allPoints[buffer1[indexBuffer1]].visited == 0) {
				return nextPointToVisit = buffer1[indexBuffer1];
			}
			indexBuffer1++;
		}
	}
	while (nextPointToVisit == 0) { // => Solange kein Punkt gefunden ist der noch nicht besucht worden ist
		ecrobot_status_monitor("Function: getNextPoint");
		indexBuffer1 = 0; // Setzt den Indexbuffer auf den ersten Punkt im Buffer1-Array
		indexBuffer2 = 0; // Der nächste neue zu überprüfende Punkt wird im Buffer2 an die Position 0 geschrieben

		while (buffer1[indexBuffer1] != -1) { // Überprüft ob es noch einen Punkt im Buffer1-Array gibt
			ecrobot_status_monitor("Function: getNextPoint");
			checkThisPoint = buffer1[indexBuffer1]; // Ließt aus dem Array die Adresse des zu überprüfenden Punkted aus
			if (allPoints[checkThisPoint].visited == 0) { // Checkt ob der momentan betrachtete Punkt schon besucht worden ist
				return nextPointToVisit = checkThisPoint; // Fals ja, setzt den nächst zu besuchenden Point gleich dem momentan betrachteten Punkt
			}
			setNeighbourPoints(checkThisPoint); // Füllt das Array neightbourPoint mit den Nachbarpunkten des momentan betrachteten Punktes auf
			for (i = 0; i < 4; i++) {
				if (allPoints[checkThisPoint].arrayEdges[i] == 1
						&& allPoints[neighbourPoint[i]].pathToPointExists
								== 0) { // Wenn es von dem momentan betrachtetem Punkt eine Kante in Richtung i gibt
					allPoints[neighbourPoint[i]].edgeToPointBefore = i; // ist der Weg zu dem Punkt die Kante i von dem jetzigem Punkt aus
					allPoints[neighbourPoint[i]].pathToPointExists = 1;
					if (allPoints[neighbourPoint[i]].visited == 0) { // und es zu dem Punkt in Richtung i keinen Weg gibt
						return nextPointToVisit = neighbourPoint[i];
					}
					buffer2[indexBuffer2] = neighbourPoint[i]; // Speichert den gerade betrachteten Nachbarpunkt an der Kante i in das Buff
					indexBuffer2++;
				}
			}
			indexBuffer1++;
		}
		for (indexBuffer2 = 0; indexBuffer2 < 10; indexBuffer2++) { // Schreibt den Buffer2 in Buffer1 und leert Buffer2 wenn Buffer1 abgearbeitet ist
			buffer1[indexBuffer2] = buffer2[indexBuffer2];
			buffer2[indexBuffer2] = -1;
		}
	}
}

short goHome() {
	ecrobot_status_monitor("Function: goHome");
	short startPunkt[2] = {0,0};
	short ziel = getArrayAddressFromCoord(startPunkt);
	short i; //Läuft in der for-schleife von 0-3 fur alle Richtungen
	short nextPointToVisit = 0; //Der nächste Punkt der noch nicht besucht ist
	short buffer1[10] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 }; // Hier sind die Punkte gespeicher die geprüft werden
	short buffer2[10] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 }; // Hier sind die nächsten Punkte die untersucht werden sollen
	short indexBuffer1 = 0; // Laufvariable die den ersten Buffer durchläuft
	short indexBuffer2; // Laufvariable für den 2. Buffer
	short checkThisPoint; //Der Punkt dessen Nachbarn als nächstes überprüft werden sollen
	for (i = 0; i < 51; i++) { // Setzt für alle Punkte zurück, dass es einen Pfad zu ihnen gibt
		allPoints[i].pathToPointExists = 0;
	}
	for (i = 0; i < 51; i++) { // Setzt für alle Punkte zurück, dass es einen Pfad zu ihnen gibt
		allPoints[i].edgeToPointBefore = -1;
	}
	allPoints[currentPoint].edgeToPointBefore = -1;
	allPoints[currentPoint].pathToPointExists = 1;
	setNeighbourPoints(currentPoint); // Sucht die Nachbarpunkte des aktuellen Punktes
	for (i = 0; i < 4; i++) { //Füllt Buffer1 für den ersten Durchlauf mit den Nachbarpunkten auf die eine Kante (i) zu dem Punkt haben
		if (allPoints[currentPoint].arrayEdges[i] == 1) {
			buffer1[indexBuffer1] = neighbourPoint[i];
			allPoints[neighbourPoint[i]].edgeToPointBefore = i; // ist der Weg zu dem Punkt die Kante i von dem jetzigem Punkt aus
			allPoints[neighbourPoint[i]].pathToPointExists = 1;
			if (buffer1[indexBuffer1] == ziel) {
				return nextPointToVisit = buffer1[indexBuffer1];
			}
			indexBuffer1++;
		}
	}
	while (nextPointToVisit == 0) { // => Solange kein Punkt gefunden ist der noch nicht besucht worden ist
		indexBuffer1 = 0; // Setzt den Indexbuffer auf den ersten Punkt im Buffer1-Array
		indexBuffer2 = 0; // Der nächste neue zu überprüfende Punkt wird im Buffer2 an die Position 0 geschrieben
		while (buffer1[indexBuffer1] != -1) { // Überprüft ob es noch einen Punkt im Buffer1-Array gibt
			checkThisPoint = buffer1[indexBuffer1]; // Ließt aus dem Array die Adresse des zu überprüfenden Punkted aus
			if (checkThisPoint == ziel) { // Checkt ob der momentan betrachtete Punkt schon besucht worden ist
				return nextPointToVisit = checkThisPoint; // Fals ja, setzt den nächst zu besuchenden Point gleich dem momentan betrachteten Punkt
			}
			setNeighbourPoints(checkThisPoint); // Füllt das Array neightbourPoint mit den Nachbarpunkten des momentan betrachteten Punktes auf
			for (i = 0; i < 4; i++) {
				if (allPoints[checkThisPoint].arrayEdges[i] == 1
						&& allPoints[neighbourPoint[i]].pathToPointExists
								== 0) { // Wenn es von dem momentan betrachtetem Punkt eine Kante in Richtung i gibt
					if (neighbourPoint[i] == ziel) { // und es zu dem Punkt in Richtung i keinen Weg gibt
						allPoints[neighbourPoint[i]].edgeToPointBefore = i; // ist der Weg zu dem Punkt die Kante i von dem jetzigem Punkt aus
						allPoints[neighbourPoint[i]].pathToPointExists = 1;
						return nextPointToVisit = neighbourPoint[i];
					}
					allPoints[neighbourPoint[i]].edgeToPointBefore = i; // ist der Weg zu dem Punkt die Kante i von dem jetzigem Punkt aus
					allPoints[neighbourPoint[i]].pathToPointExists = 1;
					buffer2[indexBuffer2] = neighbourPoint[i]; // Speichert den gerade betrachteten Nachbarpunkt an der Kante i in das Buff
					indexBuffer2++;
				}
			}
			indexBuffer1++;
		}
		for (indexBuffer2 = 0; indexBuffer2 < 10; indexBuffer2++) { // Schreibt den Buffer2 in Buffer1 und leert Buffer2 wenn Buffer1 abgearbeitet ist
			buffer1[indexBuffer2] = buffer2[indexBuffer2];
			buffer2[indexBuffer2] = -1;
		}
	}
}
void light(){
	ecrobot_set_light_sensor_active(NXT_PORT_S2);
	ecrobot_get_light_sensor(NXT_PORT_S2);
}
short allPointsVisited(){
	short i;
	for(i = 0; i < currentArrayPoint - 1; i++){
		if(allPoints[i].visited == 0){
			return 0;
		}
	}
	return 1;
}
TASK(OSEK_Main_Task) {
	ecrobot_set_light_sensor_active(NXT_PORT_S2);
	systick_wait_ms(500);
	a = ((ecrobot_get_light_sensor(NXT_PORT_S2)) - 70);
	systick_wait_ms(500);
	short pointsfiller;
	for ( pointsfiller = 0; pointsfiller < 51; pointsfiller++ ){
		allPoints[pointsfiller].pathToPointExists = 0;
		allPoints[pointsfiller].coord[0] = 0;
		allPoints[pointsfiller].coord[1] = 0;
		allPoints[pointsfiller].visited = 0;
		allPoints[pointsfiller].arrayEdges[0] = 0;
		allPoints[pointsfiller].arrayEdges[1] = 0;
		allPoints[pointsfiller].arrayEdges[2] = 0;
		allPoints[pointsfiller].arrayEdges[3] = 0;
	}
	//a=ecrobot_get_light_sensor(NXT_PORT_S2)-80;
	sensor();
	//Füllt alle Points mit werten auf
	while (1) {
		hello_world();
		if (tokenfound < 3 && allPointsVisited() == 0) {
			useNewPath(getNextPoint());
		} else if (tokenfound == 3) {
			useNewPath(goHome());
			stopp();
			systick_wait_ms(1000000);
		} else {
			ecrobot_sound_tone(220, 1000, 50);
			stopp();
			systick_wait_ms(1000000);
		}
	}
}
