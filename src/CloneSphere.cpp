//
//  CloneAtoB.h
//  ClonerMulti
//
//  Created by Hunyadi János on 2015. 01. 03..
//  Copyright (c) 2015. Janos Hunyadi. All rights reserved.
//


#include "CloneSphere.h"

// --- Instance Sphere ------
MStatus ClonerMultiThread::instanceFibonacciSphere()
{

	MStatus status;


	bool randomize = false;


	float samples = float(m_numDup);


	float offset = 2.0f/float(samples);
	float increment = M_PI * (3. - sqrt(5.0f));





	// ----------------



	float rnd = 1.0f;
	rnd = (rand() * 0.00001f) * float(m_numDup);


	m_tr_matA.clear();
	m_tr_matA.setLength(m_numDup);


	for (int i = 0; i < m_numDup; i++)
	{

		// TRANSFORM

		float  y = ((i * offset) - 1) + (offset / 2);
		float r = sqrt(1 - pow(y,2));

		float phi = ((i + int(rnd)) % m_numDup) * increment;

		float x = cos(phi) * r;
		float z = sin(phi) * r;


		MTransformationMatrix transMatrix; // Calc trans
		MFloatVector v_baseV( x * m_offsetX, y * m_offsetY ,z * m_offsetZ );

		MPoint p = v_baseV * 0.5;

		// ROTATION base orientation
		MFloatVector ab = p;
		MFloatVector normal = ab-MFloatPoint::origin;
		normal.normalize();

		MFloatVector yDir = normal ^ m_firstUpVec;
		yDir.normalize();
		MFloatVector zDir = ab ^ yDir;
		zDir.normalize();

		double m_rot[4][4] = {{yDir.x, yDir.y, yDir.z, 0.0}, {normal.x, normal.y, normal.z, 0.0}, {zDir.x, zDir.y, zDir.z, 0.0}, {0.0, 0.0, 0.0, 1.0}};
		double m[4][4] = {{1.0, 0.0 , 0.0, 0.0},{ 0.0, 1.0, 0.0, 0.0},{ 0.0, 0.0, 1.0, 0.0},{ p.x, p.y, p.z, 1.0}};


		MMatrix mat_rot = m_rot;

		if (m_orientationType == 1) {double m[4][4] = {{0.0, 1.0 , 0.0, 0.0},{ 1.0, 0.0, 0.0, 0.0},{ 0.0, 0.0, 1.0, 0.0},{ p.x, p.y, p.z, 1.0}};mat_rot = m;}
		if (m_orientationType == 2) {double m[4][4] = {{1.0, 0.0 , 0.0, 0.0},{ 0.0, 1.0, 0.0, 0.0},{ 0.0, 0.0, 1.0, 0.0},{ p.x, p.y, p.z, 1.0}};mat_rot = m;}
		if (m_orientationType == 3) {double m[4][4] = {{1.0, 0.0 , 0.0, 0.0},{ 0.0, 0.0, 1.0, 0.0},{ 0.0, -1.0, 0.0, 0.0},{ p.x, p.y, p.z, 1.0}};mat_rot = m;}

		MMatrix mat = m;
		

		// Transformation matrix
		MTransformationMatrix tr_mat(mat);
		MTransformationMatrix t_mat(mat_rot);


		// Rotation
		double rotation[3] = {m_rotateX * 0.5f * ( M_PI / 180.0f ), m_rotateY * 0.5f * ( M_PI / 180.0f ),  m_rotateZ * 0.5f * ( M_PI / 180.0f )};


		// Scale
		const double scaleV[3] = {  double(m_scaleX),  double(m_scaleY),  double(m_scaleZ) };

		// Random Transform
		MFloatVector v_rndOffV(m_rndOffsetXA[i], m_rndOffsetYA[i] ,m_rndOffsetZA[i]);
		// Random Rotate

		double rot_rnd[3] = {m_rndRotateXA[i] * 0.5f * ( M_PI / 180.0f ), m_rndRotateYA[i] * 0.5f * ( M_PI / 180.0f ),  m_rndRotateZA[i] * 0.5f * ( M_PI / 180.0f )};

		// Random Scale
		const double scaleV_rnd[3] = {  double(1.0+m_rndScaleXA[i]),  double(1.0+m_rndScaleYA[i]),  double(1.0+m_rndScaleZA[i]) };




		status = tr_mat.addTranslation(p, MSpace::kObject);
		CHECK_MSTATUS_AND_RETURN_IT(status);
		status = tr_mat.addTranslation(v_rndOffV, MSpace::kObject);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		double rot_ext[4];
		t_mat.rotation().get(rot_ext);


		status = tr_mat.addRotation(rot_ext,  MTransformationMatrix::kXYZ, MSpace::kObject);
		CHECK_MSTATUS_AND_RETURN_IT(status);
		status = tr_mat.addRotation(rotation,  MTransformationMatrix::kXYZ, MSpace::kObject);
		CHECK_MSTATUS_AND_RETURN_IT(status);
		status = tr_mat.addRotation(rot_rnd, MTransformationMatrix::kXYZ, MSpace::kObject);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		status = tr_mat.addScale(scaleV, MSpace::kObject);
		CHECK_MSTATUS_AND_RETURN_IT(status);
		status = tr_mat.addScale(scaleV_rnd, MSpace::kObject);
		CHECK_MSTATUS_AND_RETURN_IT(status);


		m_tr_matA.set(tr_mat.asMatrix(), i);

	}

	return MS::kSuccess;
}
