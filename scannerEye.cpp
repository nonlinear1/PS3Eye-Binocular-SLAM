#include scannerEye.h


void scannerEye::setup(int camID, int camWidth, int camHeight, int camRefresh, ps3eye_format format)
{

}

void initcam()
{
	ps3eye_init();
	numberOfCams = ps3eye_count_connected();
	firstCam = ps3eye_open(0, cam_width, cam_height, cam_refresh, PS3EYE_FORMAT_RGB);
}

void deInitCam()
{

}