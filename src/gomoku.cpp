

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


#include <QApplication>
#include <QStatusBar>
#include <QString>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "gomoku.hpp"


// ----- END: INCLUDES
// -----------------------------------------------------------


// -----------------------------------------------------------
// --- BEGIN: class MyButton


MyButton::MyButton(int i, int j, QString str,
		   QWidget *wRodzic): QPushButton(wRodzic)
{
  _i = i; _j = j;

  _stan = ZADEN;

  setText(str);

  connect(this,   SIGNAL(clicked()),
	  this,   SLOT(sendClicked()));
}


void MyButton::sendClicked()
{
  emit isClicked(_i, _j);
}


void MyButton::setStan(int stan)
{
  _stan = stan;
}


int MyButton::stan() const
{
  return _stan;
}


// ----- END: class MyButton
// -----------------------------------------------------------


// -----------------------------------------------------------
// --- BEGIN: class Kanwa


Kanwa::Kanwa(QWidget *wRodzic): QWidget(wRodzic)
{
  // inicjalizacja atrybutow
  ruch = KRZYZYK;

  endgame = false;

  x = new QPalette (QBrush(Qt::yellow),         // windowText 
		    QBrush(Qt::yellow),         // button
		    QBrush(Qt::yellow),         // light
		    QBrush(Qt::yellow),         // dark
		    QBrush(Qt::yellow),         // mid
		    QBrush(Qt::black),          // text
		    QBrush(Qt::yellow),         // bright_text
		    QBrush(Qt::yellow),         // base
		    QBrush(Qt::yellow));        // window

  o = new QPalette (QBrush(Qt::white),          // windowText 
		    QBrush(Qt::white),          // button
		    QBrush(Qt::white),          // light
		    QBrush(Qt::white),          // dark
		    QBrush(Qt::white),          // mid
		    QBrush(Qt::black),          // text
		    QBrush(Qt::white),          // bright_text
		    QBrush(Qt::white),          // base
		    QBrush(Qt::white));         // window

  n = new QPalette (QBrush(Qt::gray),           // windowText 
		    QBrush(Qt::gray),           // button
		    QBrush(Qt::gray),           // light
		    QBrush(Qt::gray),           // dark
		    QBrush(Qt::gray),           // mid
		    QBrush(Qt::black),          // text
		    QBrush(Qt::gray),           // bright_text
		    QBrush(Qt::gray),           // base
		    QBrush(Qt::gray));          // window

  w = new QPalette (QBrush(Qt::black),           // windowText 
		    QBrush(Qt::black),           // button
		    QBrush(Qt::black),           // light
		    QBrush(Qt::black),           // dark
		    QBrush(Qt::black),           // mid
		    QBrush(Qt::white),           // text
		    QBrush(Qt::black),           // bright_text
		    QBrush(Qt::black),           // base
		    QBrush(Qt::black));          // window

  // glowny szkielet to HBOX
  QHBoxLayout *szkielet = new QHBoxLayout;

  QGridLayout *siatka = new QGridLayout;
  siatka->setSpacing(0);
  siatka->setSizeConstraint(QLayout::SetFixedSize);

  for(int j=0; j<=SIZE; j++) {
    
    QString temp;

    pole[0][j] = new MyButton(0,j,temp.setNum(j),this);
    pole[0][j]->setFixedSize(25,25);
    pole[0][j]->setDisabled(true);

    siatka->addWidget(pole[0][j],0,j);
  }

  for(int i=1; i<=SIZE; i++) {

    QString temp;
    
    pole[i][0] = new MyButton(i,0,temp.setNum(i),this);
    pole[i][0]->setFixedSize(25,25);
    pole[i][0]->setDisabled(true);

    siatka->addWidget(pole[i][0],i,0);
  }

  for(int i=1; i<=SIZE; i++)
    for(int j=1; j<=SIZE; j++) {

      pole[i][j] = new MyButton(i, j, tr(" "), this);
      pole[i][j]->setFixedSize(25,25);
      pole[i][j]->setFocusPolicy(Qt::NoFocus);
      pole[i][j]->setPalette(*n);
      
      siatka->addWidget(pole[i][j],i,j);

      connect(pole[i][j], SIGNAL(isClicked(int,int)),
	      this,       SLOT(setButtons(int,int)));
    }

  bruch = new QPushButton("X",this);
  bruch->setDisabled(true);
  bruch->setPalette(*x); 
  bruch->setFixedSize(25,25);
  
  QLabel *nastepny = new QLabel(tr("Ruch "),this);

  QGroupBox *opcje = new QGroupBox(tr("Opcje"));

  QSpacerItem *sprezyna = new QSpacerItem(0,0,
					  QSizePolicy::Expanding,
					  QSizePolicy::Expanding);

  setx = new QCheckBox(tr("Ustaw X"),this);
  seto = new QCheckBox(tr("Ustaw O"),this);

  lcd = new QLCDNumber(5,this);
  lcd->display(0);
  lcd->setFixedSize(75,30);

  nowa = new QPushButton(tr("Nowa gra"),this);
  nowa->setFixedSize(75,30);

  komp = new QPushButton(tr("Komputer"),this);
  komp->setFixedSize(75,30);

  QHBoxLayout *kolejny = new QHBoxLayout;
  kolejny->addWidget(nastepny);
  kolejny->addWidget(bruch);

  QPushButton *bsetx = new QPushButton(tr("Ruch X"),this);
  QPushButton *bseto = new QPushButton(tr("Ruch O"),this);
  bsetx->setFixedSize(75,30);
  bseto->setFixedSize(75,30);

  QVBoxLayout *pion = new QVBoxLayout;
  pion->addLayout(kolejny);
  pion->addWidget(setx);
  pion->addWidget(seto);
  pion->addWidget(bsetx);
  pion->addWidget(bseto);
  pion->addItem(sprezyna);
  pion->addWidget(lcd);
  pion->addWidget(komp);
  pion->addWidget(nowa);
  

  opcje->setLayout(pion);

  szkielet->addLayout(siatka);
  szkielet->addWidget(opcje);

  setLayout(szkielet);

  connect(nowa,    SIGNAL(clicked()),
	  this,    SLOT(setNewGame()));

  connect(komp,    SIGNAL(clicked()),
	  this,    SLOT(autoGame()));

  connect(this,    SIGNAL(zwiekszLCD()),
	  this,    SLOT(lcdPP()));

  connect(bsetx,   SIGNAL(clicked()),
	  this,    SLOT(setRuchX()));

  connect(bseto,   SIGNAL(clicked()),
	  this,    SLOT(setRuchO()));

  connect(setx,    SIGNAL(stateChanged(int)),
	  this,    SLOT(zmienO(int)));

  connect(seto,    SIGNAL(stateChanged(int)),
	  this,    SLOT(zmienX(int)));
}


void Kanwa::zmienO(int state)
{
  if(state)
    seto->setCheckState(Qt::Unchecked);
}


void Kanwa::zmienX(int state)
{
  if(state)
    setx->setCheckState(Qt::Unchecked);
}


void Kanwa::setRuchO()
{
  ruch = KOLKO;
  
  bruch->setPalette(*o);     
  bruch->setText("O");
  
  komp->setDisabled(true);
}


void Kanwa::setRuchX()
{
  ruch = KRZYZYK;

  bruch->setPalette(*x);  
  bruch->setText("X");

  komp->setDisabled(false);
}


void Kanwa::ustawPole(TPole t_pole, int typ)
{
  pole[t_pole.i][t_pole.j]->setStan(typ);
}


void Kanwa::setButtons(int i, int j)
{
  std::list<Event> wygrane;

  if(!endgame)
    if(setx->checkState()!=Qt::Checked && seto->checkState()!=Qt::Checked) {
      
      if(ruch == KRZYZYK) {
	
	setX(i, j);
	
	checkEvent("XXXXX",PIATKA,wygrane);
	endGame(wygrane,KRZYZYK);
	
      } else if(ruch == KOLKO) {
	
	setO(i, j);
	
	checkEvent("OOOOO",PIATKA,wygrane);
	endGame(wygrane,KOLKO);
      }
      
    } else if(ruch!=ZADEN && setx->checkState()==Qt::Checked) {
      
      setX(i, j);
      
      checkEvent("XXXXX",PIATKA,wygrane);
      endGame(wygrane,KRZYZYK);
      
    } else if(ruch!=ZADEN && seto->checkState()==Qt::Checked) {
      
      setO(i, j);
      
      checkEvent("OOOOO",PIATKA,wygrane);
      endGame(wygrane,KOLKO);
    }
}


Ruch Kanwa::obronaPrzedO()
{
  Ruch punkt; Event temp;

  std::list<Event> zagrozenia;
  std::list<Event>::iterator pos;

  checkEvent("OOOO ",CZTERY5,zagrozenia);
  checkEvent(" OOOO",CZTERY5,zagrozenia);
  checkEvent("O OOO",CZTERY5,zagrozenia);
  checkEvent("OO OO",CZTERY5,zagrozenia);
  checkEvent("OOO O",CZTERY5,zagrozenia);
  checkEvent(" OOOO ",CZTERY6,zagrozenia);

  if(zagrozenia.size()) {

    int p;

    temp = *(zagrozenia.begin());

    for(p=0; p<temp.size && temp.thread[p].stan!=ZADEN; p++);
    
    punkt.i = temp.thread[p].i;
    punkt.j = temp.thread[p].j;
    punkt.error = false;

    return punkt;
  }

  zagrozenia.clear();
  checkEvent("  OOO  ",TRZY7,zagrozenia);
  checkEvent(" OOO  ",TRZY6,zagrozenia);
  checkEvent("  OOO ",TRZY6,zagrozenia);
  checkEvent(" OO O ",TRZY6,zagrozenia);
  checkEvent(" O OO ",TRZY6,zagrozenia);

  for(pos=zagrozenia.begin(); pos!=zagrozenia.end(); pos++) {
    
    punkt = wywolajDlaKazdego(*pos,KRZYZYK,KOLKO);

    if(!punkt.error)
      return punkt;
  }

  punkt.error = true;
  return punkt;
}


Ruch Kanwa::obronaPrzedX()
{
  Ruch punkt; Event temp;

  std::list<Event> zagrozenia;
  std::list<Event>::iterator pos;

  checkEvent("XXXX ",CZTERY5,zagrozenia);
  checkEvent(" XXXX",CZTERY5,zagrozenia);
  checkEvent("X XXX",CZTERY5,zagrozenia);
  checkEvent("XX XX",CZTERY5,zagrozenia);
  checkEvent("XXX X",CZTERY5,zagrozenia);
  checkEvent(" XXXX ",CZTERY6,zagrozenia);

  if(zagrozenia.size()) {

    int p;

    temp = *(zagrozenia.begin());

    for(p=0; p<temp.size && temp.thread[p].stan!=ZADEN; p++);
    
    punkt.i = temp.thread[p].i;
    punkt.j = temp.thread[p].j;
    punkt.error = false;

    return punkt;
  }

  zagrozenia.clear();
  checkEvent("  XXX  ",TRZY7,zagrozenia);
  checkEvent(" XXX  ",TRZY6,zagrozenia);
  checkEvent("  XXX ",TRZY6,zagrozenia);
  checkEvent(" XX X ",TRZY6,zagrozenia);
  checkEvent(" X XX ",TRZY6,zagrozenia);

  for(pos=zagrozenia.begin(); pos!=zagrozenia.end(); pos++) {
    
    punkt = wywolajDlaKazdego(*pos,KOLKO,KRZYZYK);

    if(!punkt.error)
      return punkt;
  }

  punkt.error = true;
  return punkt;
}


Ruch Kanwa::wywolajDlaKazdego(Event zdarzenie, int znak, int vsznak)
{
  Ruch punkt;

  for(int p=0; p<zdarzenie.size; p++) {
    if(zdarzenie.thread[p].stan==ZADEN) {
      
      zdarzenie.thread[p].stan=znak;
      ustawPole(zdarzenie.thread[p],znak);
      
      for(int k=0; k<zdarzenie.size; k++)
	if(zdarzenie.thread[k].stan==ZADEN) {

	  ustawPole(zdarzenie.thread[k],vsznak);
	  
	  punkt = obronaPrzedO();
	  
	  ustawPole(zdarzenie.thread[k],ZADEN);

	  if(!punkt.error) break;
	}
      
      zdarzenie.thread[p].stan=ZADEN;
      ustawPole(zdarzenie.thread[p],ZADEN);
      
      if(!punkt.error) {
	
	return punkt;
      }
    }
  }

  punkt.error = true;
  return punkt;
}


void Kanwa::autoGame()
{
  //printf("AUTOGAME\n");

  std::list<Event> zagrozenia;

  lcd->display(0);

  Ruch punkt;

  punkt.error = true;

  // 1. szukaj zagrozen z mojej strony - CZWORKI
  checkEvent("XXXX ",CZTERY5,zagrozenia);
  checkEvent(" XXXX",CZTERY5,zagrozenia);
  checkEvent(" XXXX ",CZTERY6,zagrozenia);
  checkEvent("X XXX",CZTERY5,zagrozenia);
  checkEvent("XX XX",CZTERY5,zagrozenia);
  checkEvent("XXX X",CZTERY5,zagrozenia);

  if(zagrozenia.size()) 

    punkt = TSS(); // TSS

  else {
    
    checkEvent("OOOO ",CZTERY5,zagrozenia);
    checkEvent(" OOOO",CZTERY5,zagrozenia);
    checkEvent(" OOOO ",CZTERY6,zagrozenia);
    checkEvent("O OOO",CZTERY5,zagrozenia);
    checkEvent("OO OO",CZTERY5,zagrozenia);
    checkEvent("OOO O",CZTERY5,zagrozenia);
    
    if(zagrozenia.size()) 
      
      punkt = obronaPrzedO(); // OBRONA
    
    else {
      
      checkEvent("  XXX  ",TRZY7,zagrozenia);
      checkEvent(" XXX  ",TRZY6,zagrozenia);
      checkEvent("  XXX ",TRZY6,zagrozenia);
      checkEvent(" XX X ",TRZY6,zagrozenia);
      checkEvent(" X XX ",TRZY6,zagrozenia);

      if(zagrozenia.size())
	
	punkt = TSS(); // TSS
      
      else {
	
	checkEvent(" XXX ",CZTERY5,zagrozenia);
	checkEvent("X XX ",CZTERY5,zagrozenia);
	checkEvent("XX X ",CZTERY5,zagrozenia);
	checkEvent("XXX  ",CZTERY5,zagrozenia);
	
	checkEvent("  XXX",CZTERY5,zagrozenia);
	checkEvent(" X XX",CZTERY5,zagrozenia);
	checkEvent(" XX X",CZTERY5,zagrozenia);
	checkEvent(" XXX ",CZTERY5,zagrozenia);

	checkEvent("  XXX ",CZTERY6,zagrozenia);
	checkEvent(" X XX ",CZTERY6,zagrozenia);
	checkEvent(" XX X ",CZTERY6,zagrozenia);
	checkEvent(" XXX  ",CZTERY6,zagrozenia);

	if(zagrozenia.size())
	  
	  punkt = createThread(0);
	
	else {
	  
	  checkEvent("  OOO  ",TRZY7,zagrozenia);
	  checkEvent(" OOO  ",TRZY6,zagrozenia);
	  checkEvent("  OOO ",TRZY6,zagrozenia);
	  checkEvent(" OO O ",TRZY6,zagrozenia);
	  checkEvent(" O OO ",TRZY6,zagrozenia);
	  
	  if(zagrozenia.size())
	    
	    punkt = obronaPrzedO(); // OBRONA
	  
	}
      }
    }
  }

  if(!punkt.error)
    
    setButtons(punkt.i, punkt.j);
  
  else {
    
    //printf("WYWOLANIE CREATE\n");
    punkt = createThread(0);

    if(!punkt.error)
      
      setButtons(punkt.i, punkt.j);
    
    else 
      
      emit setStatus("GOMOKU; autor: Ariel Bogdziewicz - ERROR autoGAME2");
    
  }
  
}


Ruch Kanwa::createThread(int g)
{
  Ruch punkt;

  Event temp;

  std::list<Event> zagrozenia;
  std::list<Event>::iterator pos;

  checkEvent(" XXX ",CZTERY5,zagrozenia);
  checkEvent("X XX ",CZTERY5,zagrozenia);
  checkEvent("XX X ",CZTERY5,zagrozenia);
  checkEvent("XXX  ",CZTERY5,zagrozenia);

  checkEvent("  XXX",CZTERY5,zagrozenia);
  checkEvent(" X XX",CZTERY5,zagrozenia);
  checkEvent(" XX X",CZTERY5,zagrozenia);
  checkEvent(" XXX ",CZTERY5,zagrozenia);

  checkEvent("  XXX ",CZTERY6,zagrozenia);
  checkEvent(" X XX ",CZTERY6,zagrozenia);
  checkEvent(" XX X ",CZTERY6,zagrozenia);
  checkEvent(" XXX  ",CZTERY6,zagrozenia);

  printf("CREATE %3d\t%3d\n",g,zagrozenia.size());

  if(zagrozenia.size())
    for(pos=zagrozenia.begin(); pos!=zagrozenia.end(); pos++) {
      
      temp = *pos;
      
      for(int p=0; p<pos->size; p++) {
	
	if((*pos).thread[p].stan==ZADEN) {
	  
	  (*pos).thread[p].stan=KRZYZYK;
	  ustawPole((*pos).thread[p],KRZYZYK);
	  
	  for(int k=0; k<pos->size; k++)
	    if((*pos).thread[k].stan==ZADEN) {
	      ustawPole((*pos).thread[k],KOLKO);
	    
	      punkt = TSS();
	      
	      if(punkt.error && g<2)
		punkt = createThread(g+1);
	      
	      ustawPole((*pos).thread[k],ZADEN);
	      
	      if(!punkt.error)
		break;
	    }
	  
	  (*pos).thread[p].stan=ZADEN;
	  ustawPole((*pos).thread[p],ZADEN);
	  
	  if(!punkt.error) {
	    
	    punkt.i = (*pos).thread[p].i;
	    punkt.j = (*pos).thread[p].j;
	    
	    printf("WYJSCIE OK\n");

	    emit setStatus("GOMOKU; autor: Ariel Bogdziewicz - Create");
	    
	    return punkt;
	  }
	}
      }
    }

  zagrozenia.clear();

  checkEvent("   XX  ",TRZY7,zagrozenia);
  checkEvent("  X X  ",TRZY7,zagrozenia);
  checkEvent("  XX   ",TRZY7,zagrozenia);

  checkEvent("  XX  ",TRZY6,zagrozenia);
  checkEvent(" X X  ",TRZY6,zagrozenia);
  checkEvent(" XX   ",TRZY6,zagrozenia);
  checkEvent("   XX ",TRZY6,zagrozenia);
  checkEvent("  X X ",TRZY6,zagrozenia);
  checkEvent(" X  X ",TRZY6,zagrozenia);

  if(zagrozenia.size())
    for(pos=zagrozenia.begin(); pos!=zagrozenia.end(); pos++) {
      
      temp = *pos;
      
      for(int p=0; p<pos->size; p++) {
	
	if((*pos).thread[p].stan==ZADEN) {
	  
	  (*pos).thread[p].stan=KRZYZYK;
	  ustawPole((*pos).thread[p],KRZYZYK);
	  
	  for(int k=0; k<pos->size; k++)
	    if((*pos).thread[k].stan==ZADEN) {
	      ustawPole((*pos).thread[k],KOLKO);
	    
	      punkt = TSS();
	      
	      if(punkt.error && g<5)
		punkt = createThread(g+1);
	      
	      ustawPole((*pos).thread[k],ZADEN);
	      
	      if(!punkt.error)
		break;
	    }
	  
	  (*pos).thread[p].stan=ZADEN;
	  ustawPole((*pos).thread[p],ZADEN);
	  
	  if(!punkt.error) {
	    
	    punkt.i = (*pos).thread[p].i;
	    punkt.j = (*pos).thread[p].j;
	    
	    printf("WYJSCIE OK\n");

	    emit setStatus("GOMOKU; autor: Ariel Bogdziewicz - Create");
	    
	    return punkt;
	  }
	}
      }
    }

  emit setStatus("GOMOKU; autor: Ariel Bogdziewicz - Error Create");
  
  //printf("WYJSCIE ERROR\n");

  punkt.error = true;
  return punkt;
}


void Kanwa::lcdPP()
{
  lcd->display(lcd->value()+1);
}


Ruch Kanwa::TSS()
{
  emit zwiekszLCD();

  emit setStatus("GOMOKU; autor: Ariel Bogdziewicz - TSS");

  Ruch punkt, obr;

  Event temp;

  std::list<Event> zagrozenia;
  std::list<Event> zagr;
  std::list<Event>::iterator pos;

  std::list<int> jakieK;
  std::list<int>::iterator ipos;

  checkEvent("XXXX ",CZTERY5,zagrozenia);
  checkEvent(" XXXX",CZTERY5,zagrozenia);
  checkEvent(" XXXX ",CZTERY6,zagrozenia);
  checkEvent("X XXX",CZTERY5,zagrozenia);
  checkEvent("XX XX",CZTERY5,zagrozenia);
  checkEvent("XXX X",CZTERY5,zagrozenia);

  printf("TSS4      \t%3d\n",zagrozenia.size());

  if(zagrozenia.size()) {

    int i;

    temp = *(zagrozenia.begin());

    for(i=0; i<temp.size && temp.thread[i].stan!=ZADEN; i++);
    if(temp.thread[i].stan==ZADEN) {
      punkt.i = temp.thread[i].i;
      punkt.j = temp.thread[i].j;

      printf("4: %d - %d\n",punkt.i, punkt.j);

      punkt.error = false;
    } else {
      punkt.error = true;
    }

    return punkt;
  }

  checkEvent("  XXX  ",TRZY7,zagrozenia);
  checkEvent(" XXX  ",TRZY6,zagrozenia);
  checkEvent("  XXX ",TRZY6,zagrozenia);
  checkEvent(" XX X ",TRZY6,zagrozenia);
  checkEvent(" X XX ",TRZY6,zagrozenia);

  printf("TSS3      \t%3d\n",zagrozenia.size());

  if(zagrozenia.size()) {
    
    int i;
    
    temp = *(zagrozenia.begin());
    
    for(i=1; i<temp.size-1 && temp.thread[i].stan!=ZADEN; i++);
    if(temp.thread[i].stan==ZADEN) {
      punkt.i = temp.thread[i].i;
      punkt.j = temp.thread[i].j;

      printf("3: %d - %d\n",punkt.i, punkt.j);

      for(int p=0; p<temp.size; p++)
	printf("|%c|%d,%d",temp.thread[p].stan,
	       temp.thread[p].i,temp.thread[p].j);
      printf("|\n");

      punkt.error = false;
    } else {
      punkt.error = true;
    }
    
    return punkt;
  }

  punkt.error = true;
  return punkt;
}


void Kanwa::setNewGame()
{
  for(int i=1; i<=SIZE; i++)
    for(int j=1; j<=SIZE; j++) {
      pole[i][j]->setPalette(*n);  
      pole[i][j]->setText(" ");
      pole[i][j]->setDisabled(false);
      pole[i][j]->setStan(ZADEN);

      ruch = KRZYZYK;
      bruch->setPalette(*x);   
      bruch->setText("X");
    }

  emit setStatus("GOMOKU; autor: Ariel Bogdziewicz");

  endgame = false;

  komp->setDisabled(false);
}


void Kanwa::setX(int i, int j)
{
  pole[i][j]->setText("X");
  pole[i][j]->setStan(KRZYZYK);
  pole[i][j]->setDisabled(true);
  pole[i][j]->setPalette(*x);   
  ruch = KOLKO;
  
  bruch->setPalette(*o);     
  bruch->setText("O");

  komp->setDisabled(true);
}


void Kanwa::setO(int i, int j)
{
  pole[i][j]->setText("O");
  pole[i][j]->setStan(KOLKO);
  pole[i][j]->setDisabled(true);
  pole[i][j]->setPalette(*o);  
  ruch = KRZYZYK;

  bruch->setPalette(*x);  
  bruch->setText("X");

  komp->setDisabled(false);
}


void Kanwa::checkEvent(const char *str, int typ,
				   std::list<Event> &lista)
{
  char odczyt[SIZE+1], *temp, *wsk;

  Event ptr;

  int i, j;

  // poziom
  for(i=1; i<=SIZE; i++) {
    for(j=1; j<=SIZE; j++) {
      
      odczyt[j-1] = (char) pole[i][j]->stan();
    }

    odczyt[j-1] = '\0';
    
    temp = odczyt;

    while((wsk = strstr(temp,str))!=NULL) {
      
      ptr.size = typ;
      
      for(int p=0; p<typ; p++) {
	
	ptr.thread[p].i    = i;
	ptr.thread[p].j    = wsk - odczyt + p + 1;
	ptr.thread[p].stan = wsk[p];
      }

      lista.push_back(ptr);

      temp = wsk + 1;
    }
  } // end: poziom

  // pion
  for(j=1; j<=SIZE; j++) {
    for(i=1; i<=SIZE; i++) {
      
      odczyt[i-1] = (char) pole[i][j]->stan();
    }

    odczyt[i-1] = '\0';
    
    temp = odczyt;

    while((wsk = strstr(temp,str))!=NULL) {
      
      ptr.size = typ;
      
      for(int p=0; p<typ; p++) {
	
	ptr.thread[p].i    = wsk - odczyt + p + 1;
	ptr.thread[p].j    = j;
	ptr.thread[p].stan = wsk[p];
      }

      lista.push_back(ptr);

      temp = wsk + 1;
    }
  } // end: pion

  // pod ukosem 1.1
  for(int q=1; q<=SIZE; q++) {
  
    for(i=1,j=q;i<=q;i++,j--)
      odczyt[i-1] = (char) pole[i][j]->stan();
    
    odczyt[i-1] = '\0';
    
    temp = odczyt;

    while((wsk = strstr(temp,str))!=NULL) {
      
      ptr.size = typ;
      
      for(int p=0; p<typ; p++) {
	
	ptr.thread[p].j    = q - (wsk - odczyt) - p;
	ptr.thread[p].i    = q - ptr.thread[p].j + 1;
	ptr.thread[p].stan = wsk[p];
      }

      lista.push_back(ptr);

      temp = wsk + 1;
    }
  } // end: pod ukosem 1.1

  // pod ukosem 1.2
  for(int q=2; q<=SIZE; q++) {
  
    for(i=q,j=SIZE;i<=SIZE;i++,j--)
      odczyt[i-q-1] = (char) pole[i][j]->stan();
    
    odczyt[i-q-1] = '\0';
    
    temp = odczyt;

    while((wsk = strstr(temp,str))!=NULL) {
      
      ptr.size = typ;
      
      for(int p=0; p<typ; p++) {
	
	ptr.thread[p].i    = q + (wsk - odczyt) + p + 2;
	ptr.thread[p].j    = q - ptr.thread[p].i;
	ptr.thread[p].stan = wsk[p];
      }

      lista.push_back(ptr);

      temp = wsk + 1;
    }
  } // end: pod ukosem 1.2

  // pod ukosem 2.1
  for(int q=1; q<=SIZE; q++) {
  
    for(i=1,j=q;i<=SIZE-q;i++,j++)
      odczyt[i-1] = (char) pole[i][j]->stan();
    
    odczyt[i-1] = '\0';
    
    temp = odczyt;

    while((wsk = strstr(temp,str))!=NULL) {
      
      ptr.size = typ;
      
      for(int p=0; p<typ; p++) {
	
	ptr.thread[p].i    = wsk - odczyt + p + 1;
	ptr.thread[p].j    = q + ptr.thread[p].i - 1;
	ptr.thread[p].stan = wsk[p];
      }

      lista.push_back(ptr);

      temp = wsk + 1;
    }
  } // end: pod ukosem 2.1

  // pod ukosem 2.2
  for(int q=2; q<=SIZE; q++) {
  
    for(i=q,j=1;i<=SIZE;i++,j++)
      odczyt[j-1] = (char) pole[i][j]->stan();
    
    odczyt[j-1] = '\0';
    
    temp = odczyt;

    while((wsk = strstr(temp,str))!=NULL) {
      
      ptr.size = typ;
      
      for(int p=0; p<typ; p++) {
	
	ptr.thread[p].j    = wsk - odczyt + p + 1;
	ptr.thread[p].i    = q + ptr.thread[p].j - 1;
	ptr.thread[p].stan = wsk[p];
      }

      lista.push_back(ptr);

      temp = wsk + 1;
    }
  } // end: pod ukosem 2.2
}


void Kanwa::endGame(std::list<Event> &lista, int stan)
{
  Event ptr;

  int i, j;

  if(lista.size()) {

    ptr = *(lista.begin());

    for(int p=0; p<ptr.size; p++) {

      i = ptr.thread[p].i;
      j = ptr.thread[p].j;

      pole[i][j]->setPalette(*w);
    }
    
    ruch = ZADEN;

    if(stan == KRZYZYK)
      emit setStatus("GOMOKU; autor: Ariel Bogdziewicz - WYGRAL KRZYZYK");
    else
      emit setStatus("GOMOKU; autor: Ariel Bogdziewicz - WYGRAL KOLKO");

    komp->setDisabled(true);

    endgame = true;
  }
}


// ----- END: class Kanwa
// -----------------------------------------------------------



// -----------------------------------------------------------
// --- BEGIN: class OknoGlowne


OknoGlowne::OknoGlowne(QWidget *wRodzic): QMainWindow(wRodzic)
{
  // inicjalizacja glownej siatki
  glownyGrid = new Kanwa(this);
  glownyGrid->setFixedHeight(375);

  // inicjalizacja paska statusu
  setStatusBar(new QStatusBar());
  setFixedSize(555,425);

  // dodanie glownej siatki
  setCentralWidget(glownyGrid);

  // sygnaly i sloty
  connect(glownyGrid, SIGNAL(setStatus(const QString &)),
	  this,       SLOT(odbierzNapisStatusu(const QString &)));

  odbierzNapisStatusu("GOMOKU; autor: Ariel Bogdziewicz");
}


void OknoGlowne::odbierzNapisStatusu(const QString &Napis)
{
  // aktualizuje napis statusu
  statusBar()->showMessage(Napis);
}


// ----- END: class OknoGlowne
// -----------------------------------------------------------



// -----------------------------------------------------------
// --- BEGIN: MAIN


int main( int argc, char * argv[] ) 
{
  QApplication App(argc,argv);
  
  OknoGlowne *Okno;
  Okno = new OknoGlowne;
  Okno->show();
  
  return App.exec();
}


// ----- END: MAIN
// -----------------------------------------------------------
