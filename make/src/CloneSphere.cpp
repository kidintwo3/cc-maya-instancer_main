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


	float offset = 2.0f / float(samples);
	float increment = M_PI * (3. - sqrt(5.0f));





	// ----------------



	float rnd = 1.0f;
	rnd = (rand() * 0.00001f) * float(m_numDup);


	m_tr_matA.clear();
	m_tr_matA.setLength(m_numDup);


	for (int i = 0; i < m_numDup; i++)
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





		// TRANSFORM

		float  y = ((i * offset) - 1) + (offset / 2);
		float r = sqrt(1 - pow(y, 2));

		float phi = ((i + int(rnd)) % m_numDup) * increment;

		float x = cos(phi) * r;
		float z = sin(phi) * r;


		//MTransformationMatrix transMatrix; // Calc trans
		MPoint p(x * (m_offsetX * off_ramp_multX), y * (m_offsetY* off_ramp_multY), z * (m_offsetZ* off_ramp_multZ));
		//MPoint p = v_baseV * 0.5;




		//
		MVector ab = p - MPoint::origin;

		MVector ab_dir = ab;
		ab_dir.normalize();

		MVector xDir = ab_dir;
		MVector yDir = xDir ^ m_firstUpVec;
		yDir.normalize();
		MVector zDir = yDir ^ xDir;
		zDir.normalize();


		//



		double m[4][4] = { { xDir.x, xDir.y, xDir.z, 0.0 },{ zDir.x, zDir.y, zDir.z, 0.0 },{ yDir.x, yDir.y, yDir.z, 0.0 },{ p.x, p.y, p.z, 1.0 } };
		MMatrix mat_rot = m;


		if (m_orientationType == 1) { double m[4][4] = { { 0.0, 1.0 , 0.0, 0.0 },{ -1.0, 0.0, 0.0, 0.0 },{ 0.0, 0.0, 1.0, 0.0 },{ p.x, p.y, p.z, 1.0 } }; mat_rot = m; }
		if (m_orientationType == 2) { double m[4][4] = { { 1.0, 0.0 , 0.0, 0.0 },{ 0.0, 1.0, 0.0, 0.0 },{ 0.0, 0.0, 1.0, 0.0 },{ p.x, p.y, p.z, 1.0 } }; mat_rot = m; }
		if (m_orientationType == 3) { double m[4][4] = { { 1.0, 0.0 , 0.0, 0.0 },{ 0.0, 0.0, 1.0, 0.0 },{ 0.0, -1.0, 0.0, 0.0 },{ p.x, p.y, p.z, 1.0 } }; mat_rot = m; }




		// Transformation matrix
		MTransformationMatrix tr_mat(mat_rot);
		MTransformationMatrix t_mat(mat_rot);


		// Rotation
		double rotation[3] = { (m_rotateX + m_rule_rot_A_X[i]) * (M_PI / 180.0f) * rot_ramp_multX, (m_rotateY + m_rule_rot_A_Y[i]) * (M_PI / 180.0f) * rot_ramp_multY,  (m_rotateZ + m_rule_rot_A_Z[i]) * (M_PI / 180.0f) * rot_ramp_multZ };


		// Scale
		const double scaleV[3] = { double(m_scaleX * m_rule_scl_A_X[i]) * scale_ramp_multX,  double(m_scaleY * m_rule_scl_A_Y[i]) * scale_ramp_multY,  double(m_scaleZ * m_rule_scl_A_Z[i]) * scale_ramp_multZ };

		// Random Transform
		MFloatVector v_rndOffV(m_rndOffsetXA[i] * off_ramp_multX, m_rndOffsetYA[i] * off_ramp_multY, m_rndOffsetZA[i] * off_ramp_multZ);
		// Random Rotate

		double rot_rnd[3] = { m_rndRotateXA[i] * 0.5f * (M_PI / 180.0f)* rot_ramp_multX, m_rndRotateYA[i] * 0.5f * (M_PI / 180.0f)* rot_ramp_multY,  m_rndRotateZA[i] * 0.5f * (M_PI / 180.0f)* rot_ramp_multZ };

		// Random Scale
		const double scaleV_rnd[3] = { double(1.0 + m_rndScaleXA[i])* scale_ramp_multX,  double(1.0 + m_rndScaleYA[i])* scale_ramp_multY,  double(1.0 + m_rndScaleZA[i])* scale_ramp_multZ };






		//status = tr_mat.addTranslation(p, MSpace::kObject);
		//CHECK_MSTATUS_AND_RETURN_IT(status);
		status = tr_mat.addTranslation(v_rndOffV, MSpace::kObject);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		//double rot_ext[4];
		//t_mat.rotation().get(rot_ext);


		//status = tr_mat.addRotation(rot_ext, MTransformationMatrix::kXYZ, MSpace::kObject);
		//CHECK_MSTATUS_AND_RETURN_IT(status);
		status = tr_mat.addRotation(rotation, MTransformationMatrix::kXYZ, MSpace::kObject);
		CHECK_MSTATUS_AND_RETURN_IT(status);
		status = tr_mat.addRotation(rot_rnd, MTransformationMatrix::kXYZ, MSpace::kObject);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		status = tr_mat.addScale(scaleV, MSpace::kObject);
		CHECK_MSTATUS_AND_RETURN_IT(status);
		status = tr_mat.addScale(scaleV_rnd, MSpace::kObject);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		MMatrix outMat = tr_mat.asMatrix();
		if (m_worldSpace) {outMat *= m_inMeshMatrixArray[m_idA[i]]; }


		m_tr_matA.set(outMat, i);

	}

	return MS::kSuccess;
}
