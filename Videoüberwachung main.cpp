#include "VideoÜberwachung.h"
#include "Kalibrierung.h"
#include "Überwachung.h"
#include <opencv2/opencv.hpp>
#include <vector>
#include <sstream>
using namespace cv;
using namespace std;

int main(){
	VideoÜberwachung videoÜberwachung;
	int videoOrCam = 0; 
	cout<<"Video-Modus(0) oder Kamera-Modus(1)? ";
	cin >> videoOrCam;
	int NoCamArrayLength = 0;
	cout<<"Anzahl der nicht Videoueberwachten Felder: ";
	cin >> NoCamArrayLength;
	NoCamArrayLength = NoCamArrayLength * 2;
	Vector<Point> noCamArray(NoCamArrayLength);
	Überwachung überwachung;
	Kalibrierung kalibrierung;
	noCamArray = kalibrierung.runCalibrate(NoCamArrayLength, noCamArray, videoOrCam);
	überwachung.runÜberwachung(noCamArray, videoOrCam);
	return 0;
}//main