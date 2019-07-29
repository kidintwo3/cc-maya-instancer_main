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
		double off_ramp_multX, off_ramp_multY, off_ramp_multZ = 1.0;

		if (i < int(m_offsetProfileAX.length())) { off_ramp_multX = m_offsetProfileAX[i]; }
		if (i < int(m_offsetProfileAY.length())) { off_ramp_multY = m_offsetProfileAY[i]; }
		if (i < int(m_offsetProfileAZ.length())) { off_ramp_multZ = m_offsetProfileAZ[i]; }

		double rot_ramp_multX, rot_ramp_multY, rot_ramp_multZ = 1.0;

		if (i < int(m_rotateProfileAX.length())) { rot_ramp_multX = m_rotateProfileAX[i]; }
		if (i < int(m_rotateProfileAY.length())) { rot_ramp_multY = m_rotateProfileAY[i]; }
		if (i < int(m_rotateProfileAZ.length())) { rot_ramp_multZ = m_rotateProfileAZ[i]; }

		double scale_ramp_multX, scale_ramp_multY, scale_ramp_multZ = 1.0;

		if (i < int(m_scaleProfileAX.length())) { scale_ramp_multX = m_scaleProfileAX[i]; }
		if (i < int(m_scaleProfileAY.length())) { scale_ramp_multY = m_scaleProfileAY[i]; }
		if (i < int(m_scaleProfileAZ.length())) { scale_ramp_multZ = m_scaleProfileAZ[i]; }


		MFloatPoint p(inLocAP + xDir * step * double(i));

		double m[4][4] = {{xDir.x, xDir.y, xDir.z, 0.0}, {zDir.x, zDir.y, zDir.z, 0.0}, {yDir.x, yDir.y, yDir.z, 0.0}, {p.x, p.y, p.z, 1.0}};

		rotMatrix = m;


		if (m_orientationType == 1) {double m[4][4] = {{0.0, 1.0 , 0.0, 0.0},{ 1.0, 0.0, 0.0, 0.0},{ 0.0, 0.0, 1.0, 0.0},{ p.x, p.y, p.z, 1.0}};rotMatrix = m;}
		if (m_orientationType == 2) {double m[4][4] = {{1.0, 0.0 , 0.0, 0.0},{ 0.0, 1.0, 0.0, 0.0},{ 0.0, 0.0, 1.0, 0.0},{ p.x, p.y, p.z, 1.0}};rotMatrix = m;}
		if (m_orientationType == 3) {double m[4][4] = {{1.0, 0.0 , 0.0, 0.0},{ 0.0, 0.0, 1.0, 0.0},{ 0.0, -1.0, 0.0, 0.0},{ p.x, p.y, p.z, 1.0}};rotMatrix = m;}


		// Translation
		MFloatVector v_baseOff((m_offsetX + m_rule_off_A_X[i]) * off_ramp_multX, (m_offsetY + m_rule_off_A_Y[i]) * off_ramp_multY, (m_offsetZ + m_rule_off_A_Z[i]) * off_ramp_multZ);


		// Rotation
		double rot[3] = { (m_rotateX + m_rule_rot_A_X[i]) * (M_PI / 180.0f) * rot_ramp_multX, (m_rotateY + m_rule_rot_A_Y[i]) * (M_PI / 180.0f) * rot_ramp_multY,  (m_rotateZ + m_rule_rot_A_Z[i]) * (M_PI / 180.0f) * rot_ramp_multZ };


		// Scale
		const double scaleV[3] = { double(m_scaleX * m_rule_scl_A_X[i]) * scale_ramp_multX,  double(m_scaleY * m_rule_scl_A_Y[i]) * scale_ramp_multY,  double(m_scaleZ * m_rule_scl_A_Z[i]) * scale_ramp_multZ };


		// Random Transform
		MFloatVector v_rndOffV(m_rndOffsetXA[i], m_rndOffsetYA[i] ,m_rndOffsetZA[i]);
		// Random Rotate

		double rot_rnd[3] = {m_rndRotateXA[i] * 0.5f * ( M_PI / 180.0f ), m_rndRotateYA[i] * 0.5f * ( M_PI / 180.0f ),  m_rndRotateZA[i] * 0.5f * ( M_PI / 180.0f )};

		// Random Scale
		const double scaleV_rnd[3] = {  double(1.0+m_rndScaleXA[i]),  double(1.0+m_rndScaleYA[i]),  double(1.0+m_rndScaleZA[i]) };




		// Matrix
		MTransformationMatrix tr_mat(rotMatrix);

		status = tr_mat.addTranslation(v_baseOff, MSpace::kObject);
		CHECK_MSTATUS_AND_RETURN_IT(status);
		status = tr_mat.addTranslation(v_rndOffV, MSpace::kObject);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		status = tr_mat.setScale(scaleV, MSpace::kObject);
		CHECK_MSTATUS_AND_RETURN_IT(status);
		status = tr_mat.addScale(scaleV_rnd, MSpace::kObject);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		status = tr_mat.addRotation(rot, MTransformationMatrix::kXYZ, MSpace::kObject);
		CHECK_MSTATUS_AND_RETURN_IT(status);
		status = tr_mat.addRotation(rot_rnd, MTransformationMatrix::kXYZ, MSpace::kObject);
		CHECK_MSTATUS_AND_RETURN_IT(status);



		MMatrix outMat = tr_mat.asMatrix();

		if (m_worldSpace) { outMat *= m_inMeshMatrixArray[m_idA[i]]; }

		m_tr_matA.set(outMat, i);


	}



	return MS::kSuccess;
}