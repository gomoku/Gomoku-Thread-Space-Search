################################################################################
# Copyright (c) 2007 Bogdan Kreczmer, wszystkie prawa zastrzezone
#
# Niniejszy plik zostal utworzony jako przyklad na potrzebe nauki
# programowania z wykorzystaniem biblioteki Qt w ramach kursu
# "Wizualizacja danych sensorycznych" prowadzonym na specjalnosci Robotyka,
# kierunku Automatyka i Robotyka, Wydzialu Elektroniki Politechniki
# Wroclawskiej.
# Plik ten mozna wykorzystywac we wlasnych projektach pod warunkiem, ze
# utworzone oprogramowanie bedzie objete licencja GNU General Public License.
# Jezeli w tym pliku wprowadzone zostana wlasne zmiany, to nalezy rowniez 
# usunac niniejszy naglowek.
# 
# Niniejszy plik nie jest objety zadnymi gwarancjami, ze bedzie mogl on
# byc wykorzystany do innych celow nie zwiazanych z nauka programowania.
#
################################################################################

NAZWA_APLIKACJI=gomoku
URUCHOMIENIE_APLIKACJI=./gomoku
ZRODLA_PROJEKTU=src/*.cpp inc/*.hpp
PODSTAWIENIA=OBJECTS_DIR=${KATALOG_OBJ} INCLUDEPATH=inc\
             MOC_DIR=${KATALOG_MOC}
KATALOG_OBJ=./obj
KATALOG_MOC=./moc
QMAKE=qt4-qmake
SLAD_SPRAWDZENIA=.qt_juz_sprawdzone



__start__: ${SLAD_SPRAWDZENIA} ${NAZWA_APLIKACJI}
	rm -f core*; ${URUCHOMIENIE_APLIKACJI}

${SLAD_SPRAWDZENIA}:
	@if which $(QMAKE) > /dev/null; then exit 0;\
        else\
          echo; echo " Brak programu qmake."\
               " Prawdopodobnie biblioteka Qt nie zostala zainstalowana.";\
          echo; exit 1;\
        fi
	@if $(QMAKE) -v | grep 'ver.*4\.[0-9]*\.[0-9]*' > /dev/null;\
         then exit 0;\
         else echo; echo " Brak biblioteki Qt w wersji 4.x.x";\
              echo;  exit 1;\
        fi
	touch ${SLAD_SPRAWDZENIA}



${NAZWA_APLIKACJI}: Makefile.app __sprawdz__

__sprawdz__:
	make -f Makefile.app

Makefile.app: ${NAZWA_APLIKACJI}.pro
	$(QMAKE) -o Makefile.app ${NAZWA_APLIKACJI}.pro

${NAZWA_APLIKACJI}.pro:
	rm -f ${NAZWA_APLIKACJI}
	$(QMAKE) -project -nopwd -o ${NAZWA_APLIKACJI}.pro\
               ${PODSTAWIENIA} ${ZRODLA_PROJEKTU} 

project: __usun_pro__ ${NAZWA_APLIKACJI}.pro

__usun_pro__:
	rm -f ${NAZWA_APLIKACJI}.pro

clean:
	make -f Makefile.app clean
	rm -r ${SLAD_SPRAWDZENIA}

cleanall: clean
	rm -f ${NAZWA_APLIKACJI}

clean_copies:
	find . -name \*~ -exec rm {} \;
	find . -name \*.bak -exec rm {} \;

cleantotally: clean_copies cleanall
	rm -f ${NAZWA_APLIKACJI}.pro Makefile.app
	rm -fr ${KATALOG_MOC} ${KATALOG_OBJ}

cleandocs:
	rm -rf doc/*

help:
	@echo 
	@echo "Dzialania:"
	@echo 
	@echo " project  - wymusza utworzenie nowego projektu"
	@echo " clean    - usuwa wszystkie produkty kompilacji i konsolidacji"
	@echo " cleanall - usuwa produkty kompilacji wraz z aplikacja"
	@echo " cleantotally - usuwa wszystko oprocz zrodel i pliku Makefile"
	@echo " cleandocs - usuwa dokumentacje wygenerowana doxygen"
	@echo " help     - wyswietla niniejsza pomoc"
	@echo
