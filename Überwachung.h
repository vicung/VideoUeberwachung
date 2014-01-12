#ifndef �BERWACHUNG_H
#define �BERWACHUNG_H

class �berwachung{
public:
	�berwachung();																		//Konstruktor von �berwachung
	void run�berwachung(Vector<Point> noCamArray, int videoOrCam);						//Startet die Vide�berwachung
private:
	bool checkBoundingrectInNoCamArea(Vector<Point> noCamArray, Point pt1, Point pt2);	//Pr�ft ob sich das Boundingrect in nicht �berwachten Bereich befindet
	void countObjects(int rectWidth, int rectHeight);									//Z�hlt die Objekte
	void calcAverageObjects();															//Berechnet den Durschnitt der Objekte innerhalb von 75 frames
	void writeText(int ObjekteSchnitt, int unsortiertegObjekte, int gObjekteSchnitt);	//Textausgabe in Video und auf Commandoconsole
	void showAlarm();																	//�ffnet Alarmfenster und setzt Alarm zur�ck
	bool alarm;																	//Boolean f�r Alarm aktiviert
	bool alarmFensterAn;														//Boolean f�r Alarmfenster An/Aus
	int gefundeneObjekte;														//Anzahl der gefundenen Objekte
	int gefaehrlicheObjekte;													//Anzahl der gefundenen gef�hrlicher Objekte
	int gefundeneObjekteSchnitt;												//Durschnitt der gefundenen Objekte in der Counter Zeit
	int gefaehrlicheObjekteSchnitt;												//Durschnitt der gefundenen gef�hrlichen Objekte in der Counter Zeit				
	int ungefaehrlicheObjekte;													//Durschnitt der gefundenen ungef�hrlichen Objekte in der Counter Zeit	
	int alarmgr�sse;															//Int Variable f�r die gr��es eines Objektes(in Pixeln) ab der Alarm ausgel�st wird
};
#endif