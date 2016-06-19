//
//  CloneMesh.h
//  ClonerMulti
//
//  Created by Hunyadi János on 2015. 01. 03..
//  Copyright (c) 2015. Janos Hunyadi. All rights reserved.
//



#include "CloneMesh.h"

MStatus ClonerMultiThread::overrideInstanceOnMeshSettings()
{
	// Override based on scatter types

	MStatus status;

	// UV
	if (m_scatterType == 0)
	{

		// Clear arrays
		mesh_pA.clear();
		mesh_uTA.clear();
		mesh_vTA.clear();
		mesh_nA.clear();

		float stepU = 1.0f / float(m_instanceY-1);
		float stepV = 1.0f / float(m_instanceX-1);

		float uC = 0.0f;
		float vC = 0.0f;

		MFloatArray uA;
		MFloatArray vA;

		for (int v = 0; v < m_instanceX; v++)
		{

			for (int u = 0; u < m_instanceY; u++)
			{

				uA.append(uC);
				vA.append(vC);

				uC += stepU;
			}

			vC += stepV;

			uC = 0.0f;
		}

		if (uA.length() != m_numDup)
		{
			return MStatus::kFailure;
		}


		// Iterate trough the mesh and find points
		MItMeshPolygon itPoly(m_refMesh);
		MVector normal,uTangent,vTangent;
		MPoint pt;


		for ( itPoly.reset() ; !itPoly.isDone() ; itPoly.next() )
		{

			for (int i = 0; i < uA.length(); i++)
			{
				float uvP[2] = {uA[i], vA[i]};


				if (itPoly.hasUVs())
				{

					if (!itPoly.zeroUVArea())
					{

						status = itPoly.getPointAtUV(pt, uvP, MSpace::kObject);
						status = itPoly.getAxisAtUV(normal, uTangent, vTangent, uvP, MSpace::kObject);

						if (status)
						{
							mesh_pA.append(pt);
							mesh_uTA.append(uTangent);
							mesh_vTA.append(vTangent);
							mesh_nA.append(normal);
						}

					}


				}


			}
		}


		m_numDup = mesh_pA.length();

	}

	// Vertex
	if (m_scatterType == 1)
	{
		MFnMesh refMesh(m_refMesh);
		m_numDup = refMesh.numVertices();
	}


	if (m_numDup < 1) { m_numDup = 1;}


	return MStatus::kSuccess;
}


// --- Instance on mesh ------
MStatus ClonerMultiThread::instanceOnMesh()
{

	MStatus status;

	m_tr_matA.clear();
	m_tr_matA.setLength(m_numDup);





	// UV
	if (m_scatterType == 0)
	{
		MMatrix rotMatrix;

		for (int i=0; i < m_numDup; i++) 
		{

			MVector tan = mesh_uTA[i];
			tan.normalize();

			MVector cross1 = tan ^ mesh_vTA[i];
			cross1.normalize() ;
			MVector cross2 =  tan ^ cross1;
			cross2.normalize();

			double m[4][4] = {{cross2.x, cross2.y , cross2.z, 0},
			{ cross1.x, cross1.y , cross1.z, 0},
			{-tan.x, -tan.y , -tan.z, 0},
			{mesh_pA[i].x, mesh_pA[i].y, mesh_pA[i].z, 1.0}};

			rotMatrix = m;


			// Translation
			MFloatVector v_baseOffV(m_offsetX, m_offsetY, m_offsetZ);

			// Rotation
			double rot[3] = {m_rotateX * 0.5f * ( M_PI / 180.0f ), m_rotateY * 0.5f * ( M_PI / 180.0f ),  m_rotateZ * 0.5f * ( M_PI / 180.0f )};

			// Scale
			const double scaleV[3] = {  double(m_scaleX),  double(m_scaleY),  double(m_scaleZ) };

			// Matrix
			MTransformationMatrix tr_mat(rotMatrix);

			status = tr_mat.addTranslation(v_baseOffV, MSpace::kObject);
			CHECK_MSTATUS_AND_RETURN_IT(status);

			status = tr_mat.setScale(scaleV, MSpace::kObject);
			CHECK_MSTATUS_AND_RETURN_IT(status);

			status = tr_mat.addRotation(rot, MTransformationMatrix::kXYZ, MSpace::kObject);
			CHECK_MSTATUS_AND_RETURN_IT(status);

			m_tr_matA.set(tr_mat.asMatrix() * m_refMeshTrMat, i);

		}


	}


	// Vertex
	if (m_scatterType == 1)
	{

		MPointArray pA;
		MFnMesh refMesh(m_refMesh);
		refMesh.getPoints(pA, MSpace::kObject);

		MItMeshVertex itVert(m_refMesh);

		MMatrix rotMatrix;

		for ( itVert.reset() ; !itVert.isDone() ; itVert.next() )
		{

			MPoint vertA = itVert.position(MSpace::kObject, &status);
			CHECK_MSTATUS_AND_RETURN_IT(status);

			MVector currN;

			status = itVert.getNormal(currN, MSpace::kObject);
			CHECK_MSTATUS_AND_RETURN_IT(status);

			MIntArray connVerts;
			itVert.getConnectedVertices(connVerts);

			if (connVerts.length() < 2)
			{
				return MStatus::kFailure;
			}

			MPoint vertB = pA[connVerts[0]];
			MPoint vertC = pA[connVerts[1]];

			MVector vAVec = vertB - vertA;
			vAVec.normalize();
			MVector vBVec = vertC - vertA;
			vBVec.normalize();


			MVector vVecCross =(vBVec^currN);
			vVecCross.normalize();

			double m[4][4]={
				{ vBVec.x, vBVec.y , vBVec.z, 0},
				{ currN.x, currN.y , currN.z, 0},
				{vVecCross.x, vVecCross.y , vVecCross.z, 0},
				{ vertA.x, vertA.y , vertA.z, 1}};

			rotMatrix = m;



			// Translation
			MFloatVector v_baseOffV(m_offsetX, m_offsetY, m_offsetZ);

			// Rotation
			double rot[3] = {m_rotateX * 0.5f * ( M_PI / 180.0f ), m_rotateY * 0.5f * ( M_PI / 180.0f ),  m_rotateZ * 0.5f * ( M_PI / 180.0f )};

			// Scale
			const double scaleV[3] = {  double(m_scaleX),  double(m_scaleY),  double(m_scaleZ) };

			// Matrix
			MTransformationMatrix tr_mat(rotMatrix);

			status = tr_mat.addTranslation(v_baseOffV, MSpace::kObject);
			CHECK_MSTATUS_AND_RETURN_IT(status);

			status = tr_mat.setScale(scaleV, MSpace::kObject);
			CHECK_MSTATUS_AND_RETURN_IT(status);

			status = tr_mat.addRotation(rot, MTransformationMatrix::kXYZ, MSpace::kObject);
			CHECK_MSTATUS_AND_RETURN_IT(status);

			m_tr_matA.set(tr_mat.asMatrix() * m_refMeshTrMat, itVert.index());

		}




	}


	return MS::kSuccess;

}

