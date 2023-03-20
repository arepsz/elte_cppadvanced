// Farkas Árpád Levente
// AFK8S6
/*guard: Ugyanannak a kódnak a többszörös include-olását (tranzitivitás
                miatt lehet például) védjük ki vele. Nem akarunk névütközéseket,
   ODR-t sérteni (One Definition Rule)*/
#ifndef __ARRAY_ERASER__
#define __ARRAY_ERASER__
/*Include: Ezzel a preprocesszor direktívával tudunk más fájlok tartalmait
                bemásoltatni a kódunk megfelelő pontjaira.*/
#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

/*Generikus programozás
                A lényege, hogy egy általános algoritmus vagy adatszerkezet
                (osztály) szkeletont írhatunk, mely több - paraméterként kapott
                - típussal is létrehozható. A függvénysablon vagy osztálysablon
                alapján a fordító (fordítási időben) képes legenerálni a
megfelelő sablon behelyettesítést létrehozva a kívánt kódot.

Sablonosztály vagy sablonfüggvény írásához elé kell írnunk a
                template kulcsszavat, melyet egy sablonparaméter listának
                kell követnie. Sablonparaméter lehet egy típusnév vagy a
                következő típusú változók egyike: integrális típus (pl. char,
                bool, int, stb.), pointer (objektumra vagy függvényre),
                balérték referencia vagy tagra mutató pointer.
                Példa:
                        template <typename T1, typename T2, int I>*/
// typename helyett class-t írni ugyan azt jelenti
template <typename T> class array_eraser {
private:
  // size_t mérete a fordítótól függ, ezért nincsen pontos mérete
  // 32 bit alatt általában unsigned int, 64 bit alatt pedig unsigned long long
  size_t array_size;
  // array pointer -> A tömb változó konvertálódik első elemre mutató pointer-ré
  // azaz az *array egy T típusú tömb első elemére mutat.
  // array[n]-ként ugyan úgy lekérhetjük az n-dik elemet, mert nyílvan a
  // pointerrel elérjük a tömböt, mivel az első elemére mutat
  T *array;

  /* a pointerek mindig fix méretűek, ez a fix méret pedig függ az oprendszertől
  és a processzortól is */
  /* a tömbök mérete: ha int tömb, és n a méret, akkor n*int mérete
   */

public:
  /*Az iterátor típusok célja, hogy egységes módon végig tudjuk haladni
                tetszőleges (akár nem szekvenciális struktúrájú) adatszerkezetek
     elemein. */
  // a using ugyan az, mint a typedef
  // különbség: a using csak az adott scopeon belül látható, a typedef pedig
  // mindenhol a kódon belül
  using iterator = T *;
  iterator begin() { return array; }
  /*A pointer aritmetikával tudjuk elérni, hogy egy memóriacímet
                "léptessünk" előre (jobbra) vagy visszafelé (balra) a memóriában
    Tömbök az első elem címét tárolják és az elemei szekvenciálisan
                vannak a memóriába, sőt azt is tudjuk, hogy az első elemre
                mutató pointerré konvertálódnak. Ezekből adódóan a következőképp
                érhetjük el az elemeket:
                        *t     -> első elem (nullás index)
                        *(t+0) -> első elem (nullás index) - nem toltuk el
    semmivel
                        *(t+1) -> második elem (egyes index)
                        *(t+2) -> harmadik elem (kettes index)
                        *(t+3) -> negyedik elem (hármas index)
    Itt ugye az array + array_size az utolsó elem lesz, tehát az end*/
  iterator end() { return array + array_size; }

  // konstruktor
  /*Speciális tagfüggvény, mely az objektum létrehozásakor (más
                néven példányosításkor) fut le.
                Inicializálja az objektumnak allokált memóriaterületet.
                Visszatérési típusa nincsen, void-ot sem szabad kiírni!
                Nevének meg kell egyeznie az osztálynévvel.
                Explicit nem tudjuk meghívni, tulajdonképpen egy objektum
                létrehozásával "hívjuk" meg.
                Ha mindegyik ktor privát vagy védett láthatóságú, akkor az
                osztályt nem lehet példányosítani.*/
  array_eraser(T *arr, size_t s) : array_size(s), array(arr) {}

  // count függvény, megszámolja hány olyan elem van a tömbben,
  // mint amit megadott a felhasználó
  int count(T type) const {
    int cnt = 0;
    for (size_t i = 0; i < array_size; i++) {
      if (array[i] == type) {
        cnt++;
      }
    }
    return cnt;
  }

  /*Az erase függvény minden olyan elemet töröl a listából, ami megegyezik
   a függvénynek paraméterként átadott értékkel.
   Nyilván a template miatt ez lehet bármi, ezért nem feltétlen int.
   Egy for ciklus végigmegy a listán, minden lépésnél előszőr megnézi, hogy
   van-e olyan eleme, és ha van, kimenti azt egy iterátorba. Ha a lista adott
   eleme megyegyezik az iterátorral, tehát van ilyen elem, akkor csökkenti az
   elmentett méretet, eltolja egyel balra az összes, a csökkentett méreten belül
   taláható elemet, majd beszúrja az utolsó elem mellé jobbra a kimentett elemet
   (a törlendőt). Ha a tömbben már csak egy elem van, amit ráadásul töröni kell,
   akkor sokszor nem jól futna, ezért kellett egy if ág, ami ezt vizsgálja.
   Miért nem jó olyankor? Vagy azért, mert már el sem indítja úgy a for ciklust,
   vagy azért, mert olyankor már 0 a size, szóval visszaszúrja az első helyre
   (?)*/
  void erase(T type) {
    for (size_t i = 0; i < array_size; i++) {
      typename array_eraser<T>::iterator it =
          std::find(array, array + array_size, type);
      if (array[i] == *it) {
        array_size--;
        T save = *it;
        for (size_t j = i; j < array_size; j++) {
          array[j] = array[j + 1];
        }
        array[array_size] = save;
      }
      if (array_size == 1 && array[0] == type) {
        array_size--;
      }
    }
  }

  /*Kitörli a listából az indexedik elemet.
  Ezt nem kell magyarázni, végig megy a listán és kitörli azt az elemet, amelyik
  az indexedik helyen van.
  Az x azért unsigned int, hogy össze lehessen hasonlítani az array[i]-vel az
  array[x]-et*/
  void erase_index(size_t index) {
    T element = array[index];
    unsigned int x = 0;
    for (size_t i = 0; i < array_size; i++) {
      if (i != index) {
        array[x] = array[i];
      } else {
        x--;
      }
      x++;
    }
    array[array_size - 1] = element;
    array_size--;
  }

  void print_array_before() {
    std::cout << "Before erase: " << std::endl;
    for (size_t i = 0; i < array_size; i++) {
      std::cout << array[i] << " ";
    }
    std::cout << "\n" << std::endl;
  }

  void print_array_after() {
    std::cout << "After erase: " << std::endl;
    for (size_t i = 0; i < array_size; i++) {
      std::cout << array[i] << " ";
    }
    std::cout << "\n" << std::endl;
  }

  // Mivel a size az size_t ezért a getter is azzal tér vissza
  size_t size() { return array_size; }
};

#endif
