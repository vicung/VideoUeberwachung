#include "Video�berwachung.h"
#include "Kalibrierung.h"
#include "�berwachung.h"
#include <opencv2/opencv.hpp>
#include <vector>
#include <sstream>
using namespace cv;
using namespace std;

int main(){
	Video�berwachung video�berwachung;
	int videoOrCam = 0; 
	cout<<"Video-Modus(0) oder Kamera-Modus(1)? ";
	cin >> videoOrCam;
	int NoCamArrayLength = 0;
	cout<<"Anzahl der nicht Videoueberwachten Felder: ";
	cin >> NoCamArrayLength;
	NoCamArrayLength = NoCamArrayLength * 2;
	Vector<Point> noCamArray(NoCamArrayLength);
	�berwachung �berwachung;
	Kalibrierung kalibrierung;
	noCamArray = kalibrierung.runCalibrate(NoCamArrayLength, noCamArray, videoOrCam);
	�berwachung.run�berwachung(noCamArray, videoOrCam);
	return 0;
}//main