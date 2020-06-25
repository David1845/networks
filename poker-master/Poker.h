//
//  Poker.h
//  pokersim
//
//  Created by David Giampa on 2020-06-11.
//  Copyright © 2020 David Giampa. All rights reserved.
//

#ifndef Poker_h
#define Poker_h
typedef struct pokerUtil{
    char PokerPlayers[10][20];
    int skipcards;
    char pokerCards[52][7];
    int randomNumber[500];
    char CustomSet[124][7];
    char AutoSetHands[32];
    int randomNumbers2[500];
}pokerutil;

///  the best hand, gets cleard after ever iteration
typedef struct winningHand{
    int pair;
    int nextHighCard[8];
    int kicker;
    int twopair[2];
    int TwoPairKicker;
    int Str8Plays;
    int Str8;
    int fullhouseTripes;
    int fullhouseTwo;
    int flush;
    int tripes;
    int Quads;
    int tie;
}winningHand;

/// information found with respect to each card
typedef struct cardResults{
    int pair;
    int tripes;
    int quads;
    int Strength;
}cardResults[2];

/// total occurences with both cards
typedef struct hands{
    char hand[2][20];
    cardResults results[2];
    int fullHouse;
    int twopair;
    int pair;
    int tripes;
    int quads;
    int straight;
    int wonHand;
    int handTie;
    int flush;
    char flushSuit[6];
    struct winningHand besthand;
}hands[8];


void displayFlop(pokerutil * util);
void displaycards(pokerutil * util);
void printarry(int *arry);
void showStrengthOfHand(struct hands * AllHands);
void setHandToZero(struct hands * AllHands);
void showAllHands(hands * AllHands);
void replace(int this, int withThat,pokerutil * util);
void higherOrderFirst(struct hands * AllHands);
void staticFlop(pokerutil * util, struct hands * AllHands, char * CCards);
void swap(int * A,int X, int Y);
void InsertationSort(int * A,int size);
void InsertationSortHighToLow(int * A,int size);
void setStrengthOfHand(struct hands * AllHands);
void SetHands(struct hands *AllHands);
void harts(pokerutil * util);
void clubs(pokerutil * util);
void diamonds(pokerutil * util);
void spades(pokerutil * util);
void createDeck(pokerutil *util);
int PocketPair(struct hands * AllHands);
int CommunityCardPair(pokerutil * util);
void getRandomNumbers(pokerutil * util);
void shuffle_deck(pokerutil * util);
void DealHands(pokerutil * util, struct hands * AllHands);
int* getCommunityCardNumberValue(pokerutil * util);
void clearPairFound(int CardCleared, int * cards);
int tripsWithInCommunityCards(pokerutil * util);
int * TypeOfPairsWithInAllCards(pokerutil * util,hands hand,int pairLength);
int TwoPairKicker(pokerutil * util,hands hand);
void NextHigher(pokerutil * util,hands hand, int * pairs);
void isFullHouse(hands hand, pokerutil * util,int firstCard, int secondCard);
void typeOfPair(hands hand,int pair, int nextCard);
void findPair(hands hand,pokerutil * util);
void PocketPairCalculations(hands hand,pokerutil * util);
void handAsAnyPairOrFullHouse(int currentHand, struct hands * AllHands,pokerutil * util);
void getCommunityCardTwoPair(pokerutil * util);
int lowestCommunityCard(pokerutil * util);
int isCommunityCardStraight(struct hands * AllHands, pokerutil * util);
int IfAceIncludeOne(int communityCard, int hand);
int ifAceInsertOne(int * Cards);
void isStr8(struct hands * AllHands,pokerutil * util,int* bestStr8);
int BestStriaght(struct hands * AllHands,pokerutil * util);
int BestPair(struct hands * AllHands,pokerutil * util, int *currentBestPair);
int TwoPairIsTie(struct hands * AllHands,int current, int Best);
int bestTwoPair(struct hands * AllHands,pokerutil * util, int * currentBestTwoPair);
int BestTripes(pokerutil * util, struct hands * AllHands, int *pos);
int BestQuads(pokerutil * util, struct hands * AllHands, int *pos);
int BestFullHouse(pokerutil * util, struct hands * AllHands, int *pos);
void flush(pokerutil * util, struct hands * AllHands);
int bestFlush(pokerutil * util, struct hands * AllHands);
int HighCard(pokerutil * util, struct hands * AllHands,int * Besthighcard);
void StatsOfAllIterations(struct hands * AllHands);
void CalculateTiesAndWins(int * arry,struct hands * AllHands,pokerutil * util);
void startSimulation(pokerutil * util, struct hands * AllHands);
struct hands *runPokerSimulation(void);
#endif /* PokerHeader_h */
