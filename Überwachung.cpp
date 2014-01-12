#include "Video�berwachung.h"
#include "�berwachung.h"
#include <opencv2/opencv.hpp>
#include <vector>
#include <sstream>
using namespace cv;
using namespace std;

//Konstruktor
�berwachung::�berwachung()
: gefundeneObjekte (0)
, gefaehrlicheObjekte(0)
, gefaehrlicheObjekteSchnitt(0)
, ungefaehrlicheObjekte(0)
, alarm(false)
, alarmFensterAn(false)
, alarmgr�sse(0)
{}

//Variablen deklaration
const string imagePath = "C:\\Users\\Lam0rMaN\\Desktop\\Alarm.jpg";					//Pfad f�r das Bild das bei einen Alarm ausgel�st werden soll
Mat image = imread(imagePath);														//Bild laden
Video�berwachung video�berwachung�;													//Video�berwachungsobjekt erstellen

//Methode �berr�ft ob sich die �bergebenen Point Werte pt1 und pt1 im nicht video�berwachten Bereich befinden
//und liefert einen Wahrheitswert als R�ckgabe (0 f�r Punkte befinden sich ausserhalb des nicht �berwachten bereichs,
//1 f�r Punkte befinden sich innerhalb des nicht �berwachten Bereichs)
bool �berwachung::checkBoundingrectInNoCamArea(Vector<Point> noCamArray, Point pt1, Point pt2){
	bool inNoCamArea = false;														//Boolean f�r Objekt befindet sich in nicht �berwachten Bereich
	if(noCamArray.size() >0){
		for(int i = 0; i < noCamArray.size() - 1; i = i + 2){
			if( (pt1.x > noCamArray[i].x && pt1.x < noCamArray[i+1].x) && (pt1.y > noCamArray[i].y && pt1.y < noCamArray[i+1].y) && 
				(pt2.x > noCamArray[i].x && pt2.x < noCamArray[i+1].x) && (pt2.y > noCamArray[i].y && pt2.y < noCamArray[i+1].y)){
				inNoCamArea = true;			
			}//if2
			else{
				inNoCamArea = false;
			}
		}//for
	}//if1
	return inNoCamArea;
}//checkBoundingrectInNoCamArea		

//Die Methode countObject z�hlt die Objekte und die gef�hrlichen Objekte f�r jeden Frame
void �berwachung::countObjects(int rectWidth, int rectHeight){
	if(rectWidth*rectHeight > 1500){
		gefundeneObjekte++;
	}//if
	if(rectWidth*rectHeight >= 1500 && rectWidth*rectHeight >= alarmgr�sse){
		gefaehrlicheObjekte++;
	}//if
}//countObjects

//Die Methode calcAverageObjects berechnet den Durschnitt der gesamten Objekte(Objekte, ungef�hrliche Objekte und gef�hrliche Objekte)
//�ber 25 Frames
void �berwachung::calcAverageObjects(){
		gefundeneObjekteSchnitt = 0;
		gefaehrlicheObjekteSchnitt = 0;
		ungefaehrlicheObjekte = 0;
		gefundeneObjekteSchnitt = (gefundeneObjekte +10) / 25;
		gefaehrlicheObjekteSchnitt = (gefaehrlicheObjekte +10) / 25;
		ungefaehrlicheObjekte = gefundeneObjekteSchnitt - gefaehrlicheObjekteSchnitt;
		if(ungefaehrlicheObjekte < 0){
			ungefaehrlicheObjekte = 0;
		}
		gefaehrlicheObjekte = 0;
		gefundeneObjekte = 0;
}//calcAverageObjects

//Die Methode writeText gibt alle 75 Frames �ber die Kommandoconsole die Anzahl aller Objekte
//(Objekte, ungef�hrliche Objekte und gef�hrliche Objekte) sowie eine Alarmmeldung, sollte dieser ausgel�st sein, aus.
//Desweiteren zeichnet sie die Anzahl der Objekte in der Videofenster, sowie eine Alarmmeldung(falls Alarm ausgel�st) 
//bei jeden 3 Frame
void �berwachung::writeText(int ObjekteSchnitt, int unsortiertegObjekte, int gObjekteSchnitt){
		int gefundeneObjekteSchnitt = ObjekteSchnitt;
		int ungefaehrlicheObjekte = unsortiertegObjekte;
		int gefaehrlicheObjekteSchnitt = gObjekteSchnitt;
		String objektString;
		String uObjektString;
		String gObjektString;
		if(video�berwachung�.frameCounter % 75 == 0){
			cout << "Gefundene Objekte: " << gefundeneObjekteSchnitt << " davon " << ungefaehrlicheObjekte << " ungefaehrliche Objekte und "<< gefaehrlicheObjekteSchnitt << endl;
			cout <<"potentiell gefaehrliche Objekte." << endl;
			cout << " " << endl;
		}//if
		ostringstream Str;
		Str << gefundeneObjekteSchnitt;
		string objekte(Str.str());
		objektString = "Objekte: " + objekte;
		ostringstream Str2;
		Str2 << ungefaehrlicheObjekte;
		string uObjekte(Str2.str());
		uObjektString = "u. Objekte: " + uObjekte;
		ostringstream Str3;
		Str3 << gefaehrlicheObjekteSchnitt;
		string gObjekte(Str3.str());
		gObjektString = "g. Objekte: " + gObjekte;
		if(alarm && video�berwachung�.frameCounter % 75 == 0){
			cout << "Alarm, Einbrecher gefunden!" << endl;
		}//if
		if(video�berwachung�.frameCounter % 3 == 0 && alarm){
			putText(video�berwachung�.contoursAndBoundingRect, "ALARM!!!", Point2f(video�berwachung�.heightVideo/2 - 30, video�berwachung�.widthVideo/2 - 75), FONT_HERSHEY_PLAIN, 4, Scalar(0,0,255), 4);
		}//if
		if(video�berwachung�.frameCounter >= 30){
			putText(video�berwachung�.contoursAndBoundingRect, objektString, Point2f(0, video�berwachung�.widthVideo - 210), FONT_HERSHEY_PLAIN, 2, Scalar(255,0,0), 2);
			putText(video�berwachung�.contoursAndBoundingRect, uObjektString, Point2f(0, video�berwachung�.widthVideo - 170), FONT_HERSHEY_PLAIN, 2, Scalar(255,0,0), 2);
			putText(video�berwachung�.contoursAndBoundingRect, gObjektString, Point2f(340, video�berwachung�.widthVideo - 170), FONT_HERSHEY_PLAIN, 2, Scalar(255,0,0), 2);	
		}
}//writeText

//Die Methode showAlam �ffnet wenn Alarm ausgel�st wurde ein Fenster mit den Alarmhinweis und einer
//Trackbar �ber die man die Alarm wieder deaktivieren kann
void �berwachung::showAlarm(){
	if(alarm && alarmFensterAn == false){
		alarmFensterAn = true;
		namedWindow("Alarm!!!", 1);
		createTrackbar("resetAlarm", "Alarm!!!",0, 1);
		setTrackbarPos("resetAlarm", "Alarm!!!", 0);
		resize(image, image, Size(300, 300));
		imshow("Alarm!!!",image); 
		resizeWindow("Alarm!!!", 300,300);
		if("resetAlarm", "Alarm!!!"){
			alarm = true;
		}//if2
		else{
			alarm = false;
		}//else
	}//if1
	if(getTrackbarPos("resetAlarm", "Alarm!!!")){
		alarm = false;
		destroyWindow("Alarm!!!");
		alarmFensterAn = false;
	}//if
}//showAlarm

void �berwachung::run�berwachung(Vector<Point> noCamArray, int videoOrCam){ 
	alarmgr�sse = video�berwachung�.getAlarmGroesse();								//Alarmgr�sse von Video�berwachung holen
	video�berwachung�.initialize(videoOrCam);										//initialisierung
	alarm = false;																	//Boolean f�r Alarm aktiviert/deaktiviert	
	namedWindow("Vide�berwachung", 1);												//video�berwachung�sfenster erstellen
	createTrackbar("Alarmgroesse", "Vide�berwachung",&alarmgr�sse,					//alarmgroesse Trackbar erstellen mit Werten von 0 bis Pixelanzahl des Videos(Startwert=aus calibrate)	
					video�berwachung�.heightVideo*video�berwachung�.widthVideo);

	while(true){
		bool success = video�berwachung�.videoCapture.read(video�berwachung�.orVideoF);
		if( success == false){
			break;
		}//if
		video�berwachung�.startVideo();											//Video starten

		//Region finding bei video�berwachung�.copyOfMask
		vector<vector<Point> > contours;
        findContours(video�berwachung�.copyOfMask, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

		//Die einzelnen Regionen durchlaufen
		for(int i = 0; i < contours.size(); i++){
			vector<Point> contour = contours[i];
			Rect rect = boundingRect(contour);
			double area = contourArea(contour);
			Point pt1 = rect.tl();
			Point pt2 = rect.br();
			int rectWidth = rect.width;
			int rectHeight = rect.height;
			Scalar contourScalar = Scalar(0,255,0);
			alarmgr�sse = getTrackbarPos("Alarmgroesse", "Vide�berwachung");
			video�berwachung�.setAlarmGroesse(alarmgr�sse);
			if((checkBoundingrectInNoCamArea(noCamArray, pt1, pt2)) == false){	//Wenn die Punkte sich ausserhalb des nicht �berwachten Bereichs befinden werden
				countObjects(rectWidth, rectHeight);							//Objekte und gef�hrliche Objekte jedes Frames gez�hlt
				//Bounding Rectangle in Kopie von Originalvideo zeichnen
				if(rectWidth*rectHeight < alarmgr�sse){						//Wenn BoundingRect < Alarmgr��e wird BR gr�n gezeichnet
					contourScalar = Scalar(0,255,0);
				} //if in if1
				else {														//Wenn BoundingRect > Alarmgr��e wird BR rot gezeichnet und Alarm ausgel�st
					if(rectWidth*rectHeight > 1500){
						if(alarm == false){
							cout << "Alarm, Einbrecher gefunden!" << endl;
						}//if2
						alarm = true;
						contourScalar = Scalar(0,0,255);
					}//if in else
				}//else
				if(rectWidth*rectHeight > 1500){												//Wenn BoundingRect > 1500 wird BR in Video gezeichnet
					rectangle(video�berwachung�.contoursAndBoundingRect, rect.tl(), rect.br(), contourScalar, 2);
				}//if inf if1
			}//if1
		}//for
		if(video�berwachung�.frameCounter % 25 == 0){
			calcAverageObjects();																//Durchschnitt der Objekte innerhalb der letzten 25 Frames berechnen
		}
		writeText(gefundeneObjekteSchnitt, ungefaehrlicheObjekte, gefaehrlicheObjekteSchnitt);	//Ausgabe auf der gez�hlten Elemente und Alarmnachricht auf Console und in Video
		imshow("Vide�berwachung", video�berwachung�.contoursAndBoundingRect);										//video�berwachung�sfenster anzeigen.
		showAlarm();																			//Pr�fen ob Alarm ausgel�st wurde, wenn ja Alarmierungfenster anzeigen
		int key = waitKey(30);
		if(key != -1){
			break;
		}//if
	}//while
}//run�berwachung
