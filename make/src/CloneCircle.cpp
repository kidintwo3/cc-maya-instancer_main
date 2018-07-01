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
				MPoint p_baseOff(newX, m_offsetY * y * off_ramp_mult, m_offsetZ * newY * off_ramp_mult);

				MMatrix rotMatrix_orient;


				if (m_orientCurveToRefGeo)
				{
					if (!m_refMesh.isNull())
					{


						// curve data
						MFnMesh meshFn(m_refMesh, &status);
						CHECK_MSTATUS_AND_RETURN_IT(status);

						MPoint p;
						MVector  tan;

						MPoint worldspaceP = p_baseOff;

						if (p_inLocA.isConnected())
						{

							worldspaceP = p_baseOff * m_inLocA_posMat;
						}



						status = meshFn.getClosestPointAndNormal(worldspaceP, p, tan, MSpace::kObject);

						if (status)
						{
							

							MVector currentNormal(0.0, 1.0, 0.0);

							MVector cross1 = currentNormal^tan;
							cross1.normalize();

							MVector cross2 = tan^cross1;
							cross2.normalize();


							double m[4][4] = { { cross2.x, cross2.y , cross2.z, 0 },
							{ cross1.x, cross1.y , cross1.z, 0 },
							{ -tan.x, -tan.y , -tan.z, 0 },
							{ p.x, p.y , p.z, 1.0 } };

							rotMatrix_orient = m;

						}

					}

				}





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

				status = tr_mat.setTranslation(p_baseOff, MSpace::kObject);
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


				MMatrix finMat = tr_mat.asMatrix();

				if (m_orientCurveToRefGeo)
				{
					if (!m_refMesh.isNull())
					{
						MTransformationMatrix tr_mat_orient(rotMatrix_orient);

			/*			status = tr_mat_orient.addTranslation(p_baseOff, MSpace::kObject);
						CHECK_MSTATUS_AND_RETURN_IT(status);
*/
						status = tr_mat_orient.addTranslation(v_rndOffV, MSpace::kObject);
						CHECK_MSTATUS_AND_RETURN_IT(status);

						status = tr_mat_orient.addScale(scaleV, MSpace::kObject);
						CHECK_MSTATUS_AND_RETURN_IT(status);
						status = tr_mat_orient.addScale(scaleV_rnd, MSpace::kObject);
						CHECK_MSTATUS_AND_RETURN_IT(status);

						status = tr_mat_orient.addRotation(rot, MTransformationMatrix::kXYZ, MSpace::kObject);
						CHECK_MSTATUS_AND_RETURN_IT(status);
						status = tr_mat_orient.addRotation(rot_rnd, MTransformationMatrix::kXYZ, MSpace::kObject);
						CHECK_MSTATUS_AND_RETURN_IT(status);

						finMat = tr_mat_orient.asMatrix();
					}
				}

				m_tr_matA.set(finMat, f);

				f += 1;
			}


		}
	}



	return MS::kSuccess;

}