//
//  ClonerMulti.cpp
//  ClonerMultiThread
//
//  Created by Hunyadi Janos on 31/01/15.
//  Copyright (c) 2015 Janos Hunyadi. All rights reserved.
//

#include "ClonerMulti.h"

MTypeId     ClonerMultiThread::id(0x00123946);

MObject     ClonerMultiThread::aOutMesh;
#if MAYA_API_VERSION > 201600
MObject		ClonerMultiThread::aOutMatrixArray;
#endif
MObject		ClonerMultiThread::aOutIDArray;
MObject     ClonerMultiThread::aInMesh;
MObject		ClonerMultiThread::aInCurve;
MObject		ClonerMultiThread::aRefMesh;
MObject		ClonerMultiThread::aRefMeshSmooth;
MObject     ClonerMultiThread::aInLocAPos;
MObject     ClonerMultiThread::aInLocBPos;
MObject     ClonerMultiThread::aRefLocPos;


MObject     ClonerMultiThread::aInstanceType;
MObject		ClonerMultiThread::aPatterType;
MObject		ClonerMultiThread::aScatterType;
MObject		ClonerMultiThread::aOrientationType;

MObject		ClonerMultiThread::aWrapToSurface;

MObject     ClonerMultiThread::aIDType;
MObject     ClonerMultiThread::aRevPattern;
MObject		ClonerMultiThread::aLimitDisplay;

MObject		ClonerMultiThread::aShowRoot;

// Instance count
MObject     ClonerMultiThread::aGridInstanceX;
MObject     ClonerMultiThread::aGridInstanceY;
MObject     ClonerMultiThread::aGridInstanceZ;

// Offset
MObject     ClonerMultiThread::aGridOffsetX;
MObject     ClonerMultiThread::aGridOffsetY;
MObject     ClonerMultiThread::aGridOffsetZ;
MObject     ClonerMultiThread::aGridOffsetRampX;
MObject     ClonerMultiThread::aGridOffsetRampY;
MObject     ClonerMultiThread::aGridOffsetRampZ;

// Rotate
MObject     ClonerMultiThread::aRotateX;
MObject     ClonerMultiThread::aRotateY;
MObject     ClonerMultiThread::aRotateZ;
MObject     ClonerMultiThread::aRotateRampX;
MObject     ClonerMultiThread::aRotateRampY;
MObject     ClonerMultiThread::aRotateRampZ;


MObject		ClonerMultiThread::aCircleRotate;

MObject     ClonerMultiThread::aTransformXRule;
MObject     ClonerMultiThread::aTransformYRule;
MObject     ClonerMultiThread::aTransformZRule;

MObject     ClonerMultiThread::aRotateXRule;
MObject     ClonerMultiThread::aRotateYRule;
MObject     ClonerMultiThread::aRotateZRule;

MObject     ClonerMultiThread::aScaleXRule;
MObject     ClonerMultiThread::aScaleYRule;
MObject     ClonerMultiThread::aScaleZRule;

// Scale
MObject     ClonerMultiThread::aScaleX;
MObject     ClonerMultiThread::aScaleY;
MObject     ClonerMultiThread::aScaleZ;
MObject     ClonerMultiThread::aScaleRampX;
MObject     ClonerMultiThread::aScaleRampY;
MObject     ClonerMultiThread::aScaleRampZ;

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
MObject		ClonerMultiThread::aDisplayEdges;

MObject     ClonerMultiThread::aFirstUpVec;
MObject     ClonerMultiThread::aFirstUpVecX;
MObject     ClonerMultiThread::aFirstUpVecY;
MObject     ClonerMultiThread::aFirstUpVecZ;

MObject		ClonerMultiThread::aOrientCurveToRefGeo;

MObject		ClonerMultiThread::aConnectPieces;
MObject		ClonerMultiThread::aMergePieces;
MObject		ClonerMultiThread::aConnectLoop;
MObject		ClonerMultiThread::aConnectArrayA;
MObject		ClonerMultiThread::aConnectArrayB;
MObject		ClonerMultiThread::aInterpolate;
MObject		ClonerMultiThread::aCustomEdgeComponent;

MObject		ClonerMultiThread::aSmoothMeshPreview;

MString		ClonerMultiThread::drawDbClassification("drawdb/geometry/ClonerMultiThread");
MString		ClonerMultiThread::drawRegistrantId("ClonerMultiThreadPlugin");

bool m_isDuplicating; // Callback hack

// ------------- MAIN PROGRAM -------------------------------------------------------------

ClonerMultiThreadOverride::ClonerMultiThreadOverride(const MObject& obj) : MHWRender::MPxDrawOverride(obj, ClonerMultiThreadOverride::draw) {}
ClonerMultiThreadOverride::~ClonerMultiThreadOverride() {}

ClonerMultiThread::ClonerMultiThread()
{
}

ClonerMultiThread::~ClonerMultiThread()
{
	MMessage::removeCallbacks(m_callbackIDs);
	m_callbackIDs.clear();
}

void* ClonerMultiThread::creator()
{
	return new ClonerMultiThread();
}

void ClonerMultiThread::postConstructor()
{
	MFnDependencyNode nodeFn(thisMObject());
	nodeFn.setName("clonerMulti_nodeShape#");

	// delete callback
	MCallbackId callbackID;

	MObject myMObject = thisMObject();

	callbackID = MNodeMessage::addNodeAboutToDeleteCallback(myMObject, aboutToDeleteCB, this);
	m_callbackIDs.append(callbackID);

	// duplicate pre callback
	callbackID = MModelMessage::addBeforeDuplicateCallback(ClonerMultiThread::preDuplicateCB, this);
	m_callbackIDs.append(callbackID);

	// duplicate post callback
	callbackID = MModelMessage::addAfterDuplicateCallback(ClonerMultiThread::postDuplicateCB, this);
	m_callbackIDs.append(callbackID);

	// object added to scene callback
	callbackID = MDGMessage::addNodeAddedCallback(ClonerMultiThread::nodeAddedCB, "dagNode", this);
	m_callbackIDs.append(callbackID);


	setExistWithoutOutConnections(false);
	setExistWithoutInConnections(false);

}


// Callbacks

void ClonerMultiThread::nodeAddedCB(MObject& node, void* clientData)
{

	MStatus status;

	if (m_isDuplicating)
	{

		if (!node.isNull())
		{


			MDagPath path;
			status = MDagPath::getAPathTo(node, path);

			if (status.error())
			{
				MGlobal::displayInfo("[ClonerMulti] Error: failed to get dag path to node.");
			}

			else
			{
				MString s = path.partialPathName();

				s.substituteFirst("__PrenotatoPerDuplicare_", "");

				MGlobal::displayInfo(s);
			}


			//MFnDependencyNode nodeFn(node);
			//MPlug worldP = nodeFn.findPlug( "outMesh" );

			//MFnDagNode mfDgN(worldP.node());

			//MPlugArray destPlugs;
			//worldP.connectedTo(destPlugs, false, true);

			//if (destPlugs.length() != 0)
			//{
			//	MPlug destPlug = destPlugs[0];
			//	mfDgN.setObject(destPlug.node());


			//	//MPlug p_out_overrideEnabled = mfDgN.findPlug("overrideEnabled", false);
			//	//p_out_overrideEnabled.setBool(false);

			//	MGlobal::displayInfo(MString("[ClonerMulti] ouptut mehs: ") + mfDgN.partialPathName() );
			//	//MGlobal::displayInfo(MString("[ClonerMulti] Duplicating: ") + mfDgN.name());
			//}

		}

		MGlobal::displayInfo("------");

	}


}

void ClonerMultiThread::preDuplicateCB(void* clientData)
{
	m_isDuplicating = true;
	//MObject* pLocatorData = (MObject*)clientData;

	//MGlobal::displayWarning(MString() + "preDuplicateCB!!" + pLocatorData->apiTypeStr());

}

void ClonerMultiThread::postDuplicateCB(void* clientData)
{
	m_isDuplicating = false;
	//MGlobal::displayWarning("Duplicating!!");

}

void ClonerMultiThread::aboutToDeleteCB(MObject& node, MDGModifier& modifier, void* pUserPtr)
{
	MFnDependencyNode nodeFn(node);
	MGlobal::displayInfo(MString("[ClonerMulti] About to delete callback for node: ") + nodeFn.name());


	// Find the output mesh connected to the node
	MPlug worldP = nodeFn.findPlug("outMesh");

	MFnDagNode mfDgN(worldP.node());

	MPlugArray destPlugs;
	worldP.connectedTo(destPlugs, false, true);

	if (destPlugs.length() != 0)
	{
		MPlug destPlug = destPlugs[0];

		mfDgN.setObject(destPlug.node());


		MPlug p_out_overrideEnabled = mfDgN.findPlug("overrideEnabled", false);
		p_out_overrideEnabled.setBool(false);

		MGlobal::displayInfo(MString("[ClonerMulti] Deleting / Setting output mesh overrides: ") + mfDgN.partialPathName());
	}

	else
	{
		MGlobal::displayInfo(MString() + "[ClonerMulti] Deleting / No connection, or wrong connection to output mesh: ");
	}

}

MStatus ClonerMultiThread::storeRules()
{

	MStatus status;

	// Global rule Arrays

	m_rule_off_A_X.clear();
	m_rule_off_A_Y.clear();
	m_rule_off_A_Z.clear();

	m_rule_rot_A_X.clear();
	m_rule_rot_A_Y.clear();
	m_rule_rot_A_Z.clear();

	m_rule_scl_A_X.clear();
	m_rule_scl_A_Y.clear();
	m_rule_scl_A_Z.clear();

	// Rule loops

	MFloatArray loop_rule_off_A_X;
	MFloatArray loop_rule_off_A_Y;
	MFloatArray loop_rule_off_A_Z;

	MFloatArray loop_rule_rot_A_X;
	MFloatArray loop_rule_rot_A_Y;
	MFloatArray loop_rule_rot_A_Z;

	MFloatArray loop_rule_scl_A_X;
	MFloatArray loop_rule_scl_A_Y;
	MFloatArray loop_rule_scl_A_Z;

	// Gather rules

	MStringArray optionList;

	//
	// Gather offset rules
	//

	// X

	if (m_transformXRule.length() > 0)
	{

		optionList.clear();
		status = m_transformXRule.split(';', optionList);

		if (status)
		{
			for (int i = 0; i < optionList.length(); i++)
			{
				loop_rule_off_A_X.append(optionList[i].asFloat());
			}
		}
	}

	// Y

	if (m_transformYRule.length() > 0)
	{

		optionList.clear();
		status = m_transformYRule.split(';', optionList);

		if (status)
		{
			for (int i = 0; i < optionList.length(); i++)
			{
				loop_rule_off_A_Y.append(optionList[i].asFloat());
			}
		}
	}


	// Z

	if (m_transformZRule.length() > 0)
	{

		optionList.clear();
		status = m_transformZRule.split(';', optionList);

		if (status)
		{
			for (int i = 0; i < optionList.length(); i++)
			{
				loop_rule_off_A_Z.append(optionList[i].asFloat());
			}
		}
	}


	//
	// Gather rotate rules
	//

	// X

	if (m_rotateXRule.length() > 0)
	{

		optionList.clear();
		status = m_rotateXRule.split(';', optionList);

		if (status)
		{
			for (int i = 0; i < optionList.length(); i++)
			{
				loop_rule_rot_A_X.append(optionList[i].asFloat());
			}
		}
	}

	// Y

	if (m_rotateYRule.length() > 0)
	{

		optionList.clear();
		status = m_rotateYRule.split(';', optionList);

		if (status)
		{
			for (int i = 0; i < optionList.length(); i++)
			{
				loop_rule_rot_A_Y.append(optionList[i].asFloat());
			}
		}
	}

	// Z

	if (m_rotateZRule.length() > 0)
	{

		optionList.clear();
		status = m_rotateZRule.split(';', optionList);

		if (status)
		{
			for (int i = 0; i < optionList.length(); i++)
			{
				loop_rule_rot_A_Z.append(optionList[i].asFloat());
			}
		}
	}




	//
	// Gather scale rules
	//

	// X

	if (m_scaleXRule.length() > 0)
	{

		optionList.clear();
		status = m_scaleXRule.split(';', optionList);

		if (status)
		{
			for (int i = 0; i < optionList.length(); i++)
			{
				loop_rule_scl_A_X.append(optionList[i].asFloat());
			}
		}
	}



	// Y

	if (m_scaleYRule.length() > 0)
	{

		optionList.clear();
		status = m_scaleYRule.split(';', optionList);

		if (status)
		{
			for (int i = 0; i < optionList.length(); i++)
			{
				loop_rule_scl_A_Y.append(optionList[i].asFloat());

			}
		}
	}




	// Z

	if (m_scaleZRule.length() > 0)
	{

		optionList.clear();
		status = m_scaleZRule.split(';', optionList);

		if (status)
		{
			for (int i = 0; i < optionList.length(); i++)
			{
				loop_rule_scl_A_Z.append(optionList[i].asFloat());
			}
		}
	}








	double rule_rot_X;

	int rule_off_A_X_counter = 0;
	int rule_off_A_Y_counter = 0;
	int rule_off_A_Z_counter = 0;

	int rule_rot_A_X_counter = 0;
	int rule_rot_A_Y_counter = 0;
	int rule_rot_A_Z_counter = 0;

	int rule_scl_A_X_counter = 0;
	int rule_scl_A_Y_counter = 0;
	int rule_scl_A_Z_counter = 0;

	for (unsigned int i = 0; i < m_numDup; i++)
	{

		//if (m_rotateXRule.length() > 0)
		//{
		//	MStringArray optionList;
		//	status = m_rotateXRule.split(';', optionList);

		//	if (status)
		//	{
		//		for (int i = 0; i < optionList.length(); i++)
		//		{
		//			m_rule_off_A_X.append(optionList[i].asInt());
		//		}

		//		if (m_rule_off_A_X.length() > 0)
		//		{

		//			rule_rot_X = m_rule_off_A_X[rule_counter];

		//		}
		//	}
		//}


		if (loop_rule_off_A_X.length() > 0) { m_rule_off_A_X.append(loop_rule_off_A_X[rule_off_A_X_counter]); }
		else { m_rule_off_A_X.append(0.0); }

		if (loop_rule_off_A_Y.length() > 0) { m_rule_off_A_Y.append(loop_rule_off_A_Y[rule_off_A_Y_counter]); }
		else { m_rule_off_A_Y.append(0.0); }

		if (loop_rule_off_A_Z.length() > 0) { m_rule_off_A_Z.append(loop_rule_off_A_Z[rule_off_A_Z_counter]); }
		else { m_rule_off_A_Z.append(0.0); }


		//

		if (loop_rule_rot_A_X.length() > 0) { m_rule_rot_A_X.append(loop_rule_rot_A_X[rule_rot_A_X_counter]); }
		else { m_rule_rot_A_X.append(0.0); }

		if (loop_rule_rot_A_Y.length() > 0) { m_rule_rot_A_Y.append(loop_rule_rot_A_Y[rule_rot_A_Y_counter]); }
		else { m_rule_rot_A_Y.append(0.0); }

		if (loop_rule_rot_A_Z.length() > 0) { m_rule_rot_A_Z.append(loop_rule_rot_A_Z[rule_rot_A_Z_counter]); }
		else { m_rule_rot_A_Z.append(0.0); }

		//

		if (loop_rule_scl_A_X.length() > 0) { m_rule_scl_A_X.append(loop_rule_scl_A_X[rule_scl_A_X_counter]); }
		else { m_rule_scl_A_X.append(1.0); }

		if (loop_rule_scl_A_Y.length() > 0) { m_rule_scl_A_Y.append(loop_rule_scl_A_Y[rule_scl_A_Y_counter]); }
		else { m_rule_scl_A_Y.append(1.0); }

		if (loop_rule_scl_A_Z.length() > 0) { m_rule_scl_A_Z.append(loop_rule_scl_A_Z[rule_scl_A_Z_counter]); }
		else { m_rule_scl_A_Z.append(1.0); }

		//

		rule_off_A_X_counter += 1;
		rule_off_A_Y_counter += 1;
		rule_off_A_Z_counter += 1;

		rule_rot_A_X_counter += 1;
		rule_rot_A_Y_counter += 1;
		rule_rot_A_Z_counter += 1;

		rule_scl_A_X_counter += 1;
		rule_scl_A_Y_counter += 1;
		rule_scl_A_Z_counter += 1;

		if (rule_off_A_X_counter >= loop_rule_off_A_X.length()) { rule_off_A_X_counter = 0; }
		if (rule_off_A_Y_counter >= loop_rule_off_A_Y.length()) { rule_off_A_Y_counter = 0; }
		if (rule_off_A_Z_counter >= loop_rule_off_A_Z.length()) { rule_off_A_Z_counter = 0; }

		if (rule_rot_A_X_counter >= loop_rule_rot_A_X.length()) { rule_rot_A_X_counter = 0; }
		if (rule_rot_A_Y_counter >= loop_rule_rot_A_Y.length()) { rule_rot_A_Y_counter = 0; }
		if (rule_rot_A_Z_counter >= loop_rule_rot_A_Z.length()) { rule_rot_A_Z_counter = 0; }

		if (rule_scl_A_X_counter >= loop_rule_scl_A_X.length()) { rule_scl_A_X_counter = 0; }
		if (rule_scl_A_Y_counter >= loop_rule_scl_A_Y.length()) { rule_scl_A_Y_counter = 0; }
		if (rule_scl_A_Z_counter >= loop_rule_scl_A_Z.length()) { rule_scl_A_Z_counter = 0; }

	}

	return MStatus::kSuccess;

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


		mfnMesh.getPoints(i_vertexArray[m], MSpace::kObject);
		mfnMesh.getVertices(i_polygonCounts[m], i_polygonConnects[m]);


	}

	// Allocate memory for vector
	int id = 0;
	int len_o_vertexArray = 0;
	int len_o_polygonCounts = 0;
	int len_o_polygonConnects = 0;

	for (int m = 0; m < m_numInputMeshes; m++)
	{
		id = m;
		for (unsigned v = 0; v < i_vertexArray[id].length(); v++) {
			len_o_vertexArray += 1;
		}
		for (unsigned v = 0; v < i_polygonCounts[id].length(); v++) {
			len_o_polygonCounts += 1;
		}
		for (unsigned v = 0; v < i_polygonConnects[id].length(); v++) {
			len_o_polygonConnects += 1;
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

			MFloatPoint currentPoint(currP.x, currP.y, currP.z, currP.w);


			o_vertexArray.set(currentPoint, v + idOffset);
		}
#pragma omp parallel for
		// polygonCounts
		for (int v = 0; v < i_polygonCounts[id].length(); v++) {
			o_polygonCounts.set(i_polygonCounts[id][v], v + polycOffset);
		}



#pragma omp parallel for
		// polygonConnects
		for (int v = 0; v < i_polygonConnects[id].length(); v++) {
			o_polygonConnects.set(i_polygonConnects[id][v] + idOffset, v + polyConnOffset);


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

	//// curve data
	//MFnNurbsCurve curveFn(m_inCurve, &status);
	//CHECK_MSTATUS_AND_RETURN_IT(status);

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


		mfnMesh.getPoints(i_vertexArray[m], MSpace::kObject);
		mfnMesh.getVertices(i_polygonCounts[m], i_polygonConnects[m]);


	}

	// Allocate memory for vector
	int len_o_vertexArray = 0;
	int len_o_polygonCounts = 0;
	int len_o_polygonConnects = 0;

	for (int m = 0; m < m_numDup; m++)
	{
		for (unsigned v = 0; v < i_vertexArray[idA[m]].length(); v++) {
			len_o_vertexArray += 1;
		}
		for (unsigned v = 0; v < i_polygonCounts[idA[m]].length(); v++) {
			len_o_polygonCounts += 1;
		}
		for (unsigned v = 0; v < i_polygonConnects[idA[m]].length(); v++) {
			len_o_polygonConnects += 1;
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
		for (int v = 0; v < i_vertexArray[idA[m]].length(); v++)
		{
			MFloatPoint currP = i_vertexArray[idA[m]][v];
			MPoint currentPoint(currP.x, currP.y, currP.z, currP.w);
			MPoint tempActiveP = currentPoint;

			//if (!m_worldSpace) { currentPoint = (MPoint(currentPoint) *= m_inMeshMatrixArray[idA[m]].inverse()); }

			currentPoint *= m_tr_matA[m];

			if (m_wrap_to_surface)
			{



				//
				// Wire deformer
				//

				// Only calculate if type is set to spline
				if (m_instanceType == 4)
				{

					MTransformationMatrix trMOrig(m_tr_matA[m]);


					// curve data
					MFnNurbsCurve curveFn(m_inCurve, &status);
					CHECK_MSTATUS_AND_RETURN_IT(status);

					MPoint ptClosest, pCvPosition;
					double param;


					MPoint pt = tempActiveP * m_tr_matA[m];

					ptClosest = curveFn.closestPoint(pt, &param, 0.1, MSpace::kWorld, &status);
					CHECK_MSTATUS_AND_RETURN_IT(status);

					MVector tan = curveFn.tangent(param, MSpace::kWorld);
					tan.normalize();

					MVector cross1 = tan ^ m_firstUpVec;
					cross1.normalize();

					MVector cross2 = cross1 ^ tan;

					cross2.normalize();


					double m[4][4] = { { tan.x, tan.y , tan.z, 0.0 },
					{ cross2.x, cross2.y, cross2.z, 0.0 },
					{ cross1.x, cross1.y, cross1.z, 0.0 },
					{ ptClosest.x, ptClosest.y, ptClosest.z, 1.0 } };

					MMatrix rotMatrix = m;



					MTransformationMatrix trMz(rotMatrix);

					//tempActiveP *= trMOrig.asRotateMatrix();
					tempActiveP *= trMOrig.asScaleMatrix();


					currentPoint = tempActiveP * trMz.asMatrix();

				}
			}




			o_vertexArray.set(MFloatPoint(currentPoint.x, currentPoint.y, currentPoint.z, currentPoint.w), v + idOffset);
		}

#pragma omp parallel for
		// polygonCounts
		for (int v = 0; v < i_polygonCounts[idA[m]].length(); v++) {
			o_polygonCounts.set(i_polygonCounts[idA[m]][v], v + polycOffset);
		}



#pragma omp parallel for
		// polygonConnects
		for (int v = 0; v < i_polygonConnects[idA[m]].length(); v++) {
			o_polygonConnects.set(i_polygonConnects[idA[m]][v] + idOffset, v + polyConnOffset);


		}



		// Offsets for loops

		idOffset += i_vertexArray[idA[m]].length();
		polycOffset += i_polygonCounts[idA[m]].length();
		polyConnOffset += i_polygonConnects[idA[m]].length();

		offX += m_offsetX;
		offY += m_offsetY;
		offZ += m_offsetZ;


	}

	////
	//// Wire deformer
	////

	//// Only calculate if type is set to spline
	//if (m_instanceType == 4)
	//{

	//	// curve data
	//	MFnNurbsCurve curveFn(m_inCurve, &status);
	//	CHECK_MSTATUS_AND_RETURN_IT(status);

	//	MPoint ptClosest, pCvPosition;
	//	double param;


	//	for (int i = 0; i < o_vertexArray.length(); i++)
	//	{

	//		MPoint pt = o_vertexArray[i];

	//		ptClosest = curveFn.closestPoint(pt, &param, 0.1, MSpace::kObject, &status);
	//		CHECK_MSTATUS_AND_RETURN_IT(status);


	//		//double distance = (pt - ptClosest).length();



	//		MVector tan = curveFn.tangent(param, MSpace::kObject);
	//		tan.normalize();

	//		//MVector norm = curveFn.normal(param, MSpace::kObject);

	//		MVector cross1 = m_firstUpVec ^ tan;
	//		cross1.normalize();

	//		MVector cross2 = tan ^ cross1;

	//		cross2.normalize();



	//		double m[4][4] = { { tan.x, tan.y , tan.z, 0.0 },
	//		{ cross2.x, cross2.y, cross2.z, 0.0 },
	//		{ cross1.x, cross1.y, cross1.z, 0.0 },
	//		{ ptClosest.x, ptClosest.y, ptClosest.z, 1.0 } };

	//		MMatrix rotMatrix = m;

	//		MTransformationMatrix trMz(rotMatrix);
	//		trMz.setTranslation(MVector(0.0, 0.0, 0.0), MSpace::kObject);
	//		
	//	

	//		MPoint Bp = pt * trMz.asRotateMatrix() ;

	//		o_vertexArray.set(Bp, i);

	//	}

	//}


	m_connPairVA.clear();
	m_connPairVB.clear();


	// Snap verts
	if (m_connectPieces)
	{



		if (m_numInputMeshes == 1)
		{

			idOffset = 0;
			MPoint ptClosest;

			int c = 0;

			for (int m = 0; m < m_numDup - 1; m++)
			{

				for (int v = 0; v < i_vertexArray[idA[m]].length(); v++)
				{

					for (int x = 0; x < m_ConnectArrayA.length(); x++)
					{


						if (v == m_ConnectArrayA[x])
						{
							MPoint aP = o_vertexArray[m_ConnectArrayA[x] + idOffset];
							MPoint bP = o_vertexArray[m_ConnectArrayB[x] + idOffset + i_vertexArray[idA[m]].length()];


							// Store pairs
							
							m_connPairVA.append(m_ConnectArrayA[x] + idOffset);
							m_connPairVB.append(m_ConnectArrayB[x] + idOffset + i_vertexArray[idA[m]].length());
							

							MPoint nP = (aP + bP) * 0.5;
							o_vertexArray.set(MFloatPoint(nP.x, nP.y, nP.z, nP.w), m_ConnectArrayA[x] + idOffset);
							o_vertexArray.set(MFloatPoint(nP.x, nP.y, nP.z, nP.w), m_ConnectArrayB[x] + idOffset + i_vertexArray[idA[m]].length());

						}

					}

					c += 1;

				}
				idOffset += i_vertexArray[idA[m]].length();
			}

			// Connect End loops
			if (m_connectLoop)
			{


				if (m_instanceType == 1 || m_instanceType == 4)
				{

					for (int x = 0; x < m_ConnectArrayA.length(); x++)
					{
						MPoint aP = o_vertexArray[m_ConnectArrayA[x] + idOffset];
						MPoint bP = o_vertexArray[m_ConnectArrayB[x]];


						MPoint nP = (aP + bP) * 0.5;


						o_vertexArray.set(MFloatPoint(nP.x, nP.y, nP.z, nP.w), m_ConnectArrayA[x] + idOffset);
						o_vertexArray.set(MFloatPoint(nP.x, nP.y, nP.z, nP.w), m_ConnectArrayB[x]);


						//// Store pairs
						//m_connPairVA.append(m_ConnectArrayA[x] );
						//m_connPairVB.append(m_ConnectArrayB[x] );

					}

				}

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


	int bbPCountA[6] = { 4,4,4,4,4,4 };
	int bbPConnA[24] = { 0, 1, 2, 3, 4, 5, 6, 7, 3, 2, 6, 5, 0, 3, 5, 4, 0, 4, 7, 1, 1, 7, 6, 2 };

	float w, h, d;



	for (int m = 0; m < m_numInputMeshes; m++)
	{

		//// Collect mesh data
		MFnMesh mfnMesh(m_inMeshArray[m]);

		MBoundingBox bBox = mfnMesh.boundingBox(&status);

		w = bBox.width() * 0.5f;
		h = bBox.height() * 0.5f;
		d = bBox.depth() * 0.5f;

		MFloatPointArray bbPA;

		bbPA.append(MFloatPoint(-w, -h, -d, 1.0f));
		bbPA.append(MFloatPoint(w, -h, -d, 1.0f));
		bbPA.append(MFloatPoint(w, -h, d, 1.0f));
		bbPA.append(MFloatPoint(-w, -h, d, 1.0f));
		bbPA.append(MFloatPoint(-w, h, -d, 1.0f));
		bbPA.append(MFloatPoint(-w, h, d, 1.0f));
		bbPA.append(MFloatPoint(w, h, d, 1.0f));
		bbPA.append(MFloatPoint(w, h, -d, 1.0f));

		i_numVertices[m] = 8;
		i_numPolygons[m] = 6;



		i_vertexArray[m] = bbPA;
		i_polygonCounts[m] = MIntArray(bbPCountA, 6);
		i_polygonConnects[m] = MIntArray(bbPConnA, 24);


	}

	// Allocate memory for vector
	int len_o_vertexArray = 0;
	int len_o_polygonCounts = 0;
	int len_o_polygonConnects = 0;

	for (int m = 0; m < m_numDup; m++)
	{
		for (unsigned v = 0; v < i_vertexArray[idA[m]].length(); v++) {
			len_o_vertexArray += 1;
		}
		for (unsigned v = 0; v < i_polygonCounts[idA[m]].length(); v++) {
			len_o_polygonCounts += 1;
		}
		for (unsigned v = 0; v < i_polygonConnects[idA[m]].length(); v++) {
			len_o_polygonConnects += 1;
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

			MPoint currentPoint(currP.x, currP.y, currP.z, currP.w);



			//if (!m_worldSpace) { currentPoint = (MPoint(currentPoint) *= m_inMeshMatrixArray[idA[m]].inverse()); }

			currentPoint *= m_tr_matA[m];

			o_vertexArray.set(MFloatPoint(currentPoint.x, currentPoint.y, currentPoint.z, currentPoint.w), v + idOffset);
		}
#pragma omp parallel for
		// polygonCounts
		for (int v = 0; v < i_polygonCounts[idA[m]].length(); v++) {
			o_polygonCounts.set(i_polygonCounts[idA[m]][v], v + polycOffset);
		}



#pragma omp parallel for
		// polygonConnects
		for (int v = 0; v < i_polygonConnects[idA[m]].length(); v++) {
			o_polygonConnects.set(i_polygonConnects[idA[m]][v] + idOffset, v + polyConnOffset);


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
		int co = o_polygonCounts.length() - 1;
		for (unsigned i = 0; i < o_polygonCounts.length(); i++)
		{
			revFCA.append(o_polygonCounts[co]);
			co -= 1;
		}


		MIntArray revPCA;
		co = o_polygonConnects.length() - 1;
		for (unsigned i = 0; i < o_polygonConnects.length(); i++)
		{
			revPCA.append(o_polygonConnects[co]);
			co -= 1;
		}

		MIntArray revUVCA;
		co = o_uvCountsA.length() - 1;
		for (unsigned i = 0; i < o_uvCountsA.length(); i++)
		{
			revUVCA.append(o_uvCountsA[co]);
			co -= 1;
		}

		MIntArray revUVIDA;
		co = o_uvIdsA.length() - 1;
		for (unsigned i = 0; i < o_uvIdsA.length(); i++)
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
		mFnA.getUVs(in_uArray, in_vArray);
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
	int len_uArray = 0;
	int len_vArray = 0;
	int len_uvCounts = 0;
	int len_uvIds = 0;
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
		mFnA.getUVs(in_uArray, in_vArray);
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
	int len_uArray = 0;
	int len_vArray = 0;
	int len_uvCounts = 0;
	int len_uvIds = 0;

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

		// If UDIM loop is set offset the uvs if they reach UDIM 1010
		if (m_uvUDIMLoop)
		{

			off_tileU += m_uvOffsetU;

			if (off_tileU >= (1.0))
			{
				off_tileU = 0.0;
				off_tileV += m_uvOffsetV;
			}

		}

		else
		{
			off_tileU += m_uvOffsetU;
			off_tileV += m_uvOffsetV;
		}



		off_tileU += m_rndOffsetUA[m];
		off_tileV += m_rndOffsetVA[m];





	}


	return MS::kSuccess;
}

MStatus ClonerMultiThread::smoothNormals(MFnMesh& meshFn)
{
	MStatus status;

	// Set edge smoothing globally
	for (int loop = 0; loop < meshFn.numEdges(); loop++)
	{

		if (m_smoothNorm == true) { meshFn.setEdgeSmoothing(loop, true); }
		if (m_smoothNorm == false) { meshFn.setEdgeSmoothing(loop, false); }
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


	MArrayDataHandle h_inMeshes = data.inputArrayValue(aInMesh);

	for (unsigned int i = 0; i < h_inMeshes.elementCount(); i++)
	{
		status = h_inMeshes.jumpToArrayElement(i);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		MObject m_inMesh = h_inMeshes.inputValue(&status).asMesh();

		if (m_inMesh.apiType() == MFn::kMeshData)
		{
			m_inMeshArray.append(m_inMesh);

			MMatrix trMat = h_inMeshes.inputValue(&status).geometryTransformMatrix();
			CHECK_MSTATUS_AND_RETURN_IT(status);

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
	p_refMeshSmooth = MPlug(this->thisMObject(), aRefMeshSmooth);
	p_inLocA = MPlug(this->thisMObject(), aInLocAPos);
	p_inLocB = MPlug(this->thisMObject(), aInLocBPos);
	p_refLoc = MPlug(this->thisMObject(), aRefLocPos);


	MString stringDataA = data.inputValue(aConnectArrayA, &status).asString();
	CHECK_MSTATUS_AND_RETURN_IT(status);
	MStringArray stringDataAArray;
	status = stringDataA.split(',', stringDataAArray);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	MString stringDataB = data.inputValue(aConnectArrayB, &status).asString();
	CHECK_MSTATUS_AND_RETURN_IT(status);
	MStringArray stringDataBArray;
	status = stringDataB.split(',', stringDataBArray);
	CHECK_MSTATUS_AND_RETURN_IT(status);



	// Get input point array
	m_customEdgeComponents.clear();
	MPlug p_customEdge = MPlug(this->thisMObject(), aCustomEdgeComponent);
	MObject o_edgeArray;
	p_customEdge.getValue(o_edgeArray);
	MFnIntArrayData fn_aPoints(o_edgeArray);
	fn_aPoints.copyTo(m_customEdgeComponents);


	//MGlobal::displayInfo(MString() + "[Custom plug]: " + customEdgecomponentData.elementCount());

	// put user input array into an mIntarray
	m_ConnectArrayA.clear();
	m_ConnectArrayB.clear();

	for (int i = 0; i < stringDataAArray.length(); i++) { m_ConnectArrayA.append(stringDataAArray[i].asInt()); }
	for (int i = 0; i < stringDataAArray.length(); i++) { m_ConnectArrayB.append(stringDataBArray[i].asInt()); }

	// If one vertex array is larger than the other expand it's size
	int maxValues[2] = { int(m_ConnectArrayA.length()), int(m_ConnectArrayB.length()) };
	int maxValue = *std::min_element(maxValues, maxValues + 2);

	m_ConnectArrayA.setLength(maxValue);
	m_ConnectArrayB.setLength(maxValue);


	m_inCurve = data.inputValue(aInCurve, &status).asNurbsCurve();
	CHECK_MSTATUS_AND_RETURN_IT(status);
	m_curveTrMat = data.inputValue(aInCurve, &status).geometryTransformMatrix();
	CHECK_MSTATUS_AND_RETURN_IT(status);


	m_smoothMeshPreview = data.inputValue(aSmoothMeshPreview, &status).asBool();
	CHECK_MSTATUS_AND_RETURN_IT(status);


	m_refMesh = data.inputValue(aRefMesh, &status).asMesh();
	CHECK_MSTATUS_AND_RETURN_IT(status);

	m_refMeshSmooth = data.inputValue(aRefMeshSmooth, &status).asMesh();
	CHECK_MSTATUS_AND_RETURN_IT(status);

	//if (p_refMeshSmooth.isConnected())
	//{
	//	if (m_smoothMeshPreview)
	//	{
	//	}
	//}


	m_refMeshMat = data.inputValue(aRefMesh, &status).geometryTransformMatrix();
	CHECK_MSTATUS_AND_RETURN_IT(status);

	m_inLocA_posMat = data.inputValue(aInLocAPos, &status).asMatrix();
	CHECK_MSTATUS_AND_RETURN_IT(status);

	m_inLocB_posMat = data.inputValue(aInLocBPos, &status).asMatrix();
	CHECK_MSTATUS_AND_RETURN_IT(status);

	m_refLoc_posMat = data.inputValue(aRefLocPos, &status).asMatrix();
	CHECK_MSTATUS_AND_RETURN_IT(status);

	m_firstUpVec = data.inputValue(aFirstUpVec, &status).asVector();
	CHECK_MSTATUS_AND_RETURN_IT(status);
	m_firstUpVec.normalize();

	h_outputMesh = data.outputValue(aOutMesh, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);
#if MAYA_API_VERSION > 201600
	h_outputMatrix = data.outputValue(aOutMatrixArray, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);
#endif
	h_outputID = data.outputValue(aOutIDArray, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	m_instanceType = data.inputValue(aInstanceType, &status).asShort();
	CHECK_MSTATUS_AND_RETURN_IT(status);
	m_patternType = data.inputValue(aPatterType, &status).asShort();
	CHECK_MSTATUS_AND_RETURN_IT(status);
	m_scatterType = data.inputValue(aScatterType, &status).asShort();
	CHECK_MSTATUS_AND_RETURN_IT(status);
	m_orientationType = data.inputValue(aOrientationType, &status).asBool();
	CHECK_MSTATUS_AND_RETURN_IT(status);

	m_id = data.inputValue(aIDType, &status).asInt();
	CHECK_MSTATUS_AND_RETURN_IT(status);
	m_rand_seed = data.inputValue(aSeedVal, &status).asInt();
	CHECK_MSTATUS_AND_RETURN_IT(status);

	m_polyLimit = data.inputValue(aLimitDisplay, &status).asInt();
	CHECK_MSTATUS_AND_RETURN_IT(status);

	m_instanceX = data.inputValue(aGridInstanceX, &status).asInt();
	CHECK_MSTATUS_AND_RETURN_IT(status);
	m_instanceY = data.inputValue(aGridInstanceY, &status).asInt();
	CHECK_MSTATUS_AND_RETURN_IT(status);
	m_instanceZ = data.inputValue(aGridInstanceZ, &status).asInt();
	CHECK_MSTATUS_AND_RETURN_IT(status);



	m_offsetX = data.inputValue(aGridOffsetX, &status).asFloat();
	CHECK_MSTATUS_AND_RETURN_IT(status);
	m_offsetY = data.inputValue(aGridOffsetY, &status).asFloat();
	CHECK_MSTATUS_AND_RETURN_IT(status);
	m_offsetZ = data.inputValue(aGridOffsetZ, &status).asFloat();
	CHECK_MSTATUS_AND_RETURN_IT(status);

	m_rotateX = data.inputValue(aRotateX, &status).asFloat();
	CHECK_MSTATUS_AND_RETURN_IT(status);
	m_rotateY = data.inputValue(aRotateY, &status).asFloat();
	CHECK_MSTATUS_AND_RETURN_IT(status);
	m_rotateZ = data.inputValue(aRotateZ, &status).asFloat();
	CHECK_MSTATUS_AND_RETURN_IT(status);

	m_circleRotate = data.inputValue(aCircleRotate, &status).asFloat();
	CHECK_MSTATUS_AND_RETURN_IT(status);

	m_scaleX = data.inputValue(aScaleX, &status).asFloat();
	CHECK_MSTATUS_AND_RETURN_IT(status);
	m_scaleY = data.inputValue(aScaleY, &status).asFloat();
	CHECK_MSTATUS_AND_RETURN_IT(status);
	m_scaleZ = data.inputValue(aScaleZ, &status).asFloat();
	CHECK_MSTATUS_AND_RETURN_IT(status);


	m_randomOffsetX = data.inputValue(aRndOffsetX, &status).asFloat();
	CHECK_MSTATUS_AND_RETURN_IT(status);
	m_randomOffsetY = data.inputValue(aRndOffsetY, &status).asFloat();
	CHECK_MSTATUS_AND_RETURN_IT(status);
	m_randomOffsetZ = data.inputValue(aRndOffsetZ, &status).asFloat();
	CHECK_MSTATUS_AND_RETURN_IT(status);

	m_randomRotateX = data.inputValue(aRndRotateX, &status).asFloat();
	CHECK_MSTATUS_AND_RETURN_IT(status);
	m_randomRotateY = data.inputValue(aRndRotateY, &status).asFloat();
	CHECK_MSTATUS_AND_RETURN_IT(status);
	m_randomRotateZ = data.inputValue(aRndRotateZ, &status).asFloat();
	CHECK_MSTATUS_AND_RETURN_IT(status);

	m_randomScaleX = data.inputValue(aRndScaleX, &status).asFloat();
	CHECK_MSTATUS_AND_RETURN_IT(status);
	m_randomScaleY = data.inputValue(aRndScaleY, &status).asFloat();
	CHECK_MSTATUS_AND_RETURN_IT(status);
	m_randomScaleZ = data.inputValue(aRndScaleZ, &status).asFloat();
	CHECK_MSTATUS_AND_RETURN_IT(status);

	m_uvOffsetU = data.inputValue(aUVOffsetU, &status).asFloat();
	CHECK_MSTATUS_AND_RETURN_IT(status);
	m_uvOffsetV = data.inputValue(aUVOffsetV, &status).asFloat();
	CHECK_MSTATUS_AND_RETURN_IT(status);

	m_rndUvOffsetU = data.inputValue(aRndUVOffsetU, &status).asFloat();
	CHECK_MSTATUS_AND_RETURN_IT(status);
	m_rndUvOffsetV = data.inputValue(aRndUVOffsetV, &status).asFloat();
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

	m_displayEdges = data.inputValue(aDisplayEdges, &status).asBool();
	CHECK_MSTATUS_AND_RETURN_IT(status);

	m_connectPieces = data.inputValue(aConnectPieces, &status).asBool();
	CHECK_MSTATUS_AND_RETURN_IT(status);

	m_mergePieces = data.inputValue(aMergePieces, &status).asBool();
	CHECK_MSTATUS_AND_RETURN_IT(status);

	m_connectLoop = data.inputValue(aConnectLoop, &status).asBool();
	CHECK_MSTATUS_AND_RETURN_IT(status);

	m_interpolate = data.inputValue(aInterpolate, &status).asBool();
	CHECK_MSTATUS_AND_RETURN_IT(status);

	m_orientCurveToRefGeo = data.inputValue(aOrientCurveToRefGeo, &status).asBool();
	CHECK_MSTATUS_AND_RETURN_IT(status);

	m_showRoot = data.inputValue(aShowRoot, &status).asBool();
	CHECK_MSTATUS_AND_RETURN_IT(status);

	m_transformXRule = data.inputValue(aTransformXRule, &status).asString();
	CHECK_MSTATUS_AND_RETURN_IT(status);

	m_transformYRule = data.inputValue(aTransformYRule, &status).asString();
	CHECK_MSTATUS_AND_RETURN_IT(status);

	m_transformZRule = data.inputValue(aTransformZRule, &status).asString();
	CHECK_MSTATUS_AND_RETURN_IT(status);

	m_rotateXRule = data.inputValue(aRotateXRule, &status).asString();
	CHECK_MSTATUS_AND_RETURN_IT(status);

	m_rotateYRule = data.inputValue(aRotateYRule, &status).asString();
	CHECK_MSTATUS_AND_RETURN_IT(status);

	m_rotateZRule = data.inputValue(aRotateZRule, &status).asString();
	CHECK_MSTATUS_AND_RETURN_IT(status);

	m_scaleXRule = data.inputValue(aScaleXRule, &status).asString();
	CHECK_MSTATUS_AND_RETURN_IT(status);

	m_scaleYRule = data.inputValue(aScaleYRule, &status).asString();
	CHECK_MSTATUS_AND_RETURN_IT(status);

	m_scaleZRule = data.inputValue(aScaleZRule, &status).asString();
	CHECK_MSTATUS_AND_RETURN_IT(status);

	m_wrap_to_surface = data.inputValue(aWrapToSurface, &status).asBool();
	CHECK_MSTATUS_AND_RETURN_IT(status);

	// Override instace count if instance type is set to Circle
	if (m_instanceType == 1) { m_instanceZ = 1; }
	if (m_instanceType == 5) { m_instanceZ = 1; }
	//if (m_instanceType == 4) { m_rotateX+=180.0;}

	m_numDup = m_instanceX * m_instanceY * m_instanceZ; if (m_numDup < 1) { m_numDup = 1; }


	// Ramp Offset attribute
	MRampAttribute a_offsetAttributeX(this->thisMObject(), aGridOffsetRampX, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);
	m_offsetProfileAX = storeProfileCurveData(a_offsetAttributeX, m_numDup);

	MRampAttribute a_offsetAttributeY(this->thisMObject(), aGridOffsetRampY, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);
	m_offsetProfileAY = storeProfileCurveData(a_offsetAttributeY, m_numDup);

	MRampAttribute a_offsetAttributeZ(this->thisMObject(), aGridOffsetRampZ, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);
	m_offsetProfileAZ = storeProfileCurveData(a_offsetAttributeZ, m_numDup);

	// Ramp Rotate attribute

	MRampAttribute a_rotateAttributeX(this->thisMObject(), aRotateRampX, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);
	m_rotateProfileAX = storeProfileCurveData(a_rotateAttributeX, m_numDup);

	MRampAttribute a_rotateAttributeY(this->thisMObject(), aRotateRampY, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);
	m_rotateProfileAY = storeProfileCurveData(a_rotateAttributeY, m_numDup);

	MRampAttribute a_rotateAttributeZ(this->thisMObject(), aRotateRampZ, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);
	m_rotateProfileAZ = storeProfileCurveData(a_rotateAttributeZ, m_numDup);

	// Ramp Scale attribute

	MRampAttribute a_scaleAttributeX(this->thisMObject(), aScaleRampX, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);
	m_scaleProfileAX = storeProfileCurveData(a_scaleAttributeX, m_numDup);

	MRampAttribute a_scaleAttributeY(this->thisMObject(), aScaleRampY, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);
	m_scaleProfileAY = storeProfileCurveData(a_scaleAttributeY, m_numDup);

	MRampAttribute a_scaleAttributeZ(this->thisMObject(), aScaleRampZ, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);
	m_scaleProfileAZ = storeProfileCurveData(a_scaleAttributeZ, m_numDup);

	// Override instace count if instance type is set to Mesh
	if (m_instanceType == 5) { overrideInstanceOnMeshSettings(); }

	//cout << "-----------------" << endl;

	return MStatus::kSuccess;

}


MFloatArray ClonerMultiThread::storeProfileCurveData(MRampAttribute a_segmentsAttribute, int segments)
{
	MStatus status;

	MFloatArray curve_segments_values;

	for (int i = 0; i < segments; i++)
	{
		float rampPosition = (1.0f / float(segments - 1)) * float(i);
		float curveRampValue;

		a_segmentsAttribute.getValueAtPosition(rampPosition, curveRampValue, &status);
		CHECK_MSTATUS(status);

		curve_segments_values.append(curveRampValue);

	}

	if (segments == 0)
	{
		curve_segments_values.append(1);
	}

	return curve_segments_values;

}

MFloatArray ClonerMultiThread::generateRndArray(float& surfaceNoise, int& numValues, int& seedVal)
{

	// Random Surface

	MFloatArray randArray;
	srand(seedVal);

	float randVal;

	for (int i = 0; i < numValues; i++) {

		randVal = (rand() / ((float)RAND_MAX)) * surfaceNoise;
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
	p_refLoc = MPlug(this->thisMObject(), aRefLocPos);

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

	if (m_instanceType == 3)
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


	if (m_instanceType == 4)
	{
		if (!p_inCurve.isConnected())
		{
			return MStatus::kSuccess;
		}
	}

	if (m_instanceType == 5)
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
MStatus ClonerMultiThread::compute(const MPlug& plug, MDataBlock& data)
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

		MFnMeshData ex_meshDataFn;
		MObject ex_newMeshData = ex_meshDataFn.create(&status);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		MFnMesh ex_meshFn;

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

		// No input mesh attached so exit node.

		MGlobal::displayWarning(MString() + "[ClonerMulti] No input meshes connected... ");

		return MS::kSuccess;
	}




	if (plug == aOutMesh)
	{

		// Collect input meshes
		status = collectInputMeshes(data);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		// Generate rule arrays

		storeRules();

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
		if (m_instanceType == 0) { status = instanceGrid(); CHECK_MSTATUS_AND_RETURN_IT(status); }
		else if (m_instanceType == 1) { status = instanceCircle(); CHECK_MSTATUS_AND_RETURN_IT(status); }
		else if (m_instanceType == 2) { status = instanceFibonacciSphere(); CHECK_MSTATUS_AND_RETURN_IT(status); }

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

		else { status = instanceGrid(); CHECK_MSTATUS_AND_RETURN_IT(status); }


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
				m_idA.clear(); for (int i = 0; i < m_numDup; i++) { m_idA.append(0); }


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


				if (m_connectPieces && m_mergePieces && m_numInputMeshes == 1)
				{

					MFloatPointArray n_vertexArray;

					for (int i = 0; i < o_vertexArray.length(); i++)
					{

						// Find in array
						bool is_found = false;
						for (int f = 0; f < m_connPairVB.length(); f++)
						{
							if (m_connPairVB[f] == i)
							{
								is_found = true;
								break;
							}
						}

						if (!is_found)
						{
							n_vertexArray.append(o_vertexArray[i]);
						}

					}

					MIntArray n_polygonConnects;
					status = n_polygonConnects.copy(o_polygonConnects);
					CHECK_MSTATUS_AND_RETURN_IT(status);

					o_polygonConnects.clear();
					o_vertexArray.clear();

					int largestnum_delete = -1;

					for (int z = 0; z < m_connPairVB.length(); z++)
					{
		

						for (int i = 0; i < n_polygonConnects.length(); i++)
						{

							if (n_polygonConnects[i] == m_connPairVB[z])
							{
								n_polygonConnects[i] = m_connPairVA[z];
							}
							if (n_polygonConnects[i] > m_connPairVB[z])
							{
								n_polygonConnects[i] -= 1;
								largestnum_delete = m_connPairVB[z];
							}

						}


						for (int t = 0; t < m_connPairVB.length(); t++)
						{
							if (m_connPairVB[t] > largestnum_delete)
							{
								m_connPairVB[t] -= 1;
							}
						}

						for (int t = 0; t < m_connPairVA.length(); t++)
						{
							if (m_connPairVA[t] > largestnum_delete)
							{
								m_connPairVA[t] -= 1;
							}
						}

					}

					int n_numVertices = o_numVertices - m_connPairVB.length();

				/*	if (m_connectLoop)
					{
						n_numVertices -= m_ConnectArrayA.length();
					}*/


					ex_meshFn.create(n_numVertices, o_numPolygons, n_vertexArray, o_polygonCounts, n_polygonConnects, o_uArrayA, o_vArrayA, ex_newMeshData, &status);
					CHECK_MSTATUS_AND_RETURN_IT(status);


				}

				else
				{
					ex_meshFn.create(o_numVertices, o_numPolygons, o_vertexArray, o_polygonCounts, o_polygonConnects, o_uArrayA, o_vArrayA, ex_newMeshData, &status);
					CHECK_MSTATUS_AND_RETURN_IT(status);
				}



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

		//MGlobal::displayInfo("-------");

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





#if MAYA_API_VERSION > 201600
		// Output matrix
		MFnMatrixArrayData ex_matrixDataFn;
		MObject ex_matrixData = ex_matrixDataFn.create(m_tr_matA, &status);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		// Send matrix array to output datablock
		status = h_outputMatrix.set(ex_matrixData);
		CHECK_MSTATUS_AND_RETURN_IT(status);
#endif

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
void ClonerMultiThread::draw(M3dView& view, const MDagPath& path, M3dView::DisplayStyle style, M3dView::DisplayStatus status)
{
	MObject thisNode = thisMObject();

	MFnDependencyNode fnDepCloner(thisNode);
#if MAYA_API_VERSION < 201800
	// Draw locator
	view.beginGL();

	glPushAttrib(GL_CURRENT_BIT);

	if (status == M3dView::kActive) {
		view.setDrawColor(13, M3dView::kActiveColors);
	}
	else {
		view.setDrawColor(13, M3dView::kDormantColors);
	}


	view.drawText(".", MPoint::origin, M3dView::kCenter);



	glPopAttrib();

	view.endGL();
#endif
}


// VP 2.0 Override functions

MHWRender::DrawAPI ClonerMultiThreadOverride::supportedDrawAPIs() const
{

#if MAYA_API_VERSION > 201600

	return (MHWRender::kOpenGL | MHWRender::kDirectX11 | MHWRender::kOpenGLCoreProfile);

#else
	return (MHWRender::kOpenGL | MHWRender::kDirectX11);
#endif

}

bool ClonerMultiThreadOverride::isBounded(const MDagPath& /*objPath*/, const MDagPath& /*cameraPath*/) const
{
	return true;
}

MBoundingBox ClonerMultiThreadOverride::boundingBox(const MDagPath& objPath, const MDagPath& cameraPath) const
{


	MStatus status;
	MObject CLonerMultiNode = objPath.node(&status);


	MPoint corner1(-1.0, 1.0, -1.0);
	MPoint corner2(1.0, 0.0, 1.0);

	return MBoundingBox(corner1, corner2);

	//return m_bbP;
}

vector<MPointArray> ClonerMultiThreadOverride::getEdgePoints(const MDagPath& objPath) const
{
	MStatus status;


	MObject locatorNode = objPath.node(&status);

	if (!status)
	{
		return vector<MPointArray>();
	}

	//

	MPlug refMeshPlug(locatorNode, ClonerMultiThread::aRefMesh);
	vector<MPointArray> currPA;


	

	// Get bounding box
	if (!refMeshPlug.isConnected())
	{
		return currPA;
	}

	MDagPath mesh_path;


	MPlugArray inputs_plugArr;
	refMeshPlug.connectedTo(inputs_plugArr, true, false, &status);

	if (status)
	{
		MPlug currP = inputs_plugArr[0];

		if (!currP.isNull())
		{

			MFnDagNode inMesh_dn(currP.node());
			inMesh_dn.getPath(mesh_path);

		}
	}

	
	if (!mesh_path.isValid()) { return currPA; }


		// Get input point array
	MIntArray m_customEdgeComponents;
	MPlug p_customEdge(locatorNode, ClonerMultiThread::aCustomEdgeComponent);
	MObject o_edgeArray;
	p_customEdge.getValue(o_edgeArray);
	MFnIntArrayData fn_aPoints(o_edgeArray);
	fn_aPoints.copyTo(m_customEdgeComponents);

	MFnSingleIndexedComponent customEdgeComponents;
	// Add the vertex ids to the component MObject
	customEdgeComponents.addElements(m_customEdgeComponents);

	// Creates an MObject of type kMeshVertComponent
	MObject m_o_customEdgeComponents = customEdgeComponents.create(MFn::kMeshEdgeComponent, &status);

	// Add the vertex ids to the component MObject
	customEdgeComponents.addElements(m_customEdgeComponents);

	MItMeshEdge itEdge(mesh_path, m_o_customEdgeComponents, &status);

	//

	for (itEdge.reset(); !itEdge.isDone(); itEdge.next())
	{
		// Calculate edge trmat
		MPoint vertA = itEdge.point(0, MSpace::kObject, &status);
		MPoint vertB = itEdge.point(1, MSpace::kObject, &status);

		MPointArray pA;
		pA.append(vertA);
		pA.append(vertB);

		currPA.push_back(pA);
	}

	return currPA;
}

vector<MPointArray> ClonerMultiThreadOverride::getInstancePoints(const MDagPath& objPath) const
{
	MStatus status;


	MObject locatorNode = objPath.node(&status);

	if (!status)
	{
		return vector<MPointArray>();
	}


#if MAYA_API_VERSION > 201600
	MPlug pointsPlug(locatorNode, ClonerMultiThread::aOutMatrixArray);
#endif
	MPlug inMeshPlug(locatorNode, ClonerMultiThread::aInMesh);
	MPlug idPlug(locatorNode, ClonerMultiThread::aOutIDArray);

	vector<MBoundingBox> inmeshBBA;
	vector<MPointArray> currPA;


	// Get bounding box
	if (inMeshPlug.numConnectedElements() != 0)
	{

		for (auto i = 0; i < inMeshPlug.numConnectedElements(); i++)
		{




			MPlug currP = inMeshPlug.elementByPhysicalIndex(i, &status);

			if (status)
			{

				if (currP.isConnected())
				{

					MPlugArray inputs_plugArr;
					currP.connectedTo(inputs_plugArr, true, false, &status);

					if (status)
					{
						currP = inputs_plugArr[0];

						if (!currP.isNull())
						{

							MFnDagNode inMesh_dn(currP.node());
							MBoundingBox bb = inMesh_dn.boundingBox();

							inmeshBBA.push_back(bb);

						}
					}


				}
			}
		}

	}





#if MAYA_API_VERSION > 201600



	if (inMeshPlug.numConnectedElements() != 0)
	{
		if (!pointsPlug.isNull() && !idPlug.isNull())
		{

			MObject matrixObject;
			pointsPlug.getValue(matrixObject);

			MObject idObject;
			idPlug.getValue(idObject);

			MFnIntArrayData idArrayData(idObject);
			MIntArray out_idArray;
			idArrayData.copyTo(out_idArray);

			MFnMatrixArrayData worldMatrixData(matrixObject);
			MMatrixArray out_matrixArray;
			worldMatrixData.copyTo(out_matrixArray);


			//currPA.setLength(out_matrixArray.length());

			if (out_matrixArray.length() == out_idArray.length())
			{

				/*			MGlobal::displayInfo("yolooooooo");*/


				for (auto i = 0; i < out_matrixArray.length(); i++)
				{


					MBoundingBox bb;


					bb = inmeshBBA[out_idArray[i]];

					double w = bb.width() * 0.5;
					double h = bb.height() * 0.5;
					double d = bb.depth() * 0.5;

					double cx = bb.center().x;
					double cy = bb.center().y;
					double cz = bb.center().x;

					//if (!m_worldSpace) { currentPoint = (MPoint(currentPoint) *= m_inMeshMatrixArray[idA[m]].inverse()); }

					MPoint p00 = MPoint(-w, -h, d, 1.0f) * out_matrixArray[i];
					MPoint p01 = MPoint(w, -h, d, 1.0f) * out_matrixArray[i];
					MPoint p02 = MPoint(-w, h, d, 1.0f) * out_matrixArray[i];
					MPoint p03 = MPoint(w, h, d, 1.0f) * out_matrixArray[i];
					MPoint p04 = MPoint(-w, h, -d, 1.0f) * out_matrixArray[i];
					MPoint p05 = MPoint(w, h, -d, 1.0f) * out_matrixArray[i];
					MPoint p06 = MPoint(-w, -h, -d, 1.0f) * out_matrixArray[i];
					MPoint p07 = MPoint(w, -h, -d, 1.0f) * out_matrixArray[i];
					MPoint p08 = MPoint(-w, -h, d, 1.0f) * out_matrixArray[i];
					MPoint p09 = MPoint(-w, h, d, 1.0f) * out_matrixArray[i];
					MPoint p10 = MPoint(w, -h, d, 1.0f) * out_matrixArray[i];
					MPoint p11 = MPoint(w, h, d, 1.0f) * out_matrixArray[i];
					MPoint p12 = MPoint(-w, h, d, 1.0f) * out_matrixArray[i];
					MPoint p13 = MPoint(-w, h, -d, 1.0f) * out_matrixArray[i];
					MPoint p14 = MPoint(w, h, d, 1.0f) * out_matrixArray[i];
					MPoint p15 = MPoint(w, h, -d, 1.0f) * out_matrixArray[i];
					MPoint p16 = MPoint(-w, h, -d, 1.0f) * out_matrixArray[i];
					MPoint p17 = MPoint(-w, -h, -d, 1.0f) * out_matrixArray[i];
					MPoint p18 = MPoint(w, h, -d, 1.0f) * out_matrixArray[i];
					MPoint p19 = MPoint(w, -h, -d, 1.0f) * out_matrixArray[i];
					MPoint p20 = MPoint(-w, -h, -d, 1.0f) * out_matrixArray[i];
					MPoint p21 = MPoint(-w, -h, d, 1.0f) * out_matrixArray[i];
					MPoint p22 = MPoint(w, -h, -d, 1.0f) * out_matrixArray[i];
					MPoint p23 = MPoint(w, -h, d, 1.0f) * out_matrixArray[i];

					MPointArray pA;


					pA.append(p00);
					pA.append(p01);
					pA.append(p02);
					pA.append(p03);
					pA.append(p04);
					pA.append(p05);
					pA.append(p06);
					pA.append(p07);
					pA.append(p08);
					pA.append(p09);
					pA.append(p10);
					pA.append(p11);
					pA.append(p12);
					pA.append(p13);
					pA.append(p14);
					pA.append(p15);
					pA.append(p16);
					pA.append(p17);
					pA.append(p18);
					pA.append(p19);
					pA.append(p20);
					pA.append(p21);
					pA.append(p22);
					pA.append(p23);



					currPA.push_back(pA);

				}

			}

			//for (int i = 0; i < out_matrixArray.length(); i++)
			//{
			//	/*currPA[i] = MPoint() * out_matrixArray[i];*/




			//}

		}
	}

#endif

	return currPA;
}



// Called by Maya each time the object needs to be drawn.
MUserData* ClonerMultiThreadOverride::prepareForDraw(const MDagPath& objPath, const MDagPath& cameraPath, const MHWRender::MFrameContext& frameContext, MUserData* oldData)
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
	MPlug dispEdgesPlug(objPath.node(), ClonerMultiThread::aDisplayEdges);
	MPlug showRootPlug(objPath.node(), ClonerMultiThread::aShowRoot);

	data->m_displayProxy = dispProxyPlug.asBool();
	data->m_displayEdges = dispEdgesPlug.asBool();
	data->m_showRoot = showRootPlug.asBool();

	data->m_dispPointA = getInstancePoints(objPath);
	data->m_edgeLineA = getEdgePoints(objPath);


	data->m_inLoc_mat = objPath.exclusiveMatrix();

	// get correct color based on the state of object, e.g. active or dormant
	data->m_locColor = MHWRender::MGeometryUtilities::wireframeColor(objPath);

	return data;
}


void ClonerMultiThreadOverride::addUIDrawables(const MDagPath& objPath, MHWRender::MUIDrawManager& drawManager, const MHWRender::MFrameContext& frameContext, const MUserData* data)
{
	ClonerMultiThreadData* pLocatorData = (ClonerMultiThreadData*)data;
	if (!pLocatorData)
	{
		return;
	}

	drawManager.beginDrawable();



	MColor fillCol, lineCol;

	fillCol = MColor(1.0f, 0.5f, 0.3f, 1.0f);



	if (frameContext.getDisplayStyle() & MHWRender::MDrawContext::kWireFrame) {


		fillCol = MColor(pLocatorData->m_locColor.r, pLocatorData->m_locColor.g, pLocatorData->m_locColor.b, 0.0);

	}

	if (MHWRender::MGeometryUtilities::displayStatus(objPath) == M3dView::kLead) {


		fillCol = MColor(pLocatorData->m_locColor.r, pLocatorData->m_locColor.g, pLocatorData->m_locColor.b, fillCol.a);

	}

	if (MHWRender::MGeometryUtilities::displayStatus(objPath) == M3dView::kActive) {

		fillCol = MColor(pLocatorData->m_locColor.r, pLocatorData->m_locColor.g, pLocatorData->m_locColor.b, fillCol.a);

	}

	if (MHWRender::MGeometryUtilities::displayStatus(objPath) == M3dView::kTemplate) {

		fillCol = MColor(pLocatorData->m_locColor.r, pLocatorData->m_locColor.g, pLocatorData->m_locColor.b, fillCol.a);

	}

	if (pLocatorData->m_displayEdges)
	{
		if (pLocatorData->m_edgeLineA.size() > 0)
		{
			drawManager.setColor(MColor(1.0f, 0.0f, 0.0f, 1.0f));
			drawManager.setLineWidth(12);
			drawManager.setLineStyle(MHWRender::MUIDrawManager::kDotted);

			for (auto i = 0; i < pLocatorData->m_edgeLineA.size(); i++)
			{
				drawManager.mesh(MHWRender::MUIDrawManager::kLines, pLocatorData->m_edgeLineA[i]);
			}
		}
	}

	if (pLocatorData->m_displayProxy)
	{

		drawManager.setColor(MColor(fillCol.r, fillCol.g, fillCol.b, 1.0f));
		//drawManager.setPointSize(4);

		//drawManager.setLineStyle(MHWRender::MUIDrawManager::kDotted);

		for (auto i = 0; i < pLocatorData->m_dispPointA.size(); i++)
		{
			drawManager.mesh(MHWRender::MUIDrawManager::kLines, pLocatorData->m_dispPointA[i]);
		}

	}


	if (pLocatorData->m_showRoot)
	{
		M3dView view = M3dView::active3dView();
		short ox, oy;

		MPoint p = MPoint::origin;
		p *= pLocatorData->m_inLoc_mat;

		view.worldToView(p, ox, oy);

		drawManager.setColor(MColor(fillCol.r + 0.5, fillCol.g + 0.5, fillCol.b + 0.5, 1.0f));
		drawManager.circle2d(MPoint(ox, oy), 3.0, false);
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
	MRampAttribute				rAttr;

	MFnStringData      fnStringData;
	MObject            defaultStringA;
	defaultStringA = fnStringData.create("0,1");


	// Plugs
	ClonerMultiThread::aInMesh = tAttr.create("inMesh", "inMesh", MFnData::kMesh);
	tAttr.setDisconnectBehavior(MFnAttribute::kDelete);
	tAttr.setChannelBox(false);
	tAttr.setArray(true);
	tAttr.setWritable(true);
	tAttr.setReadable(false);
	addAttribute(ClonerMultiThread::aInMesh);

	ClonerMultiThread::aOutMesh = tAttr.create("outMesh", "outMesh", MFnData::kMesh);
	tAttr.setChannelBox(false);
	tAttr.setWritable(false);
	tAttr.setReadable(true);
	addAttribute(ClonerMultiThread::aOutMesh);
#if MAYA_API_VERSION > 201600
	ClonerMultiThread::aOutMatrixArray = tAttr.create("outMatrixArray", "outMatrixArray", MFnMatrixData::kMatrixArray);
	tAttr.setChannelBox(false);
	tAttr.setWritable(false);
	tAttr.setReadable(true);
	tAttr.setHidden(true);
	addAttribute(ClonerMultiThread::aOutMatrixArray);
#endif
	ClonerMultiThread::aOutIDArray = tAttr.create("outIDArray", "outIDArray", MFnIntArrayData::kIntArray);
	tAttr.setChannelBox(false);
	tAttr.setWritable(false);
	tAttr.setReadable(true);
	tAttr.setHidden(true);
	addAttribute(ClonerMultiThread::aOutIDArray);

	ClonerMultiThread::aRefMesh = tAttr.create("referenceMesh", "referenceMesh", MFnData::kMesh);
	tAttr.setWritable(true);
	tAttr.setReadable(false);
	tAttr.setStorable(false);
	tAttr.setKeyable(true);
	addAttribute(ClonerMultiThread::aRefMesh);

	ClonerMultiThread::aRefMeshSmooth = tAttr.create("referenceMeshSmooth", "referenceMeshSmooth", MFnData::kMesh);
	tAttr.setWritable(true);
	tAttr.setReadable(false);
	tAttr.setStorable(false);
	tAttr.setKeyable(false);
	//tAttr.setInternal(true);
	addAttribute(ClonerMultiThread::aRefMeshSmooth);

	ClonerMultiThread::aInCurve = tAttr.create("inCurve", "inCurve", MFnData::kNurbsCurve);
	tAttr.setWritable(true);
	tAttr.setReadable(false);
	tAttr.setStorable(false);
	tAttr.setKeyable(true);
	addAttribute(ClonerMultiThread::aInCurve);

	// A - B in locators
	ClonerMultiThread::aInLocAPos = mAttr.create("locatorAPos", "locatorAPos", MFnMatrixAttribute::kDouble);
	mAttr.setChannelBox(false);
	mAttr.setWritable(true);
	mAttr.setReadable(false);
	mAttr.setStorable(false);
	mAttr.setKeyable(false);
	addAttribute(ClonerMultiThread::aInLocAPos);

	ClonerMultiThread::aInLocBPos = mAttr.create("locatorBPos", "locatorBPos", MFnMatrixAttribute::kDouble);
	mAttr.setChannelBox(false);
	mAttr.setWritable(true);
	mAttr.setReadable(false);
	mAttr.setStorable(false);
	mAttr.setKeyable(false);
	addAttribute(ClonerMultiThread::aInLocBPos);


	ClonerMultiThread::aRefLocPos = mAttr.create("referenceLocator", "referenceLocator", MFnMatrixAttribute::kDouble);
	mAttr.setChannelBox(false);
	mAttr.setWritable(true);
	mAttr.setReadable(false);
	mAttr.setStorable(false);
	mAttr.setKeyable(false);
	addAttribute(ClonerMultiThread::aRefLocPos);

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
	eAttr.addField("Polygon Borders", 3);
	eAttr.addField("Custom Edges", 4);
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
	nAttr.setSoftMax(180.0);
	nAttr.setSoftMin(-180.0);
	nAttr.setKeyable(true);
	nAttr.setChannelBox(true);
	addAttribute(ClonerMultiThread::aRotateX);

	ClonerMultiThread::aRotateY = nAttr.create("rotateY", "rotateY", MFnNumericData::kFloat);
	nAttr.setStorable(true);
	nAttr.setDefault(0.0);
	nAttr.setSoftMax(180.0);
	nAttr.setSoftMin(-180.0);
	nAttr.setKeyable(true);
	nAttr.setChannelBox(true);
	addAttribute(ClonerMultiThread::aRotateY);

	ClonerMultiThread::aRotateZ = nAttr.create("rotateZ", "rotateZ", MFnNumericData::kFloat);
	nAttr.setStorable(true);
	nAttr.setDefault(0.0);
	nAttr.setSoftMax(180.0);
	nAttr.setSoftMin(-180.0);
	nAttr.setKeyable(true);
	nAttr.setChannelBox(true);
	addAttribute(ClonerMultiThread::aRotateZ);

	// Circle rotate
	ClonerMultiThread::aCircleRotate = nAttr.create("circleRotate", "circleRotate", MFnNumericData::kFloat);
	nAttr.setStorable(true);
	nAttr.setDefault(0.0);
	nAttr.setSoftMax(180.0);
	nAttr.setSoftMin(-180.0);
	nAttr.setKeyable(true);
	nAttr.setChannelBox(true);
	addAttribute(ClonerMultiThread::aCircleRotate);


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


	ClonerMultiThread::aFirstUpVecX = nAttr.create("firstUpVecX", "fux", MFnNumericData::kDouble, 0);
	nAttr.setStorable(true);
	nAttr.setKeyable(true);
	addAttribute(ClonerMultiThread::aFirstUpVecX);

	ClonerMultiThread::aFirstUpVecY = nAttr.create("firstUpVecY", "fuy", MFnNumericData::kDouble, 1);
	nAttr.setStorable(true);
	nAttr.setKeyable(true);
	addAttribute(ClonerMultiThread::aFirstUpVecY);


	ClonerMultiThread::aFirstUpVecZ = nAttr.create("firstUpVecZ", "fuz", MFnNumericData::kDouble, 0);
	nAttr.setStorable(true);
	nAttr.setKeyable(true);
	addAttribute(ClonerMultiThread::aFirstUpVecZ);


	ClonerMultiThread::aFirstUpVec = cAttr.create("firstUpVec", "fu");
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


	ClonerMultiThread::aShowRoot = nAttr.create("showRootLoc", "showRootLoc", MFnNumericData::kBoolean);
	nAttr.setStorable(true);
	nAttr.setDefault(true);
	nAttr.setKeyable(true);
	nAttr.setChannelBox(true);
	addAttribute(ClonerMultiThread::aShowRoot);

	ClonerMultiThread::aWrapToSurface = nAttr.create("wrapToSurface", "wrapToSurface", MFnNumericData::kBoolean);
	nAttr.setStorable(true);
	nAttr.setDefault(false);
	nAttr.setKeyable(true);
	nAttr.setChannelBox(true);
	addAttribute(ClonerMultiThread::aWrapToSurface);

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

	ClonerMultiThread::aDisplayEdges = nAttr.create("displayEdges", "displayEdges", MFnNumericData::kBoolean);
	nAttr.setStorable(true);
	nAttr.setDefault(true);
	nAttr.setKeyable(true);
	nAttr.setChannelBox(true);
	addAttribute(ClonerMultiThread::aDisplayEdges);

	ClonerMultiThread::aConnectPieces = nAttr.create("connectPieces", "connectPieces", MFnNumericData::kBoolean);
	nAttr.setStorable(true);
	nAttr.setDefault(false);
	nAttr.setKeyable(true);
	nAttr.setChannelBox(true);
	addAttribute(ClonerMultiThread::aConnectPieces);

	ClonerMultiThread::aMergePieces = nAttr.create("mergePieces", "mergePieces", MFnNumericData::kBoolean);
	nAttr.setStorable(true);
	nAttr.setDefault(true);
	nAttr.setKeyable(true);
	nAttr.setChannelBox(true);
	addAttribute(ClonerMultiThread::aMergePieces);

	ClonerMultiThread::aConnectLoop = nAttr.create("connectLoop", "connectLoop", MFnNumericData::kBoolean);
	nAttr.setStorable(true);
	nAttr.setDefault(false);
	nAttr.setKeyable(true);
	nAttr.setChannelBox(true);
	addAttribute(ClonerMultiThread::aConnectLoop);

	ClonerMultiThread::aInterpolate = nAttr.create("interpolate", "interpolate", MFnNumericData::kBoolean);
	nAttr.setStorable(true);
	nAttr.setDefault(false);
	nAttr.setKeyable(true);
	nAttr.setChannelBox(true);
	addAttribute(ClonerMultiThread::aInterpolate);

	ClonerMultiThread::aOrientCurveToRefGeo = nAttr.create("orientCurveToRefGeo", "orientCurveToRefGeo", MFnNumericData::kBoolean, false);
	nAttr.setStorable(true);
	nAttr.setKeyable(true);
	nAttr.setChannelBox(true);
	addAttribute(ClonerMultiThread::aOrientCurveToRefGeo);

	ClonerMultiThread::aSmoothMeshPreview = nAttr.create("smoothMeshPreview", "smoothMeshPreview", MFnNumericData::kBoolean, false);
	nAttr.setStorable(false);
	nAttr.setKeyable(false);
	nAttr.setChannelBox(false);
	//nAttr.setInternal(true);
	addAttribute(ClonerMultiThread::aSmoothMeshPreview);

	ClonerMultiThread::aConnectArrayA = tAttr.create("connectArrayA", "connectArrayA", MFnData::kString, defaultStringA);
	tAttr.setStorable(true);
	tAttr.setChannelBox(false);
	addAttribute(ClonerMultiThread::aConnectArrayA);

	ClonerMultiThread::aConnectArrayB = tAttr.create("connectArrayB", "connectArrayB", MFnData::kString, defaultStringA);
	tAttr.setStorable(true);
	tAttr.setChannelBox(false);
	ClonerMultiThread::addAttribute(aConnectArrayB);

	// Custom Edges
	ClonerMultiThread::aCustomEdgeComponent = tAttr.create("customEdgeComponent", "customEdgeComponent", MFnIntArrayData::kIntArray);
	tAttr.setStorable(true);
	tAttr.setInternal(true);
	tAttr.setHidden(true);
	ClonerMultiThread::addAttribute(aCustomEdgeComponent);

	// Rulest Transform

	ClonerMultiThread::aTransformXRule = tAttr.create("transformXRule", "transformXRule", MFnData::kString);
	tAttr.setStorable(true);
	tAttr.setChannelBox(false);
	ClonerMultiThread::addAttribute(aTransformXRule);

	ClonerMultiThread::aTransformYRule = tAttr.create("transformYRule", "transformYRule", MFnData::kString);
	tAttr.setStorable(true);
	tAttr.setChannelBox(false);
	ClonerMultiThread::addAttribute(aTransformYRule);

	ClonerMultiThread::aTransformZRule = tAttr.create("transformZRule", "transformZRule", MFnData::kString);
	tAttr.setStorable(true);
	tAttr.setChannelBox(false);
	ClonerMultiThread::addAttribute(aTransformZRule);

	// Rules Rotate

	ClonerMultiThread::aRotateXRule = tAttr.create("rotateXRule", "rotateXRule", MFnData::kString);
	tAttr.setStorable(true);
	tAttr.setChannelBox(false);
	ClonerMultiThread::addAttribute(aRotateXRule);

	ClonerMultiThread::aRotateYRule = tAttr.create("rotateYRule", "rotateYRule", MFnData::kString);
	tAttr.setStorable(true);
	tAttr.setChannelBox(false);
	ClonerMultiThread::addAttribute(aRotateYRule);

	ClonerMultiThread::aRotateZRule = tAttr.create("rotateZRule", "rotateZRule", MFnData::kString);
	tAttr.setStorable(true);
	tAttr.setChannelBox(false);
	ClonerMultiThread::addAttribute(aRotateZRule);

	// Scale Rotate

	ClonerMultiThread::aScaleXRule = tAttr.create("scaleXRule", "scaleXRule", MFnData::kString);
	tAttr.setStorable(true);
	tAttr.setChannelBox(false);
	ClonerMultiThread::addAttribute(aScaleXRule);

	ClonerMultiThread::aScaleYRule = tAttr.create("scaleYRule", "scaleYRule", MFnData::kString);
	tAttr.setStorable(true);
	tAttr.setChannelBox(false);
	ClonerMultiThread::addAttribute(aScaleYRule);

	ClonerMultiThread::aScaleZRule = tAttr.create("scaleZRule", "scaleZRule", MFnData::kString);
	tAttr.setStorable(true);
	tAttr.setChannelBox(false);
	ClonerMultiThread::addAttribute(aScaleZRule);

	//

	ClonerMultiThread::aGridOffsetRampX = rAttr.createCurveRamp("offsetRampX", "offsetRampX");
	ClonerMultiThread::addAttribute(aGridOffsetRampX);

	ClonerMultiThread::aGridOffsetRampY = rAttr.createCurveRamp("offsetRampY", "offsetRampY");
	ClonerMultiThread::addAttribute(aGridOffsetRampY);

	ClonerMultiThread::aGridOffsetRampZ = rAttr.createCurveRamp("offsetRampZ", "offsetRampZ");
	ClonerMultiThread::addAttribute(aGridOffsetRampZ);

	ClonerMultiThread::aRotateRampX = rAttr.createCurveRamp("rotateRampX", "rotateRampX");
	ClonerMultiThread::addAttribute(aRotateRampX);

	ClonerMultiThread::aRotateRampY = rAttr.createCurveRamp("rotateRampY", "rotateRampY");
	ClonerMultiThread::addAttribute(aRotateRampY);

	ClonerMultiThread::aRotateRampZ = rAttr.createCurveRamp("rotateRampZ", "rotateRampZ");
	ClonerMultiThread::addAttribute(aRotateRampZ);

	ClonerMultiThread::aScaleRampX = rAttr.createCurveRamp("scaleRampX", "scaleRampX");
	ClonerMultiThread::addAttribute(aScaleRampX);

	ClonerMultiThread::aScaleRampY = rAttr.createCurveRamp("scaleRampY", "scaleRampY");
	ClonerMultiThread::addAttribute(aScaleRampY);

	ClonerMultiThread::aScaleRampZ = rAttr.createCurveRamp("scaleRampZ", "scaleRampZ");
	ClonerMultiThread::addAttribute(aScaleRampZ);


	// Attribute affects
	// Output mesh


	attributeAffects(ClonerMultiThread::aGridOffsetRampX, ClonerMultiThread::aOutMesh);
	attributeAffects(ClonerMultiThread::aGridOffsetRampY, ClonerMultiThread::aOutMesh);
	attributeAffects(ClonerMultiThread::aGridOffsetRampZ, ClonerMultiThread::aOutMesh);

	attributeAffects(ClonerMultiThread::aRotateRampX, ClonerMultiThread::aOutMesh);
	attributeAffects(ClonerMultiThread::aRotateRampY, ClonerMultiThread::aOutMesh);
	attributeAffects(ClonerMultiThread::aRotateRampZ, ClonerMultiThread::aOutMesh);

	attributeAffects(ClonerMultiThread::aScaleRampX, ClonerMultiThread::aOutMesh);
	attributeAffects(ClonerMultiThread::aScaleRampY, ClonerMultiThread::aOutMesh);
	attributeAffects(ClonerMultiThread::aScaleRampZ, ClonerMultiThread::aOutMesh);

	attributeAffects(ClonerMultiThread::aInMesh, ClonerMultiThread::aOutMesh);
	attributeAffects(ClonerMultiThread::aRefMesh, ClonerMultiThread::aOutMesh);
	attributeAffects(ClonerMultiThread::aRefMeshSmooth, ClonerMultiThread::aOutMesh);
	attributeAffects(ClonerMultiThread::aInCurve, ClonerMultiThread::aOutMesh);

	attributeAffects(ClonerMultiThread::aSmoothMeshPreview, ClonerMultiThread::aOutMesh);

	attributeAffects(ClonerMultiThread::aInLocAPos, ClonerMultiThread::aOutMesh);
	attributeAffects(ClonerMultiThread::aInLocBPos, ClonerMultiThread::aOutMesh);
	attributeAffects(ClonerMultiThread::aRefLocPos, ClonerMultiThread::aOutMesh);


	attributeAffects(ClonerMultiThread::aInstanceType, ClonerMultiThread::aOutMesh);
	attributeAffects(ClonerMultiThread::aPatterType, ClonerMultiThread::aOutMesh);
	attributeAffects(ClonerMultiThread::aScatterType, ClonerMultiThread::aOutMesh);
	attributeAffects(ClonerMultiThread::aOrientationType, ClonerMultiThread::aOutMesh);

	attributeAffects(ClonerMultiThread::aIDType, ClonerMultiThread::aOutMesh);

	attributeAffects(ClonerMultiThread::aFirstUpVec, ClonerMultiThread::aOutMesh);


	attributeAffects(ClonerMultiThread::aLimitDisplay, ClonerMultiThread::aOutMesh);

	attributeAffects(ClonerMultiThread::aOrientCurveToRefGeo, ClonerMultiThread::aOutMesh);

	attributeAffects(ClonerMultiThread::aGridInstanceX, ClonerMultiThread::aOutMesh);
	attributeAffects(ClonerMultiThread::aGridInstanceY, ClonerMultiThread::aOutMesh);
	attributeAffects(ClonerMultiThread::aGridInstanceZ, ClonerMultiThread::aOutMesh);
	attributeAffects(ClonerMultiThread::aRotateX, ClonerMultiThread::aOutMesh);
	attributeAffects(ClonerMultiThread::aRotateY, ClonerMultiThread::aOutMesh);
	attributeAffects(ClonerMultiThread::aRotateZ, ClonerMultiThread::aOutMesh);

	attributeAffects(ClonerMultiThread::aCircleRotate, ClonerMultiThread::aOutMesh);

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
	attributeAffects(ClonerMultiThread::aDisplayEdges, ClonerMultiThread::aOutMesh);

	attributeAffects(ClonerMultiThread::aConnectPieces, ClonerMultiThread::aOutMesh);
	attributeAffects(ClonerMultiThread::aMergePieces, ClonerMultiThread::aOutMesh);
	attributeAffects(ClonerMultiThread::aConnectLoop, ClonerMultiThread::aOutMesh);
	attributeAffects(ClonerMultiThread::aConnectArrayA, ClonerMultiThread::aOutMesh);
	attributeAffects(ClonerMultiThread::aConnectArrayB, ClonerMultiThread::aOutMesh);
	attributeAffects(ClonerMultiThread::aInterpolate, ClonerMultiThread::aOutMesh);
	attributeAffects(ClonerMultiThread::aCustomEdgeComponent, ClonerMultiThread::aOutMesh);

	attributeAffects(ClonerMultiThread::aTransformXRule, ClonerMultiThread::aOutMesh);
	attributeAffects(ClonerMultiThread::aTransformYRule, ClonerMultiThread::aOutMesh);
	attributeAffects(ClonerMultiThread::aTransformZRule, ClonerMultiThread::aOutMesh);

	attributeAffects(ClonerMultiThread::aRotateXRule, ClonerMultiThread::aOutMesh);
	attributeAffects(ClonerMultiThread::aRotateYRule, ClonerMultiThread::aOutMesh);
	attributeAffects(ClonerMultiThread::aRotateZRule, ClonerMultiThread::aOutMesh);

	attributeAffects(ClonerMultiThread::aScaleXRule, ClonerMultiThread::aOutMesh);
	attributeAffects(ClonerMultiThread::aScaleYRule, ClonerMultiThread::aOutMesh);
	attributeAffects(ClonerMultiThread::aScaleZRule, ClonerMultiThread::aOutMesh);

	attributeAffects(ClonerMultiThread::aShowRoot, ClonerMultiThread::aOutMesh);

	attributeAffects(ClonerMultiThread::aWrapToSurface, ClonerMultiThread::aOutMesh);


#if MAYA_API_VERSION > 201600
	// Output Matrix array
	attributeAffects(ClonerMultiThread::aInMesh, ClonerMultiThread::aOutMatrixArray);
	attributeAffects(ClonerMultiThread::aRefMesh, ClonerMultiThread::aOutMatrixArray);
	attributeAffects(ClonerMultiThread::aRefMeshSmooth, ClonerMultiThread::aOutMatrixArray);
	attributeAffects(ClonerMultiThread::aInCurve, ClonerMultiThread::aOutMatrixArray);
	attributeAffects(ClonerMultiThread::aSmoothMeshPreview, ClonerMultiThread::aOutMatrixArray);
	attributeAffects(ClonerMultiThread::aInLocAPos, ClonerMultiThread::aOutMatrixArray);
	attributeAffects(ClonerMultiThread::aInLocBPos, ClonerMultiThread::aOutMatrixArray);
	attributeAffects(ClonerMultiThread::aRefLocPos, ClonerMultiThread::aOutMatrixArray);
	attributeAffects(ClonerMultiThread::aInstanceType, ClonerMultiThread::aOutMatrixArray);
	attributeAffects(ClonerMultiThread::aPatterType, ClonerMultiThread::aOutMatrixArray);
	attributeAffects(ClonerMultiThread::aScatterType, ClonerMultiThread::aOutMatrixArray);
	attributeAffects(ClonerMultiThread::aOrientationType, ClonerMultiThread::aOutMatrixArray);
	attributeAffects(ClonerMultiThread::aIDType, ClonerMultiThread::aOutMatrixArray);
	attributeAffects(ClonerMultiThread::aFirstUpVec, ClonerMultiThread::aOutMatrixArray);
	attributeAffects(ClonerMultiThread::aLimitDisplay, ClonerMultiThread::aOutMatrixArray);
	attributeAffects(ClonerMultiThread::aOrientCurveToRefGeo, ClonerMultiThread::aOutMatrixArray);
	attributeAffects(ClonerMultiThread::aGridInstanceX, ClonerMultiThread::aOutMatrixArray);
	attributeAffects(ClonerMultiThread::aGridInstanceY, ClonerMultiThread::aOutMatrixArray);
	attributeAffects(ClonerMultiThread::aGridInstanceZ, ClonerMultiThread::aOutMatrixArray);
	attributeAffects(ClonerMultiThread::aRotateX, ClonerMultiThread::aOutMatrixArray);
	attributeAffects(ClonerMultiThread::aRotateY, ClonerMultiThread::aOutMatrixArray);
	attributeAffects(ClonerMultiThread::aRotateZ, ClonerMultiThread::aOutMatrixArray);
	attributeAffects(ClonerMultiThread::aCircleRotate, ClonerMultiThread::aOutMatrixArray);
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
	attributeAffects(ClonerMultiThread::aDisplayEdges, ClonerMultiThread::aOutMatrixArray);

	attributeAffects(ClonerMultiThread::aConnectPieces, ClonerMultiThread::aOutMatrixArray);
	attributeAffects(ClonerMultiThread::aMergePieces, ClonerMultiThread::aOutMatrixArray);
	attributeAffects(ClonerMultiThread::aConnectLoop, ClonerMultiThread::aOutMatrixArray);
	attributeAffects(ClonerMultiThread::aConnectArrayA, ClonerMultiThread::aOutMatrixArray);
	attributeAffects(ClonerMultiThread::aConnectArrayB, ClonerMultiThread::aOutMatrixArray);
	attributeAffects(ClonerMultiThread::aInterpolate, ClonerMultiThread::aOutMatrixArray);
	attributeAffects(ClonerMultiThread::aCustomEdgeComponent, ClonerMultiThread::aOutMatrixArray);

	attributeAffects(ClonerMultiThread::aTransformXRule, ClonerMultiThread::aOutMatrixArray);
	attributeAffects(ClonerMultiThread::aTransformYRule, ClonerMultiThread::aOutMatrixArray);
	attributeAffects(ClonerMultiThread::aTransformZRule, ClonerMultiThread::aOutMatrixArray);

	attributeAffects(ClonerMultiThread::aRotateXRule, ClonerMultiThread::aOutMatrixArray);
	attributeAffects(ClonerMultiThread::aRotateYRule, ClonerMultiThread::aOutMatrixArray);
	attributeAffects(ClonerMultiThread::aRotateZRule, ClonerMultiThread::aOutMatrixArray);

	attributeAffects(ClonerMultiThread::aScaleXRule, ClonerMultiThread::aOutMatrixArray);
	attributeAffects(ClonerMultiThread::aScaleYRule, ClonerMultiThread::aOutMatrixArray);
	attributeAffects(ClonerMultiThread::aScaleZRule, ClonerMultiThread::aOutMatrixArray);

	attributeAffects(ClonerMultiThread::aShowRoot, ClonerMultiThread::aOutMatrixArray);
	attributeAffects(ClonerMultiThread::aWrapToSurface, ClonerMultiThread::aOutMatrixArray);

#endif

	// Output ID array
	attributeAffects(ClonerMultiThread::aInMesh, ClonerMultiThread::aOutIDArray);
	attributeAffects(ClonerMultiThread::aRefMesh, ClonerMultiThread::aOutIDArray);
	attributeAffects(ClonerMultiThread::aRefMeshSmooth, ClonerMultiThread::aOutIDArray);
	attributeAffects(ClonerMultiThread::aSmoothMeshPreview, ClonerMultiThread::aOutIDArray);
	attributeAffects(ClonerMultiThread::aInCurve, ClonerMultiThread::aOutIDArray);
	attributeAffects(ClonerMultiThread::aInLocAPos, ClonerMultiThread::aOutIDArray);
	attributeAffects(ClonerMultiThread::aInLocBPos, ClonerMultiThread::aOutIDArray);
	attributeAffects(ClonerMultiThread::aRefLocPos, ClonerMultiThread::aOutIDArray);
	attributeAffects(ClonerMultiThread::aInstanceType, ClonerMultiThread::aOutIDArray);
	attributeAffects(ClonerMultiThread::aPatterType, ClonerMultiThread::aOutIDArray);
	attributeAffects(ClonerMultiThread::aScatterType, ClonerMultiThread::aOutIDArray);
	attributeAffects(ClonerMultiThread::aOrientationType, ClonerMultiThread::aOutIDArray);
	attributeAffects(ClonerMultiThread::aIDType, ClonerMultiThread::aOutIDArray);
	attributeAffects(ClonerMultiThread::aFirstUpVec, ClonerMultiThread::aOutIDArray);
	attributeAffects(ClonerMultiThread::aLimitDisplay, ClonerMultiThread::aOutIDArray);
	attributeAffects(ClonerMultiThread::aOrientCurveToRefGeo, ClonerMultiThread::aOutIDArray);
	attributeAffects(ClonerMultiThread::aGridInstanceX, ClonerMultiThread::aOutIDArray);
	attributeAffects(ClonerMultiThread::aGridInstanceY, ClonerMultiThread::aOutIDArray);
	attributeAffects(ClonerMultiThread::aGridInstanceZ, ClonerMultiThread::aOutIDArray);
	attributeAffects(ClonerMultiThread::aRotateX, ClonerMultiThread::aOutIDArray);
	attributeAffects(ClonerMultiThread::aRotateY, ClonerMultiThread::aOutIDArray);
	attributeAffects(ClonerMultiThread::aRotateZ, ClonerMultiThread::aOutIDArray);
	attributeAffects(ClonerMultiThread::aCircleRotate, ClonerMultiThread::aOutIDArray);
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
	attributeAffects(ClonerMultiThread::aMergePieces, ClonerMultiThread::aOutIDArray);
	attributeAffects(ClonerMultiThread::aConnectLoop, ClonerMultiThread::aOutIDArray);
	attributeAffects(ClonerMultiThread::aConnectArrayA, ClonerMultiThread::aOutIDArray);
	attributeAffects(ClonerMultiThread::aConnectArrayB, ClonerMultiThread::aOutIDArray);
	attributeAffects(ClonerMultiThread::aInterpolate, ClonerMultiThread::aOutIDArray);
	attributeAffects(ClonerMultiThread::aCustomEdgeComponent, ClonerMultiThread::aOutIDArray);

	attributeAffects(ClonerMultiThread::aTransformXRule, ClonerMultiThread::aOutIDArray);
	attributeAffects(ClonerMultiThread::aTransformYRule, ClonerMultiThread::aOutIDArray);
	attributeAffects(ClonerMultiThread::aTransformZRule, ClonerMultiThread::aOutIDArray);

	attributeAffects(ClonerMultiThread::aRotateXRule, ClonerMultiThread::aOutIDArray);
	attributeAffects(ClonerMultiThread::aRotateYRule, ClonerMultiThread::aOutIDArray);
	attributeAffects(ClonerMultiThread::aRotateZRule, ClonerMultiThread::aOutIDArray);

	attributeAffects(ClonerMultiThread::aScaleXRule, ClonerMultiThread::aOutIDArray);
	attributeAffects(ClonerMultiThread::aScaleYRule, ClonerMultiThread::aOutIDArray);
	attributeAffects(ClonerMultiThread::aScaleZRule, ClonerMultiThread::aOutIDArray);

	attributeAffects(ClonerMultiThread::aShowRoot, ClonerMultiThread::aOutIDArray);
	attributeAffects(ClonerMultiThread::aWrapToSurface, ClonerMultiThread::aOutIDArray);

	return MS::kSuccess;
}


