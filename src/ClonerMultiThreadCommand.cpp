#include "ClonerMultiThreadCommand.h"
#include "DagUtils.h"

ClonerMultiThreadCommand::ClonerMultiThreadCommand()
{
}


void* ClonerMultiThreadCommand::creator()
{
	return new ClonerMultiThreadCommand;
}


bool ClonerMultiThreadCommand::isUndoable() const
{
	return true;
}

MSyntax ClonerMultiThreadCommand::newSyntax()
{
	MSyntax syntax;

	syntax.addFlag( "-r", "-remove", MSyntax::kString );

	syntax.setObjectType( MSyntax::kSelectionList, 1, 1 );
	syntax.useSelectionAsDefault( true );

	syntax.enableEdit( false );
	syntax.enableQuery( false );

	return syntax;
}

MStatus ClonerMultiThreadCommand::createPrimGenFromCurves(MDagPathArray p_currSelTrA, MDagPathArray p_currSelShapeA)
{
	MStatus status;



	for (unsigned int i = 0; i < p_currSelTrA.length(); i++)
	{
		p_currSelShapeA[i].extendToShape();

		if (p_currSelShapeA[i].node().apiType() != MFn::kNurbsCurve)
		{
			MGlobal::displayWarning(MString() + "[ClonerMultiThread] Only select curves, / " + p_currSelShapeA[i].partialPathName() + " / is not a curve");
			return MStatus::kFailure;
		}

	}

	o_primGenNodeA.clear();
	o_outputMeshA.clear();

	for (unsigned int i = 0; i < p_currSelShapeA.length(); i++)
	{

		MObject o_primGenNode = createNodeCustom(m_DAGMod, "primitiveGenerator" );
		MObject o_outputMesh = createNodeMaya(m_DEPNode, "mesh");

		o_primGenNodeA.append(o_primGenNode);
		o_outputMeshA.append(o_outputMesh);

		connectPlug(m_DAGMod,p_currSelShapeA[i].node(),o_primGenNode,"worldSpace","inCurve");
		connectPlug(m_DAGMod,o_primGenNode,o_outputMesh,"outMesh","inMesh");

		MFnNurbsCurve mFnC(p_currSelShapeA[i]);
		MGlobal::displayInfo(MString() + mFnC.length() );

		int i_curveLength = int(mFnC.length());

		setPlugs(o_primGenNode, "segments", "50");
		setPlugs(o_primGenNode, "profilePresets", "0");
		setPlugs(o_primGenNode, "radius", "0.5");
		setPlugs(o_primGenNode, "sides", "8");

		MFnDependencyNode mfDgN(o_primGenNode);
		MPlug a_curveAttribute = mfDgN.findPlug("segmentsRamp", status);

		MRampAttribute a_Ramp(a_curveAttribute);
		a_Ramp.setValueAtIndex(1.0,0);

		assignInitialShadingGroup(o_outputMesh);


		// Overrides on output mesh

		MFnDependencyNode mfDgN_out(o_outputMesh);
		MPlug p_overrideEnabled = mfDgN_out.findPlug("overrideEnabled", false, &status);
		CHECK_MSTATUS_AND_RETURN_IT(status);
		MPlug p_overrideDisplayType = mfDgN_out.findPlug("overrideDisplayType", false, &status);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		p_overrideEnabled.setBool(true);
		p_overrideDisplayType.setInt(2);



	}


	return MStatus::kSuccess;
}

MStatus ClonerMultiThreadCommand::createPrimGenFromLocators()
{
	MStatus status;

	o_primGenNodeA.clear();
	o_outputMeshA.clear();

	MObject o_primGenNode = createNodeCustom(m_DAGMod, "primitiveGenerator" );
	MObject o_outputMesh = createNodeMaya(m_DEPNode, "mesh");
	o_locA = createNodeMaya(m_DEPNode, "PrimitiveGeneratorLoc" );
	o_locB = createNodeMaya(m_DEPNode, "PrimitiveGeneratorLoc" );

	o_primGenNodeA.append(o_primGenNode);
	o_outputMeshA.append(o_outputMesh);

	connectPlug(m_DAGMod,o_primGenNode,o_outputMesh,"outMesh","inMesh");

	connectPlug(m_DAGMod,o_locA,o_primGenNode,"worldMatrix","locatorAPos");
	connectPlug(m_DAGMod,o_locB,o_primGenNode,"worldMatrix","locatorBPos");


	MFnTransform mfnTrA(o_locA);
	MFnTransform mfnTrB(o_locB);

	mfnTrA.setTranslation(MPoint(-5.0,0.0,0.0), MSpace::kTransform);
	mfnTrB.setTranslation(MPoint(5.0,0.0,0.0), MSpace::kTransform);

	MFnDependencyNode mfDgN(o_primGenNode);
	MPlug a_curveAttribute = mfDgN.findPlug("segmentsRamp", &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);
	MRampAttribute a_Ramp(a_curveAttribute);

	// Overrides on output mesh

	MFnDependencyNode mfDgN_out(o_outputMesh);
	MPlug p_overrideEnabled = mfDgN_out.findPlug("overrideEnabled", false, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);
	MPlug p_overrideDisplayType = mfDgN_out.findPlug("overrideDisplayType", false, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	p_overrideEnabled.setBool(true);
	p_overrideDisplayType.setInt(2);




	assignInitialShadingGroup(o_outputMesh);

	return MStatus::kSuccess;
}


MStatus ClonerMultiThreadCommand::doIt( const MArgList& argList )
{
	MStatus status;

	MArgDatabase argData( syntax(), argList, &status );
	CHECK_MSTATUS_AND_RETURN_IT(status);


	// Get Selected Object


	MDagPathArray p_currSelTrA = getCurrSelectionDAGArray();
	MDagPathArray p_currSelShapeA = p_currSelTrA;

	if (p_currSelTrA.length() == 0)
	{
		MGlobal::displayInfo(MString() + "[ClonerMultiThread] Nothing selected. Creating ClonerMultiThread with Locators");
		createPrimGenFromLocators();
	}

	else
	{
		status = createPrimGenFromCurves(p_currSelTrA, p_currSelShapeA);
		if (status == MStatus::kFailure)
		{
			MGlobal::displayWarning(MString() + "[ClonerMultiThread] Selection is not a curve" );
			return::MStatus::kFailure;
		}
		MGlobal::displayInfo(MString() + "[ClonerMultiThread] Attaching ClonerMultiThread to curves" );
	}





	return redoIt();
}

MStatus ClonerMultiThreadCommand::redoIt()
{
	MStatus status;







	return MS::kSuccess;
}

MStatus ClonerMultiThreadCommand::undoIt()
{
	MStatus status;

	// Restore the initial state
	status = m_DGMod.undoIt();
	CHECK_MSTATUS_AND_RETURN_IT( status );

	status = m_DAGMod.undoIt();
	CHECK_MSTATUS_AND_RETURN_IT(status);

	for (unsigned int i = 0; i < o_primGenNodeA.length(); i++)
	{
		deleteNode(o_primGenNodeA[i]);
		deleteNode(o_outputMeshA[i]);
	}



	return MS::kSuccess;
}