#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <cstring>
#include <vector>
#include <time.h>
using namespace std;

inline void toggle(int *board, int index)
{
   board[index] ^= 1;
}

class Point
{
public:
   Point(int a, int b) : x(a), y(b)
   {
   }

   Point(int i) : x(i % 8), y(i / 8)
   {
   }

   bool isValid()
   {
      return ((x >= 0) && (y >= 0) && (x < 8) && (y < 8));
   }

   int index()
   {
      return x + (y * 8);
   }

   Point transpose()
   {
      return Point(y, x);
   }

   bool operator==(const Point &p)
   {
      return ((x == p.x) && (y == p.y));
   }

   friend ostream& operator<<(ostream &os, const Point &p);

   int x;
   int y;
};

ostream& operator<<(ostream &os, const Point &p)
{
   os << p.x << " " << p.y;
   return os;
}

void dumpBoard(int player, int *board)
{
   cerr << player << endl;
   int *bp = board;
   for(int j = 0; j < 8; j++)
   {
      for(int i = 0; i < 8; i++, bp++)
      {
         if(*bp == 2)
            cerr << 'X';
         else
            cerr << *bp;
      }
      cerr << endl;
   }
}

void dumpExpected(int player, int *board, Point p)
{
   int localBoard[64];
   memcpy(localBoard, board, 64 * sizeof(int));
   int i = p.index();
   localBoard[i] = 2;

   Point t1 = p, t2 = p;
   t1.x++;
   t2.y++;

   if(t1.isValid())
      toggle(localBoard, t1.index());
   if(t2.isValid())
      toggle(localBoard, t2.index());

   dumpBoard(player, localBoard);
}

Point chooseRandomValidPoint(int *board)
{
   vector<int> valid;

   for(int i = 0; i < 64; i++)
      if(board[i] == 1)
         valid.push_back(i);

   int size = valid.size();
   int n = rand() % size;

   return Point(n);
}

int localOnSize(int *board, int i)
{
   Point c(i);
   Point r = c;
   r.x++;
   Point d = c;
   d.y++;

   int count = 0;

   count += board[i];
   if(r.isValid())
      count += board[r.index()];
   if(d.isValid())
      count += board[d.index()];

   return count;
}

int randomIndex(vector<int> &v)
{
   int size = v.size();
   int i = rand() % size;
   return v[i];
}

Point chooseBiggestRandomPoint(int *board)
{
   vector<int> size3;
   vector<int> size2;
   vector<int> size1;

   //categorize all the points
   for(int i = 0; i < 64; i++)
   {
      if(board[i] == 1)
      {
         int n = localOnSize(board, i);
         if(n == 3)
            size3.push_back(i);
         else if(n == 2)
            size2.push_back(i);
         else if(n == 1)
            size1.push_back(i);
      }
   }

   //choose from best
   int i = 0;
   if(!size3.empty())
      i = randomIndex(size3);
   else if(!size2.empty())
      i = randomIndex(size2);
   else
      i = randomIndex(size1);

   return Point(i);
}

int countOn(int *board)
{
   int count = 0;
   for(int i = 0; i < 64; i++)
      count += board[i];
   return count;
}

Point chooseEndgame(int *board, int count)
{
   if(count == 1)
   {
      //obviously only one choice
      for(int i = 0; i < 64; i++)
         if(board[i] == 1)
            return Point(i);
   }
   else if(count == 2)
   {
      //find 2 last points
      int i1 = 0;
      for(; i1 < 64; i1++)
         if(board[i1] == 1)
            break;
      int i2 = i1 + 1;
      for(; i2 < 64; i2++)
         if(board[i2] == 1)
            break;

      Point p1(i1);
      Point p2(i2);

      //check if the points are right next to each other
      if((p2.x - p1.x) == 1)
      {
         //always choose the leftmost point
         return p1;
      }

      //check for bottom row special cases
      if((p1.y == 7) && (p2.y == 7))
      {
         //always pick the rightmost
         return p2;
      }
   }
   return chooseBiggestRandomPoint(board);
}

Point chooseBiggestRandomWithEndgame(int *board)
{
   int count = countOn(board);
   if(count > 2)
   {
      return chooseBiggestRandomPoint(board);
   }
   else
   {
      return chooseEndgame(board, count);
   }
}

void nextMove(int player, int *board)
{
#if 0 // totally random solution
   Point p = chooseRandomValidPoint(board);
#endif
#if 1 // best first random solution
   Point p = chooseBiggestRandomPoint(board);
#endif
#if 1 //DEBUG
   dumpExpected(player, board, p);
#endif
   cout << p.transpose() << endl;
}

void readBoard(int &player, int *board)
{
   cin >> player;

   char buffer[64];
   int *bp = board;
   for(int i = 0; i < 8; i++)
   {
      cin >> buffer;
      for(int j = 0; j < 8; j++, bp++)
         *bp = static_cast<int>(buffer[j] - '0');
   }
}

int main()
{
   int player;
   int board[64];

   srand(time(NULL));

   readBoard(player, board);
   nextMove(player, board);

   return 0;
}
