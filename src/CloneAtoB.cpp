//
//  CloneSphere.h
//  ClonerMulti
//
//  Created by Hunyadi János on 2015. 01. 03..
//  Copyright (c) 2015. Janos Hunyadi. All rights reserved.
//



#include "CloneSphere.h"

// --- Instance A to B ------
MStatus ClonerMultiThread::instanceAtoB(){

	MStatus status;

	m_tr_matA.clear();
	m_tr_matA.setLength(m_numDup);

	MTransformationMatrix trMatA(m_inLocA_posMat);
	MTransformationMatrix trMatB(m_inLocB_posMat);

	MPoint inLocAP = trMatA.getTranslation(MSpace::kWorld);
	MPoint inLocBP = trMatB.getTranslation(MSpace::kWorld);

	MVector ab = inLocBP - inLocAP;


	// Set up a temp numdup divider just for the curve
	int numdup_divider = m_numDup - 1;
	if (numdup_divider < 1) { numdup_divider = 1; } 
	 


	double step = (ab.length() / double(numdup_divider) );



	MVector ab_dir = ab;
	ab_dir.normalize();

	MVector xDir = ab_dir;
	MVector yDir = xDir ^ m_firstUpVec;
	yDir.normalize();
	MVector zDir = yDir ^ xDir;
	zDir.normalize();

	MTransformationMatrix mTr;

	MPoint b;

	MMatrix rotMatrix;


	//double bbW,bbMinX;

	//bbMinX = m_bbPosMinArray[m_idArray[0]][0].x;
	//double cX = 0.0-bbMinX;


	for(int i=0; i < m_numDup; i++)
	{



		MFloatPoint p(inLocAP + xDir * step * double(i));

		double m[4][4] = {{xDir.x, xDir.y, xDir.z, 0.0}, {zDir.x, zDir.y, zDir.z, 0.0}, {yDir.x, yDir.y, yDir.z, 0.0}, {p.x, p.y, p.z, 1.0}};

		rotMatrix = m;


		// Translation
		MFloatVector v_baseOffV(m_offsetX, m_offsetY, m_offsetZ);

		// Rotation
		double rot[3] = {m_rotateX * 0.5f * ( M_PI / 180.0f ), m_rotateY * 0.5f * ( M_PI / 180.0f ),  m_rotateZ * 0.5f * ( M_PI / 180.0f )};

		// Scale
		const double scaleV[3] = {  double(m_scaleX),  double(m_scaleY),  double(m_scaleZ) };

		// Matrix
		MTransformationMatrix tr_mat(rotMatrix);

		status = tr_mat.addTranslation(v_baseOffV, MSpace::kObject);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		status = tr_mat.setScale(scaleV, MSpace::kObject);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		status = tr_mat.addRotation(rot, MTransformationMatrix::kXYZ, MSpace::kObject);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		m_tr_matA.set(tr_mat.asMatrix(), i);


	}



	return MS::kSuccess;
}