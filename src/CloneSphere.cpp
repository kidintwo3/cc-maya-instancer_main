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

	float rnd = 1.0f;
	rnd = (rand() * 0.00001f) * float(m_numDup);


	float rad = m_offsetX;

	if (rad < 0.0f)
	{
		rad = 0.0001f;
	}

	float offset = 2.0f / float(m_numDup);
	float increment = float(M_PI) * (3.0f - sqrt(5.0f));

	m_tr_matA.clear();
	m_tr_matA.setLength(m_numDup);


	for (int i = 0; i < m_numDup; i++)
	{

		// TRANSFORM
		float y = ((float(i) * offset) - 1.0f) + (offset / 2.0f);

		float r = sqrt(1.0f - pow(y, 2.0f));
		float phi = (int(float(i) + rnd) % m_numDup) * increment;

		float x = cos(phi) * r;
		float z = sin(phi) * r;

		MPoint p;

		MTransformationMatrix transMatrix; // Calc trans
		MFloatVector v_baseV( x * m_offsetX, y * m_offsetY ,z * m_offsetZ );

		p = v_baseV * 2.0f;

		// ROTATION base orientation
		MFloatVector upVec(0.0f, 1.0f, 0.0f);
		MFloatVector ab = p;
		MFloatVector ab_dir = ab;
		ab_dir.normalize();
		MFloatVector xDir = ab_dir;
		MFloatVector yDir = xDir ^ upVec;
		yDir.normalize();
		MFloatVector zDir = xDir ^ yDir;
		zDir.normalize();
		float m[4][4] = {{xDir.x, xDir.y, xDir.z, 0.0}, {yDir.x, yDir.y, yDir.z, 0.0}, {zDir.x, zDir.y, zDir.z, 0.0}, {0.0, 0.0, 0.0, 1.0}};

		MMatrix mat = m;


		// Transformation matrix
		MTransformationMatrix tr_mat(mat);


		// Rotation
		double rotation[3] = {m_rotateX * 0.5f * ( M_PI / 180.0f ), m_rotateY * 0.5f * ( M_PI / 180.0f ),  m_rotateZ * 0.5f * ( M_PI / 180.0f )};


		// Scale
		const double scaleV[3] = {  double(m_scaleX),  double(m_scaleY),  double(m_scaleZ) };

		status = tr_mat.addTranslation(p, MSpace::kObject);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		status = tr_mat.addRotation(rotation,  MTransformationMatrix::kXYZ, MSpace::kObject);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		status = tr_mat.addScale(scaleV, MSpace::kObject);
		CHECK_MSTATUS_AND_RETURN_IT(status);


		m_tr_matA.set(tr_mat.asMatrix(), i);

	}

	return MS::kSuccess;
}
