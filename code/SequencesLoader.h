/*
 	Ray
    Copyright (C) 2010, 2011  Sébastien Boisvert

	http://DeNovoAssembler.SourceForge.Net/

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, version 3 of the License.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You have received a copy of the GNU General Public License
    along with this program (COPYING).  
	see <http://www.gnu.org/licenses/>

*/

// TYPE: ALGORITHM

#ifndef _SequencesLoader
#define _SequencesLoader

#include<Parameters.h>
#include<RingAllocator.h>
#include<Loader.h>
#include<MyAllocator.h>
#include<StaticVector.h>
#include<vector>
#include<Message.h>
#include<Read.h>
#include<BubbleData.h>
#include<time.h>
using namespace std;

class SequencesLoader{
	MyAllocator*m_persistentAllocator;
	ArrayOfReads*m_myReads;
	Parameters*m_parameters;
	uint64_t m_distribution_currentSequenceId;
	int m_distribution_file_id;
	uint64_t m_distribution_sequence_id;
	bool m_LOADER_isLeftFile;
	bool m_LOADER_isRightFile;
	Loader m_loader;
	bool m_isInterleavedFile;
	int m_rank;
	int m_size;

	void registerSequence();

public:
	bool loadSequences(int rank,int size,StaticVector*m_outbox,
	RingAllocator*m_outboxAllocator,
	bool*m_loadSequenceStep,
	BubbleData*m_bubbleData,
	time_t*m_lastTime,
	Parameters*m_parameters,int*m_master_mode,int*m_mode
);
	bool computePartition(int rank,int size,StaticVector*m_outbox,
	RingAllocator*m_outboxAllocator,
	bool*m_loadSequenceStep,
	BubbleData*m_bubbleData,
	time_t*m_lastTime,
	Parameters*m_parameters,int*m_master_mode,int*m_mode);

	void constructor(int size,MyAllocator*m_persistentAllocator,ArrayOfReads*m_myReads);
};
#endif
