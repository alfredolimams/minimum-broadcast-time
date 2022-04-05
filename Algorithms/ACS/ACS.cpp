#include "ACS.hpp"

ACS::ACS(UnweightedGraph * graph, BossaTimer * timer){
	this->graph = graph;
	this->timer = timer;
	this->bestAnswer = 1<<20;
	this->lb = graph->LBB_BFS();
}

ACS::~ACS(){
	
}

bool ACS::stop(){
	bool can_stop = false;
	can_stop |= (this->bestAnswer == this->lb);
	can_stop |= ((this->bestAnswer != 1<<20) and (this->timer->isTimeExpired()));
	return can_stop;
}

void ACS::setBestAnswer(int value){
	if( value < bestAnswer ){
		bestAnswer = value;
		this->timer->lap();
	}
}

vector<pair<int,int>> ACS::possibleBroadcasts(set<int> & sources){
	vector<pair<int,int>> edges;
	for ( auto v : sources ){
		for ( auto u : this->graph->getNeighbors(v) ){
			if( !sources.count(u) ){
				edges.push_back( {v,u} );
			}
		}
	}
	return edges;
}


double ACS::computeEdgeValue(int type, int V, int i, int j, int p,
							vector<unordered_set<int>> &nm, 
							vector<unordered_set<int>> &nwm, 
							vector<unordered_set<int>> &nwms ){

	double S = ( p ? this->pheromone[i][j] : 1.0 ) * ( V / (nm[i].size()+1.0) );
	double T1 = ( p ? this->pheromone[i][j] : 1.0 ) * ( V / (nm[j].size()+1.0) );
	double T2 = ( p ? this->pheromone[i][j] : 1.0 ) * ( (nwm[j].size()+1.0) / V );
	double T3 = ( p ? this->pheromone[i][j] : 1.0 ) * ( (nwms[j].size()+1.0) / V );

	switch (type){
		case 0:
			return S + T1;
		case 1:
			return S + T2;
		case 2:
			return S + T3;
		case 3:
			return S;
		case 4:
			return S;
		case 5:
			return S;
		case 6:
			return T1;
		case 7:
			return T2;
		case 8:
			return T3;
		default:
			return -1<<20;
	}

}

BroadcastSchedule ACS::buildTreeByAnt(int type, int hasMutation){

	BroadcastSchedule schedule;

	if (this->stop()) {
		return schedule;
	}

	int qtVertex = this->graph->numberVertices();
	bool found = false;
	vector<unordered_set<int>> nmList(qtVertex);
	vector<unordered_set<int>> nwmList(qtVertex);
	vector<unordered_set<int>> nwmsList(qtVertex);

	set<int> sList;
	set<int> tList;
	set<int> gateways;
	for ( auto s : this->graph->getSources() ){
		gateways.insert(s);
	}

	// Initialize ant
	for (int v = 0; v < qtVertex; ++v) {
		for ( auto u : this->graph->getNeighbors(v) ){
			nwmList[v].insert(u);
			nwmsList[v].insert(u);
		}
	}

	// Execute in step 0
	for (int v = 0; v < qtVertex; ++v) {
		if( gateways.count(v) ){
			sList.insert(v);
			nwmsList[v].clear();	
			for ( auto u : this->graph->getNeighbors(v) ){
				nwmList[u].erase(v);
				nwmsList[u].erase(v);
				nmList[u].insert(v);
			}
		} else {
			tList.insert(v);
		}
	}

	int step = 0;
	vector<vector<int>> ordering(qtVertex);
	// Step 3	
	while( !found ){

		if (this->stop()) {
			break;
		}

		vector<pair<int,int>> broadcasts = possibleBroadcasts(sList);

		for (auto e : broadcasts){
			for (auto v : this->graph->getNeighbors(e.second) ){
				nwmsList[v].erase(e.second);
			}
		}

		// have-msg-list
		set<int> transmitters;
		for ( auto v : sList ) {
			if( nwmList[v].empty() ) continue;
			transmitters.insert(v);
		}

		vector<pair<int,int>> E;

		while ( !transmitters.empty() && !this->stop() ){

			vector<double> ranking;
			pair<int,int> chosenEdge;
			int index;
			if( !hasMutation ){
				for (auto e : broadcasts){
					ranking.push_back( computeEdgeValue( this->pheromone[e.first][e.second], type, 
										qtVertex, e.first, e.second,
										nmList, nwmList, nwmsList ) );
				}
				index = max_element(ranking.begin(), ranking.end()) - ranking.begin();
			} else {
				index = rand() % broadcasts.size();
			}

			chosenEdge = broadcasts[index];
			E.push_back(chosenEdge);

			transmitters.erase(chosenEdge.first); // remove from have-msg-list

			for (auto it = broadcasts.begin(); it != broadcasts.end(); it++) { // remove every e
				if( (*it).first == chosenEdge.first or (*it).second == chosenEdge.second ){
					broadcasts.erase(it--);
				}
			}

			nwmsList[chosenEdge.second].clear(); // <- { }

			for (auto v : this->graph->getNeighbors(chosenEdge.second) ){
				nwmList[v].erase(chosenEdge.second);
				nmList[v].insert(chosenEdge.second);
				if ( nwmList[v].empty() ){
					transmitters.erase(v);
				}
			}
		}
		for ( auto e : E ) {
			tList.erase(e.second);
			sList.insert(e.second);
			ordering[e.first].push_back(e.second);
		}

		++step;
		if( tList.empty() ){
			found = true;
		}
	}

	if ( found ){
		schedule = BroadcastSchedule(this->graph->getSources(), ordering);
		setBestAnswer(schedule.getMBT());
		return schedule;
	} else {
		BroadcastSchedule _;
		return _;
	}
}

BroadcastSchedule ACS::MBT(int T, int numberAnts, double tal, double alpha, double beta, double rho, double mutation){

	BroadcastSchedule betterScheduling;
	vector<BroadcastSchedule> SchedulingList;
	unordered_map<int, unordered_map<int,double>> numVisitors;
	unordered_map<int, unordered_map<int,double>> sumSteps;
	int qtVertex = this->graph->numberVertices();
	int t = 0;

	for (int v = 0; v < qtVertex; ++v) {
		for (auto u : this->graph->getNeighbors(v) ){
			this->pheromone[v][u] = tal;
			numVisitors[v][u] = 0;
			sumSteps[v][u] = 0;
		}
	}

	while ( t < T && !this->stop() ){
		int yBest = 1<<20;
		for (int k = 0; k < numberAnts ; ++k) {
			BroadcastSchedule Tk = buildTreeByAnt(numberAnts%9,  (mutation*100) < (rand()%100) );
			if ( Tk.getMBT() <= yBest ){
				if ( Tk.getMBT() < yBest ){
					SchedulingList.clear();
				}
				yBest = Tk.getMBT();
				SchedulingList.push_back(Tk);
			}
		}

		if ( SchedulingList[0].getMBT() < betterScheduling.getMBT() ){
			betterScheduling = SchedulingList[0];
		}

		if( !this->stop() ){
			for ( auto schedule : SchedulingList ){
				auto broadcasts = schedule.getSchedule();
				for( int k = 0; k < (int)broadcasts.size(); k++ ){
					for( auto e : broadcasts[k] ){
						sumSteps[e.first][e.second] = sumSteps[e.first][e.second] + 1.0/(k+1);
						++numVisitors[e.first][e.second];
					}
				}
			}

			for (int v = 0; v < qtVertex; ++v) {
				for (auto u : this->graph->getNeighbors(v) ){
					this->pheromone[v][u] = pow(this->pheromone[v][u], rho) + 
										pow(numVisitors[v][u]/(numberAnts+1), alpha) + 
										pow(sumSteps[v][u]/(numberAnts+1), beta); 
					numVisitors[v][u] = 0;
					sumSteps[v][u] = 0;
				}
			}
		}
		++t;
	}

	return betterScheduling;
}