//
//  ClonerCircle.cpp
//  ClonerMulti
//
//  Created by Hunyadi János on 2015. 01. 03..
//  Copyright (c) 2015. Janos Hunyadi. All rights reserved.
//



#include "CloneCircle.h"


// --- Instance Circle ------
MStatus ClonerMultiThread::instanceCircle()
{

	MStatus status;

	int f = 0;
	m_tr_matA.clear();
	m_tr_matA.setLength(m_numDup);

	float radius = m_offsetX;
	float slice = 2.0f * float(M_PI) / m_instanceX;



	for (int y = 0; y < m_instanceY; y++)
	{
		for (int x = 0; x < m_instanceX; x++)
		{

			for (int z = 0; z < m_instanceZ; z++)
			{


				double off_ramp_mult = 1.0;

				if (f < int(m_offsetProfileA.length()))
				{
					off_ramp_mult = m_offsetProfileA[f];
				}


				double rot_ramp_mult = 1.0;

				if (f < int(m_rotateProfileA.length()))
				{
					rot_ramp_mult = m_rotateProfileA[f];
				}

				double scale_ramp_mult = 1.0;

				if (f < int(m_scaleProfileA.length()))
				{
					scale_ramp_mult = m_scaleProfileA[f];
				}






				float angle = slice * x;
				float newX = radius * cos(angle);
				float newY = radius * sin(angle);
				//double newZ = radius * sin(angle);


				// Translation
				MFloatVector v_baseOffV(newX, m_offsetY * y * off_ramp_mult, m_offsetZ * newY * off_ramp_mult);


				double rot[3] = { m_rotateX * 0.5f * (M_PI / 180.0f) * rot_ramp_mult, (m_rotateY * 0.5f * (M_PI / 180.0f)) - angle * rot_ramp_mult,  m_rotateZ * 0.5f * (M_PI / 180.0f)* rot_ramp_mult };


				// Scale
				const double scaleV[3] = { double(m_scaleX)*scale_ramp_mult,  double(m_scaleY)*scale_ramp_mult,  double(m_scaleZ)*scale_ramp_mult };





				// Random Transform
				MFloatVector v_rndOffV(m_rndOffsetXA[f] * off_ramp_mult, m_rndOffsetYA[f] * off_ramp_mult, m_rndOffsetZA[f] * off_ramp_mult);
				// Random Rotate

				double rot_rnd[3] = { m_rndRotateXA[f] * 0.5f * (M_PI / 180.0f)* rot_ramp_mult, m_rndRotateYA[f] * 0.5f * (M_PI / 180.0f)* rot_ramp_mult,  m_rndRotateZA[f] * 0.5f * (M_PI / 180.0f)* rot_ramp_mult };

				// Random Scale
				const double scaleV_rnd[3] = { double(1.0 + m_rndScaleXA[f])*scale_ramp_mult,  double(1.0 + m_rndScaleYA[f])*scale_ramp_mult,  double(1.0 + m_rndScaleZA[f])*scale_ramp_mult };




				// Matrix
				MTransformationMatrix tr_mat;

				status = tr_mat.setTranslation(v_baseOffV, MSpace::kObject);
				CHECK_MSTATUS_AND_RETURN_IT(status);
				status = tr_mat.addTranslation(v_rndOffV, MSpace::kObject);
				CHECK_MSTATUS_AND_RETURN_IT(status);

				status = tr_mat.setScale(scaleV, MSpace::kObject);
				CHECK_MSTATUS_AND_RETURN_IT(status);
				status = tr_mat.addScale(scaleV_rnd, MSpace::kObject);
				CHECK_MSTATUS_AND_RETURN_IT(status);

				status = tr_mat.setRotation(rot, MTransformationMatrix::kXYZ);
				CHECK_MSTATUS_AND_RETURN_IT(status);
				status = tr_mat.addRotation(rot_rnd, MTransformationMatrix::kXYZ, MSpace::kObject);
				CHECK_MSTATUS_AND_RETURN_IT(status);


				m_tr_matA.set(tr_mat.asMatrix(), f);

				f += 1;
			}


		}
	}



	return MS::kSuccess;

}