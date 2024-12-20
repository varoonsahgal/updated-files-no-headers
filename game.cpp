#include "headers/game.hpp"
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>

Game::Game()
    : deck(),       // Default construct Deck
    player(),     // Default construct Player
    dealer(),     // Default construct Dealer
    interaction(deck, player, dealer) 
{
    logic.initializeDeck(deck);
}

//////////////* Game Starters *////
void Game::beginGame()
{
    char continuePlaying;

    do {
        // Ensure the deck is refreshed when running low on cards
        if (deck.getSize() < 36) {
            logic.initializeDeck(deck);
        }

        // Clear previous round's cards
        player.clearCards();
        dealer.clearCards();

        // Place bets
        if (!interaction.startBet(player)) {
            interaction.displayMessage(string("\nBankrupt! Game over.\n"), lightRed);
            break;
        }

        // Run the game round
        if (logic.startGame(player, dealer, deck, interaction)) {
            if (logic.dealDealer(player, dealer, deck)) {
                // Calculate and handle results
                char result = calculator.determineWinner(player, dealer);
                updateGameState(result);
            }
        }


        // Display final hands and results
        interaction.showDealerCards(dealer);
        interaction.showPlayerCards(player);

        // Display statistics
        interaction.displayStatistics(player);

        if (stat.check(player)) {
            interaction.displayHighScore();
        }

        // Prompt for another game
        continuePlaying = interaction.getContinueChoice();
    } while (continuePlaying != 'N' && continuePlaying != 'n');

    // Save the game if requested
    if (interaction.getSaveChoice()) {
        saveGame();
    }
}

//////////////* Main Method to be Called *////
void Game::beginMenu(bool rep, std::string message){
    clearscr();

    interaction.displayMessage(Print::title_blackjack(), yellow);
    interaction.displayMessage(Print::begin_menu());

    if(rep){
        interaction.displayMessage(message, red);
    }

    char c;
    std::cout<<"Input : ";
    std::cin>>c;
    switch(c){
        case '1': char nm[100];
                  std::cout<<"Enter player name: ";
                  std::cin>>nm;
                  player.setName(nm);
                  beginGame();
                  break;
        case '2': loadGame();
                  beginGame();
                  break;
        case '3': interaction.printStatistics(stat);
                  beginMenu(false, "");
                  break;
        case '4': interaction.printInstructions();
                  beginMenu(false, "");
                  break;
        case '5': exit(0);
                  break;
        default: beginMenu(true, "Invalid input.");
    }
}

//////////////* Data File Handling *////

void Game::saveGame()
{
    std::string filename;
    std::string path = "data/";

    do
    {
        std::cout<<"Enter filename: ";
        std::cin>>filename;
        std::transform(filename.begin(), filename.end(), filename.begin(), ::tolower);
    }
    while(filename.compare("statistics")==0);

    path += filename + ".bin";

    persistence.saveGame(player, path);
}

void Game::loadGame()
{
    std::string filename;
    std::string path = "data/";

    do
    {
        std::cout<<"Enter filename: ";
        std::cin>>filename;
        std::transform(filename.begin(), filename.end(), filename.begin(), ::tolower);
    }
    while (filename.compare("statistics") == 0);

    path += filename + ".bin";

    persistence.loadGame(player, path);
}

void Game::updateGameState(char result) 
{
    switch (result) {
    case 'p': // Player wins
        player.incrementWins();
        player.addCash(player.getBet() * 2);
        break;
    case 'd': // Dealer wins
        player.incrementLoses();
        break;
    case 'n': // Tie
        player.addCash(player.getBet());
        break;
    }
}
