#include "ClonerMulti.h"
#include "ClonerMultiCommand.h"
#include "ClonerMultiLoc.h"
#include "AETemplate.h"


#include <maya/MFnPlugin.h>




MStatus initializePlugin( MObject obj )
{
	MStatus status;

	MFnPlugin fnPlugin( obj, "Janos Hunyadi", "1.0", "Any" );

	MGlobal::executeCommand( mel_AETemplate() );

	status = fnPlugin.registerCommand( "clonerMultiCommand", ClonerMultiCommand::creator, ClonerMultiCommand::newSyntax );
	CHECK_MSTATUS_AND_RETURN_IT( status );

	status = fnPlugin.registerNode("clonerMulti",ClonerMultiThread::id, ClonerMultiThread::creator, ClonerMultiThread::initialize, MPxNode::kLocatorNode);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	// Locator
	status = fnPlugin.registerNode( "clonerMultiLoc", ClonerMultiLoc::id, &ClonerMultiLoc::creator, &ClonerMultiLoc::initialize, MPxNode::kLocatorNode, &ClonerMultiLoc::drawDbClassification);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	status = MHWRender::MDrawRegistry::registerDrawOverrideCreator( ClonerMultiLoc::drawDbClassification, ClonerMultiLoc::drawRegistrantId, ClonerMultiLocOverride::Creator);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	return MS::kSuccess;
}


MStatus uninitializePlugin( MObject obj )
{
	MStatus status;

	MFnPlugin fnPlugin( obj );


	status = fnPlugin.deregisterCommand( "clonerMultiCommand" );
	CHECK_MSTATUS_AND_RETURN_IT( status );


	status = fnPlugin.deregisterNode(ClonerMultiThread::id);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	status = fnPlugin.deregisterNode( ClonerMultiLoc::id );
	CHECK_MSTATUS_AND_RETURN_IT(status);


	return MS::kSuccess;
}

