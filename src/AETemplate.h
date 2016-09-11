//
//  AETemplates.h
//  clonerMulti
//
//  Created by Hunyadi J�nos on 2015. 01. 03..
//  Copyright (c) 2015. Janos Hunyadi. All rights reserved.
//

#ifndef MELSCRIPTS_H
#define MELSCRIPTS_H

#include <maya/MString.h>


MString mel_AETemplate()
{
	MString s_aeTemplate = MString() + 
		"global proc AEclonerMultiTemplate( string $nodeName )\r\n"
		"{\r\n"
		"	editorTemplate -beginScrollLayout;\r\n"
		"	\r\n"
		"	editorTemplate -beginLayout \"ClonerMulti instance settings\" -collapse 0;\r\n"
		"	editorTemplate -addControl \"instanceType\";\r\n"
		"	editorTemplate -addControl \"patternType\";\r\n"
		"	editorTemplate -addControl \"scatterType\";\r\n"
		"	editorTemplate -addSeparator;\r\n"
		"	editorTemplate -addControl \"id\";\r\n"
		"	editorTemplate -addSeparator;\r\n"
		"	editorTemplate -addControl \"reversePattern\";\r\n"
		"	editorTemplate -label \"Lock Output Mesh\" -addControl \"outputMeshDisplayOverride\";\r\n"
		"	editorTemplate -label \"Proxy Display\" -addControl \"displayProxy\";\r\n"
		"	editorTemplate -endLayout;\r\n"
		"	\r\n"
		"	editorTemplate -beginLayout \"Random Pattern Settings\" -collapse 1;	\r\n"
		"	editorTemplate -label \"Pattern Seed\" -addControl \"randomSeed\";\r\n"
		"	editorTemplate -addSeparator;\r\n"
		"	editorTemplate -label \"Gap Amount\" -addControl \"randomIDGapLevel\";\r\n"
		"	editorTemplate -addSeparator;\r\n"
		"	editorTemplate -endLayout;\r\n"
		"	editorTemplate -beginLayout \"Instance count\" -collapse 0;	\r\n"
		"	editorTemplate -addControl \"instanceX\";\r\n"
		"	editorTemplate -addControl \"instanceY\";\r\n"
		"	editorTemplate -addControl \"instanceZ\";\r\n"
		"	editorTemplate -endLayout;\r\n"
		"	// -------------------------------------------------------------\r\n"
		"	editorTemplate -beginLayout \"Input objects\" -collapse 0;	\r\n"
		"	editorTemplate -callCustom \"AE_cm_objList_create\" \"AE_cm_objList_edit\" \"\";\r\n"
		"	editorTemplate -callCustom \"AE_cm_abLoc_create\" \"AE_cm_abLoc_edit\" \"\";\r\n"
		"	editorTemplate -addSeparator;\r\n"
		"	editorTemplate -addControl \"mergeInputMeshes\";\r\n"
		"	editorTemplate -endLayout;\r\n"
		"	// -------------------------------------------------------------\r\n"
		"	editorTemplate -beginLayout \"Primary Transform\" -collapse 0;\r\n"
		"	\r\n"
		"	editorTemplate -label \"Up Vector\" -addControl \"firstUpVec\";\r\n"
		"	editorTemplate -addSeparator;\r\n"
		"	editorTemplate -label \"World Space Input\" -addControl \"worldSpace\";\r\n"
		"	editorTemplate -label \"Loop Curve Offset\" -addControl \"loopOffset\";\r\n"
		"	editorTemplate -addSeparator;\r\n"
		"	\r\n"
		"	editorTemplate -addControl \"offsetX\";\r\n"
		"	editorTemplate -addControl \"offsetY\";\r\n"
		"	editorTemplate -addControl \"offsetZ\";\r\n"
		"	editorTemplate -addSeparator;\r\n"
		"	\r\n"
		"	editorTemplate -addControl \"rotateX\";\r\n"
		"	editorTemplate -addControl \"rotateY\";\r\n"
		"	editorTemplate -addControl \"rotateZ\";\r\n"
		"	editorTemplate -addSeparator;\r\n"
		"	\r\n"
		"	editorTemplate -addControl \"scaleX\";\r\n"
		"	editorTemplate -addControl \"scaleY\";\r\n"
		"	editorTemplate -addControl \"scaleZ\";\r\n"
		"	editorTemplate -addSeparator;\r\n"
		"	\r\n"
		"	editorTemplate -label \"Snap Orientation\" -addControl \"orientationType\";\r\n"
		"	editorTemplate -addSeparator;\r\n"
		"	\r\n"
		"	editorTemplate -endLayout;\r\n"
		"	\r\n"
		"	editorTemplate -beginLayout \"Randomize Transform\" -collapse 0;\r\n"
		"	editorTemplate -label \"Random Offset X\" -addControl \"randomizeOffsetX\";\r\n"
		"	editorTemplate -label \"Random Offset Y\" -addControl \"randomizeOffsetY\";\r\n"
		"	editorTemplate -label \"Random Offset Z\" -addControl \"randomizeOffsetZ\";\r\n"
		"	editorTemplate -addSeparator;\r\n"
		"	editorTemplate -label \"Random Rotate X\" -addControl \"randomizeRotateX\";\r\n"
		"	editorTemplate -label \"Random Rotate Y\" -addControl \"randomizeRotateY\";\r\n"
		"	editorTemplate -label \"Random Rotate Z\" -addControl \"randomizeRotateZ\";\r\n"
		"	editorTemplate -addSeparator;\r\n"
		"	editorTemplate -label \"Random Scale X\" -addControl \"randomizeScaleX\";\r\n"
		"	editorTemplate -label \"Random Scale Y\" -addControl \"randomizeScaleY\";\r\n"
		"	editorTemplate -label \"Random Scale Z\" -addControl \"randomizeScaleZ\";\r\n"
		"	editorTemplate -addSeparator;\r\n"
		"	editorTemplate -endLayout;\r\n"
		"	// -------------------------------------------------------------\r\n"
		"	editorTemplate -beginLayout \"Snap vert pairs\" -collapse 0;\r\n"
		"	editorTemplate -addControl \"connectPieces\";\r\n"
		"	editorTemplate -addControl \"interpolate\";\r\n"
		"	editorTemplate -addControl \"connectLoop\";\r\n"
		"	editorTemplate -addSeparator;\r\n"
		"    editorTemplate -addControl \"connectArrayA\";\r\n"
		"	editorTemplate -addControl \"connectArrayB\";\r\n"
		"	editorTemplate -addSeparator;\r\n"
		"	editorTemplate -callCustom \"AE_cm_setArrays_create\" \"AE_cm_setArrays_edit\" \"\";\r\n"
		"	editorTemplate -endLayout;\r\n"
		"	// -------------------------------------------------------------\r\n"
		"	editorTemplate -beginLayout \"UV Settings\" -collapse 0;\r\n"
		"	editorTemplate -callCustom \"AE_cm_refreshUVs_create\" \"AE_cm_refreshUVs_edit\" \"\";\r\n"
		"	editorTemplate -addSeparator;\r\n"
		"	editorTemplate -addControl \"uvOffsetU\";\r\n"
		"	editorTemplate -addControl \"uvOffsetV\";\r\n"
		"	editorTemplate -addSeparator;\r\n"
		"	editorTemplate -addControl \"randomUvOffsetU\";\r\n"
		"	editorTemplate -addControl \"randomUvOffsetV\";\r\n"
		"	editorTemplate -addSeparator;\r\n"
		"	editorTemplate -addControl \"UvUdimLoop\";\r\n"
		"	editorTemplate -endLayout;\r\n"
		"	// -------------------------------------------------------------\r\n"
		"	editorTemplate -beginLayout \"Bake objects\" -collapse 0;	\r\n"
		"	editorTemplate -callCustom \"AE_cm_bakeObjects_create\" \"AE_cm_bakeObjects_edit\" \"\";\r\n"
		"	editorTemplate -endLayout;\r\n"
		"	// -------------------------------------------------------------\r\n"
		"	editorTemplate -beginLayout \"Output Mesh Settings\" -collapse 0;\r\n"
		"	editorTemplate -addControl \"smoothNormals\";\r\n"
		"	editorTemplate -addSeparator;\r\n"
		"    editorTemplate -addControl \"reverseNormals\";\r\n"
		"	editorTemplate -endLayout;\r\n"
		"	// -------------------------------------------------------------\r\n"
		"	editorTemplate -beginLayout \"Plug-in Info\" -collapse 1;\r\n"
		"	editorTemplate -callCustom \"AE_cm_website_create\" \"AE_cm_website_edit\" \"\";\r\n"
		"	editorTemplate -endLayout;\r\n"
		"	editorTemplate -suppress \"inMesh\";\r\n"
		"	editorTemplate -suppress \"inCurve\";\r\n"
		"	editorTemplate -suppress \"referenceMesh\";\r\n"
		"	editorTemplate -suppress \"outMesh\";\r\n"
		"	editorTemplate -suppress \"locatorAPos\";\r\n"
		"	editorTemplate -suppress \"locatorBPos\";\r\n"
		"	\r\n"
		"	AEdependNodeTemplate $nodeName;\r\n"
		"	editorTemplate -addExtraControls;\r\n"
		"	editorTemplate -endScrollLayout;\r\n"
		"	\r\n"
		"	\r\n"
		"}\r\n"
		"global proc AE_cm_objList_create(string $attrName)\r\n"
		"{\r\n"
		"    string $nodeName[];\r\n"
		"    tokenize($attrName, \".\", $nodeName);\r\n"
		"    \r\n"
		"    \r\n"
		"	textScrollList -allowMultiSelection false -h 120 \"cm_list_objList\";\r\n"
		"    \r\n"
		"    setParent ..;\r\n"
		"    \r\n"
		"    separator -height 10 -style \"in\";\r\n"
		"    \r\n"
		"    gridLayout -cwh 100 18 -numberOfColumns 4 -bgc 0.2 0.2 0.2;\r\n"
		"	iconTextButton -style \"iconAndTextHorizontal\" -image1 \"clonerMulti_Plus.png\" -label \"add\" -bgc 0.4 0.4 0.4 -c  (\"AE_cm_objList_add \" + $nodeName[0] ) \"cm_b_add\";\r\n"
		"	iconTextButton -style \"iconAndTextHorizontal\" -image1 \"clonerMulti_Minus.png\" -label \"remove\" -bgc 0.6 0.2 0.3 -c  (\"AE_cm_objList_remove \" + $nodeName[0] ) \"cm_b_rem\";\r\n"
		"	iconTextButton -style \"iconAndTextHorizontal\" -image1 \"clonerMulti_Plus.png\" -label \"up\" -bgc 0.2 0.2 0.2 -c (\"AE_cm_objList_moveUp \" + $nodeName[0] ) \"cm_b_up\";\r\n"
		"	iconTextButton -style \"iconAndTextHorizontal\" -image1 \"clonerMulti_Minus.png\" -label \"down\" -bgc 0.2 0.2 0.2 -c  (\"AE_cm_objList_moveDown \" + $nodeName[0] ) \"cm_b_down\";\r\n"
		"	setParent ..;\r\n"
		"	\r\n"
		"	separator -height 5 -style \"in\";\r\n"
		"	iconTextButton -style \"iconAndTextHorizontal\" -image1 \"clonerMulti_Refresh.png\" -label \"Refesh list\" -bgc 0.3 0.4 0.4 -c  (\"AE_cm_objList_refresh \" + $nodeName[0] ) \"cm_b_refr\";\r\n"
		"    separator -height 5 -style \"in\";\r\n"
		"    \r\n"
		"	gridLayout -numberOfColumns 3 -cellWidthHeight 100 18 -bgc 0.2 0.2 0.2;\r\n"
		"	text -label \"Ref Geo\";\r\n"
		"	textField -bgc 0.3 0.3 0.3 -ed false -tx \"none\" \"cm_t_driverObj\";\r\n"
		"	iconTextButton -style \"iconAndTextHorizontal\" -image1 \"clonerMulti_Apply.png\" -label \"Set Geo\" -c  (\"AE_cm_reference_set \" + $nodeName[0] ) \"cm_b_setRef\";\r\n"
		"    \r\n"
		"    \r\n"
		"    setParent ..;\r\n"
		"    separator -height 5 -style \"in\";\r\n"
		"	gridLayout -numberOfColumns 3 -cellWidthHeight 100 18 -bgc 0.2 0.2 0.2;\r\n"
		"	text -label \"Ref Curve\";\r\n"
		"	textField -bgc 0.3 0.3 0.3 -ed false -tx \"none\" \"cm_t_curveObj\";\r\n"
		"	iconTextButton -style \"iconAndTextHorizontal\" -image1 \"clonerMulti_Apply.png\" -label \"Set Curve\" -c  (\"AE_cm_curve_set \" + $nodeName[0] ) \"cm_b_setCrv\";\r\n"
		"    // Refresh list\r\n"
		"    AE_cm_objList_refresh($nodeName[0]);\r\n"
		"    scriptJob -rp -kws -p \"AttrEdclonerMultiFormLayout\" -connectionChange ($nodeName[0]+\".inMesh\") (\"AE_cm_objList_refresh \" + $nodeName[0] );\r\n"
		"    string $refMeshAttr = $nodeName[0] + \".referenceMesh\";\r\n"
		"    string $refMeshShapePlug = `connectionInfo -sourceFromDestination $refMeshAttr`;\r\n"
		"    string $refMeshShape[];\r\n"
		"    tokenize($refMeshShapePlug, \".\", $refMeshShape);\r\n"
		"    \r\n"
		"    if ($refMeshShape[0] != \"\")\r\n"
		"    {\r\n"
		"	    textField -edit -bgc 0.6 0.6 0.8 -tx $refMeshShape[0] \"cm_t_driverObj\";\r\n"
		"	}\r\n"
		"	\r\n"
		"    else\r\n"
		"    {\r\n"
		"	    textField -edit -bgc 0.3 0.3 0.3 -tx \"none\" \"cm_t_driverObj\";\r\n"
		"	}\r\n"
		"    string $refCurveAttr = $nodeName[0] + \".inCurve\";\r\n"
		"    string $refCurveShapePlug = `connectionInfo -sourceFromDestination $refCurveAttr`;\r\n"
		"    string $refCurveShape[];\r\n"
		"    tokenize($refCurveShapePlug, \".\", $refCurveShape);\r\n"
		"    \r\n"
		"    if ($refCurveShape[0] != \"\")\r\n"
		"    {\r\n"
		"	    textField -edit -bgc 0.8 0.6 0.6 -tx $refCurveShape[0] \"cm_t_curveObj\";\r\n"
		"	}\r\n"
		"	\r\n"
		"    else\r\n"
		"    {\r\n"
		"	    textField -edit -bgc 0.3 0.3 0.3 -tx \"none\" \"cm_t_curveObj\";\r\n"
		"	}\r\n"
		"	\r\n"
		"}\r\n"
		"global proc AE_cm_objList_edit(string $attrName)\r\n"
		"{\r\n"
		"    string $nodeName[];\r\n"
		"    tokenize($attrName, \".\", $nodeName);\r\n"
		"    \r\n"
		"    iconTextButton -edit -c (\"AE_cm_objList_add \" + $nodeName[0] ) \"cm_b_add\";\r\n"
		"	iconTextButton -edit -c (\"AE_cm_objList_remove \" + $nodeName[0] ) \"cm_b_rem\";\r\n"
		"	iconTextButton -edit -c (\"AE_cm_objList_moveUp \" + $nodeName[0] ) \"cm_b_up\";\r\n"
		"	iconTextButton -edit -c (\"AE_cm_objList_moveDown \" + $nodeName[0] ) \"cm_b_down\";\r\n"
		"	iconTextButton -edit -c (\"AE_cm_objList_refresh \" + $nodeName[0] ) \"cm_b_refr\";\r\n"
		"    string $refMeshAttr = $nodeName[0] + \".referenceMesh\";\r\n"
		"    string $refMeshShapePlug = `connectionInfo -sourceFromDestination $refMeshAttr`;\r\n"
		"    string $refMeshShape[];\r\n"
		"    tokenize($refMeshShapePlug, \".\", $refMeshShape);\r\n"
		"    \r\n"
		"    textField -edit -bgc 0.3 0.3 0.3 -tx \"none\" \"cm_t_driverObj\";\r\n"
		"    \r\n"
		"    if ($refMeshShape[0] != \"\")\r\n"
		"    {\r\n"
		"	    textField -edit -bgc 0.6 0.6 0.8 -tx $refMeshShape[0] \"cm_t_driverObj\";\r\n"
		"	}\r\n"
		"	\r\n"
		"    else\r\n"
		"    {\r\n"
		"	    textField -edit -bgc 0.3 0.3 0.3 -tx \"none\" \"cm_t_driverObj\";\r\n"
		"	}\r\n"
		"    string $refCurveAttr = $nodeName[0] + \".inCurve\";\r\n"
		"    string $refCurveShapePlug = `connectionInfo -sourceFromDestination $refCurveAttr`;\r\n"
		"    string $refCurveShape[];\r\n"
		"    tokenize($refCurveShapePlug, \".\", $refCurveShape);\r\n"
		"    \r\n"
		"    if ($refCurveShape[0] != \"\")\r\n"
		"    {\r\n"
		"	    textField -edit -bgc 0.8 0.6 0.6 -tx $refCurveShape[0] \"cm_t_curveObj\";\r\n"
		"	}\r\n"
		"	\r\n"
		"    else\r\n"
		"    {\r\n"
		"	    textField -edit -bgc 0.3 0.3 0.3 -tx \"none\" \"cm_t_curveObj\";\r\n"
		"	}\r\n"
		"	iconTextButton -edit -c (\"AE_cm_reference_set \" + $nodeName[0] ) \"cm_b_setRef\";\r\n"
		"	iconTextButton -edit -c (\"AE_cm_curve_set \" + $nodeName[0] ) \"cm_b_setCrv\";\r\n"
		"	\r\n"
		"	scriptJob -rp -kws -p \"AttrEdclonerMultiFormLayout\" -connectionChange ($nodeName[0]+\".inMesh\") (\"AE_cm_objList_refresh \" + $nodeName[0] );\r\n"
		"	\r\n"
		"	AE_cm_objList_refresh($attrName);\r\n"
		"   \r\n"
		"}\r\n"
		"global proc AE_cm_objList_add(string $attrName)\r\n"
		"{\r\n"
		"    \r\n"
		"    string $nodeName[];\r\n"
		"    tokenize($attrName, \".\", $nodeName);\r\n"
		"    \r\n"
		"    \r\n"
		"    string $selObj[] = `ls -sl -o -tr`;\r\n"
		"    string $selObjShape[];\r\n"
		"    \r\n"
		"    int $numObj = size($selObj);\r\n"
		"    \r\n"
		"    if ($numObj != 0)\r\n"
		"    {\r\n"
		"        string $goalAttr = \"\";\r\n"
		"        string $sourceAttr = \"\";\r\n"
		"        string $outMeshAttr = $nodeName[0] + \".outMesh\";\r\n"
		"        string $outMeshShapePlug[] = `connectionInfo -destinationFromSource $outMeshAttr`;\r\n"
		"        \r\n"
		"        if (size($outMeshShapePlug) != 0)\r\n"
		"        {\r\n"
		"            string $outMeshShape[];\r\n"
		"            tokenize($outMeshShapePlug[0], \".\", $outMeshShape);\r\n"
		"            string $parents[] = `listRelatives -ni -path -parent $outMeshShape[0]`;\r\n"
		"            \r\n"
		"            \r\n"
		"            int $plugSize = `getAttr -size ($nodeName[0] + \".inMesh\")`;\r\n"
		"            \r\n"
		"            for( $i=0; $i< $plugSize; ++$i )\r\n"
		"            {\r\n"
		"                $goalAttr = $nodeName[0] + \".inMesh[\" + $i + \"]\";\r\n"
		"                $sourceAttr = `connectionInfo -sfd $goalAttr`;\r\n"
		"                \r\n"
		"                if($sourceAttr != \"\")\r\n"
		"                {\r\n"
		"                    disconnectAttr $sourceAttr $goalAttr;\r\n"
		"                }\r\n"
		"            }\r\n"
		"            \r\n"
		"            string $listMeshes[] = `textScrollList -query -ai \"cm_list_objList\"`;\r\n"
		"            int $cc_count = 0;\r\n"
		"            \r\n"
		"            for( $i=0; $i<size($listMeshes); ++$i )\r\n"
		"            {\r\n"
		"                 connectAttr -f ( $listMeshes[$i] + \".worldMesh[0]\") ( $nodeName[0] + \".inMesh[\" + $i + \"]\");\r\n"
		"                 $cc_count +=1;\r\n"
		"            }\r\n"
		"            for( $i=0; $i < $numObj; ++$i )\r\n"
		"            {\r\n"
		"                \r\n"
		"                if($selObj[$i] != $parents[0])\r\n"
		"                {\r\n"
		"                    $selObjShape = `listRelatives -shapes $selObj[$i]`;\r\n"
		"                    connectAttr -f ( $selObjShape[0] + \".worldMesh[0]\") ( $nodeName[0] + \".inMesh[\" + ($i + ($cc_count)) + \"]\");\r\n"
		"                }\r\n"
		"            }\r\n"
		"            \r\n"
		"            AE_cm_objList_refresh($nodeName[0]);\r\n"
		"        }\r\n"
		"        \r\n"
		"        if (size($outMeshShapePlug) == 0)\r\n"
		"        {\r\n"
		"            warning -n \"[ClonerMulti] ClonerMulti node does not have an output mesh!\"; \r\n"
		"        }\r\n"
		"    }\r\n"
		"}\r\n"
		"global proc AE_cm_objList_remove(string $attrName)\r\n"
		"{\r\n"
		"    string $nodeName[];\r\n"
		"    tokenize($attrName, \".\", $nodeName);\r\n"
		"    \r\n"
		"    string $goalAttr = \"\";\r\n"
		"    string $sourceAttr = \"\";\r\n"
		"    \r\n"
		"    int $selIdx[] = `textScrollList -query -sii \"cm_list_objList\"`;\r\n"
		"    \r\n"
		"    if (`size($selIdx)` != 0)\r\n"
		"    {\r\n"
		"        int $plugSize = `getAttr -size ($nodeName[0] + \".inMesh\")`;\r\n"
		"        \r\n"
		"        for( $i=0; $i< $plugSize; ++$i )\r\n"
		"        {\r\n"
		"            $goalAttr = $nodeName[0] + \".inMesh[\" + $i + \"]\";\r\n"
		"            $sourceAttr = `connectionInfo -sfd $goalAttr`;\r\n"
		"            \r\n"
		"            if($sourceAttr != \"\")\r\n"
		"            {\r\n"
		"                disconnectAttr $sourceAttr $goalAttr;\r\n"
		"            }\r\n"
		"        }\r\n"
		"            textScrollList -edit -rii $selIdx[0] \"cm_list_objList\";\r\n"
		"            \r\n"
		"            string $listMeshes[] = `textScrollList -query -ai \"cm_list_objList\"`;\r\n"
		"            int $cc_count = 0;\r\n"
		"            \r\n"
		"            for( $i=0; $i<size($listMeshes); ++$i )\r\n"
		"            {\r\n"
		"                 connectAttr -f ( $listMeshes[$i] + \".worldMesh[0]\") ( $nodeName[0] + \".inMesh[\" + $i + \"]\");\r\n"
		"                 $cc_count +=1;\r\n"
		"            }\r\n"
		"    }\r\n"
		"    AE_cm_objList_refresh($attrName);\r\n"
		"    \r\n"
		"}\r\n"
		"global proc AE_cm_objList_moveUp(string $attrName)\r\n"
		"{\r\n"
		"    string $nodeName[];\r\n"
		"    tokenize($attrName, \".\", $nodeName);\r\n"
		"    \r\n"
		"    int $selIdx[] = `textScrollList -query -sii \"cm_list_objList\"`;\r\n"
		"    \r\n"
		"    if (`size($selIdx)` != 0)\r\n"
		"    {\r\n"
		"        if ($selIdx[0]-1 != 0)\r\n"
		"        {\r\n"
		"            string $listObjects[] = `textScrollList -query -ai \"cm_list_objList\"`;\r\n"
		"            string $listCurrOjb = $listObjects[$selIdx[0]-1];\r\n"
		"            $listObjects = `textScrollList -query -ai \"cm_list_objList\"`;\r\n"
		"            textScrollList -edit -rii ($selIdx[0]) \"cm_list_objList\";\r\n"
		"            textScrollList -edit -ap ($selIdx[0]-1) $listCurrOjb \"cm_list_objList\";\r\n"
		"            $listObjects = `textScrollList -query -ai \"cm_list_objList\"`;\r\n"
		"            textScrollList -edit -sii ($selIdx[0]-1) \"cm_list_objList\";\r\n"
		"        }\r\n"
		"        \r\n"
		"        else\r\n"
		"        {\r\n"
		"            textScrollList -edit -sii ($selIdx[0]) \"cm_list_objList\";  \r\n"
		"        }\r\n"
		"        AE_cm_reconnectPlugs_set($attrName);\r\n"
		"    }\r\n"
		"    \r\n"
		"}\r\n"
		"global proc AE_cm_objList_moveDown(string $attrName)\r\n"
		"{\r\n"
		"    \r\n"
		"    string $nodeName[];\r\n"
		"    tokenize($attrName, \".\", $nodeName);\r\n"
		"    \r\n"
		"    int $selIdx[] = `textScrollList -query -sii \"cm_list_objList\"`;\r\n"
		"    \r\n"
		"    if (`size($selIdx)` != 0)\r\n"
		"    {\r\n"
		"        int $numIndx = `textScrollList -query -ni \"cm_list_objList\"`;\r\n"
		"        \r\n"
		"        \r\n"
		"        if ($selIdx[0]-1 < $numIndx-1)\r\n"
		"        {\r\n"
		"            string $listObjects[] = `textScrollList -query -ai \"cm_list_objList\"`;\r\n"
		"            string $listCurrOjb = $listObjects[$selIdx[0]-1];\r\n"
		"            \r\n"
		"            //AE_cm_objList_refresh($attrName);\r\n"
		"            $listObjects = `textScrollList -query -ai \"cm_list_objList\"`;\r\n"
		"            \r\n"
		"            textScrollList -edit -rii ($selIdx[0]) \"cm_list_objList\";\r\n"
		"            textScrollList -edit -ap ($selIdx[0]+1) $listCurrOjb \"cm_list_objList\";\r\n"
		"            \r\n"
		"            $listObjects = `textScrollList -query -ai \"cm_list_objList\"`;\r\n"
		"            \r\n"
		"            textScrollList -edit -sii ($selIdx[0]+1) \"cm_list_objList\";\r\n"
		"        }\r\n"
		"        \r\n"
		"        else\r\n"
		"        {\r\n"
		"            textScrollList -edit -sii ($selIdx[0]) \"cm_list_objList\";  \r\n"
		"        }\r\n"
		"        \r\n"
		"        AE_cm_reconnectPlugs_set($attrName);\r\n"
		"    }\r\n"
		"}\r\n"
		"global proc AE_cm_reconnectPlugs_set(string $attrName)\r\n"
		"{\r\n"
		"    string $nodeName[];\r\n"
		"    tokenize($attrName, \".\", $nodeName);\r\n"
		"    \r\n"
		"	int $nodeExists;\r\n"
		"	$nodeExists = 0;\r\n"
		"	\r\n"
		"	print `objExists $nodeName[0]`;\r\n"
		"	\r\n"
		"	if( `objExists $nodeName[0]` == 1 ) \r\n"
		"	{\r\n"
		"		$nodeExists = 1;\r\n"
		"	} \r\n"
		"	else \r\n"
		"	{\r\n"
		"		warning(\"Object does not extist any more!\");\r\n"
		"	}\r\n"
		"	\r\n"
		"	if ($nodeExists == 1)\r\n"
		"	{\r\n"
		"		int $plugSize = `getAttr -size ($nodeName[0] + \".inMesh\")`;\r\n"
		"		\r\n"
		"		for( $i=0; $i< $plugSize; ++$i )\r\n"
		"		{\r\n"
		"			$goalAttr = $nodeName[0] + \".inMesh[\" + $i + \"]\";\r\n"
		"			$sourceAttr = `connectionInfo -sfd $goalAttr`;\r\n"
		"			\r\n"
		"			if($sourceAttr != \"\")\r\n"
		"			{\r\n"
		"				disconnectAttr $sourceAttr $goalAttr;\r\n"
		"			}\r\n"
		"		}\r\n"
		"		\r\n"
		"		string $listMeshes[] = `textScrollList -query -ai \"cm_list_objList\"`;\r\n"
		"		int $cc_count = 0;\r\n"
		"		\r\n"
		"		for( $i=0; $i<size($listMeshes); ++$i )\r\n"
		"		{\r\n"
		"			 \r\n"
		"			 string $list_shapes[] = `listRelatives -ni -pa -shapes $listMeshes[$i]`;\r\n"
		"			 connectAttr -f ( $list_shapes[0] + \".worldMesh[0]\") ( $nodeName[0] + \".inMesh[\" + $i + \"]\");\r\n"
		"			 \r\n"
		"			 $cc_count +=1;\r\n"
		"		}\r\n"
		"	}\r\n"
		"	\r\n"
		"    AE_cm_objList_refresh($attrName);\r\n"
		"}\r\n"
		"global proc AE_cm_curve_set(string $attrName)\r\n"
		"{\r\n"
		"    string $nodeName[];\r\n"
		"    tokenize($attrName, \".\", $nodeName);\r\n"
		"    \r\n"
		"    int $isSame = 0;\r\n"
		"    \r\n"
		"    string $selObj[] = `ls -sl -o -tr`;\r\n"
		"    string $selObjShape[];\r\n"
		"    \r\n"
		"    if (size($selObj) != 0)\r\n"
		"    {\r\n"
		"        $selObjShape = `listRelatives  -type \"nurbsCurve\" -shapes $selObj[0]`;\r\n"
		"        \r\n"
		"		if (size($selObjShape) == 0)\r\n"
		"		{\r\n"
		"			warning -n \"[ClonerMulti]Selected object is not a curve!\";\r\n"
		"			return;\r\n"
		"		}\r\n"
		"		\r\n"
		"        if (`objectType $selObjShape` == \"nurbsCurve\")\r\n"
		"        {\r\n"
		"            // Check if we are trying to add the same curve\r\n"
		"            \r\n"
		"            string $goalAttr = \"\";\r\n"
		"            string $sourceAttr = \"\";\r\n"
		"            \r\n"
		"            string $inCurveAttr = $nodeName[0] + \".inCurve\";\r\n"
		"    		\r\n"
		"            string $InCurveShapePlug = `connectionInfo -sourceFromDestination $inCurveAttr`;\r\n"
		"            string $InCurveShape[];\r\n"
		"            tokenize($InCurveShapePlug, \".\", $InCurveShape);\r\n"
		"            // string $parents[] = `listRelatives -ni -path -parent $InCurveShape[0]`;\r\n"
		"    		\r\n"
		"    		if($selObjShape[0] == $InCurveShape[0])\r\n"
		"            {\r\n"
		"                $isSame = 1;\r\n"
		"            }\r\n"
		"            \r\n"
		"            if($isSame == 0)\r\n"
		"            {\r\n"
		"    \r\n"
		"    			textField -edit -bgc 0.8 0.6 0.6 -tx $selObjShape[0] \"cm_t_curveObj\";\r\n"
		"    			connectAttr -f ( $selObjShape[0] + \".worldSpace[0]\") ( $nodeName[0] + \".inCurve\");\r\n"
		"    		}\r\n"
		"    		\r\n"
		"            if($isSame == 1)\r\n"
		"            {\r\n"
		"                warning -n \"[ClonerMulti] Curve allready added!\";\r\n"
		"            }\r\n"
		"		}\r\n"
		"    }\r\n"
		"    \r\n"
		"    else\r\n"
		"    {\r\n"
		"        warning -n \"[ClonerMulti] Nothing selected / Select only one object!\";\r\n"
		"    }\r\n"
		"}\r\n"
		"global proc AE_cm_reference_set(string $attrName)\r\n"
		"{\r\n"
		"    string $nodeName[];\r\n"
		"    tokenize($attrName, \".\", $nodeName);\r\n"
		"    int $isSame = 0;\r\n"
		"    string $selObj[] = `ls -sl -o -tr`;\r\n"
		"    string $selObjShape[];\r\n"
		"    \r\n"
		"    if (size($selObj) != 0)\r\n"
		"    {\r\n"
		"        $selObjShape = `listRelatives  -type \"mesh\" -shapes $selObj[0]`;\r\n"
		"        \r\n"
		"        \r\n"
		"        // Check if we are trying to add the output mesh\r\n"
		"        \r\n"
		"        string $goalAttr = \"\";\r\n"
		"        string $sourceAttr = \"\";\r\n"
		"        \r\n"
		"        string $outMeshAttr = $nodeName[0] + \".outMesh\";\r\n"
		"        string $outMeshShapePlug[] = `connectionInfo -destinationFromSource $outMeshAttr`;\r\n"
		"        string $outMeshShape[];\r\n"
		"        tokenize($outMeshShapePlug[0], \".\", $outMeshShape);\r\n"
		"        string $parents[] = `listRelatives -ni -path -parent $outMeshShape[0]`;\r\n"
		"        \r\n"
		"        //\r\n"
		"        \r\n"
		"        if($selObjShape[0] == $outMeshShape[0])\r\n"
		"        {\r\n"
		"            $isSame = 1;\r\n"
		"        }\r\n"
		"        \r\n"
		"        int $plugSize = `getAttr -size ($nodeName[0] + \".inMesh\")`;\r\n"
		"        \r\n"
		"        string $inMeshShape[];\r\n"
		"        \r\n"
		"        for( $i=0; $i< $plugSize; ++$i )\r\n"
		"        {\r\n"
		"            $goalAttr = $nodeName[0] + \".inMesh[\" + $i + \"]\";\r\n"
		"            $sourceAttr = `connectionInfo -sfd $goalAttr`;\r\n"
		"            \r\n"
		"            \r\n"
		"            \r\n"
		"            if($sourceAttr != \"\")\r\n"
		"            {\r\n"
		"                tokenize($sourceAttr, \".\", $inMeshShape);\r\n"
		"                \r\n"
		"                if($selObjShape[0] == $inMeshShape[0])\r\n"
		"                {\r\n"
		"                    $isSame = 1;\r\n"
		"                }\r\n"
		"                \r\n"
		"                \r\n"
		"            }\r\n"
		"        }\r\n"
		"        \r\n"
		"        //\r\n"
		"        \r\n"
		"        if($isSame == 0)\r\n"
		"        {\r\n"
		"            textField -edit -bgc 0.6 0.6 0.8 -tx $selObjShape[0] \"cm_t_driverObj\";\r\n"
		"            connectAttr -f ( $selObjShape[0] + \".worldMesh[0]\") ( $nodeName[0] + \".referenceMesh\");\r\n"
		"            \r\n"
		"        }\r\n"
		"        \r\n"
		"        if($isSame == 1)\r\n"
		"        {\r\n"
		"            warning -n \"[ClonerMulti] Reference mesh can't be the same as the output/input meshes!\";\r\n"
		"        }\r\n"
		"    }\r\n"
		"    else\r\n"
		"    {\r\n"
		"        warning -n \"[ClonerMulti] Nothing selected / Select only one object!\";\r\n"
		"    }\r\n"
		"}\r\n"
		"global proc AE_cm_objList_refresh(string $attrName)\r\n"
		"{\r\n"
		"    string $nodeName[];\r\n"
		"    tokenize($attrName, \".\", $nodeName);\r\n"
		"    int $currSelIdx[] = `textScrollList -query -sii \"cm_list_objList\"`;\r\n"
		"    textScrollList -edit -ra \"cm_list_objList\";\r\n"
		"    string $inMeshes[] = `listConnections -s true ($nodeName[0] + \".inMesh\")`;\r\n"
		"    \r\n"
		"    for( $i=0; $i<size($inMeshes); ++$i )\r\n"
		"	{\r\n"
		"	    textScrollList -edit -append $inMeshes[$i] \"cm_list_objList\";\r\n"
		"	}\r\n"
		"	\r\n"
		"	if($currSelIdx[0] != 0)\r\n"
		"	{\r\n"
		"	    textScrollList -edit -sii $currSelIdx[0] \"cm_list_objList\";\r\n"
		"	}\r\n"
		"    string $refMeshAttr = $nodeName[0] + \".referenceMesh\";\r\n"
		"    string $refMeshShapePlug = `connectionInfo -sourceFromDestination $refMeshAttr`;\r\n"
		"    string $refMeshShape[];\r\n"
		"    tokenize($refMeshShapePlug, \".\", $refMeshShape);\r\n"
		"    \r\n"
		"    textField -edit -bgc 0.3 0.3 0.3 -tx \"none\" \"cm_t_driverObj\";\r\n"
		"    \r\n"
		"    if ($refMeshShape[0] != \"\")\r\n"
		"    {\r\n"
		"	    textField -edit -bgc 0.6 0.6 0.8 -tx $refMeshShape[0] \"cm_t_driverObj\";\r\n"
		"	}\r\n"
		"    else\r\n"
		"    {\r\n"
		"	    textField -edit -bgc 0.3 0.3 0.3 -tx \"none\" \"cm_t_driverObj\";\r\n"
		"	}\r\n"
		"    string $refCurveAttr = $nodeName[0] + \".inCurve\";\r\n"
		"    string $refCurveShapePlug = `connectionInfo -sourceFromDestination $refCurveAttr`;\r\n"
		"    string $refCurveShape[];\r\n"
		"    tokenize($refCurveShapePlug, \".\", $refCurveShape);\r\n"
		"    \r\n"
		"    if ($refCurveShape[0] != \"\")\r\n"
		"    {\r\n"
		"	    textField -edit -bgc 0.8 0.6 0.6 -tx $refCurveShape[0] \"cm_t_curveObj\";\r\n"
		"	}\r\n"
		"	\r\n"
		"    else\r\n"
		"    {\r\n"
		"	    textField -edit -bgc 0.3 0.3 0.3 -tx \"none\" \"cm_t_curveObj\";\r\n"
		"	}\r\n"
		"}\r\n"
		"global proc AE_cm_abLoc_create(string $attrName)\r\n"
		"{\r\n"
		"    string $nodeName[];\r\n"
		"    tokenize($attrName, \".\", $nodeName);\r\n"
		"    separator -height 5 -style \"in\";\r\n"
		"    iconTextButton -style \"iconAndTextHorizontal\" -image1 \"clonerMulti_Apply.png\" -label \"Create A-B loc\"  -bgc 0.3 0.4 0.4 -c  (\"AE_cm_abLoc_set \" + $nodeName[0] ) \"cm_b_createABLoc\";\r\n"
		"    separator -height 5 -style \"in\";\r\n"
		"}\r\n"
		"global proc AE_cm_abLoc_edit(string $attrName)\r\n"
		"{\r\n"
		"    string $nodeName[];\r\n"
		"    tokenize($attrName, \".\", $nodeName);\r\n"
		"    iconTextButton -edit -c (\"AE_cm_abLoc_set \" + $nodeName[0] ) \"cm_b_createABLoc\";\r\n"
		"}\r\n"
		"global proc AE_cm_abLoc_set(string $attrName)\r\n"
		"{\r\n"
		"    string $nodeName[];\r\n"
		"    tokenize($attrName, \".\", $nodeName);\r\n"
		"    clonerMultiCommand -ab -no $nodeName[0];\r\n"
		"}\r\n"
		"global proc AE_cm_bakeObjects_create(string $attrName)\r\n"
		"{\r\n"
		"    string $nodeName[];\r\n"
		"    tokenize($attrName, \".\", $nodeName);\r\n"
		"    separator -height 5 -style \"in\";\r\n"
		"	iconTextButton -style \"iconAndTextHorizontal\" -image1 \"clonerMulti_Trash.png\" -label \"Bake objects to Instances\" -bgc 0.6 0.2 0.3 -c  (\"AE_cm_bakeObjects_doBake \" + $nodeName[0] ) \"cm_b_bake\";\r\n"
		"    separator -height 5 -style \"in\";\r\n"
		"}\r\n"
		"global proc AE_cm_bakeObjects_edit(string $attrName)\r\n"
		"{\r\n"
		"    string $nodeName[];\r\n"
		"    tokenize($attrName, \".\", $nodeName);\r\n"
		"    iconTextButton -edit -c (\"AE_cm_bakeObjects_doBake \" + $nodeName[0] ) \"cm_b_bake\";\r\n"
		"}\r\n"
		"global proc AE_cm_bakeObjects_doBake(string $attrName)\r\n"
		"{\r\n"
		"    string $nodeName[];\r\n"
		"    tokenize($attrName, \".\", $nodeName);\r\n"
		"    print \"[ClonerMulti] Baking to Instances, Please wait...\";\r\n"
		"    string $nodeNameParent[];\r\n"
		"    $nodeNameParent = `listRelatives -parent $nodeName[0]`;\r\n"
		"    if (size($nodeNameParent) != 0)\r\n"
		"    {\r\n"
		"        clonerMultiCommand -no $nodeNameParent[0] -b;\r\n"
		"        deleteUI AttrEdclonerMultiFormLayout;\r\n"
		"    }\r\n"
		"}\r\n"
		"global proc AE_cm_refreshUVs_create(string $attrName)\r\n"
		"{\r\n"
		"    string $nodeName[];\r\n"
		"    tokenize($attrName, \".\", $nodeName);\r\n"
		"    separator -height 5 -style \"in\";\r\n"
		"	iconTextButton -style \"iconAndTextHorizontal\"  -image1 \"clonerMulti_Refresh.png\"  -label \"Refresh UVs\" -bgc 0.8 0.4 0.4 -c  (\"AE_cm_refreshUVs_doRefreshUVs \" + $nodeName[0] ) \"cm_b_refreshuv\";\r\n"
		"    separator -height 5 -style \"in\";\r\n"
		"}\r\n"
		"global proc AE_cm_refreshUVs_edit(string $attrName)\r\n"
		"{\r\n"
		"    string $nodeName[];\r\n"
		"    tokenize($attrName, \".\", $nodeName);\r\n"
		"    iconTextButton -edit -style \"iconAndTextHorizontal\" -image1 \"clonerMulti_Refresh.png\" -c (\"AE_cm_refreshUVs_doRefreshUVs \" + $nodeName[0] ) \"cm_b_refreshuv\";\r\n"
		"}\r\n"
		"global proc AE_cm_refreshUVs_doRefreshUVs(string $attrName)\r\n"
		"{\r\n"
		"    string $nodeName[];\r\n"
		"    tokenize($attrName, \".\", $nodeName);\r\n"
		"    print \"[ClonerMulti] Refreshing UVs...\\n\";\r\n"
		"    dgdirty $nodeName[0]; \r\n"
		"}\r\n"
		"global proc AE_cm_setArrays_create(string $attrName)\r\n"
		"{\r\n"
		"    string $nodeName[];\r\n"
		"    tokenize($attrName, \".\", $nodeName);\r\n"
		"    separator -height 5 -style \"in\";\r\n"
		"    text -label \"Snap is limited to one input object only...\";\r\n"
		"    separator -height 5 -style \"in\";\r\n"
		"    \r\n"
		"    rowLayout -numberOfColumns 3;\r\n"
		"    iconTextButton -style \"iconAndTextHorizontal\" -image1 \"clonerMulti_Apply.png\" -label \"Set A Array\"  -bgc 0.3 0.4 0.4 -c  (\"AE_cm_setArrays_A \" + $nodeName[0] ) \"cm_b_applyAArray\";\r\n"
		"    iconTextButton -style \"iconAndTextHorizontal\" -image1 \"clonerMulti_Refresh.png\" -label \"Shift\"  -bgc 0.6 0.4 0.4 -c  (\"AE_cm_shiftArrays_A \" + $nodeName[0] ) \"cm_b_shiftAArray\";\r\n"
		"    iconTextButton -style \"iconAndTextHorizontal\" -image1 \"clonerMulti_Refresh.png\" -label \"Reverse\"  -bgc 0.3 0.6 0.4 -c  (\"AE_cm_reverseArrays_A \" + $nodeName[0] ) \"cm_b_reverseAArray\";\r\n"
		"    setParent ..;\r\n"
		"    \r\n"
		"    separator -height 5 -style \"in\";\r\n"
		"    rowLayout -numberOfColumns 3;\r\n"
		"    iconTextButton -style \"iconAndTextHorizontal\" -image1 \"clonerMulti_Apply.png\" -label \"Set B Array\"  -bgc 0.3 0.4 0.4 -c  (\"AE_cm_setArrays_B \" + $nodeName[0] ) \"cm_b_applyBArray\";\r\n"
		"    iconTextButton -style \"iconAndTextHorizontal\" -image1 \"clonerMulti_Refresh.png\" -label \"Shift\"  -bgc 0.6 0.4 0.4 -c  (\"AE_cm_shiftArrays_B \" + $nodeName[0] ) \"cm_b_shiftBArray\";\r\n"
		"    iconTextButton -style \"iconAndTextHorizontal\" -image1 \"clonerMulti_Refresh.png\" -label \"Reverse\"  -bgc 0.3 0.6 0.4 -c  (\"AE_cm_reverseArrays_B \" + $nodeName[0] ) \"cm_b_reverseBArray\";\r\n"
		"    setParent ..;\r\n"
		"    separator -height 5 -style \"in\";\r\n"
		"}\r\n"
		"global proc AE_cm_setArrays_edit(string $attrName)\r\n"
		"{\r\n"
		"    string $nodeName[];\r\n"
		"    tokenize($attrName, \".\", $nodeName);\r\n"
		"    iconTextButton -edit -style \"iconAndTextHorizontal\" -image1 \"clonerMulti_Apply.png\" -c (\"AE_cm_setArrays_A \" + $nodeName[0] ) \"cm_b_applyAArray\";\r\n"
		"    iconTextButton -edit -style \"iconAndTextHorizontal\" -image1 \"clonerMulti_Refresh.png\"  -c (\"AE_cm_shiftArrays_A \" + $nodeName[0] ) \"cm_b_shiftAArray\";\r\n"
		"    iconTextButton -edit -style \"iconAndTextHorizontal\" -image1 \"clonerMulti_Refresh.png\"  -c (\"AE_cm_reverseArrays_A \" + $nodeName[0] ) \"cm_b_reverseAArray\";\r\n"
		"    iconTextButton -edit -style \"iconAndTextHorizontal\" -image1 \"clonerMulti_Apply.png\" -c (\"AE_cm_setArrays_B \" + $nodeName[0] ) \"cm_b_applyBArray\";\r\n"
		"    iconTextButton -edit -style \"iconAndTextHorizontal\" -image1 \"clonerMulti_Refresh.png\"  -c (\"AE_cm_shiftArrays_B \" + $nodeName[0] ) \"cm_b_shiftBArray\";\r\n"
		"    iconTextButton -edit -style \"iconAndTextHorizontal\" -image1 \"clonerMulti_Refresh.png\"  -c (\"AE_cm_reverseArrays_B \" + $nodeName[0] ) \"cm_b_reverseBArray\";\r\n"
		"}\r\n"
		"global proc AE_cm_setArrays_A(string $attrName)\r\n"
		"{\r\n"
		"    string $nodeName[];\r\n"
		"    tokenize($attrName, \".\", $nodeName);\r\n"
		"    string $vert_array = `clonerMultiCommand -ac`;\r\n"
		"    setAttr -type \"string\" ($nodeName[0] + \".connectArrayA\") $vert_array;\r\n"
		"}\r\n"
		"global proc AE_cm_setArrays_B(string $attrName)\r\n"
		"{\r\n"
		"    string $nodeName[];\r\n"
		"    tokenize($attrName, \".\", $nodeName);\r\n"
		"    string $vert_array = `clonerMultiCommand -ac`;\r\n"
		"    setAttr -type \"string\" ($nodeName[0] + \".connectArrayB\") $vert_array;\r\n"
		"}\r\n"
		"global proc AE_cm_shiftArrays_A(string $attrName)\r\n"
		"{\r\n"
		"    string $nodeName[];\r\n"
		"    tokenize($attrName, \".\", $nodeName);\r\n"
		"    string $shiftArrayString = `getAttr ($nodeName[0] + \".connectArrayA\")`;\r\n"
		"    string $shiftArrayString_sorted = `clonerMultiCommand -s $shiftArrayString`;\r\n"
		"    setAttr -type \"string\" ($nodeName[0] + \".connectArrayA\") $shiftArrayString_sorted;\r\n"
		"}\r\n"
		"global proc AE_cm_shiftArrays_B(string $attrName)\r\n"
		"{\r\n"
		"    string $nodeName[];\r\n"
		"    tokenize($attrName, \".\", $nodeName);\r\n"
		"    string $shiftArrayString = `getAttr ($nodeName[0] + \".connectArrayB\")`;\r\n"
		"    string $shiftArrayString_sorted = `clonerMultiCommand -s $shiftArrayString`;\r\n"
		"    setAttr -type \"string\" ($nodeName[0] + \".connectArrayB\") $shiftArrayString_sorted;\r\n"
		"}\r\n"
		"global proc AE_cm_reverseArrays_A(string $attrName)\r\n"
		"{\r\n"
		"    string $nodeName[];\r\n"
		"    tokenize($attrName, \".\", $nodeName);\r\n"
		"    string $shiftArrayString = `getAttr ($nodeName[0] + \".connectArrayA\")`;\r\n"
		"    string $shiftArrayString_sorted = `clonerMultiCommand -r $shiftArrayString`;\r\n"
		"    setAttr -type \"string\" ($nodeName[0] + \".connectArrayA\") $shiftArrayString_sorted;\r\n"
		"}\r\n"
		"global proc AE_cm_reverseArrays_B(string $attrName)\r\n"
		"{\r\n"
		"    string $nodeName[];\r\n"
		"    tokenize($attrName, \".\", $nodeName);\r\n"
		"    string $shiftArrayString = `getAttr ($nodeName[0] + \".connectArrayB\")`;\r\n"
		"    string $shiftArrayString_sorted = `clonerMultiCommand -r $shiftArrayString`;\r\n"
		"    setAttr -type \"string\" ($nodeName[0] + \".connectArrayB\") $shiftArrayString_sorted;\r\n"
		"}\r\n"
		"global proc AE_cm_launch_website()\r\n"
		"{\r\n"
		"    launch -web \"http://gumroad.com/creativecase\";\r\n"
		"}\r\n"
		"global proc AE_cm_website_create(string $attrName)\r\n"
		"{\r\n"
		"	string $nodeName[];\r\n"
		"    tokenize($attrName, \".\", $nodeName);\r\n"
		"    iconTextButton -label \"Creative Case website\" -style \"iconAndTextHorizontal\" -image1 \"clonerMulti_CCLogo.png\" -c \"AE_cm_launch_website()\";\r\n"
		"    setParent ..;\r\n"
		"}\r\n"
		"global proc AE_cm_website_edit(string $attrName)\r\n"
		"{\r\n"
		"}\r\n";




	return s_aeTemplate;
}

MString mel_createShelf()
{

	MString s_shelf = 
		"int $cc_doesShelfExist = `shelfLayout -query -ex \"CreativeCase\"`;\r\n"
		"if ($cc_doesShelfExist == 1)\r\n"
		"{\r\n"
		"	string $shelfButtons[] = `shelfLayout -q -ca \"CreativeCase\"`;\r\n"
		"	int $ex_b01,$ex_b02 = 0;\r\n"
		"	for( $i=0; $i<size($shelfButtons); ++$i )\r\n"
		"	{\r\n"
		"		if( `control -exists $shelfButtons[$i]` == true)\r\n"
		"		{\r\n"
		"			if (`control -q -docTag $shelfButtons[$i]` == \"cm_createCmButton\") {$ex_b01 = 1;}\r\n"
		"		}\r\n"
		"	}\r\n"
		"	if ($ex_b01 == 0) {shelfButton -p \"CreativeCase\" -dtg \"cm_createCmButton\" -annotation \"Apply a ClonerMulti modifier on the selected objects and/or curves\" -image1 \"clonerMulti.png\" -command \"clonerMultiCommand\";}\r\n"
		"}\r\n"
		"	\r\n"
		"if ($cc_doesShelfExist == false)\r\n"
		"{\r\n"
		"		shelfLayout -cellWidth 33 -cellHeight 33 -p $gShelfTopLevel CreativeCase;\r\n"
		"		shelfButton -p \"CreativeCase\" -dtg \"cm_createCmButton\" -annotation \"Apply a ClonerMulti modifier on the selected objects and/or curves\" -image1 \"clonerMulti.png\" -command \"clonerMultiCommand\";\r\n"
		"}\r\n";




	return s_shelf;
}

#endif