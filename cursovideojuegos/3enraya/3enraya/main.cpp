#include <iostream>
#include <ctype.h>

using namespace std;

#include "cboard.h" // class's header file
#include "casciiboardrender.h" // class's header file
#include "CLocalHumanPlayer.h"
#include "CIAPlayer.h"

int main(int argc, char *argv[])
{
  CBoard board;
  CBoardRender *boardRender=new CASCIIBoardRender();
  CPlayer *p1,*p2;
  string p1name,p2name;
  int p1nivel,p2nivel;
  cout << "Curso de videojuegos. Primer Juego : tres en raya en modo ASCII" << endl;
  cout << "Autor : Oscar Hernandez Baño" << endl;
  char opcion;
  do
  {
      cout << "Menu:" << endl;
      cout << "1. Humano contra Humano" << endl;
      cout << "2. Humano contra cpu" << endl;
      cout << "3. CPU contra Humano" << endl;
      cout << "4. CPU contra CPU" << endl;
      cout << "0. Salir" << endl;
      cin >> opcion;
      if (opcion=='0')
          return 0;
  } while (opcion!='1' && opcion!='2' && opcion!='3' && opcion!='4');
  int isFinish=false;
  p1=p2=NULL;
  if (opcion=='1' || opcion=='2')
  {
    cout << "Nombre juegador 1 : ";
    cin >> p1name;
    p1=new CLocalHumanPlayer(CBoard::P1,p1name);
  }
  if (opcion=='4')
  {
      cout << "Nivel para la CPU 1:";
      cin >> p1nivel;
      p1=new CIAPlayer(CBoard::P1,"CPU1",p1nivel);
  }
  if (opcion=='1' || opcion=='3')
  {
      cout << "Nombre juegador 2 : ";
      cin >> p2name;
      p2=new CLocalHumanPlayer(CBoard::P2,p2name);
  }
  if (opcion=='3' || opcion=='4')
  {
      cout << "Nivel para la CPU 2:";
      cin >> p2nivel;
      p2=new CIAPlayer(CBoard::P2,"CPU2",p2nivel);
  }
  do
  {
      board.Init();
      boardRender->render(&board);
      CBoard::SEstado estado;
      for (;;)
      {
          p1->update(&board);
          boardRender->render(&board);
          estado=board.isFinish();
          if (estado.isfinish)
              break;
          p2->update(&board);
          boardRender->render(&board);
          estado=board.isFinish();
          if (estado.isfinish)
              break;
      }
      if (estado.isfinish)
      {
          if (estado.isempate)
              cout << p1->getName() << " y " << p2->getName() << " han empatado";
          else
          {
              CPlayer *pwiner;
              switch (estado.winer)
              {
                  case CBoard::P1:
                      pwiner=p1;
                      break;
                  case CBoard::P2:
                      pwiner=p2;
                      break;
              }
              cout << "Ha ganado " << pwiner->getName() << endl;
              pwiner->winer();
              cout << p1->getName() << " ha ganado " << p1->getScore() << " partidas" << endl;
              cout << p2->getName() << " ha ganado " << p2->getScore() << " partidas" << endl;
          }
          cout << endl;
      }      
      do
      {
        cout << "Quiere jugar otra vez (S/N) ";
        cin >> opcion;
        opcion=toupper(opcion);
      } while (opcion!='S' && opcion!='N');
      if (opcion=='N')
          isFinish=true;
  } while (!isFinish);
  delete p1;
  delete p2;
  delete boardRender;
  cout << "Pulse tecla para continuar ....";
  cin.get();
  return 0;
}
