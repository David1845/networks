//
//  main.c
//  simulationpoker
//
//  Created by David Giampa on 2018-12-09.
//  Copyright © 2018 David Giampa. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <math.h>
#define Decksize 52
#define NumPlayers 2
#define Handset 2
#define cardsDelt 3

typedef struct pokerUtil{
    char PokerPlayers[10][20];
    int skipcards;
    char pokerCards[52][7];
    int randomNumber[500];
    char flop[50];
    char AutoSetHands[32];
    int randomNumbers2[500];
}pokerutil;

typedef struct winningHand{
    int pair;
    int kicker;
    int twopair;
    int card1;
    int card2;
}winningHand;

typedef struct cardResults{
    int pair;
    int tripes;
    int quads;
    int Strength;
}cardResults[2];

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
    struct winningHand besthand;
}hands[26];


void displayFlop(pokerutil * util){
    int counter=0;
    for(int i=NumPlayers*2;i<NumPlayers*2+3;i++){
        printf("%s ",util->pokerCards[i]);
        counter++;
    }
    printf("\n");
}
void displaycards(pokerutil * util){
    int counter=0;
    for(int i=0;i<52;i++){
        printf("%s ",util->pokerCards[i]);
        counter++;
        if(counter%13==0)
            printf("\n");
    }
    printf("\n");
}
void showAllHands(hands * AllHands){
    for(int i=0;i<Handset;i++){
        for(int j=0;j<2;j++)
            printf("%s",AllHands[i]->hand[j]);
    }
}
void replace(int this, int withThat,pokerutil * util){
    char tempPokerCards[1][7];
    memcpy(tempPokerCards[0], util->pokerCards[withThat], sizeof(util->pokerCards[0]));
    memcpy(util->pokerCards[withThat], util->pokerCards[this], sizeof(util->pokerCards[0]));
    memcpy(util->pokerCards[this], tempPokerCards[0], sizeof(util->pokerCards[0]));
}

void higherOrderFirst(struct hands * AllHands){
    for(int i=0;i<Handset;i++){
        if(AllHands[i].results[0]->Strength < AllHands[i].results[1]->Strength){
            char tempPokerCards[1][20];
            memcpy(tempPokerCards[0], AllHands[i].hand[0], sizeof(AllHands[i].hand[0]));
            memcpy(AllHands[i].hand[0], AllHands[i].hand[1], sizeof(AllHands[i].hand[0]));
            memcpy(AllHands[i].hand[1], tempPokerCards[0], sizeof(AllHands[i].hand[0]));
        }
    }
    
}
void setStrengthOfHand(struct hands * AllHands){
    /// complete rest of cards
    
    for(int i =0;i<Handset;i++){
        for(int j =0;j<2;j++){
            if(AllHands[i].hand[j][0] == 'A'){
                AllHands[i].results[j]->Strength= 14;
            }
            else if(AllHands[i].hand[j][0] == 'K'){
                AllHands[i].results[j]->Strength=13;
            }
            else if(AllHands[i].hand[j][0] == 'Q'){
                AllHands[i].results[j]->Strength=12;
            }
            else if(AllHands[i].hand[j][0] == 'J'){
                AllHands[i].results[j]->Strength=11;
            }
            else if(AllHands[i].hand[j][0] == 'T'){
                AllHands[i].results[j]->Strength=10;
            }
            else if(AllHands[i].hand[j][0] == '9'){
                AllHands[i].results[j]->Strength=9;
            }
            else if(AllHands[i].hand[j][0] == '8'){
                AllHands[i].results[j]->Strength=8;
            }
            
        }
    }
    
}
void SetHands(struct hands *AllHands){
    strcat(AllHands[0].hand[0], "K");
    strcat(AllHands[0].hand[1], "Q");
    
    //strcat(AllHands[1]->hand[0], "8");
   // strcat(AllHands[1]->hand[1], "A");
     //printf("%s",AllHands[1]->hand[0]);
    AllHands[1].hand[0][0]='A';
    AllHands[1].hand[1][0]='8';
    // printf("%s",AllHands[1]->hand[1]);
}


void getRandomNumbers(pokerutil * util){
    
    for(int i = 0; i<200; i++){
        util->randomNumber[i]= rand()% (52-(2*Handset)) + 2*Handset;
        util->randomNumbers2[i] = rand()% (52-(2*Handset)) + 2*Handset;
    }
}


void harts(pokerutil * util){
    char card[8];
    for(int i=2;i<10;i++){
        sprintf(card, "%d\u2665", i);
        strcpy(util->pokerCards[i-2], card);
    }
    strcpy(util->pokerCards[8], "T\u2665");
    strcpy(util->pokerCards[9], "J\u2665");
    strcpy(util->pokerCards[10], "Q\u2665");
    strcpy(util->pokerCards[11], "K\u2665");
    strcpy(util->pokerCards[12], "A\u2665");
}


void clubs(pokerutil * util){
    char card[8];
    for(int i=2;i<10;i++){
        sprintf(card, "%d\u2660", i);
        strcpy(util->pokerCards[i+11], card);
    }
    strcpy(util->pokerCards[21], "T\u2660");
    strcpy(util->pokerCards[22], "J\u2660");
    strcpy(util->pokerCards[23], "Q\u2660");
    strcpy(util->pokerCards[24], "K\u2660");
    strcpy(util->pokerCards[25], "A\u2660");
    
}


void diamonds(pokerutil * util){
    char card[8];
    for(int i=2;i<10;i++){
        sprintf(card, "%d\u2666", i);
        strcpy(util->pokerCards[i+24], card);
    }
    strcpy(util->pokerCards[34], "T\u2666");
    strcpy(util->pokerCards[35], "J\u2666");
    strcpy(util->pokerCards[36], "Q\u2666");
    strcpy(util->pokerCards[37], "K\u2666");
    strcpy(util->pokerCards[38], "A\u2666");
}


void spades(pokerutil * util){
    char card[8];
    for(int i=2;i<10;i++){
        sprintf(card, "%d\u2663", i);
        strcpy(util->pokerCards[i+37], card);
    }
    strcpy(util->pokerCards[47], "T\u2663 ");
    strcpy(util->pokerCards[48], "J\u2663");
    strcpy(util->pokerCards[49], "Q\u2663");
    strcpy(util->pokerCards[50], "K\u2663");
    strcpy(util->pokerCards[51], "A\u2663");
    
}

int PocketPair(struct hands * AllHands){
    if(AllHands[0].hand[0][0]== AllHands[0].hand[1][0]){
        return 1;
    }
    return 0;
}

int CommunityCardPair(pokerutil * util){
    for(int j =(NumPlayers*2) ; j<NumPlayers*2+3;j++){
        for(int i=j ; i<NumPlayers*2+2;i++){
            if(util->pokerCards[j][0]==util->pokerCards[i+1][0]){
                return 1;
            }
        }
    }
    return 0;
}
void createDeck(pokerutil *util){
    harts(util);
    clubs(util);
    diamonds(util);
    spades(util);
}

void shuffle_deck(pokerutil * util){
    getRandomNumbers(util);
    for(int i=0;i<200;i++){
        replace(util->randomNumbers2[i], util->randomNumber[i], util);
    }
}

void DealHands(pokerutil * util, struct hands * AllHands){
    int pos;
    for(int j=1; j<(2*Handset)+1;j++){
        /// pos moves to the next hand
        pos =(int)(ceil((double)j/2)-1.0);
        for(int i=j; i<52;i++){
            if(util->pokerCards[i][0] == AllHands[pos].hand[(j-1)%2][0]){replace(j-1, i,util); break;}
        }
        
    }
}

void isFullHouse(hands hand, int firstCard, int secondCard){
    //printf("\n%d %d\n",firstCard,secondCard);
    /// need to fix fullhouse when community card pairs board
    if(firstCard==2 && secondCard == 1){
        hand->fullHouse++;
    }else if (secondCard == 2 && firstCard == 1){
        hand->fullHouse++;
    }else if (secondCard == 1 && firstCard == 0){
        hand->pair++;
        hand->besthand.pair = hand->results[1]->Strength;
        hand->besthand.kicker = hand->results[0]->Strength;
    }else if (firstCard == 1 && secondCard == 0){
        hand->pair++;
        hand->besthand.pair = hand->results[0]->Strength;
        hand->besthand.kicker = hand->results[1]->Strength;
    }else if (firstCard == 2 && secondCard == 0){
        hand->tripes++;
    }else if (secondCard ==2 & firstCard == 0){
        hand->tripes++;
    }else if (secondCard == 3){
        hand->quads++;
    }else if (firstCard == 3){
        hand->quads++;
    }else if (secondCard == 1 && firstCard == 1){
        hand->twopair++;
        hand->besthand.twopair++;
        hand->besthand.card1 = hand->results[0]->Strength;
        hand->besthand.card2 = hand->results[1]->Strength;
    }
}
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////// pair logic //////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

 // NumPlayers*2 skips that many postions in the deck +N is how many cards are seen
 
// how many times matches accured.

void typeOfPair(hands hand,int pair, int nextCard){
    
    if(pair==1){
        hand->results[nextCard]->pair++;
    }else if(pair ==2) {
        hand->results[nextCard]->tripes++;
    }else if(pair == 3){
        hand->results[nextCard]->quads++;
    }
}

void findPair(hands hand,pokerutil * util){
    int pair=0;
    int firstCard = 0;
    int secondCard=0;
    /// iterates through a players hand and compares it with the community cards
    for(int nextCard=0;nextCard<2;nextCard++){
        for(int i = NumPlayers*2; i<(NumPlayers*2)+cardsDelt;i++){
            if(util->pokerCards[i][0] == hand->hand[nextCard][0]){
                pair++;
            }
        }
        typeOfPair(hand,pair,nextCard);
        if(nextCard == 0){
            firstCard = pair;
        }else{
            secondCard = pair;
            isFullHouse(hand,firstCard,secondCard);
        }
        pair = 0;
    }
}

void PocketPairCalculations(hands hand,pokerutil * util){
    int pair = 0;
    for(int i = NumPlayers*2; i<(NumPlayers*2)+cardsDelt;i++){
        if(util->pokerCards[i][0] == hand->hand[0][0]){
            pair++;
        }
    }
    if(pair == 1){
        hand->tripes++;
        if(CommunityCardPair(util)){
            hand->fullHouse++;
        }
    }else if(pair == 2){
        hand->quads++;
    }
}

void handAsAnyPairOrFullHouse(int currentHand, struct hands * AllHands,pokerutil * util){
    if(PocketPair(AllHands)){
        PocketPairCalculations(&AllHands[currentHand],util);
    }else{
        findPair(&AllHands[currentHand],util);
    }
}

/////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////Straight logic////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

int* getCommunityCardNumberValue(pokerutil * util){
    int static communityCards[5];
    for(int i=NumPlayers*2;i<NumPlayers*2+cardsDelt;i++ ){
        if(util->pokerCards[i][0]== 'T'){
            communityCards[i-NumPlayers*2] = 10;
        }else if (util->pokerCards[i][0]== 'J'){
            communityCards[i-NumPlayers*2] = 11;
        }else if (util->pokerCards[i][0]=='Q'){
            communityCards[i-NumPlayers*2]=12;
        }else if(util->pokerCards[i][0]=='K'){
            communityCards[i-NumPlayers*2] = 13;
        }else if(util->pokerCards[i][0] == 'A'){
            communityCards[i-NumPlayers*2] = 14;
        }
        else{
            communityCards[i-NumPlayers*2] = atoi(&util->pokerCards[i][0]);
        }
        
    }
    
    return communityCards;
}



void Straight(hands hand,pokerutil * util){
    int *cards;
    int count=0;
    int Playercard1 = atoi(hand->hand[1]);
    cards = getCommunityCardNumberValue(util);
    
    for(int j = Playercard1+1;j<Playercard1+6;j++){
        for(int i=0;i<5;i++){
            if(j == cards[i] || atoi(hand->hand[0])==j){
                count++;
                break;
            }
        }
        
        // break statement is for ending th loop when the next card for the straight is not found
        if(j-count != Playercard1){
            break;
        }
    }
    
    int tempCount= count;
    count =0;
    
    for(int j = Playercard1-1; j>Playercard1-6;j--){
        for(int i=0;i<5;i++){
            if(j == cards[i] || atoi(hand->hand[0])==j){
                count++;
                break;
            }
        }
        
        // break statement is for ending the loop when the next card for the straight is not found
        if(j+count != Playercard1){
            break;
        }
    }
    
    if (tempCount+count > 3){
        hand->straight++;
    }
    
}

void showStrengthOfHand(struct hands * AllHands){
    for(int i =0;i<Handset;i++){
        for(int j =0;j<2;j++){
            printf("%d\n",AllHands[i].results[j]->Strength);
        }
    }
}

void setHandToZero(struct hands * AllHands){
    for(int i=0;i<Handset;i++){
        AllHands[i].besthand.card1=0;
        AllHands[i].besthand.card2=0;
        AllHands[i].besthand.kicker=0;
        AllHands[i].besthand.pair=0;
        AllHands[i].besthand.twopair=0;
    }
}

void BestPair(struct hands * AllHands,pokerutil * util){
    int currentBestPair=0;
    for(int i=1; i<Handset;i++){
        if(AllHands[i].besthand.pair > 0 && AllHands[i].besthand.pair >= AllHands[currentBestPair].besthand.pair){
            /// update: depends if the kicker plays
            if(AllHands[i].besthand.pair == AllHands[currentBestPair].besthand.pair){
                if(AllHands[i].besthand.kicker > AllHands[currentBestPair].besthand.kicker){
                    currentBestPair =i;
                }
            }else{
                currentBestPair = i;
            }
            
        }
    }
    if(AllHands[currentBestPair].besthand.pair > 0){
    printf("Top pair %d and Kicker %d\n",AllHands[currentBestPair].besthand.pair,AllHands[currentBestPair].besthand.kicker);
    displayFlop(util);
    }
    setHandToZero(AllHands);
}
void bestTwoPair(){
    
}
void StatsOfAllIterations(struct hands * AllHands){
    printf("odds of getting pair on the flop %f\n",(float)AllHands[0].pair/1000000);
    printf("odds of getting 2 pair on the flop %f\n",(float)AllHands[0].twopair/1000000);
    printf("odds of getting tripes on the flop %f\n",(float)AllHands[0].tripes/1000000);
}
void startSimulation(pokerutil * util, struct hands * AllHands){
    for(int i =0 ;i<100000;i++){
        for(int j=0; j<Handset;j++){
            handAsAnyPairOrFullHouse(j,AllHands,util);
        }
        BestPair(AllHands,util);
        shuffle_deck(util);
    }
    StatsOfAllIterations(AllHands);
}
int main(int argc, const char * argv[]) {
    // insert code here...
    //shouldbreak();
    struct hands AllHands[26];
    struct pokerUtil util;
    srand((unsigned int )time(NULL));
    SetHands(AllHands);
    createDeck(&util);
    DealHands(&util,AllHands);
    
    setStrengthOfHand(AllHands);
    higherOrderFirst(AllHands);
    setStrengthOfHand(AllHands);
    
    displaycards(&util);
    shuffle_deck(&util);
    startSimulation(&util,AllHands);
    //test(&AllHands);
    printf("%d\n", AllHands[1].pair);
   
    return 0;
}
//12006 raintree drive
//245 sherman ave

