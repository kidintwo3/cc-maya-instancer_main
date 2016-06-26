#ifndef CLONERMULTICOMMAND_H
#define CLONERMULTICOMMAND_H

#include <maya/MArgDataBase.h>
#include <maya/MDagPath.h>
#include <maya/MDGModifier.h>
#include <maya/MFnDagNode.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MFnIntArrayData.h>
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

	MDagModifier m_DAGMod;
	MDGModifier m_DGMod;
	MFnDependencyNode m_DEPNode;

	//MObject o_newMesh;
	MDagPathArray p_currSelTrA;
	MDagPathArray p_currSelShapeA;
	MDagPathArray p_currSelTrA_curves;
	MDagPathArray p_currSelShapeA_curves;

	MObject o_clonerMultiNode;

	MObject o_locA;
	MObject o_locB;

	MObject o_clonerOutputMesh;

};


#endif