#include "VideoÜberwachung.h"
#include "Kalibrierung.h"
#include <opencv2/opencv.hpp>
#include <vector>
#include <sstream>
using namespace cv;
using namespace std;

//Konstruktor
Kalibrierung::Kalibrierung()
: alarmgroesse(3000)
{}

//Variablen deklaration
VideoÜberwachung videoüberwachungV;	//Videoüberwachungsobjekt erstellen
bool mouseDragged = false;			//Boolean ob die Mouse gedrückt ist
Point mousePoint1, mousePoint2;		//Point für Mauskordinaten zwischen denen die Rechtecke für B

//MousHandler implemtentiert eine drag Funktion und wird im Programm genutzt um Rechtecke zu zeichnen während die Maus gedrückt ist,
//mit diesen Rechtecken wird die Größe für gefährliche Objekte bestimmt, sowie Bereiche definiert die nicht vom Programm überwacht werden sollen.
void mCallback(int event, int x, int y, int flags, void* userdata){
	if  ( event == EVENT_LBUTTONDOWN && mouseDragged== false){						
		mousePoint1 = Point(x, y);												//speichert die Mousekordinate beim drücken der linken Maustaste
		mouseDragged = true;														//und setzt gleichzeitig die Variable mouseDragged auf 1
	}
	else if  ( event == EVENT_LBUTTONUP && mouseDragged){
		mousePoint1 = Point(0, 0);
		mousePoint2 = Point(0, 0);
		mouseDragged = false;
	}
    else if ( event == EVENT_MOUSEMOVE && mouseDragged){
        mousePoint2 = Point(x, y);
    }
}

//Methode die das noCamArry mit den Anfangs- und Endkoordinaten für die nicht videoüberwachten Bereiche übergeben bekommt
//und dieses sortiert, so das die kleineren x- und y-koordinaten der einzelnen Rechtecke als deren erste Koordinaten gespeichert sind
Vector<Point> Kalibrierung::sortNoCamArray(Vector<Point> noCamArray){
	int temp;
	for(int i = 0; i < noCamArray.size() - 1; i = i + 2){
		if(noCamArray[i].x > noCamArray[i+1].x){
			temp = noCamArray[i].x;
			noCamArray[i].x = noCamArray[i+1].x;
			noCamArray[i+1].x = temp;
		}//if
		if(noCamArray[i].y > noCamArray[i+1].y){
			temp = noCamArray[i].y;
			noCamArray[i].y = noCamArray[i+1].y;
			noCamArray[i+1].y = temp;
		}//if
	}//for
	return noCamArray;
}//sortNoCamArray

//Methode zur Kalibrierung
Vector<Point> Kalibrierung::runCalibrate(int NoCamArrayLength		//Anzahl der Bereiche die nicht vom System überwacht werden sollen
						,Vector<Point> noCamArray					//Point Array mit den Afangs- und Endpunkten der Rechtecke welche nicht vom System überwacht werden sollen
						, int videoOrCam){							//Auswahl ob ein Video abgespielt oder von der Kamera aufgenommen werden soll


	//Initialisierung
	videoüberwachungV.initialize(videoOrCam);									//Initaliserung der Aufnahme bzw. abspielen des Videos mit Parameter übergabe für Video oder Kamera


	//Variablendeklaration
	namedWindow("Kalibrierung", 1);												//Kablibrierungsfenster erstellen
	setMouseCallback("Kalibrierung", mCallback, NULL);							//MouseHandler für das Kalibrierungsfenster aktivieren
	int kalibrierung;															//Wert aus Trackbar "Kalibrieren"
	createTrackbar("Kalibrieren", "Kalibrierung",&kalibrierung, 1);				//Kalibrieren Trackbar erstellen mit den Werten 0 und1 (dienst als Button für An/Aus)
	setTrackbarPos("Kalibrieren", "Kalibrierung", 1 );							//Startposition der Kalibrieren Trackbar auf 1 setzen
	int setGroesse;																//Wert aus Trackbar "setGroesse"
	createTrackbar("setGroesse", "Kalibrierung",&setGroesse, 1);				//setGroesse Trackbar erstellen mit den Werten 0 und1 (dienst als Button für An/Aus)
	setTrackbarPos("setGroesse", "Kalibrierung", 1 );							//Startposition der setGroesse Trackbar auf 1 setzen
	createTrackbar("Alarmgroesse", "Kalibrierung",&alarmgroesse,				//Alarmgroesse Trackbar erstellen mit Werten von 0 bis Pixelanzahl des Videos(Startwert=3000)
					videoüberwachungV.heightVideo*videoüberwachungV.widthVideo);	
	int key = waitKey(1500);													//Delay damit die Kamera angeschaltet wird
	int rechteckAlarmgroesse = 0;												//Variable für die Rechteckgröße des Rechteckes mit den die Alarmgröße für objekte bestimmt wird 			
	int noCamCount = 0;															//Counter für die Bereiche die nicht vom System überwacht werden sollen
	int noCamRecSet = false;													//Boolean ob ein Bereich ausgewählt wurde der nicht vom System überwacht werden soll


	while(true){																//Originalvideo wird abgespielt
		bool success = videoüberwachungV.videoCapture.read(videoüberwachungV.orVideoF);
		if( success == false){
			break;
		}//if
		videoüberwachungV.startVideo();

		//Region finding bei copyOfMask
		vector<vector<Point> > contours;
        findContours(videoüberwachungV.copyOfMask, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

		//Die einzelnen Regionen durchlaufen
		for(int i = 0; i < contours.size(); i++){
			vector<Point> contour = contours[i];
			Rect rect = boundingRect(contour);
			double area = contourArea(contour);
			Point pt1 = rect.tl();
			Point pt2 = rect.br();
			int rectWidth = rect.width;
			int rectHeight = rect.height;
			Scalar contourScalar = Scalar(255,0,0);								
			if(rectWidth*rectHeight < alarmgroesse){
				contourScalar = Scalar(0,255,0);								//Bei einer größe des Bounding Rectangles < als die Alarmgrösse wird das BR grün gefärbt
			}//if
			else {
				contourScalar = Scalar(0,0,255);								//Bei einer größe des Bounding Rectangles < als die Alarmgrösse wird das BR rot gefärbt
			}//else
			if(rectWidth*rectHeight > 1500){																	 //Wenn Bounding Rectangle > 1500 Pixel wird 
				rectangle(videoüberwachungV.contoursAndBoundingRect, rect.tl(), rect.br(), contourScalar, 2);	 //Bounding Rectangle und Konturen in Kopie von Originalvideo zeichnen
			}//if
		}//for

			//Bei gedrückter linker Maustaste und "setGroesse" Trackbar = 1 kann ein blaues Rechteck für die Alarmgrösse gezeichnet werden und 
			//die Fläche dieses Rechteckes in Pixeln als Alarmgröße gesetzt, sowie die Trackbar auf Position der Alarmgröße gesetzt
			if(mouseDragged && setGroesse){
				rectangle(videoüberwachungV.contoursAndBoundingRect, mousePoint1, mousePoint2, Scalar(255, 0, 0), 2);
				rechteckAlarmgroesse = (mousePoint2.x - mousePoint1.x) * (mousePoint2.y - mousePoint1.y);
				if(rechteckAlarmgroesse < 0){
					rechteckAlarmgroesse = rechteckAlarmgroesse * (-1);
				}//if2
				setTrackbarPos("Alarmgroesse", "Kalibrierung", rechteckAlarmgroesse);
				videoüberwachungV.setAlarmGroesse(rechteckAlarmgroesse);
			}//if1

			//Bei gedrückter linker Maustaste und "setGroesse" Trackbar = 0 können soviele weiße Rechteck für die nicht Videoüberwachten bereiche 
			//gezeichnet werden wie beim Start des Programmes angegeben
			if(mouseDragged && setGroesse == 0 && noCamCount <= NoCamArrayLength -1){
				rectangle(videoüberwachungV.contoursAndBoundingRect, mousePoint1, mousePoint2, Scalar(255, 255, 255), 2);
				noCamArray[noCamCount] = mousePoint1;
				noCamArray[noCamCount+1] = mousePoint2;
				noCamRecSet = true;
			}//if
			if(mouseDragged == false && noCamRecSet && noCamCount <= NoCamArrayLength -1){
				noCamCount = noCamCount+ 2;
				noCamRecSet = false;
			}//if
		//Das Video wird solange abgespielt bis die Kalibrierung abgeschlossen wurde (Kalibrieren Setbar auf 0 gesetzt) oder das Video zuende ist.
		int key = waitKey(30);
		if(key != -1){
			break;
		}//if
		imshow("Kalibrierung", videoüberwachungV.contoursAndBoundingRect);
		if (kalibrierung == 0){
			break;
		}//if
	}//while
	destroyWindow("Kalibrierung");
	//Nach dem Ende der Kalibrierung wird das Arry mit den Anfangs- und Endkoordinaten für sortiert, so das 
	//die kleineren x- und y-koordinaten der einzelnen Rechtecke als deren erste Koordinaten gespeichert sind
	if(NoCamArrayLength > 0){
		noCamArray = sortNoCamArray(noCamArray);
	}//if
	return noCamArray;				//Am Ende der Funktion wird das noCamArray noch zurückgegeben
}//runCalibration
