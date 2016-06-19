#include "ClonerMulti.h"
//#include "ClonerMultiThreadCommand.h"
#include "AETemplate.h"


#include <maya/MFnPlugin.h>




MStatus initializePlugin( MObject obj )
{
	MStatus status;

	MFnPlugin fnPlugin( obj, "Janos Hunyadi", "1.0", "Any" );

	MGlobal::executeCommand( mel_AETemplate() );

	status = fnPlugin.registerCommand( "clonerMultiCommand", ClonerMultiThreadCommand::creator, ClonerMultiThreadCommand::newSyntax );
    CHECK_MSTATUS_AND_RETURN_IT( status );

	status = fnPlugin.registerNode("clonerMulti",ClonerMultiThread::id, ClonerMultiThread::creator, ClonerMultiThread::initialize);
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


	return MS::kSuccess;
}

