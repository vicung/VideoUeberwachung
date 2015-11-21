#ifndef ÜBERWACHUNG_H
#define ÜBERWACHUNG_H

class Überwachung{
public:
	Überwachung();																		//Konstruktor von Überwachung
	void runÜberwachung(Vector<Point> noCamArray, int videoOrCam);						//Startet die Videüberwachung
private:
	bool checkBoundingrectInNoCamArea(Vector<Point> noCamArray, Point pt1, Point pt2);	//Prüft ob sich das Boundingrect in nicht überwachten Bereich befindet
	void countObjects(int rectWidth, int rectHeight);									//Zählt die Objekte
	void calcAverageObjects();															//Berechnet den Durschnitt der Objekte innerhalb von 75 frames
	void writeText(int ObjekteSchnitt, int unsortiertegObjekte, int gObjekteSchnitt);	//Textausgabe in Video und auf Commandoconsole
	void showAlarm();																	//Öffnet Alarmfenster und setzt Alarm zurück
	bool alarm;																	//Boolean für Alarm aktiviert
	bool alarmFensterAn;														//Boolean für Alarmfenster An/Aus
	int gefundeneObjekte;														//Anzahl der gefundenen Objekte
	int gefaehrlicheObjekte;													//Anzahl der gefundenen gefährlicher Objekte
	int gefundeneObjekteSchnitt;												//Durschnitt der gefundenen Objekte in der Counter Zeit
	int gefaehrlicheObjekteSchnitt;												//Durschnitt der gefundenen gefährlichen Objekte in der Counter Zeit				
	int ungefaehrlicheObjekte;													//Durschnitt der gefundenen ungefährlichen Objekte in der Counter Zeit	
	int alarmgrösse;															//Int Variable für die größes eines Objektes(in Pixeln) ab der Alarm ausgelöst wird
};
#endif