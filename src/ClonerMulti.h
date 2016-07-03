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
#include <maya/MFnMatrixArrayData.h>
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

#include <maya/MDagModifier.h>

#include <vector>
#include <iostream>


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



	// Id
	static MTypeId				id;

	// Input output
	static MObject				aOutMesh;
	static MObject				aInMesh;
	static MObject              aInCurve;
	static MObject              aRefMesh;
	static MObject				aInLocAPos;
	static MObject				aInLocBPos;

	// Types
	static MObject				aInstanceType;
	static MObject				aIDType;
	static MObject				aPatterType;
	static MObject				aScatterType;

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

	// Upvector
	static MObject              aFirstUpVec;
	static MObject              aFirstUpVecX;
	static MObject              aFirstUpVecY;
	static MObject              aFirstUpVecZ;

	// Output string for baking
	static MObject				aOutMatrixArray;
	static MObject				aOutIDArray;

private:

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
	MDataHandle					h_outputMatrix;
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


	// ID's
	MIntArray					m_idA;
	MIntArray					m_manualIDA;

	// Random
	int							m_rand_seed;

	// Function sliders
	short						m_instanceType;
	short						m_patternType;
	short						m_scatterType;
	int							m_id;


	// Number of instances
	int                         m_instanceX;
	int                         m_instanceY;
	int                         m_instanceZ;


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

	// Matrix
	MMatrixArray				m_tr_matA;

	MMatrix						m_inLocA_posMat;
	MMatrix						m_inLocB_posMat;

	// Upvector
	MVector                     m_firstUpVec;




	MMatrix						m_refMeshMat;

	// Mesh instance variables

	MPointArray					mesh_pA;
	MVectorArray				mesh_uTA;
	MVectorArray				mesh_vTA;
	MVectorArray				mesh_nA;

	

};



#endif