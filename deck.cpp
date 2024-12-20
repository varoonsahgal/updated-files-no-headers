#include "headers/deck.hpp"
#include <iostream>
#include <algorithm>
#include <random>

// Constructs a Deck
void Deck::initializeDeck(){
    deck.clear();
    char suits[4] = {'S','H','D','C'};
    for(int i=0;i<4;i++){
        for(int j=0;j<13;j++){
            Card c(j+1,suits[i]);
            deck.push_back(c);
        }
    }
    std::random_device rd;  // Seed for the random number engine
std::mt19937 g(rd());   // Mersenne Twister engine
std::shuffle(deck.begin(), deck.end(), g);
}

// Getter Function for size of deck
int Deck::getSize(){
    return deck.size();
}

// Deals by returning one card from the deck
Card Deck::deal(){
    int val = (rand()%(deck.size()));
    Card t = deck[val];
    deck.erase(deck.begin()+val);
    return t;
}