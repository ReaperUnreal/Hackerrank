#include <iostream>
#include <time.h>
#include <math.h>
using namespace std;

#define BLACK 'b'
#define WHITE 'w'
#define DEAD '-'
#define CHOSEN 'x'

const int boardWidth = 29;
const int boardHeight = 29;
const int boardSize = boardWidth * boardHeight;

const int MAX_TRIES = 512;

const char pufferUp = [0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1,
                       1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1,
                       0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1,
                       0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1,
                       0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0];
const char pufferDown = [0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,
                         0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1,
                         0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1,
                         1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1,
                         0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1];
const int pufferWidth = 18;
const int pufferHeight = 5;
const int pufferCount = 22;

const char glider1 = [0, 1, 0,
                      0, 0, 1,
                      1, 1, 1];
const char glider2 = [0, 1, 0,
                      1, 0, 0,
                      1, 1, 1];
const char glider3 = [1, 1, 1,
                      0, 0, 1,
                      0, 1, 0];
const char glider4 = [1, 1, 1,
                      1, 0, 0,
                      0, 1, 0];
const int gliderWidth = 3;
const int gliderHeight = 3;
const int gliderCount = 5;

const char methusela = [0, 1, 0, 0, 0, 0, 0,
                        0, 0, 0, 1, 0, 0, 0,
                        1, 1, 0, 0, 1, 1, 1];
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
#if 0
   if(p.y == 0)
      return 100;

   if(p.x == 0)
      return 100;

   if(p.y == (boardHeight - 1))
      return 100;

   if(p.x == (boardWidth - 1))
      return 100;
#endif

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

Point choosePufferLocation(char player, char *board)
{
   char opponent = getOpponent(player);
   int pcount = countPlayer(player, board);
   int ocount = countPlayer(opponent, board);

   if(pcount == 0)
   {
      //1 0
      //2 3
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
         quadrant = 2;

      //get top left point
      Point p(0);
      switch(quadrant)
      {
      case 0: p = Point(boardWidth - pufferWidth - 1, 1); break;
      case 1: p = Point(1, 1); break;
      case 2: p = Point(1, boardHeight - pufferHeight - 1); break;
      case 3: p = Point(boardWidth - pufferWidth - 1, boardHeight - pufferHeight - 1); break;
      default: break;
      }
      return p;
   }

}

Point pufferGlifer(char player, char *board)
{
   return Point(0);
}

void nextMove(char player, char *board)
{
#if 0 //adjacent strategy
   Point p = findAdjacent(player, board);
#endif
#if 0 // random spread strategy
   Point p = placeSpread(player, board);
#endif
#if 1 // puffer and 3 glider strategy
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