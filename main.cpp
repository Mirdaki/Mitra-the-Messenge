// SQUAD Project 2
// COP 3502
#include<iostream>
#include<string>
#include<math.h>

using namespace std;

// input parameters are 2 strings, output is minimum number of swaps to get from 1 string to another 
int min_swaps_needed(string first, string second){

// 2D array is created to store the previously computed optimal solutions
	int swaps[first.length()+1][second.length()+1];

// first row is set to 0,1,2,3.....first.length()-1	
	for(int i=0; i<first.length()+1; i++){
		swaps[i][0] = i;
		}

// first column is set to 0,1,2,3.... second.length()-1	
	for(int j=0; j<second.length()+1; j++){
		swaps[0][j] = j;
		}
/** after the first 2 operations we have something like this
 * 
 *  0 1 2 3 4 5
 *  1 0 0 0 0 0
 *  2 0 0 0 0 0
 *  3 0 0 0 0 0
 * 
 **/
 
// now we itterate through the remaining unassigned indexes in the 2D array
		for(int i=1; i<second.length()+1; i++){
		
			for(int j=1; j<first.length()+1; j++){
			
// we compare 2 elements in the string
				if(second.at(i-1) == first.at(j-1)){
// we have a match, we use the previous optimal amount 
					swaps[j][i] = swaps[j-1][i-1];
					}
// no match, we take the minimum of the three previous optimal solutions and add 1		
				else{
				int compare = min(swaps[j][i-1], swaps[j-1][i]);
				int minimum = min(compare, swaps[j-1][i-1]);
				swaps[j][i] = minimum+1;
				}
				
				}
			}
// the number of swaps required is the number in the lower right of the 2D array 			
			return swaps[first.length()][second.length()];
		}

int numGems(int steps, vector<int> mag){

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

	out(memo);

	//if it took too many steps, return -1
	if(memo[0][0] > steps)
		return -1;

	//trace back
	r = 0;
	int c = 1;
	int numGems = 0;
	int numSteps = 0;
	while(r<mag.size()-1){

		//if you took this one, add his value to the gems
		if(memo[r][c] > memo[r+1][c]){

			numGems += mag[r];

			//trace back
			c = mag[r]+1;
		}

		r++;
	}

	//if you took the last one, add it's gems
	if(memo[r][c] == 1){
		numGems += mag[r];
	}

	return numGems;

}

int main(){

    // Code goes here!

    return 0;
}
