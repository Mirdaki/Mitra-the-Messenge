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

int main(){

    // Code goes here!

    return 0;
}
