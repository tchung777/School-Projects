/**
 * @file	LRU.cpp
 *
 * @brief This program uses the Least Recently Used paging algorithm to simulate and count page faults.
 *		  It uses a double ended queue (a deque) as a page table and linear search to find if a page is already in the deque.
 *		  It moves the most recent number use to the front of the deque.
 *
 * @author Thomas Chung
 * @date November 18, 2016
**/

#include <deque>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <ctype.h>

using namespace std;

int main(int argc, char *argv[]) {
	if(argc <  2) {
		printf("Not enough parameters.");
		return 1;
	}
	
	//Get the cache_size from argv[1]
	int cache_size = atoi(argv[1]);
	//Variable we'll hold the incoming data in
	int num;
	//Deque size
	int dCurSize = 0;
	
	//Create a double ended queue
	deque<int> deck; 
	deck.resize(cache_size);
	
	
	while(!cin.eof()) {
		cin >> num;
		
		int idx = -1;
		
		//Linear search to find a page is already in the deck.
		for(int i = 0;i < dCurSize; i++) { 
			if(deck[i] == num) {
				idx = i;
				break; 
			}
		}
		
		if(idx != -1) {
			//We found the page in the deck
			//Erase everything from beginning of the deck all the way to the index;
			deck.erase(deck.begin()+idx);
			deck.push_front(num);		
			
		} else {
			//We have a page fault
			cout << num << endl;
			
			if(dCurSize < cache_size) {
				deck.push_front(num);
				dCurSize++;
			} else {
				deck.pop_back();
				deck.push_front(num);
			}
		}
	}
	
	
	return 0;
}