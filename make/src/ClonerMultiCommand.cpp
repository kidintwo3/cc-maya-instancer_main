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
	syntax.addFlag( "-ab", "-atob");
	syntax.addFlag( "-ac", "-aconn");
	syntax.addFlag( "-bc", "-bconn");
	syntax.addFlag( "-s", "-shift", MSyntax::kString);
	syntax.addFlag( "-r", "-reverse", MSyntax::kString);
	syntax.addFlag( "-b", "-bake");
	syntax.addFlag( "-no", "-node", MSyntax::kString  );

	// syntax.setObjectType( MSyntax::kSelectionList, 1, 1 );

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


	// Shift string
	if (argData.isFlagSet("-s"))
	{
		MString stringDataA;
		argData.getFlagArgument("-s", 0, stringDataA);

		MStringArray stringDataAArray;
		status = stringDataA.split(',', stringDataAArray);
		CHECK_MSTATUS_AND_RETURN_IT(status);


		// put user input array into an mIntarray
		MIntArray connectArrayA;
		for(int i = 0; i < stringDataAArray.length(); i++ ){ connectArrayA.append(stringDataAArray[i].asInt()); }

		int size = connectArrayA.length();

		int temp;

		for (int i=0; i<(size -1); i++)
		{
			temp = connectArrayA[size-1];
			connectArrayA[size-1] = connectArrayA[i];
			connectArrayA[i] = temp;
		}


		MString outputString;

		for (int i = 0; i < connectArrayA.length(); i++)
		{
			//MGlobal::displayInfo(MString() + connpA[i]);

			outputString += MString() + connectArrayA[i];

			if (i < connectArrayA.length()-1)
			{
				outputString += MString() + ",";
			}

		}

		MPxCommand::setResult(outputString);


		return MS::kSuccess;
	}


	// Reverse string
	if (argData.isFlagSet("-r"))
	{
		MString stringDataA;
		argData.getFlagArgument("-r", 0, stringDataA);

		MStringArray stringDataAArray;
		status = stringDataA.split(',', stringDataAArray);
		CHECK_MSTATUS_AND_RETURN_IT(status);


		// put user input array into an mIntarray
		MIntArray connectArrayA;
		for(int i = 0; i < stringDataAArray.length(); i++ ){ connectArrayA.append(stringDataAArray[i].asInt()); }

		int size = connectArrayA.length();

		int temp, i;

		for (i = 0; i < size/2; ++i) {
			temp = connectArrayA[size-i-1];
			connectArrayA[size-i-1] = connectArrayA[i];
			connectArrayA[i] = temp;
		}


		MString outputString;

		for (int i = 0; i < connectArrayA.length(); i++)
		{
			//MGlobal::displayInfo(MString() + connpA[i]);

			outputString += MString() + connectArrayA[i];

			if (i < connectArrayA.length()-1)
			{
				outputString += MString() + ",";
			}

		}

		MPxCommand::setResult(outputString);


		return MS::kSuccess;
	}


	// Set A connection
	if (argData.isFlagSet("-ac"))
	{

		MItSelectionList iter(selectedObjects);

		MObject component;
		MDagPath m_pathBaseMeshShape;
		iter.getDagPath(m_pathBaseMeshShape, component);

		if (component.apiType() ==  MFn::kMeshPolygonComponent )
		{
			MGlobal::displayWarning(MString() + "Select verts or edges");
			return MS::kFailure;
		}



		if (component.apiType() ==  MFn::kMeshEdgeComponent )
		{
			MGlobal::displayInfo(MString() + "Converting edges to Verts" );
			MItMeshEdge eIter(m_pathBaseMeshShape, component, &status);
			MItMeshVertex pIter(m_pathBaseMeshShape);

			if(status != MS::kSuccess)
			{
				return MS::kFailure;
			}


			MIntArray connpA;

			MIntArray vertIdA;


			int prevIndex;

			for ( eIter.reset() ; !eIter.isDone() ; eIter.next() )
			{
				vertIdA.append(eIter.index(0));
				vertIdA.append(eIter.index(1));
			}




			int vertId = -1;

			for (int i = 0; i < vertIdA.length(); i++)
			{
				pIter.setIndex(vertIdA[i], prevIndex);
				MIntArray connVerts;
				pIter.getConnectedVertices(connVerts);

				int connCount = 0;

				for (int x = 0; x < connVerts.length(); x++)
				{

					for (int z = 0; z < vertIdA.length(); z++)
					{
						if (vertIdA[z] == connVerts[x])
						{
							connCount += 1;
							break;
						}
					}

				}

				if (connCount == 1)
				{
					vertId = vertIdA[i];
					break;
				}

			}

			if (vertId == -1)
			{

				vertId = vertIdA[0];
				//MGlobal::displayInfo(MString() + "First: " + vertId);
			}




			MIntArray vertIdA_sub = vertIdA;

			if (vertId != -1)
			{

				connpA.append(vertId);

				for (int i = 0; i < vertIdA_sub.length(); i++)
				{
					if (vertIdA_sub[i] == vertId)
					{
						vertIdA_sub.remove(i);
						break;
					}
				}

				pIter.reset();

				for (int i = 0; i < vertIdA.length(); i++)
				{
					pIter.setIndex(vertId, prevIndex);
					MIntArray connVerts;
					pIter.getConnectedVertices(connVerts);


					int connCount = -1;

					for (int x = 0; x < connVerts.length(); x++)
					{



						for (int z = 0; z < vertIdA_sub.length(); z++)
						{
							if (connVerts[x] == vertIdA_sub[z])
							{
								connCount = vertIdA_sub[z];
								vertId = vertIdA_sub[z];;
								vertIdA_sub.remove(z);
								break;
							}
						}

						if (connCount != -1)
						{



							bool ff = false;

							for (int p = 0; p < connpA.length(); p++)
							{
								if (vertId == connpA[p])
								{
									ff = true;
									break;
								}
							}

							if (!ff)
							{
								connpA.append(vertId);
							}


							break;
						}

					}
				}




				//for (int i = 0; i < connA.length(); i++)
				//{
				//	MGlobal::displayInfo(MString() + connA[i]);
				//}



				//	if (connCount == 1)
				//	{
				//		vertId = vertIdA[i];

				//		MGlobal::displayInfo(MString() + vertId);

				//		//bool ff = false;

				//		//for (int v = 0; v < connA.length(); v++)
				//		//{
				//		//	if (connA[v] == vertId)
				//		//	{
				//		//		ff = true;
				//		//		break;
				//		//	}
				//		//}

				//		//if (!ff)
				//		//{
				//		//	connA.append(vertId);
				//		//}

				//		
				//	}

				//}


				//for (int i = 0; i < connA.length(); i++)
				//{
				//	MGlobal::displayInfo(MString() + connA[i]);
				//}


				// MGlobal::displayInfo(MString() + vertId);

				/*
				eIter.setIndex(edgeId,prevIndex);
				MIntArray connEdges;
				eIter.getConnectedEdges(connEdges);

				for (int x = 0; x < connEdges.length(); x++)
				{
				for (int z = 0; z < edgeIdA.length(); z++)
				{
				if (connEdges[x] == edgeIdA[z])
				{

				edgeIdA.remove(z);
				MGlobal::displayInfo(MString() + edgeIdA[z]);
				break;
				}
				}


				}*/


			}


			//eIter.setIndex(edgeId,prevIndex);
			//		int firstIndex = eIter.index(0);


			//		MItMeshVertex pIter(m_pathBaseMeshShape);
			//		pIter.setIndex(firstIndex, prevIndex);

			//		connpA.append(firstIndex);


			//		MIntArray connEdges;
			//		pIter.getConnectedEdges(connEdges);


			//		bool ff = false;

			//		for (int i = 0; i < edgeIdA.length(); i++)
			//		{


			//			for (int x = 0; x < connEdges.length(); x++)
			//			{

			//				if (connEdges[x] == edgeIdA[i])
			//				{
			//					ff=true;
			//					break;
			//				}

			//			}


			//			if (ff)
			//			{
			//				int vertexId;
			//				status = pIter.getOppositeVertex(vertexId, edgeIdA[i]);

			//				if (status)
			//				{
			//					connpA.append(vertexId);
			//				}

			//				edgeIdA.remove(i);
			//				break;
			//			}

			//		}





			//MIntArray vertIDArray;

			//for ( eIter.reset() ; !eIter.isDone() ; eIter.next() )
			//{
			//	//int lastInd = eIter.index(0);
			//	vertIDArray.append( eIter.index(0));
			//	vertIDArray.append( eIter.index(1));

			//}

			//MFnComponentListData fnComponentList;
			//MObject componentData = fnComponentList.create();

			//MFnSingleIndexedComponent fnSingleComponent;
			//component = fnSingleComponent.create( MFn::kMeshVertComponent );
			//fnSingleComponent.addElements( vertIDArray );
			//fnComponentList.add( component );

			//MItMeshVertex pIter(m_pathBaseMeshShape, component, &status);

			//if(status != MS::kSuccess)
			//{
			//	return MS::kFailure;
			//}


			////

			//MIntArray oldPIndexA;

			//for (pIter.reset() ; !pIter.isDone(); pIter.next() )
			//{
			//	oldPIndexA.append(pIter.index());
			//}

			//// 

			//MIntArray connpA;
			//int currIndex = oldPIndexA[0];
			//int connIndex;
			//int previndex;

			//pIter.reset();

			//for (unsigned int z = 0; z < oldPIndexA.length(); z++)
			//{
			//	MIntArray connVertsA;
			//	pIter.setIndex(currIndex,previndex);

			//	pIter.getOppositeVertex(

			//	//pIter.getConnectedVertices(connVertsA);

			//	//bool ff = false;

			//	//for (unsigned int i = 0; i < connVertsA.length(); i++)
			//	//{

			//	//	for (unsigned int p = 0; p < oldPIndexA.length(); p++)
			//	//	{
			//	//		if (connVertsA[i] == oldPIndexA[p] )
			//	//		{
			//	//			connIndex = connVertsA[i];
			//	//			currIndex = connIndex;

			//	//			connpA.append(connIndex);
			//	//			ff=true;
			//	//			break;
			//	//		}

			//	//		if (ff)
			//	//		{
			//	//			break;
			//	//		}
			//	//	}

			//	//	if (ff)
			//	//	{
			//	//		break;
			//	//	}

			//	//}
			//}


			/*MGlobal::displayInfo(MString() + "---------");*/

			MString outputString;

			for (int i = 0; i < connpA.length(); i++)
			{
				//MGlobal::displayInfo(MString() + connpA[i]);

				outputString += MString() + connpA[i];

				if (i < connpA.length()-1)
				{
					outputString += MString() + ",";
				}

			}

			MPxCommand::setResult(outputString);

		}

		return::MStatus::kSuccess;

	}

	// Set B connection
	if (argData.isFlagSet("-bc"))
	{

		return::MStatus::kSuccess;
	}









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

	MString clonerMulti_node_name("clonerMulti_node#");
	MString clonerMulti_mesh_name("clonerMulti_mesh#");
	MString clonerMulti_locA_name("clonerMulti_locA#");
	MString clonerMulti_locB_name("clonerMulti_locB#");



	// Setting the name
	if ( argData.isFlagSet( "-na" ) )
	{
		MString customName;
		argData.getFlagArgument("-na", 0, customName);

		clonerMulti_node_name = MString() + customName + "_node#";
		clonerMulti_mesh_name = MString() + clonerMulti_node_name + "_mesh#";
	}




	// Default node creation
	if ( !argData.isFlagSet( "-no" ) )
	{

		// Create clonerMulti node
		o_clonerMultiNode = m_DAGMod.createNode(MTypeId(0x00123946), MObject::kNullObj, &status);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		status = m_DAGMod.doIt();
		CHECK_MSTATUS_AND_RETURN_IT(status);

		MFnDependencyNode fnDepCloner( o_clonerMultiNode );
		fnDepCloner.setName( clonerMulti_node_name  );

		// Create clonerMulti output mesh
		o_clonerOutputMesh = m_DAGMod.createNode("mesh", MObject::kNullObj, &status);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		status = m_DAGMod.doIt();
		CHECK_MSTATUS_AND_RETURN_IT(status);

		MFnDependencyNode fnDepClonerOutputMesh( o_clonerOutputMesh );
		fnDepClonerOutputMesh.setName( clonerMulti_mesh_name  );

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
		MFnDependencyNode fnDepClonerOutputMeshTransform( dag_outMeshTr.node() );
		MFnDependencyNode fnDepClonerNodeShape( dag_clonerNodeShape.node() );
		MFnDependencyNode fnDepClonerNodeTransform( dag_clonerNodeTr.node() );

		fnDepClonerOutputMeshShape.setName( clonerMulti_mesh_name + "Shape#" );

		// Mesh plugs
		MPlug p_clonerMultiMesh_inMesh = fnDepClonerOutputMeshShape.findPlug( "inMesh", &status );
		CHECK_MSTATUS_AND_RETURN_IT(status);

		MPlug p_clonerMultiMesh_translate = fnDepClonerOutputMeshTransform.findPlug( "translate", &status );
		CHECK_MSTATUS_AND_RETURN_IT(status);

		MPlug p_clonerMultiMesh_rotate = fnDepClonerOutputMeshTransform.findPlug( "rotate", &status );
		CHECK_MSTATUS_AND_RETURN_IT(status);

		MPlug p_clonerMultiMesh_scale = fnDepClonerOutputMeshTransform.findPlug( "scale", &status );
		CHECK_MSTATUS_AND_RETURN_IT(status);



		// Node plugs

		MPlug p_clonerMultiNode_translate = fnDepClonerNodeTransform.findPlug( "translate", &status );
		CHECK_MSTATUS_AND_RETURN_IT(status);

		MPlug p_clonerMultiNode_rotate = fnDepClonerNodeTransform.findPlug( "rotate", &status );
		CHECK_MSTATUS_AND_RETURN_IT(status);

		MPlug p_clonerMultiNode_scale = fnDepClonerNodeTransform.findPlug( "scale", &status );
		CHECK_MSTATUS_AND_RETURN_IT(status);

		MPlug p_clonerMultiNode_outMesh = fnDepClonerNodeShape.findPlug( "outMesh", &status );
		CHECK_MSTATUS_AND_RETURN_IT(status);

		MPlug p_clonerMultiNode_inMesh = fnDepClonerNodeShape.findPlug( "inMesh", &status );
		CHECK_MSTATUS_AND_RETURN_IT(status);

		MPlug p_clonerMultiNode_inCurve = fnDepClonerNodeShape.findPlug( "inCurve", &status );
		CHECK_MSTATUS_AND_RETURN_IT(status);

		status = m_DAGMod.connect( p_clonerMultiNode_outMesh, p_clonerMultiMesh_inMesh );
		CHECK_MSTATUS_AND_RETURN_IT(status);

		status = m_DAGMod.connect( p_clonerMultiNode_translate, p_clonerMultiMesh_translate );
		CHECK_MSTATUS_AND_RETURN_IT(status);

		status = m_DAGMod.connect( p_clonerMultiNode_rotate, p_clonerMultiMesh_rotate );
		CHECK_MSTATUS_AND_RETURN_IT(status);

		status = m_DAGMod.connect( p_clonerMultiNode_scale, p_clonerMultiMesh_scale );
		CHECK_MSTATUS_AND_RETURN_IT(status);

		status = m_DAGMod.doIt();
		CHECK_MSTATUS_AND_RETURN_IT(status);


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

			status = m_DAGMod.doIt();
			CHECK_MSTATUS_AND_RETURN_IT(status);



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

				status = m_DAGMod.doIt();
				CHECK_MSTATUS_AND_RETURN_IT(status);

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

		status = m_DAGMod.doIt();
		CHECK_MSTATUS_AND_RETURN_IT(status);

		return::MStatus::kSuccess;
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

					status = m_DAGMod.doIt();
					CHECK_MSTATUS_AND_RETURN_IT(status);

					// Create clonerMulti locator B
					o_locB = m_DAGMod.createNode(MTypeId(0x00123947));
					MFnDependencyNode fnDepLocB( o_locB );
					fnDepLocB.setName( clonerMulti_locB_name );

					status = m_DAGMod.doIt();
					CHECK_MSTATUS_AND_RETURN_IT(status);



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
					status = m_DAGMod.doIt();
					CHECK_MSTATUS_AND_RETURN_IT(status);

					// Connect Loc B
					status = m_DAGMod.connect( p_locB_wm, inLocator_B_plug );
					CHECK_MSTATUS_AND_RETURN_IT(status);
					status = m_DAGMod.doIt();
					CHECK_MSTATUS_AND_RETURN_IT(status);


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


			status = m_DAGMod.doIt();
			CHECK_MSTATUS_AND_RETURN_IT(status);

			return::MStatus::kSuccess;
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

				status = idArrayData.copyTo(out_idArray);
				CHECK_MSTATUS_AND_RETURN_IT(status);

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


#if MAYA_API_VERSION > 201600
				MFnMatrixArrayData worldMatrixData( matrixObject );
				MMatrixArray out_matrixArray;

				status = worldMatrixData.copyTo(out_matrixArray);
				CHECK_MSTATUS_AND_RETURN_IT(status);



				if (out_idArray.length() == out_matrixArray.length())
				{




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


					// Create group for output meshes
					MObject o_groupNode = m_DAGMod.createNode("transform", MObject::kNullObj, &status);
					CHECK_MSTATUS_AND_RETURN_IT(status);

					status = m_DAGMod.doIt();
					CHECK_MSTATUS_AND_RETURN_IT(status);


					MFnDependencyNode fnDep_group( o_groupNode );
					fnDep_group.setName( "clonerMulti_instance#"  );

					if(status)
					{

						for (int i = 0; i < o_outputObjectsA.length(); i++)
						{
							m_DAGMod.reparentNode(o_outputObjectsA[i], o_groupNode);
						}
					}

					o_outputObjectsA.append(o_groupNode);

				}
#endif


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


				//// Delete output mesh
				//status = m_DAGMod.deleteNode(parent_depN.object());
				//CHECK_MSTATUS_AND_RETURN_IT(status);

				//// Delete cloner node
				//status = m_DAGMod.deleteNode(fnDepCloner.object());
				//CHECK_MSTATUS_AND_RETURN_IT(status);

				status = m_DAGMod.doIt();
				CHECK_MSTATUS_AND_RETURN_IT(status);


				MSelectionList currSel_list;

				for (int i = 0; i < o_outputObjectsA.length(); i++)
				{
					currSel_list.add(o_outputObjectsA[i]);
				}

				MGlobal::setActiveSelectionList(currSel_list);

			}

		}

		return::MStatus::kSuccess;

	}

	else
	{
		MGlobal::displayError(MString() + "[ClonerMulti] No/wrong ClonerMulti name set for command (use the -cm flag to set)");
		return MStatus::kFailure;
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


	status = m_DAGMod.undoIt();
	CHECK_MSTATUS_AND_RETURN_IT(status);



	for (int i = 0; i < o_outputObjectsA.length(); i++)
	{
		MDagModifier m_DAGMod_bake;

		status = m_DAGMod_bake.deleteNode(o_outputObjectsA[i]);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		status = m_DAGMod_bake.doIt();
		CHECK_MSTATUS_AND_RETURN_IT(status);
	}




	return MS::kSuccess;
}