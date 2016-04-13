# Mitra-the-Messenge
For COP3502 Project 2

## Breakdown 

### Functions

int numsteps(str a, str b) // Given two strings, return the number of operations needed to change string a to string b

int numGems(int steps, int[] maggi) // Given the number of steps to change the string and a vector of integers return the number of gems neccesary or -1 if impossible 

Create Table, 2D vector with row and column of all realms,  with the number of gems neccesary to move between two realms at the interesctions. 

Dijkstra for decididng the optimal path from the begining realm to the destination realm. 

### Class

Realm
- str name
- int[] maggi

