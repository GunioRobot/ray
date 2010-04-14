/*
 	Ray
    Copyright (C) 2010  Sébastien Boisvert

	http://DeNovoAssembler.SourceForge.Net/

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, version 3 of the License.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You have received a copy of the GNU General Public License
    along with this program (gpl-3.0.txt).  
	see <http://www.gnu.org/licenses/>

*/

#include<assert.h>
#include<BufferedData.h>

void BufferedData::constructor(int numberOfRanks,int capacity,MyAllocator*allocator){
	m_sizes=(int*)allocator->allocate(sizeof(int)*numberOfRanks);
	m_data=(u64*)allocator->allocate(sizeof(u64)*capacity*numberOfRanks);
	for(int i=0;i<(int)numberOfRanks;i++)
		m_sizes[i]=0;
	m_capacity=capacity;
	m_ranks=numberOfRanks;
}


int BufferedData::size(int i){
	#ifdef DEBUG
	assert(i<m_ranks);
	#endif
	return m_sizes[i];
}

u64 BufferedData::getAt(int i,int j){
	return m_data[i*m_capacity+j];
}

void BufferedData::addAt(int i,u64 k){
	int j=size(i);
	m_data[i*m_capacity+j]=k;
	m_sizes[i]++;
}

void BufferedData::reset(int i){
	m_sizes[i]=0;
}