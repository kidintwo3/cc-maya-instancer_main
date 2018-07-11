#ifndef ICONS_H
#define ICONS_H

#include "icons_repo.h"

void writeIcon_binary(MString path, MString filename, const unsigned char output[], int char_size)
{

	ofstream myfile;

	myfile.open(MString(path + filename).asChar(),  ios::out | ios::binary);
	myfile.write((char *)output, char_size);
	myfile.close();
	MGlobal::displayInfo(MString() + "[ClonerMulti] Created icon: " + path + filename);


}



void icons_data_write()
{


	MString path;
	MGlobal::executeCommand("internalVar -userBitmapsDir", path);
	MGlobal::displayInfo(MString() + "[ClonerMulti] Icons path: " + path);

	std::string c_path = path.asChar();

	writeIcon_binary(path, "out_clonerMultiLoc.png", __out_clonerMultiLoc_png, 16845);
	writeIcon_binary(path, "out_clonerMulti.png", __out_clonerMulti_png, 16496);
	writeIcon_binary(path, "clonerMulti_Trash.png", __clonerMulti_Trash_png, 17794);
	writeIcon_binary(path, "clonerMulti_Refresh.png", __clonerMulti_Refresh_png, 17788);
	writeIcon_binary(path, "clonerMulti_Plus.png", __clonerMulti_Plus_png, 17772);
	writeIcon_binary(path, "clonerMulti_Minus.png", __clonerMulti_Minus_png, 17772);
	writeIcon_binary(path, "clonerMulti_CCLogo.png", __clonerMulti_CCLogo_png, 19052);
	writeIcon_binary(path, "clonerMulti_Apply.png", __clonerMulti_Apply_png, 17762);
	writeIcon_binary(path, "clonerMulti.png", __clonerMulti_png, 20066);



}

#endif