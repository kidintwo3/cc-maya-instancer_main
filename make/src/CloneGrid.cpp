//
//  ClonerGrid.h
//  ClonerMulti
//
//  Created by Hunyadi János on 2015. 01. 03..
//  Copyright (c) 2015. Janos Hunyadi. All rights reserved.
//



#include "CloneGrid.h"


// --- Instance Grid ------
MStatus ClonerMultiThread::instanceGrid()
{


	MStatus status;

	int f = 0;
	m_tr_matA.clear();
	m_tr_matA.setLength(m_numDup);






	MTransformationMatrix transTMatrix;

	for (int z = 0; z < m_instanceZ; z++)
	{
		for (int y = 0; y < m_instanceY; y++)
		{
			for (int x = 0; x < m_instanceX; x++)
			{



				double off_ramp_multX, off_ramp_multY, off_ramp_multZ = 1.0;

				if (f < int(m_offsetProfileAX.length())) { off_ramp_multX = m_offsetProfileAX[f]; }
				if (f < int(m_offsetProfileAY.length())) { off_ramp_multY = m_offsetProfileAY[f]; }
				if (f < int(m_offsetProfileAZ.length())) { off_ramp_multZ = m_offsetProfileAZ[f]; }

				double rot_ramp_multX, rot_ramp_multY, rot_ramp_multZ = 1.0;

				if (f < int(m_rotateProfileAX.length())) { rot_ramp_multX = m_rotateProfileAX[f]; }
				if (f < int(m_rotateProfileAY.length())) { rot_ramp_multY = m_rotateProfileAY[f]; }
				if (f < int(m_rotateProfileAZ.length())) { rot_ramp_multZ = m_rotateProfileAZ[f]; }

				double scale_ramp_multX, scale_ramp_multY, scale_ramp_multZ = 1.0;

				if (f < int(m_scaleProfileAX.length())) { scale_ramp_multX = m_scaleProfileAX[f]; }
				if (f < int(m_scaleProfileAY.length())) { scale_ramp_multY = m_scaleProfileAY[f]; }
				if (f < int(m_scaleProfileAZ.length())) { scale_ramp_multZ = m_scaleProfileAZ[f]; }
				


				// Translation
				MFloatVector v_baseOffV((m_offsetX * float(x) + m_rule_off_A_X[f]) * off_ramp_multX, (m_offsetY * float(y) + m_rule_off_A_Y[f]) * off_ramp_multY, (m_offsetZ * float(z) + m_rule_off_A_Z[f]) *off_ramp_multZ);


				// Rotation
				double rot[3] = { ((m_rotateX  ) + m_rule_rot_A_X[f]) * (M_PI / 180.0f) * rot_ramp_multX , ((m_rotateY  ) + m_rule_rot_A_Y[f]) * (M_PI / 180.0f) * rot_ramp_multY,  ((m_rotateZ  ) + m_rule_rot_A_Z[f]) * (M_PI / 180.0f) * rot_ramp_multZ };

				// Scale
				const double scaleV[3] = { double(m_scaleX * m_rule_scl_A_X[f])* (scale_ramp_multX),  double(m_scaleY * m_rule_scl_A_X[f])* (scale_ramp_multY),  double(m_scaleZ * m_rule_scl_A_Z[f])* (scale_ramp_multZ) };





				// Random Transform
				MFloatVector v_rndOffV(m_rndOffsetXA[f] * off_ramp_multX, m_rndOffsetYA[f] * off_ramp_multY, m_rndOffsetZA[f] * off_ramp_multZ);
				// Random Rotate

				double rot_rnd[3] = { m_rndRotateXA[f] * 0.5f * (M_PI / 180.0f)* rot_ramp_multX, m_rndRotateYA[f] * 0.5f * (M_PI / 180.0f)* rot_ramp_multY,  m_rndRotateZA[f] * 0.5f * (M_PI / 180.0f)* rot_ramp_multZ };

				// Random Scale
				const double scaleV_rnd[3] = { double(1.0 + m_rndScaleXA[f])* (scale_ramp_multX),  double(1.0 + m_rndScaleYA[f])* (scale_ramp_multY),  double(1.0 + m_rndScaleZA[f]) * (scale_ramp_multZ) };



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


				MMatrix outMat = tr_mat.asMatrix();

				if (m_worldSpace) { outMat *= m_inMeshMatrixArray[m_idA[f]]; }

				m_tr_matA.set(outMat, f);

				f += 1;


			}
		}
	}


	return MS::kSuccess;

}