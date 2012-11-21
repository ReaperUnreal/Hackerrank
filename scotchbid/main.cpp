#include <cstdlib>
#include <iostream>
#include <cstring>
#include <vector>
using namespace std;

void dumpState(int player, int position, vector<int> &p1Bids, vector<int> &p2Bids)
{
   cerr << player << endl;
   cerr << position << endl;

   int s = p1Bids.size();
   for(int i = 0; i < s; i++)
      cerr << p1Bids[i] << " ";
   cerr << endl;

   s = p2Bids.size();
   for(int i = 0; i < s; i++)
      cerr << p2Bids[i] << " ";
   cerr << endl;
}

void readBoard(int &player, int &position, vector<int> &p1Bids, vector<int> &p2Bids)
{
   char buffer[256];
   memset(buffer, 0, 256);

   //read the player
   cin.getline(buffer, 256);
   player = atoi(buffer);

   //read the position
   cin.getline(buffer, 256);
   position = atoi(buffer);

   //read the p1 bids
   cin.getline(buffer, 256);
   char *num = strtok(buffer, " \n");
   while(num != NULL)
   {
      int n = atoi(num);
      p1Bids.push_back(n);
      num = strtok(NULL, " \n");
   }

   //read the p2 bids
   cin.getline(buffer, 256);
   num = strtok(buffer, " \n");
   while(num != NULL)
   {
      int n = atoi(num);
      p2Bids.push_back(n);
      num = strtok(NULL, " \n");
   }
}

void calcPaid(vector<int> &p1b, vector<int> &p2b, int &p1, int &p2)
{
   int advantage = 1;
   p1 = 0;
   p2 = 0;

   // sum up all the winning bids
   int n = p1b.size();
   for(int i = 0; i < n; i++)
   {
      int b1 = p1b[i];
      int b2 = p2b[i];

      if(b1 > b2)
         p1 += b1; //player 1 wins
      else if(b2 > b1)
         p2 += b2; //player 2 wins
      else if(advantage == 1)
         p1 += b1; //player 1 wins with advantage
      else
         p2 += b2; //player 2 wins with advantage

      //toggle advantage
      advantage ^= 3;
   }
}

int uniformRemaining(int player, int pos, vector<int> &p1b, vector<int> &p2b)
{
   //find out how much money I have left
   int p1paid = 0, p2paid = 0;
   calcPaid(p1b, p2b, p1paid, p2paid);
   int remaining = 100;
   if(player == 1)
      remaining -= p1paid;
   else
      remaining -= p2paid;

   //find distance
   int distance = 0;
   if(player == 1)
      distance = pos;
   else
      distance = 10 - pos;

   //pay average remaining
   int pay = remaining / distance;
   if(pay <= 0)
      pay = 1;

   return pay;
}

bool haveAdvantage(int player, int n)
{
   int playerAdvantage = (n % 2) + 1;
   return (player == playerAdvantage);
}

int uniformRemainingWithDraw(int player, int pos, vector<int> &p1b, vector<int> &p2b)
{
   //what turn number are we on
   int turn = p1b.size();

   //find out how much money I have left
   int p1paid = 0, p2paid = 0;
   calcPaid(p1b, p2b, p1paid, p2paid);
   int remaining = 100;
   int opponentRemaining = 100;
   if(player == 1)
   {
      remaining -= p1paid;
      opponentRemaining -= p2paid;
   }
   else
   {
      remaining -= p2paid;
      opponentRemaining -= p1paid;
   }

   //find distance
   int distance = 0;
   if(player == 1)
      distance = pos;
   else
      distance = 10 - pos;

   //init pay
   int pay = 0;

   //check if about to lose
   if(distance >= 9)
   {
      //try and win and force a draw
      if(remaining > opponentRemaining)
      {
         //bid only as much as we need to
         pay = haveAdvantage(player, turn) ? opponentRemaining : opponentRemaining + 1;
         cerr << "force draw with upper hand" << endl;
      }
      else
      {
         //bet it all
         pay = remaining;
         cerr << "force draw" << endl;
      }
   }
   else
   {
      //average remaining
      pay = remaining / distance;
      cerr << "uniform" << endl;
   }

   //make sure we pay the right amount
   return (pay <= 0) ? 1 : pay;
}

void calcBid(int player, int position, vector<int> &p1Bids, vector<int> &p2Bids)
{
#if 0 // uniform remaining strategy
   int bid = uniformRemaining(player, position, p1Bids, p2Bids);
#endif
#if 1 // uniform remaining with draw force
   int bid = uniformRemainingWithDraw(player, position, p1Bids, p2Bids);
#endif
#if 1 //DEBUG
   dumpState(player, position, p1Bids, p2Bids);
#endif
   cout << bid << endl;
}

int main()
{
   int player;
   int position;
   vector<int> p1Bids;
   vector<int> p2Bids;

   readBoard(player, position, p1Bids, p2Bids);
   calcBid(player, position, p1Bids, p2Bids);

   return 0;
}
