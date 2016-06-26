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

				// Translation
				MFloatVector v_baseOffV((m_offsetX * float(x)), (m_offsetY * float(y)), (m_offsetZ * float(z))  );

				// Rotation

				double rot[3] = {m_rotateX * 0.5f * ( M_PI / 180.0f ), m_rotateY * 0.5f * ( M_PI / 180.0f ),  m_rotateZ * 0.5f * ( M_PI / 180.0f )};

				// Scale
				const double scaleV[3] = {  double(m_scaleX),  double(m_scaleY),  double(m_scaleZ) };





				// Random Transform
				MFloatVector v_rndOffV(m_rndOffsetXA[f], m_rndOffsetYA[f] ,m_rndOffsetZA[f]);
				// Random Rotate

				double rot_rnd[3] = {m_rndRotateXA[f] * 0.5f * ( M_PI / 180.0f ), m_rndRotateYA[f] * 0.5f * ( M_PI / 180.0f ),  m_rndRotateZA[f] * 0.5f * ( M_PI / 180.0f )};

			// Random Scale
			const double scaleV_rnd[3] = {  double(1.0+m_rndScaleXA[f]),  double(1.0+m_rndScaleYA[f]),  double(1.0+m_rndScaleZA[f]) };



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