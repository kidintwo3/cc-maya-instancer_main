//
//  CloneMesh.h
//  ClonerMulti
//
//  Created by Hunyadi János on 2015. 01. 03..
//  Copyright (c) 2015. Janos Hunyadi. All rights reserved.
//



#include "CloneMesh.h"

void ClonerMultiThread::lerp(MPoint& p1, MPoint& p2, double blend, MPoint& outPoint) {
	outPoint.x = p1.x + (p2.x - p1.x) * blend;
	outPoint.y = p1.y + (p2.y - p1.y) * blend;
	outPoint.z = p1.z + (p2.z - p1.z) * blend;
}


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

		float stepU = 1.0f / float(m_instanceY - 1);
		float stepV = 1.0f / float(m_instanceX - 1);

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
		MVector normal, uTangent, vTangent;
		MPoint pt;


		for (itPoly.reset(); !itPoly.isDone(); itPoly.next())
		{

			for (int i = 0; i < uA.length(); i++)
			{
				float uvP[2] = { uA[i], vA[i] };


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


	// Polygon Borders
	if (m_scatterType == 3)
	{

		m_numDup = 0;

		MItMeshPolygon itPoly(m_refMesh);

		for (itPoly.reset(); !itPoly.isDone(); itPoly.next())
		{
			if (itPoly.onBoundary())
			{
				m_numDup += 1;
			}
		}


	}

	// Custom edge
	if (m_scatterType == 4)
	{

		// Clear arrays
		mesh_edge_mA.clear();
		mesh_pA.clear();

		m_numDup = 0;

		MPointArray pA;
		MFnMesh refMesh(m_refMesh, &status);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		refMesh.getPoints(pA, MSpace::kObject);

		MFnSingleIndexedComponent customEdgeComponents;

		// Creates an MObject of type kMeshVertComponent
		m_o_customEdgeComponents = customEdgeComponents.create(MFn::kMeshEdgeComponent, &status);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		// Add the vertex ids to the component MObject
		customEdgeComponents.addElements(m_customEdgeComponents);

		MItMeshEdge itEdge(m_refMesh, m_o_customEdgeComponents, &status);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		MMatrix rotMatrix;


		//int z = 0;
		m_customEdgeLengthTotal = 0;

		vector<int> edgeDupCountA;
		vector<int> edgeIDxA;
		vector<double> edgeLengthA;

		for (itEdge.reset(); !itEdge.isDone(); itEdge.next())
		{
			double e_length;
			itEdge.getLength(e_length);

			m_customEdgeLengthTotal += e_length;
			edgeLengthA.push_back(e_length);

			edgeIDxA.push_back(itEdge.index());
		}

		// Early exit
		if (m_customEdgeLengthTotal <= 0.0)
		{
			return MStatus::kSuccess;
		}

		int dupCount = 0;

		for (int i = 0; i < edgeLengthA.size(); i++)
		{

			double ratio = (1.0 - abs(((edgeLengthA[i] - m_customEdgeLengthTotal) * 100.0) / m_customEdgeLengthTotal) * 0.01);
			double count = round(double(m_instanceX) * ratio);

			edgeDupCountA.push_back(int(count));

			//m_numDup += int(count);

		}

		if (edgeDupCountA.size() == 0)
		{
			return MStatus::kSuccess;
		}

		int prevIDx;
		int i = 0;

		bool overflow = false;

		MPointArray edgePA;

		for (int z = 0; z < edgeIDxA.size(); z++)
		{
			itEdge.setIndex(edgeIDxA[z], prevIDx);

			// Calculate edge trmat
			MPoint vertA = itEdge.point(0, MSpace::kObject, &status);
			CHECK_MSTATUS_AND_RETURN_IT(status);

			MPoint vertB = itEdge.point(1, MSpace::kObject, &status);
			CHECK_MSTATUS_AND_RETURN_IT(status);



			MVector vertNormA, vertNormB;
			refMesh.getVertexNormal(itEdge.index(0), true, vertNormA);
			refMesh.getVertexNormal(itEdge.index(1), true, vertNormB);


			MVector vertNormAv = (vertNormA + vertNormB) * 0.5;
			MVector ab = vertB - vertA;

			MVector ab_dir = ab;
			ab_dir.normalize();

			MVector xDir = ab_dir;
			MVector yDir = xDir ^ vertNormAv;
			yDir.normalize();
			MVector zDir = yDir ^ xDir;
			zDir.normalize();

			//
			MMatrix rotMatrix;
			double ratio = (1.0 / double(edgeDupCountA[z]));
			double ratio_count = 0.0;

			for (int t = 0; t < edgeDupCountA[z]; t++)
			{

				MPoint lerpPoint;
				lerp(vertA, vertB, ratio_count, lerpPoint);
				MFloatPoint p = lerpPoint;

				//

				bool on_edge = false;
				// Check if its a border edge
				for (int c = 0; c < edgePA.length(); c++)
				{
					if (p.distanceTo(edgePA[c]) == 0.0)
					{
						on_edge = true;
						break;
					}
				}

				if (i == m_numDup)
				{
					on_edge = false;
				}

				
				if (!on_edge)
				{
					double m[4][4] = { {xDir.x, xDir.y, xDir.z, 0.0}, {zDir.x, zDir.y, zDir.z, 0.0}, {yDir.x, yDir.y, yDir.z, 0.0}, {p.x, p.y, p.z, 1.0} };
					rotMatrix = m;

					mesh_edge_mA.append(rotMatrix);
					mesh_pA.append(p);

					m_numDup += 1;

					if (ratio_count == 0.0 || ratio_count == 1.0 )
					{
						edgePA.append(p);
					}
				}

			


				ratio_count += ratio;

			}
		}

	}


	if (m_numDup < 1) { m_numDup = 1; }


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

		for (int i = 0; i < m_numDup; i++)
		{

			MVector tan = mesh_uTA[i];
			tan.normalize();

			MVector cross1 = tan ^ mesh_vTA[i];
			cross1.normalize();
			MVector cross2 = tan ^ cross1;
			cross2.normalize();

			double m[4][4] = { {cross2.x, cross2.y , cross2.z, 0},
			{ cross1.x, cross1.y , cross1.z, 0},
			{-tan.x, -tan.y , -tan.z, 0},
			{mesh_pA[i].x, mesh_pA[i].y, mesh_pA[i].z, 1.0} };

			rotMatrix = m;


			if (m_orientationType == 1) { double m_orient[4][4] = { {0.0, 1.0 , 0.0, 0.0},{ 1.0, 0.0, 0.0, 0.0},{ 0.0, 0.0, 1.0, 0.0},{ mesh_pA[i].x, mesh_pA[i].y, mesh_pA[i].z, 1.0} }; rotMatrix = m_orient; }
			if (m_orientationType == 2) { double m_orient[4][4] = { {1.0, 0.0 , 0.0, 0.0},{ 0.0, 1.0, 0.0, 0.0},{ 0.0, 0.0, 1.0, 0.0},{ mesh_pA[i].x, mesh_pA[i].y, mesh_pA[i].z, 1.0} }; rotMatrix = m_orient; }
			if (m_orientationType == 3) { double m_orient[4][4] = { {1.0, 0.0 , 0.0, 0.0},{ 0.0, 0.0, 1.0, 0.0},{ 0.0, -1.0, 0.0, 0.0},{ mesh_pA[i].x, mesh_pA[i].y, mesh_pA[i].z, 1.0} }; rotMatrix = m_orient; }




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

			// Translation
			MVector v_baseOffV((m_offsetX + m_rule_off_A_X[i]) * off_ramp_multX, (m_offsetY + m_rule_off_A_Y[i]) * off_ramp_multY, m_rule_off_A_Z[i] * off_ramp_multZ);

			// Rotation
			double rot[3] = { (m_rotateX + m_rule_rot_A_X[i]) * (M_PI / 180.0f) * rot_ramp_multX, (m_rotateY + m_rule_rot_A_Y[i]) * (M_PI / 180.0f) * rot_ramp_multY,  (m_rotateZ + m_rule_rot_A_Z[i]) * (M_PI / 180.0f) * rot_ramp_multZ };

			// Scale
			const double scaleV[3] = { double(m_scaleX * m_rule_scl_A_X[i]) * scale_ramp_multX,  double(m_scaleY * m_rule_scl_A_Y[i]) * scale_ramp_multY,  double(m_scaleZ * m_rule_scl_A_Z[i]) * scale_ramp_multZ };



			// Random Transform
			MFloatVector v_rndOffV(m_rndOffsetXA[i], m_rndOffsetYA[i], m_rndOffsetZA[i]);
			// Random Rotate

			double rot_rnd[3] = { m_rndRotateXA[i] * 0.5f * (M_PI / 180.0f), m_rndRotateYA[i] * 0.5f * (M_PI / 180.0f),  m_rndRotateZA[i] * 0.5f * (M_PI / 180.0f) };

			// Random Scale
			const double scaleV_rnd[3] = { double(1.0 + m_rndScaleXA[i]),  double(1.0 + m_rndScaleYA[i]),  double(1.0 + m_rndScaleZA[i]) };



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

		int i = 0;

		for (itVert.reset(); !itVert.isDone(); itVert.next())
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

			double m[4][4] = {
				{ v3.x, v3.y , v3.z, 0},
				{ v1.x, v1.y , v1.z, 0},
				{ v4.x, v4.y , v4.z, 0},
				{ vertA.x, vertA.y , vertA.z, 1} };

			rotMatrix = m;

			if (m_orientationType == 1) { double m_orient[4][4] = { {0.0, 1.0 , 0.0, 0.0},{ 1.0, 0.0, 0.0, 0.0},{ 0.0, 0.0, 1.0, 0.0},{ vertA.x, vertA.y, vertA.z, 1.0} }; rotMatrix = m_orient; }
			if (m_orientationType == 2) { double m_orient[4][4] = { {1.0, 0.0 , 0.0, 0.0},{ 0.0, 1.0, 0.0, 0.0},{ 0.0, 0.0, 1.0, 0.0},{ vertA.x, vertA.y, vertA.z, 1.0} }; rotMatrix = m_orient; }
			if (m_orientationType == 3) { double m_orient[4][4] = { {1.0, 0.0 , 0.0, 0.0},{ 0.0, 0.0, 1.0, 0.0},{ 0.0, -1.0, 0.0, 0.0},{ vertA.x, vertA.y,vertA.z, 1.0} }; rotMatrix = m_orient; }


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

			// Translation
			MVector v_baseOffV((m_offsetX + m_rule_off_A_X[i]) * off_ramp_multX, (m_offsetY + m_rule_off_A_Y[i]) * off_ramp_multY, m_rule_off_A_Z[i] * off_ramp_multZ);

			// Rotation
			double rot[3] = { (m_rotateX + m_rule_rot_A_X[i]) * (M_PI / 180.0f) * rot_ramp_multX, (m_rotateY + m_rule_rot_A_Y[i]) * (M_PI / 180.0f) * rot_ramp_multY,  (m_rotateZ + m_rule_rot_A_Z[i]) * (M_PI / 180.0f) * rot_ramp_multZ };

			// Scale
			const double scaleV[3] = { double(m_scaleX * m_rule_scl_A_X[i]) * scale_ramp_multX,  double(m_scaleY * m_rule_scl_A_Y[i]) * scale_ramp_multY,  double(m_scaleZ * m_rule_scl_A_Z[i]) * scale_ramp_multZ };




			// Random Transform
			MFloatVector v_rndOffV(m_rndOffsetXA[i], m_rndOffsetYA[i], m_rndOffsetZA[i]);
			// Random Rotate

			double rot_rnd[3] = { m_rndRotateXA[i] * 0.5f * (M_PI / 180.0f), m_rndRotateYA[i] * 0.5f * (M_PI / 180.0f),  m_rndRotateZA[i] * 0.5f * (M_PI / 180.0f) };

			// Random Scale
			const double scaleV_rnd[3] = { double(1.0 + m_rndScaleXA[i]),  double(1.0 + m_rndScaleYA[i]),  double(1.0 + m_rndScaleZA[i]) };



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


			MMatrix outMat = tr_mat.asMatrix();
			if (m_worldSpace) { outMat *= m_inMeshMatrixArray[m_idA[i]]; }

			status = m_tr_matA.set(outMat * m_refMeshMat, i);
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

		int i = 0;

		for (itPoly.reset(); !itPoly.isDone(); itPoly.next())
		{

			i = itPoly.index();

			MPoint vertA = itPoly.center(MSpace::kObject, &status);
			CHECK_MSTATUS_AND_RETURN_IT(status);

			MVector currN;


			MVector uTangent, vTangent;
			float2 uvP;

			itPoly.getUVAtPoint(vertA, uvP);
			CHECK_MSTATUS_AND_RETURN_IT(status);

			status = itPoly.getAxisAtUV(currN, uTangent, vTangent, uvP, MSpace::kObject);
			CHECK_MSTATUS_AND_RETURN_IT(status);

			MVector tan = uTangent;
			tan.normalize();

			MVector cross1 = tan ^ vTangent;
			cross1.normalize();
			MVector cross2 = tan ^ cross1;
			cross2.normalize();

			double m[4][4] = { { cross2.x, cross2.y , cross2.z, 0 },
			{ cross1.x, cross1.y , cross1.z, 0 },
			{ -tan.x, -tan.y , -tan.z, 0 },
			{ vertA.x, vertA.y , vertA.z, 1.0 } };

			rotMatrix = m;


			//rotMatrix = m;

			if (m_orientationType == 1) { double m_orient[4][4] = { {0.0, 1.0 , 0.0, 0.0},{ 1.0, 0.0, 0.0, 0.0},{ 0.0, 0.0, 1.0, 0.0},{ vertA.x, vertA.y, vertA.z, 1.0} }; rotMatrix = m_orient; }
			if (m_orientationType == 2) { double m_orient[4][4] = { {1.0, 0.0 , 0.0, 0.0},{ 0.0, 1.0, 0.0, 0.0},{ 0.0, 0.0, 1.0, 0.0},{ vertA.x, vertA.y, vertA.z, 1.0} }; rotMatrix = m_orient; }
			if (m_orientationType == 3) { double m_orient[4][4] = { {1.0, 0.0 , 0.0, 0.0},{ 0.0, 0.0, 1.0, 0.0},{ 0.0, -1.0, 0.0, 0.0},{ vertA.x, vertA.y, vertA.z, 1.0} }; rotMatrix = m_orient; }


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


			double area;
			status = itPoly.getArea(area);
			CHECK_MSTATUS_AND_RETURN_IT(status);

			//MGlobal::displayInfo(MString() + area);

			// Translation
			MVector v_baseOffV((m_offsetX + m_rule_off_A_X[i]) * off_ramp_multX, (m_offsetY + m_rule_off_A_Y[i]) * off_ramp_multY, m_rule_off_A_Z[i] * off_ramp_multZ);

			// Rotation
			double rot[3] = { (m_rotateX + m_rule_rot_A_X[i]) * (M_PI / 180.0f) * rot_ramp_multX, (m_rotateY + m_rule_rot_A_Y[i]) * (M_PI / 180.0f) * rot_ramp_multY,  (m_rotateZ + m_rule_rot_A_Z[i]) * (M_PI / 180.0f) * rot_ramp_multZ };

			// Scale
			const double scaleV[3] = { double(m_scaleX * m_rule_scl_A_X[i]) * scale_ramp_multX,  double(m_scaleY * m_rule_scl_A_Y[i]) * scale_ramp_multY,  double(m_scaleZ * m_rule_scl_A_Z[i]) * scale_ramp_multZ };



			// Random Transform
			MFloatVector v_rndOffV(m_rndOffsetXA[i], m_rndOffsetYA[i], m_rndOffsetZA[i]);
			// Random Rotate

			double rot_rnd[3] = { m_rndRotateXA[i] * 0.5f * (M_PI / 180.0f), m_rndRotateYA[i] * 0.5f * (M_PI / 180.0f),  m_rndRotateZA[i] * 0.5f * (M_PI / 180.0f) };

			// Random Scale
			const double scaleV_rnd[3] = { double(1.0 + m_rndScaleXA[i]),  double(1.0 + m_rndScaleYA[i]),  double(1.0 + m_rndScaleZA[i]) };



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


			MMatrix outMat = tr_mat.asMatrix();
			if (m_worldSpace) { outMat *= m_inMeshMatrixArray[m_idA[i]]; }

			status = m_tr_matA.set(outMat * m_refMeshMat, i);
			CHECK_MSTATUS_AND_RETURN_IT(status);

		}



	}


	// Polygon borders
	if (m_scatterType == 3)
	{

		MPointArray pA;
		MFnMesh refMesh(m_refMesh);
		refMesh.getPoints(pA, MSpace::kObject);

		MItMeshPolygon itPoly(m_refMesh);

		MMatrix rotMatrix;

		int i = 0;

		for (itPoly.reset(); !itPoly.isDone(); itPoly.next())
		{

			if (itPoly.onBoundary())
			{

				MPoint vertA = itPoly.center(MSpace::kObject, &status);
				CHECK_MSTATUS_AND_RETURN_IT(status);

				MVector currN;


				MVector uTangent, vTangent;
				float2 uvP;

				itPoly.getUVAtPoint(vertA, uvP);
				CHECK_MSTATUS_AND_RETURN_IT(status);

				status = itPoly.getAxisAtUV(currN, uTangent, vTangent, uvP, MSpace::kObject);
				CHECK_MSTATUS_AND_RETURN_IT(status);

				MVector tan = uTangent;
				tan.normalize();

				MVector cross1 = tan ^ vTangent;
				cross1.normalize();
				MVector cross2 = tan ^ cross1;
				cross2.normalize();

				double m[4][4] = { { cross2.x, cross2.y , cross2.z, 0 },
				{ cross1.x, cross1.y , cross1.z, 0 },
				{ -tan.x, -tan.y , -tan.z, 0 },
				{ vertA.x, vertA.y , vertA.z, 1.0 } };

				rotMatrix = m;


				if (m_orientationType == 1) { double m_orient[4][4] = { { 0.0, 1.0 , 0.0, 0.0 },{ 1.0, 0.0, 0.0, 0.0 },{ 0.0, 0.0, 1.0, 0.0 },{ vertA.x, vertA.y, vertA.z, 1.0 } }; rotMatrix = m_orient; }
				if (m_orientationType == 2) { double m_orient[4][4] = { { 1.0, 0.0 , 0.0, 0.0 },{ 0.0, 1.0, 0.0, 0.0 },{ 0.0, 0.0, 1.0, 0.0 },{ vertA.x, vertA.y, vertA.z, 1.0 } }; rotMatrix = m_orient; }
				if (m_orientationType == 3) { double m_orient[4][4] = { { 1.0, 0.0 , 0.0, 0.0 },{ 0.0, 0.0, 1.0, 0.0 },{ 0.0, -1.0, 0.0, 0.0 },{ vertA.x, vertA.y, vertA.z, 1.0 } }; rotMatrix = m_orient; }


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


				double area;
				status = itPoly.getArea(area);
				CHECK_MSTATUS_AND_RETURN_IT(status);

				//MGlobal::displayInfo(MString() + area);

				// Translation
				MVector v_baseOffV((m_offsetX + m_rule_off_A_X[i]) * off_ramp_multX, (m_offsetY + m_rule_off_A_Y[i]) * off_ramp_multY, m_rule_off_A_Z[i] * off_ramp_multZ);

				// Rotation
				double rot[3] = { (m_rotateX + m_rule_rot_A_X[i]) * (M_PI / 180.0f) * rot_ramp_multX, (m_rotateY + m_rule_rot_A_Y[i]) * (M_PI / 180.0f) * rot_ramp_multY,  (m_rotateZ + m_rule_rot_A_Z[i]) * (M_PI / 180.0f) * rot_ramp_multZ };

				// Scale
				const double scaleV[3] = { double(m_scaleX * m_rule_scl_A_X[i]) * scale_ramp_multX,  double(m_scaleY * m_rule_scl_A_Y[i]) * scale_ramp_multY,  double(m_scaleZ * m_rule_scl_A_Z[i]) * scale_ramp_multZ };


				// Random Transform
				MVector v_rndOffV(m_rndOffsetXA[i], m_rndOffsetYA[i], m_rndOffsetZA[i]);
				// Random Rotate

				double rot_rnd[3] = { m_rndRotateXA[i] * 0.5f * (M_PI / 180.0f), m_rndRotateYA[i] * 0.5f * (M_PI / 180.0f),  m_rndRotateZA[i] * 0.5f * (M_PI / 180.0f) };

				// Random Scale
				const double scaleV_rnd[3] = { double(1.0 + m_rndScaleXA[i]),  double(1.0 + m_rndScaleYA[i]),  double(1.0 + m_rndScaleZA[i]) };


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


				MMatrix outMat = tr_mat.asMatrix();
				if (m_worldSpace) { outMat *= m_inMeshMatrixArray[m_idA[i]]; }

				status = m_tr_matA.set(outMat * m_refMeshMat, i);
				CHECK_MSTATUS_AND_RETURN_IT(status);

				i += 1;

			}


		}



	}


	// Custom Edges
	if (m_scatterType == 4)
	{

		//MPointArray pA;
		//MFnMesh refMesh(m_refMesh, &status);
		//CHECK_MSTATUS_AND_RETURN_IT(status);

		//refMesh.getPoints(pA, MSpace::kObject);

		//MFnSingleIndexedComponent customEdgeComponents;

		//// Creates an MObject of type kMeshVertComponent
		//m_o_customEdgeComponents = customEdgeComponents.create(MFn::kMeshEdgeComponent, &status);
		//CHECK_MSTATUS_AND_RETURN_IT(status);

		//// Add the vertex ids to the component MObject
		//customEdgeComponents.addElements(m_customEdgeComponents);

		//MItMeshEdge itEdge(m_refMesh, m_o_customEdgeComponents, &status);
		//CHECK_MSTATUS_AND_RETURN_IT(status);

		//MMatrix rotMatrix;


		////int z = 0;
		//m_customEdgeLengthTotal = 0;

		//vector<int> edgeDupCountA;
		//vector<int> edgeIDxA;
		//vector<double> edgeLengthA;

		//for (itEdge.reset(); !itEdge.isDone(); itEdge.next())
		//{
		//	double e_length;
		//	itEdge.getLength(e_length);

		//	m_customEdgeLengthTotal += e_length;
		//	edgeLengthA.push_back(e_length);

		//	edgeIDxA.push_back(itEdge.index());
		//}

		//// Early exit
		//if (m_customEdgeLengthTotal <= 0.0)
		//{
		//	return MStatus::kSuccess;
		//}

		//int dupCount = 0;

		//for (int i = 0; i < edgeLengthA.size(); i++)
		//{

		//	double ratio =  (1.0 - abs(((edgeLengthA[i] - m_customEdgeLengthTotal) * 100.0) / m_customEdgeLengthTotal) * 0.01);
		//	double count = round(double(m_instanceX) * ratio);
		//	
		//	edgeDupCountA.push_back(int(count));
		//}

		//if (edgeDupCountA.size() == 0)
		//{
		//	return MStatus::kSuccess;
		//}

		//int prevIDx;
		//int i = 0;

		//bool overflow = false;


		//MPointArray edgePA;

		//for (int z = 0; z < edgeIDxA.size(); z++)
		//{
		//	itEdge.setIndex(edgeIDxA[z], prevIDx);

		//	// Calculate edge trmat
		//	MPoint vertA = itEdge.point(0, MSpace::kObject, &status);
		//	CHECK_MSTATUS_AND_RETURN_IT(status);

		//	MPoint vertB = itEdge.point(1, MSpace::kObject, &status);
		//	CHECK_MSTATUS_AND_RETURN_IT(status);



		//	MVector vertNormA, vertNormB;
		//	refMesh.getVertexNormal(itEdge.index(0), true, vertNormA);
		//	refMesh.getVertexNormal(itEdge.index(1), true, vertNormB);
		//	

		//	MVector vertNormAv = (vertNormA + vertNormB) * 0.5;
		//	MVector ab = vertB - vertA;

		//	MVector ab_dir = ab;
		//	ab_dir.normalize();

		//	MVector xDir = ab_dir;
		//	MVector yDir = xDir ^ vertNormAv;
		//	yDir.normalize();
		//	MVector zDir = yDir ^ xDir;
		//	zDir.normalize();

		//	MTransformationMatrix mTr;

		//	MPoint b;

		//	MMatrix rotMatrix;


		//	double ratio = (1.0 / double(edgeDupCountA[z] ) );

		//	if (overflow)
		//	{
		//		break;
		//	}

		//	double ratio_count = 0.0;

		//	for (int t = 0; t < edgeDupCountA[z]; t++)
		//	{



		//		if (i > m_numDup)
		//		{
		//			overflow = true;
		//			break;
		//		}
		//		
		//		//ratio = 0.5;


		//		//MGlobal::displayInfo(MString() + ratio_count + " / " + edgeDupCountA[z] +  " - " + t + " . " + i);

		///*		double off_ramp_multX, off_ramp_multY, off_ramp_multZ = 1.0;

		//		if (i < int(m_offsetProfileAX.length())) { off_ramp_multX = m_offsetProfileAX[i]; }
		//		if (i < int(m_offsetProfileAY.length())) { off_ramp_multY = m_offsetProfileAY[i]; }
		//		if (i < int(m_offsetProfileAZ.length())) { off_ramp_multZ = m_offsetProfileAZ[i]; }

		//		double rot_ramp_multX, rot_ramp_multY, rot_ramp_multZ = 1.0;

		//		if (i < int(m_rotateProfileAX.length())) { rot_ramp_multX = m_rotateProfileAX[i]; }
		//		if (i < int(m_rotateProfileAY.length())) { rot_ramp_multY = m_rotateProfileAY[i]; }
		//		if (i < int(m_rotateProfileAZ.length())) { rot_ramp_multZ = m_rotateProfileAZ[i]; }

		//		double scale_ramp_multX, scale_ramp_multY, scale_ramp_multZ = 1.0;

		//		if (i < int(m_scaleProfileAX.length())) { scale_ramp_multX = m_scaleProfileAX[i]; }
		//		if (i < int(m_scaleProfileAY.length())) { scale_ramp_multY = m_scaleProfileAY[i]; }
		//		if (i < int(m_scaleProfileAZ.length())) { scale_ramp_multZ = m_scaleProfileAZ[i]; }*/


		//		MPoint lerpPoint;
		//		lerp(vertA, vertB, ratio_count, lerpPoint);
		//		MFloatPoint p = lerpPoint;



		//	

		//		////p = (vertA + vertB) * 0.5;

		//		////MGlobal::executeCommand(MString() + "spaceLocator -p " + p.x + " " + p.y + " " + p.z + ";");

		//		double m[4][4] = { {xDir.x, xDir.y, xDir.z, 0.0}, {zDir.x, zDir.y, zDir.z, 0.0}, {yDir.x, yDir.y, yDir.z, 0.0}, {p.x, p.y, p.z, 1.0} };

		//		rotMatrix = m;


		//		if (m_orientationType == 1) { double m[4][4] = { {0.0, 1.0 , 0.0, 0.0},{ 1.0, 0.0, 0.0, 0.0},{ 0.0, 0.0, 1.0, 0.0},{ p.x, p.y, p.z, 1.0} }; rotMatrix = m; }
		//		if (m_orientationType == 2) { double m[4][4] = { {1.0, 0.0 , 0.0, 0.0},{ 0.0, 1.0, 0.0, 0.0},{ 0.0, 0.0, 1.0, 0.0},{ p.x, p.y, p.z, 1.0} }; rotMatrix = m; }
		//		if (m_orientationType == 3) { double m[4][4] = { {1.0, 0.0 , 0.0, 0.0},{ 0.0, 0.0, 1.0, 0.0},{ 0.0, -1.0, 0.0, 0.0},{ p.x, p.y, p.z, 1.0} }; rotMatrix = m; }


		//		// Translation
		//		MFloatVector v_baseOff(m_offsetX, m_offsetY , m_offsetZ );


		//		// Rotation
		//		double rot[3] = { (m_rotateX) * (M_PI / 180.0f), (m_rotateY ) * (M_PI / 180.0f) ,  (m_rotateZ ) * (M_PI / 180.0f)  };


		//		// Scale
		//		const double scaleV[3] = { double(m_scaleX ),  double(m_scaleY ),  double(m_scaleZ ) };


		//		// Random Transform
		//		MFloatVector v_rndOffV(m_rndOffsetXA[i], m_rndOffsetYA[i], m_rndOffsetZA[i]);
		//		// Random Rotate

		//		double rot_rnd[3] = { m_rndRotateXA[i] * 0.5f * (M_PI / 180.0f), m_rndRotateYA[i] * 0.5f * (M_PI / 180.0f),  m_rndRotateZA[i] * 0.5f * (M_PI / 180.0f) };

		//		// Random Scale
		//		const double scaleV_rnd[3] = { double(1.0 + m_rndScaleXA[i]),  double(1.0 + m_rndScaleYA[i]),  double(1.0 + m_rndScaleZA[i]) };


		//		// Matrix
		//		MTransformationMatrix tr_mat(rotMatrix);

		//		status = tr_mat.addTranslation(v_baseOff, MSpace::kObject);
		//		CHECK_MSTATUS_AND_RETURN_IT(status);
		//		status = tr_mat.addTranslation(v_rndOffV, MSpace::kObject);
		//		CHECK_MSTATUS_AND_RETURN_IT(status);

		//		status = tr_mat.setScale(scaleV, MSpace::kObject);
		//		CHECK_MSTATUS_AND_RETURN_IT(status);
		//		status = tr_mat.addScale(scaleV_rnd, MSpace::kObject);
		//		CHECK_MSTATUS_AND_RETURN_IT(status);

		//		status = tr_mat.addRotation(rot, MTransformationMatrix::kXYZ, MSpace::kObject);
		//		CHECK_MSTATUS_AND_RETURN_IT(status);
		//		status = tr_mat.addRotation(rot_rnd, MTransformationMatrix::kXYZ, MSpace::kObject);
		//		CHECK_MSTATUS_AND_RETURN_IT(status);



		//		MMatrix outMat = tr_mat.asMatrix();

		//		//////if (m_worldSpace) { outMat *= m_inMeshMatrixArray[m_idA[i]]; }

		//

		//		if (i <= m_numDup)
		//		{

		//			bool on_edge = false;
		//			// Check if its a border edge
		//			for (int c = 0; c < edgePA.length(); c++)
		//			{
		//				if (p.distanceTo(edgePA[c]) == 0.0)
		//				{
		//					on_edge = true;
		//					break;
		//				}
		//			}

		//			if (i == m_numDup)
		//			{
		//				on_edge = false;
		//			}

		//			if (!on_edge)
		//			{
		//				status = m_tr_matA.set(outMat * m_refMeshMat, i);
		//				CHECK_MSTATUS_AND_RETURN_IT(status);

		//				if (ratio_count == 0.0 || ratio_count == 1.0)
		//				{
		//					edgePA.append(p);
		//				}
		//			}


		//			else
		//			{
		//				MMatrix or ;
		//				status = m_tr_matA.set(or, i);
		//				CHECK_MSTATUS_AND_RETURN_IT(status);
		//			}

		//		}


		//		

		//		i += 1;

		//		ratio_count += ratio;
		//	}

		//}

		//MGlobal::displayInfo(MString() + "total: " + i + " / " + m_tr_matA.length() + " / " + m_offsetProfileAX.length() + " / " + m_rndRotateYA.length());

		 //m_tr_matA.setLength(m_numDup);

	//}
		MMatrix rotMatrix;
		MPoint p;

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

			rotMatrix = mesh_edge_mA[i];
			p = mesh_pA[i];

			if (m_orientationType == 1) { double m[4][4] = { {0.0, 1.0 , 0.0, 0.0},{ 1.0, 0.0, 0.0, 0.0},{ 0.0, 0.0, 1.0, 0.0},{ p.x, p.y, p.z, 1.0} }; rotMatrix = m; }
			if (m_orientationType == 2) { double m[4][4] = { {1.0, 0.0 , 0.0, 0.0},{ 0.0, 1.0, 0.0, 0.0},{ 0.0, 0.0, 1.0, 0.0},{ p.x, p.y, p.z, 1.0} }; rotMatrix = m; }
			if (m_orientationType == 3) { double m[4][4] = { {1.0, 0.0 , 0.0, 0.0},{ 0.0, 0.0, 1.0, 0.0},{ 0.0, -1.0, 0.0, 0.0},{ p.x, p.y, p.z, 1.0} }; rotMatrix = m; }


			// Translation
			MFloatVector v_baseOff(m_offsetX, m_offsetY, m_offsetZ);


			// Rotation
			double rot[3] = { (m_rotateX) * (M_PI / 180.0f), (m_rotateY) * (M_PI / 180.0f) ,  (m_rotateZ) * (M_PI / 180.0f) };


			// Scale
			const double scaleV[3] = { double(m_scaleX),  double(m_scaleY),  double(m_scaleZ) };


			// Random Transform
			MFloatVector v_rndOffV(m_rndOffsetXA[i], m_rndOffsetYA[i], m_rndOffsetZA[i]);
			// Random Rotate

			double rot_rnd[3] = { m_rndRotateXA[i] * 0.5f * (M_PI / 180.0f), m_rndRotateYA[i] * 0.5f * (M_PI / 180.0f),  m_rndRotateZA[i] * 0.5f * (M_PI / 180.0f) };

			// Random Scale
			const double scaleV_rnd[3] = { double(1.0 + m_rndScaleXA[i]),  double(1.0 + m_rndScaleYA[i]),  double(1.0 + m_rndScaleZA[i]) };


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



			MMatrix outMat = tr_mat.asMatrix();

			status = m_tr_matA.set(outMat * m_refMeshMat, i);
			CHECK_MSTATUS_AND_RETURN_IT(status);

		}

		return MS::kSuccess;

	}
}
