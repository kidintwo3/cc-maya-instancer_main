//
//  ClonerMulti.h
//  ClonerMultiThread
//
//  Created by Hunyadi Janos on 31/01/15.
//  Copyright (c) 2015 Janos Hunyadi. All rights reserved.
//

#ifndef CLONERMULTI_H
#define CLONERMULTI_H

#include <maya/MPxNode.h>
#include <maya/MPxTransform.h>

#include <maya/MPointArray.h>
#include <maya/MFloatPointArray.h>
#include <maya/MVectorArray.h>
#include <maya/MVector.h>
#include <maya/MFnMesh.h>
#include <maya/MItMeshEdge.h>
#include <maya/MMatrix.h>
#include <maya/MMatrixArray.h>
#include <maya/MEulerRotation.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MFnCompoundAttribute.h>
#include <maya/MFnMeshData.h>
#include <maya/MFnIntArrayData.h>
#include <maya/MFnPointArrayData.h>
#include <maya/MFnVectorArrayData.h>
#include <maya/MFloatVectorArray.h>
#include <maya/MItMeshPolygon.h>
#include <maya/MItMeshVertex.h>
#include <maya/MBoundingBox.h>
#include <maya/MGlobal.h>
#include <maya/MPlugArray.h>
#include <maya/MDagPath.h>
#include <maya/MSelectionList.h>
#include <maya/MFnSet.h>
#include <maya/MFnMeshData.h>
#include <maya/MFnNurbsCurve.h>
#include <maya/MTransformationMatrix.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MFnDagNode.h>
#include <maya/MArrayDataBuilder.h>
#include <maya/MArrayDataHandle.h>
#include <maya/MObjectArray.h>
#include <maya/MPlugArray.h>
#include <maya/MQuaternion.h>
#include <maya/MFnStringData.h>
#include <maya/MStringArray.h>
#include <maya/MFnMatrixData.h>
#include <maya/MFnTransform.h>
#include <maya/MFnSingleIndexedComponent.h>
#include <maya/MMeshIntersector.h>
#include <maya/MPxLocatorNode.h>
#include <maya/MRampAttribute.h>
#include <maya/MAnimControl.h>


#if MAYA_API_VERSION > 201600
#include <maya/MFnMatrixArrayData.h>
#endif

#include <maya/MDagModifier.h>

#include <maya/MModelMessage.h>
#include <maya/MNodeMessage.h>
#include <maya/MDGMessage.h>

#include <maya/MCallbackIdArray.h>

// Viewport 2.0 includes
#include <maya/MDrawRegistry.h>
#include <maya/MPxDrawOverride.h>
#include <maya/MUserData.h>
#include <maya/MDrawContext.h>
#include <maya/MHWGeometryUtilities.h>


#include <vector>
#include <iostream>
#include <math.h>
#include <algorithm>
#include <fstream>

using namespace std;




class ClonerMultiThread : public MPxLocatorNode
{

public:

	ClonerMultiThread();
	virtual						~ClonerMultiThread();
	static void*				creator();
	virtual void				postConstructor(); 

	virtual MStatus				compute(const MPlug& plug, MDataBlock& data);
	static  MStatus				initialize();
    
	MFloatArray					storeProfileCurveData(MRampAttribute a_segmentsAttribute, int segments);

    virtual void				draw( M3dView & view, const MDagPath & path, M3dView::DisplayStyle style, M3dView::DisplayStatus status );

	// Vp 2.0 stuff
	static	MString				drawDbClassification;
	static	MString				drawRegistrantId;

	// Callbacks

	static void					aboutToDeleteCB( MObject& node, MDGModifier& modifier, void* clientData );

	// Id
	static MTypeId				id;

	// Input output
	static MObject				aOutMesh;
	static MObject				aInMesh;
	static MObject              aInCurve;
	static MObject              aRefMesh;
	static MObject				aInLocAPos;
	static MObject				aInLocBPos;
	static MObject				aRefLocPos;
	

	// Display
	static MObject				aLimitDisplay;
	static MObject				aShowRoot;

	// Types
	static MObject				aInstanceType;
	static MObject				aIDType;
	static MObject				aPatterType;
	static MObject				aScatterType;
	static MObject				aOrientationType;

	// Transforms
	static MObject				aGridInstanceX;
	static MObject				aGridInstanceY;
	static MObject				aGridInstanceZ;
	static MObject				aGridInstance;

	static MObject				aGridOffsetX;
	static MObject				aGridOffsetY;
	static MObject				aGridOffsetZ;

	static MObject				aRotateX;
	static MObject				aRotateY;
	static MObject				aRotateZ;


	static MObject				aTransformXRule;
	static MObject				aTransformYRule;
	static MObject				aTransformZRule;

	static MObject				aRotateXRule;
	static MObject				aRotateYRule;
	static MObject				aRotateZRule;

	static MObject				aScaleXRule;
	static MObject				aScaleYRule;
	static MObject				aScaleZRule;


	static MObject				aScaleX;
	static MObject				aScaleY;
	static MObject				aScaleZ;

	// Random
	static MObject				aRndOffsetX;
	static MObject				aRndOffsetY;
	static MObject				aRndOffsetZ;
	static MObject				aRndRotateX;
	static MObject				aRndRotateY;
	static MObject				aRndRotateZ;
	static MObject				aRndScaleX;
	static MObject				aRndScaleY;
	static MObject				aRndScaleZ;
	static MObject				aSeedVal;
	static MObject				aRandIDLev;

	// Ramps
	static MObject				aGridOffsetRamp;
	static MObject				aRotateRamp;
	static MObject				aScaleRamp;

	// UV
	static MObject				aUVOffsetU;
	static MObject				aUVOffsetV;
	static MObject				aRndUVOffsetU;
	static MObject				aRndUVOffsetV;

	// Methods
	static MObject				aReverseNormals;
	static MObject				aSmoothNormals;
	static MObject				aMergeInputMeshes;
	static MObject				aWorldSpace;
	static MObject				aRevPattern;
	static MObject				aLoopOffset;
	static MObject				aUvUDIMLoop;
	static MObject				aOutputMeshDisplayOverride;
	static MObject				aDisplayProxy;

	// Upvector
	static MObject              aFirstUpVec;
	static MObject              aFirstUpVecX;
	static MObject              aFirstUpVecY;
	static MObject              aFirstUpVecZ;

	static MObject				aConnectPieces;
	static MObject				aConnectLoop;
	static MObject				aConnectArrayA;
	static MObject				aConnectArrayB;
	static MObject				aInterpolate;

	static MObject				aOrientCurveToRefGeo;

	// Output string for baking
	static MObject				aOutMatrixArray;
	static MObject				aOutIDArray;

private:

	// Callbacks
	static void					preDuplicateCB(void* data);
	static void					postDuplicateCB(void* data);
	static void					nodeAddedCB(MObject& node, void* clientData);

	// Rules
	MStatus						storeRules();

	// Sanity check
	MStatus						checkInputPlugs();

	// Modules Main
	MStatus						collectInputMeshes(MDataBlock& data);
	MStatus						collectPlugs(MDataBlock& data);

	// Modules CloneIDs
	MIntArray					generatePatternIDs(short& patternType, int& m_numDup);
	MFloatArray					generateRndArray(float& surfaceNoise, int& numValues, int& seedVal);

	MStatus						mergeInputMeshes();
	MStatus						duplicateInputMeshes(MIntArray& idA);

	MStatus						generateBBMeshes(MIntArray& idA);

	MStatus						overrideInstanceOnMeshSettings();

	MStatus                     reverseNormals();

	MStatus						mergeUVs();
	MStatus						duplicateUVs(MIntArray& idA);

	MStatus						smoothNormals(MFnMesh &meshFn);
	MStatus						displayOverride();

	// Instance types
	MStatus						instanceGrid();
	MStatus                     instanceAtoB();
	MStatus                     instanceSpline();
	MStatus                     instanceCircle();
	MStatus                     instanceFibonacciSphere();
	MStatus                     instanceOnMesh();


	// Datahandles
	MDataHandle					h_outputMesh;
	#if MAYA_API_VERSION > 201600
	MDataHandle					h_outputMatrix;
#endif
	MDataHandle					h_outputID;
	

	// Input data arrays
	MObjectArray				m_inMeshArray;
	MMatrixArray				m_inMeshMatrixArray;


	// Reference mesh
	MObject						m_refMesh;

	// Plugs
	MPlug						p_inMesh;
	MPlug						p_outMesh;
	MPlug						p_inCurve;
	MPlug						p_refMesh;
	MPlug						p_inLocA;
	MPlug						p_inLocB;
	MPlug						p_refLoc;

	// Curve
	MObject						m_inCurve;
	MMatrix						m_curveTrMat;

	// Mesh generation attributes
	int							m_numInputMeshes;

	vector<int>					i_numVertices;
	int                         o_numVertices;

	vector<int>					i_numPolygons;
	int                         o_numPolygons;

	vector<MFloatPointArray>	i_vertexArray;
	MFloatPointArray		    o_vertexArray;

	vector<MIntArray>			i_polygonCounts;
	MIntArray					o_polygonCounts;

	vector<MIntArray>			i_polygonConnects;
	MIntArray					o_polygonConnects;

	vector<MVectorArray>		i_faceVertexNormals;
	MVectorArray				o_faceVertexNormals;

	bool						m_disableBaseMeshOverride;

	// UV's
	MFloatArray					o_uArrayA;
	MFloatArray					o_vArrayA;

	MFloatArray					n_uArrayA;
	MFloatArray					n_vArrayA;

	MIntArray					o_uvCountsA;
	MIntArray					o_uvIdsA;

	MString						o_defaultUVSetNameA;

	// Rules

	MFloatArray					m_rule_off_A_X;
	MFloatArray					m_rule_off_A_Y;
	MFloatArray					m_rule_off_A_Z;

	MFloatArray					m_rule_rot_A_X;
	MFloatArray					m_rule_rot_A_Y;
	MFloatArray					m_rule_rot_A_Z;

	MFloatArray					m_rule_scl_A_X;
	MFloatArray					m_rule_scl_A_Y;
	MFloatArray					m_rule_scl_A_Z;

	// ID's
	MIntArray					m_idA;
	MIntArray					m_manualIDA;

	// Random
	int							m_rand_seed;

	// Function sliders
	short						m_instanceType;
	short						m_patternType;
	short						m_scatterType;
	bool						m_orientationType;
	int							m_id;

	// Display
	int							m_polyLimit;

	// Number of instances
	int                         m_instanceX;
	int                         m_instanceY;
	int                         m_instanceZ;

	// Ramps

	MFloatArray					m_offsetProfileA;
	MFloatArray					m_rotateProfileA;
	MFloatArray					m_scaleProfileA;

	// Transforms
	float					    m_offsetX;
	float                       m_offsetY;
	float                       m_offsetZ;
	float                       m_rotateX;
	float                       m_rotateY;
	float                       m_rotateZ;
	float                       m_scaleX;
	float                       m_scaleY;
	float                       m_scaleZ;


	// Random
	float						m_randomOffsetX;
	float                       m_randomOffsetY;
	float                       m_randomOffsetZ;
	float                       m_randomRotateX;
	float                       m_randomRotateY;
	float                       m_randomRotateZ;
	float                       m_randomScaleX;
	float                       m_randomScaleY;
	float                       m_randomScaleZ;

	// UV
	float						m_uvOffsetU;
	float						m_uvOffsetV;
	float					    m_rndUvOffsetU;
	float						m_rndUvOffsetV;

	MFloatArray                 m_rndOffsetXA;
	MFloatArray                 m_rndOffsetYA;
	MFloatArray                 m_rndOffsetZA;
	MFloatArray                 m_rndRotateXA;
	MFloatArray                 m_rndRotateYA;
	MFloatArray                 m_rndRotateZA;
	MFloatArray                 m_rndScaleXA;
	MFloatArray                 m_rndScaleYA;
	MFloatArray                 m_rndScaleZA;
	MFloatArray                 m_rndOffsetUA;
	MFloatArray                 m_rndOffsetVA;

	float						m_rndIDLev;

	// Function switches
	int							m_numDup;

	bool						m_reverseNormals;
	bool						m_smoothNorm;
	bool						m_mergeInputMeshes;
	bool						m_worldSpace;
	bool						m_revPattern;
	bool						m_loopOffset;
	bool						m_uvUDIMLoop;
	bool						m_outputMeshDisplayOverride;
	bool						m_displayProxy;
	bool						m_connectPieces;
	bool						m_connectLoop;
	bool						m_interpolate;
	bool						m_orientCurveToRefGeo;
	bool						m_showRoot;

	// Matrix
	MMatrixArray				m_tr_matA;

	MMatrix						m_inLocA_posMat;
	MMatrix						m_inLocB_posMat;
	MMatrix						m_refLoc_posMat;

	// Upvector
	MVector                     m_firstUpVec;

	// Connection strings
	MIntArray					m_ConnectArrayA;
	MIntArray					m_ConnectArrayB;


	MMatrix						m_refMeshMat;

	// Mesh instance variables
	MPointArray					mesh_pA;
	MVectorArray				mesh_uTA;
	MVectorArray				mesh_vTA;
	MVectorArray				mesh_nA;

	// Callback
	MCallbackIdArray			m_callbackIDs;

	MString						m_transformXRule;
	MString						m_transformYRule;
	MString						m_transformZRule;

	MString						m_rotateXRule;
	MString						m_rotateYRule;
	MString						m_rotateZRule;

	MString						m_scaleXRule;
	MString						m_scaleYRule;
	MString						m_scaleZRule;

};


//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// Viewport 2.0 override implementation
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------


// data for override

class ClonerMultiThreadData : public MUserData
{
public:
	ClonerMultiThreadData() : MUserData(false) {} // don't delete after draw
	virtual ~ClonerMultiThreadData() {}


	MMatrix					m_inLoc_mat;

	MColor					m_locColor;
	MPoint					m_controllerPos;

	bool					m_displayProxy;
	bool					m_showRoot;

	MPointArray				m_dispPointA;

};


// override

class ClonerMultiThreadOverride : public MHWRender::MPxDrawOverride
{
public:
	static MHWRender::MPxDrawOverride* Creator(const MObject& obj)
	{
		return new ClonerMultiThreadOverride(obj);
	}

	virtual ~ClonerMultiThreadOverride();
	virtual MHWRender::DrawAPI supportedDrawAPIs() const;

	virtual bool isBounded( const MDagPath& objPath, const MDagPath& cameraPath) const;
	virtual MBoundingBox boundingBox( const MDagPath& objPath, const MDagPath& cameraPath) const;

	virtual MUserData* prepareForDraw( const MDagPath& objPath, const MDagPath& cameraPath, const MHWRender::MFrameContext& frameContext, MUserData* oldData);

	virtual bool hasUIDrawables() const { return true; }
	virtual void addUIDrawables( const MDagPath& objPath, MHWRender::MUIDrawManager& drawManager, const MHWRender::MFrameContext& frameContext, const MUserData* data);

	static void draw(const MHWRender::MDrawContext& context, const MUserData* data) {};

private:
	ClonerMultiThreadOverride(const MObject& obj);

	MPointArray getInstancePoints(const MDagPath& objPath) const;

};




#endif