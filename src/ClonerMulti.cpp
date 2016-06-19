//
//  ClonerMulti.cpp
//  ClonerMultiThread
//
//  Created by Hunyadi Janos on 31/01/15.
//  Copyright (c) 2015 Janos Hunyadi. All rights reserved.
//

#include "ClonerMulti.h"

MTypeId     ClonerMultiThread::id( 0x80709 );

MObject     ClonerMultiThread::aOutMesh;
MObject     ClonerMultiThread::aInMesh;
MObject		ClonerMultiThread::aInCurve;
MObject		ClonerMultiThread::aRefMesh;

MObject     ClonerMultiThread::aInstanceType;
MObject		ClonerMultiThread::aPatterType;
MObject     ClonerMultiThread::aIDType;
MObject     ClonerMultiThread::aRevPattern;

MObject     ClonerMultiThread::aGridInstanceX;
MObject     ClonerMultiThread::aGridInstanceY;
MObject     ClonerMultiThread::aGridInstanceZ;

MObject     ClonerMultiThread::aGridOffsetX;
MObject     ClonerMultiThread::aGridOffsetY;
MObject     ClonerMultiThread::aGridOffsetZ;

MObject     ClonerMultiThread::aRotateX;
MObject     ClonerMultiThread::aRotateY;
MObject     ClonerMultiThread::aRotateZ;

MObject     ClonerMultiThread::aScaleX;
MObject     ClonerMultiThread::aScaleY;
MObject     ClonerMultiThread::aScaleZ;

MObject     ClonerMultiThread::aReverseNormals;
MObject     ClonerMultiThread::aSmoothNormals;
MObject     ClonerMultiThread::aMergeInputMeshes;
MObject		ClonerMultiThread::aWorldSpace;

MObject     ClonerMultiThread::aFirstUpVec;
MObject     ClonerMultiThread::aFirstUpVecX;
MObject     ClonerMultiThread::aFirstUpVecY;
MObject     ClonerMultiThread::aFirstUpVecZ;



// ------------- MAIN PROGRAM -------------------------------------------------------------
ClonerMultiThread::ClonerMultiThread()
{
}

ClonerMultiThread::~ClonerMultiThread(){}

void* ClonerMultiThread::creator()
{
	return new ClonerMultiThread();
}

void ClonerMultiThread::postConstructor()
{

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
    
    
    // Flip normals if set
    reverseNormals();


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


MStatus ClonerMultiThread::generateUVs()
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

		mFnA.getCurrentUVSetName(defaultUVSetName);
		mFnA.getUVs(in_uArray,in_vArray);
		mFnA.getAssignedUVs(in_uvCounts, in_uvIds, &defaultUVSetName);

		//v_defaultUVSetName[i] = defaultUVSetName;
		o_defaultUVSetNameA = defaultUVSetName;
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

MStatus ClonerMultiThread::duplicateUVs()
{
	MStatus status;

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


	return MStatus::kSuccess;
}


MStatus ClonerMultiThread::collectPlugs(MDataBlock& data)
{

	MStatus status;
    

    m_inCurve = data.inputValue(aInCurve, &status).asNurbsCurve();
    CHECK_MSTATUS_AND_RETURN_IT(status);
    m_curveTrMat = data.inputValue(aInCurve, &status).geometryTransformMatrix();
    CHECK_MSTATUS_AND_RETURN_IT(status);
    
    m_refMesh = data.inputValue(aRefMesh, &status).asMesh();
    CHECK_MSTATUS_AND_RETURN_IT(status);
    
    m_firstUpVec = data.inputValue(aFirstUpVec, &status).asVector();
    CHECK_MSTATUS_AND_RETURN_IT(status);
    m_firstUpVec.normalize();
    
    h_outputMesh = data.outputValue(aOutMesh, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	m_instanceType = data.inputValue(aInstanceType, &status).asInt();
	CHECK_MSTATUS_AND_RETURN_IT(status);
	m_patternType = data.inputValue(aPatterType, &status).asInt();
	CHECK_MSTATUS_AND_RETURN_IT(status);
	m_id = data.inputValue(aIDType, &status).asInt();
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

    // Override instace count if circle is set
    if (m_instanceType == 1) { m_instanceZ = 1;}
    

	m_numDup = m_instanceX * m_instanceY * m_instanceZ; if (m_numDup < 1) { m_numDup = 1;}

	return MStatus::kSuccess;

}


// --- COMPUTE ----------------------------------------------------------------------------------
MStatus ClonerMultiThread::compute( const MPlug& plug, MDataBlock& data )
{
	MStatus status;

	// collect the input plug data
	status = collectPlugs(data);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	// Collect input meshes
	status = collectInputMeshes(data);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	// Generate IDs
	m_idA = generatePatternIDs(m_patternType, m_numDup);

	// Generate transormation matrix array
    
    if (m_instanceType == 0) { status = instanceGrid(); CHECK_MSTATUS_AND_RETURN_IT(status);}
    else if (m_instanceType == 1) { status = instanceCircle(); CHECK_MSTATUS_AND_RETURN_IT(status);}
    else if (m_instanceType == 4) { status = instanceSpline(); CHECK_MSTATUS_AND_RETURN_IT(status);}
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

	if (m_mergeInputMeshes)
	{

		// Merge uv's
		status = generateUVs();
		CHECK_MSTATUS_AND_RETURN_IT(status);

		//Merge input meshes
		status = mergeInputMeshes();
		CHECK_MSTATUS_AND_RETURN_IT(status);

		meshFn.create(o_numVertices, o_numPolygons, o_vertexArray, o_polygonCounts, o_polygonConnects, o_uArrayA, o_vArrayA, newMeshData, &status);
		CHECK_MSTATUS_AND_RETURN_IT(status);
		status = meshFn.assignUVs(o_uvCountsA, o_uvIdsA, &o_defaultUVSetNameA);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		m_inMeshArray.clear();
		m_inMeshArray.append(newMeshData);
		m_numInputMeshes = 1;
		m_idA.clear(); for (int i = 0; i < m_numDup; i++) {m_idA.append(0);}


		status = duplicateInputMeshes(m_idA);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		ex_meshFn.create(o_numVertices, o_numPolygons, o_vertexArray, o_polygonCounts, o_polygonConnects, ex_newMeshData, &status);
		CHECK_MSTATUS_AND_RETURN_IT(status);


		// Smooth the normals globally
		smoothNormals(ex_meshFn);

		// Send mesh to output datablock
		status = h_outputMesh.set(ex_newMeshData);
		CHECK_MSTATUS_AND_RETURN_IT(status);

	}

	else
	{

		status = duplicateInputMeshes(m_idA);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		ex_meshFn.create(o_numVertices, o_numPolygons, o_vertexArray, o_polygonCounts, o_polygonConnects, ex_newMeshData, &status);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		// Smooth the normals globally
		smoothNormals(ex_meshFn);

		// Send mesh to output datablock
		status = h_outputMesh.set(ex_newMeshData);
		CHECK_MSTATUS_AND_RETURN_IT(status);

	}






	return MS::kSuccess;
}

MStatus ClonerMultiThread::initialize()
{

	MStatus status;


	MFnTypedAttribute			tAttr;
	MFnNumericAttribute			nAttr;
	MFnEnumAttribute			eAttr;
    MFnCompoundAttribute        cAttr;



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
    
    
    // Types
	ClonerMultiThread::aInstanceType = eAttr.create("instanceType", "instanceType", 0);
	eAttr.setStorable(true);
	eAttr.addField("Grid", 0);
	eAttr.addField("Circular", 1);
	eAttr.addField("Sphere", 2);
	eAttr.addField("A - B", 3);
	eAttr.addField("Spline", 4);
	eAttr.addField("Input geo", 5);
    eAttr.setStorable(true);
	//eAttr.setInternal(true);
	addAttribute(ClonerMultiThread::aInstanceType);

	ClonerMultiThread::aIDType = nAttr.create("id", "id", MFnNumericData::kInt);
	nAttr.setStorable(true);
	nAttr.setDefault(0);
	nAttr.setMin(0);
	nAttr.setKeyable(true);
	nAttr.setChannelBox(true);
    eAttr.setStorable(true);
	addAttribute(ClonerMultiThread::aIDType);

	ClonerMultiThread::aPatterType = eAttr.create("patternType", "patternType", 0);
	eAttr.setStorable(true);
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


	// Instance
	ClonerMultiThread::aGridInstanceX = nAttr.create("instanceX", "instanceX", MFnNumericData::kInt);
	nAttr.setStorable(true);
	nAttr.setDefault(1);
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

    
    
    ClonerMultiThread::aFirstUpVecX = nAttr.create("firstUpVecX","fux",MFnNumericData::kDouble,0);
    nAttr.setStorable(true);
    nAttr.setKeyable(true);
    addAttribute(ClonerMultiThread::aFirstUpVecX);
    
    ClonerMultiThread::aFirstUpVecY = nAttr.create("firstUpVecY","fuy",MFnNumericData::kDouble,0);
    nAttr.setStorable(true);
    nAttr.setKeyable(true);
    addAttribute(ClonerMultiThread::aFirstUpVecY);
    
    
    ClonerMultiThread::aFirstUpVecZ = nAttr.create("firstUpVecZ","fuz",MFnNumericData::kDouble,1);
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

	// Attribute affects
	attributeAffects(ClonerMultiThread::aInMesh, ClonerMultiThread::aOutMesh);
    attributeAffects(ClonerMultiThread::aRefMesh, ClonerMultiThread::aOutMesh);
    attributeAffects(ClonerMultiThread::aInCurve, ClonerMultiThread::aOutMesh);

	attributeAffects(ClonerMultiThread::aInstanceType, ClonerMultiThread::aOutMesh);
	attributeAffects(ClonerMultiThread::aIDType, ClonerMultiThread::aOutMesh);
	attributeAffects(ClonerMultiThread::aPatterType, ClonerMultiThread::aOutMesh);
	attributeAffects(ClonerMultiThread::aRevPattern, ClonerMultiThread::aOutMesh);
    
    attributeAffects(ClonerMultiThread::aFirstUpVec, ClonerMultiThread::aOutMesh);

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

	attributeAffects(ClonerMultiThread::aReverseNormals, ClonerMultiThread::aOutMesh);
	attributeAffects(ClonerMultiThread::aSmoothNormals, ClonerMultiThread::aOutMesh);
	attributeAffects(ClonerMultiThread::aMergeInputMeshes, ClonerMultiThread::aOutMesh);
	attributeAffects(ClonerMultiThread::aWorldSpace, ClonerMultiThread::aOutMesh);

	return MS::kSuccess;
}


