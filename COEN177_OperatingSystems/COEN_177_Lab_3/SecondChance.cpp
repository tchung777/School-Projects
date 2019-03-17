/**
 * @file	SecondChance.cpp
 *
 * @brief This program uses the Second Chance paging algorithm to simulate and count page faults.
 *		  It uses a double ended queue (a deque) as a page table and linear search to find if a page is already in the deque.
 *		  There is a Page class object that holds two data: its number and its second chance bit. If a page fault has 	   
 *        been detected, we look at the top of the deque and see if it's second chance bit is off. If it is, remove it. If 	            
 *  	  ,move it to the end of the deque and keeps progressing through the deque until you hit a Page object with its second 
 *	      chance bit off. Then, remove it.
 *
 * @author Thomas Chung
 * @date November 18, 2016
**/

#include <iostream>
#include <deque>
#include <utility>
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

class Page {
	public:
		int num;
		bool chanceBit;
};

int main(int argc, char* argv[]) {
	if (argc < 2) {
		printf("Not enough parameters.");
		return 1;
	}
	
	
	// Cache size from argv[1]
	int cache_size = atoi(argv[1]);
	
	
	// Create a page table using a deque of Page class objects
	deque<Page> deck;
	
	// Create a iterator that will follow along with the deck
	deque<Page>::iterator hand = deck.begin();
	
	// Variable we'll hold incoming data in
	int num;
	
	while (!cin.eof()) {
		cin >> num;
		
		bool hasFound = false;
		
		
		deque< Page >::iterator myIterator;
		for (myIterator = deck.begin(); myIterator != deck.end(); myIterator++) {
			
			// If it is in the deque, set its chanceBit to true because it was called forth
			if (myIterator->num == num) {
				myIterator->chanceBit = true;
				hasFound = true;
				break;
			}

		}
		
		// If not, we've found report a page fault and need to place it in the front.
		if (!hasFound) {
			cout << num << endl;
			
			// Check if page table exceeds "cache_size" and remove entry that clock points to
			while (deck.size() >= cache_size) {
				
				for (int i = 0; i < cache_size; i++) {
					
					hand = deck.begin();
					
					// Erase or continue based on reference bit
					if (!hand->chanceBit) {
						deck.pop_front();
						break;
					}
					else if (hand->chanceBit) {
						hand->chanceBit = false;
					}
					
					// Shifts all items in the deque one to the left
					// Items in the front will wrap around the the end
					rotate(deck.begin(), deck.begin()+1, deck.end());
					
				}
				
			}
			
			Page p;
			p.num = num;
			p.chanceBit = true;
			deck.push_back(p);
		}
		
	}
	
	
	return 0;
}
