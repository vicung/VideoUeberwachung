#ifndef VIDEOÜBERWACHUNG_H
#define VIDEOÜBERWACHUNG_H
#include <opencv2/opencv.hpp>
#include <vector>
#include <sstream>
using namespace cv;
using namespace std;

class VideoÜberwachung{
public:
	void initialize(int videoOrCam);
	void startVideo();
	void setAlarmGroesse(int aGroesse);
	int getAlarmGroesse();
	VideoCapture videoCapture;			//Videcapture

	Mat orVideoF;						//original Videofile
	Mat greyVideoF;						//graues Videofile
	Mat firstVideoFGrey;				//erster Frame des gauen Videofiles
	Mat greySubF;						//greySubF = abs(greyVideoF - firstVideoFGrey)
	Mat binaryMaskF;					//Binärmaske des 1. Videoframes
	Mat binaryMaskCopy;					//Kopie der Binärmaske
	Mat contoursAndBoundingRect;		//Videofile für Video mit countours und Boundingrect
	Mat maskMedianBlur;					//Maske für Medianblur
	Mat maskClosed;						//Maske fürs Closing (genutzt um die Lücken in der Maske zu entfernen)
	Mat maskOpened;						//Maske fürs Opening (genutzt um kleine überflüssige bzw. falsche Punkte in der Maske zu entfernen)
	Mat copyOfMask;						//Kopie der Maske: maskOpened, da der Aufruf von findContours das Originalbild zerstört

	int widthVideo;						//Integer Variable für die Breite des Videos
	int heightVideo;					//Integer Variable für die Höhe des Videos	
	int frameCounter;					//Zähler für die Videoframes
};
#endif