/*
 	OpenAssembler -- a de Bruijn DNA assembler for mixed high-throughput technologies
    Copyright (C) 2009  Sébastien Boisvert

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


#include<common_functions.h>
#include<Read.h>
#include<cstdlib>
#include<iostream>
#include<cstring>

using namespace  std;

Read::Read(){
	m_sequence=NULL;
	//m_id=NULL;
}


void Read::copy(const char*id,const char*sequence,MyAllocator*seqMyAllocator){
	m_sequence=(char*)seqMyAllocator->allocate(strlen(sequence)+1);
	//m_id=(char*)malloc(strlen(id)+1);
	//m_id=NULL;
	strcpy(m_sequence,sequence);
	//strcpy(m_id,id);
}

Read::Read(const char*id,const char*sequence,MyAllocator*seqMyAllocator){
	copy(id,sequence,seqMyAllocator);
}

Read::~Read(){
/*
	if(m_id!=NULL){
		free(m_id);
		m_id=NULL;
	}
	if(m_sequence!=NULL){
		free(m_sequence);
		m_sequence=NULL;
	}
*/
}


char*Read::getSeq(){
	return m_sequence;
}


char*Read::getId(){
	return NULL;
}




int Read::length(){
	return strlen(m_sequence);
}

/*                      
 *           -----------------------------------
 *           -----------------------------------
 *                     p p-1 p-2               0
 */
VertexMer Read::Vertex(int pos,int w,char strand){
	VertexMer key=0;
	if(strand=='F'){
		for(int i=0;i<w;i++){
			char a=m_sequence[pos+i];
			uint64_t mask=0;
			switch(a){
				/*
				case 'A':
					mask=0;
					break;
				*/
				case 'T':
					mask=1;
					break;
				case 'C':
					mask=2;
					break;
				case 'G':
					mask=3;
					break;
			}
			key=key|(mask<<(2*i));
		}
	}else{
		for(int i=0;i<w;i++){
			char a=m_sequence[strlen(m_sequence)-1-i-pos];
			uint64_t mask=0;

		// TODO: change to switch.
		//
			if(a=='A'){
				mask=1;
			}else if(a=='T'){
				mask=0;
			}else if(a=='C'){
				mask=3;
			}else if(a=='G'){
				mask=2;
			}

			key=key|(mask<<(2*i));
		}
	}

	return key;
}