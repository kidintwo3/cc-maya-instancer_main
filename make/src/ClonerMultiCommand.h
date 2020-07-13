#ifndef CLONERMULTICOMMAND_H
#define CLONERMULTICOMMAND_H


#ifdef __linux__ 
	#include <maya/MArgDatabase.h>
#else
	#include <maya/MArgDataBase.h>
#endif

#include <maya/MDagPath.h>
#include <maya/MDGModifier.h>
#include <maya/MFnDagNode.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MFnIntArrayData.h>
#if MAYA_API_VERSION > 201600
#include <maya/MFnMatrixArrayData.h>
#endif
#include <maya/MFnIntArrayData.h>
#include <maya/MMatrixArray.h>
#include <maya/MTransformationMatrix.h>
#include <maya/MFnMesh.h>
#include <maya/MGlobal.h>
#include <maya/MIntArray.h>
#include <maya/MItDependencyGraph.h>
#include <maya/MMeshIntersector.h>
#include <maya/MObject.h>
#include <maya/MPlug.h>
#include <maya/MPlugArray.h>
#include <maya/MPointArray.h>
#include <maya/MPxCommand.h>
#include <maya/MSelectionList.h>
#include <maya/MSyntax.h>
#include <maya/MDagModifier.h>
#include <maya/MFnSet.h>
#include <maya/MTransformationMatrix.h>
#include <maya/MFnTransform.h>
#include <maya/MDagPathArray.h>
#include <maya/MItMeshEdge.h>
#include <maya/MFnComponentListData.h>
#include <maya/MFnSingleIndexedComponent.h>
#include <maya/MItMeshVertex.h>
#include <maya/MRampAttribute.h>
#include <maya/MFnIntArrayData.h>
#include <maya/MArgList.h>

class ClonerMultiCommand : public MPxCommand
{
public:
    ClonerMultiCommand();
    virtual MStatus doIt( const MArgList& argList );
    virtual MStatus redoIt();
    virtual MStatus undoIt();
    virtual bool isUndoable() const;
    static void* creator();
    static MSyntax newSyntax();

	void setResult();

private:

	MDagModifier		m_DAGMod;



	MDagPathArray		p_currSelTrA;
	MDagPathArray		p_currSelShapeA;
	MDagPathArray		p_currSelTrA_curves;
	MDagPathArray		p_currSelTrA_locators;
	MDagPathArray		p_currSelShapeA_curves;
	MDagPathArray		p_currSelShapeA_locators;

	MObject				o_clonerMultiNode;

	MObject				o_locA;
	MObject				o_locB;

	MObject				o_clonerOutputMesh;

	MObjectArray		o_outputObjectsA;
	

};


#endif