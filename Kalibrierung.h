#ifndef KALIBRIERUNG_H
#define KALIBRIERUNG_H

class Kalibrierung{
public:
	Kalibrierung();																					//Konstruktor von Kalibrierung
	Vector<Point> runCalibrate(int NoCamArrayLength,Vector<Point> noCamArray, int videoOrCam);		//Startet die Kalibrierung
private:
	Vector<Point> sortNoCamArray(Vector<Point> noCamArray);											//Sortier das Array in welchen die nicht �berwachten Felder gespeichert sind
	int alarmgroesse;																				//Int Variable f�r die gr��es eines Objektes(in Pixeln) ab der Alarm ausgel�st wird
};
#endif