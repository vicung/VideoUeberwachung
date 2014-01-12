#include "VideoÜberwachung.h"
#include "Überwachung.h"
#include <opencv2/opencv.hpp>
#include <vector>
#include <sstream>
using namespace cv;
using namespace std;

//Konstruktor
Überwachung::Überwachung()
: gefundeneObjekte (0)
, gefaehrlicheObjekte(0)
, gefaehrlicheObjekteSchnitt(0)
, ungefaehrlicheObjekte(0)
, alarm(false)
, alarmFensterAn(false)
, alarmgrösse(0)
{}

//Variablen deklaration
const string imagePath = "C:\\Users\\Lam0rMaN\\Desktop\\Alarm.jpg";					//Pfad für das Bild das bei einen Alarm ausgelöst werden soll
Mat image = imread(imagePath);														//Bild laden
VideoÜberwachung videoüberwachungÜ;													//Videoüberwachungsobjekt erstellen

//Methode überrüft ob sich die übergebenen Point Werte pt1 und pt1 im nicht videoüberwachten Bereich befinden
//und liefert einen Wahrheitswert als Rückgabe (0 für Punkte befinden sich ausserhalb des nicht überwachten bereichs,
//1 für Punkte befinden sich innerhalb des nicht überwachten Bereichs)
bool Überwachung::checkBoundingrectInNoCamArea(Vector<Point> noCamArray, Point pt1, Point pt2){
	bool inNoCamArea = false;														//Boolean für Objekt befindet sich in nicht überwachten Bereich
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

//Die Methode countObject zählt die Objekte und die gefährlichen Objekte für jeden Frame
void Überwachung::countObjects(int rectWidth, int rectHeight){
	if(rectWidth*rectHeight > 1500){
		gefundeneObjekte++;
	}//if
	if(rectWidth*rectHeight >= 1500 && rectWidth*rectHeight >= alarmgrösse){
		gefaehrlicheObjekte++;
	}//if
}//countObjects

//Die Methode calcAverageObjects berechnet den Durschnitt der gesamten Objekte(Objekte, ungefährliche Objekte und gefährliche Objekte)
//über 25 Frames
void Überwachung::calcAverageObjects(){
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

//Die Methode writeText gibt alle 75 Frames über die Kommandoconsole die Anzahl aller Objekte
//(Objekte, ungefährliche Objekte und gefährliche Objekte) sowie eine Alarmmeldung, sollte dieser ausgelöst sein, aus.
//Desweiteren zeichnet sie die Anzahl der Objekte in der Videofenster, sowie eine Alarmmeldung(falls Alarm ausgelöst) 
//bei jeden 3 Frame
void Überwachung::writeText(int ObjekteSchnitt, int unsortiertegObjekte, int gObjekteSchnitt){
		int gefundeneObjekteSchnitt = ObjekteSchnitt;
		int ungefaehrlicheObjekte = unsortiertegObjekte;
		int gefaehrlicheObjekteSchnitt = gObjekteSchnitt;
		String objektString;
		String uObjektString;
		String gObjektString;
		if(videoüberwachungÜ.frameCounter % 75 == 0){
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
		if(alarm && videoüberwachungÜ.frameCounter % 75 == 0){
			cout << "Alarm, Einbrecher gefunden!" << endl;
		}//if
		if(videoüberwachungÜ.frameCounter % 3 == 0 && alarm){
			putText(videoüberwachungÜ.contoursAndBoundingRect, "ALARM!!!", Point2f(videoüberwachungÜ.heightVideo/2 - 30, videoüberwachungÜ.widthVideo/2 - 75), FONT_HERSHEY_PLAIN, 4, Scalar(0,0,255), 4);
		}//if
		if(videoüberwachungÜ.frameCounter >= 30){
			putText(videoüberwachungÜ.contoursAndBoundingRect, objektString, Point2f(0, videoüberwachungÜ.widthVideo - 210), FONT_HERSHEY_PLAIN, 2, Scalar(255,0,0), 2);
			putText(videoüberwachungÜ.contoursAndBoundingRect, uObjektString, Point2f(0, videoüberwachungÜ.widthVideo - 170), FONT_HERSHEY_PLAIN, 2, Scalar(255,0,0), 2);
			putText(videoüberwachungÜ.contoursAndBoundingRect, gObjektString, Point2f(340, videoüberwachungÜ.widthVideo - 170), FONT_HERSHEY_PLAIN, 2, Scalar(255,0,0), 2);	
		}
}//writeText

//Die Methode showAlam öffnet wenn Alarm ausgelöst wurde ein Fenster mit den Alarmhinweis und einer
//Trackbar über die man die Alarm wieder deaktivieren kann
void Überwachung::showAlarm(){
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

void Überwachung::runÜberwachung(Vector<Point> noCamArray, int videoOrCam){ 
	alarmgrösse = videoüberwachungÜ.getAlarmGroesse();								//Alarmgrösse von Videoüberwachung holen
	videoüberwachungÜ.initialize(videoOrCam);										//initialisierung
	alarm = false;																	//Boolean für Alarm aktiviert/deaktiviert	
	namedWindow("Videüberwachung", 1);												//videoüberwachungÜsfenster erstellen
	createTrackbar("Alarmgroesse", "Videüberwachung",&alarmgrösse,					//alarmgroesse Trackbar erstellen mit Werten von 0 bis Pixelanzahl des Videos(Startwert=aus calibrate)	
					videoüberwachungÜ.heightVideo*videoüberwachungÜ.widthVideo);

	while(true){
		bool success = videoüberwachungÜ.videoCapture.read(videoüberwachungÜ.orVideoF);
		if( success == false){
			break;
		}//if
		videoüberwachungÜ.startVideo();											//Video starten

		//Region finding bei videoüberwachungÜ.copyOfMask
		vector<vector<Point> > contours;
        findContours(videoüberwachungÜ.copyOfMask, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

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
			alarmgrösse = getTrackbarPos("Alarmgroesse", "Videüberwachung");
			videoüberwachungÜ.setAlarmGroesse(alarmgrösse);
			if((checkBoundingrectInNoCamArea(noCamArray, pt1, pt2)) == false){	//Wenn die Punkte sich ausserhalb des nicht überwachten Bereichs befinden werden
				countObjects(rectWidth, rectHeight);							//Objekte und gefährliche Objekte jedes Frames gezählt
				//Bounding Rectangle in Kopie von Originalvideo zeichnen
				if(rectWidth*rectHeight < alarmgrösse){						//Wenn BoundingRect < Alarmgröße wird BR grün gezeichnet
					contourScalar = Scalar(0,255,0);
				} //if in if1
				else {														//Wenn BoundingRect > Alarmgröße wird BR rot gezeichnet und Alarm ausgelöst
					if(rectWidth*rectHeight > 1500){
						if(alarm == false){
							cout << "Alarm, Einbrecher gefunden!" << endl;
						}//if2
						alarm = true;
						contourScalar = Scalar(0,0,255);
					}//if in else
				}//else
				if(rectWidth*rectHeight > 1500){												//Wenn BoundingRect > 1500 wird BR in Video gezeichnet
					rectangle(videoüberwachungÜ.contoursAndBoundingRect, rect.tl(), rect.br(), contourScalar, 2);
				}//if inf if1
			}//if1
		}//for
		if(videoüberwachungÜ.frameCounter % 25 == 0){
			calcAverageObjects();																//Durchschnitt der Objekte innerhalb der letzten 25 Frames berechnen
		}
		writeText(gefundeneObjekteSchnitt, ungefaehrlicheObjekte, gefaehrlicheObjekteSchnitt);	//Ausgabe auf der gezählten Elemente und Alarmnachricht auf Console und in Video
		imshow("Videüberwachung", videoüberwachungÜ.contoursAndBoundingRect);										//videoüberwachungÜsfenster anzeigen.
		showAlarm();																			//Prüfen ob Alarm ausgelöst wurde, wenn ja Alarmierungfenster anzeigen
		int key = waitKey(30);
		if(key != -1){
			break;
		}//if
	}//while
}//runÜberwachung
