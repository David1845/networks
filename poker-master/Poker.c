//
//  Poker.c
//  pokersim
//
//  Created by David Giampa on 2020-06-11.
//  Copyright © 2020 David Giampa. All rights reserved.
//

#include "Poker.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <math.h>
#include <pthread.h>

#define Decksize 52
#define NumPlayers 2
#define Handset 2
#define cardsDelt 5


void displayFlop(pokerutil * util){
    int counter=0;
    for(int i=NumPlayers*2;i<NumPlayers*2+cardsDelt;i++){
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

void printarry(int *arry){
    int i = 0;
    for(int i=0;i<5;i++){
            printf(" %d",arry[i]);
    }
    printf("\n");
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
        
        AllHands[i].besthand.kicker = 0;
        AllHands[i].besthand.pair = 0;
        AllHands[i].besthand.flush = 0;
        AllHands[i].besthand.twopair[0] = 0;
        AllHands[i].besthand.twopair[1] = 0;
        AllHands[i].besthand.TwoPairKicker = 0;
        AllHands[i].besthand.fullhouseTwo = 0;
        AllHands[i].besthand.fullhouseTripes = 0;
        AllHands[i].besthand.tripes = 0;
        AllHands[i].besthand.Quads = 0;
        bzero(AllHands[i].besthand.nextHighCard, sizeof(AllHands[i].besthand.nextHighCard));
        
    }
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


void staticFlop(pokerutil * util, struct hands * AllHands, char * CCards){

    util->CustomSet[0][0] = CCards[0];
    util->CustomSet[1][0] = CCards[1];
    util->CustomSet[2][0] = CCards[2];
    util->CustomSet[3][0] = CCards[3];
    util->CustomSet[4][0] = CCards[4];

    // inner for loop +j, -j are for duplicate cards being set.

    for(int j = 0;j<cardsDelt;j++){
        for(int i = NumPlayers*2+j; i < 52; i++){
            if(util->pokerCards[i][0] == util->CustomSet[j][0]){
                replace(i, NumPlayers*2+j, util);
                break;
            }
        }
    }
    displayFlop(util);
   // displaycards(util);
}

void swap(int * A,int X, int Y){
    int temp = A[X];
    A[X]= A[Y];
    A[Y] = temp;
}

void InsertationSort(int * A,int size){
    int key =0;
    
    for(int i =1;i<size;i++){
        key =i;
        while(key!=0 && A[key]<A[key-1]){
            
            swap(A,key,key-1);
            
            key = key -1;
        }
    }
}
void InsertationSortHighToLow(int * A,int size){
    int key =0;
    
    for(int i =1;i<size;i++){
        key =i;
        while(key!=0 && A[key]>A[key-1]){
            
            swap(A,key,key-1);
            
            key = key -1;
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
            else if(AllHands[i].hand[j][0] == '7'){
                AllHands[i].results[j]->Strength=7;
            }
            else if(AllHands[i].hand[j][0] == '6'){
                AllHands[i].results[j]->Strength=6;
            }
            else if(AllHands[i].hand[j][0] == '5'){
                AllHands[i].results[j]->Strength=5;
            }
            else if(AllHands[i].hand[j][0] == '4'){
                AllHands[i].results[j]->Strength = 4;
            }
            else if(AllHands[i].hand[j][0] == '3'){
                AllHands[i].results[j]->Strength = 3;
            }
            else if(AllHands[i].hand[j][0] == '2'){
                AllHands[i].results[j]->Strength=2;
            }
            

        }
    }

}

void SetHands(struct hands *AllHands){

    
    strcpy(AllHands[0].hand[0],"Q\u2660");
    strcpy(AllHands[0].hand[1],"J\u2666");
    
    strcpy(AllHands[1].hand[0],"5\u2660");
    strcpy(AllHands[1].hand[1],"6\u2666");
    
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
    strcpy(util->pokerCards[47], "T\u2663");
    strcpy(util->pokerCards[48], "J\u2663");
    strcpy(util->pokerCards[49], "Q\u2663");
    strcpy(util->pokerCards[50], "K\u2663");
    strcpy(util->pokerCards[51], "A\u2663");

}

void createDeck(pokerutil *util){
    harts(util);
    clubs(util);
    diamonds(util);
    spades(util);
}

int PocketPair(struct hands * AllHands){
    if(AllHands[0].hand[0][0]== AllHands[0].hand[1][0]){
        return 1;
    }
    return 0;
}

int CommunityCardPair(pokerutil * util){
    for(int j =(NumPlayers*2) ; j<NumPlayers*2+cardsDelt;j++){
        for(int i=j ; i<NumPlayers*2+2;i++){
            if(util->pokerCards[j][0] == util->pokerCards[i+1][0]){
                return 1;
            }
        }
    }
    return 0;
}

void getRandomNumbers(pokerutil * util){

    for(int i = 0; i<200; i++){
        util->randomNumber[i]= rand()% (52-(2*Handset)) + 2*Handset;
        util->randomNumbers2[i] = rand()% (52-(2*Handset)) + 2*Handset;
    }
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
            
            if(strcmp(util->pokerCards[i],AllHands[pos].hand[(j-1)%2])==0){
                
                replace(j-1, i,util);
                
                break;
                
            }
            
        }

    }
}

int* getCommunityCardNumberValue(pokerutil * util){
    
    int static communityCards[16];
    
    for(int i=NumPlayers*2;i<(NumPlayers*2)+cardsDelt;i++ ){
        
        if(util->pokerCards[i][0]== 'T'){
            
            communityCards[i-(NumPlayers*2)] = 10;
            
        }else if (util->pokerCards[i][0]== 'J'){
            
            communityCards[i-(NumPlayers*2)] = 11;
            
        }else if (util->pokerCards[i][0]=='Q'){
            
            communityCards[i-(NumPlayers*2)] = 12;
            
        }else if(util->pokerCards[i][0]=='K'){
            
            communityCards[i-(NumPlayers*2)] = 13;
            
        }else if(util->pokerCards[i][0] == 'A'){
            
            communityCards[i-(NumPlayers*2)] = 14;
            
        }
        else{
            
            communityCards[i-(NumPlayers*2)] = atoi(&util->pokerCards[i][0]);
            
        }

    }
    return communityCards;
}

void clearPairFound(int CardCleared, int * cards){
    static int trash =100;
    for(int i=0;i<cardsDelt+2;i++){
        if(cards[i] == CardCleared){
            cards[i] = trash++;
        }
    }
}
int tripsWithInCommunityCards(pokerutil * util){
    //J♣ J♦ A♣ K♦ J♥  tie A♦ K♣ vs A Q
    int * communitycards = getCommunityCardNumberValue(util);
    int pairFound = 0;
    int howManyPairs = 0;
    
        for(int i = 0; i < cardsDelt-1; i++){
            
            for(int j = 1; j < cardsDelt; j++){
                
                if(communitycards[i] == communitycards[j]){
                    
                    pairFound++;
                    
                    if(pairFound == 2){
                        
                        pairFound = communitycards[j];
                        return pairFound;
                        
                }
            }
        }
    }
  return 0;
}


int * TypeOfPairsWithInAllCards(pokerutil * util,hands hand,int pairLength){
    int * communitycards = getCommunityCardNumberValue(util);
    communitycards[cardsDelt] = hand->results[0]->Strength;
    communitycards[cardsDelt+1] = hand->results[1]->Strength;

    static int pairsFound[4][10];
    for(int i=0;i<10;i++){
        pairsFound[pairLength-1][i] = 0;
    }
    int count = 0;
    int twopairFound = 0;
    
    for(int i = 0; i < cardsDelt+1; ++i){

        for(int j = 1; j < cardsDelt+2; ++j){
            if(communitycards[i] == communitycards[j] && i != j){
                
                count++;
                
            }
        }
        if(count == pairLength){
            //printf("\n%d found %d\n",pairLength,communitycards[i]);
            pairsFound[pairLength-1][twopairFound] = communitycards[i];
            twopairFound++;
            clearPairFound(communitycards[i],communitycards);
        }else if(count > 0){
            clearPairFound(communitycards[i],communitycards);
        }
         count = 0;
    }
    // highest Two Pair in first postion
    InsertationSortHighToLow(pairsFound[pairLength-1],twopairFound);

    return pairsFound[pairLength-1];
}


int TwoPairKicker(pokerutil * util,hands hand){
    
    int Kicker = 0;
    int * Allcards = getCommunityCardNumberValue(util);
    Allcards[cardsDelt] = hand->results[0]->Strength;;
    Allcards[cardsDelt+1] = hand->results[1]->Strength;;
    for(int i =0;i<cardsDelt+2;i++){
        if(Allcards[i] != hand->besthand.twopair[0]){
            if(Allcards[i] != hand->besthand.twopair[1]){
                if(Allcards[i] > Kicker){
                    Kicker = Allcards[i];
                }
            }
        }
    }
    //printf("the kicker %d\n",Kicker);
    return Kicker;
}

void NextHigher(pokerutil * util,hands hand, int * pairs){
    int * Allcards = getCommunityCardNumberValue(util);
    Allcards[cardsDelt] = hand->results[0]->Strength;
    Allcards[cardsDelt+1] = hand->results[1]->Strength;
    int PlayableCards[8];
    bzero(PlayableCards,sizeof(PlayableCards));

    for(int i = 0;i<cardsDelt+2;i++){
        if(Allcards[i] != pairs[0]){
            hand->besthand.nextHighCard[i] = Allcards[i];
        }
    }
    InsertationSortHighToLow(hand->besthand.nextHighCard,8);
}

void isFullHouse(hands hand, pokerutil * util,int firstCard, int secondCard){
    //printf("\n%d %d\n",firstCard,secondCard);
    /// need to fix fullhouse when community card pairs board
    int trip = 2;
    int pair = 1;
    int quad = 4;
    int * Trips = TypeOfPairsWithInAllCards(util,hand,trip);
    int * Pairs = TypeOfPairsWithInAllCards(util,hand,pair);
    int * Quads = TypeOfPairsWithInAllCards(util,hand,quad);
 //printf("Pairs ");printarry(Pairs
    if(Quads[0] > 0){
        hand->quads++;
        hand->besthand.Quads = Quads[0];
    }
    else if(Trips[0] > 0 && (Pairs[0] > 0 || Trips[1]>0)){
        hand->fullHouse++;
        hand->besthand.fullhouseTripes = Trips[0];
        if(Pairs[0]>Trips[1]){
            hand->besthand.fullhouseTwo = Pairs[0];
        }else{
            hand->besthand.fullhouseTwo = Trips[1];
        }
    }
    else if(Trips[0] > 0 && Pairs[0] == 0){
        hand->tripes++;
        hand->besthand.tripes = Trips[0];
    }else if(Pairs[0] > 0 && Pairs[1] > 0){
        hand->twopair++;
        printf("%s %s: ",hand->hand[0],hand->hand[1]);displayFlop(util);
        if(Pairs[0] > Pairs[1]){
        hand->besthand.twopair[0] = Pairs[0];
        hand->besthand.twopair[1] = Pairs[1];
        }else{
        hand->besthand.twopair[0] = Pairs[1];
        hand->besthand.twopair[1] = Pairs[0];
        }
        hand->besthand.TwoPairKicker = TwoPairKicker(util,hand);
    }else if(Pairs[0] > 0){
        NextHigher(util,hand,Pairs);
        hand->pair++;
        hand->besthand.pair = Pairs[0];
        if(hand->results[0]->Strength == Pairs[0]){
            hand->besthand.kicker = hand->results[1]->Strength;
        }else{
            hand->besthand.kicker = hand->results[0]->Strength;
        }
        
    }
}




/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////// pair logic //////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

void typeOfPair(hands hand,int pair, int nextCard){

    if(pair == 1){
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
            
            isFullHouse(hand,util,firstCard,secondCard);
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
///////////////////////////////////Straight logic/////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////



void getCommunityCardTwoPair(pokerutil * util){
    int * communityCards = getCommunityCardNumberValue(util);
    
}

int lowestCommunityCard(pokerutil * util){
    
    int * communitycards = getCommunityCardNumberValue(util);
    
    int small = 10000;
    
    for(int i =0; i<cardsDelt;i++){
        
        if(communitycards[i] < small){
            
            small = communitycards[i];
            
        }
    }
    
    return small;
}


int isCommunityCardStraight(struct hands * AllHands, pokerutil * util){
    int * communitycards = getCommunityCardNumberValue(util);
    
    int lowestCC = lowestCommunityCard(util);
    
    int count =0;
    
    for(int i =0; i< cardsDelt; i++){
        
        if(communitycards[i] == ++lowestCC){
            
            count++;

        }else{
            break;
        }
    }
    if(count == 4){
        
        for(int i =0;i<Handset;i++){
            
            if(AllHands[i].results[0]->Strength == lowestCC ||AllHands[i].results[1]->Strength== lowestCC){
                
                //printf("%s %s ||",AllHands[i].hand[0],AllHands[i].hand[1]);
                
                return 1;
            }
        }
        return 0;
    }
    return 1;
}

int IfAceIncludeOne(int communityCard, int hand){
    
    if(communityCard == 14 || hand == 14){
        return 1;
    }
    else{
        return 0;
    }
}

int ifAceInsertOne(int * Cards){
    
    for(int i=0;i<cardsDelt+2;i++){
        
        if(Cards[i] == 14){
            
            Cards[cardsDelt+2]=1;
            
            return 1;
        }
    }
    return 0;
    
}

void isStr8(struct hands * AllHands,pokerutil * util,int* bestStr8){
    int count =0;
    int TotalStr8Cards = cardsDelt+2; // 2 is how many cards a player holds
    int * Allcards = getCommunityCardNumberValue(util);
    Allcards[cardsDelt] = AllHands->results[0]->Strength;
    Allcards[cardsDelt+1] = AllHands->results[1]->Strength;
    
    if(ifAceInsertOne(Allcards)){
        TotalStr8Cards++; // adding one to the deck
    }
    
    InsertationSort(Allcards,TotalStr8Cards);

     for(int i =0;i<TotalStr8Cards;i++){
         if(Allcards[i] == Allcards[i+1]){
             continue;
         }
         else if(Allcards[i]+1 ==Allcards[i+1]){
             count++;
         }else if(count>3){
             *bestStr8 = Allcards[i];
             count=0;
         }else{
             count=0;
         }
     }
    if(*bestStr8>0){
        AllHands->straight++;
        AllHands->besthand.Str8++;
    }
}



int BestStriaght(struct hands * AllHands,pokerutil * util){
    int beststr8=0;
    int temp=0;
    int str8Found = -1;
    int tie=0;
        for(int i =0; i<Handset;i++){
            beststr8=0;
            isStr8(&AllHands[i], util, &beststr8);
            if(beststr8 > temp){
                temp = beststr8;
                str8Found = i;
                tie =0;
            }else if(beststr8 == temp && temp!=0){
                tie = 1;
            }
        }
    if(tie ==1){
        return -2;
        
    }
    return str8Found;
}
/*
 function: BestPair
    *currentBestPair starts at 0.
        for loop starts at 1.
 */

int BestPair(struct hands * AllHands,pokerutil * util, int *currentBestPair){
    int static SameHand[16];
    memset(SameHand,-1,sizeof(SameHand));
    int SmallCc = lowestCommunityCard(util);
    int count=0;
    if(AllHands[0].besthand.pair > 0){
        SameHand[0] = 0;
        currentBestPair[0] = 0;
        count++;
    }

    for(int i=1; i<Handset;i++){
        if(AllHands[i].besthand.pair > 0 && AllHands[i].besthand.pair >= AllHands[*currentBestPair].besthand.pair){
            // if a pair was found and that pair is greater than or equal too the currentBestPair.
            if(AllHands[i].besthand.pair == AllHands[currentBestPair[0]].besthand.pair){
                // both pairs are the same
         
                if(AllHands[i].besthand.kicker < SmallCc && AllHands[currentBestPair[0]].besthand.kicker < SmallCc ){
                    //checks that the kicker is greater than the smallest community card
                    SameHand[count] = i;
                    count++;
                }
                else if(AllHands[i].besthand.kicker == AllHands[currentBestPair[0]].besthand.kicker){
                    int found=0;
                    for(int k=0;k<3;k++){
                        if(AllHands[i].besthand.nextHighCard[k] > AllHands[currentBestPair[0]].besthand.nextHighCard[k]){
                            memset(SameHand,-1,sizeof(SameHand));
                            currentBestPair[0] = i;
                            count = 0;
                            found = -1;
                        }
                    }
                    if(found == 0){
                        SameHand[count] = i;
                        count++;
                    }
                }
                else if(AllHands[i].besthand.kicker > AllHands[currentBestPair[0]].besthand.kicker){
                    memset(SameHand,-1,sizeof(SameHand));
                    currentBestPair[0] = i;
                    count=0;
                        
                }
            }else{
                    memset(SameHand,-1,sizeof(SameHand));
                    currentBestPair[0] = i;
                    count = 0;
            }
            
        }
        
    }

    if(AllHands[currentBestPair[0]].besthand.pair > 0 && SameHand[1] < 0){
        return 1;
    }else if(SameHand[1]> -1){
        for(int i =0;i<Handset;i++){
            currentBestPair[i] = SameHand[i];
        }
        return 1;
    }

    return -1;
}




int TwoPairIsTie(struct hands * AllHands,int current, int Best){
    
    if(AllHands[Best].besthand.twopair[0] == AllHands[current].besthand.twopair[0]){
        
            if(AllHands[Best].besthand.twopair[1] == AllHands[current].besthand.twopair[1]){
                
                if(AllHands[Best].besthand.TwoPairKicker == AllHands[current].besthand.TwoPairKicker){
                    
                return 0;
                    
                }else if(AllHands[Best].besthand.TwoPairKicker < AllHands[current].besthand.TwoPairKicker){
                    
                return -1;
                    
                }
            }
        }
    return -2;
}
    


int bestTwoPair(struct hands * AllHands,pokerutil * util, int * currentBestTwoPair){
    int count = 0;
    int before = 0;
    int BestTwoPair = 0;
    
    if(AllHands[BestTwoPair].besthand.twopair[0] > 0){
        currentBestTwoPair[count] = 0;
        count++;
    }
        for(int i =1; i<Handset;i++){
            if(AllHands[i].besthand.twopair[0] > AllHands[BestTwoPair].besthand.twopair[0]){
                memset(currentBestTwoPair,-1,sizeof(int)*NumPlayers);
                count =0;
                currentBestTwoPair[count] = i;
                count++;
                BestTwoPair = i;
            }else if(AllHands[i].besthand.twopair[1] > AllHands[BestTwoPair].besthand.twopair[1] && AllHands[i].besthand.twopair[0] == AllHands[BestTwoPair].besthand.twopair[0]){
                memset(currentBestTwoPair,-1,sizeof(int)*NumPlayers);
                count =0;
                currentBestTwoPair[count] = i;
                count++;
                BestTwoPair = i;
            }else if(TwoPairIsTie(AllHands,i,BestTwoPair) == 0){
                currentBestTwoPair[count] = i;
                count++;
            }else if(TwoPairIsTie(AllHands,i,BestTwoPair) == -1){
                memset(currentBestTwoPair,-1,sizeof(int)*NumPlayers);
                count =0;
                currentBestTwoPair[count] = i;
                count++;
                BestTwoPair = i;
            }

            
        }

     if(AllHands[BestTwoPair].besthand.twopair[0] > 0){
        return 1;
     }

    return -1;

}


int BestTripes(pokerutil * util, struct hands * AllHands, int *pos){
    int bestThree = 0;
    for(int i =0; i<Handset;i++){
        if(AllHands[i].besthand.tripes > bestThree){
            bestThree = AllHands[i].besthand.tripes;
            *pos = i;
        }
    }
    
    if(bestThree > 0){
        return 1;
    }else{
        return -1;
    }
}


int BestQuads(pokerutil * util, struct hands * AllHands, int *pos){
    
    int BestQuads = -1;
    for(int i=1 ;i<Handset;i++){
        if(AllHands[i].besthand.Quads > BestQuads){
            BestQuads = AllHands[i].besthand.Quads;
                *pos = i;
        }
    }
    if(BestQuads > 0){
        return 1;
    }
    return 0;
}

int BestFullHouse(pokerutil * util, struct hands * AllHands, int *pos){
    
    int FH = 0;
    int currentFH = 0;
    if(AllHands[currentFH].besthand.fullhouseTripes > 0){
        pos[FH] = 0;
        FH++;
    }
    
    for(int i =1; i<Handset;i++){
      //  printf("in full %d %d\n\n\n",AllHands[i].besthand.fullhouseTwo, AllHands[*pos].besthand.fullhouseTwo);
        if(AllHands[i].besthand.fullhouseTripes == AllHands[currentFH].besthand.fullhouseTripes && AllHands[i].besthand.fullhouseTripes > 0 ){
             //printf("tie in full house 753\n");
            if(AllHands[i].besthand.fullhouseTwo == AllHands[currentFH].besthand.fullhouseTwo){
             //   printf("tie in full house 753\n");
                pos[FH] = i;
                FH++;
            }else if(AllHands[i].besthand.fullhouseTwo > AllHands[currentFH].besthand.fullhouseTwo){
                currentFH = i;
                memset(pos,-1,NumPlayers*sizeof(int));
                FH = 0;
                pos[FH] = i;
                FH++;
            }
        }
        else if(AllHands[i].besthand.fullhouseTripes  > AllHands[currentFH].besthand.fullhouseTripes){
                memset(pos,-1,NumPlayers*sizeof(int));
                FH = 0;
                pos[FH] = i;
                FH++;
            }
    }

    if(pos[0] > -1){
        return 1;
    }else{
        return -1;
    }
        
}

void flush(pokerutil * util, struct hands * AllHands){
    ///// move suits into pokerutil
   char suits[4][6];
   int count[4];
   int PlayerSuitCount[9][4];
   bzero(PlayerSuitCount, sizeof(PlayerSuitCount));
   bzero(count, sizeof(count));
   memset(suits,'\0', sizeof(suits[0][0]) * 4 * 6);
   strcpy(suits[0],"\u2665");
   strcpy(suits[1],"\u2663");
   strcpy(suits[2],"\u2660");
   strcpy(suits[3],"\u2666");

   //// identifies the community cards suits

   for(int fourSuits = 0; fourSuits < 4; fourSuits++){
       for(int i = NumPlayers*2; i < (NumPlayers*2)+cardsDelt; i++){
            if(strstr(util->pokerCards[i], suits[fourSuits]) != NULL){
                count[fourSuits]++;
            }
       }
   }

   //// identifies the hand suits
    
   for(int fourSuits =0; fourSuits<4; fourSuits++){
       for(int i =0; i< NumPlayers;i++){
           if(strstr(AllHands[i].hand[0],suits[fourSuits]) != NULL){
               PlayerSuitCount[i][fourSuits]++;
           }
            if(strstr(AllHands[i].hand[1],suits[fourSuits]) != NULL){
               PlayerSuitCount[i][fourSuits]++;
           }
       }
   }
    
    for(int fourSuits = 0; fourSuits < 4; fourSuits++){
        for(int i=0; i<NumPlayers;i++){
            if((count[fourSuits] + PlayerSuitCount[i][fourSuits]) >= 5){
                //displayFlop(util);
                AllHands[i].besthand.flush++;
                AllHands[i].flush++;
                strcpy(AllHands[i].flushSuit,suits[fourSuits]);
              
            }
        }
    }
}


int bestFlush(pokerutil * util, struct hands * AllHands){
    
    flush(util,AllHands);
    int bestFlush=-1;
    for(int i=0; i<NumPlayers; i++){
        
        if(AllHands[i].besthand.flush > 0){
            
            if(strstr(AllHands[i].hand[0],AllHands[i].flushSuit)!=NULL){
                
                if(bestFlush != -1){
                    if(AllHands[i].results[0]->Strength > AllHands[bestFlush].results[0]->Strength)
                        bestFlush = i;
                }else
                    bestFlush = i;
                
                
            }else if (strstr(AllHands[i].hand[1],AllHands[i].flushSuit)!=NULL){
                
                if(bestFlush != -1){
                    if(AllHands[i].results[1]->Strength > AllHands[bestFlush].results[1]->Strength)
                        bestFlush = i;
                }else
                    bestFlush = i;
                              
            }else
                bestFlush = 0;
            
        }
    }

        return bestFlush;
    
}


int HighCard(pokerutil * util, struct hands * AllHands,int * Besthighcard){
    int highcard = 0;
    int tie =0;
    int lowestCC = lowestCommunityCard(util);
    for(int i=0; i<Handset;i++){
        if(AllHands[i].results[0]->Strength > highcard){
            
            if(AllHands[i].results[0]->Strength > lowestCC){
                    highcard = AllHands[i].results[0]->Strength;
                    *Besthighcard = i;
            }
            
        }else if(AllHands[i].results[0]->Strength == highcard){
            if(AllHands[i].results[1]->Strength > AllHands[*Besthighcard].results[1]->Strength){
                if (AllHands[i].results[1]->Strength > lowestCC){
                        highcard = AllHands[i].results[0]->Strength;
                        *Besthighcard = i;
                }else{
                    tie = 1;
                }
            }
            
        }
    }
    
    if(tie == 1){
        printf("its a tie\n");
    }else{
        //printf("%s %s",AllHands[*Besthighcard].hand[0], AllHands[*Besthighcard].hand[1]);
        return 1;
    }
    return -1;
}

void StatsOfAllIterations(struct hands * AllHands){
    for(int i=0 ;i<Handset;i++){
        printf("hand %s %s won #%d\n",AllHands[i].hand[0],AllHands[i].hand[1],AllHands[i].wonHand);
        printf("hand %s %s tie #%d\n",AllHands[i].hand[0],AllHands[i].hand[1],AllHands[i].handTie);

        printf("odds of getting pair on the flop %f\n",(float)AllHands[i].pair/100000);
        printf("odds of getting 2 pair on the flop %f\n",(float)AllHands[i].twopair/100000);
        printf("odds of getting tripes on the flop %f\n",(float)AllHands[i].tripes/100000);
        printf("striaght probability after %d cards delt is %f\n",cardsDelt,(float)AllHands[i].straight/100000);
        printf("flush percentage after %f cards dealt %d\n",(float)AllHands[i].flush/100000,cardsDelt);
        printf("odds of getting 2 pair on the flop %f\n",(float)AllHands[i].twopair/100000);

        printf("odds of getting fullhouse %f\n\n",(float)AllHands[i].fullHouse/100000);
        
    }
}
void CalculateTiesAndWins(int * arry,struct hands * AllHands,pokerutil * util){
             if(arry[1]>0){
                 //displayFlop(util);
                     int i =0;
                     while(arry[i]!= -1){
                         AllHands[arry[i]].handTie++;
                         i++;
                     }
             }else{
                 AllHands[arry[0]].wonHand++;
             }

}

void startSimulation(pokerutil * util, struct hands * AllHands){
    int AllTwoPair[9];
    int currentBestPair[9];
    int BestStrFound;
    int BestFH[9];
    int BestFlush;
    int BestHighcard;
    int BestThree=0;
    int Quads =0;
    int tie =0;
    for(int i =0;i<10000;i++){
        /// set all values of currentBestPair, AllTwoPair and BestFH to -1
        memset(currentBestPair,-1,sizeof(currentBestPair));
        memset(AllTwoPair,-1,sizeof(AllTwoPair));
        memset(BestFH,-1,sizeof(BestFH));
        BestStrFound = 0;
        BestFlush = 0;
        BestHighcard = 0;
        BestThree= 0;
        Quads = 0;
        
        for(int j=0; j<Handset;j++){
            
            handAsAnyPairOrFullHouse(j,AllHands,util);
        }
        BestFlush = bestFlush(util,AllHands);

        BestStrFound = BestStriaght(AllHands, util);
        

        
        if(BestQuads(util, AllHands, &Quads)){
            AllHands[Quads].wonHand++;
        }
       else if(BestFullHouse(util, AllHands,BestFH)==1){
           if(BestFH[1] > -1){
               int i =0;
               while(BestFH[i]!= -1){
                   // displayFlop(util);
                   AllHands[BestFH[i]].handTie++;
                   i++;
               }
           }else if(BestFH[0] > -1){
                AllHands[BestFH[0]].wonHand++;
           }
    
        }else if(BestFlush > -1){
            
            if(BestFlush == 0 ){
                tie++;
            }else
                AllHands[BestFlush].wonHand++;
            
        }
        else if(BestStrFound != -1){
            if(BestStrFound == -2 ){
                tie++;
            }else{
            AllHands[BestStrFound].wonHand++;
            }
            
        }else if(BestTripes(util,AllHands, &BestThree) != -1 ){
            
            AllHands[BestThree].wonHand++;
            
        }
        else if(bestTwoPair(AllHands,util,AllTwoPair) == 1){
            CalculateTiesAndWins(AllTwoPair,AllHands,util);

        }else if(BestPair(AllHands,util,currentBestPair) == 1){
            CalculateTiesAndWins(currentBestPair,AllHands,util);

            
        }else if(HighCard(util, AllHands,&BestHighcard) == 1){
            AllHands[BestHighcard].wonHand++;
            
        }
        shuffle_deck(util);
        setHandToZero(AllHands);
    }
    StatsOfAllIterations(AllHands);
}

struct hands * runPokerSimulation(){
    char fullhouseTestData[16][6];
    
    char twopairTestData[16][6];
    
    char pairTestData[16][6];
    //4♠ Q♠ 4♥ 5♠ Q♦
    strcpy(pairTestData[0],"KQ934");
    strcpy(pairTestData[1],"KAJQ9");
    strcpy(pairTestData[2],"22568");
    strcpy(pairTestData[3],"32J77");
    
    strcpy(twopairTestData[0],"AKQ23");
    strcpy(twopairTestData[1],"JJ77A");
    strcpy(twopairTestData[2],"JJ7KA");
    strcpy(twopairTestData[3],"3456A");
     //AK vs AQ
    strcpy(fullhouseTestData[0],"A6669");
    strcpy(fullhouseTestData[1],"KKQAA");
    strcpy(fullhouseTestData[6],"K7QAA");
    strcpy(fullhouseTestData[2],"JK88K");
    strcpy(fullhouseTestData[3],"99A98");
    strcpy(fullhouseTestData[4],"KQQAA");
    strcpy(fullhouseTestData[5],"88877");
    
    
   static struct hands AllHands[8];
    struct pokerUtil util;
    

    bzero(AllHands, sizeof(AllHands));
    bzero(&util, sizeof(util));

    srand((unsigned int )time(NULL));
    SetHands(AllHands);
    createDeck(&util);
    DealHands(&util,AllHands);

    
    setStrengthOfHand(AllHands);
    higherOrderFirst(AllHands);
    setStrengthOfHand(AllHands);

    staticFlop(&util, AllHands,pairTestData[3]);
    startSimulation(&util,AllHands);

    return AllHands;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
