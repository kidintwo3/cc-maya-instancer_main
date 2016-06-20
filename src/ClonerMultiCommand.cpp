#include "ClonerMultiCommand.h"
#include "DagUtils.h"

ClonerMultiCommand::ClonerMultiCommand()
{
}


void* ClonerMultiCommand::creator()
{
	return new ClonerMultiCommand;
}


bool ClonerMultiCommand::isUndoable() const
{
	return true;
}

MSyntax ClonerMultiCommand::newSyntax()
{
	MSyntax syntax;

	syntax.addFlag( "-ab", "-cloneAtoB");
	syntax.addFlag( "-sp", "-cloneSpline");

	syntax.addFlag( "-cm", "-clonerMultiNode", MSyntax::kString  );

	syntax.setObjectType( MSyntax::kSelectionList, 1, 1 );
	syntax.useSelectionAsDefault( true );

	syntax.enableEdit( false );
	syntax.enableQuery( false );

	return syntax;
}





MStatus ClonerMultiCommand::doIt( const MArgList& argList )
{
	MStatus status;

	MArgDatabase argData( syntax(), argList, &status );


	// Get Selected Object

	MSelectionList selectedObjects;
	MGlobal::getActiveSelectionList(selectedObjects);

	if (selectedObjects.length() != 0)
	{
		//MGlobal::displayInfo(MString() + "[ClonerMulti] Nothing selected");




		MDagPath currDagPathTr;
		//MDagPath currDagPathShape;
		p_currSelTrA.clear();
		p_currSelShapeA.clear();
		p_currSelTrA_curves.clear();
		p_currSelShapeA_curves.clear();

		//bool ffound = false;


		// Get Meshes
		for (unsigned int i = 0; i < selectedObjects.length(); i++)
		{
			selectedObjects.getDagPath(i, currDagPathTr);

			if (currDagPathTr.apiType() == MFn::kTransform)
			{

				p_currSelTrA.append(currDagPathTr);

				status = getShapeNodeFromTransformDAG(currDagPathTr);
				if (status)
				{
					p_currSelShapeA.append(currDagPathTr);
				}
			}
		}


		// Get Curves
		for (unsigned int i = 0; i < selectedObjects.length(); i++)
		{
			selectedObjects.getDagPath(i, currDagPathTr);

			if (currDagPathTr.apiType() == MFn::kTransform)
			{

				p_currSelTrA_curves.append(currDagPathTr);

				status = getShapeNodeFromTransformDAG_curve(currDagPathTr);
				if (status)
				{
					p_currSelShapeA_curves.append(currDagPathTr);
				}
			}
		}

	}



	// --------------------------------------------------------------------------------------------------------------------------------------------
	// Loc A to B
	if ( argData.isFlagSet( "-ab" ) )
	{
		MString s_clonerMultiNodeName;

		if (argData.isFlagSet( "-cm" ))
		{
			MGlobal::displayError(MString() + "[Shellmod] No ClonerMulti name set for command (use the -cm flag to set)");
			argData.getFlagArgument("-cm", 0, s_clonerMultiNodeName);
			//return MStatus::kFailure;

			// Find ClonerMulti from name
			MObject MObj;
			MSelectionList selList;
			selList.add(s_clonerMultiNodeName);
			selList.getDependNode(0,MObj);
			MFnDependencyNode mfDgN(MObj);
			o_clonerMultiNode = mfDgN.object();
		}

		if (!argData.isFlagSet( "-cm" ))
		{
			o_clonerMultiNode = createNodeMaya(m_DEPNode,"clonerMulti");
		}


		//

		MFnDependencyNode fnDepCloner( o_clonerMultiNode );

		bool isConn; isConn = false;
		if (fnDepCloner.findPlug("locatorAPos").isConnected() || fnDepCloner.findPlug("locatorBPos").isConnected())
		{
			isConn = true;
			MGlobal::displayWarning(MString() + "[ClonerMulti] connected: A and/or B locator is already connected! - Delete both locators to attach a new one!"  );
		}


		MDagPath dag_LocBTr; // store this outside so we can select it

		if (!isConn)
		{



			o_locA = createNodeMaya(m_DEPNode,"clonerMultiLoc");
			o_locB = createNodeMaya(m_DEPNode,"clonerMultiLoc");


			MFnDependencyNode fnDepLocA( o_locA );
			MFnDependencyNode fnDepLocB( o_locB );

			// rename the nodes
			if (!argData.isFlagSet( "-cm" ))
			{
				fnDepCloner.setName( "clonerMulti_node#" );
			}

			fnDepLocA.setName( "clonerMulti_locA#" );
			fnDepLocB.setName( "clonerMulti_locB#" );

			MGlobal::displayInfo(MString() + "[ClonerMulti] Node created: " + fnDepCloner.name() + ", and " + fnDepLocA.name() + ", " + fnDepLocB.name() );




			// --------------------------------------------------------------------------------------------------------------------------------------------
			// DAgpath Loc A
			MDagPath dag_LocATr;
			MDagPath dag_LocAShape;
			MDagPath dag_LocA;
			MSelectionList sel_list;
			sel_list.add(fnDepLocA.name());
			sel_list.getDagPath(0,dag_LocA);
			dag_LocATr = dag_LocA;
			dag_LocA.extendToShape();
			dag_LocAShape = dag_LocA;

			// DAgpath Loc B
			//MDagPath dag_LocBTr;
			MDagPath dag_LocBShape;
			MDagPath dag_LocB;
			sel_list.clear();
			sel_list.add(fnDepLocB.name());
			sel_list.getDagPath(0,dag_LocB);
			dag_LocBTr = dag_LocB;
			dag_LocB.extendToShape();
			dag_LocBShape = dag_LocB;




			// Plugs of the clonerMulti node
			MPlug p_clonerMultiNode_inMesh = fnDepCloner.findPlug("inMesh");





			// Get loc B Plug and loc A plug
			MFnDependencyNode fnDepLocBShape( dag_LocBShape.node() );
			MFnDependencyNode fnDepLocAShape( dag_LocAShape.node() );

			MPlug p_locA_wm = fnDepLocAShape.findPlug( "worldMatrix" );
			MPlug p_locA_locID = fnDepLocAShape.findPlug( "locID" );
			p_locA_wm = p_locA_wm.elementByLogicalIndex(p_locA_wm.numConnectedElements());

			MPlug p_locB_wm = fnDepLocBShape.findPlug( "worldMatrix" );
			MPlug p_locB_locPos = fnDepLocBShape.findPlug( "locatorPos" );
			p_locB_wm = p_locB_wm.elementByLogicalIndex(p_locB_wm.numConnectedElements());

			// Connect Loc A to loc B
			status = m_DGMod.connect( p_locA_wm, p_locB_locPos );
			CHECK_MSTATUS_AND_RETURN_IT(status);
			m_DGMod.doIt();

			// Set loc A ID
			p_locA_locID.setInt(2);

			// Move locators to its deafault positions
			MFnTransform fnTrLocA( dag_LocATr );
			MFnTransform fnTrLocB( dag_LocBTr );

			fnTrLocA.translateBy( MVector(-5.0,0.0,0.0), MSpace::kWorld );
			fnTrLocB.translateBy( MVector( 5.0,0.0,0.0), MSpace::kWorld );



			if (!argData.isFlagSet( "-cm" ))
			{

				// -----------------------------------------------------------------------------------------
				// Connect the meshes
				MGlobal::displayInfo(MString() + "[ClonerMulti] --------------------------------------------");
				MGlobal::displayInfo(MString() + "[ClonerMulti] connected meshes: " + p_currSelShapeA.length());
				MGlobal::displayInfo(MString() + "[ClonerMulti] -----");

				for (int i = 0; i < p_currSelShapeA.length(); i++)
				{
					MFnDagNode fnDepSource( p_currSelShapeA[i].node() );
					MPlug plugSource = fnDepSource.findPlug( "worldMesh" );
					plugSource = plugSource.elementByLogicalIndex(0);

					MPlug plugTarget = fnDepCloner.findPlug( "inMesh" );
					plugTarget = plugTarget.elementByLogicalIndex(plugTarget.numConnectedElements());

					MGlobal::displayInfo(MString() + "[ClonerMulti] connected: " + plugSource.name() + " -> " + plugTarget.name()  );

					status = m_DGMod.connect( plugSource, plugTarget );
					CHECK_MSTATUS_AND_RETURN_IT(status);
					m_DGMod.doIt();

				}

			}



			// set clonerMulti node options to AB
			fnDepCloner.findPlug( "instanceType" ).setInt(3);
			fnDepCloner.findPlug( "patternType" ).setInt(1);
			fnDepCloner.findPlug( "instanceX" ).setInt(fnDepCloner.findPlug( "inMesh" ).numConnectedElements());
			fnDepCloner.findPlug( "instanceY" ).setInt(1);
			fnDepCloner.findPlug( "instanceZ" ).setInt(1);

			fnDepCloner.findPlug( "offsetX" ).setDouble(0.0);
			fnDepCloner.findPlug( "offsetY" ).setDouble(0.0);
			fnDepCloner.findPlug( "offsetZ" ).setDouble(0.0);


			// -----------------------------------------------------------------------------------------
			// Connect loc A and loc B to clonerMulti node
			MPlug p_clonerMultiNode_locA = fnDepCloner.findPlug("locatorAPos");
			MPlug p_clonerMultiNode_locB = fnDepCloner.findPlug("locatorBPos");

			status = m_DGMod.connect( p_locA_wm, p_clonerMultiNode_locA );
			CHECK_MSTATUS_AND_RETURN_IT(status);
			m_DGMod.doIt();

			status = m_DGMod.connect( p_locB_wm, p_clonerMultiNode_locB );
			CHECK_MSTATUS_AND_RETURN_IT(status);
			m_DGMod.doIt();




			if (!argData.isFlagSet( "-cm" ))
			{

				// -----------------------------------------------------------------------------------------
				// create output mesh


				MPlug p_clonerMultiNode_outMesh = fnDepCloner.findPlug( "outMesh" );

				if (!p_clonerMultiNode_outMesh.isConnected())
				{
					o_clonerOutputMesh = createNodeMaya(m_DEPNode,"mesh");
				}

				else
				{

					MGlobal::displayInfo(MString() + p_clonerMultiNode_outMesh.parent().partialName() );

					

					//// Find ClonerMulti from name
					//MObject MObj;
					//MSelectionList selList;
					//selList.add(s_clonerMultiNodeName);
					//selList.getDependNode(0,MObj);
					//MFnDependencyNode mfDgN(MObj);
					//o_clonerMultiNode = mfDgN.object();
				}



				MFnDependencyNode fnDepClonerOutputMesh( o_clonerOutputMesh );
				fnDepClonerOutputMesh.setName( "clonerMulti_mesh#" );
				// Find shape node of output mesh
				MDagPath dag_outMeshTr;
				MDagPath dag_outMeshShape;
				MDagPath dag_outMesh;
				sel_list.clear();
				sel_list.add(fnDepClonerOutputMesh.name());
				sel_list.getDagPath(0,dag_outMesh);
				dag_outMeshTr = dag_outMesh;
				dag_outMesh.extendToShape();
				dag_outMeshShape = dag_outMesh;

				MFnDependencyNode fnDepClonerOutputMeshShape( dag_outMeshShape.node() );


				MPlug p_clonerMultiMesh_inMesh = fnDepClonerOutputMeshShape.findPlug( "inMesh" );

				MGlobal::displayInfo(MString() + "[ClonerMulti] connected: " + p_clonerMultiNode_outMesh.name() + " -> " + p_clonerMultiMesh_inMesh.name()  );

				status = m_DGMod.connect( p_clonerMultiNode_outMesh, p_clonerMultiMesh_inMesh );
				CHECK_MSTATUS_AND_RETURN_IT(status);
				m_DGMod.doIt();

				// -----------------------------------------------------------------------------------------
				// Lock output mesh attributes

				fnDepClonerOutputMesh.findPlug( "tx" ).setLocked(true);
				fnDepClonerOutputMesh.findPlug( "ty" ).setLocked(true);
				fnDepClonerOutputMesh.findPlug( "tz" ).setLocked(true);

				fnDepClonerOutputMesh.findPlug( "rx" ).setLocked(true);
				fnDepClonerOutputMesh.findPlug( "ry" ).setLocked(true);
				fnDepClonerOutputMesh.findPlug( "rz" ).setLocked(true);

				fnDepClonerOutputMesh.findPlug( "sx" ).setLocked(true);
				fnDepClonerOutputMesh.findPlug( "sy" ).setLocked(true);
				fnDepClonerOutputMesh.findPlug( "sz" ).setLocked(true);


				// -------------------------------------------------------
				// Assign Same material as input mesh
				assignSameMaterial(p_currSelShapeA[0], o_clonerOutputMesh);
			}

			sel_list.clear();
			sel_list.add(dag_LocBTr.partialPathName());
			MGlobal::setActiveSelectionList(sel_list, MGlobal::kReplaceList);

		}
	}

	// --------------------------------------------------------------------------------------------------------------------------------------------
	// Spline

	else if ( argData.isFlagSet( "-sp" ) )
	{


		o_clonerMultiNode = createNodeMaya(m_DEPNode,"clonerMulti");
		MFnDependencyNode fnDepCloner( o_clonerMultiNode );

		// rename the nodes
		fnDepCloner.setName( "clonerMulti_node#" );

		MGlobal::displayInfo(MString() + "[ClonerMulti] Node created: " + fnDepCloner.name() );

		MGlobal::displayInfo(MString() + "[ClonerMulti] Number of meshes: " + p_currSelShapeA.length() );
		MGlobal::displayInfo(MString() + "[ClonerMulti] Number of curves: " + p_currSelShapeA_curves.length() );


		// --------------------------------------------------------------------------------------------------------------------------------------------

	}









	// --------------------------------------------------------------------------------------------------------------------------------------------
	// Default (just create the node by itself) if nothing is selected
	if (selectedObjects.length() == 0)
	{

		o_clonerMultiNode = createNodeMaya(m_DEPNode,"clonerMulti");
		MFnDependencyNode fnDepCloner( o_clonerMultiNode );

		// rename the nodes
		fnDepCloner.setName( "clonerMulti_node#" );

	//	//MGlobal::displayInfo(MString() + "[ClonerMulti] Node created: " + fnDepCloner.name() );

	//	// -----------------------------------------------------------------------------------------
	//	// create output mesh

		o_clonerOutputMesh = createNodeMaya(m_DEPNode,"mesh");
		MFnDependencyNode fnDepClonerOutputMesh( o_clonerOutputMesh );
		fnDepClonerOutputMesh.setName( "clonerMulti_mesh#" );
	//	// Find shape node of output mesh
		MDagPath dag_outMeshTr;
		MDagPath dag_outMeshShape;
		MDagPath dag_outMesh;
		MSelectionList sel_list;
		sel_list.clear();
		sel_list.add(fnDepClonerOutputMesh.name());
		sel_list.getDagPath(0,dag_outMesh);
		dag_outMeshTr = dag_outMesh;
		dag_outMesh.extendToShape();
		dag_outMeshShape = dag_outMesh;

		MFnDependencyNode fnDepClonerOutputMeshShape( dag_outMeshShape.node() );

		MPlug p_clonerMultiNode_outMesh = fnDepCloner.findPlug( "outMesh" );
		MPlug p_clonerMultiMesh_inMesh = fnDepClonerOutputMeshShape.findPlug( "inMesh" );

	//	MGlobal::displayInfo(MString() + "[ClonerMulti] connected: " + p_clonerMultiNode_outMesh.name() + " -> " + p_clonerMultiMesh_inMesh.name()  );

		status = m_DGMod.connect( p_clonerMultiNode_outMesh, p_clonerMultiMesh_inMesh );
		CHECK_MSTATUS_AND_RETURN_IT(status);
		m_DGMod.doIt();

		assignInitialShadingGroup(o_clonerOutputMesh);

		MGlobal::selectByName(fnDepCloner.name(), MGlobal::kReplaceList);



	}





	return redoIt();
}

MStatus ClonerMultiCommand::redoIt()
{
	MStatus status;







	return MS::kSuccess;
}

MStatus ClonerMultiCommand::undoIt()
{
	MStatus status;

	// Restore the initial state
	status = m_DGMod.undoIt();
	CHECK_MSTATUS_AND_RETURN_IT( status );

	status = m_DAGMod.undoIt();
	CHECK_MSTATUS_AND_RETURN_IT(status);


	status = m_DGMod.deleteNode(o_clonerMultiNode);
	CHECK_MSTATUS_AND_RETURN_IT(status);
	status = m_DAGMod.deleteNode(o_locA);
	CHECK_MSTATUS_AND_RETURN_IT(status);
	status = m_DAGMod.deleteNode(o_locB);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	status = m_DAGMod.doIt();
	CHECK_MSTATUS_AND_RETURN_IT(status);
	status = m_DGMod.doIt();
	CHECK_MSTATUS_AND_RETURN_IT(status);

	return MS::kSuccess;
}