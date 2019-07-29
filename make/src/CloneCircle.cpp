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

	int rc = 0;

	MFnMesh meshFn;

	if (p_refMesh.isConnected())
	{

		status = meshFn.setObject(m_refMesh);

		// Normal mesh data
		MFnMesh meshFn(m_refMesh, &status);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		// Smooth mesh data
		if (p_refMeshSmooth.isConnected())
		{
			if (m_smoothMeshPreview)
			{
				status = meshFn.setObject(m_refMeshSmooth);
				CHECK_MSTATUS_AND_RETURN_IT(status);
			}
		}
	}



	for (int y = 0; y < m_instanceY; y++)
	{
		for (int x = 0; x < m_instanceX; x++)
		{

			for (int z = 0; z < m_instanceZ; z++)
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
			




				radius += m_rule_off_A_X[rc];
				radius *= off_ramp_multX;



				float angle = slice * x;

				angle += m_circleRotate * (M_PI / 180.0f);

				float newX = radius * cos(angle);
				float newY = radius * sin(angle);
				//double newZ = radius * sin(angle);


				// Translation
				MPoint p_baseOff(newX, (m_offsetY * y) * off_ramp_multY, (m_offsetZ * newY) * off_ramp_multZ);
				
				
				MMatrix rotMatrix;
				MPoint p;



				if (m_orientCurveToRefGeo)
				{
					if (!m_refMesh.isNull())
					{



						MVector  closest_normal;

						

						MMatrix faceRotmat;

						if (p_refLoc.isConnected())
						{

							/*worldspaceP = p_baseOff * m_refLoc_posMat;*/
							MPoint worldspaceP = MPoint::origin * m_refLoc_posMat;

							status = meshFn.getClosestPointAndNormal(worldspaceP, p, closest_normal, MSpace::kObject);


							if (status)
							{

								MVector cross1 = closest_normal ^ m_firstUpVec;
								cross1.normalize();

								MVector cross2 = closest_normal ^ cross1;
								cross2.normalize();

								double m[4][4] = { { cross2.x, cross2.y , cross2.z, 0.0 },
								{ closest_normal.x, closest_normal.y, closest_normal.z, 0.0 },
								{ cross1.x, cross1.y, cross1.z, 0.0 },
								{ p.x, p.y, p.z, 1.0 } };

								faceRotmat = m;

							}

						}


						p_baseOff *= faceRotmat;

						status = meshFn.getClosestPointAndNormal(p_baseOff, p, closest_normal, MSpace::kObject);

						if (status)
						{

							MVector cross1 = closest_normal ^ m_firstUpVec;
							cross1.normalize();

							MVector cross2 = closest_normal ^ cross1;
							cross2.normalize();


							p += closest_normal * m_offsetY;

							double m[4][4] = { { cross2.x, cross2.y , cross2.z, 0.0 },
							{ closest_normal.x, closest_normal.y, closest_normal.z, 0.0 },
							{ cross1.x, cross1.y, cross1.z, 0.0 },
							{ p.x, p.y, p.z, 1.0 } };

							rotMatrix = m;

						}

					}

				}



				//if (m_orientationType == 1) { double m[4][4] = { { 0.0, 1.0 , 0.0, 0.0 },{ 1.0, 0.0, 0.0, 0.0 },{ 0.0, 0.0, 1.0, 0.0 },{ p.x, p.y, p.z, 1.0 } }; rotMatrix = m; }
				//if (m_orientationType == 2) { double m[4][4] = { { 1.0, 0.0 , 0.0, 0.0 },{ 0.0, 1.0, 0.0, 0.0 },{ 0.0, 0.0, 1.0, 0.0 },{ p.x, p.y, p.z, 1.0 } }; rotMatrix = m; }
				//if (m_orientationType == 3) { double m[4][4] = { { 1.0, 0.0 , 0.0, 0.0 },{ 0.0, 0.0, 1.0, 0.0 },{ 0.0, -1.0, 0.0, 0.0 },{ p.x, p.y, p.z, 1.0 } }; rotMatrix = m; }


				//double rot[3] = { m_rotateX * 0.5f * (M_PI / 180.0f) * rot_ramp_mult, (m_rotateY * 0.5f * (M_PI / 180.0f)) - angle * rot_ramp_mult,  m_rotateZ * 0.5f * (M_PI / 180.0f)* rot_ramp_mult };


				//// Scale
				//const double scaleV[3] = { double(m_scaleX)*scale_ramp_mult,  double(m_scaleY)*scale_ramp_mult,  double(m_scaleZ)*scale_ramp_mult };





				//// Random Transform
				//MFloatVector v_rndOffV(m_rndOffsetXA[f] * off_ramp_mult, m_rndOffsetYA[f] * off_ramp_mult, m_rndOffsetZA[f] * off_ramp_mult);
				//// Random Rotate

				//double rot_rnd[3] = { m_rndRotateXA[f] * 0.5f * (M_PI / 180.0f)* rot_ramp_mult, m_rndRotateYA[f] * 0.5f * (M_PI / 180.0f)* rot_ramp_mult,  m_rndRotateZA[f] * 0.5f * (M_PI / 180.0f)* rot_ramp_mult };

				//// Random Scale
				//const double scaleV_rnd[3] = { double(1.0 + m_rndScaleXA[f])*scale_ramp_mult,  double(1.0 + m_rndScaleYA[f])*scale_ramp_mult,  double(1.0 + m_rndScaleZA[f])*scale_ramp_mult };




				//// Matrix
				//MTransformationMatrix tr_mat(rotMatrix);;

				//status = tr_mat.addTranslation(p_baseOff, MSpace::kObject);
				//CHECK_MSTATUS_AND_RETURN_IT(status);
				//status = tr_mat.addTranslation(v_rndOffV, MSpace::kObject);
				//CHECK_MSTATUS_AND_RETURN_IT(status);

				//status = tr_mat.addScale(scaleV, MSpace::kObject);
				//CHECK_MSTATUS_AND_RETURN_IT(status);
				//status = tr_mat.addScale(scaleV_rnd, MSpace::kObject);
				//CHECK_MSTATUS_AND_RETURN_IT(status);

				//status = tr_mat.addRotation(rot, MTransformationMatrix::kXYZ, MSpace::kObject);
				//CHECK_MSTATUS_AND_RETURN_IT(status);
				//status = tr_mat.addRotation(rot_rnd, MTransformationMatrix::kXYZ, MSpace::kObject);
				//CHECK_MSTATUS_AND_RETURN_IT(status);


				if (m_orientationType == 1) { double m[4][4] = { { 0.0, 1.0 , 0.0, 0.0 },{ 1.0, 0.0, 0.0, 0.0 },{ 0.0, 0.0, 1.0, 0.0 },{ p.x, p.y, p.z, 1.0 } }; rotMatrix = m; }
				if (m_orientationType == 2) { double m[4][4] = { { 1.0, 0.0 , 0.0, 0.0 },{ 0.0, 1.0, 0.0, 0.0 },{ 0.0, 0.0, 1.0, 0.0 },{ p.x, p.y, p.z, 1.0 } }; rotMatrix = m; }
				if (m_orientationType == 3) { double m[4][4] = { { 1.0, 0.0 , 0.0, 0.0 },{ 0.0, 0.0, 1.0, 0.0 },{ 0.0, -1.0, 0.0, 0.0 },{ p.x, p.y, p.z, 1.0 } }; rotMatrix = m; }





				// Translation 
				/*MFloatVector v_baseOff((m_offsetX + m_rule_off_A_X[rc]) * off_ramp_mult, (m_offsetY + m_rule_off_A_Y[rc])  * off_ramp_mult, m_rule_off_A_Z[rc] * off_ramp_mult);*/
				p_baseOff += MFloatVector((m_rule_off_A_X[f] * off_ramp_multX), (m_rule_off_A_Y[f] * off_ramp_multY), (m_rule_off_A_Z[f] * off_ramp_multZ));


				// Rotation
				double rot[3] = { (m_rotateX + m_rule_rot_A_X[rc])  * (M_PI / 180.0f) * rot_ramp_multX, (m_rotateY + m_rule_rot_A_Y[rc])  * (M_PI / 180.0f) - angle * rot_ramp_multY,  (m_rotateZ + m_rule_rot_A_Z[rc]) * (M_PI / 180.0f) * rot_ramp_multZ };


				//double rot[3] = { m_rotateX * 0.5f * (M_PI / 180.0f) * rot_ramp_mult, (m_rotateY * 0.5f * (M_PI / 180.0f)) - angle * rot_ramp_mult,  m_rotateZ * 0.5f * (M_PI / 180.0f)* rot_ramp_mult };



				// Scale
				const double scaleV[3] = { double(m_scaleX * m_rule_scl_A_X[rc])  * (scale_ramp_multX),  double(m_scaleY * m_rule_scl_A_Y[rc])  * (scale_ramp_multY),  double(m_scaleZ *  m_rule_scl_A_Z[rc])  * (scale_ramp_multZ) };

				// Random Transform
				MFloatVector v_rndOffV(m_rndOffsetXA[f] * off_ramp_multX, m_rndOffsetYA[f] * off_ramp_multY, m_rndOffsetZA[f] * off_ramp_multZ);
				// Random Rotate

				double rot_rnd[3] = { m_rndRotateXA[f] * 0.5f * (M_PI / 180.0f) * rot_ramp_multX, m_rndRotateYA[f] * 0.5f * (M_PI / 180.0f) * rot_ramp_multY,  m_rndRotateZA[f] * 0.5f * (M_PI / 180.0f) * rot_ramp_multZ };

				// Random Scale
				const double scaleV_rnd[3] = { double(1.0 + m_rndScaleXA[f]) * (scale_ramp_multX),  double(1.0 + m_rndScaleYA[f]) * (scale_ramp_multY),  double(1.0 + m_rndScaleZA[f]) * (scale_ramp_multZ) };





				// Matrix
				MTransformationMatrix tr_mat(rotMatrix);


				if (!m_orientCurveToRefGeo)
				{

					status = tr_mat.setTranslation(p_baseOff, MSpace::kObject);
					CHECK_MSTATUS_AND_RETURN_IT(status);
				}

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

				if (m_worldSpace) { outMat *= m_inMeshMatrixArray[m_idA[f]]; }

				m_tr_matA.set(outMat, f);


				f += 1;
				rc += 1;
			}


		}

		

	}



	return MS::kSuccess;

}