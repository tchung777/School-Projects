/**
 * @file	Random.cpp
 *
 * @brief This program uses the Random paging algorithm to simulate and count page faults.
 *		  It uses a double ended queue (a deque) as a page table and linear search to find if a page is already in the deque.
 *		  There is a Page class object that holds two data: its number and how often it has been called. If a page fault has 	   
 *        been detected, a linear search runs through the deque to find the page with the least used and pushes it out.
 *
 * @author Thomas Chung
 * @date November 18, 2016
**/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdlib.h>
#include <deque>
#include <ctype.h>
#include <iostream>
#include <cerrno>
#include <err.h>

using namespace std;

class Page {
	public:
		int pageNum;
		bool chanceBit;
};


int main (int argc, char *argv[]) {
	if(argc < 2) {
			printf("Not enough parameters.");
			return 1;
	}

	/* initialize random seed: */
	srand (time(NULL));

	//use a deque to store the cache
	deque<Page> deck;

	int cacheSize = atoi(argv[1]);
	deck.resize(cacheSize);
	int num;
	int dCurSize = 0;
	int hand = -1;

	while(!cin.eof()) { //read until EOF
			cin>>num;

			//check if the page is in the cache
			int index=-1;
			for(int i = 0; i < dCurSize; i++) {
					if(deck[i].pageNum == num) {
							index = i;
							break;
					}
			}

			Page n;
			n.pageNum = num;
			n.chanceBit = true;

			if(index!=-1) {
					//page is in cache, so make sure it has a second chance
					deck[index].chanceBit=true;
			} else {
					//We have a page fault.

					cout<< num << endl;

					//if there is room, add to the cache
					if(dCurSize<cacheSize) {
							deck.push_front(n);
							dCurSize++;
					} else {
						bool done = false;
						while(!done) {
							//give them a chance
							if(deck[hand].chanceBit) {
								deck[hand].chanceBit = false;
							} else {
								//this index has no remaining chances, it will be purged
								deck.erase(deck.begin() + hand);
								deck.insert(deck.begin() + hand, n);
								done = true;
							}
						}
					}
			}
			//Set a new radom hand location
			hand = rand() % dCurSize;

	}
	return 0;
}