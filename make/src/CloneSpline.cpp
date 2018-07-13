//
//  CloneSpline.h
//  ClonerMulti
//
//  Created by Hunyadi János on 2015. 01. 03..
//  Copyright (c) 2015. Janos Hunyadi. All rights reserved.
//



#include "CloneSpline.h"


// --- Instance Spline ------
MStatus ClonerMultiThread::instanceSpline()
{


	MStatus status;

	m_tr_matA.clear();
	m_tr_matA.setLength(m_numDup);

	MVector currentNormal = m_firstUpVec;
	currentNormal.normalize();

	MVector prevNormal;
	MMatrix rotMatrix, rotMatrix_orient;


	// curve data
	MFnNurbsCurve curveFn(m_inCurve, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);



	if (curveFn.length() <= 0.0)
	{
		return MStatus::kSuccess;
	}


	int numdup_divider = m_numDup - 1;
	if (numdup_divider < 1)
	{
		numdup_divider = 1;
	}

	// Set up a temp numdup divider just for the curve
	double length_curve = curveFn.length();
	double length_section = (length_curve / double(numdup_divider));


	double offsetV = m_offsetZ;
	if (offsetV == 0.0) { offsetV = 0.001; }


	int rc = 0;

	for (unsigned int i = 0; i < m_numDup; i++)
	{



		double off_ramp_mult = 1.0;

		if (i < int(m_offsetProfileA.length()))
		{
			off_ramp_mult = m_offsetProfileA[i];
		}



		double scale_ramp_mult = 1.0;

		if (i < int(m_scaleProfileA.length()))
		{
			scale_ramp_mult = m_scaleProfileA[i];
		}


		double rot_ramp_mult = 1.0;

		if (i < int(m_rotateProfileA.length()))
		{
			rot_ramp_mult = m_rotateProfileA[i];
		}





		double tempLength = double(i) * length_section;
		tempLength += offsetV;

		//if (i=0)
		//{
		//	tempLength += (m_offsetZ + m_rule_off_A_Z[rc]) * off_ramp_mult;
		//}




		if (m_loopOffset)
		{



			double  timesLength = (offsetV / length_curve);

			int  timesLengthFloor = 0;

			if (offsetV > 0)
			{
				timesLengthFloor = int(floor(timesLength));
			}

			else
			{
				timesLengthFloor = int(ceil(timesLength));
			}



			offsetV -= (length_curve*double(timesLengthFloor));


			if (tempLength < 0)
			{
				tempLength = length_curve + tempLength;
			}

			else if (tempLength > length_curve)
			{
				tempLength -= length_curve;

			}
		}


		// Calculate matrix
		MPoint p;
		MVector tan, closest_normal;
		MPoint closestPoint;

		double param = curveFn.findParamFromLength(tempLength);

		curveFn.getPointAtParam(param, p, MSpace::kObject);

		tan = curveFn.tangent(param, MSpace::kObject);
		tan.normalize();


		MVector cross1 = currentNormal ^ tan;
		cross1.normalize();

		MVector cross2 = tan ^ cross1;

		cross2.normalize();
		currentNormal = cross2;


		double m[4][4] = { { tan.x, tan.y , tan.z, 0.0 },
		{ cross2.x, cross2.y, cross2.z, 0.0 },
		{ cross1.x, cross1.y, cross1.z, 0.0 },
		{ p.x, p.y, p.z, 1.0 } };

		rotMatrix = m;


		if (m_orientCurveToRefGeo)
		{
			if (!m_refMesh.isNull())
			{
				// curve data
				MFnMesh meshFn(m_refMesh, &status);
				CHECK_MSTATUS_AND_RETURN_IT(status);

				if (p_refMeshSmooth.isConnected())
				{
					if (m_smoothMeshPreview)
					{
						status = meshFn.setObject(m_refMeshSmooth);
						CHECK_MSTATUS_AND_RETURN_IT(status);
					}
				}


				p *= m_curveTrMat;

				int closestPolygon;
				status = meshFn.getClosestPointAndNormal(p, closestPoint, closest_normal, MSpace::kObject, &closestPolygon);


				if (status)
				{

					p = closestPoint * m_curveTrMat.inverse();

					MVector cross1 = closest_normal ^ tan;
					cross1.normalize();

					MVector cross2 = closest_normal ^ cross1;
					cross2.normalize();


					double m[4][4] = { { cross2.x, cross2.y , cross2.z, 0.0 },
					{ closest_normal.x, closest_normal.y, closest_normal.z, 0.0 },
					{ cross1.x, cross1.y, cross1.z, 0.0 },
					{ p.x, p.y, p.z, 1.0 } };

					rotMatrix = m;
				}

			}

		}

		//



		if (m_orientationType == 1) { double m[4][4] = { { 0.0, 1.0 , 0.0, 0.0 },{ 1.0, 0.0, 0.0, 0.0 },{ 0.0, 0.0, 1.0, 0.0 },{ p.x, p.y, p.z, 1.0 } }; rotMatrix = m; }
		if (m_orientationType == 2) { double m[4][4] = { { 1.0, 0.0 , 0.0, 0.0 },{ 0.0, 1.0, 0.0, 0.0 },{ 0.0, 0.0, 1.0, 0.0 },{ p.x, p.y, p.z, 1.0 } }; rotMatrix = m; }
		if (m_orientationType == 3) { double m[4][4] = { { 1.0, 0.0 , 0.0, 0.0 },{ 0.0, 0.0, 1.0, 0.0 },{ 0.0, -1.0, 0.0, 0.0 },{ p.x, p.y, p.z, 1.0 } }; rotMatrix = m; }





		// Translation 
		MFloatVector v_baseOff((m_offsetX + m_rule_off_A_X[rc]) * off_ramp_mult, (m_offsetY + m_rule_off_A_Y[rc])  * off_ramp_mult, m_rule_off_A_Z[rc] * off_ramp_mult);


		// Rotation
		double rot[3] = { (m_rotateX + m_rule_rot_A_X[rc]) * 0.5f * (M_PI / 180.0f) * rot_ramp_mult, (m_rotateY + m_rule_rot_A_Y[rc]) * 0.5f * (M_PI / 180.0f) * rot_ramp_mult,  (m_rotateZ + m_rule_rot_A_Z[rc]) * 0.5f * (M_PI / 180.0f) * rot_ramp_mult };

		// Scale
		const double scaleV[3] = { double(m_scaleX * m_rule_scl_A_X[rc])  * scale_ramp_mult,  double(m_scaleY * m_rule_scl_A_Y[rc])  * scale_ramp_mult,  double(m_scaleZ *  m_rule_scl_A_Z[rc])  * scale_ramp_mult };

		// Random Transform
		MFloatVector v_rndOffV(m_rndOffsetXA[i] * off_ramp_mult, m_rndOffsetYA[i] * off_ramp_mult, m_rndOffsetZA[i] * off_ramp_mult);
		// Random Rotate

		double rot_rnd[3] = { m_rndRotateXA[i] * 0.5f * (M_PI / 180.0f) * rot_ramp_mult, m_rndRotateYA[i] * 0.5f * (M_PI / 180.0f) * rot_ramp_mult,  m_rndRotateZA[i] * 0.5f * (M_PI / 180.0f) * rot_ramp_mult };

		// Random Scale
		const double scaleV_rnd[3] = { double(1.0 + m_rndScaleXA[i]) * scale_ramp_mult,  double(1.0 + m_rndScaleYA[i]) * scale_ramp_mult,  double(1.0 + m_rndScaleZA[i]) * scale_ramp_mult };





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


		m_tr_matA.set(tr_mat.asMatrix() * m_curveTrMat, i);


		rc += 1;


	}



	return MS::kSuccess;

}


