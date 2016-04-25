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
    bool operator() (const Realm * x, const Realm * y){
        return x->distance > y->distance;
    }
};

// input parameters are 2 strings, output is minimum number of swaps to get from 1 string to another
int min_swaps_needed(string first, string second) {

	// Empty 2D array is created to store the previously computed optimal solutions
	int swaps[first.length()+1][second.length()+1];

	// first row is set to 0,1,2,3.....first.length()-1
	for(int i = 0; i < first.length()+1; i++) {
		swaps[i][0] = i;
	}

	// first column is set to 0,1,2,3.... second.length()-1
	for(int j = 0; j < second.length()+1; j++) {
		swaps[0][j] = j;
	}
	/** after the above 2 operations we have something like this, the goal of the next 2 for loops is to build up the rest of the 2D array
	 *
	 *  0 1 2 3 4 5
	 *  1 0 0 0 0 0
	 *  2 0 0 0 0 0
	 *  3 0 0 0 0 0
	 *
	 **/

	// now we itterate through the rest of the 2D array and we build it bottom up using the initial row and column
	for(int i = 1; i < second.length()+1; i++) {
		for(int j = 1; j < first.length()+1; j++) {

			// we compare 2 elements in the string
			if(second.at(i-1) == first.at(j-1)) {
				// we have a match, we use the previous optimal amount which is in the top left adjacent location
				swaps[j][i] = swaps[j-1][i-1];
			}
			
			// no match, we take the minimum of the three previous optimal solutions and add 1
			else {
				// the min() function in C++ compares only 2 things at once so I had to run it twice since we have to find the min of 3 elements
				int compare = min(swaps[j][i-1], swaps[j-1][i]);
				int minimum = min(compare, swaps[j-1][i-1]);
				swaps[j][i] = minimum+1;
			}
		}
	}

	// after the whole 2D array is built, the number of swaps required is the number in the lower right location of the 2D array
	return swaps[first.length()][second.length()];

	// space complextiy of this algorithm is O(N*M)
	// time complextiy of this algorithm is O(N*M)
	// where N is the length of string 1 and M is the length of String 2
}

//dynamic algorithm to determine the number of gems necessary to complete all possible numbers of steps
//given the vector of maggi values, find the maximum ascending subsequence (not necessarily contiguous)
//return the number of gems required for each number of steps given the maximum ascending subsequence
vector<int> numGems(vector<int> mag) {

	//find the maximum maggi value (O(n));
	//this will determine the number of columns in the memo array
	int max = 0;
	for(int i = 0; i<mag.size(); i++) {
		if(mag[i] > max)
			max = mag[i];
	}

	//memo array is number of (maggi) x (maximum value + 1)
	//initialized as all zeros
	vector<int> correctSizeZeroVect(max+1,0);
	vector<vector<int> > memo(mag.size(),correctSizeZeroVect); 
	
	//Problem state: minimum maggi value allowable (column)
	//iterate through maggi vector backwards (row)
	//Decision: is it possible and advantageous to use this maggi given it's value? take the max between:
		//the steps if this maggi is not taken (the value above in memo)
		//if it can be taken, the number of steps if this maggi is taken
			//(the row above, making the minimum allowed 1 above this maggi's value)

	//fill in top row
	//0 when the value is less than min allowed - you can't use this maggi
	//1 when the value is at least min allowed - you can use this maggi
	int r = mag.size()-1;
	for(int c = max; c >= 0; c--) {	
		memo[r][c] = (mag[r] >= c);
	}	

	//iterate through memo matrix backward
	r--;
	int valAbove;
	int takingThis;
	while (r >=0 ) {
		for (int c = max; c >= 0; c--) {
			
			//valAbove = the steps if this maggi is not taken
			valAbove = memo[r+1][c];
			
			//takingThis=
			//if it can be taken, the number of steps if this maggi is taken
			//(the row above, making the minimum allowed 1 above this maggi's value)
			if (mag[r] < c) { 										//if this maggi couldn't be selected regardless,
				takingThis = 0;										//don't select it
			}									
			else {
				takingThis = 1;

				if(mag[r]+1 <= max) {								//if this maggi can be taken while leaving other selections possible
					takingThis += memo[r+1][mag[r]+1];				//add the max number of other selections from above
				}
			}
			
			//place the maximum of those in the grid
			memo[r][c] = (takingThis > valAbove) ? takingThis : valAbove;	//take the max of if you selected or rejected this maggi
		}
		r--;

	}

	//traceback
	r = 0;
	int c = 1;
	int numberGems = 0;
	vector<int> gemsUsed;
	gemsUsed.push_back(0);
	while (r < mag.size()-1) {

		//if you took this one, add his value to the gems
		if(memo[r][c] > memo[r+1][c]) {

			numberGems += mag[r];
			
			//add it to the vector
			gemsUsed.push_back(numberGems);

			//trace back
			c = mag[r]+1;
		}

		r++;
	}

	//if you took the last one, add it's gems and push it
	if (memo[r][c] == 1) {
		numberGems += mag[r];
		gemsUsed.push_back(numberGems);
	}

	return gemsUsed;

}

// This will find the the shortest path if possible, and print out the results
void dijkstra(Realm& start, Realm& final, vector<vector<int> >& graph, vector<Realm>& realms) {

    // Minimum Priority Queue to find the next shortest path
    priority_queue<Realm*, vector<Realm*>, compare> minQue;

    // Assign start node a distance of zero and add it to queue
    start.distance = 0;
    minQue.push(&start);

    // Add all but start realm to unvisted list
    queue <Realm*> unvisted;

    for (int i = 0; i < realms.size(); i++){
        if (realms[i].name != start.name) unvisted.push(&realms[i]);
    }


    // The current thing we're visiting
    Realm *current;

    // The actual dijkstra algorithim
    while (!minQue.empty()) {

        // Make current the next shortest path
        current = minQue.top();
        //cout << "Current: " << current->numId << endl;
        minQue.pop();

        // Break if we have destination
        if (final.name == current->name) break;

        /* // Empty the priority queue so the new distances can be added
        while (minQue.empty() == false) {
            minQue.pop();
        } */

                // Go through each unvisted Realm
        for (int i = 0; i < unvisted.size();){ // May have an issue with size, seeing it's being chnaged in loop
         	// If we're at the current node, ignore it, don't push it back on to the queue
             if (unvisted.front()->numId != current->numId){ // May want to make this -1 instead of NULL
                 // Get the new distance
                 int newDistance = current->distance + graph[current->numId][unvisted.front()->numId];
                 // If new distance is less than previous, change previous and add to priority queue
                 if (newDistance < unvisted.front()->distance && unvisted.front()->gems_required.size() > graph[current->numId][unvisted.front()->numId]){
                     unvisted.front()->lastRealmId = current->numId;
                     unvisted.front()->distance = newDistance;
                    // Add to priority queue
                    minQue.push(unvisted.front());

                }

                // Add realm back to unvisted queue, because it still might have a shorter path
                unvisted.push(unvisted.front());
                i++;
            }
            unvisted.pop();
        }
    }
}

int main() {
	// The number of realms in the world
	int N;
	cin >> N;

	// Vector storing list of all realms
	vector<Realm> realms;

	// Input all realm info and store it
	for (int i = 0; i < N; i++) {
		Realm r;
		string name;
		cin >> name;

		// Get the number of magi per realm, and add them to a vector
		vector<int> magi;
		int num_magi;
		cin >> num_magi;

		for (int j = 0; j < num_magi; j++) {
			int num;
			cin >> num;
			magi.push_back(num);
		}

		// Assemble the realm object and add to vector
		r.name = name;
		r.magi = magi;
		r.numId = i;
		realms.push_back(r);
	}

	// Get start and destination realms
	string start_name, end_name;
	cin >> start_name >> end_name;

	// Figure out what their indexes are
	int start, end;
	for (int i = 0; i < N; i++) {
		if (start_name == realms[i].name) start = i;
		if (end_name == realms[i].name) end = i;
	}

	vector<vector<int> > graph( N, vector<int>(N,0) );


	// Precompute graph edges
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


	// Do Dijkstras from end to start
	dijkstra(realms[end], realms[start], graph, realms);

	// See if we have a path
	if (realms[start].distance == numeric_limits<int>::max()) {
		cout << "IMPOSSIBLE" << endl;
	} else {
		int total_gems = 0;
		Realm current = realms[start];

		// If we do, backtrace to find number of gems
		while (current.numId != realms[end].numId) {
			total_gems += current.gems_required[graph[current.lastRealmId][current.numId]];
			current = realms[current.lastRealmId];
		}

		cout << realms[start].distance << " " << total_gems << endl;
	}


	// Reset tentative distances
	for (int i = 0; i < N; i++) {
		realms[i].distance = numeric_limits<int>::max();
	}


	// Do Dijkstras from start to end
	dijkstra(realms[start], realms[end], graph, realms);

	// See if we have a path
	if (realms[end].distance == numeric_limits<int>::max()) {
		cout << "IMPOSSIBLE" << endl;
	} 
	else {
		int total_gems = 0;
		Realm current = realms[end];

		// If we do, backtrace to find number of gems
		while (current.numId != realms[start].numId) {
			total_gems += current.gems_required[graph[current.lastRealmId][current.numId]];
			current = realms[current.lastRealmId];
		}

		cout << realms[end].distance << " " << total_gems << endl;
	}

    return 0;
}

/*
4
sitting
6
1 2 1 3 2 4
knitting
4
4 2 3 1
knowing
5
2 3 1 4 2
kneeding
4
1 3 4 2
sitting
kneeding
*/
