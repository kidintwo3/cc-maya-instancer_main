//
//  ClonerMesh.h
//  ClonerMulti
//
//  Created by Hunyadi János on 2015. 01. 03..
//  Copyright (c) 2015. Janos Hunyadi. All rights reserved.
//

#ifndef CLONEMESH_H
#define CLONEMESH_H


#include "ClonerMulti.h"

MStatus overrideInstanceOnMeshSettings();
MStatus instanceOnMesh();
void lerp(MPoint& p1, MPoint& p2, double blend, MPoint& outPoint);

#endif
