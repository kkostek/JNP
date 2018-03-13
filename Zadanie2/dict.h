#ifdef __cplusplus
extern "C" {
#endif
#include <stddef.h>
/**
 * Tworzy nowy, pusty słownik i zwraca jego identyfikator.
 * Assert, gdu przekręci się licznik;
 */
unsigned long dict_new();

/**
 *Jeżeli istnieje słownik o identyfikatorze id, usuwa go, a w przeciwnym
 *przypadku nic nie robi.
 */
void dict_delete(unsigned long id);

/**
 * Jeżeli istnieje słownik o identyfikatorze id, zwraca liczbę jej
 * elementów, a w przeciwnym przypadku zwraca 0.
 */
size_t dict_size(unsigned long id);

/**
 * Jeżeli istnieje słownik o identyfikatorze id oraz key != NULL
 * i value != NULL, wstawia wartość value pod kluczem key. W przeciwnym przypadku nic nie robi.
 * Gwarancje odnośnie kosztów wstawienia mają być takie same jak w przypadku
 * kontenera unordered_map (plus koszt odnalezienia słownika o danym
 * identyfikatorze).
 */
void dict_insert(unsigned long id, const char* key, const char* value);

/**
 * Jeżeli istnieje słownik o identyfikatorze id i zawiera klucz key, to usuwa
 * klucz oraz wartość związaną z tym kluczem, a w przeciwnym przypadku nic nie
 * robi.
 */
void dict_remove(unsigned long id, const char* key);


/**
 * Jeżeli istnieje słownik o identyfikatorze id i zawiera wartość pod kluczem
 * key, to zwraca wskaźnik do tej wartości, a w przeciwnym zwraca wartość
 * ze słownika globalnego. Jeśli w słowniku globalnym nie ma wartości
 * pod kluczem key, to zwraca NULL.
 */
const char* dict_find(unsigned long id, const char* key);


/**
 * Jeżeli istnieje słownik o identyfikatorze id, usuwa wszystkie jego elementy,
 * a w przeciwnym przypadki nic nie robi.
 */
void dict_clear(unsigned long id);


/**
 * Jeżeli istnieją słowniki o identyfikatorach src_id oraz dst_id, to kopiuje
 * zawartość słownika o identyfikatorze src_id do słownika o identyfikatorze
 * dst_id, a w przeciwnym przypadku nic nie robi.
 */
void dict_copy(unsigned long src_id, unsigned long dst_id);

#ifdef __cplusplus
}
#endif
