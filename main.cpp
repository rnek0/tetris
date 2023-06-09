//http://www.youtube.com/watch?v=ZUOnNbIM_dA&list=PLBF37E6885CD3A358
#include "miniwin.h"#include <cstdlib>#include <ctime>#include <sstream>using namespace std;
using namespace miniwin;
const int TAM = 25;const int FILAS = 20;const int COLUMNAS = 10;typedef int Tablero[COLUMNAS][FILAS];struct Coord{   int x,y;};struct Pieza{                          // Coordonn�es des 4 blocks de la pi�ce + couleur   Coord orig;                         // bloque central (pivot)   Coord perif[3];                     // bloques perifericos   int color;                          // couleur   Coord posicion(int n)const;              // n entre 0 y 3 (0 = carr� central, 1-3 carr�s perifericos)};/* Retourne la position d'un carr� de la pi�ce */Coord Pieza::posicion(int n)const{   Coord ret = {orig.x,orig.y};   if(n != 0){      ret.x += perif[n-1].x;      ret.y += perif[n-1].y;   }   return ret;}void cuadrado(int x,int y){            // Simple carr�   rectangulo_lleno(20+1+x*TAM,                    20+1+y*TAM,                    20+x*TAM+TAM,                    20+y*TAM+TAM);}/* Dessine la pi�ce */void pinta_pieza(const Pieza& P){   color(P.color);// Couleur de la pi�ce   for(int i = 0; i<4; i++){         Coord c = P.posicion(i);         cuadrado(c.x, c.y);//Dessine la pi�ce en fonction de ses coordonn�es   }}Coord rota_derecha(const Coord& c){   Coord ret = { -c.y, c.x};  // rotation coords (on inverse les coordonn�es x et y et on met "-" a x)   return ret;}void rota_derecha(Pieza& P){   for(int i = 0; i < 3; i++){      P.perif[i] = rota_derecha(P.perif[i]); // Seulement les pi�ces p�riph�riques tournent   }}/* Vide la matrice (tout en noir)*/void tablero_vacia(Tablero& T){   for(int i = 0; i< COLUMNAS;i++){      for(int j = 0;j < FILAS; j++){         T[i][j] =   NEGRO; //Esto significa casilla vacia      }   }}/* Dessine la matrice */void tablero_pinta(const Tablero& T){   for(int i = 0; i< COLUMNAS;i++){      for(int j = 0;j < FILAS; j++){         color(T[i][j]);         cuadrado(i,j);      }   }}void tablero_incrusta_pieza(Tablero& T, const Pieza& P){   for(int i = 0; i < 4; i++){      Coord c = P.posicion(i);      T[c.x][c.y] = P.color;// on insere les entiers couleur dans la matrice   }                                                    // Carr�s dans leurs positions relatives}bool tablero_colision(const Tablero& T, const Pieza& P){   for(int i=0; i<4;i++){      Coord c = P.posicion(i);      // Comprobar limites      if(c.x<0 || c.x >= COLUMNAS){         return true;      }      if(c.y<0 || c.y >= FILAS){         return true;      }      // Mirar basurilla      if(T[c.x][c.y] != NEGRO){         return true;      }   }   return false;}/* 6 filas de piezas, columnas bloques perifericos de cada pi�za*/const Coord perifs[7][3] = {   {{ 1,0 }, { 0,1 }, { 1,1}},  // cuadrado   {{ 1,0 }, {-1,1 }, { 0,1}},  // S   {{ 0,1 }, { 1,1 }, {-1,0}},  // 2   {{ 0,1 }, { 0,-1}, { 1,1}},  // L   {{ 0,1 }, { 0,-1}, {-1,1}},  // Lr   {{ 0,1 }, { 0,-1}, { 0,2}},  // Palo   {{ -1,0}, { 1,0 }, { 0,1}},  // T};void pieza_nueva(Pieza& P){   P.orig.x = 12;   P.orig.y = 2;   P.color = 1 + rand()%6;   // Pieza al azar   int r = rand()%7;   for(int i = 0; i <3;i++)   {      P.perif[i] = perifs[r][i];   }}/* Ligne pleine ou non ? */bool tablero_fila_llena(const Tablero& T, int fila){   for(int i=0; i< COLUMNAS; i++){      if(T[i][fila] == NEGRO) return false;   }   return true;}/* */void tablero_colapsa(Tablero& T, int fila){   // Copiar de abajo a arriba   for(int j = fila; j>0; j--){      for(int i = 0; i < COLUMNAS; i++){         T[i][j] = T[i][j-1];      }   }   // Vaciar la de arriba   for(int i =0; i < COLUMNAS; i++){      T[i][0] = NEGRO;   }}int tablero_cuenta_lineas(Tablero& T){   int fila = FILAS -1, cont = 0;   while(fila >=0){      if(tablero_fila_llena(T,fila))      {         tablero_colapsa(T,fila);         cont++;      }else{         fila--;      }   }   return cont;}string vers_string(int puntos){   stringstream sout;   sout << puntos;   return sout.str();}void repinta(const Tablero& T, const Pieza& p, const Pieza& sig, int puntos, int nivel){   const int ancho = TAM * COLUMNAS;   borra();   tablero_pinta(T);   color_rgb(128,128,128);   linea(20,20,20, 20+TAM*FILAS);   linea(20,20+TAM*FILAS,20+ancho,20+TAM*FILAS);   linea(20+ancho,20+TAM*FILAS,20+ancho,20);   texto(40 + ancho, 20, "PIECE SUIVANTE");   texto(40 + ancho, 160, "NIVEAU");   texto(40 + ancho, 200, "POINTS");   color(BLANCO);   texto(40 + ancho, 175,vers_string(nivel+1));   texto(40 + ancho, 215,vers_string(puntos));   pinta_pieza(p);   pinta_pieza(sig);   refresca();}const int puntos_limite[10] = {   50,100,150,170,200,220,240,260,280,400};const int tics_nivel[10]{   33,25,20,18,16,14,12,10,8,2};void game_over(){   color(BLANCO);   texto(140,240,"GAME OVER !");   refresca();   espera(2000);   vcierra();}
int main() {   srand(time(0));   vredimensiona(TAM*COLUMNAS +220, TAM*FILAS+100);
   int tic = 0, puntos = 0, nivel = 0;   Tablero T;        //Matriz   tablero_vacia(T); //vide la matriz   Pieza c, sig;   pieza_nueva(c);   pieza_nueva(sig);   c.orig.x = 5;   repinta(T,c,sig, puntos,nivel);   int t = tecla();   while(t!=ESCAPE){      // 0. Copiar la pieza actual      Pieza copia = c;      // Faire descendre la piece      if (t == NINGUNA && tic > tics_nivel[nivel]){         tic = 0;         t = ABAJO;      }      if(t== int('N')){         nivel++;      }      // 1. Mover en el papel      if(t==ABAJO) c.orig.y++;      else if(t==ARRIBA) rota_derecha(c);      else if(t==DERECHA)c.orig.x++;      else if(t==IZQUIERDA)c.orig.x--;      // 2. Mirar si hay colision      if(tablero_colision(T,c)){         c = copia;         if(t==ABAJO){            tablero_incrusta_pieza(T,c);            int cont =tablero_cuenta_lineas(T);            puntos += cont * cont;            if(puntos>puntos_limite[nivel]){               nivel++;            }            c = sig;            pieza_nueva(sig);            c.orig.x = 5;            if(tablero_colision(T,c)){               game_over();            }         }      }      // Repintar      if(t!=NINGUNA){         repinta(T,c,sig,puntos, nivel);      }      espera(30); //      tic++;      t = tecla();   }   vcierra();//ferme la fenetre
   return 0;
}
/*   //T[0][0] = VERDE;   //T[1][0] = ROJO;   //T[5][5] = AZUL;   //tablero_incrusta_pieza(T,c);   if(tablero_colision(T,c)){      mensaje("si hay colision");   }   borra();   tablero_pinta(T); //Dessine les carr�s de couleur dans la matrice   refresca();   Pieza s1 = {{2,2},{{-1,-1},{0, -1},{1,0}},VERDE};   //Pieza l1 = {{2,8},{{-1,0},{-1, -1},{1,0}}};   int t = tecla();   //pinta_pieza(s1);   refresca();   while(t!=ESCAPE){      if(t==DERECHA) rota_derecha(s1);      else if(t==IZQUIERDA) rota_izquierda(s1);      if(t!=NINGUNA){         borra();         //pinta_pieza(s1);         refresca();      }      t=tecla();   }*/