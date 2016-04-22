// SQUAD Project 2
// COP 3530
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <math.h>
#include <limits>

using namespace std;

struct Realm {
	string name;
	vector<int> magi;
	vector<int> gems_required;
    int distance = numeric_limits<int>::max(); // May want to default this to -1
    int numId;
    int lastRealmId;
};

struct compare {
    bool operator() (const Realm& x, const Realm& y){
        return x.distance > y.distance;
    }
};

// input parameters are 2 strings, output is minimum number of swaps to get from 1 string to another
int min_swaps_needed(string first, string second){

// Empty 2D array is created to store the previously computed optimal solutions
	int swaps[first.length()+1][second.length()+1];

// first row is set to 0,1,2,3.....first.length()-1
	for(int i=0; i<first.length()+1; i++){
		swaps[i][0] = i;}

// first column is set to 0,1,2,3.... second.length()-1
	for(int j=0; j<second.length()+1; j++){
		swaps[0][j] = j;}
/** after the above 2 operations we have something like this, the goal of the next 2 for loops is to build up the rest of the 2D array
 *
 *  0 1 2 3 4 5
 *  1 0 0 0 0 0
 *  2 0 0 0 0 0
 *  3 0 0 0 0 0
 *
 **/

// now we itterate through the rest of the 2D array and we build it bottom up using the initial row and column
		for(int i=1; i<second.length()+1; i++){
			for(int j=1; j<first.length()+1; j++){

// we compare 2 elements in the string
				if(second.at(i-1) == first.at(j-1)){
// we have a match, we use the previous optimal amount which is in the top left adjacent location
					swaps[j][i] = swaps[j-1][i-1];}

// no match, we take the minimum of the three previous optimal solutions and add 1
				else{
// the min() function in C++ compares only 2 things at once so I had to run it twice since we have to find the min of 3 elements
					int compare = min(swaps[j][i-1], swaps[j-1][i]);
					int minimum = min(compare, swaps[j-1][i-1]);
					swaps[j][i] = minimum+1;}
			}
		}
// after the whole 2D array is built, the number of swaps required is the number in the lower right location of the 2D array
	return swaps[first.length()][second.length()];

// space complextiy of this algorithm is O(N*M)
// time complextiy of this algorithm is O(N*M)
// where N is the length of string 1 and M is the length of String 2
}

vector<int> numGems(vector<int> mag){

	int max = 0;
	for(int i = 0; i<mag.size(); i++){
		if(mag[i] > max)
			max = mag[i];
	}

	vector<int> correctSizeZeroVect(max+1,0);
	vector<vector<int> > memo(mag.size(),correctSizeZeroVect); 	//mag.size-1 by max array (all zeros to begin)
																//column is min power allowed, row is index
																//fill in backwards


	int r = mag.size()-1;										//start by filling in top row
	for(int c = max; c>=0; c--)									//if this maggi's value is large enough, write a 1
		memo[r][c] = (mag[r] >= c);

	r--;
	int valAbove;
	int takingThis;
	while (r>=0){
		for(int c = max; c>=0; c--){

			valAbove = memo[r+1][c];

			if(mag[r] < c)											//if this maggi couldn't be selected regardless,
				takingThis = 0;										//don't select it
			else{
				takingThis = 1;

				if(mag[r]+1 <= max){								//if this maggi can be taken while leaving other selections possible
					takingThis += memo[r+1][mag[r]+1];				//add the max number of other selections from above
				}
			}

			memo[r][c] = (takingThis>valAbove)?takingThis:valAbove;	//take the max of if you selected or rejected this maggi
		}
		r--;

	}

	//out(memo);

	//if it took too many steps, return -1
	// if(memo[0][0] < steps)
	// 	// return -1;

	//trace back
	r = 0;
	int c = 1;
	int numGems = 0;
	vector<int> gemsUsed;
	gemsUsed.push_back(0);
	while(r<mag.size()-1){

		//if you took this one, add his value to the gems
		if(memo[r][c] > memo[r+1][c]){

			numGems += mag[r];

			gemsUsed.push_back(numGems);

			//trace back
			c = mag[r]+1;
		}

		r++;
	}



	//if you took the last one, add it's gems
	if(memo[r][c] == 1){
		numGems += mag[r];
		gemsUsed.push_back(numGems);
	}

	// for(int i = 0; i<gemsUsed.size(); i++)
	// 	cout << gemsUsed[i] << " ";

	return gemsUsed;

}

// This will find the the shortest path if possible, and print out the results
void dijkstra(Realm& start, Realm& final, vector<vector<int> >& graph, vector<Realm>& realms){

    // Minimum Priority Queue to find the next shortest path
    priority_queue<Realm, vector<Realm>, compare> minQue;

    // Assign start node a distance of zero and add it to queue
    start.distance = 0;
    minQue.push(start);

    // Add all but start realm to unvisted list
    queue <Realm> unvisted;
    for (int i = 0; i < realms.size(); i++){
        if (realms[i].name != start.name) unvisted.push(realms[i]);
    }

    // This will be used to keep track of total distance and if we reached the final realm
    Realm current;

    // The actual dijkstra algorithim
    while (minQue.empty() == false){

        // Make current the next shortest path
        current = minQue.top();
        minQue.pop();

        // Break if we have destination
        if (final.name == current.name) break;

        // Empty the priority queue so the new distances can be added
        while (minQue.empty() == false) {
            minQue.pop();
        }

        // Go through each unvisted Realm
        for (int i = 0; i < unvisted.size() - 1; i++){ // May have an issue with size, seeing it's being chnaged in loop

            // Accounting for realms with no distance
            if (unvisted.front().distance == numeric_limits<int>::max() && unvisted.front().gems_required.size() > graph[current.numId][unvisted.front().numId]){ // May want to make this -1 instead of NULL
                unvisted.front().distance = graph[current.numId][unvisted.front().numId];
            } else {
                // Get the new distance
                int newDistance = current.distance + graph[current.numId][unvisted.front().numId];
                // If new distance is less than previous, change previous and add to priority queue
                if (newDistance < unvisted.front().distance && unvisted.front().gems_required.size() > graph[current.numId][unvisted.front().numId]){
                    unvisted.front().lastRealmId = current.numId;
                    unvisted.front().distance = newDistance;
                }

                // Add realm back to unvisted queue
                unvisted.push(unvisted.front());
                minQue.push(unvisted.front());
            }
            // Pop unvisted queue
            unvisted.pop();
        }
    }

    // // Print the results
    // if (current.name != final.name){
    //     cout << "IMPOSSIBLE";
    // } else {
    //     cout << current.distance;
    // }
}

int main(){
	int N;
	cin >> N;
	vector<Realm> realms;

	for (int i = 0; i < N; i++) {
		Realm r;
		string name;
		cin >> name;

		// Get the number of magi, and add them to a vector
		vector<int> magi;
		int num_magi;
		cin >> num_magi;
		//cout << "Num Magi: " << num_magi << endl;
		for (int j = 0; j < num_magi; j++) {
			int num;
			cin >> num;
			magi.push_back(num);
		}
		//cout << "Got magi" << endl;
		r.name = name;
		r.magi = magi;
		r.numId = i;
		realms.push_back(r);
	}

	string start_name, end_name;
	cin >> start_name >> end_name;

	Realm start, end;
	for (int i = 0; i < N; i++) {
		if (start_name == realms[i].name) start = realms[i];
		if (end_name == realms[i].name) end = realms[i];
	}

	vector<vector<int> > graph( N, vector<int>(N,0) );

	for (int i = 0; i < N; i++) {
		// Calculate the number of gems required for a given number of incantations/swaps
		realms[i].gems_required = numGems(realms[i].magi);

		for (int j = 0; j < N; j++) {
			if (i != j) {
				int min_swaps = min_swaps_needed(realms[i].name, realms[j].name);
				graph[i][j] = min_swaps;
			}
		}
	}

	// Do Dijkstras from start to end
	dijkstra(start, end, graph, realms);

	if (end.distance == numeric_limits<int>::max()) {
		cout << "IMPOSSIBLE" << endl;
	} else {
		int total_gems = 0;
		Realm current = end;

		// Backtrace to find number of gems
		while (current.numId != start.numId) {
			//total_gems += gems[current.lastRealmId][current.numId];
			current = realms[current.lastRealmId];
		}



		cout << end.distance << " " << total_gems << endl;
	}

	// Reset distances
	for (int i = 0; i < N; i++) {
		realms[i].distance = numeric_limits<int>::max();
	}


	// Do Dijkstras from end to start
	dijkstra(end, start, graph, realms);

	if (start.distance == numeric_limits<int>::max()) {
		cout << "IMPOSSIBLE" << endl;
	} else {
		int total_gems = 0;
		Realm current = start;

		// Backtrace to find number of gems
		while (current.numId != end.numId) {
		//	cout << "Current: " << current.numId << endl;
	    // cout << "previous: " << current.lastRealmId << endl;
			//total_gems += gems[current.lastRealmId][current.numId];
			current = realms[current.lastRealmId];
		}

		cout << start.distance << " " << total_gems << endl;
	}


    return 0;
}
