#ifdef __cplusplus
extern "C" {
#endif

/**
 * Zwraca identyfikator globalnego słownika, którego nie można usunąć.
 */
unsigned long dict_global();

/**
 * Stała jest równa 42 i określa maksymalny rozmiar globalnego słownika.
 * Do globalnego słownika można wstawiać kolejne klucze z wartościami tylko,
 * gdy jego rozmiar po wstawieniu nie będzie przekraczał maksymalnego
 * rozmiaru.
 */
#define MAX_GLOBAL_DICT_SIZE 42

#ifdef __cplusplus
}
#endif
