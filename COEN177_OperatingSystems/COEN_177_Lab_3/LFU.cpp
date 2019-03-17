/**
 * @file	LFU.cpp
 *
 * @brief This program uses the Least Frequent Used paging algorithm to simulate and count page faults.
 *		  It uses a double ended queue (a deque) as a page table and linear search to find if a page is already in the deque.
 *		  There is a Page class object that holds two data: its number and how often it has been called. If a page fault has 	   
 *        been detected, a linear search runs through the deque to find the page with the least used and pushes it out.
 *
 * @author Thomas Chung
 * @date November 18, 2016
**/

#include <deque>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <ctype.h>

#define MAXINT 2147483647
using namespace std;

class Page {
	public:
		int num;
		int uses;	
};

int main(int argc, char *argv[]) {
	if(argc <  2) {
		printf("Not enough parameters.");
		return 1;
	}
	
	//Grab the size of the cache from argv[1]
	int cache_size = atoi(argv[1]);
	
	//Variable we'll hold incoming data in
	int num;
	int dCurSize = 0;
	
	//Create a double ended queue
	deque<Page> deck; 
	deck.resize(cache_size);
	
	while (!cin.eof()) {
		cin >> num;
				
		int idx = -1;
		
		for(int i = 0;i < dCurSize; i++) { //Linear search to find a page is already in the deck.
			if(deck[i].num == num) {
				idx = i;
				break; 
			}
		}
		
		Page newPage;
		newPage.num = num;
		newPage.uses = 1;
		
		if(idx != -1) {
			//We found the page in the deck
			newPage.uses = deck[idx].uses+1;
			
			//Erase everything from beginning of the deck all the way to the index;
			deck.erase(deck.begin()+idx); 
			deck.push_front(newPage);		
		} else {
			//We have a page fault
			cout<< num << endl;
			
			if(dCurSize < cache_size) {
				deck.push_front(newPage);
				dCurSize++;
			} else {
				//Find least frequently used and push it out.
				int min = deck[0].uses;
				int index = 0;
				for(int i = 0; i < dCurSize; i++) {
						if(deck[i].uses <= min) {
							min = deck[i].uses;
							index = i;
						}
				}
				
				deck.erase(deck.begin()+index);
				deck.push_front(newPage);
			}
		}
	}
	
	return 0;
}