#include "ClonerMulti.h"
#include "ClonerMultiCommand.h"
#include "ClonerMultiLoc.h"
#include "AETemplate.h"
#include "icons.h"


#include <maya/MFnPlugin.h>
#include <maya/MCommonSystemUtils.h>




MStatus initializePlugin( MObject obj )
{
	MStatus status;

	MFnPlugin fnPlugin( obj, "Creative Case", "1.6", "Any" );


	//

	MString rebuild_icons = MCommonSystemUtils::getEnv("CLONERMULTI_REBUILD_ICONS", &status);

	if( !rebuild_icons.asShort() )
	{
		icons_data_write();
	}

	MString rebuild_shelf = MCommonSystemUtils::getEnv("CLONERMULTI_REBUILD_SHELF", &status);

	if( !rebuild_shelf.asShort() )
	{
		MGlobal::executeCommand( mel_createShelf() );
	}

	MStringArray aeTemplateA = mel_AETemplate();

	for (int i = 0; i < aeTemplateA.length(); i++)
	{
		MGlobal::executeCommand(aeTemplateA[i]);
	}

	status = fnPlugin.registerCommand( "clonerMultiCommand", ClonerMultiCommand::creator, ClonerMultiCommand::newSyntax );
	CHECK_MSTATUS_AND_RETURN_IT( status );

	status = fnPlugin.registerNode("clonerMulti",ClonerMultiThread::id, ClonerMultiThread::creator, ClonerMultiThread::initialize, MPxNode::kLocatorNode, &ClonerMultiThread::drawDbClassification);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	status = MHWRender::MDrawRegistry::registerDrawOverrideCreator( ClonerMultiThread::drawDbClassification, ClonerMultiThread::drawRegistrantId, ClonerMultiThreadOverride::Creator);
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


	status = MHWRender::MDrawRegistry::deregisterDrawOverrideCreator( ClonerMultiThread::drawDbClassification, ClonerMultiThread::drawRegistrantId);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	status = MHWRender::MDrawRegistry::deregisterDrawOverrideCreator( ClonerMultiLoc::drawDbClassification, ClonerMultiLoc::drawRegistrantId);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	status = fnPlugin.deregisterNode( ClonerMultiLoc::id );
	CHECK_MSTATUS_AND_RETURN_IT(status);


	return MS::kSuccess;
}

