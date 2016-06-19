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
    
    m_tr_matA.clear();
    m_tr_matA.setLength(m_numDup);
    
    MVector currentNormal = m_firstUpVec;
    currentNormal.normalize();
    
    MVector prevNormal;
    MMatrix rotMatrix;
    
    
    // curve data
    MFnNurbsCurve curveFn(m_inCurve, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);
    
    
    double length_section = (curveFn.length() / double(m_numDup) );
    double length_curve = curveFn.length();
    
    double offsetV = m_offsetZ;
    
    for(int i=0; i < m_numDup; i++)
    {
        
        double tempLength = double(i) * length_section;
        tempLength += offsetV;
        
        double  timesLength = ( offsetV / length_curve);
        
        int  timesLengthFloor = 0;
        
        if (offsetV > 0 )
        {
            timesLengthFloor = int(floor(timesLength));
        }
        
        else
        {
            timesLengthFloor = int(ceil(timesLength));
        }
        
        offsetV -= (length_curve*double(timesLengthFloor));
        
        
        
        
        
        
        
        if (tempLength < 0)
        {
            
            tempLength = length_curve + tempLength;
            
            
        }
        
        else if (tempLength > length_curve )
        {
            tempLength -= length_curve;
            
            
        }
        
        
        
        // Calculate matrix
        MPoint p;
        
        double param = curveFn.findParamFromLength( tempLength  );
        
        curveFn.getPointAtParam(param, p, MSpace::kWorld );
        
        
        MVector tan = curveFn.tangent(param , MSpace::kObject);
        tan.normalize();
        
        MVector cross1 = currentNormal^tan;
        cross1.normalize() ;
        
        MVector cross2 =  tan^cross1;
        cross2.normalize();
        currentNormal = cross2;
        
        
        double m[4][4] = {{tan.x, tan.y , tan.z, 0.0},
            { cross1.x, cross1.y , cross1.z, 0.0},
            {cross2.x, cross2.y , cross2.z, 0.0},
            {p.x, p.y, p.z, 1.0}};
        
        rotMatrix = m;
        
        
        m_tr_matA.set(rotMatrix, i);
        
    }
    
    
    
    return MS::kSuccess;
    
}


