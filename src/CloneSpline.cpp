//
//  CloneSpline.h
//  ClonerMulti
//
//  Created by Hunyadi János on 2015. 01. 03..
//  Copyright (c) 2015. Janos Hunyadi. All rights reserved.
//



#include "CloneSpline.h"


// --- Instance Spline ------
MStatus ClonerMultiThread::instanceSpline()
{ 


	MStatus status;

	if (!p_InCurve.isConnected())
	{
		return MStatus::kFailure;
	}


	m_tr_matA.clear();
	m_tr_matA.setLength(m_numDup);

	MVector m_firstUpVec(0.0,1.0,0.0);

	// m_firstUpVec *= m_curveTrMat;

	int dup = m_numDup;

	if (dup < 2)
	{
		dup = 2;
	}

	double m_length = 1.0;

	int f = 0;

	//
	MFnNurbsCurve curveFn(m_inCurve);

	double length = (curveFn.length() / double(dup-1)) * m_length;
	double m_offset = 0.0;

	MVector tan, norm, bi;
	MVector prevtan, prevNormal;

	MVector currentNormal = m_firstUpVec;
	currentNormal.normalize();

	if ( m_length <= 0 ) m_length = 0.0001;


	for(int i=0; i <= dup * int(m_length); i++){

		MPoint p;

		double step = double (dup * int(m_length)) - int (dup * int(m_length));
		double param = curveFn.findParamFromLength( i * length * (1.0/m_length) + (step * (length * (1.0/m_length))) + m_offsetX );

		curveFn.getPointAtParam(param, p, MSpace::kWorld );

		

		MVector tan = curveFn.tangent(param , MSpace::kWorld); // do calculations here
		MPoint outPos;
		curveFn.getPointAtParam(param , outPos , MSpace::kWorld);
		tan.normalize();


		MVector cross1 = currentNormal^tan;
		cross1.normalize() ;
		MVector cross2 =  tan^cross1;
		cross2.normalize();
		currentNormal = cross2 ;
		norm = cross2;



		MVector currN = curveFn.normal(param, MSpace::kWorld);
		currN.normalize();
		p += (currN * m_offsetY);

		double m[4][4] = {{tan.x, tan.y , tan.z, 0},
		{ cross1.x, cross1.y , cross1.z, 0},
		{ cross2.x, cross2.y , cross2.z, 0},
		{ p.x, p.y, p.z, 1.0}};
		MMatrix mat = m;


		// Transformation matrix
		MTransformationMatrix tr_mat(mat); 


		//// Translation
		//status = tr_mat.addTranslation(tan * m_offsetX, MSpace::kObject);
		//CHECK_MSTATUS_AND_RETURN_IT(status);

		//status = tr_mat.addTranslation(cross1 * m_offsetY, MSpace::kObject);
		//CHECK_MSTATUS_AND_RETURN_IT(status);

		//status = tr_mat.addTranslation(cross2 * m_offsetZ, MSpace::kObject);
		//CHECK_MSTATUS_AND_RETURN_IT(status);


		// Rotation
		double rotation[3] = {m_rotateX * 0.5f * ( M_PI / 180.0f ), m_rotateY * 0.5f * ( M_PI / 180.0f ),  m_rotateZ * 0.5f * ( M_PI / 180.0f )};


		// Scale
		const double scaleV[3] = {  double(m_scaleX),  double(m_scaleY),  double(m_scaleZ) };





		status = tr_mat.addRotation(rotation,  MTransformationMatrix::kXYZ, MSpace::kObject);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		status = tr_mat.addScale(scaleV, MSpace::kObject);
		CHECK_MSTATUS_AND_RETURN_IT(status);

		if (f < dup) // put everything back
		{
			m_tr_matA.set(tr_mat.asMatrix(), f);
		}



		f+=1;
	}

	return MS::kSuccess;

}


