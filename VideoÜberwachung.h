#ifndef VIDEO�BERWACHUNG_H
#define VIDEO�BERWACHUNG_H
#include <opencv2/opencv.hpp>
#include <vector>
#include <sstream>
using namespace cv;
using namespace std;

class Video�berwachung{
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
	Mat binaryMaskF;					//Bin�rmaske des 1. Videoframes
	Mat binaryMaskCopy;					//Kopie der Bin�rmaske
	Mat contoursAndBoundingRect;		//Videofile f�r Video mit countours und Boundingrect
	Mat maskMedianBlur;					//Maske f�r Medianblur
	Mat maskClosed;						//Maske f�rs Closing (genutzt um die L�cken in der Maske zu entfernen)
	Mat maskOpened;						//Maske f�rs Opening (genutzt um kleine �berfl�ssige bzw. falsche Punkte in der Maske zu entfernen)
	Mat copyOfMask;						//Kopie der Maske: maskOpened, da der Aufruf von findContours das Originalbild zerst�rt

	int widthVideo;						//Integer Variable f�r die Breite des Videos
	int heightVideo;					//Integer Variable f�r die H�he des Videos	
	int frameCounter;					//Z�hler f�r die Videoframes
};
#endif