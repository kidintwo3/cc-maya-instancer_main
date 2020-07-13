//
//  CloneIDs.cpp
//  ClonerMulti
//
//  Created by Hunyadi János on 2015. 01. 03..
//  Copyright (c) 2015. Janos Hunyadi. All rights reserved.
//



#include "CloneIDs.h"


MIntArray ClonerMultiThread::generatePatternIDs(short& patternType, int& m_numDup)
{

	// temp
	//double m_rndIDLev = 1.0;


	MIntArray idAr;
	idAr.setLength(m_numDup);

	// MGlobal::displayInfo(MString() + "numdup: " + m_numDup + "/" + patternType);


	MIntArray m_manualIDA;

	if (patternType == 0) // Generate one ID array
	{


		int setID = m_id;
		if (setID >= m_numInputMeshes-1)
		{
			setID = m_numInputMeshes-1;
		}

		for (int i =0;i < m_numDup;i++) {
			idAr.set(setID,i);
		}
	}


	if (patternType == 1) // Generate forward loop ID array
	{
		int c = 0;

		for (int i =0;i < m_numDup;i++) {

			idAr.set(c,i);

			c+=1;

			if (c >= m_numInputMeshes)
			{
				c = 0;
			}

		}



	}


	if (patternType == 2) // Generate backward loop ID array
	{

		int c = m_numInputMeshes-1;

		for (int i =0;i < m_numDup;i++) {

			idAr.set(c,i);

			c-=1;

			if (c == -1)
			{
				c = m_numInputMeshes-1;
			}

		}

	}


	if (patternType == 3) // Generate random ID array

	{
		MIntArray rndArFade;
		rndArFade.setLength(m_numDup);

		int randID, randIDFade;

		for (int i =0;i < m_numDup;i++) {

			randID = rand() % m_numInputMeshes;
			idAr.set(randID,i);

			randIDFade = rand() % 100;
			rndArFade.set(randIDFade,i);
		}

		for (int i =0;i < m_numDup;i++) {
			double fadeVal = rndArFade[i] - (100 * m_rndIDLev);

			if (fadeVal <= 0 ) {
				fadeVal = 0;
				idAr.set(0,i);
			}

		}



	}



	if (patternType == 4  || patternType == 7)
	{
		if (m_numInputMeshes >= 3)
		{
			idAr.set(0,0);

			for (int i =1;i < m_numDup;i++) 
			{
				idAr.set(1,i);
			}

			idAr.set(2,m_numDup-1);
		}


		else
		{
			for (int i =0;i < m_numDup;i++) 
			{
				idAr.set(0,i);
			}
		}

	}

	if (patternType == 5)
	{

		if (m_numInputMeshes >= 2)
		{
			for (int i =0;i < m_numDup-1;i++) 
			{
				idAr.set(0,i);
			}

			idAr.set(1,m_numDup-1);
		}



		else
		{
			for (int i =0;i < m_numDup;i++) 
			{
				idAr.set(0,i);
			}
		}

	}

	if (patternType == 6)
	{

		if (m_numInputMeshes >= 2)
		{
			idAr.set(0,0);

			for (int i =1;i < m_numDup;i++) 
			{
				idAr.set(1,i);
			}
		}

		else
		{
			for (int i =0;i < m_numDup;i++) 
			{
				idAr.set(0,i);
			}
		}




	}


	// Reverse pattern option

	if (m_revPattern) // Reverse pattern order
	{
		MIntArray reverseArray;

		for (int i =  idAr.length()-1; i >= 0; i--) {
			reverseArray.append(idAr[i]);
		}

		idAr = reverseArray;


	}




	return idAr;

}

