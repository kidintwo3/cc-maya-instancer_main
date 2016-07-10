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

	syntax.addFlag( "-na", "-name", MSyntax::kString);

	//syntax.addFlag( "-g", "-grid");
	//syntax.addFlag( "-c", "-circle");
	syntax.addFlag( "-ab", "-atob");
	//syntax.addFlag( "-s", "-sphere");
	//syntax.addFlag( "-c", "-curve");
	//syntax.addFlag( "-m", "-mesh");
	syntax.addFlag( "-b", "-bake");

	syntax.addFlag( "-no", "-node", MSyntax::kString  );

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




	// Get selection

	if (selectedObjects.length() != 0)
	{

		MDagPath currDagPathTr;
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










	o_outputObjectsA.clear();

	MString clonerMulti_node_name("clonerMulti_node_#");
	MString clonerMulti_mesh_name("clonerMulti_mesh_#");
	MString clonerMulti_locA_name("clonerMulti_locA_#");
	MString clonerMulti_locB_name("clonerMulti_locB_#");



	// Setting the name
	if ( argData.isFlagSet( "-na" ) )
	{
		MString customName;
		argData.getFlagArgument("-na", 0, customName);

		clonerMulti_node_name = MString() + customName + "_node_#";
		clonerMulti_mesh_name = MString() + customName + "_mesh_#";
	}




	// Default node creation
	if ( !argData.isFlagSet( "-no" ) )
	{

		// Create clonerMulti node
		o_clonerMultiNode = m_DAGMod.createNode(MTypeId(0x00123946));
		MFnDependencyNode fnDepCloner( o_clonerMultiNode );
		fnDepCloner.setName( clonerMulti_node_name  );

		// Create clonerMulti output mesh
		o_clonerOutputMesh = m_DAGMod.createNode("mesh");
		MFnDependencyNode fnDepClonerOutputMesh( o_clonerOutputMesh );
		fnDepClonerOutputMesh.setName( clonerMulti_mesh_name );

		status = m_DAGMod.doIt();
		CHECK_MSTATUS_AND_RETURN_IT(status);


		// Find shape node of node
		MDagPath dag_clonerNodeTr;
		MDagPath dag_clonerNodeShape;
		MDagPath dag_clonerNode;
		MSelectionList sel_list;
		sel_list.clear();
		sel_list.add(fnDepCloner.name());
		sel_list.getDagPath(0,dag_clonerNode);
		dag_clonerNodeTr = dag_clonerNode;
		dag_clonerNode.extendToShape();
		dag_clonerNodeShape = dag_clonerNode;

		// Find shape node of output mesh
		MDagPath dag_outMeshTr;
		MDagPath dag_outMeshShape;
		MDagPath dag_outMesh;
		sel_list.clear();
		status = sel_list.add(fnDepClonerOutputMesh.name());
		CHECK_MSTATUS_AND_RETURN_IT(status);
		status = sel_list.getDagPath(0,dag_outMesh);
		CHECK_MSTATUS_AND_RETURN_IT(status);
		dag_outMeshTr = dag_outMesh;
		status = dag_outMesh.extendToShape();
		CHECK_MSTATUS_AND_RETURN_IT(status);
		dag_outMeshShape = dag_outMesh;

		// Connect the mesh to the node
		MFnDependencyNode fnDepClonerOutputMeshShape( dag_outMeshShape.node() );
		MFnDependencyNode fnDepClonerNodeShape( dag_clonerNodeShape.node() );

		MPlug p_clonerMultiMesh_inMesh = fnDepClonerOutputMeshShape.findPlug( "inMesh", &status );
		CHECK_MSTATUS_AND_RETURN_IT(status);

		MPlug p_clonerMultiNode_outMesh = fnDepClonerNodeShape.findPlug( "outMesh", &status );
		CHECK_MSTATUS_AND_RETURN_IT(status);

		MPlug p_clonerMultiNode_inMesh = fnDepClonerNodeShape.findPlug( "inMesh", &status );
		CHECK_MSTATUS_AND_RETURN_IT(status);

		MPlug p_clonerMultiNode_inCurve = fnDepClonerNodeShape.findPlug( "inCurve", &status );
		CHECK_MSTATUS_AND_RETURN_IT(status);

		status = m_DAGMod.connect( p_clonerMultiNode_outMesh, p_clonerMultiMesh_inMesh );
		CHECK_MSTATUS_AND_RETURN_IT(status);
		m_DAGMod.doIt();




		// Assign Same material as input mesh
		assignInitialShadingGroup(o_clonerOutputMesh);


		// Add selected objects
		for (int i = 0; i < p_currSelShapeA.length(); i++)
		{


			MFnDagNode fnDepSource( p_currSelShapeA[i].node() );
			MPlug plugSource = fnDepSource.findPlug( "worldMesh" );
			plugSource = plugSource.elementByLogicalIndex(0);


			MPlug plugTarget = p_clonerMultiNode_inMesh.elementByLogicalIndex(i);

			MGlobal::displayInfo(MString() + "[ClonerMulti] connected: " + plugSource.name() + " -> " + p_clonerMultiNode_inMesh.name()  );

			status = m_DAGMod.connect( plugSource, plugTarget );
			CHECK_MSTATUS_AND_RETURN_IT(status);
			m_DAGMod.doIt();



		}

		// Add selected curves
		for (int i = 0; i < p_currSelShapeA_curves.length(); i++)
		{

			if (i==0)
			{

				MFnDagNode fnDepSource( p_currSelShapeA_curves[i].node() );
				MPlug plugSource = fnDepSource.findPlug( "worldSpace" );
				plugSource = plugSource.elementByLogicalIndex(0);





				MGlobal::displayInfo(MString() + "[ClonerMulti] connected: " + plugSource.name() + " -> " + p_clonerMultiNode_inCurve.name()  );

				status = m_DAGMod.connect( plugSource, p_clonerMultiNode_inCurve );
				CHECK_MSTATUS_AND_RETURN_IT(status);
				m_DAGMod.doIt();

				// Plugs of the clonerMulti node
				MPlug p_instancetype = fnDepClonerNodeShape.findPlug("instanceType", &status);
				CHECK_MSTATUS_AND_RETURN_IT(status);

				status = p_instancetype.setShort(4);
				CHECK_MSTATUS_AND_RETURN_IT(status);


				fnDepClonerNodeShape.findPlug("offsetX").setFloat(0.0);
				fnDepClonerNodeShape.findPlug("offsetY").setFloat(0.0);
				fnDepClonerNodeShape.findPlug("offsetZ").setFloat(0.0);


			}


		}


	}





	// A-B locator creation
	if (argData.isFlagSet("-ab"))
	{

		if (argData.isFlagSet("-no"))
		{


			MString s_clonerMultiNodeName;

			argData.getFlagArgument("-no", 0, s_clonerMultiNodeName);

			// Find ClonerMulti from name
			MObject MObj;
			MSelectionList selList;
			selList.add(s_clonerMultiNodeName);
			selList.getDependNode(0,MObj);
			MFnDependencyNode mfDgN(MObj);
			o_clonerMultiNode = mfDgN.object();


			if (!o_clonerMultiNode.isNull())
			{
				// Dependency node so we can get the worldMatrix attribute
				MFnDependencyNode fnDepCloner(o_clonerMultiNode);

				// Find shape node of node
				MDagPath dag_clonerNodeTr;
				MDagPath dag_clonerNodeShape;
				MDagPath dag_clonerNode;
				MSelectionList sel_list;
				sel_list.clear();
				status = sel_list.add(fnDepCloner.name());
				CHECK_MSTATUS_AND_RETURN_IT(status);
				status = sel_list.getDagPath(0,dag_clonerNode);
				CHECK_MSTATUS_AND_RETURN_IT(status);
				dag_clonerNodeTr = dag_clonerNode;
				status = dag_clonerNode.extendToShape();
				CHECK_MSTATUS_AND_RETURN_IT(status);

				dag_clonerNodeShape = dag_clonerNode;

				MFnDependencyNode fnDepClonerNodeShape( dag_clonerNodeShape.node() );



				// Get it's locator A plug
				MObject inLocator_A_Attr = fnDepClonerNodeShape.attribute( "locatorAPos", &status );
				CHECK_MSTATUS_AND_RETURN_IT(status);

				// Get it's locator B plug
				MObject inLocator_B_Attr = fnDepClonerNodeShape.attribute( "locatorBPos", &status );
				CHECK_MSTATUS_AND_RETURN_IT(status);

				MPlug inLocator_A_plug = MPlug( fnDepClonerNodeShape.object(), inLocator_A_Attr );
				MPlug inLocator_B_plug = MPlug( fnDepClonerNodeShape.object(), inLocator_B_Attr );


				if (!inLocator_A_plug.isConnected() && !inLocator_B_plug.isConnected())
				{

					// Create clonerMulti locator A
					o_locA = m_DAGMod.createNode(MTypeId(0x00123947));
					MFnDependencyNode fnDepLocA( o_locA );
					fnDepLocA.setName( clonerMulti_locA_name );

					// Create clonerMulti locator B
					o_locB = m_DAGMod.createNode(MTypeId(0x00123947));
					MFnDependencyNode fnDepLocB( o_locB );
					fnDepLocB.setName( clonerMulti_locB_name );

					m_DAGMod.doIt();



					MDagPath dag_LocATr;
					MDagPath dag_LocAShape;
					MDagPath dag_LocA;
					sel_list.clear();
					sel_list.add(fnDepLocA.name());
					sel_list.getDagPath(0,dag_LocA);
					dag_LocATr = dag_LocA;
					dag_LocA.extendToShape();
					dag_LocAShape = dag_LocA;

					// DAgpath Loc B
					MDagPath dag_LocBTr;
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
					p_locA_wm = p_locA_wm.elementByLogicalIndex(p_locA_wm.numConnectedElements());

					MPlug p_locB_wm = fnDepLocBShape.findPlug( "worldMatrix" );
					p_locB_wm = p_locB_wm.elementByLogicalIndex(p_locB_wm.numConnectedElements());

					// Connect Loc A 
					status = m_DAGMod.connect( p_locA_wm, inLocator_A_plug );
					CHECK_MSTATUS_AND_RETURN_IT(status);
					m_DAGMod.doIt();

					// Connect Loc B
					status = m_DAGMod.connect( p_locB_wm, inLocator_B_plug );
					CHECK_MSTATUS_AND_RETURN_IT(status);
					m_DAGMod.doIt();


					// Move locators to its deafault positions
					MFnTransform fnTrLocA( dag_LocATr );
					MFnTransform fnTrLocB( dag_LocBTr );

					fnTrLocA.translateBy( MVector(-10.0,0.0,0.0), MSpace::kWorld );
					fnTrLocB.translateBy( MVector( 10.0,0.0,0.0), MSpace::kWorld );


					// Plugs of the clonerMulti node
					MPlug p_instancetype = fnDepCloner.findPlug("instanceType", &status);
					CHECK_MSTATUS_AND_RETURN_IT(status);

					status = p_instancetype.setShort(3);
					CHECK_MSTATUS_AND_RETURN_IT(status);

					fnDepClonerNodeShape.findPlug("offsetX").setFloat(0.0);
					fnDepClonerNodeShape.findPlug("offsetY").setFloat(0.0);
					fnDepClonerNodeShape.findPlug("offsetZ").setFloat(0.0);


				}


			}


		}

	}






	// Baking objects
	if (argData.isFlagSet("-b"))
	{

		if (argData.isFlagSet("-no"))
		{



			MString s_clonerMultiNodeName;

			argData.getFlagArgument("-no", 0, s_clonerMultiNodeName);

			// Find ClonerMulti from name
			MObject MObj;
			MSelectionList selList;
			selList.add(s_clonerMultiNodeName);
			selList.getDependNode(0,MObj);
			MFnDependencyNode mfDgN(MObj);
			o_clonerMultiNode = mfDgN.object();


			if (!o_clonerMultiNode.isNull())
			{
				// Dependency node so we can get the worldMatrix attribute
				MFnDependencyNode fnDepCloner(o_clonerMultiNode);

				// Find shape node of node
				MDagPath dag_clonerNodeTr;
				MDagPath dag_clonerNodeShape;
				MDagPath dag_clonerNode;
				MSelectionList sel_list;
				sel_list.clear();
				status = sel_list.add(fnDepCloner.name());
				CHECK_MSTATUS_AND_RETURN_IT(status);
				status = sel_list.getDagPath(0,dag_clonerNode);
				CHECK_MSTATUS_AND_RETURN_IT(status);
				dag_clonerNodeTr = dag_clonerNode;
				status = dag_clonerNode.extendToShape();
				CHECK_MSTATUS_AND_RETURN_IT(status);

				dag_clonerNodeShape = dag_clonerNode;

				MFnDependencyNode fnDepClonerNodeShape( dag_clonerNodeShape.node() );



				// Get it's world matrix plug
				MObject worldMatrixAttr = fnDepClonerNodeShape.attribute( "outMatrixArray", &status );
				CHECK_MSTATUS_AND_RETURN_IT(status);

				MObject idArrayAttr = fnDepClonerNodeShape.attribute( "outIDArray", &status );
				CHECK_MSTATUS_AND_RETURN_IT(status);

				MObject inMeshAttr = fnDepClonerNodeShape.attribute( "inMesh", &status );
				CHECK_MSTATUS_AND_RETURN_IT(status);

				MObject outMeshAttr = fnDepClonerNodeShape.attribute( "outMesh", &status );
				CHECK_MSTATUS_AND_RETURN_IT(status);

				// Get it's locator A plug
				MObject inLocator_A_Attr = fnDepClonerNodeShape.attribute( "locatorAPos", &status );
				CHECK_MSTATUS_AND_RETURN_IT(status);

				// Get it's locator B plug
				MObject inLocator_B_Attr = fnDepClonerNodeShape.attribute( "locatorBPos", &status );
				CHECK_MSTATUS_AND_RETURN_IT(status);




				// Getting mPlug by plugging in our MObject and attribute
				MPlug matrixPlug = MPlug( fnDepClonerNodeShape.object(), worldMatrixAttr );
				MPlug idPlug = MPlug( fnDepClonerNodeShape.object(), idArrayAttr );
				MPlug inMeshPlug = MPlug( fnDepClonerNodeShape.object(), inMeshAttr );
				MPlug outMeshPlug = MPlug( fnDepClonerNodeShape.object(), outMeshAttr );
				MPlug inLocator_A_plug = MPlug( fnDepClonerNodeShape.object(), inLocator_A_Attr );
				MPlug inLocator_B_plug = MPlug( fnDepClonerNodeShape.object(), inLocator_B_Attr );

				//o_locA = inLocator_A_plug.asMObject();
				//o_locB = inLocator_B_plug.asMObject();

				//MFnDependencyNode fnDepLocA( o_locA );
				//MFnDependencyNode fnDepLocB( o_locB );

				MObject matrixObject = matrixPlug.asMObject();
				MObject idObject = idPlug.asMObject();


				MFnIntArrayData idArrayData( idObject );
				MIntArray out_idArray;

				MFnMatrixArrayData worldMatrixData( matrixObject );
				MMatrixArray out_matrixArray;

				status = idArrayData.copyTo(out_idArray);
				CHECK_MSTATUS_AND_RETURN_IT(status);

				status = worldMatrixData.copyTo(out_matrixArray);
				CHECK_MSTATUS_AND_RETURN_IT(status);


				if (out_idArray.length() == out_matrixArray.length())
				{


					// Find output mesh
					MPlugArray destPlugs;
					outMeshPlug.connectedTo(destPlugs, false, true);

					MPlug destPlug = destPlugs[0];
					MFnDagNode depN(destPlug.node());

					MObject parent_tr = depN.parent(0, &status);
					CHECK_MSTATUS_AND_RETURN_IT(status);
					MFnDagNode parent_depN(parent_tr);

					MFnTransform trMat(parent_tr);

					MMatrix out_tr_mat = trMat.transformation().asMatrix();
					CHECK_MSTATUS_AND_RETURN_IT(status);

					// MGlobal::displayInfo(MString() + parent_depN.name());

					// Collect input meshes


					for (int i = 0; i < out_idArray.length(); i++)
					{
						int currId = out_idArray[i];

						MPlug currP = inMeshPlug.elementByPhysicalIndex(currId, &status);
						CHECK_MSTATUS_AND_RETURN_IT(status);

						if (status)
						{

							if (currP.isConnected())
							{

								MPlugArray inputs_plugArr;
								currP.connectedTo(inputs_plugArr, true, false, &status);
								CHECK_MSTATUS_AND_RETURN_IT(status);
								currP = inputs_plugArr[0];

								if (!currP.isNull())
								{

									MFnDagNode inMesh_dn(currP.node());

									// Create output mesh
									MObject outputObj = inMesh_dn.duplicate(true, false, &status);
									CHECK_MSTATUS_AND_RETURN_IT(status);

									

					/*				MFnDependencyNode fnDepLocB( outputObj );
									fnDepLocB.setName( MString() + inMesh_dn.name() + "_clone_#", false, &status );
									CHECK_MSTATUS_AND_RETURN_IT(status);*/

									// Set transforms of output mesh
									MFnTransform fn_transform(outputObj);
									MTransformationMatrix trMAt(out_matrixArray[i] * out_tr_mat);

									status = fn_transform.set(trMAt);
									CHECK_MSTATUS_AND_RETURN_IT(status);



									// Find shape node of output mesh
									MString sMaterial;

									MFnMesh fnMesh(currP.node());
									// A ShadingGroup will have a MFnSet
									MObjectArray sets, comps;
									fnMesh.getConnectedSetsAndMembers(0, sets, comps, true);

									// Each set is a Shading Group. Loop through them
									for (unsigned int i = 0; i < sets.length(); ++i)
									{
										MFnDependencyNode fnDepSGNode(sets[i]);
										sMaterial = fnDepSGNode.name();
									}

									MSelectionList sList;
									MGlobal::getSelectionListByName(sMaterial, sList);
									MObject oInitialShadingGroup;
									status = sList.getDependNode(0, oInitialShadingGroup);
									CHECK_MSTATUS_AND_RETURN_IT(status);

									MFnSet fnShadingGroup(oInitialShadingGroup, &status);
									CHECK_MSTATUS_AND_RETURN_IT(status);

									status = fnShadingGroup.addMember(outputObj);
									CHECK_MSTATUS_AND_RETURN_IT(status);


									// Add output objects
									o_outputObjectsA.append(outputObj);

								}

							}


						}


					}



					if (inLocator_A_plug.isConnected())
					{

						

						// Find input locator
						MPlugArray destPlugs;
						inLocator_A_plug.connectedTo(destPlugs, true, false);


						MGlobal::displayInfo(MString() + destPlugs.length());

						MPlug destPlug = destPlugs[0];
						MFnDagNode depN(destPlug.node());

						MObject parent_tr = depN.parent(0, &status);
						CHECK_MSTATUS_AND_RETURN_IT(status);
						MFnDagNode parent_depN(parent_tr);

						if (!parent_depN.object().isNull())
						{
							// Delete A locator
							status = m_DAGMod.deleteNode(parent_depN.object());
							CHECK_MSTATUS_AND_RETURN_IT(status);
						}


					}

					if (inLocator_B_plug.isConnected())
					{
						// Find input locator
						MPlugArray destPlugs;
						inLocator_B_plug.connectedTo(destPlugs, true, false);

						MPlug destPlug = destPlugs[0];
						MFnDagNode depN(destPlug.node());

						MObject parent_tr = depN.parent(0, &status);
						CHECK_MSTATUS_AND_RETURN_IT(status);
						MFnDagNode parent_depN(parent_tr);

						if (!parent_depN.object().isNull())
						{
							// Delete A locator
							status = m_DAGMod.deleteNode(parent_depN.object());
							CHECK_MSTATUS_AND_RETURN_IT(status);
						}

					}


					// Delete output mesh
					status = m_DAGMod.deleteNode(parent_depN.object());
					CHECK_MSTATUS_AND_RETURN_IT(status);

					// Delete cloner node
					status = m_DAGMod.deleteNode(fnDepCloner.object());
					CHECK_MSTATUS_AND_RETURN_IT(status);

					status = m_DAGMod.doIt();
					CHECK_MSTATUS_AND_RETURN_IT(status);

				}

			}
		}

		else
		{
			MGlobal::displayError(MString() + "[ClonerMulti] No/wrong ClonerMulti name set for command (use the -cm flag to set)");
			return MStatus::kFailure;
		}




	}














	//if (selectedObjects.length() != 0)
	//{
	//	//MGlobal::displayInfo(MString() + "[ClonerMulti] Nothing selected");




	//	MDagPath currDagPathTr;
	//	//MDagPath currDagPathShape;
	//	p_currSelTrA.clear();
	//	p_currSelShapeA.clear();
	//	p_currSelTrA_curves.clear();
	//	p_currSelShapeA_curves.clear();

	//	//bool ffound = false;


	//	// Get Meshes
	//	for (unsigned int i = 0; i < selectedObjects.length(); i++)
	//	{
	//		selectedObjects.getDagPath(i, currDagPathTr);

	//		if (currDagPathTr.apiType() == MFn::kTransform)
	//		{

	//			p_currSelTrA.append(currDagPathTr);

	//			status = getShapeNodeFromTransformDAG(currDagPathTr);
	//			if (status)
	//			{
	//				p_currSelShapeA.append(currDagPathTr);
	//			}
	//		}
	//	}


	//	// Get Curves
	//	for (unsigned int i = 0; i < selectedObjects.length(); i++)
	//	{
	//		selectedObjects.getDagPath(i, currDagPathTr);

	//		if (currDagPathTr.apiType() == MFn::kTransform)
	//		{

	//			p_currSelTrA_curves.append(currDagPathTr);

	//			status = getShapeNodeFromTransformDAG_curve(currDagPathTr);
	//			if (status)
	//			{
	//				p_currSelShapeA_curves.append(currDagPathTr);
	//			}
	//		}
	//	}

	//}



	//// --------------------------------------------------------------------------------------------------------------------------------------------
	//// Loc A to B
	//if ( argData.isFlagSet( "-ab" ) )
	//{
	//	MString s_clonerMultiNodeName;

	//	if (argData.isFlagSet( "-cm" ))
	//	{
	//		MGlobal::displayError(MString() + "[ClonerMulti] No ClonerMulti name set for command (use the -cm flag to set)");
	//		argData.getFlagArgument("-cm", 0, s_clonerMultiNodeName);
	//		//return MStatus::kFailure;

	//		// Find ClonerMulti from name
	//		MObject MObj;
	//		MSelectionList selList;
	//		selList.add(s_clonerMultiNodeName);
	//		selList.getDependNode(0,MObj);
	//		MFnDependencyNode mfDgN(MObj);
	//		o_clonerMultiNode = mfDgN.object();
	//	}

	//	if (!argData.isFlagSet( "-cm" ))
	//	{
	//		o_clonerMultiNode = createNodeMaya(m_DEPNode,"clonerMulti");
	//	}


	//	//

	//	MFnDependencyNode fnDepCloner( o_clonerMultiNode );

	//	bool isConn; isConn = false;
	//	if (fnDepCloner.findPlug("locatorAPos").isConnected() || fnDepCloner.findPlug("locatorBPos").isConnected())
	//	{
	//		isConn = true;
	//		MGlobal::displayWarning(MString() + "[ClonerMulti] connected: A and/or B locator is already connected! - Delete both locators to attach a new one!"  );
	//	}


	//	MDagPath dag_LocBTr; // store this outside so we can select it

	//	if (!isConn)
	//	{



	//		o_locA = createNodeMaya(m_DEPNode,"clonerMultiLoc");
	//		o_locB = createNodeMaya(m_DEPNode,"clonerMultiLoc");


	//		MFnDependencyNode fnDepLocA( o_locA );
	//		MFnDependencyNode fnDepLocB( o_locB );

	//		// rename the nodes
	//		if (!argData.isFlagSet( "-cm" ))
	//		{
	//			fnDepCloner.setName( "clonerMulti_node#" );
	//		}

	//		fnDepLocA.setName( "clonerMulti_locA#" );
	//		fnDepLocB.setName( "clonerMulti_locB#" );

	//		MGlobal::displayInfo(MString() + "[ClonerMulti] Node created: " + fnDepCloner.name() + ", and " + fnDepLocA.name() + ", " + fnDepLocB.name() );




	//		// --------------------------------------------------------------------------------------------------------------------------------------------
	//		// DAgpath Loc A
	//		MDagPath dag_LocATr;
	//		MDagPath dag_LocAShape;
	//		MDagPath dag_LocA;
	//		MSelectionList sel_list;
	//		sel_list.add(fnDepLocA.name());
	//		sel_list.getDagPath(0,dag_LocA);
	//		dag_LocATr = dag_LocA;
	//		dag_LocA.extendToShape();
	//		dag_LocAShape = dag_LocA;

	//		// DAgpath Loc B
	//		//MDagPath dag_LocBTr;
	//		MDagPath dag_LocBShape;
	//		MDagPath dag_LocB;
	//		sel_list.clear();
	//		sel_list.add(fnDepLocB.name());
	//		sel_list.getDagPath(0,dag_LocB);
	//		dag_LocBTr = dag_LocB;
	//		dag_LocB.extendToShape();
	//		dag_LocBShape = dag_LocB;




	//		// Plugs of the clonerMulti node
	//		MPlug p_clonerMultiNode_inMesh = fnDepCloner.findPlug("inMesh");





	//		// Get loc B Plug and loc A plug
	//		MFnDependencyNode fnDepLocBShape( dag_LocBShape.node() );
	//		MFnDependencyNode fnDepLocAShape( dag_LocAShape.node() );

	//		MPlug p_locA_wm = fnDepLocAShape.findPlug( "worldMatrix" );
	//		MPlug p_locA_locID = fnDepLocAShape.findPlug( "locID" );
	//		p_locA_wm = p_locA_wm.elementByLogicalIndex(p_locA_wm.numConnectedElements());

	//		MPlug p_locB_wm = fnDepLocBShape.findPlug( "worldMatrix" );
	//		MPlug p_locB_locPos = fnDepLocBShape.findPlug( "locatorPos" );
	//		p_locB_wm = p_locB_wm.elementByLogicalIndex(p_locB_wm.numConnectedElements());

	//		// Connect Loc A to loc B
	//		status = m_DGMod.connect( p_locA_wm, p_locB_locPos );
	//		CHECK_MSTATUS_AND_RETURN_IT(status);
	//		m_DGMod.doIt();

	//		// Set loc A ID
	//		p_locA_locID.setInt(2);

	//		// Move locators to its deafault positions
	//		MFnTransform fnTrLocA( dag_LocATr );
	//		MFnTransform fnTrLocB( dag_LocBTr );

	//		fnTrLocA.translateBy( MVector(-5.0,0.0,0.0), MSpace::kWorld );
	//		fnTrLocB.translateBy( MVector( 5.0,0.0,0.0), MSpace::kWorld );



	//		if (!argData.isFlagSet( "-cm" ))
	//		{

	//			// -----------------------------------------------------------------------------------------
	//			// Connect the meshes
	//			MGlobal::displayInfo(MString() + "[ClonerMulti] --------------------------------------------");
	//			MGlobal::displayInfo(MString() + "[ClonerMulti] connected meshes: " + p_currSelShapeA.length());
	//			MGlobal::displayInfo(MString() + "[ClonerMulti] -----");

	//			for (int i = 0; i < p_currSelShapeA.length(); i++)
	//			{
	//				MFnDagNode fnDepSource( p_currSelShapeA[i].node() );
	//				MPlug plugSource = fnDepSource.findPlug( "worldMesh" );
	//				plugSource = plugSource.elementByLogicalIndex(0);

	//				MPlug plugTarget = fnDepCloner.findPlug( "inMesh" );
	//				plugTarget = plugTarget.elementByLogicalIndex(plugTarget.numConnectedElements());

	//				MGlobal::displayInfo(MString() + "[ClonerMulti] connected: " + plugSource.name() + " -> " + plugTarget.name()  );

	//				status = m_DGMod.connect( plugSource, plugTarget );
	//				CHECK_MSTATUS_AND_RETURN_IT(status);
	//				m_DGMod.doIt();

	//			}

	//		}



	//		// set clonerMulti node options to AB
	//		fnDepCloner.findPlug( "instanceType" ).setInt(3);
	//		fnDepCloner.findPlug( "patternType" ).setInt(1);
	//		fnDepCloner.findPlug( "instanceX" ).setInt(fnDepCloner.findPlug( "inMesh" ).numConnectedElements());
	//		fnDepCloner.findPlug( "instanceY" ).setInt(1);
	//		fnDepCloner.findPlug( "instanceZ" ).setInt(1);

	//		fnDepCloner.findPlug( "offsetX" ).setDouble(0.0);
	//		fnDepCloner.findPlug( "offsetY" ).setDouble(0.0);
	//		fnDepCloner.findPlug( "offsetZ" ).setDouble(0.0);


	//		// -----------------------------------------------------------------------------------------
	//		// Connect loc A and loc B to clonerMulti node
	//		MPlug p_clonerMultiNode_locA = fnDepCloner.findPlug("locatorAPos");
	//		MPlug p_clonerMultiNode_locB = fnDepCloner.findPlug("locatorBPos");

	//		status = m_DGMod.connect( p_locA_wm, p_clonerMultiNode_locA );
	//		CHECK_MSTATUS_AND_RETURN_IT(status);
	//		m_DGMod.doIt();

	//		status = m_DGMod.connect( p_locB_wm, p_clonerMultiNode_locB );
	//		CHECK_MSTATUS_AND_RETURN_IT(status);
	//		m_DGMod.doIt();




	//		if (!argData.isFlagSet( "-cm" ))
	//		{

	//			// -----------------------------------------------------------------------------------------
	//			// create output mesh


	//			MPlug p_clonerMultiNode_outMesh = fnDepCloner.findPlug( "outMesh" );

	//			if (!p_clonerMultiNode_outMesh.isConnected())
	//			{
	//				o_clonerOutputMesh = createNodeMaya(m_DEPNode,"mesh");
	//			}

	//			else
	//			{

	//				MGlobal::displayInfo(MString() + p_clonerMultiNode_outMesh.parent().partialName() );

	//				

	//				//// Find ClonerMulti from name
	//				//MObject MObj;
	//				//MSelectionList selList;
	//				//selList.add(s_clonerMultiNodeName);
	//				//selList.getDependNode(0,MObj);
	//				//MFnDependencyNode mfDgN(MObj);
	//				//o_clonerMultiNode = mfDgN.object();
	//			}



	//			MFnDependencyNode fnDepClonerOutputMesh( o_clonerOutputMesh );
	//			fnDepClonerOutputMesh.setName( "clonerMulti_mesh#" );
	//			// Find shape node of output mesh
	//			MDagPath dag_outMeshTr;
	//			MDagPath dag_outMeshShape;
	//			MDagPath dag_outMesh;
	//			sel_list.clear();
	//			sel_list.add(fnDepClonerOutputMesh.name());
	//			sel_list.getDagPath(0,dag_outMesh);
	//			dag_outMeshTr = dag_outMesh;
	//			dag_outMesh.extendToShape();
	//			dag_outMeshShape = dag_outMesh;

	//			MFnDependencyNode fnDepClonerOutputMeshShape( dag_outMeshShape.node() );


	//			MPlug p_clonerMultiMesh_inMesh = fnDepClonerOutputMeshShape.findPlug( "inMesh" );

	//			MGlobal::displayInfo(MString() + "[ClonerMulti] connected: " + p_clonerMultiNode_outMesh.name() + " -> " + p_clonerMultiMesh_inMesh.name()  );

	//			status = m_DGMod.connect( p_clonerMultiNode_outMesh, p_clonerMultiMesh_inMesh );
	//			CHECK_MSTATUS_AND_RETURN_IT(status);
	//			m_DGMod.doIt();

	//			// -----------------------------------------------------------------------------------------
	//			// Lock output mesh attributes

	//			fnDepClonerOutputMesh.findPlug( "tx" ).setLocked(true);
	//			fnDepClonerOutputMesh.findPlug( "ty" ).setLocked(true);
	//			fnDepClonerOutputMesh.findPlug( "tz" ).setLocked(true);

	//			fnDepClonerOutputMesh.findPlug( "rx" ).setLocked(true);
	//			fnDepClonerOutputMesh.findPlug( "ry" ).setLocked(true);
	//			fnDepClonerOutputMesh.findPlug( "rz" ).setLocked(true);

	//			fnDepClonerOutputMesh.findPlug( "sx" ).setLocked(true);
	//			fnDepClonerOutputMesh.findPlug( "sy" ).setLocked(true);
	//			fnDepClonerOutputMesh.findPlug( "sz" ).setLocked(true);


	//			// -------------------------------------------------------
	//			// Assign Same material as input mesh
	//			assignSameMaterial(p_currSelShapeA[0], o_clonerOutputMesh);
	//		}

	//		sel_list.clear();
	//		sel_list.add(dag_LocBTr.partialPathName());
	//		MGlobal::setActiveSelectionList(sel_list, MGlobal::kReplaceList);

	//	}
	//}

	//// --------------------------------------------------------------------------------------------------------------------------------------------
	//// Spline

	//else if ( argData.isFlagSet( "-sp" ) )
	//{


	//	o_clonerMultiNode = createNodeMaya(m_DEPNode,"clonerMulti");
	//	MFnDependencyNode fnDepCloner( o_clonerMultiNode );

	//	// rename the nodes
	//	fnDepCloner.setName( "clonerMulti_node#" );

	//	MGlobal::displayInfo(MString() + "[ClonerMulti] Node created: " + fnDepCloner.name() );

	//	MGlobal::displayInfo(MString() + "[ClonerMulti] Number of meshes: " + p_currSelShapeA.length() );
	//	MGlobal::displayInfo(MString() + "[ClonerMulti] Number of curves: " + p_currSelShapeA_curves.length() );


	//	// --------------------------------------------------------------------------------------------------------------------------------------------

	//}









	//// --------------------------------------------------------------------------------------------------------------------------------------------
	//// Default (just create the node by itself) if nothing is selected
	//if (selectedObjects.length() == 0)
	//{

	//	o_clonerMultiNode = createNodeMaya(m_DEPNode,"clonerMulti");
	//	MFnDependencyNode fnDepCloner( o_clonerMultiNode );

	//	// rename the nodes
	//	fnDepCloner.setName( "clonerMulti_node#" );

	////	//MGlobal::displayInfo(MString() + "[ClonerMulti] Node created: " + fnDepCloner.name() );

	////	// -----------------------------------------------------------------------------------------
	////	// create output mesh

	//	o_clonerOutputMesh = createNodeMaya(m_DEPNode,"mesh");
	//	MFnDependencyNode fnDepClonerOutputMesh( o_clonerOutputMesh );
	//	fnDepClonerOutputMesh.setName( "clonerMulti_mesh#" );
	////	// Find shape node of output mesh
	//	MDagPath dag_outMeshTr;
	//	MDagPath dag_outMeshShape;
	//	MDagPath dag_outMesh;
	//	MSelectionList sel_list;
	//	sel_list.clear();
	//	sel_list.add(fnDepClonerOutputMesh.name());
	//	sel_list.getDagPath(0,dag_outMesh);
	//	dag_outMeshTr = dag_outMesh;
	//	dag_outMesh.extendToShape();
	//	dag_outMeshShape = dag_outMesh;

	//	MFnDependencyNode fnDepClonerOutputMeshShape( dag_outMeshShape.node() );

	//	MPlug p_clonerMultiNode_outMesh = fnDepCloner.findPlug( "outMesh" );
	//	MPlug p_clonerMultiMesh_inMesh = fnDepClonerOutputMeshShape.findPlug( "inMesh" );

	////	MGlobal::displayInfo(MString() + "[ClonerMulti] connected: " + p_clonerMultiNode_outMesh.name() + " -> " + p_clonerMultiMesh_inMesh.name()  );

	//	status = m_DGMod.connect( p_clonerMultiNode_outMesh, p_clonerMultiMesh_inMesh );
	//	CHECK_MSTATUS_AND_RETURN_IT(status);
	//	m_DGMod.doIt();

	//	assignInitialShadingGroup(o_clonerOutputMesh);

	//	MGlobal::selectByName(fnDepCloner.name(), MGlobal::kReplaceList);



	//}



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


	status = m_DAGMod.undoIt();
	CHECK_MSTATUS_AND_RETURN_IT(status);



	for (int i = 0; i < o_outputObjectsA.length(); i++)
	{
		MDagModifier		m_DAGMod_bake;
		status = m_DAGMod_bake.deleteNode(o_outputObjectsA[i]);
		CHECK_MSTATUS_AND_RETURN_IT(status);
		status = m_DAGMod_bake.doIt();
		CHECK_MSTATUS_AND_RETURN_IT(status);
	}




	return MS::kSuccess;
}