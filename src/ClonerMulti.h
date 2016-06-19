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
#include <maya/MFnPointArrayData.h>
#include <maya/MFnVectorArrayData.h>
#include <maya/MFloatVectorArray.h>
#include <maya/MItMeshPolygon.h>
#include <maya/MBoundingBox.h>
#include <maya/MGlobal.h>
#include <maya/MPlugArray.h>
#include <maya/MDagPath.h>
#include <maya/MSelectionList.h>
#include <maya/MFnSet.h>
#include <maya/MFnMeshData.h>
#include <maya/MFnNurbsCurve.h>
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

#include <maya/MPxManipContainer.h>

#include <maya/MDagModifier.h>

#include <vector>
#include <iostream>



using namespace std;


class ClonerMultiThread : public MPxNode
{

public:

	ClonerMultiThread();
	virtual						~ClonerMultiThread();

	static void*				creator();
	virtual MStatus				compute(const MPlug& plug, MDataBlock& data);
	static  MStatus				initialize();

	virtual void 				postConstructor();

	// Id
	static MTypeId				id;

	// Input output
	static MObject				aOutMesh;
	static MObject				aInMesh;
    static MObject              aInCurve;
    static MObject              aRefMesh;

	// Types
	static MObject				aInstanceType;
	static MObject				aIDType;
	static MObject				aPatterType;

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

	// Methods
	static MObject				aReverseNormals;
	static MObject				aSmoothNormals;
	static MObject				aMergeInputMeshes;
	static MObject				aWorldSpace;
	static MObject				aRevPattern;
    
    // Upvector
    static MObject              aFirstUpVec;
    static MObject              aFirstUpVecX;
    static MObject              aFirstUpVecY;
    static MObject              aFirstUpVecZ;


private:

	// Modules Main
	MStatus						collectInputMeshes(MDataBlock& data);
	MStatus						collectPlugs(MDataBlock& data);

	// Modules CloneIDs
	MIntArray					generatePatternIDs(int& patternType, int& m_numDup);
	MDoubleArray				generateRndArray(double& surfaceNoise, int& numValues, int& seedVal);;

	MStatus						mergeInputMeshes();
	MStatus						duplicateInputMeshes(MIntArray& idA);
    
    MStatus                     reverseNormals();

	MStatus						generateUVs();
	MStatus						duplicateUVs();

	MStatus						smoothNormals(MFnMesh &meshFn);

	// Instance types
	MStatus						instanceGrid();
    MStatus                     instanceSpline();
    MStatus                     instanceCircle();


	// Datahandles
	MDataHandle					h_outputMesh;

	// Input data arrays
	MObjectArray				m_inMeshArray;
	MMatrixArray				m_inMeshMatrixArray;

    
    // Reference mesh
    MObject						m_refMesh;

	// Plugs
	MPlug						p_inMesh;
	MPlug						p_outMesh;
    
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

	// Function sliders
	int							m_instanceType;
	int							m_patternType;
	int							m_id;

	int                         m_instanceX;
	int                         m_instanceY;
	int                         m_instanceZ;

	float					    m_offsetX;
	float                       m_offsetY;
	float                       m_offsetZ;
	float                       m_rotateX;
	float                       m_rotateY;
	float                       m_rotateZ;
	float                       m_scaleX;
	float                       m_scaleY;
	float                       m_scaleZ;

	// Function switches
	int							m_numDup;

	bool						m_reverseNormals;
	bool						m_smoothNorm;
	bool						m_mergeInputMeshes;
	bool						m_worldSpace;
	bool						m_revPattern;



	// Matrix
	MMatrixArray				m_tr_matA;
    
    // Upvector
    MVector                     m_firstUpVec;

};



#endif