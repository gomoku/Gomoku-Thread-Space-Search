

#ifndef IRVIEW_HPP
#define IRVIEW_HPP


/* -----------------------------------------------------------
      PROGRAM gomoku 1.0
----------------------------------------------------------- */
/* -----------------------------------------------------------
      Ariel Bogdziewicz (C) 2007
      Zezwalam na dowolna modyfikacje tego kodu.
      Prosze tylko nie usuwanie tego naglowka
      i o dopisanie, ze dokonano zmian bez mojej wiedzy.
----------------------------------------------------------- */
/* -----------------------------------------------------------
      Program napisany w ramach kursu:
      -----------------------------------------
      METODY I ALGORYTMY SZTUCZNEJ INTELIGENCJI
      -----------------------------------------
      Prowadzacy: dr inz. Witold Paluszynski
      -----------------------------------------
      Politechnika Wroclawska
      Wydzial Elektroniki
----------------------------------------------------------- */



// -----------------------------------------------------------
// --- BEGIN: INCLUDES


#include <QWidget>            // QT 4.3
#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QComboBox>
#include <QLineEdit>
#include <QThread>
#include <QProgressBar>
#include <QCheckBox>
#include <QLCDNumber>
#include <QGroupBox>
#include <QFile>
#include <QByteArray>

#include <list>               // STL
#include <cstdio>             // C
#include <cstring>


// ----- END: INCLUDES
// -----------------------------------------------------------



// -----------------------------------------------------------
// --- BEGIN: DEFINES


#define SIZE       15    // rozmiar planszy
#define TSIZE      7     // rozmiar maksymalnego zagrozenia

#define KRZYZYK    'X'   // stany pola
#define KOLKO      'O'    
#define ZADEN      ' '   
#define ZAGROZENIE 'Z'

#define TRZY5      5     // rozmiary zagrozen
#define TRZY6      6
#define TRZY7      7
#define CZTERY6    6
#define CZTERY5    5
#define PIATKA     5


// ----- END: DEFINES
// -----------------------------------------------------------



// -----------------------------------------------------------
// --- BEGIN: struct Ruch


struct Ruch 
{
  int i, j;
  bool error;
};


// ----- END: struct Ruch
// -----------------------------------------------------------



// -----------------------------------------------------------
// --- BEGIN: struct TPole


struct TPole
{
  int i, j; // wspolrzedne pola
  int stan; // stan pola
};


// ----- END: struct TPole
// -----------------------------------------------------------



// -----------------------------------------------------------
// --- BEGIN: struct Event


struct Event
{
  TPole thread[TSIZE];
  int   size;
};


// ----- END: struct Event
// -----------------------------------------------------------



// -----------------------------------------------------------
// --- BEGIN: class MyButton


class MyButton: public QPushButton {

  Q_OBJECT

  int   _i;
  int   _j;

  int   _stan;

public:
  MyButton(int i, int j, QString str, QWidget *wRodzic = 0L);

  void setStan(int stan);
  int  stan() const;

public slots:
  void sendClicked();

signals:
  void isClicked(int i, int j); 
};


// ----- END: class MyButton
// -----------------------------------------------------------



// -----------------------------------------------------------
// --- BEGIN: class Kanwa


class Kanwa: public QWidget {

  Q_OBJECT

private:
  MyButton      *pole[SIZE+1][SIZE+1];

  QPushButton   *bruch;
  QPushButton   *nowa;
  QPushButton   *komp;

  QPalette      *x;
  QPalette      *o;
  QPalette      *n;
  QPalette      *w;

  QLCDNumber    *lcd;

  QCheckBox     *setx;
  QCheckBox     *seto;

  int            ruch;
  bool           endgame;

  
  void setX(int i, int j);
  void setO(int i, int j);

public:
  Kanwa(QWidget *wRodzic = 0L);

  void checkEvent(const char *str, int stan,
		  std::list<Event> &lista);
  void endGame(std::list<Event> &lista, int stan);

  // algorytm
  Ruch TSS();
  void ustawPole(TPole t_pole, int typ);
  Ruch createThread(int);
  Ruch obronaPrzedO();
  Ruch obronaPrzedX();
  Ruch wywolajDlaKazdego(Event zdarzenie, 
			 int znak, int vsznak);

public slots:
  void setButtons(int i, int j);
  void setNewGame();
  void autoGame();
  void lcdPP();
  void setRuchX();
  void setRuchO();
  void zmienX(int state);
  void zmienO(int state);

signals:
  void setStatus(const QString &);
  void zwiekszLCD();
};


// ----- END: class Kanwa
// -----------------------------------------------------------



// -----------------------------------------------------------
// --- BEGIN: class OknoGlowne


class OknoGlowne: public QMainWindow {

  Q_OBJECT

public:
  OknoGlowne(QWidget *wRodzic = 0L);

  /**
   *
   * \brief Inicjalizacja zarządcy widgetów.
   *
   */
  Kanwa *glownyGrid;

public slots:
  /**
   *
   * \brief Wyświetla odebrany napis na pasku statusu.
   *
   */
  void odbierzNapisStatusu(const QString &);                        
}; 


// ----- END: class OknoGlowne
// -----------------------------------------------------------


#endif
