//
//  ClonerCircle.cpp
//  ClonerMulti
//
//  Created by Hunyadi János on 2015. 01. 03..
//  Copyright (c) 2015. Janos Hunyadi. All rights reserved.
//



#include "CloneCircle.h"


// --- Instance Circle ------
MStatus ClonerMultiThread::instanceCircle()
{

    MStatus status;
    
    int f = 0;
    m_tr_matA.clear();
    m_tr_matA.setLength(m_numDup);
    
    float radius = m_offsetX;
    float slice = 2.0f * float(M_PI) / m_instanceX;
    

    
    for (int y = 0; y < m_instanceY; y++)
    {
        for (int x = 0; x < m_instanceX; x++)
        {
            
            for (int z = 0; z < m_instanceZ; z++)
            {
                
                float angle = slice * x;
                float newX = radius * cos(angle);
                float newY = radius * sin(angle);
                //double newZ = radius * sin(angle);

                
                // Translation
                MFloatVector v_baseOffV(newX, m_offsetY * y, m_offsetZ * newY );
                
                
                double rot[3] = {m_rotateX * 0.5f * ( M_PI / 180.0f ), (m_rotateY * 0.5f * ( M_PI / 180.0f )) -angle,  m_rotateZ * 0.5f * ( M_PI / 180.0f )};
                
                
                // Scale
                const double scaleV[3] = {  double(m_scaleX),  double(m_scaleY),  double(m_scaleZ) };
                
                // Matrix
                MTransformationMatrix tr_mat;
                
                status = tr_mat.setTranslation(v_baseOffV, MSpace::kObject);
                CHECK_MSTATUS_AND_RETURN_IT(status);
                
                status = tr_mat.setScale(scaleV, MSpace::kObject);
                CHECK_MSTATUS_AND_RETURN_IT(status);
                
                status = tr_mat.setRotation(rot, MTransformationMatrix::kXYZ);
                CHECK_MSTATUS_AND_RETURN_IT(status);
                

                
                m_tr_matA.set(tr_mat.asMatrix(), f);
                
                f += 1;
            }
            
            
        }
    }
    
    
    
    return MS::kSuccess;
    
}