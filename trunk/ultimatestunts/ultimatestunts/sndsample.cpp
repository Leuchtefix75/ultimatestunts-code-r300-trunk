/***************************************************************************
                          sndsample.cpp  -  description
                             -------------------
    begin                : di feb 25 2003
    copyright            : (C) 2003 by CJP
    email                : cornware-cjp@users.sourceforge.net
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "sndsample.h"

#include <stdio.h>
#ifdef HAVE_LIBFMOD

#ifdef FMOD_HEADER
#include <fmod/fmod.h>
#include <fmod/fmod_errors.h>
#endif

CSndSample::CSndSample()
{
	  m_Sample = NULL;
}

CSndSample::CSndSample(int t)
{
	m_Sample = NULL;
	init(t);
}

CSndSample::~CSndSample()
{
  FSOUND_Sample_Free((FSOUND_SAMPLE  *)m_Sample);
  m_Sample = NULL;
}

void CSndSample::init(int t)
{
  m_Type = t;
}

int CSndSample::loadFromFile(CString filename)
{
  switch(m_Type)
  {
    case SAMPLE_2D:
      m_Sample = (void *)FSOUND_Sample_Load(FSOUND_UNMANAGED, filename.c_str(), FSOUND_2D, 0);
      break;
    case SAMPLE_3D:
      m_Sample = (void *)FSOUND_Sample_Load(FSOUND_FREE, filename.c_str(), FSOUND_HW3D, 0);
      break;
  }

  if (!m_Sample)
  {
    printf("   sample: %s\n", FMOD_ErrorString(FSOUND_GetError()));
    return 1;
  }
  switch(m_Type)
  {
    case SAMPLE_2D:
      FSOUND_Sample_SetLoopMode((FSOUND_SAMPLE  *)m_Sample, FSOUND_LOOP_NORMAL);
      break;
    case SAMPLE_3D:
      // increasing mindistnace makes it louder in 3d space
      FSOUND_Sample_SetMinMaxDistance((FSOUND_SAMPLE  *)m_Sample, 4.0f, 1000.0f);
      FSOUND_Sample_SetLoopMode((FSOUND_SAMPLE  *)m_Sample, FSOUND_LOOP_NORMAL);
      break;
  }
}

int CSndSample::attachToChannel(int c)
{
  return FSOUND_PlaySoundEx(c, (FSOUND_SAMPLE  *)m_Sample, NULL, true);
}

#else //libfmod

CSndSample::CSndSample(int t)
{;}
CSndSample::~CSndSample()
{;}

int CSndSample::loadFromFile(CString filename)
{
  return 1;
}

int CSndSample::attachToChannel(int c)
{
  return -1; //no sound implementation
}

#endif //libfmod
