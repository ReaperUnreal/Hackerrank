#include <iostream>
#include <ctime>
#include <cmath>
#include <cstdlib>
#include <queue>
using namespace std;

#define BLACK 'b'
#define WHITE 'w'
#define DEAD '-'
#define CHOSEN 'x'

const int boardWidth = 29;
const int boardHeight = 29;
const int boardSize = boardWidth * boardHeight;

const int MAX_TRIES = 512;

const char pufferUp[90] = {0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1,
                           1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1,
                           0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1,
                           0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1,
                           0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0};
const char pufferDown[90] = {0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,
                             0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1,
                             0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1,
                             1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1,
                             0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1};
const int pufferWidth = 18;
const int pufferHeight = 5;
const int pufferCount = 22;

const char glider1[9] = {0, 1, 0,
                         0, 0, 1,
                         1, 1, 1};
const char glider2[9] = {0, 1, 0,
                         1, 0, 0,
                         1, 1, 1};
const char glider3[9] = {1, 1, 1,
                         0, 0, 1,
                         0, 1, 0};
const char glider4[9] = {1, 1, 1,
                         1, 0, 0,
                         0, 1, 0};
const int gliderWidth = 3;
const int gliderHeight = 3;
const int gliderCount = 5;

const char methusela[21] = {0, 1, 0, 0, 0, 0, 0,
                            0, 0, 0, 1, 0, 0, 0,
                            1, 1, 0, 0, 1, 1, 1};
const int methuselaWidth = 7;
const int methuselaHeight = 3;
const int methuselaCount = 7;

class Point
{
public:
   Point(int a, int b) : x(a), y(b)
   {
   }

   Point(int i) : x(i % boardWidth), y(i / boardWidth)
   {
   }

   Point transpose()
   {
      return Point(y, x);
   }

   int index()
   {
      return x + y * boardWidth;
   }

   void setIndex(int i)
   {
      x = i % boardWidth;
      y = i / boardWidth;
   }

   bool isValid()
   {
      if((x < 0) || (x >= boardWidth) || (y < 0) || (y >= boardHeight))
         return false;
      return true;
   }

   bool isDead(char *board)
   {
      int i = x + y * boardWidth;
      return (board[i] == DEAD);
   }

   bool isWhite(char *board)
   {
      int i = x + y * boardWidth;
      return (board[i] == WHITE);
   }

   bool isBlack(char *board)
   {
      int i = x + y * boardWidth;
      return (board[i] == BLACK);
   }

   bool is(char type, char *board)
   {
      int i = x + y * boardWidth;
      return (board[i] == type);
   }

   friend ostream& operator<<(ostream &os, const Point &p);
   int x;
   int y;
};

class IndexScore
{
public:
   IndexScore(int i, int s) : index(i), score(s)
   {
   }

   IndexScore() : index(0), score(0)
   {
   }

   bool operator<(const IndexScore &is) const
   {
      return score < is.score;
   }

   int index;
   int score;
};

ostream& operator<<(ostream &os, const Point &p)
{
   os << p.x << " " << p.y << endl;
   return os;
}

char getOpponent(char player)
{
   if(player == WHITE)
      return BLACK;
   return WHITE;
}

Point findRandomDead(char *board)
{
   int i = rand() % boardSize;
   while(!Point(i).isDead(board))
      i = rand() % boardSize;
   return Point(i);
}

int countPlayerNeighborhood(char player, char *board, Point p)
{
   int count = 0;

   Point t = p;
   
   //top row
   t.y--;
   t.x--;
   for(int i = 0; i < 3; i++, t.x++)
      if(t.isValid() && t.is(player, board))
         count++;
   t = p;
   t.x -= 1;

   //middle row
   if(t.isValid() && t.is(player, board))
      count++;
   t.x += 2;
   if(t.isValid() && t.is(player, board))
      count++;

   //bottom row
   t = p;
   t.y++;
   t.x--;
   for(int i = 0; i < 3; i++, t.x++)
      if(t.isValid() && t.is(player, board))
         count++;

   return count;
}

Point findAdjacent(char player, char *board)
{
   for(int tries = 0; tries < MAX_TRIES; tries++)
   {
      int i = rand() % boardSize;
      Point p(i);
      if(p.isDead(board))
      {
         int count = countPlayerNeighborhood(player, board, p);
         if(count > 0 && count < 3)
            return p;
      }
   }
   return findRandomDead(board);
}

void dumpBoard(char player, char *board)
{
   cerr << player << endl;
   for(int j = 0; j < boardHeight; j++)
   {
      for(int i = 0; i < boardWidth; i++)
         cerr << board[j * boardWidth + i];
      cerr << endl;
   }
}

int countPlayer(char player, char *board)
{
   int count = 0;
   for(int i = 0; i < boardSize; i++)
   {
      if(board[i] == player)
         count++;
   }
   return count;
}

Point findSinglePlayerPoint(char player, char *board)
{
   // find the single point that player has
   for(int i = 0; i < boardSize; i++)
      if(board[i] == player)
         return Point(i);

   //not found? error
   return Point(-1, -1);
}

int getPointQuadrant(Point &p)
{
   //1 0
   //2 3
   if(p.x < (boardWidth / 2))
   {
      if(p.y < (boardHeight / 2))
         return 1;
      return 2;
   }
   else
   {
      if(p.y < (boardHeight / 2))
         return 0;
      return 3;
   }
   return -1;
}

int getOppositeQuadrant(int q)
{
   //1 0
   //2 3
   int oq = 0;
   switch(q)
   {
   case 0: oq = 2; break;
   case 1: oq = 3; break;
   case 2: oq = 0; break;
   case 3: oq = 1; break;
   default: oq = -1; break;
   }

   return oq;
}

Point randomPointInQuadrant(char *board, int q)
{
   int i = rand() % boardSize;
   Point p(i);
   while(!(p.isDead(board) && (getPointQuadrant(p) == q)))
   {
      i = rand() % boardSize;
      p.setIndex(i);
   }

   return p;
}

float getSumPointDistance(char player, char *board, Point &p)
{
   float sum = 0.0f;
   for(int i = 0; i < boardSize; i++)
   {
      if(board[i] == player)
      {
         Point bp(i);
         float x = static_cast<float>(p.x - bp.x);
         float y = static_cast<float>(p.y - bp.y);
         float d = sqrtf(x * x + y * y);
         sum += d;
      }
   }

   return sum;
}

Point getGoodSpreadPoint(char player, char *board, int pcount)
{
   int mini = rand() % boardSize;
   Point minPoint = Point(mini);
   float minDist = 100000000.0f;

   //do MAX_TRIES trials to get a decently close point that's not too close
   for(int tries = 0; tries < MAX_TRIES; tries++)
   {
      int i = rand() % boardSize;
      Point pi(i);
      if(pi.isDead(board))
      {
         int icount = countPlayerNeighborhood(player, board, pi);
         if(icount < 3)
         {
            float dist = getSumPointDistance(player, board, pi);
            if(pcount > 20)
               dist *= static_cast<float>(3 - icount);
            if(dist < minDist)
            {
               minDist = dist;
               mini = i;
               minPoint = pi;
            }
         }
      }
   }

   return minPoint;
}

Point getAttackLocation(char player, char *board, int pcount)
{
   char opponent = getOpponent(player);

   //try every point on the map, and if that fails, just get a good point spread point
   for(int i = 0; i < boardSize; i++)
   {
      Point pi(i);
      if(pi.isDead(board))
      {
         int picount = countPlayerNeighborhood(player, board, pi);
         int oicount = countPlayerNeighborhood(opponent, board, pi);
         if((picount < 3) && (oicount == 3))
            return pi;
      }
   }

   //failed to get a decent point, default to a new point for myself
   return getGoodSpreadPoint(player, board, pcount);
}

Point getAttackLocationNoFallback(char player, char *board)
{
   char opponent = getOpponent(player);
   priority_queue<IndexScore> attackLocations;

   //try every point on the map, and if that fails, just get a good point spread point
   for(int i = 0; i < boardSize; i++)
   {
      Point pi(i);
      if(pi.isDead(board))
      {
         int picount = countPlayerNeighborhood(player, board, pi);
         if(picount < 3) // don't want to hurt ourselves by accident
         {
            int oicount = countPlayerNeighborhood(opponent, board, pi);
            attackLocations.push(IndexScore(i, oicount));
         }
      }
   }

   IndexScore topAttackLocation = attackLocations.top();

   cerr << "Attack " << topAttackLocation.score << endl;

   return Point(topAttackLocation.index);
}

Point placeSpread(char player, char *board)
{
   char opponent = getOpponent(player);
   int pcount = countPlayer(player, board);
   int ocount = countPlayer(opponent, board);

   if(pcount == 0)
   {
      //first placement
      //select quadrant
      int quadrant = 0;
      if(ocount != 0)
      {
         //want to give myself some breathing room
         Point op = findSinglePlayerPoint(opponent, board);
         int oq = getPointQuadrant(op);
         quadrant = getOppositeQuadrant(oq);
      }
      else
         quadrant = rand() % 4;

      //get point in quadrant
      return randomPointInQuadrant(board, quadrant);
   }
   else if(true) //if(pcount < 35)
   {
      //main strategy
      //get a point within a good distance
      Point p = getGoodSpreadPoint(player, board, pcount);
      return p;
   }
   else
   {
      //use last 5 turns to attack
      Point p = getAttackLocation(player, board, pcount);
      return p;
   }
}

Point choosePufferLocation(char player, char opponent, int pcount, int ocount, char *board)
{
   if(pcount == 0)
   {
      //1 0
      //2 3
      //first placement
      //select top or bottom
      bool top = false;
      if(ocount != 0)
      {
         //want to give myself some breathing room
         Point op = findSinglePlayerPoint(opponent, board);
         int oq = getPointQuadrant(op);
         if((oq == 1) || (oq == 0))
            top = false;
         else
            top = true;
      }

      //get top left point
      Point p(0);
      if(top)
         p = Point((boardWidth / 2) - (pufferWidth / 2), (boardHeight / 2) - (pufferHeight / 2));
      else
         p = Point((boardWidth / 2) - (pufferWidth / 2), (boardHeight / 2) + (pufferHeight / 2));

      cerr << "Init Puffer " << (top ? "Top" : "Bottom") << endl;

      return p;
   }
   else
   {
      Point p(0);
      int count = 0;
      do
      {
         //find the index where we're at
         int size = pufferWidth * pufferHeight;
         count = 0;
         int index;
         for(index = 0; index < size; index++)
         {
            if(pufferUp[index] == 1)
            {
               count++;
               if(count > pcount)
                  break;
            }
         }

         //find the first index
         int first;
         for(first = 0; first < size; first++)
         {
            if(pufferUp[first] == 1)
               break;
         }

         //find the point which we're using as top-left
         //it'll be the most top-left point that's our colour on the board
         int boardIndex = 0;
         for(boardIndex = 0; boardIndex < boardSize; boardIndex++)
         {
            if(board[boardIndex] == player)
               break;
         }

         //calculate the new return point
         Point boardTL(boardIndex);
         Point firstPoint(first % pufferWidth, first / pufferWidth);
         Point pufferPoint(index % pufferWidth, index / pufferWidth);

         //true top left = board top left - first puffer point
         //new point = true top left + new puffer point
         p.x = boardTL.x - firstPoint.x + pufferPoint.x;
         p.y = boardTL.y - firstPoint.y + pufferPoint.y;

         //increment for next iteration, just in case
         pcount++;
      }
      while(!p.isDead(board)); //make sure the point is dead

      if(p.isDead(board))
      {
         cerr << "Puffer " << count << endl;
         return p;
      }

      //fallback is to attack
      cerr << "Fallback ";
      return getAttackLocationNoFallback(player, board);
   }
}

Point pufferGlider(char player, char *board)
{
   //get the base info that every step needs
   char opponent = getOpponent(player);
   int pcount = countPlayer(player, board);
   int ocount = countPlayer(opponent, board);

   Point p(0);
   if(pcount < pufferCount)
      p = choosePufferLocation(player, opponent, pcount, ocount, board);
   else
      p = getAttackLocationNoFallback(player, board);
   return p;
}

void nextMove(char player, char *board)
{
#if 0 //adjacent strategy
   Point p = findAdjacent(player, board);
#endif
#if 0 // random spread strategy
   Point p = placeSpread(player, board);
#endif
#if 1 // puffer, methusela, and 2 glider strategy
   Point p = pufferGlider(player, board);
#endif
#if 1 //debug
   board[p.index()] = CHOSEN;
   dumpBoard(player, board);
#endif
   cout << p.transpose();
}

void readBoard(char &player, char *board)
{
   char buffer[256];

   //load the player
   cin >> buffer;
   player = *buffer;

   //load the board
   for(int j = 0; j < boardHeight; j++)
   {
      cin >> buffer;
      for(int i = 0; i < boardWidth; i++)
         board[j * boardWidth + i] = buffer[i];
   }
}

int main()
{
    char player;
    char board[boardSize];
    srand(time(NULL));

    readBoard(player, board);

    nextMove(player, board);

    return 0;
}
