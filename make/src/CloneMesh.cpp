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
#if MAYA_API_VERSION > 201600
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

		// MGlobal::displayInfo(MString() + m_numDup);

	}
#endif
	// Vertex
	if (m_scatterType == 1)
	{
		MFnMesh refMesh(m_refMesh);
		m_numDup = refMesh.numVertices(&status);
		CHECK_MSTATUS_AND_RETURN_IT(status);
	}


	// Polygon
	if (m_scatterType == 2)
	{
		MFnMesh refMesh(m_refMesh);
		m_numDup = refMesh.numPolygons(&status);
		CHECK_MSTATUS_AND_RETURN_IT(status);
	}



	if (m_numDup < 1) { m_numDup = 1;}


	return MStatus::kSuccess;
}


// --- Instance on mesh ------
MStatus ClonerMultiThread::instanceOnMesh()
{

	MStatus status;

	m_tr_matA.clear();
	status = m_tr_matA.setLength(m_numDup);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	// MGlobal::displayInfo(MString() + m_orientationType);

	// UV
	if (m_scatterType == 0)
	{
		if (mesh_uTA.length() < 1)
		{
			return MStatus::kSuccess;
		}

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


			if (m_orientationType == 1) {double m_orient[4][4] = {{0.0, 1.0 , 0.0, 0.0},{ 1.0, 0.0, 0.0, 0.0},{ 0.0, 0.0, 1.0, 0.0},{ mesh_pA[i].x, mesh_pA[i].y, mesh_pA[i].z, 1.0}}; rotMatrix=m_orient;}
			if (m_orientationType == 2) {double m_orient[4][4] = {{1.0, 0.0 , 0.0, 0.0},{ 0.0, 1.0, 0.0, 0.0},{ 0.0, 0.0, 1.0, 0.0},{ mesh_pA[i].x, mesh_pA[i].y, mesh_pA[i].z, 1.0}}; rotMatrix=m_orient;}
			if (m_orientationType == 3) {double m_orient[4][4] = {{1.0, 0.0 , 0.0, 0.0},{ 0.0, 0.0, 1.0, 0.0},{ 0.0, -1.0, 0.0, 0.0},{ mesh_pA[i].x, mesh_pA[i].y, mesh_pA[i].z, 1.0}}; rotMatrix=m_orient;}




			// Translation
			MFloatVector v_baseOffV(m_offsetX, m_offsetY, m_offsetZ);


			// Rotation
			double rot[3] = {m_rotateX * 0.5f * ( M_PI / 180.0f ), m_rotateY * 0.5f * ( M_PI / 180.0f ),  m_rotateZ * 0.5f * ( M_PI / 180.0f )};

			// Scale
			const double scaleV[3] = {  double(m_scaleX),  double(m_scaleY),  double(m_scaleZ) };

			// Random Transform
			MFloatVector v_rndOffV(m_rndOffsetXA[i], m_rndOffsetYA[i] ,m_rndOffsetZA[i]);
			// Random Rotate

			double rot_rnd[3] = {m_rndRotateXA[i] * 0.5f * ( M_PI / 180.0f ), m_rndRotateYA[i] * 0.5f * ( M_PI / 180.0f ),  m_rndRotateZA[i] * 0.5f * ( M_PI / 180.0f )};

			// Random Scale
			const double scaleV_rnd[3] = {  double(1.0+m_rndScaleXA[i]),  double(1.0+m_rndScaleYA[i]),  double(1.0+m_rndScaleZA[i]) };



			// Matrix
			MTransformationMatrix tr_mat(rotMatrix);

			status = tr_mat.addTranslation(v_baseOffV, MSpace::kObject);
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

			status = m_tr_matA.set(tr_mat.asMatrix() * m_refMeshMat, i);
			CHECK_MSTATUS_AND_RETURN_IT(status);

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

		int i=0;

		for ( itVert.reset() ; !itVert.isDone() ; itVert.next() )
		{

			i = itVert.index();

			MPoint vertA = itVert.position(MSpace::kObject, &status);
			CHECK_MSTATUS_AND_RETURN_IT(status);

			MVector currN;

			status = itVert.getNormal(currN, MSpace::kObject);
			CHECK_MSTATUS_AND_RETURN_IT(status);

			MIntArray connVerts;
			status = itVert.getConnectedVertices(connVerts);
			CHECK_MSTATUS_AND_RETURN_IT(status);

			if (connVerts.length() < 1)
			{
				return MStatus::kFailure;
			}

			MVector v1 = currN;

			MPoint vertB = pA[connVerts[0]];

			MVector v2 = vertB - vertA;
			v2.normalize();

			MVector v3 = currN ^ v2;
			v3.normalize();

			MVector v4 = v3 ^ currN;

			double m[4][4]={
				{ v3.x, v3.y , v3.z, 0},
				{ v1.x, v1.y , v1.z, 0},
				{ v4.x, v4.y , v4.z, 0},
				{ vertA.x, vertA.y , vertA.z, 1}};

			rotMatrix = m;

			if (m_orientationType == 1) {double m_orient[4][4] = {{0.0, 1.0 , 0.0, 0.0},{ 1.0, 0.0, 0.0, 0.0},{ 0.0, 0.0, 1.0, 0.0},{ vertA.x, vertA.y, vertA.z, 1.0}}; rotMatrix=m_orient;}
			if (m_orientationType == 2) {double m_orient[4][4] = {{1.0, 0.0 , 0.0, 0.0},{ 0.0, 1.0, 0.0, 0.0},{ 0.0, 0.0, 1.0, 0.0},{ vertA.x, vertA.y, vertA.z, 1.0}}; rotMatrix=m_orient;}
			if (m_orientationType == 3) {double m_orient[4][4] = {{1.0, 0.0 , 0.0, 0.0},{ 0.0, 0.0, 1.0, 0.0},{ 0.0, -1.0, 0.0, 0.0},{ vertA.x, vertA.y,vertA.z, 1.0}}; rotMatrix=m_orient;}


			// Translation
			MFloatVector v_baseOffV(m_offsetX, m_offsetY, m_offsetZ);

			// Rotation
			double rot[3] = {m_rotateX * 0.5f * ( M_PI / 180.0f ), m_rotateY * 0.5f * ( M_PI / 180.0f ),  m_rotateZ * 0.5f * ( M_PI / 180.0f )};

			// Scale
			const double scaleV[3] = {  double(m_scaleX),  double(m_scaleY),  double(m_scaleZ) };



			// Random Transform
			MFloatVector v_rndOffV(m_rndOffsetXA[i], m_rndOffsetYA[i] ,m_rndOffsetZA[i]);
			// Random Rotate

			double rot_rnd[3] = {m_rndRotateXA[i] * 0.5f * ( M_PI / 180.0f ), m_rndRotateYA[i] * 0.5f * ( M_PI / 180.0f ),  m_rndRotateZA[i] * 0.5f * ( M_PI / 180.0f )};

			// Random Scale
			const double scaleV_rnd[3] = {  double(1.0+m_rndScaleXA[i]),  double(1.0+m_rndScaleYA[i]),  double(1.0+m_rndScaleZA[i]) };



			// Matrix
			MTransformationMatrix tr_mat(rotMatrix);

			status = tr_mat.addTranslation(v_baseOffV, MSpace::kObject);
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

			status = m_tr_matA.set(tr_mat.asMatrix() * m_refMeshMat, i);
			CHECK_MSTATUS_AND_RETURN_IT(status);

		}




	}


	// Polygons
	if (m_scatterType == 2)
	{

		MPointArray pA;
		MFnMesh refMesh(m_refMesh);
		refMesh.getPoints(pA, MSpace::kObject);

		MItMeshPolygon itPoly(m_refMesh);

		MMatrix rotMatrix;

		int i=0;

		for ( itPoly.reset() ; !itPoly.isDone() ; itPoly.next() )
		{

			i = itPoly.index();

			MPoint vertA = itPoly.center(MSpace::kObject, &status);
			CHECK_MSTATUS_AND_RETURN_IT(status);

			MVector currN;

			status = itPoly.getNormal(currN, MSpace::kObject);
			CHECK_MSTATUS_AND_RETURN_IT(status);

			MIntArray connVerts;
			status = itPoly.getConnectedVertices(connVerts);
			CHECK_MSTATUS_AND_RETURN_IT(status);

			if (connVerts.length() < 1)
			{
				return MStatus::kFailure;
			}

			MVector v1 = currN;

			MPoint vertB = pA[connVerts[0]];

			MVector v2 = vertB - vertA;
			v2.normalize();

			MVector v3 = currN ^ v2;
			v3.normalize();

			MVector v4 = v3 ^ currN;

			double m[4][4]={
				{ v3.x, v3.y , v3.z, 0},
				{ v1.x, v1.y , v1.z, 0},
				{ v4.x, v4.y , v4.z, 0},
				{ vertA.x, vertA.y , vertA.z, 1}};


			rotMatrix = m;

			if (m_orientationType == 1) {double m_orient[4][4] = {{0.0, 1.0 , 0.0, 0.0},{ 1.0, 0.0, 0.0, 0.0},{ 0.0, 0.0, 1.0, 0.0},{ vertA.x, vertA.y, vertA.z, 1.0}}; rotMatrix = m_orient;}
			if (m_orientationType == 2) {double m_orient[4][4] = {{1.0, 0.0 , 0.0, 0.0},{ 0.0, 1.0, 0.0, 0.0},{ 0.0, 0.0, 1.0, 0.0},{ vertA.x, vertA.y, vertA.z, 1.0}}; rotMatrix = m_orient;}
			if (m_orientationType == 3) {double m_orient[4][4] = {{1.0, 0.0 , 0.0, 0.0},{ 0.0, 0.0, 1.0, 0.0},{ 0.0, -1.0, 0.0, 0.0},{ vertA.x, vertA.y, vertA.z, 1.0}}; rotMatrix = m_orient;}




			double area;
			status = itPoly.getArea(area);
			CHECK_MSTATUS_AND_RETURN_IT(status);

			//MGlobal::displayInfo(MString() + area);

			// Translation
			MFloatVector v_baseOffV(m_offsetX, m_offsetY, m_offsetZ);

			// Rotation
			double rot[3] = {m_rotateX * 0.5f * ( M_PI / 180.0f ), m_rotateY * 0.5f * ( M_PI / 180.0f ),  m_rotateZ * 0.5f * ( M_PI / 180.0f )};

			// Scale
			const double scaleV[3] = {  double(m_scaleX + area),  double(m_scaleY + area),  double(m_scaleZ + area) };


			// Random Transform
			MFloatVector v_rndOffV(m_rndOffsetXA[i], m_rndOffsetYA[i] ,m_rndOffsetZA[i]);
			// Random Rotate

			double rot_rnd[3] = {m_rndRotateXA[i] * 0.5f * ( M_PI / 180.0f ), m_rndRotateYA[i] * 0.5f * ( M_PI / 180.0f ),  m_rndRotateZA[i] * 0.5f * ( M_PI / 180.0f )};

			// Random Scale
			const double scaleV_rnd[3] = {  double(1.0+m_rndScaleXA[i]),  double(1.0+m_rndScaleYA[i]),  double(1.0+m_rndScaleZA[i]) };



			// Matrix
			MTransformationMatrix tr_mat(rotMatrix);

			status = tr_mat.addTranslation(v_baseOffV, MSpace::kObject);
			CHECK_MSTATUS_AND_RETURN_IT(status);
			status = tr_mat.addTranslation(v_rndOffV, MSpace::kObject);
			CHECK_MSTATUS_AND_RETURN_IT(status);

			status = tr_mat.setScale(scaleV, MSpace::kObject);
			CHECK_MSTATUS_AND_RETURN_IT(status);
			//status = tr_mat.addScale(scaleV_area, MSpace::kObject);
			//CHECK_MSTATUS_AND_RETURN_IT(status);

			status = tr_mat.addScale(scaleV_rnd, MSpace::kObject);
			CHECK_MSTATUS_AND_RETURN_IT(status);



			status = tr_mat.addRotation(rot, MTransformationMatrix::kXYZ, MSpace::kObject);
			CHECK_MSTATUS_AND_RETURN_IT(status);
			status = tr_mat.addRotation(rot_rnd, MTransformationMatrix::kXYZ, MSpace::kObject);
			CHECK_MSTATUS_AND_RETURN_IT(status);

			status = m_tr_matA.set(tr_mat.asMatrix() * m_refMeshMat, i);
			CHECK_MSTATUS_AND_RETURN_IT(status);

		}




	}



	return MS::kSuccess;

}

