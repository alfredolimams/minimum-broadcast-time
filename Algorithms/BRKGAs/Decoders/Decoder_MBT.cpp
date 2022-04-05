/*
 * Decoder_MBT.cpp
 *
 *  Created on: Jan 14, 2011
 *      Author: rtoso
 */

#include "Decoder_MBT.hpp"

using namespace std;

Decoder_MBT::Decoder_MBT() {
}

Decoder_MBT::~Decoder_MBT() {
	this->graph = NULL;
	this->timer = NULL;	
}

bool Decoder_MBT::init(UnweightedGraph * graph, BossaTimer * timer, int lb){
	
	this->graph = graph;
	this->timer = timer;
	this->lb = lb;
	this->bestAnswer = 1<<20;
	this->timer->reset();
	this->timer->start();

	return true;
}

void Decoder_MBT::setBestAnswer(int value) const {
	if( value < this->bestAnswer ){
		this->bestAnswer = value;
		this->timeAnswer = timer->getTime();
	}
}

int Decoder_MBT::getBestAnswer() {
	return this->bestAnswer;
}

double Decoder_MBT::getTimeAnswer() {
	return this->timeAnswer;
}

bool Decoder_MBT::stop() const {
	bool can_stop = false;
	can_stop |= (this->bestAnswer == this->lb);
	can_stop |= ((this->bestAnswer != 1<<20) and (this->timer->isTimeExpired()));
	return can_stop;
}

double Decoder_MBT::getPe(){
	return pe;
}

double Decoder_MBT::getPm(){
	return pm;
}

double Decoder_MBT::getRhoe(){
	return rhoe;
}