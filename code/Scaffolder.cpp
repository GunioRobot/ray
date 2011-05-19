/*
 	Ray
    Copyright (C) 2011  Sébastien Boisvert

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

#include <ReadFetcher.h>
#include <Scaffolder.h>
#include <Message.h>
#include <vector>
#include <assert.h>
using namespace std;

void Scaffolder::constructor(StaticVector*outbox,StaticVector*inbox,RingAllocator*outboxAllocator,Parameters*parameters,
	int*slaveMode,VirtualCommunicator*vc){
	m_virtualCommunicator=vc;
	m_outbox=outbox;
	m_inbox=inbox;
	m_outboxAllocator=outboxAllocator;
	m_parameters=parameters;
	m_slave_mode=slaveMode;
	m_initialised=false;
	m_workerId=0;
}

void Scaffolder::run(){
	if(!m_initialised){
		m_initialised=true;
		m_ready=true;
		m_contigId=0;
		m_positionOnContig=0;
		m_forwardDone=false;
		m_coverageRequested=false;
	}

	m_virtualCommunicator->forceFlush();
	m_virtualCommunicator->processInbox(&m_activeWorkers);
	m_activeWorkers.clear();

	if(m_contigId<(int)m_contigs.size()){
		if(m_positionOnContig<(int)m_contigs[m_contigId].size()){
			#ifdef ASSERT
			assert(m_contigId<(int)m_contigs.size());
			assert(m_positionOnContig<(int)m_contigs[m_contigId].size());
			#endif

			VERTEX_TYPE vertex=m_contigs[m_contigId][m_positionOnContig];
			#ifdef ASSERT
			assert(m_parameters!=NULL);
			#endif
			VERTEX_TYPE reverseComplement=m_parameters->_complementVertex(vertex);
			if(!m_forwardDone){
				// get the coverage
				// if < maxCoverage
				// 	get read markers
				// 	for each read marker
				// 		if it is paired
				// 			get its pair
				// 				get the vertex for the opposite strand of the first read
				// 				get the coverage of this vertex
				// 				if < maxCoverage
				// 					get the paths that goes on them
				// 					print the linking information
				if(!m_coverageRequested){
					//cout<<"Requesting coverage contig "<<m_contigId<<"/"<<m_contigs.size()<<" position "<<m_positionOnContig<<"/"<<m_contigs[m_contigId].size()<<endl;
					uint64_t*buffer=(uint64_t*)m_outboxAllocator->allocate(1*sizeof(VERTEX_TYPE));
					buffer[0]=vertex;
					Message aMessage(buffer,1,MPI_UNSIGNED_LONG_LONG,
						m_parameters->_vertexRank(vertex),RAY_MPI_TAG_REQUEST_VERTEX_COVERAGE,m_parameters->getRank());
					m_virtualCommunicator->pushMessage(m_workerId,&aMessage);
					m_coverageRequested=true;
					m_coverageReceived=false;
				}else if(!m_coverageReceived
					&&m_virtualCommunicator->isMessageProcessed(m_workerId)){
					//cout<<"Received coverage"<<endl;
					m_receivedCoverage=m_virtualCommunicator->getResponseElements(m_workerId)[0];
					m_coverageReceived=true;
					m_initialisedFetcher=false;
					//cout<<"Coverage= "<<m_receivedCoverage<<endl;
				}else if(m_coverageReceived){
					if(m_coverageReceived<m_parameters->getPeakCoverage()){
						if(!m_initialisedFetcher){
							m_readFetcher.constructor(vertex,m_outboxAllocator,m_inbox,
							m_outbox,m_parameters,m_virtualCommunicator,m_workerId);
							m_readAnnotationId=0;
							m_initialisedFetcher=true;
							m_hasPairRequested=false;
						}else if(!m_readFetcher.isDone()){
							m_readFetcher.work();
						}else{
							if(m_readAnnotationId<(int)m_readFetcher.getResult()->size()){
								//cout<<"ReadAnnotation "<<m_readAnnotationId<<endl;
								// if is paired
								// 	get the forward and the reverse markers
								// 	get the coverage of the forward vertex
								// 	if < maxCoverage
								//	 	get the Direction
								//	 	if only 1 Direction
								//	 		if contig is not self
								//	 			get its length
								//	 			print link information
								//
								// 	get the coverage of the reverse vertex
								// 	if < maxCoverage
								//	 	get the Direction
								//	 	if only 1 Direction
								//	 		if contig is not self
								//	 			get its length
								//	 			print link information
								//

								ReadAnnotation*a=&(m_readFetcher.getResult()->at(m_readAnnotationId));
								int rank=a->getRank();
								int sequenceId=a->getReadIndex();
								char strand=a->getStrand();
								int positionOnStrand=a->getPositionOnStrand();
								if(!m_hasPairRequested){
									uint64_t*buffer=(uint64_t*)m_outboxAllocator->allocate(1*sizeof(VERTEX_TYPE));
									buffer[0]=sequenceId;
									Message aMessage(buffer,1,MPI_UNSIGNED_LONG_LONG,
									rank,RAY_MPI_TAG_HAS_PAIRED_READ,m_parameters->getRank());
									m_virtualCommunicator->pushMessage(m_workerId,&aMessage);
									m_hasPairRequested=true;
									m_hasPairReceived=false;
									//cout<<"Requests has pair?"<<endl;
								}else if(!m_hasPairReceived
								&&m_virtualCommunicator->isMessageProcessed(m_workerId)){
									m_hasPair=m_virtualCommunicator->getResponseElements(m_workerId)[0];
									m_hasPairReceived=true;
									m_pairRequested=false;
									//cout<<"Answer has pair?"<<endl;
								}else if(!m_hasPairReceived){
									return;
								}else if(!m_hasPair){
									//cout<<"No pair"<<endl;
									m_readAnnotationId++;
									m_hasPairRequested=false;
								}else if(!m_pairRequested){
									uint64_t*buffer=(uint64_t*)m_outboxAllocator->allocate(1*sizeof(VERTEX_TYPE));
									buffer[0]=sequenceId;
									Message aMessage(buffer,1,MPI_UNSIGNED_LONG_LONG,
									rank,RAY_MPI_TAG_GET_READ_MATE,m_parameters->getRank());
									m_virtualCommunicator->pushMessage(m_workerId,&aMessage);
									//cout<<"Requests Pair"<<endl;
									m_pairRequested=true;
									m_pairReceived=false;
								}else if(!m_pairReceived
								&&m_virtualCommunicator->isMessageProcessed(m_workerId)){
									vector<uint64_t> response=m_virtualCommunicator->getResponseElements(m_workerId);
									m_readLength=response[0];
									m_pairedReadRank=response[1];
									m_pairedReadIndex=response[2];
									m_pairedReadLibrary=response[3];
									m_pairReceived=true;
									m_markersRequested=false;
									//cout<<"Receives pair"<<endl;
								}else if(!m_pairReceived){
									return;
								}else if(!m_markersRequested){
									uint64_t*buffer=(uint64_t*)m_outboxAllocator->allocate(1*sizeof(VERTEX_TYPE));
									buffer[0]=m_pairedReadIndex;
									Message aMessage(buffer,1,MPI_UNSIGNED_LONG_LONG,
									m_pairedReadRank,RAY_MPI_TAG_GET_READ_MARKERS,m_parameters->getRank());
									m_virtualCommunicator->pushMessage(m_workerId,&aMessage);
									m_markersRequested=true;
									m_markersReceived=false;
								}else if(!m_markersReceived
								&&m_virtualCommunicator->isMessageProcessed(m_workerId)){
									vector<uint64_t> response=m_virtualCommunicator->getResponseElements(m_workerId);
									m_pairedReadLength=response[0];
									m_pairedForwardMarker=response[1];
									m_pairedReverseMarker=response[2];
									m_pairedForwardOffset=response[3];
									m_pairedReverseOffset=response[4];
									m_markersReceived=true;
									m_forwardDirectionsRequested=false;
								}else if(!m_markersReceived){
									return;
								}else if(!m_forwardDirectionsRequested){
									//cout<<"OriginalVertex= "<<idToWord(vertex,m_parameters->getWordSize())<<" ";
									//cout<<"ForwardMarker= "<<idToWord(m_pairedForwardMarker,m_parameters->getWordSize())<<" ";
									//cout<<"ReverseMarker= "<<idToWord(m_pairedReverseMarker,m_parameters->getWordSize())<<" "<<endl;
									uint64_t*buffer=(uint64_t*)m_outboxAllocator->allocate(1*sizeof(VERTEX_TYPE));
									buffer[0]=m_pairedForwardMarker;
									Message aMessage(buffer,1,MPI_UNSIGNED_LONG_LONG,
									m_parameters->_vertexRank(m_pairedForwardMarker),
									RAY_MPI_TAG_GET_COVERAGE_AND_DIRECTION,m_parameters->getRank());
									//cout<<"Sending "<<" RAY_MPI_TAG_GET_COVERAGE_AND_DIRECTION"<<endl;
									m_virtualCommunicator->pushMessage(m_workerId,&aMessage);
									m_forwardDirectionsRequested=true;
									m_forwardDirectionsReceived=false;
								}else if(!m_forwardDirectionsReceived
								&&m_virtualCommunicator->isMessageProcessed(m_workerId)){
									//cout<<"Received direction."<<endl;
									vector<uint64_t> response=m_virtualCommunicator->getResponseElements(m_workerId);
									m_pairedForwardMarkerCoverage=response[0];
									m_pairedForwardHasDirection=response[1];
									m_pairedForwardDirectionName=response[2];
									m_pairedForwardDirectionPosition=response[3];
									m_forwardDirectionsReceived=true;
									m_reverseDirectionsRequested=false;
									cout<<"PairedForwardMarkerCoverage "<<m_pairedForwardMarkerCoverage<<" HasDirection"<<m_pairedForwardHasDirection<<endl;

									if(m_pairedForwardHasDirection
									&&m_pairedForwardMarkerCoverage<m_parameters->getMaxCoverage()){
										cout<<"Self= "<<m_contigNames[m_contigId]<<" Other= "<<m_pairedForwardDirectionName<<endl;
									}
								}else if(!m_forwardDirectionsReceived){
									return;
								}else if(m_forwardDirectionsReceived){
									m_readAnnotationId++;
									m_hasPairRequested=false;
								}
							
							}else{
								m_forwardDone=true;
								m_reverseDone=false;
							}
						}
					}else{
						m_forwardDone=true;
						m_reverseDone=false;
					}
				}
			}else if(!m_reverseDone){
				// get the coverage
				// if < maxCoverage
				// 	get read markers
				// 	for each read marker
				// 		if it is paired
				// 			get its pair
				// 				get the vertex for the opposite strand of the first read
				// 				get the coverage of this vertex
				// 				if < maxCoverage
				// 					get the paths that goes on them
				// 					print the linking information

				m_reverseDone=true;
			}else{
				m_positionOnContig++;
				m_forwardDone=false;
				m_coverageRequested=false;
			}
		}else{
			m_contigId++;
			m_positionOnContig=0;
		}
	}else{
		//cout<<"done."<<endl;
		Message aMessage(NULL,0,MPI_UNSIGNED_LONG_LONG,MASTER_RANK,RAY_MPI_TAG_I_FINISHED_SCAFFOLDING,
			m_parameters->getRank());
		m_outbox->push_back(aMessage);
		(*m_slave_mode)=RAY_SLAVE_MODE_DO_NOTHING;
	}
}

void Scaffolder::addContig(uint64_t name,vector<uint64_t>*vertices){
	m_contigNames.push_back(name);
	m_contigs.push_back(*vertices);
}