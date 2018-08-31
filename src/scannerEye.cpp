#include "scannerEye.h"

using namespace ps3eye;

scannerEye::scannerEye() 
{

}

void scannerEye::setup() //get ID passed in here? nope -- in initCam()
{
	
}

void scannerEye::initCam(int ps3eyeID, int width, int height, int refresh) // pass ID to initCam()
{
	//ps3eye_init(); // might need to call this sooner, possibly even out of this class? check it.
	camID = ps3eyeID;
	cam_width = width;
	cam_height = height;
	cam_refresh = refresh;
	camInstance = ps3eye_open(camID, cam_width, cam_height, cam_refresh, PS3EYE_FORMAT_RGB);
}

void scannerEye::deInitCam()
{
	ps3eye_close(camInstance);
	ps3eye_uninit(); // library specifically mentions this function will
					 // do nothing if there are any other pseye_t *
					 // objects still running -- can't hurt to try freeing resources!
}

// grabs and returns raw image from camera
// to do: functions for bgr format, function to return opencv mat rather than raw image
unsigned char* scannerEye::grabRawFrame()
{
	ps3eye_grab_frame(camInstance, camRawPixels);
	//printf((char*)camRawPixels);
	return camRawPixels;
}