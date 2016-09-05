//
//  ClonerMulti.cpp
//  ClonerMultiThread
//
//  Created by Hunyadi Janos on 31/01/15.
//  Copyright (c) 2015 Janos Hunyadi. All rights reserved.
//

#include "ClonerMulti.h"

MTypeId     ClonerMultiThread::id( 0x00123946 );

MObject     ClonerMultiThread::aOutMesh;
MObject		ClonerMultiThread::aOutMatrixArray;
MObject		ClonerMultiThread::aOutIDArray;
MObject     ClonerMultiThread::aInMesh;
MObject		ClonerMultiThread::aInCurve;
MObject		ClonerMultiThread::aRefMesh;
MObject     ClonerMultiThread::aInLocAPos;
MObject     ClonerMultiThread::aInLocBPos;

MObject     ClonerMultiThread::aInstanceType;
MObject		ClonerMultiThread::aPatterType;
MObject		ClonerMultiThread::aScatterType;
MObject		ClonerMultiThread::aOrientationType;

MObject     ClonerMultiThread::aIDType;
MObject     ClonerMultiThread::aRevPattern;
MObject		ClonerMultiThread::aLimitDisplay;

// Instance count
MObject     ClonerMultiThread::aGridInstanceX;
MObject     ClonerMultiThread::aGridInstanceY;
MObject     ClonerMultiThread::aGridInstanceZ;

// Offset
MObject     ClonerMultiThread::aGridOffsetX;
MObject     ClonerMultiThread::aGridOffsetY;
MObject     ClonerMultiThread::aGridOffsetZ;

// Rotate
MObject     ClonerMultiThread::aRotateX;
MObject     ClonerMultiThread::aRotateY;
MObject     ClonerMultiThread::aRotateZ;

// Scale
MObject     ClonerMultiThread::aScaleX;
MObject     ClonerMultiThread::aScaleY;
MObject     ClonerMultiThread::aScaleZ;

// Random
MObject     ClonerMultiThread::aRndOffsetX;
MObject     ClonerMultiThread::aRndOffsetY;
MObject     ClonerMultiThread::aRndOffsetZ;
MObject     ClonerMultiThread::aRndRotateX;
MObject     ClonerMultiThread::aRndRotateY;
MObject     ClonerMultiThread::aRndRotateZ;
MObject     ClonerMultiThread::aRndScaleX;
MObject     ClonerMultiThread::aRndScaleY;
MObject     ClonerMultiThread::aRndScaleZ;

// UV
MObject		ClonerMultiThread::aUVOffsetU;
MObject		ClonerMultiThread::aUVOffsetV;
MObject		ClonerMultiThread::aRndUVOffsetU;
MObject		ClonerMultiThread::aRndUVOffsetV;

MObject     ClonerMultiThread::aSeedVal;
MObject     ClonerMultiThread::aRandIDLev;

MObject     ClonerMultiThread::aReverseNormals;
MObject     ClonerMultiThread::aSmoothNormals;
MObject     ClonerMultiThread::aMergeInputMeshes;
MObject		ClonerMultiThread::aWorldSpace;
MObject		ClonerMultiThread::aLoopOffset;
MObject		ClonerMultiThread::aUvUDIMLoop;
MObject		ClonerMultiThread::aOutputMeshDisplayOverride;
MObject		ClonerMultiThread::aDisplayProxy;

MObject     ClonerMultiThread::aFirstUpVec;
MObject     ClonerMultiThread::aFirstUpVecX;
MObject     ClonerMultiThread::aFirstUpVecY;
MObject     ClonerMultiThread::aFirstUpVecZ;


MObject		ClonerMultiThread::aConnectPieces;
MObject		ClonerMultiThread::aConnectArrayA;
MObject		ClonerMultiThread::aConnectArrayB;


MString		ClonerMultiThread::drawDbClassification("drawdb/geometry/ClonerMultiThread");
MString		ClonerMultiThread::drawRegistrantId("ClonerMultiThreadPlugin");



// ------------- MAIN PROGRAM -------------------------------------------------------------

ClonerMultiThreadOverride::ClonerMultiThreadOverride(const MObject& obj) : MHWRender::MPxDrawOverride(obj, ClonerMultiThreadOverride::draw){}
ClonerMultiThreadOverride::~ClonerMultiThreadOverride(){}

ClonerMultiThread::ClonerMultiThread()
{
}

ClonerMultiThread::~ClonerMultiThread()
{
}

void* ClonerMultiThread::creator()
{
	return new ClonerMultiThread();
}

void ClonerMultiThread::postConstructor() 
{
	MFnDependencyNode nodeFn(thisMObject());

	nodeFn.setName("clonerMultiShape#");

}




MStatus ClonerMultiThread::mergeInputMeshes()
{
	MStatus status;

	// Setup mesh creation vector arrays for all meshes
	i_numVertices.resize(m_numInputMeshes);
	i_numPolygons.resize(m_numInputMeshes);
	i_vertexArray.resize(m_numInputMeshes);
	i_polygonCounts.resize(m_numInputMeshes);
	i_polygonConnects.resize(m_numInputMeshes);

	for (int m = 0; m < m_numInputMeshes; m++)
	{

		//// Collect mesh data
		MFnMesh mfnMesh(m_inMeshArray[m]);

		i_numVertices[m] = mfnMesh.numVertices();
		i_numPolygons[m] = mfnMesh.numPolygons();


		mfnMesh.getPoints(i_vertexArray[m],MSpace::kWorld);
		mfnMesh.getVertices(i_polygonCounts[m], i_polygonConnects[m]);


	}

	// Allocate memory for vector
	int id = 0;
	int len_o_vertexArray=0;
	int len_o_polygonCounts=0;
	int len_o_polygonConnects=0;

	for (int m = 0; m < m_numInputMeshes; m++)
	{
		id = m;
		for (unsigned v = 0; v < i_vertexArray[id].length(); v++) {
			len_o_vertexArray+=1;
		}
		for (unsigned v = 0; v < i_polygonCounts[id].length(); v++) {
			len_o_polygonCounts+=1;
		}
		for (unsigned v = 0; v < i_polygonConnects[id].length(); v++) {
			len_o_polygonConnects+=1;
		}

	}

	//
	o_numVertices = 0;
	o_numPolygons = 0;
	o_vertexArray.clear();
	o_polygonCounts.clear();
	o_polygonConnects.clear();

	o_vertexArray.setLength(len_o_vertexArray);
	o_polygonCounts.setLength(len_o_polygonCounts);
	o_polygonConnects.setLength(len_o_polygonConnects);

	for (int m = 0; m < m_numInputMeshes; m++)
	{
		id = m;

		// numVertices
		o_numVertices += i_numVertices[id];

		// numPolygons
		o_numPolygons += i_numPolygons[id];
	}

	int idOffset = 0;
	int polycOffset = 0;
	int polyConnOffset = 0;
	//int edgeSmoothOffset = 0;

	for (int m = 0; m < m_numInputMeshes; m++)
	{

		id = m;

#pragma omp parallel for
		// vertexArray
		for (int v = 0; v < i_vertexArray[id].length(); v++) {
			MFloatPoint currP = i_vertexArray[id][v];

			MFloatPoint currentPoint( currP.x, currP.y, currP.z, currP.w  );


			o_vertexArray.set(currentPoint,v + idOffset );
		}
#pragma omp parallel for
		// polygonCounts
		for (int v = 0; v < i_polygonCounts[id].length(); v++) {
			o_polygonCounts.set(i_polygonCounts[id][v], v+polycOffset);
		}



#pragma omp parallel for
		// polygonConnects
		for (int v = 0; v < i_polygonConnects[id].length(); v++) {
			o_polygonConnects.set(i_polygonConnects[id][v] +  idOffset,  v + polyConnOffset );


		}



		// Offsets for loops

		idOffset += i_vertexArray[id].length();
		polycOffset += i_polygonCounts[id].length();
		polyConnOffset += i_polygonConnects[id].length();



	}





	// Flip normals if set
	reverseNormals();

	return MS::kSuccess;
}

MStatus ClonerMultiThread::duplicateInputMeshes(MIntArray& idA)
{

	MStatus status;


	// Setup mesh creation vector arrays for all meshes



	i_numVertices.resize(m_numInputMeshes);
	i_numPolygons.resize(m_numInputMeshes);
	i_vertexArray.resize(m_numInputMeshes);
	i_polygonCounts.resize(m_numInputMeshes);
	i_polygonConnects.resize(m_numInputMeshes);

	for (int m = 0; m < m_numInputMeshes; m++)
	{

		//// Collect mesh data
		MFnMesh mfnMesh(m_inMeshArray[m]);

		i_numVertices[m] = mfnMesh.numVertices();
		i_numPolygons[m] = mfnMesh.numPolygons();


		mfnMesh.getPoints(i_vertexArray[m],MSpace::kWorld);
		mfnMesh.getVertices(i_polygonCounts[m], i_polygonConnects[m]);


	}

	// Allocate memory for vector
	int len_o_vertexArray=0;
	int len_o_polygonCounts=0;
	int len_o_polygonConnects=0;

	for (int m = 0; m < m_numDup; m++)
	{
		for (unsigned v = 0; v < i_vertexArray[idA[m]].length(); v++) {
			len_o_vertexArray+=1;
		}
		for (unsigned v = 0; v < i_polygonCounts[idA[m]].length(); v++) {
			len_o_polygonCounts+=1;
		}
		for (unsigned v = 0; v < i_polygonConnects[idA[m]].length(); v++) {
			len_o_polygonConnects+=1;
		}

	}

	//
	o_numVertices = 0;
	o_numPolygons = 0;
	o_vertexArray.clear();
	o_polygonCounts.clear();
	o_polygonConnects.clear();

	status = o_vertexArray.setLength(len_o_vertexArray);
	CHECK_MSTATUS_AND_RETURN_IT(status);
	status = o_polygonCounts.setLength(len_o_polygonCounts);
	CHECK_MSTATUS_AND_RETURN_IT(status);
	status = o_polygonConnects.setLength(len_o_polygonConnects);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	for (int m = 0; m < m_numDup; m++)
	{

		// numVertices
		o_numVertices += i_numVertices[idA[m]];

		// numPolygons
		o_numPolygons += i_numPolygons[idA[m]];
	}

	int idOffset = 0;
	int polycOffset = 0;
	int polyConnOffset = 0;

	float offX = 0.0f;
	float offY = 0.0f;
	float offZ = 0.0f;


	//if (m_connectPieces)
	//{
	//	for (int i = 0; i < m_ConnectArrayA.length(); i++)
	//	{
	//		MGlobal::displayInfo(MString() + m_ConnectArrayA[i]);
	//	}

	//	MGlobal::displayInfo(MString() + "---");

	//	for (int i = 0; i < m_ConnectArrayB.length(); i++)
	//	{
	//		MGlobal::displayInfo(MString() + m_ConnectArrayB[i]);
	//	}
	//}

	//MGlobal::displayInfo(MString() + "---");


	for (int m = 0; m < m_numDup; m++)
	{


#pragma omp parallel for
		// vertexArray
		for (int v = 0; v < i_vertexArray[idA[m]].length(); v++) 
		{
			MFloatPoint currP = i_vertexArray[idA[m]][v];
			MPoint currentPoint( currP.x, currP.y, currP.z, currP.w  );

			if (!m_worldSpace) { currentPoint = (MPoint(currentPoint) *= m_inMeshMatrixArray[idA[m]].inverse()); }

			currentPoint *= m_tr_matA[m];

			o_vertexArray.set(currentPoint,v + idOffset );
		}

#pragma omp parallel for
		// polygonCounts
		for (int v = 0; v < i_polygonCounts[idA[m]].length(); v++) {
			o_polygonCounts.set(i_polygonCounts[idA[m]][v], v+polycOffset);
		}



#pragma omp parallel for
		// polygonConnects
		for (int v = 0; v < i_polygonConnects[idA[m]].length(); v++) {
			o_polygonConnects.set(i_polygonConnects[idA[m]][v] +  idOffset,  v + polyConnOffset );


		}



		// Offsets for loops

		idOffset += i_vertexArray[idA[m]].length();
		polycOffset += i_polygonCounts[idA[m]].length();
		polyConnOffset += i_polygonConnects[idA[m]].length();

		offX += m_offsetX;
		offY += m_offsetY;
		offZ += m_offsetZ;


	}



	// Snap verts
	if (m_connectPieces)
	{

		if (m_numInputMeshes == 1)
		{

			idOffset = 0;

			for (int m = 0; m < m_numDup - 1; m++)
			{

				// MGlobal::displayInfo(MString() + idOffset);

				for (int v = 0; v < i_vertexArray[idA[m]].length(); v++) 
				{
					for (int x = 0; x < m_ConnectArrayA.length(); x++)
					{
						if (v == m_ConnectArrayA[x])
						{
							// MGlobal::displayInfo(MString() + (m_ConnectArrayA[x] + idOffset) + " -> " + (m_ConnectArrayB[x]  + idOffset + i_vertexArray[idA[m]].length()) );

							MPoint aP =  o_vertexArray[m_ConnectArrayA[x] + idOffset];
							MPoint bP =  o_vertexArray[m_ConnectArrayB[x]  + idOffset + i_vertexArray[idA[m]].length()];

							MPoint nP = (aP + bP) * 0.5;

							o_vertexArray.set(nP, m_ConnectArrayA[x] + idOffset );
							o_vertexArray.set(nP, m_ConnectArrayB[x]  + idOffset + i_vertexArray[idA[m]].length() );
						}
						//MPoint currentPoint = (o_vertexArray[m_ConnectArrayA[x] + idOffset] + o_vertexArray[m_ConnectArrayB[x] + (idOffset + i_vertexArray[idA[m]].length() )]) * 0.5;
						//o_vertexArray.set(currentPoint, v + idOffset );
					}
				}

				idOffset += i_vertexArray[idA[m]].length();

			}

		}

	}


	// Flip normals if set
	reverseNormals();


	return MS::kSuccess;
}

MStatus ClonerMultiThread::generateBBMeshes(MIntArray& idA)
{

	MStatus status;


	// Setup mesh creation vector arrays for all meshes



	i_numVertices.resize(m_numInputMeshes);
	i_numPolygons.resize(m_numInputMeshes);
	i_vertexArray.resize(m_numInputMeshes);
	i_polygonCounts.resize(m_numInputMeshes);
	i_polygonConnects.resize(m_numInputMeshes);


	int bbPCountA[6] = {4,4,4,4,4,4};
	int bbPConnA[24] = { 0, 1, 2, 3, 4, 5, 6, 7, 3, 2, 6, 5, 0, 3, 5, 4, 0, 4, 7, 1, 1, 7, 6, 2 };

	float w,h,d;



	for (int m = 0; m < m_numInputMeshes; m++)
	{

		//// Collect mesh data
		MFnMesh mfnMesh(m_inMeshArray[m]);

		MBoundingBox bBox = mfnMesh.boundingBox(&status);

		w = bBox.width() * 0.5f;
		h = bBox.height() * 0.5f;
		d = bBox.depth() * 0.5f;

		MFloatPointArray bbPA;

		bbPA.append(MFloatPoint( -w,-h,-d, 1.0f ));
		bbPA.append(MFloatPoint( w,-h,-d, 1.0f ));
		bbPA.append(MFloatPoint( w,-h,d, 1.0f ));
		bbPA.append(MFloatPoint( -w,-h,d, 1.0f ));
		bbPA.append(MFloatPoint( -w,h,-d, 1.0f ));
		bbPA.append(MFloatPoint( -w,h,d, 1.0f ));
		bbPA.append(MFloatPoint( w,h,d, 1.0f ));
		bbPA.append(MFloatPoint( w,h,-d, 1.0f ));

		i_numVertices[m] = 8;
		i_numPolygons[m] = 6;



		i_vertexArray[m] = bbPA;
		i_polygonCounts[m] = MIntArray(bbPCountA,6);
		i_polygonConnects[m] = MIntArray(bbPConnA,24);


	}

	// Allocate memory for vector
	int len_o_vertexArray=0;
	int len_o_polygonCounts=0;
	int len_o_polygonConnects=0;

	for (int m = 0; m < m_numDup; m++)
	{
		for (unsigned v = 0; v < i_vertexArray[idA[m]].length(); v++) {
			len_o_vertexArray+=1;
		}
		for (unsigned v = 0; v < i_polygonCounts[idA[m]].length(); v++) {
			len_o_polygonCounts+=1;
		}
		for (unsigned v = 0; v < i_polygonConnects[idA[m]].length(); v++) {
			len_o_polygonConnects+=1;
		}

	}

	//
	o_numVertices = 0;
	o_numPolygons = 0;
	o_vertexArray.clear();
	o_polygonCounts.clear();
	o_polygonConnects.clear();

	status = o_vertexArray.setLength(len_o_vertexArray);
	CHECK_MSTATUS_AND_RETURN_IT(status);
	status = o_polygonCounts.setLength(len_o_polygonCounts);
	CHECK_MSTATUS_AND_RETURN_IT(status);
	status = o_polygonConnects.setLength(len_o_polygonConnects);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	for (int m = 0; m < m_numDup; m++)
	{

		// numVertices
		o_numVertices += i_numVertices[idA[m]];

		// numPolygons
		o_numPolygons += i_numPolygons[idA[m]];
	}

	int idOffset = 0;
	int polycOffset = 0;
	int polyConnOffset = 0;

	float offX = 0.0f;
	float offY = 0.0f;
	float offZ = 0.0f;

	for (int m = 0; m < m_numDup; m++)
	{


#pragma omp parallel for
		// vertexArray
		for (int v = 0; v < i_vertexArray[idA[m]].length(); v++) {
			MFloatPoint currP = i_vertexArray[idA[m]][v];

			MPoint currentPoint( currP.x, currP.y, currP.z, currP.w  );



			if (!m_worldSpace) { currentPoint = (MPoint(currentPoint) *= m_inMeshMatrixArray[idA[m]].inverse()); }

			currentPoint *= m_tr_matA[m];

			o_vertexArray.set(currentPoint,v + idOffset );
		}
#pragma omp parallel for
		// polygonCounts
		for (int v = 0; v < i_polygonCounts[idA[m]].length(); v++) {
			o_polygonCounts.set(i_polygonCounts[idA[m]][v], v+polycOffset);
		}



#pragma omp parallel for
		// polygonConnects
		for (int v = 0; v < i_polygonConnects[idA[m]].length(); v++) {
			o_polygonConnects.set(i_polygonConnects[idA[m]][v] +  idOffset,  v + polyConnOffset );


		}



		// Offsets for loops

		idOffset += i_vertexArray[idA[m]].length();
		polycOffset += i_polygonCounts[idA[m]].length();
		polyConnOffset += i_polygonConnects[idA[m]].length();

		offX += m_offsetX;
		offY += m_offsetY;
		offZ += m_offsetZ;


	}


	return MS::kSuccess;
}



MStatus ClonerMultiThread::reverseNormals()
{

	MStatus status;

	// Flip normals
	if (m_reverseNormals)
	{

		MIntArray revFCA;
		int co = o_polygonCounts.length() -1;
		for (unsigned i = 0; i <  o_polygonCounts.length() ; i++)
		{
			revFCA.append(o_polygonCounts[co]);
			co -= 1;
		}


		MIntArray revPCA;
		co = o_polygonConnects.length() -1;
		for (unsigned i = 0; i <  o_polygonConnects.length() ; i++)
		{
			revPCA.append(o_polygonConnects[co]);
			co -= 1;
		}

		MIntArray revUVCA;
		co = o_uvCountsA.length() -1;
		for (unsigned i = 0; i <  o_uvCountsA.length() ; i++)
		{
			revUVCA.append(o_uvCountsA[co]);
			co -= 1;
		}

		MIntArray revUVIDA;
		co = o_uvIdsA.length() -1;
		for (unsigned i = 0; i <  o_uvIdsA.length() ; i++)
		{
			revUVIDA.append(o_uvIdsA[co]);
			co -= 1;
		}




		o_polygonConnects = revPCA;
		o_polygonCounts = revFCA;

		o_uvCountsA = revUVCA;
		o_uvIdsA = revUVIDA;

	}

	return MStatus::kSuccess;
}


MStatus ClonerMultiThread::mergeUVs()
{
	MStatus status;


	// Clear arrays
	o_uArrayA.clear();
	o_vArrayA.clear();
	o_uvCountsA.clear();
	o_uvIdsA.clear();

	// Setup vectors for gathering base data from meshes
	MString				defaultUVSetName;
	MFloatArray         in_uArray;
	MFloatArray         in_vArray;
	MIntArray           in_uvCounts;
	MIntArray           in_uvIds;

	//vector <MString>		v_defaultUVSetName;
	vector <MFloatArray>	v_in_uArray;
	vector <MFloatArray>	v_in_vArray;
	vector <MIntArray>		v_in_uvCounts;
	vector <MIntArray>		v_in_uvIds;

	int vecSize = m_inMeshArray.length();

	//v_defaultUVSetName.resize(vecSize);
	v_in_uArray.resize(vecSize);
	v_in_vArray.resize(vecSize);
	v_in_uvCounts.resize(vecSize);
	v_in_uvIds.resize(vecSize);

	for (int i = 0; i < m_inMeshArray.length(); i++)
	{

		MFnMesh mFnA(m_inMeshArray[i]);

		//mFnA.getCurrentUVSetName(defaultUVSetName);
		mFnA.getUVs(in_uArray,in_vArray);
		//mFnA.getAssignedUVs(in_uvCounts, in_uvIds, &defaultUVSetName);
		mFnA.getAssignedUVs(in_uvCounts, in_uvIds);

		//v_defaultUVSetName[i] = defaultUVSetName;
		//o_defaultUVSetNameA = defaultUVSetName;
		v_in_uArray[i] = in_uArray;
		v_in_vArray[i] = in_vArray;
		v_in_uvCounts[i] = in_uvCounts;
		v_in_uvIds[i] = in_uvIds;

	}

	// Calculate the outpout array size

	int id = 0;
	int len_uArray=0;
	int len_vArray=0;
	int len_uvCounts=0;
	int len_uvIds=0;
	for (int m = 0; m < m_numInputMeshes; m++)
	{
		id = m;

		len_uArray += v_in_uArray[id].length();
		len_vArray += v_in_vArray[id].length();
		len_uvCounts += v_in_uvCounts[id].length();
		len_uvIds += v_in_uvIds[id].length();

	}



	// Set output Array length

	o_uArrayA.setLength(len_uArray);
	o_vArrayA.setLength(len_vArray);
	o_uvCountsA.setLength(len_uvCounts);
	o_uvIdsA.setLength(len_uvIds);

	int o_uArrayA_offset = 0;
	int o_vArrayA_offset = 0;
	int o_uvCountsA_offset = 0;
	int o_uvIdsA_offset = 0;

	id = 0;

	float off_tileU = 0.0;
	float off_tileV = 0.0;

	for (int m = 0; m < m_numInputMeshes; m++)
	{
		id = m;
#pragma omp parallel for
		for (int v = 0; v < v_in_uArray[id].length(); v++) {
			o_uArrayA.set(v_in_uArray[id][v] + off_tileU, v + o_uArrayA_offset);
		}
#pragma omp parallel for
		for (int v = 0; v < v_in_vArray[id].length(); v++) {
			o_vArrayA.set(v_in_vArray[id][v] + off_tileV, v + o_vArrayA_offset);
		}
#pragma omp parallel for
		for (int v = 0; v < v_in_uvCounts[id].length(); v++) {
			o_uvCountsA.set(v_in_uvCounts[id][v], v + o_uvCountsA_offset);
		}
#pragma omp parallel for
		for (int v = 0; v < v_in_uvIds[id].length(); v++) {
			o_uvIdsA.set(v_in_uvIds[id][v] + o_uArrayA_offset, v + o_uvIdsA_offset);
		}

		o_uArrayA_offset += v_in_uArray[id].length();
		o_vArrayA_offset += v_in_vArray[id].length();
		o_uvCountsA_offset += v_in_uvCounts[id].length();
		o_uvIdsA_offset += v_in_uvIds[id].length();

	}



	return MS::kSuccess;
}

MStatus ClonerMultiThread::duplicateUVs(MIntArray& idA)
{
	MStatus status;


	// Clear arrays
	o_uArrayA.clear();
	o_vArrayA.clear();
	o_uvCountsA.clear();
	o_uvIdsA.clear();


	// Setup vectors for gathering base data from meshes
	MString				defaultUVSetName;
	MFloatArray         in_uArray;
	MFloatArray         in_vArray;
	MIntArray           in_uvCounts;
	MIntArray           in_uvIds;

	//vector <MString>		v_defaultUVSetName;
	vector <MFloatArray>	v_in_uArray;
	vector <MFloatArray>	v_in_vArray;
	vector <MIntArray>		v_in_uvCounts;
	vector <MIntArray>		v_in_uvIds;

	int vecSize = m_inMeshArray.length();

	//v_defaultUVSetName.resize(vecSize);
	v_in_uArray.resize(vecSize);
	v_in_vArray.resize(vecSize);
	v_in_uvCounts.resize(vecSize);
	v_in_uvIds.resize(vecSize);

	for (int i = 0; i < m_inMeshArray.length(); i++)
	{

		MFnMesh mFnA(m_inMeshArray[i]);

		//mFnA.getCurrentUVSetName(defaultUVSetName);
		mFnA.getUVs(in_uArray,in_vArray);
		//mFnA.getAssignedUVs(in_uvCounts, in_uvIds, &defaultUVSetName);
		mFnA.getAssignedUVs(in_uvCounts, in_uvIds);

		//v_defaultUVSetName[i] = defaultUVSetName;
		//o_defaultUVSetNameA = defaultUVSetName;
		v_in_uArray[i] = in_uArray;
		v_in_vArray[i] = in_vArray;
		v_in_uvCounts[i] = in_uvCounts;
		v_in_uvIds[i] = in_uvIds;

	}

	// Calculate the outpout array size

	int id = 0;
	int len_uArray=0;
	int len_vArray=0;
	int len_uvCounts=0;
	int len_uvIds=0;

	for (int m = 0; m < m_numDup; m++)
	{
		id = idA[m];

		len_uArray += v_in_uArray[id].length();
		len_vArray += v_in_vArray[id].length();
		len_uvCounts += v_in_uvCounts[id].length();
		len_uvIds += v_in_uvIds[id].length();

	}



	// Set output Array length
	o_uArrayA.setLength(len_uArray);
	o_vArrayA.setLength(len_vArray);
	o_uvCountsA.setLength(len_uvCounts);
	o_uvIdsA.setLength(len_uvIds);

	int o_uArrayA_offset = 0;
	int o_vArrayA_offset = 0;
	int o_uvCountsA_offset = 0;
	int o_uvIdsA_offset = 0;

	id = 0;

	float off_tileU = 0.0;
	float off_tileV = 0.0;

	for (int m = 0; m < m_numDup; m++)
	{
		id = idA[m];
#pragma omp parallel for
		for (int v = 0; v < v_in_uArray[id].length(); v++) {
			o_uArrayA.set(v_in_uArray[id][v] + off_tileU, v + o_uArrayA_offset);
		}
#pragma omp parallel for
		for (int v = 0; v < v_in_vArray[id].length(); v++) {
			o_vArrayA.set(v_in_vArray[id][v] + off_tileV, v + o_vArrayA_offset);
		}
#pragma omp parallel for
		for (int v = 0; v < v_in_uvCounts[id].length(); v++) {
			o_uvCountsA.set(v_in_uvCounts[id][v], v + o_uvCountsA_offset);
		}
#pragma omp parallel for
		for (int v = 0; v < v_in_uvIds[id].length(); v++) {
			o_uvIdsA.set(v_in_uvIds[id][v] + o_uArrayA_offset, v + o_uvIdsA_offset);
		}

		o_uArrayA_offset += v_in_uArray[id].length();
		o_vArrayA_offset += v_in_vArray[id].length();
		o_uvCountsA_offset += v_in_uvCounts[id].length();
		o_uvIdsA_offset += v_in_uvIds[id].length();

		off_tileU += m_uvOffsetU;
		off_tileV += m_uvOffsetV;

		off_tileU += m_rndOffsetUA[m];
		off_tileV += m_rndOffsetVA[m];


		// If UDIM loop is set offset the uvs if they reach UDIM 1010
		if (m_uvUDIMLoop)
		{
			if (off_tileU >= 10.0)
			{
				off_tileU = 0.0;
				off_tileV += 1.0;
			}

		}


	}


	return MS::kSuccess;
}

MStatus ClonerMultiThread::smoothNormals(MFnMesh &meshFn)
{
	MStatus status;

	// Set edge smoothing globally
	for (int loop = 0; loop < meshFn.numEdges(); loop++) 
	{

		if (m_smoothNorm == true){ meshFn.setEdgeSmoothing(loop, true); }
		if (m_smoothNorm == false){ meshFn.setEdgeSmoothing(loop, false); }
	}

	meshFn.cleanupEdgeSmoothing();

	status = meshFn.updateSurface();
	CHECK_MSTATUS_AND_RETURN_IT(status);


	return MS::kSuccess;
}


MStatus ClonerMultiThread::collectInputMeshes(MDataBlock& data)
{
	MStatus status;

	// Clear mobject and matrix array
	m_inMeshArray.clear();
	m_inMeshMatrixArray.clear();

	// Check if input and output mesh is plugged in
	p_inMesh = MPlug(this->thisMObject(), aInMesh);
	p_outMesh = MPlug(this->thisMObject(), aOutMesh);


	MArrayDataHandle h_inMeshes = data.inputArrayValue( aInMesh );

	for (unsigned int i = 0; i < h_inMeshes.elementCount(); i++)
	{
		status = h_inMeshes.jumpToArrayElement(i);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		MObject m_inMesh = h_inMeshes.inputValue(&status).asMesh();

		if (m_inMesh.apiType() == MFn::kMeshData)
		{
			m_inMeshArray.append(m_inMesh);

			MMatrix trMat = h_inMeshes.inputValue(&status).geometryTransformMatrix();
			CHECK_MSTATUS_AND_RETURN_IT( status );

			m_inMeshMatrixArray.append(trMat);

		}

	}

	if (m_inMeshArray.length() == 0)
	{
		MGlobal::displayWarning(MString() + "[ClonerMulti] - No input / or / output meshes connected");
		return MS::kFailure;
	}


	m_numInputMeshes = m_inMeshArray.length();


	// MGlobal::displayInfo(MString() + m_numInputMeshes);

	return MStatus::kSuccess;
}


MStatus ClonerMultiThread::collectPlugs(MDataBlock& data)
{

	MStatus status;

	p_inMesh = MPlug(this->thisMObject(), aInMesh);
	p_outMesh = MPlug(this->thisMObject(), aOutMesh);
	p_inCurve = MPlug(this->thisMObject(), aInCurve);
	p_refMesh = MPlug(this->thisMObject(), aRefMesh);
	p_inLocA = MPlug(this->thisMObject(), aInLocAPos);
	p_inLocB = MPlug(this->thisMObject(), aInLocBPos);

	MString stringDataA = data.inputValue(aConnectArrayA, &status).asString();
	CHECK_MSTATUS_AND_RETURN_IT(status);
	MStringArray stringDataAArray;
	status = stringDataA.split(',', stringDataAArray);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	MString stringDataB = data.inputValue (aConnectArrayB, &status).asString();
	CHECK_MSTATUS_AND_RETURN_IT(status);
	MStringArray stringDataBArray;
	status = stringDataB.split(',', stringDataBArray);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	// put user input array into an mIntarray
	m_ConnectArrayA.clear();
	m_ConnectArrayB.clear();

	for(int i = 0; i < stringDataAArray.length(); i++ ){ m_ConnectArrayA.append(stringDataAArray[i].asInt()); }
	for(int i = 0; i < stringDataAArray.length(); i++ ){ m_ConnectArrayB.append(stringDataBArray[i].asInt()); }

	// If one vertex array is larger than the other expand it's size
	int maxValues[2] = {int(m_ConnectArrayA.length()), int(m_ConnectArrayB.length())};
	int maxValue = *std::min_element(maxValues,maxValues+2);

	m_ConnectArrayA.setLength(maxValue);
	m_ConnectArrayB.setLength(maxValue);



	m_inCurve = data.inputValue(aInCurve, &status).asNurbsCurve();
	CHECK_MSTATUS_AND_RETURN_IT(status);
	m_curveTrMat = data.inputValue(aInCurve, &status).geometryTransformMatrix();
	CHECK_MSTATUS_AND_RETURN_IT(status);

	m_refMesh = data.inputValue(aRefMesh, &status).asMesh();
	CHECK_MSTATUS_AND_RETURN_IT(status);
	m_refMeshMat = data.inputValue(aRefMesh, &status).geometryTransformMatrix();
	CHECK_MSTATUS_AND_RETURN_IT(status);

	m_inLocA_posMat = data.inputValue( aInLocAPos, &status ).asMatrix();
	CHECK_MSTATUS_AND_RETURN_IT(status);

	m_inLocB_posMat = data.inputValue( aInLocBPos, &status ).asMatrix();
	CHECK_MSTATUS_AND_RETURN_IT(status);

	m_firstUpVec = data.inputValue(aFirstUpVec, &status).asVector();
	CHECK_MSTATUS_AND_RETURN_IT(status);
	m_firstUpVec.normalize();

	h_outputMesh = data.outputValue(aOutMesh, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	h_outputMatrix = data.outputValue(aOutMatrixArray, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	h_outputID = data.outputValue(aOutIDArray, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	m_instanceType = data.inputValue(aInstanceType, &status).asShort();
	CHECK_MSTATUS_AND_RETURN_IT(status);
	m_patternType = data.inputValue(aPatterType, &status).asShort();
	CHECK_MSTATUS_AND_RETURN_IT(status);
	m_scatterType = data.inputValue(aScatterType, &status).asShort();
	CHECK_MSTATUS_AND_RETURN_IT(status);
	m_orientationType = data.inputValue(aOrientationType, &status).asShort();
	CHECK_MSTATUS_AND_RETURN_IT(status);
	
	m_id = data.inputValue(aIDType, &status).asInt();
	CHECK_MSTATUS_AND_RETURN_IT(status);
	m_rand_seed = data.inputValue(aSeedVal, &status).asInt();
	CHECK_MSTATUS_AND_RETURN_IT(status);

	m_polyLimit = data.inputValue(aLimitDisplay, &status).asInt();
	CHECK_MSTATUS_AND_RETURN_IT(status);

	m_instanceX = data.inputValue( aGridInstanceX, &status ).asInt();
	CHECK_MSTATUS_AND_RETURN_IT(status);
	m_instanceY = data.inputValue( aGridInstanceY, &status ).asInt();
	CHECK_MSTATUS_AND_RETURN_IT(status);
	m_instanceZ = data.inputValue( aGridInstanceZ, &status ).asInt();
	CHECK_MSTATUS_AND_RETURN_IT(status);



	m_offsetX = data.inputValue( aGridOffsetX, &status ).asFloat();
	CHECK_MSTATUS_AND_RETURN_IT(status);
	m_offsetY = data.inputValue( aGridOffsetY, &status ).asFloat();
	CHECK_MSTATUS_AND_RETURN_IT(status);
	m_offsetZ = data.inputValue( aGridOffsetZ, &status ).asFloat();
	CHECK_MSTATUS_AND_RETURN_IT(status);

	m_rotateX = data.inputValue( aRotateX, &status ).asFloat();
	CHECK_MSTATUS_AND_RETURN_IT(status);
	m_rotateY = data.inputValue( aRotateY, &status ).asFloat();
	CHECK_MSTATUS_AND_RETURN_IT(status);
	m_rotateZ = data.inputValue( aRotateZ, &status ).asFloat();
	CHECK_MSTATUS_AND_RETURN_IT(status);

	m_scaleX = data.inputValue( aScaleX, &status ).asFloat();
	CHECK_MSTATUS_AND_RETURN_IT(status);
	m_scaleY = data.inputValue( aScaleY, &status ).asFloat();
	CHECK_MSTATUS_AND_RETURN_IT(status);
	m_scaleZ = data.inputValue( aScaleZ, &status ).asFloat();
	CHECK_MSTATUS_AND_RETURN_IT(status);


	m_randomOffsetX = data.inputValue( aRndOffsetX, &status ).asFloat();
	CHECK_MSTATUS_AND_RETURN_IT(status);
	m_randomOffsetY = data.inputValue( aRndOffsetY, &status ).asFloat();
	CHECK_MSTATUS_AND_RETURN_IT(status);
	m_randomOffsetZ = data.inputValue( aRndOffsetZ, &status ).asFloat();
	CHECK_MSTATUS_AND_RETURN_IT(status);

	m_randomRotateX = data.inputValue( aRndRotateX, &status ).asFloat();
	CHECK_MSTATUS_AND_RETURN_IT(status);
	m_randomRotateY = data.inputValue( aRndRotateY, &status ).asFloat();
	CHECK_MSTATUS_AND_RETURN_IT(status);
	m_randomRotateZ = data.inputValue( aRndRotateZ, &status ).asFloat();
	CHECK_MSTATUS_AND_RETURN_IT(status);

	m_randomScaleX = data.inputValue( aRndScaleX, &status ).asFloat();
	CHECK_MSTATUS_AND_RETURN_IT(status);
	m_randomScaleY = data.inputValue( aRndScaleY, &status ).asFloat();
	CHECK_MSTATUS_AND_RETURN_IT(status);
	m_randomScaleZ = data.inputValue( aRndScaleZ, &status ).asFloat();
	CHECK_MSTATUS_AND_RETURN_IT(status);

	m_uvOffsetU = data.inputValue(aUVOffsetU, &status ).asFloat();
	CHECK_MSTATUS_AND_RETURN_IT(status);
	m_uvOffsetV = data.inputValue(aUVOffsetV, &status ).asFloat();
	CHECK_MSTATUS_AND_RETURN_IT(status);

	m_rndUvOffsetU = data.inputValue(aRndUVOffsetU, &status ).asFloat();
	CHECK_MSTATUS_AND_RETURN_IT(status);
	m_rndUvOffsetV = data.inputValue(aRndUVOffsetV, &status ).asFloat();
	CHECK_MSTATUS_AND_RETURN_IT(status);





	m_rndIDLev = data.inputValue(aRandIDLev, &status).asFloat();
	CHECK_MSTATUS_AND_RETURN_IT(status);

	m_reverseNormals = data.inputValue(aReverseNormals, &status).asBool();
	CHECK_MSTATUS_AND_RETURN_IT(status);

	m_smoothNorm = data.inputValue(aSmoothNormals, &status).asBool();
	CHECK_MSTATUS_AND_RETURN_IT(status);

	m_mergeInputMeshes = data.inputValue(aMergeInputMeshes, &status).asBool();
	CHECK_MSTATUS_AND_RETURN_IT(status);

	m_worldSpace = data.inputValue(aWorldSpace, &status).asBool();
	CHECK_MSTATUS_AND_RETURN_IT(status);

	m_revPattern = data.inputValue(aRevPattern, &status).asBool();
	CHECK_MSTATUS_AND_RETURN_IT(status);

	m_loopOffset = data.inputValue(aLoopOffset, &status).asBool();
	CHECK_MSTATUS_AND_RETURN_IT(status);

	m_uvUDIMLoop = data.inputValue(aUvUDIMLoop, &status).asBool();
	CHECK_MSTATUS_AND_RETURN_IT(status);

	m_outputMeshDisplayOverride = data.inputValue(aOutputMeshDisplayOverride, &status).asBool();
	CHECK_MSTATUS_AND_RETURN_IT(status);

	m_displayProxy = data.inputValue(aDisplayProxy, &status).asBool();
	CHECK_MSTATUS_AND_RETURN_IT(status);

	m_connectPieces = data.inputValue(aConnectPieces, &status).asBool();
	CHECK_MSTATUS_AND_RETURN_IT(status);

	// Override instace count if instance type is set to Circle
	if (m_instanceType == 1) { m_instanceZ = 1;}
	if (m_instanceType == 5) { m_instanceZ = 1;}
	//if (m_instanceType == 4) { m_rotateX+=180.0;}

	m_numDup = m_instanceX * m_instanceY * m_instanceZ; if (m_numDup < 1) { m_numDup = 1;}


	// Override instace count if instance type is set to Mesh
	if (m_instanceType == 5) {overrideInstanceOnMeshSettings();}

	return MStatus::kSuccess;

}


MFloatArray ClonerMultiThread::generateRndArray(float& surfaceNoise, int& numValues, int& seedVal)
{

	// Random Surface

	MFloatArray randArray;
	srand(seedVal);

	float randVal;

	for (int i=0; i < numValues; i++) {

		randVal = (rand() / ((float) RAND_MAX)) * surfaceNoise;
		randArray.append(randVal);

	}

	return randArray;

}

MStatus ClonerMultiThread::checkInputPlugs()
{

	MStatus status;

	p_inMesh = MPlug(this->thisMObject(), aInMesh);
	p_outMesh = MPlug(this->thisMObject(), aOutMesh);
	p_inCurve = MPlug(this->thisMObject(), aInCurve);
	p_refMesh = MPlug(this->thisMObject(), aRefMesh);
	p_inLocA = MPlug(this->thisMObject(), aInLocAPos);
	p_inLocB = MPlug(this->thisMObject(), aInLocBPos);

	//eAttr.addField("Grid", 0);
	//eAttr.addField("Circular", 1);
	//eAttr.addField("Sphere", 2);
	//eAttr.addField("A - B", 3);
	//eAttr.addField("Spline", 4);
	//eAttr.addField("Input geo", 5);


	if (!p_inMesh.isConnected())
	{
		return MStatus::kSuccess;
	}

	if (!p_outMesh.isConnected())
	{
		return MStatus::kSuccess;
	}

	if (m_instanceType==3)
	{

		if (!p_inLocA.isConnected())
		{
			return MStatus::kSuccess;
		}

		if (!p_inLocB.isConnected())
		{
			return MStatus::kSuccess;
		}
	}


	if (m_instanceType==4)
	{
		if (!p_inCurve.isConnected())
		{
			return MStatus::kSuccess;
		}
	}

	if (m_instanceType==5)
	{
		if (!p_refMesh.isConnected())
		{
			return MStatus::kSuccess;
		}
	}

	return MStatus::kSuccess;
}



MStatus ClonerMultiThread::displayOverride()
{

	MStatus status;

	if (p_outMesh.isConnected())
	{
		// -----------------------------------------------
		// Collect output plug mesh's name
		//status = p_outMesh.selectAncestorLogicalIndex(0);
		//CHECK_MSTATUS_AND_RETURN_IT(status);
		MPlugArray outputs_plugArr;
		p_outMesh.connectedTo(outputs_plugArr, false, true, &status);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		if (outputs_plugArr.length() > 0)
		{
			MPlug outMeshPlug_shape = outputs_plugArr[0];
			MFnDependencyNode outMesh_dn(outMeshPlug_shape.node());


			MPlug p_out_overrideEnabled = outMesh_dn.findPlug("overrideEnabled", false, &status);
			CHECK_MSTATUS_AND_RETURN_IT(status);
			p_out_overrideEnabled.setBool(m_outputMeshDisplayOverride);

			MPlug p_out_overrideDisplayType = outMesh_dn.findPlug("overrideDisplayType", false, &status);
			CHECK_MSTATUS_AND_RETURN_IT(status);
			p_out_overrideDisplayType.setInt(2);
		}

	}


	return MS::kSuccess;
}



// --- COMPUTE ----------------------------------------------------------------------------------
MStatus ClonerMultiThread::compute( const MPlug& plug, MDataBlock& data )
{
	MStatus status;

	// collect the input plug data
	status = collectPlugs(data);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	if (m_numDup < 1)
	{
		return MStatus::kSuccess;
	}

	if (p_inMesh.numConnectedElements() == 0)
	{
		// No input mesh attached so exit node.
		return MS::kSuccess;
	}




	if ( plug == aOutMesh ) 
	{

		// Collect input meshes
		status = collectInputMeshes(data);
		CHECK_MSTATUS_AND_RETURN_IT(status);



		// Generate IDs
		m_idA = generatePatternIDs(m_patternType, m_numDup);

		// Generate Random Arrays
		srand(m_rand_seed);
		m_rndOffsetXA = generateRndArray(m_randomOffsetX, m_numDup, m_rand_seed);
		m_rndOffsetYA = generateRndArray(m_randomOffsetY, m_numDup, m_rand_seed);
		m_rndOffsetZA = generateRndArray(m_randomOffsetZ, m_numDup, m_rand_seed);
		m_rndRotateXA = generateRndArray(m_randomRotateX, m_numDup, m_rand_seed);
		m_rndRotateYA = generateRndArray(m_randomRotateY, m_numDup, m_rand_seed);
		m_rndRotateZA = generateRndArray(m_randomRotateZ, m_numDup, m_rand_seed);
		m_rndScaleXA = generateRndArray(m_randomScaleX, m_numDup, m_rand_seed);
		m_rndScaleYA = generateRndArray(m_randomScaleY, m_numDup, m_rand_seed);
		m_rndScaleZA = generateRndArray(m_randomScaleZ, m_numDup, m_rand_seed);
		m_rndOffsetUA = generateRndArray(m_rndUvOffsetU, m_numDup, m_rand_seed);
		m_rndOffsetVA = generateRndArray(m_rndUvOffsetV, m_numDup, m_rand_seed);

		// Generate transormation matrix array
		if (m_instanceType == 0) { status = instanceGrid(); CHECK_MSTATUS_AND_RETURN_IT(status);}
		else if (m_instanceType == 1) { status = instanceCircle(); CHECK_MSTATUS_AND_RETURN_IT(status);}
		else if (m_instanceType == 2) { status = instanceFibonacciSphere(); CHECK_MSTATUS_AND_RETURN_IT(status);}

		else if (m_instanceType == 3) 
		{ 

			if (p_inLocA.isConnected() && p_inLocB.isConnected())
			{
				status = instanceAtoB(); CHECK_MSTATUS_AND_RETURN_IT(status);
			}

			else
			{
				status = instanceGrid(); CHECK_MSTATUS_AND_RETURN_IT(status);
			}
		}

		else if (m_instanceType == 4) 
		{
			if (p_inCurve.isConnected())
			{
				status = instanceSpline(); CHECK_MSTATUS_AND_RETURN_IT(status);
			}

			else
			{
				status = instanceGrid(); CHECK_MSTATUS_AND_RETURN_IT(status);
			}

		}

		else if (m_instanceType == 5)
		{ 
			if (p_refMesh.isConnected())
			{
				status = instanceOnMesh(); CHECK_MSTATUS_AND_RETURN_IT(status);
			}

			else
			{
				status = instanceGrid(); CHECK_MSTATUS_AND_RETURN_IT(status);
			}
		}

		else { status = instanceGrid(); CHECK_MSTATUS_AND_RETURN_IT(status);}


		// Create output mobject
		MFnMeshData meshDataFn;
		MObject newMeshData = meshDataFn.create(&status);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		MFnMeshData ex_meshDataFn;
		MObject ex_newMeshData = ex_meshDataFn.create(&status);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		// Construct combined Meshes
		MFnMesh meshFn;
		MFnMesh ex_meshFn;

		if (!m_displayProxy)
		{



			if (m_mergeInputMeshes)
			{

				// Merge uv's
				status = mergeUVs();
				CHECK_MSTATUS_AND_RETURN_IT(status);

				//Merge input meshes
				status = mergeInputMeshes();
				CHECK_MSTATUS_AND_RETURN_IT(status);

				meshFn.create(o_numVertices, o_numPolygons, o_vertexArray, o_polygonCounts, o_polygonConnects, o_uArrayA, o_vArrayA, newMeshData, &status);
				CHECK_MSTATUS_AND_RETURN_IT(status);
				//status = meshFn.assignUVs(o_uvCountsA, o_uvIdsA, &o_defaultUVSetNameA);
				status = meshFn.assignUVs(o_uvCountsA, o_uvIdsA);
				CHECK_MSTATUS_AND_RETURN_IT(status);

				m_inMeshArray.clear();
				m_inMeshArray.append(newMeshData);
				m_numInputMeshes = 1;
				m_idA.clear(); for (int i = 0; i < m_numDup; i++) {m_idA.append(0);}


				status = duplicateUVs(m_idA);
				CHECK_MSTATUS_AND_RETURN_IT(status);

				status = duplicateInputMeshes(m_idA);
				CHECK_MSTATUS_AND_RETURN_IT(status);

				ex_meshFn.create(o_numVertices, o_numPolygons, o_vertexArray, o_polygonCounts, o_polygonConnects, o_uArrayA, o_vArrayA, ex_newMeshData, &status);
				CHECK_MSTATUS_AND_RETURN_IT(status);
				//status = ex_meshFn.assignUVs(o_uvCountsA, o_uvIdsA, &o_defaultUVSetNameA);
				status = ex_meshFn.assignUVs(o_uvCountsA, o_uvIdsA);
				CHECK_MSTATUS_AND_RETURN_IT(status);

				// Smooth the normals globally
				smoothNormals(ex_meshFn);

				// Send mesh to output datablock
				status = h_outputMesh.set(ex_newMeshData);
				CHECK_MSTATUS_AND_RETURN_IT(status);


			}

			else
			{


				status = duplicateUVs(m_idA);
				CHECK_MSTATUS_AND_RETURN_IT(status);

				status = duplicateInputMeshes(m_idA);
				CHECK_MSTATUS_AND_RETURN_IT(status);

				ex_meshFn.create(o_numVertices, o_numPolygons, o_vertexArray, o_polygonCounts, o_polygonConnects, o_uArrayA, o_vArrayA, ex_newMeshData, &status);
				CHECK_MSTATUS_AND_RETURN_IT(status);
				//status = ex_meshFn.assignUVs(o_uvCountsA, o_uvIdsA, &o_defaultUVSetNameA);
				status = ex_meshFn.assignUVs(o_uvCountsA, o_uvIdsA);
				CHECK_MSTATUS_AND_RETURN_IT(status);


				// Smooth the normals globally
				smoothNormals(ex_meshFn);

				// Send mesh to output datablock
				status = h_outputMesh.set(ex_newMeshData);
				CHECK_MSTATUS_AND_RETURN_IT(status);

			}
		}

		if (m_displayProxy)
		{
			o_numVertices = 0;
			o_numPolygons = 0;
			o_vertexArray.clear();
			o_polygonCounts.clear();
			o_polygonConnects.clear();

			ex_meshFn.create(o_numVertices, o_numPolygons, o_vertexArray, o_polygonCounts, o_polygonConnects, ex_newMeshData, &status);
			CHECK_MSTATUS_AND_RETURN_IT(status);

			// Send mesh to output datablock
			status = h_outputMesh.set(ex_newMeshData);
			CHECK_MSTATUS_AND_RETURN_IT(status);

		}


		// Output matrix
		MFnMatrixArrayData ex_matrixDataFn;
		MObject ex_matrixData = ex_matrixDataFn.create(m_tr_matA, &status);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		// Send matrix array to output datablock
		status = h_outputMatrix.set(ex_matrixData);
		CHECK_MSTATUS_AND_RETURN_IT(status);


		// Output ID array
		MFnIntArrayData ex_idDataFn;
		MObject ex_idData = ex_idDataFn.create(m_idA, &status);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		// Send ID array to output datablock
		status = h_outputID.set(ex_idData);
		CHECK_MSTATUS_AND_RETURN_IT(status);


		displayOverride();
	}

	return MS::kSuccess;
}















// ----------------------------------------------------------------------------------------------------------------------------------------------------


// VP 1.0 Functions
void ClonerMultiThread::draw( M3dView & view, const MDagPath & path, M3dView::DisplayStyle style,  M3dView::DisplayStatus status )
{
	MObject thisNode = thisMObject();

	MFnDependencyNode fnDepCloner( thisNode );

	// Draw locator
	view.beginGL();

	glPushAttrib( GL_CURRENT_BIT );

	if ( status == M3dView::kActive ) {
		view.setDrawColor( 13, M3dView::kActiveColors );
	} else {
		view.setDrawColor( 13, M3dView::kDormantColors );
	}


	view.drawText( ".", MPoint::origin, M3dView::kCenter );



	glPopAttrib();

	view.endGL();

}


// VP 2.0 Override functions

MHWRender::DrawAPI ClonerMultiThreadOverride::supportedDrawAPIs() const
{

#if MAYA_API_VERSION > 201600

	return (MHWRender::kOpenGL | MHWRender::kDirectX11 | MHWRender::kOpenGLCoreProfile );

#else
	return (MHWRender::kOpenGL | MHWRender::kDirectX11 );
#endif

}

bool ClonerMultiThreadOverride::isBounded(const MDagPath& /*objPath*/, const MDagPath& /*cameraPath*/) const
{
	return true;
}

MBoundingBox ClonerMultiThreadOverride::boundingBox( const MDagPath& objPath, const MDagPath& cameraPath) const
{


	MStatus status;
	MObject CLonerMultiNode = objPath.node(&status);


	MPoint corner1( -1.0, 1.0, -1.0 );
	MPoint corner2( 1.0, 0.0, 1.0 );

	return MBoundingBox( corner1, corner2 );

	//return m_bbP;
}

MPointArray ClonerMultiThreadOverride::getInstancePoints(const MDagPath& objPath) const
{
	MStatus status;

	MObject locatorNode = objPath.node(&status);

	if (!status)
	{
		return MPointArray();
	}


	MPointArray currPA;

	MPlug pointsPlug(locatorNode, ClonerMultiThread::aOutMatrixArray);
	MPlug inMeshPlug(locatorNode, ClonerMultiThread::aInMesh);


	if (inMeshPlug.numConnectedElements() != 0)
	{
		if (!pointsPlug.isNull())
		{

			MObject matrixObject;

			pointsPlug.getValue(matrixObject);

			if (status)
			{

				MFnMatrixArrayData worldMatrixData( matrixObject );
				MMatrixArray out_matrixArray;

				worldMatrixData.copyTo(out_matrixArray);


				currPA.setLength(out_matrixArray.length());

				for (int i = 0; i < out_matrixArray.length(); i++)
				{
					currPA[i] = MPoint() * out_matrixArray[i];
				}
			}
		}
	}

	return currPA;
}



// Called by Maya each time the object needs to be drawn.
MUserData* ClonerMultiThreadOverride::prepareForDraw( const MDagPath& objPath, const MDagPath& cameraPath, const MHWRender::MFrameContext& frameContext, MUserData* oldData)
{

	// Get outside data from plugs
	MStatus status;
	MObject CLonerMultiNode = objPath.node(&status);


	//

	// Add data
	ClonerMultiThreadData* data = dynamic_cast<ClonerMultiThreadData*>(oldData);
	if (!data)
	{
		data = new ClonerMultiThreadData();
	}


	MPlug dispProxyPlug(objPath.node(), ClonerMultiThread::aDisplayProxy);

	data->m_displayProxy = dispProxyPlug.asBool();

	data->m_dispPointA = getInstancePoints(objPath);

	data->m_inLoc_mat = objPath.exclusiveMatrix();

	// get correct color based on the state of object, e.g. active or dormant
	data->m_locColor = MHWRender::MGeometryUtilities::wireframeColor(objPath);

	return data;
}


void ClonerMultiThreadOverride::addUIDrawables( const MDagPath& objPath, MHWRender::MUIDrawManager& drawManager, const MHWRender::MFrameContext& frameContext, const MUserData* data)
{
	ClonerMultiThreadData* pLocatorData = (ClonerMultiThreadData*)data;
	if (!pLocatorData)
	{
		return;
	}

	drawManager.beginDrawable();



	MColor fillCol, lineCol;

	fillCol = MColor( 1.0f, 0.5f, 0.3f, 1.0f );



	if ( frameContext.getDisplayStyle() & MHWRender::MDrawContext::kWireFrame ) {


		fillCol = MColor( pLocatorData->m_locColor.r, pLocatorData->m_locColor.g, pLocatorData->m_locColor.b, 0.0 );

	}

	if ( MHWRender::MGeometryUtilities::displayStatus(objPath) == M3dView::kLead ) {


		fillCol = MColor( pLocatorData->m_locColor.r, pLocatorData->m_locColor.g, pLocatorData->m_locColor.b, fillCol.a );

	}

	if ( MHWRender::MGeometryUtilities::displayStatus(objPath) == M3dView::kActive ) {

		fillCol = MColor( pLocatorData->m_locColor.r, pLocatorData->m_locColor.g, pLocatorData->m_locColor.b, fillCol.a );

	}

	if ( MHWRender::MGeometryUtilities::displayStatus(objPath) == M3dView::kTemplate ) {

		fillCol = MColor( pLocatorData->m_locColor.r, pLocatorData->m_locColor.g, pLocatorData->m_locColor.b, fillCol.a );

	}



	if (pLocatorData->m_displayProxy)
	{

		drawManager.setColor( MColor(fillCol.r,fillCol.g,fillCol.b, 1.0f) );
		drawManager.setPointSize(4);
		drawManager.mesh(MHWRender::MUIDrawManager::kPoints,  pLocatorData->m_dispPointA);


		M3dView view = M3dView::active3dView();
		short ox, oy;

		MPoint p = MPoint::origin;
		p *= pLocatorData->m_inLoc_mat;

		view.worldToView(p, ox, oy);

		drawManager.setColor( MColor(fillCol.r + 0.5,fillCol.g + 0.5,fillCol.b + 0.5, 1.0f) );
		drawManager.circle2d(MPoint(ox,oy),3.0, false );

	}


	else
	{
		M3dView view = M3dView::active3dView();
		short ox, oy;

		MPoint p = MPoint::origin;
		p *= pLocatorData->m_inLoc_mat;

		view.worldToView(p, ox, oy);

		drawManager.setColor( MColor(fillCol.r + 0.5,fillCol.g + 0.5,fillCol.b + 0.5, 1.0f) );
		drawManager.circle2d(MPoint(ox,oy),3.0, false );
	}



	drawManager.endDrawable();
}




MStatus ClonerMultiThread::initialize()
{

	MStatus status;


	MFnTypedAttribute			tAttr;
	MFnNumericAttribute			nAttr;
	MFnEnumAttribute			eAttr;
	MFnCompoundAttribute        cAttr;
	MFnMatrixAttribute			mAttr;

	MFnStringData      fnStringData;
	MObject            defaultStringA;
	defaultStringA = fnStringData.create( "0,1" );


	// Plugs
	ClonerMultiThread::aInMesh = tAttr.create( "inMesh", "inMesh", MFnData::kMesh );
	tAttr.setDisconnectBehavior(MFnAttribute::kDelete);
	tAttr.setChannelBox( false );
	tAttr.setArray(true);
	tAttr.setWritable(true);
	tAttr.setReadable(false);
	addAttribute( ClonerMultiThread::aInMesh );

	ClonerMultiThread::aOutMesh = tAttr.create( "outMesh", "outMesh", MFnData::kMesh );
	tAttr.setChannelBox( false );
	tAttr.setWritable(false);
	tAttr.setReadable(true);
	addAttribute( ClonerMultiThread::aOutMesh );

	ClonerMultiThread::aOutMatrixArray = tAttr.create( "outMatrixArray", "outMatrixArray", MFnMatrixData::kMatrixArray );
	tAttr.setChannelBox( false );
	tAttr.setWritable(false);
	tAttr.setReadable(true);
	tAttr.setHidden(true);
	addAttribute( ClonerMultiThread::aOutMatrixArray );

	ClonerMultiThread::aOutIDArray = tAttr.create( "outIDArray", "outIDArray", MFnIntArrayData::kIntArray );
	tAttr.setChannelBox( false );
	tAttr.setWritable(false);
	tAttr.setReadable(true);
	tAttr.setHidden(true);
	addAttribute( ClonerMultiThread::aOutIDArray );

	ClonerMultiThread::aRefMesh = tAttr.create( "referenceMesh", "referenceMesh", MFnData::kMesh );
	tAttr.setWritable(true);
	tAttr.setReadable(false);
	tAttr.setStorable(false);
	tAttr.setKeyable(true);
	addAttribute( ClonerMultiThread::aRefMesh );

	ClonerMultiThread::aInCurve = tAttr.create("inCurve", "inCurve", MFnData::kNurbsCurve);
	tAttr.setWritable(true);
	tAttr.setReadable(false);
	tAttr.setStorable(false);
	tAttr.setKeyable(true);
	addAttribute(ClonerMultiThread::aInCurve);

	// A - B in locators
	ClonerMultiThread::aInLocAPos = mAttr.create( "locatorAPos", "locatorAPos", MFnMatrixAttribute::kDouble );
	mAttr.setChannelBox(false);
	mAttr.setWritable(true);
	mAttr.setReadable(false);
	mAttr.setStorable(false);
	mAttr.setKeyable(false);
	addAttribute(ClonerMultiThread::aInLocAPos);

	ClonerMultiThread::aInLocBPos = mAttr.create( "locatorBPos", "locatorBPos", MFnMatrixAttribute::kDouble );
	mAttr.setChannelBox(false);
	mAttr.setWritable(true);
	mAttr.setReadable(false);
	mAttr.setStorable(false);
	mAttr.setKeyable(false);
	addAttribute(ClonerMultiThread::aInLocBPos);




	// Types
	ClonerMultiThread::aInstanceType = eAttr.create("instanceType", "instanceType", 0);
	eAttr.addField("Grid", 0);
	eAttr.addField("Circular", 1);
	eAttr.addField("Sphere", 2);
	eAttr.addField("A - B", 3);
	eAttr.addField("Spline", 4);
	eAttr.addField("Input geo", 5);
	eAttr.setStorable(true);
	//eAttr.setInternal(true);
	addAttribute(ClonerMultiThread::aInstanceType);

	ClonerMultiThread::aScatterType = eAttr.create("scatterType", "scatterType", 0);
	eAttr.addField("UV", 0);
	eAttr.addField("Vertex", 1);
	eAttr.addField("Polygon", 2);
	eAttr.setStorable(true);
	addAttribute(ClonerMultiThread::aScatterType);


	ClonerMultiThread::aPatterType = eAttr.create("patternType", "patternType", 1);
	eAttr.addField("ID", 0);
	eAttr.addField("Order Loop Forward", 1);
	eAttr.addField("Order Loop Backwards", 2);
	eAttr.addField("Random", 3);
	eAttr.addField("Start / Mid / End", 4);
	eAttr.addField("Loop / End", 5);
	eAttr.addField("End / Loop", 6);
	eAttr.addField("Start / Mid / Start Mirror", 7);
	eAttr.setStorable(true);
	addAttribute(ClonerMultiThread::aPatterType);

	// Orientation
	ClonerMultiThread::aOrientationType = eAttr.create("orientationType", "orientationType", 0);
	eAttr.addField("Auto", 0);
	eAttr.addField("X", 1);
	eAttr.addField("Y", 2);
	eAttr.addField("Z", 3);
	eAttr.setStorable(true);
	//eAttr.setInternal(true);
	addAttribute(ClonerMultiThread::aOrientationType);

	ClonerMultiThread::aLimitDisplay = nAttr.create("polyDisplayLimit", "polyDisplayLimit", MFnNumericData::kInt);
	nAttr.setDefault(50000);
	nAttr.setKeyable(true);
	nAttr.setChannelBox(true);
	addAttribute(ClonerMultiThread::aLimitDisplay);

	ClonerMultiThread::aIDType = nAttr.create("id", "id", MFnNumericData::kInt);
	nAttr.setDefault(0);
	nAttr.setMin(0);
	nAttr.setKeyable(true);
	nAttr.setChannelBox(true);
	eAttr.setStorable(true);
	addAttribute(ClonerMultiThread::aIDType);

	ClonerMultiThread::aSeedVal = nAttr.create("randomSeed", "randomSeed", MFnNumericData::kInt);
	nAttr.setDefault(4354);
	nAttr.setKeyable(true);
	nAttr.setChannelBox(true);
	addAttribute(ClonerMultiThread::aSeedVal);

	// Instance
	ClonerMultiThread::aGridInstanceX = nAttr.create("instanceX", "instanceX", MFnNumericData::kInt);
	nAttr.setStorable(true);
	nAttr.setDefault(10);
	nAttr.setMin(1);
	nAttr.setSoftMax(10);
	nAttr.setKeyable(true);
	nAttr.setChannelBox(true);
	addAttribute(ClonerMultiThread::aGridInstanceX);

	ClonerMultiThread::aGridInstanceY = nAttr.create("instanceY", "instanceY", MFnNumericData::kInt);
	nAttr.setStorable(true);
	nAttr.setDefault(1);
	nAttr.setMin(1);
	nAttr.setSoftMax(10);
	nAttr.setKeyable(true);
	nAttr.setChannelBox(true);
	addAttribute(ClonerMultiThread::aGridInstanceY);

	ClonerMultiThread::aGridInstanceZ = nAttr.create("instanceZ", "instanceZ", MFnNumericData::kInt);
	nAttr.setStorable(true);
	nAttr.setDefault(1);
	nAttr.setMin(1);
	nAttr.setSoftMax(10);
	nAttr.setKeyable(true);
	nAttr.setChannelBox(true);
	addAttribute(ClonerMultiThread::aGridInstanceZ);




	// Offset
	ClonerMultiThread::aGridOffsetX = nAttr.create("offsetX", "offsetX", MFnNumericData::kFloat);
	nAttr.setStorable(true);
	nAttr.setDefault(1.0);
	nAttr.setSoftMin(0.0);
	nAttr.setSoftMax(1.0);
	nAttr.setKeyable(true);
	nAttr.setChannelBox(true);
	addAttribute(aGridOffsetX);

	ClonerMultiThread::aGridOffsetY = nAttr.create("offsetY", "offsetY", MFnNumericData::kFloat);
	nAttr.setStorable(true);
	nAttr.setDefault(1.0);
	nAttr.setSoftMin(0.0);
	nAttr.setSoftMax(1.0);
	nAttr.setKeyable(true);
	nAttr.setChannelBox(true);
	addAttribute(ClonerMultiThread::aGridOffsetY);

	ClonerMultiThread::aGridOffsetZ = nAttr.create("offsetZ", "offsetZ", MFnNumericData::kFloat);
	nAttr.setStorable(true);
	nAttr.setDefault(1.0);
	nAttr.setSoftMin(0.0);
	nAttr.setSoftMax(1.0);
	nAttr.setKeyable(true);
	nAttr.setChannelBox(true);
	addAttribute(ClonerMultiThread::aGridOffsetZ);


	// Rotate
	ClonerMultiThread::aRotateX = nAttr.create("rotateX", "rotateX", MFnNumericData::kFloat);
	nAttr.setStorable(true);
	nAttr.setDefault(0.0);
	nAttr.setSoftMax(360.0);
	nAttr.setSoftMin(-360.0);
	nAttr.setKeyable(true);
	nAttr.setChannelBox(true);
	addAttribute(ClonerMultiThread::aRotateX);

	ClonerMultiThread::aRotateY = nAttr.create("rotateY", "rotateY", MFnNumericData::kFloat);
	nAttr.setStorable(true);
	nAttr.setDefault(0.0);
	nAttr.setSoftMax(360.0);
	nAttr.setSoftMin(-360.0);
	nAttr.setKeyable(true);
	nAttr.setChannelBox(true);
	addAttribute(ClonerMultiThread::aRotateY);

	ClonerMultiThread::aRotateZ = nAttr.create("rotateZ", "rotateZ", MFnNumericData::kFloat);
	nAttr.setStorable(true);
	nAttr.setDefault(0.0);
	nAttr.setSoftMax(360.0);
	nAttr.setSoftMin(-360.0);
	nAttr.setKeyable(true);
	nAttr.setChannelBox(true);
	addAttribute(ClonerMultiThread::aRotateZ);

	// Scale
	ClonerMultiThread::aScaleX = nAttr.create("scaleX", "scaleX", MFnNumericData::kFloat);
	nAttr.setStorable(true);
	nAttr.setDefault(1.0);
	nAttr.setSoftMax(1.0);
	nAttr.setSoftMin(0.0);
	nAttr.setKeyable(true);
	nAttr.setChannelBox(true);
	addAttribute(ClonerMultiThread::aScaleX);

	ClonerMultiThread::aScaleY = nAttr.create("scaleY", "scaleY", MFnNumericData::kFloat);
	nAttr.setStorable(true);
	nAttr.setDefault(1.0);
	nAttr.setSoftMax(1.0);
	nAttr.setSoftMin(0.0);
	nAttr.setKeyable(true);
	nAttr.setChannelBox(true);
	addAttribute(ClonerMultiThread::aScaleY);

	ClonerMultiThread::aScaleZ = nAttr.create("scaleZ", "scaleZ", MFnNumericData::kFloat);
	nAttr.setStorable(true);
	nAttr.setDefault(1.0);
	nAttr.setSoftMax(1.0);
	nAttr.setSoftMin(0.0);
	nAttr.setKeyable(true);
	nAttr.setChannelBox(true);
	addAttribute(ClonerMultiThread::aScaleZ);

	// Randomize Translate
	ClonerMultiThread::aRndOffsetX = nAttr.create("randomizeOffsetX", "randomizeOffsetX", MFnNumericData::kFloat);
	nAttr.setStorable(true);
	nAttr.setDefault(0.0);
	nAttr.setSoftMax(1.0);
	nAttr.setSoftMin(0.0);
	nAttr.setKeyable(true);
	nAttr.setChannelBox(true);
	addAttribute(ClonerMultiThread::aRndOffsetX);

	ClonerMultiThread::aRndOffsetY = nAttr.create("randomizeOffsetY", "randomizeOffsetY", MFnNumericData::kFloat);
	nAttr.setStorable(true);
	nAttr.setDefault(0.0);
	nAttr.setSoftMax(1.0);
	nAttr.setSoftMin(0.0);
	nAttr.setKeyable(true);
	nAttr.setChannelBox(true);
	addAttribute(ClonerMultiThread::aRndOffsetY);

	ClonerMultiThread::aRndOffsetZ = nAttr.create("randomizeOffsetZ", "randomizeOffsetZ", MFnNumericData::kFloat);
	nAttr.setStorable(true);
	nAttr.setDefault(0.0);
	nAttr.setSoftMax(1.0);
	nAttr.setSoftMin(0.0);
	nAttr.setKeyable(true);
	nAttr.setChannelBox(true);
	addAttribute(ClonerMultiThread::aRndOffsetZ);

	// Randomize Rotate
	ClonerMultiThread::aRndRotateX = nAttr.create("randomizeRotateX", "randomizeRotateX", MFnNumericData::kFloat);
	nAttr.setStorable(true);
	nAttr.setDefault(0.0);
	nAttr.setSoftMax(360.0);
	nAttr.setSoftMin(-360.0);
	nAttr.setKeyable(true);
	nAttr.setChannelBox(true);
	addAttribute(ClonerMultiThread::aRndRotateX);

	ClonerMultiThread::aRndRotateY = nAttr.create("randomizeRotateY", "randomizeRotateY", MFnNumericData::kFloat);
	nAttr.setStorable(true);
	nAttr.setDefault(0.0);
	nAttr.setSoftMax(360.0);
	nAttr.setSoftMin(-360.0);
	nAttr.setKeyable(true);
	nAttr.setChannelBox(true);
	addAttribute(ClonerMultiThread::aRndRotateY);

	ClonerMultiThread::aRndRotateZ = nAttr.create("randomizeRotateZ", "randomizeRotateZ", MFnNumericData::kFloat);
	nAttr.setStorable(true);
	nAttr.setDefault(0.0);
	nAttr.setSoftMax(360.0);
	nAttr.setSoftMin(-360.0);
	nAttr.setKeyable(true);
	nAttr.setChannelBox(true);
	addAttribute(ClonerMultiThread::aRndRotateZ);

	// Randomize Scale
	ClonerMultiThread::aRndScaleX = nAttr.create("randomizeScaleX", "randomizeScaleX", MFnNumericData::kFloat);
	nAttr.setStorable(true);
	nAttr.setDefault(0.0);
	nAttr.setSoftMax(1.0);
	nAttr.setSoftMin(0.0);
	nAttr.setKeyable(true);
	nAttr.setChannelBox(true);
	addAttribute(ClonerMultiThread::aRndScaleX);

	ClonerMultiThread::aRndScaleY = nAttr.create("randomizeScaleY", "randomizeScaleY", MFnNumericData::kFloat);
	nAttr.setStorable(true);
	nAttr.setDefault(0.0);
	nAttr.setSoftMax(1.0);
	nAttr.setSoftMin(0.0);
	nAttr.setKeyable(true);
	nAttr.setChannelBox(true);
	addAttribute(ClonerMultiThread::aRndScaleY);

	ClonerMultiThread::aRndScaleZ = nAttr.create("randomizeScaleZ", "randomizeScaleZ", MFnNumericData::kFloat);
	nAttr.setStorable(true);
	nAttr.setDefault(0.0);
	nAttr.setSoftMax(1.0);
	nAttr.setSoftMin(0.0);
	nAttr.setKeyable(true);
	nAttr.setChannelBox(true);
	addAttribute(ClonerMultiThread::aRndScaleZ);

	ClonerMultiThread::aRandIDLev = nAttr.create("randomIDGapLevel", "randomIDGapLevel", MFnNumericData::kFloat);
	nAttr.setStorable(true);
	nAttr.setDefault(0.0);
	nAttr.setMin(0.0);
	nAttr.setMax(1.0);
	nAttr.setKeyable(true);
	nAttr.setChannelBox(true);
	addAttribute(ClonerMultiThread::aRandIDLev);

	ClonerMultiThread::aUVOffsetU = nAttr.create("uvOffsetU", "uvOffsetU", MFnNumericData::kFloat);
	nAttr.setStorable(true);
	nAttr.setDefault(0.0);
	nAttr.setSoftMin(0.0);
	nAttr.setSoftMax(1.0);
	nAttr.setKeyable(true);
	nAttr.setChannelBox(true);
	addAttribute(ClonerMultiThread::aUVOffsetU);

	ClonerMultiThread::aUVOffsetV = nAttr.create("uvOffsetV", "uvOffsetV", MFnNumericData::kFloat);
	nAttr.setStorable(true);
	nAttr.setDefault(0.0);
	nAttr.setSoftMin(0.0);
	nAttr.setSoftMax(1.0);
	nAttr.setKeyable(true);
	nAttr.setChannelBox(true);
	addAttribute(ClonerMultiThread::aUVOffsetV);

	ClonerMultiThread::aRndUVOffsetU = nAttr.create("randomUvOffsetU", "randomUvOffsetU", MFnNumericData::kFloat);
	nAttr.setStorable(true);
	nAttr.setDefault(0.0);
	nAttr.setSoftMin(0.0);
	nAttr.setSoftMax(1.0);
	nAttr.setKeyable(true);
	nAttr.setChannelBox(true);
	addAttribute(ClonerMultiThread::aRndUVOffsetU);

	ClonerMultiThread::aRndUVOffsetV = nAttr.create("randomUvOffsetV", "randomUvOffsetV", MFnNumericData::kFloat);
	nAttr.setStorable(true);
	nAttr.setDefault(0.0);
	nAttr.setSoftMin(0.0);
	nAttr.setSoftMax(1.0);
	nAttr.setKeyable(true);
	nAttr.setChannelBox(true);
	addAttribute(ClonerMultiThread::aRndUVOffsetV);


	ClonerMultiThread::aFirstUpVecX = nAttr.create("firstUpVecX","fux",MFnNumericData::kDouble,0);
	nAttr.setStorable(true);
	nAttr.setKeyable(true);
	addAttribute(ClonerMultiThread::aFirstUpVecX);

	ClonerMultiThread::aFirstUpVecY = nAttr.create("firstUpVecY","fuy",MFnNumericData::kDouble,1);
	nAttr.setStorable(true);
	nAttr.setKeyable(true);
	addAttribute(ClonerMultiThread::aFirstUpVecY);


	ClonerMultiThread::aFirstUpVecZ = nAttr.create("firstUpVecZ","fuz",MFnNumericData::kDouble,0);
	nAttr.setStorable(true);
	nAttr.setKeyable(true);
	addAttribute(ClonerMultiThread::aFirstUpVecZ);


	ClonerMultiThread::aFirstUpVec = cAttr.create("firstUpVec","fu");
	cAttr.addChild(ClonerMultiThread::aFirstUpVecX);
	cAttr.addChild(ClonerMultiThread::aFirstUpVecY);
	cAttr.addChild(ClonerMultiThread::aFirstUpVecZ);
	addAttribute(ClonerMultiThread::aFirstUpVec);


	ClonerMultiThread::aReverseNormals = nAttr.create("reverseNormals", "reverseNormals", MFnNumericData::kBoolean);
	nAttr.setStorable(true);
	nAttr.setDefault(false);
	nAttr.setKeyable(true);
	nAttr.setChannelBox(true);
	addAttribute(ClonerMultiThread::aReverseNormals);

	ClonerMultiThread::aSmoothNormals = nAttr.create("smoothNormals", "smoothNormals", MFnNumericData::kBoolean);
	nAttr.setStorable(true);
	nAttr.setDefault(false);
	nAttr.setKeyable(true);
	nAttr.setChannelBox(true);
	addAttribute(ClonerMultiThread::aSmoothNormals);

	// Rev pattern
	ClonerMultiThread::aRevPattern = nAttr.create("reversePattern", "reversePattern", MFnNumericData::kBoolean);
	nAttr.setStorable(true);
	nAttr.setDefault(false);
	nAttr.setKeyable(true);
	nAttr.setChannelBox(true);
	addAttribute(ClonerMultiThread::aRevPattern);

	ClonerMultiThread::aMergeInputMeshes = nAttr.create("mergeInputMeshes", "mergeInputMeshes", MFnNumericData::kBoolean);
	nAttr.setStorable(true);
	nAttr.setDefault(false);
	nAttr.setKeyable(true);
	nAttr.setChannelBox(true);
	addAttribute(ClonerMultiThread::aMergeInputMeshes);

	ClonerMultiThread::aWorldSpace = nAttr.create("worldSpace", "worldSpace", MFnNumericData::kBoolean);
	nAttr.setStorable(true);
	nAttr.setDefault(false);
	nAttr.setKeyable(true);
	nAttr.setChannelBox(true);
	addAttribute(ClonerMultiThread::aWorldSpace);

	ClonerMultiThread::aLoopOffset = nAttr.create("loopOffset", "loopOffset", MFnNumericData::kBoolean);
	nAttr.setStorable(true);
	nAttr.setDefault(false);
	nAttr.setKeyable(true);
	nAttr.setChannelBox(true);
	addAttribute(ClonerMultiThread::aLoopOffset);

	ClonerMultiThread::aUvUDIMLoop = nAttr.create("UvUdimLoop", "UvUdimLoop", MFnNumericData::kBoolean);
	nAttr.setStorable(true);
	nAttr.setDefault(false);
	nAttr.setKeyable(true);
	nAttr.setChannelBox(true);
	addAttribute(ClonerMultiThread::aUvUDIMLoop);


	ClonerMultiThread::aOutputMeshDisplayOverride = nAttr.create("outputMeshDisplayOverride", "outputMeshDisplayOverride", MFnNumericData::kBoolean);
	nAttr.setStorable(true);
	nAttr.setDefault(true);
	nAttr.setKeyable(true);
	nAttr.setChannelBox(true);
	addAttribute(ClonerMultiThread::aOutputMeshDisplayOverride);

	ClonerMultiThread::aDisplayProxy = nAttr.create("displayProxy", "displayProxy", MFnNumericData::kBoolean);
	nAttr.setStorable(true);
	nAttr.setDefault(false);
	nAttr.setKeyable(true);
	nAttr.setChannelBox(true);
	addAttribute(ClonerMultiThread::aDisplayProxy);

	ClonerMultiThread::aConnectPieces = nAttr.create( "connectPieces", "connectPieces", MFnNumericData::kBoolean );
	nAttr.setStorable(true);
	nAttr.setDefault(false);
	nAttr.setKeyable(true);
	nAttr.setChannelBox(true);
	addAttribute( ClonerMultiThread::aConnectPieces );

	ClonerMultiThread::aConnectArrayA = tAttr.create( "connectArrayA", "connectArrayA", MFnData::kString, defaultStringA );
	tAttr.setStorable(true);
	tAttr.setChannelBox(false);
	addAttribute( ClonerMultiThread::aConnectArrayA );

	ClonerMultiThread::aConnectArrayB = tAttr.create( "connectArrayB", "connectArrayB", MFnData::kString, defaultStringA);
	tAttr.setStorable(true);
	tAttr.setChannelBox(false);
	ClonerMultiThread::addAttribute( aConnectArrayB );


	// Attribute affects
	// Output mesh


	attributeAffects(ClonerMultiThread::aInMesh, ClonerMultiThread::aOutMesh);
	attributeAffects(ClonerMultiThread::aRefMesh, ClonerMultiThread::aOutMesh);
	attributeAffects(ClonerMultiThread::aInCurve, ClonerMultiThread::aOutMesh);

	attributeAffects(ClonerMultiThread::aInLocAPos, ClonerMultiThread::aOutMesh);
	attributeAffects(ClonerMultiThread::aInLocBPos, ClonerMultiThread::aOutMesh);


	attributeAffects(ClonerMultiThread::aInstanceType, ClonerMultiThread::aOutMesh);
	attributeAffects(ClonerMultiThread::aPatterType, ClonerMultiThread::aOutMesh);
	attributeAffects(ClonerMultiThread::aScatterType, ClonerMultiThread::aOutMesh);
	attributeAffects(ClonerMultiThread::aOrientationType, ClonerMultiThread::aOutMesh);

	attributeAffects(ClonerMultiThread::aIDType, ClonerMultiThread::aOutMesh);

	attributeAffects(ClonerMultiThread::aFirstUpVec, ClonerMultiThread::aOutMesh);


	attributeAffects(ClonerMultiThread::aLimitDisplay, ClonerMultiThread::aOutMesh);

	attributeAffects(ClonerMultiThread::aGridInstanceX, ClonerMultiThread::aOutMesh);
	attributeAffects(ClonerMultiThread::aGridInstanceY, ClonerMultiThread::aOutMesh);
	attributeAffects(ClonerMultiThread::aGridInstanceZ, ClonerMultiThread::aOutMesh);
	attributeAffects(ClonerMultiThread::aRotateX, ClonerMultiThread::aOutMesh);
	attributeAffects(ClonerMultiThread::aRotateY, ClonerMultiThread::aOutMesh);
	attributeAffects(ClonerMultiThread::aRotateZ, ClonerMultiThread::aOutMesh);
	attributeAffects(ClonerMultiThread::aScaleX, ClonerMultiThread::aOutMesh);
	attributeAffects(ClonerMultiThread::aScaleY, ClonerMultiThread::aOutMesh);
	attributeAffects(ClonerMultiThread::aScaleZ, ClonerMultiThread::aOutMesh);
	attributeAffects(ClonerMultiThread::aGridOffsetX, ClonerMultiThread::aOutMesh);
	attributeAffects(ClonerMultiThread::aGridOffsetY, ClonerMultiThread::aOutMesh);
	attributeAffects(ClonerMultiThread::aGridOffsetZ, ClonerMultiThread::aOutMesh);

	attributeAffects(ClonerMultiThread::aRndScaleX, ClonerMultiThread::aOutMesh);
	attributeAffects(ClonerMultiThread::aRndScaleY, ClonerMultiThread::aOutMesh);
	attributeAffects(ClonerMultiThread::aRndScaleZ, ClonerMultiThread::aOutMesh);
	attributeAffects(ClonerMultiThread::aRndRotateX, ClonerMultiThread::aOutMesh);
	attributeAffects(ClonerMultiThread::aRndRotateY, ClonerMultiThread::aOutMesh);
	attributeAffects(ClonerMultiThread::aRndRotateZ, ClonerMultiThread::aOutMesh);
	attributeAffects(ClonerMultiThread::aRndOffsetX, ClonerMultiThread::aOutMesh);
	attributeAffects(ClonerMultiThread::aRndOffsetY, ClonerMultiThread::aOutMesh);
	attributeAffects(ClonerMultiThread::aRndOffsetZ, ClonerMultiThread::aOutMesh);

	attributeAffects(ClonerMultiThread::aUVOffsetU, ClonerMultiThread::aOutMesh);
	attributeAffects(ClonerMultiThread::aUVOffsetV, ClonerMultiThread::aOutMesh);
	attributeAffects(ClonerMultiThread::aRndUVOffsetU, ClonerMultiThread::aOutMesh);
	attributeAffects(ClonerMultiThread::aRndUVOffsetV, ClonerMultiThread::aOutMesh);
	attributeAffects(ClonerMultiThread::aSeedVal, ClonerMultiThread::aOutMesh);
	attributeAffects(ClonerMultiThread::aRandIDLev, ClonerMultiThread::aOutMesh);

	attributeAffects(ClonerMultiThread::aRevPattern, ClonerMultiThread::aOutMesh);
	attributeAffects(ClonerMultiThread::aReverseNormals, ClonerMultiThread::aOutMesh);
	attributeAffects(ClonerMultiThread::aSmoothNormals, ClonerMultiThread::aOutMesh);
	attributeAffects(ClonerMultiThread::aMergeInputMeshes, ClonerMultiThread::aOutMesh);
	attributeAffects(ClonerMultiThread::aWorldSpace, ClonerMultiThread::aOutMesh);
	attributeAffects(ClonerMultiThread::aLoopOffset, ClonerMultiThread::aOutMesh);
	attributeAffects(ClonerMultiThread::aUvUDIMLoop, ClonerMultiThread::aOutMesh);
	attributeAffects(ClonerMultiThread::aOutputMeshDisplayOverride, ClonerMultiThread::aOutMesh);
	attributeAffects(ClonerMultiThread::aDisplayProxy, ClonerMultiThread::aOutMesh);
	attributeAffects(ClonerMultiThread::aConnectPieces, ClonerMultiThread::aOutMesh);

	attributeAffects(ClonerMultiThread::aConnectArrayA, ClonerMultiThread::aOutMesh);
	attributeAffects(ClonerMultiThread::aConnectArrayB, ClonerMultiThread::aOutMesh);


	// Output Matrix array
	attributeAffects(ClonerMultiThread::aInMesh, ClonerMultiThread::aOutMatrixArray);
	attributeAffects(ClonerMultiThread::aRefMesh, ClonerMultiThread::aOutMatrixArray);
	attributeAffects(ClonerMultiThread::aInCurve, ClonerMultiThread::aOutMatrixArray);
	attributeAffects(ClonerMultiThread::aInLocAPos, ClonerMultiThread::aOutMatrixArray);
	attributeAffects(ClonerMultiThread::aInLocBPos, ClonerMultiThread::aOutMatrixArray);
	attributeAffects(ClonerMultiThread::aInstanceType, ClonerMultiThread::aOutMatrixArray);
	attributeAffects(ClonerMultiThread::aPatterType, ClonerMultiThread::aOutMatrixArray);
	attributeAffects(ClonerMultiThread::aScatterType, ClonerMultiThread::aOutMatrixArray);
	attributeAffects(ClonerMultiThread::aOrientationType, ClonerMultiThread::aOutMatrixArray);
	attributeAffects(ClonerMultiThread::aIDType, ClonerMultiThread::aOutMatrixArray);
	attributeAffects(ClonerMultiThread::aFirstUpVec, ClonerMultiThread::aOutMatrixArray);
	attributeAffects(ClonerMultiThread::aLimitDisplay, ClonerMultiThread::aOutMatrixArray);
	attributeAffects(ClonerMultiThread::aGridInstanceX, ClonerMultiThread::aOutMatrixArray);
	attributeAffects(ClonerMultiThread::aGridInstanceY, ClonerMultiThread::aOutMatrixArray);
	attributeAffects(ClonerMultiThread::aGridInstanceZ, ClonerMultiThread::aOutMatrixArray);
	attributeAffects(ClonerMultiThread::aRotateX, ClonerMultiThread::aOutMatrixArray);
	attributeAffects(ClonerMultiThread::aRotateY, ClonerMultiThread::aOutMatrixArray);
	attributeAffects(ClonerMultiThread::aRotateZ, ClonerMultiThread::aOutMatrixArray);
	attributeAffects(ClonerMultiThread::aScaleX, ClonerMultiThread::aOutMatrixArray);
	attributeAffects(ClonerMultiThread::aScaleY, ClonerMultiThread::aOutMatrixArray);
	attributeAffects(ClonerMultiThread::aScaleZ, ClonerMultiThread::aOutMatrixArray);
	attributeAffects(ClonerMultiThread::aGridOffsetX, ClonerMultiThread::aOutMatrixArray);
	attributeAffects(ClonerMultiThread::aGridOffsetY, ClonerMultiThread::aOutMatrixArray);
	attributeAffects(ClonerMultiThread::aGridOffsetZ, ClonerMultiThread::aOutMatrixArray);
	attributeAffects(ClonerMultiThread::aRndScaleX, ClonerMultiThread::aOutMatrixArray);
	attributeAffects(ClonerMultiThread::aRndScaleY, ClonerMultiThread::aOutMatrixArray);
	attributeAffects(ClonerMultiThread::aRndScaleZ, ClonerMultiThread::aOutMatrixArray);
	attributeAffects(ClonerMultiThread::aRndRotateX, ClonerMultiThread::aOutMatrixArray);
	attributeAffects(ClonerMultiThread::aRndRotateY, ClonerMultiThread::aOutMatrixArray);
	attributeAffects(ClonerMultiThread::aRndRotateZ, ClonerMultiThread::aOutMatrixArray);
	attributeAffects(ClonerMultiThread::aRndOffsetX, ClonerMultiThread::aOutMatrixArray);
	attributeAffects(ClonerMultiThread::aRndOffsetY, ClonerMultiThread::aOutMatrixArray);
	attributeAffects(ClonerMultiThread::aRndOffsetZ, ClonerMultiThread::aOutMatrixArray);
	attributeAffects(ClonerMultiThread::aUVOffsetU, ClonerMultiThread::aOutMatrixArray);
	attributeAffects(ClonerMultiThread::aUVOffsetV, ClonerMultiThread::aOutMatrixArray);
	attributeAffects(ClonerMultiThread::aRndUVOffsetU, ClonerMultiThread::aOutMatrixArray);
	attributeAffects(ClonerMultiThread::aRndUVOffsetV, ClonerMultiThread::aOutMatrixArray);
	attributeAffects(ClonerMultiThread::aSeedVal, ClonerMultiThread::aOutMatrixArray);
	attributeAffects(ClonerMultiThread::aRandIDLev, ClonerMultiThread::aOutMatrixArray);
	attributeAffects(ClonerMultiThread::aRevPattern, ClonerMultiThread::aOutMatrixArray);
	attributeAffects(ClonerMultiThread::aReverseNormals, ClonerMultiThread::aOutMatrixArray);
	attributeAffects(ClonerMultiThread::aSmoothNormals, ClonerMultiThread::aOutMatrixArray);
	attributeAffects(ClonerMultiThread::aMergeInputMeshes, ClonerMultiThread::aOutMatrixArray);
	attributeAffects(ClonerMultiThread::aWorldSpace, ClonerMultiThread::aOutMatrixArray);
	attributeAffects(ClonerMultiThread::aLoopOffset, ClonerMultiThread::aOutMatrixArray);
	attributeAffects(ClonerMultiThread::aUvUDIMLoop, ClonerMultiThread::aOutMatrixArray);
	attributeAffects(ClonerMultiThread::aDisplayProxy, ClonerMultiThread::aOutMatrixArray);

	attributeAffects(ClonerMultiThread::aConnectPieces, ClonerMultiThread::aOutMatrixArray);
	attributeAffects(ClonerMultiThread::aConnectArrayA, ClonerMultiThread::aOutMatrixArray);
	attributeAffects(ClonerMultiThread::aConnectArrayB, ClonerMultiThread::aOutMatrixArray);

	// Output ID array
	attributeAffects(ClonerMultiThread::aInMesh, ClonerMultiThread::aOutIDArray);
	attributeAffects(ClonerMultiThread::aRefMesh, ClonerMultiThread::aOutIDArray);
	attributeAffects(ClonerMultiThread::aInCurve, ClonerMultiThread::aOutIDArray);
	attributeAffects(ClonerMultiThread::aInLocAPos, ClonerMultiThread::aOutIDArray);
	attributeAffects(ClonerMultiThread::aInLocBPos, ClonerMultiThread::aOutIDArray);
	attributeAffects(ClonerMultiThread::aInstanceType, ClonerMultiThread::aOutIDArray);
	attributeAffects(ClonerMultiThread::aPatterType, ClonerMultiThread::aOutIDArray);
	attributeAffects(ClonerMultiThread::aScatterType, ClonerMultiThread::aOutIDArray);
	attributeAffects(ClonerMultiThread::aOrientationType, ClonerMultiThread::aOutIDArray);
	attributeAffects(ClonerMultiThread::aIDType, ClonerMultiThread::aOutIDArray);
	attributeAffects(ClonerMultiThread::aFirstUpVec, ClonerMultiThread::aOutIDArray);
	attributeAffects(ClonerMultiThread::aLimitDisplay, ClonerMultiThread::aOutIDArray);
	attributeAffects(ClonerMultiThread::aGridInstanceX, ClonerMultiThread::aOutIDArray);
	attributeAffects(ClonerMultiThread::aGridInstanceY, ClonerMultiThread::aOutIDArray);
	attributeAffects(ClonerMultiThread::aGridInstanceZ, ClonerMultiThread::aOutIDArray);
	attributeAffects(ClonerMultiThread::aRotateX, ClonerMultiThread::aOutIDArray);
	attributeAffects(ClonerMultiThread::aRotateY, ClonerMultiThread::aOutIDArray);
	attributeAffects(ClonerMultiThread::aRotateZ, ClonerMultiThread::aOutIDArray);
	attributeAffects(ClonerMultiThread::aScaleX, ClonerMultiThread::aOutIDArray);
	attributeAffects(ClonerMultiThread::aScaleY, ClonerMultiThread::aOutIDArray);
	attributeAffects(ClonerMultiThread::aScaleZ, ClonerMultiThread::aOutIDArray);
	attributeAffects(ClonerMultiThread::aGridOffsetX, ClonerMultiThread::aOutIDArray);
	attributeAffects(ClonerMultiThread::aGridOffsetY, ClonerMultiThread::aOutIDArray);
	attributeAffects(ClonerMultiThread::aGridOffsetZ, ClonerMultiThread::aOutIDArray);
	attributeAffects(ClonerMultiThread::aRndScaleX, ClonerMultiThread::aOutIDArray);
	attributeAffects(ClonerMultiThread::aRndScaleY, ClonerMultiThread::aOutIDArray);
	attributeAffects(ClonerMultiThread::aRndScaleZ, ClonerMultiThread::aOutIDArray);
	attributeAffects(ClonerMultiThread::aRndRotateX, ClonerMultiThread::aOutIDArray);
	attributeAffects(ClonerMultiThread::aRndRotateY, ClonerMultiThread::aOutIDArray);
	attributeAffects(ClonerMultiThread::aRndRotateZ, ClonerMultiThread::aOutIDArray);
	attributeAffects(ClonerMultiThread::aRndOffsetX, ClonerMultiThread::aOutIDArray);
	attributeAffects(ClonerMultiThread::aRndOffsetY, ClonerMultiThread::aOutIDArray);
	attributeAffects(ClonerMultiThread::aRndOffsetZ, ClonerMultiThread::aOutIDArray);
	attributeAffects(ClonerMultiThread::aUVOffsetU, ClonerMultiThread::aOutIDArray);
	attributeAffects(ClonerMultiThread::aUVOffsetV, ClonerMultiThread::aOutIDArray);
	attributeAffects(ClonerMultiThread::aRndUVOffsetU, ClonerMultiThread::aOutIDArray);
	attributeAffects(ClonerMultiThread::aRndUVOffsetV, ClonerMultiThread::aOutIDArray);
	attributeAffects(ClonerMultiThread::aSeedVal, ClonerMultiThread::aOutIDArray);
	attributeAffects(ClonerMultiThread::aRandIDLev, ClonerMultiThread::aOutIDArray);
	attributeAffects(ClonerMultiThread::aRevPattern, ClonerMultiThread::aOutIDArray);
	attributeAffects(ClonerMultiThread::aReverseNormals, ClonerMultiThread::aOutIDArray);
	attributeAffects(ClonerMultiThread::aSmoothNormals, ClonerMultiThread::aOutIDArray);
	attributeAffects(ClonerMultiThread::aMergeInputMeshes, ClonerMultiThread::aOutIDArray);
	attributeAffects(ClonerMultiThread::aWorldSpace, ClonerMultiThread::aOutIDArray);
	attributeAffects(ClonerMultiThread::aLoopOffset, ClonerMultiThread::aOutIDArray);
	attributeAffects(ClonerMultiThread::aUvUDIMLoop, ClonerMultiThread::aOutIDArray);

	attributeAffects(ClonerMultiThread::aConnectPieces, ClonerMultiThread::aOutIDArray);
	attributeAffects(ClonerMultiThread::aConnectArrayA, ClonerMultiThread::aOutIDArray);
	attributeAffects(ClonerMultiThread::aConnectArrayB, ClonerMultiThread::aOutIDArray);

	return MS::kSuccess;
}


