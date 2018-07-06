//
//  ClonerMultiLoc.cpp
//  ClonerMultiLoc
//
//  Created by Hunyadi Janos on 31/01/15.
//  Copyright (c) 2015 Janos Hunyadi. All rights reserved.
//

#include "ClonerMultiLoc.h"

// Plugin

MTypeId ClonerMultiLoc::id(0x00123947);

MObject ClonerMultiLoc::aLocatorType;
MObject ClonerMultiLoc::aLocNumber;
MObject ClonerMultiLoc::aShowNumber;


MString	ClonerMultiLoc::drawDbClassification("drawdb/geometry/ClonerMultiLoc");
MString	ClonerMultiLoc::drawRegistrantId("ClonerMultiLocPlugin");

ClonerMultiLoc::ClonerMultiLoc() {}
ClonerMultiLoc::~ClonerMultiLoc() {}

void ClonerMultiLoc::postConstructor()
{
	MFnDependencyNode nodeFn(thisMObject());
	nodeFn.setName("clonerMulti_locAShape#");

}

void* ClonerMultiLoc::creator() { return new ClonerMultiLoc(); }

ClonerMultiLocOverride::ClonerMultiLocOverride(const MObject& obj) : MHWRender::MPxDrawOverride(obj, ClonerMultiLocOverride::draw) {}
ClonerMultiLocOverride::~ClonerMultiLocOverride()
{






}

// ----------------------------------------------------------------------------------------------------------------------------------------------------

// VP 1.0 functions

MStatus ClonerMultiLoc::compute(const MPlug& plug, MDataBlock& data)
{

	return MS::kUnknownParameter;
}


void ClonerMultiLoc::draw(M3dView & view, const MDagPath & path, M3dView::DisplayStyle style, M3dView::DisplayStatus status)
{
	MObject thisNode = thisMObject();

	MFnDependencyNode fnDepCloner(thisNode);

	// Draw locator
	view.beginGL();

	glPushAttrib(GL_CURRENT_BIT);

	if (status == M3dView::kActive) {
		view.setDrawColor(13, M3dView::kActiveColors);
	}
	else {
		view.setDrawColor(13, M3dView::kDormantColors);
	}



	float r = 1.0f;

	int lats = 10;
	int longs = 10;

	for (int i = 0; i <= lats; i++) {
		double lat0 = M_PI * (-0.5 + (double)(i - 1) / lats);
		double z0 = sin(lat0);
		double zr0 = cos(lat0);
		z0 *= r * 0.5;
		zr0 *= r * 0.5;

		double lat1 = M_PI * (-0.5 + (double)i / lats);
		double z1 = sin(lat1);
		double zr1 = cos(lat1);
		z1 *= r * 0.5;
		zr1 *= r * 0.5;

		glBegin(GL_QUAD_STRIP);
		for (int j = 0; j <= longs; j++)
		{
			double lng = 2 * M_PI * (double)(j - 1) / longs;
			double x = cos(lng);
			double y = sin(lng);

			MPoint a(float(x) * float(zr0), float(y) * float(zr0), float(z0));
			MPoint b(float(x) * float(zr1), float(y) * float(zr1), float(z1));


			glVertex3f(float(a.x), float(a.y), float(a.z));
			glVertex3f(float(b.x), float(b.y), float(b.z));
		}
		glEnd();

	}





	// view.drawText( "Switch to VP 2.0", MPoint::origin, M3dView::kCenter );



	glPopAttrib();

	view.endGL();

}


bool ClonerMultiLoc::isBounded() const
{
	return true;
}

MBoundingBox ClonerMultiLoc::boundingBox() const
{

	MBoundingBox bbox;

	MFnDependencyNode fnDepLocNode(thisMObject());

	MPoint corner1(-0.5, 0.0, -0.5);
	MPoint corner2(0.5, 0.0, 0.5);

	return MBoundingBox(corner1, corner2);


}




// ----------------------------------------------------------------------------------------------------------------------------------------------------

// VP 2.0 Override functions

MHWRender::DrawAPI ClonerMultiLocOverride::supportedDrawAPIs() const
{

#if MAYA_API_VERSION > 201600

	return (MHWRender::kOpenGL | MHWRender::kDirectX11 | MHWRender::kOpenGLCoreProfile);

#else
	return (MHWRender::kOpenGL | MHWRender::kDirectX11);
#endif

}

bool ClonerMultiLocOverride::isBounded(const MDagPath& /*objPath*/, const MDagPath& /*cameraPath*/) const
{
	return true;
}

MBoundingBox ClonerMultiLocOverride::boundingBox(const MDagPath& objPath, const MDagPath& cameraPath) const
{

	MStatus status;
	MObject CLonerMultiNode = objPath.node(&status);

	MPoint corner1(-0.5, 0.0, -0.5);
	MPoint corner2(0.5, 0.0, 0.5);

	return MBoundingBox(corner1, corner2);

}


// Called by Maya each time the object needs to be drawn.
MUserData* ClonerMultiLocOverride::prepareForDraw(const MDagPath& objPath, const MDagPath& cameraPath, const MHWRender::MFrameContext& frameContext, MUserData* oldData)
{

	// Get outside data from plugs
	MStatus status;
	MObject o_cLonerMultiNode = objPath.node(&status);

	MPlug p;

	//

	// Add data
	ClonerMultiLocData* data = dynamic_cast<ClonerMultiLocData*>(oldData);
	if (!data)
	{
		data = new ClonerMultiLocData();
	}


	p = MPlug(o_cLonerMultiNode, ClonerMultiLoc::aLocatorType);
	p.getValue(data->m_locatorType);


	p = MPlug(o_cLonerMultiNode, ClonerMultiLoc::aLocNumber);
	p.getValue(data->m_locNumber);

	p = MPlug(o_cLonerMultiNode, ClonerMultiLoc::aShowNumber);
	p.getValue(data->m_showNumber);


	data->m_inLoc_mat = objPath.exclusiveMatrix();

	// get correct color based on the state of object, e.g. active or dormant
	data->m_locColor = MHWRender::MGeometryUtilities::wireframeColor(objPath);

	return data;
}


void ClonerMultiLocOverride::addUIDrawables(const MDagPath& path, MHWRender::MUIDrawManager& drawManager, const MHWRender::MFrameContext& frameContext, const MUserData* data)
{
	ClonerMultiLocData* pLocatorData = (ClonerMultiLocData*)data;
	if (!pLocatorData)
	{
		return;
	}

	//bool drawAsBoundingbox = false;

	//bool animPlay = MAnimControl::isPlaying();
	//bool animScrub = MAnimControl::isScrubbing();


	//// For any viewport interactions switch to bounding box mode,
	//// except when we are in playback. Uncomment to enable usage.
	//if (frameContext.inUserInteraction() || frameContext.userChangingViewContext())
	//{
	//	if (!animPlay && !animScrub)
	//	{
	//		drawAsBoundingbox = true;
	//	}
	//}



	M3dView view = M3dView::active3dView();
	short ox, oy;


	MColor fillCol, lineCol;

	fillCol = MColor(1.0f, 0.0f, 0.0f, 0.5f);
	lineCol = MColor(1.0f, 1.0f, 1.0f, 1.0f);

	if (pLocatorData->m_locatorType == 1)
	{

		fillCol = MColor(0.6f, 0.0f, 0.2f, 0.5f);
		lineCol = MColor(0.0f, 0.0f, 0.0f, 1.0f);
	}


	if (frameContext.getDisplayStyle() & MHWRender::MDrawContext::kWireFrame) {


		fillCol = MColor(pLocatorData->m_locColor.r, pLocatorData->m_locColor.g, pLocatorData->m_locColor.b, 0.0);

	}

	if (MHWRender::MGeometryUtilities::displayStatus(path) == M3dView::kLead) {


		fillCol = MColor(pLocatorData->m_locColor.r, pLocatorData->m_locColor.g, pLocatorData->m_locColor.b, fillCol.a);

	}

	if (MHWRender::MGeometryUtilities::displayStatus(path) == M3dView::kActive) {

		fillCol = MColor(pLocatorData->m_locColor.r, pLocatorData->m_locColor.g, pLocatorData->m_locColor.b, fillCol.a);

	}

	if (MHWRender::MGeometryUtilities::displayStatus(path) == M3dView::kTemplate) {

		fillCol = MColor(pLocatorData->m_locColor.r, pLocatorData->m_locColor.g, pLocatorData->m_locColor.b, fillCol.a);

	}


	MPoint p = MPoint::origin;
	MPoint camP = MPoint::origin * path.inclusiveMatrix();
	double cam_d = camP.distanceTo(p);


	double dist = 1.0;


	drawManager.beginDrawable();



	if (pLocatorData->m_locatorType == 0)
	{



		p *= pLocatorData->m_inLoc_mat;
		view.worldToView(p, ox, oy);


		drawManager.setColor(MColor(fillCol.r, fillCol.g, fillCol.b, 1.0f));
		drawManager.circle2d(MPoint(ox, oy), 3.0, true);

		drawManager.setColor(MColor(lineCol.r, lineCol.g, lineCol.b, 1.0f));
		drawManager.circle2d(MPoint(ox, oy), 4.0, false);

		if (pLocatorData->m_showNumber)
		{
			drawManager.setColor(MColor(0.0, 0.0, 0.0, 1.0f));
			drawManager.text2d(MPoint(ox, oy + 7), MString() + pLocatorData->m_locNumber, MHWRender::MUIDrawManager::kCenter);

			drawManager.setColor(MColor(1.0, 1.0, 1.0, 1.0f));
			drawManager.text2d(MPoint(ox, oy + 10), MString() + pLocatorData->m_locNumber, MHWRender::MUIDrawManager::kCenter);
		}
	}

	if (pLocatorData->m_locatorType == 1)
	{

		drawManager.setColor(MColor(fillCol.r, fillCol.g, fillCol.b, 0.2f));
		drawManager.cone(p + MVector(0.0, 0.25, 0.0), MVector(0.0, -1.0, 0.0), 0.05, 0.25, true);

		drawManager.setColor(MColor(lineCol.r, lineCol.g, lineCol.b, 1.0f));
		drawManager.cone(p + MVector(0.0, 0.25, 0.0), MVector(0.0, -1.0, 0.0), 0.05, 0.25, false);

	}






	drawManager.endDrawable();
}

MStatus ClonerMultiLoc::initialize()
{
	MFnMatrixAttribute		mAttr;
	MFnNumericAttribute		nAttr;
	MFnEnumAttribute		eAttr;
	MStatus					status;

	// Instance
	ClonerMultiLoc::aLocatorType = eAttr.create("locatorType", "locatorType", 0);
	eAttr.addField("Point", 0);
	eAttr.addField("Direction", 1);
	eAttr.setStorable(true);
	addAttribute(ClonerMultiLoc::aLocatorType);

	ClonerMultiLoc::aShowNumber = nAttr.create("showNumber", "showNumber", MFnNumericData::kBoolean);
	nAttr.setStorable(true);
	nAttr.setDefault(true);
	nAttr.setKeyable(true);
	nAttr.setChannelBox(true);
	addAttribute(ClonerMultiLoc::aShowNumber);

	ClonerMultiLoc::aLocNumber = nAttr.create("locNumber", "locNumber", MFnNumericData::kInt);
	nAttr.setStorable(true);
	nAttr.setDefault(1);
	nAttr.setMin(0);
	nAttr.setKeyable(true);
	nAttr.setChannelBox(true);
	addAttribute(ClonerMultiLoc::aLocNumber);

	return MS::kSuccess;
}
