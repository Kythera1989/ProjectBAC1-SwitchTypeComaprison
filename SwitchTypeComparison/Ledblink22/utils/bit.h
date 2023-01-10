/**
 * @Authors: Venier/Winter
 * @brief Bit manipulation macros
 *
 */

#ifndef bit_h_wse8AeRu22unoh7l
#define bit_h_wse8AeRu22unoh7l

/** @return a word with only bit-th bit set. */
#define BIT(bit) (1ull << (bit))
/** Raise bit-th bit in word. */
#define BIT_SET(word, bit) ((word) |= BIT(bit))
/** Clear bit-th bit in word. */
#define BIT_CLR(word, bit) ((word) &= ~BIT(bit))
/** Toggle the bit-th bit in word. */
#define BIT_TOGGLE(word, bit) ((word) ^= BIT(bit))

/** @return value "1" if bit-th bit of word is set and zero otherwise. */
#define BIT_IS_SET(word, bit) !!(word & BIT(bit))

#endif
