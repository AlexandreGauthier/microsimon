#include "main.h"

#define XOR_A 8;
#define XOR_B 27;
#define XOR_C 2;

// 32-bit xorshift. See Marsaglia (2003)
inline uint32 xorshift(uint32 y)
{
    y ^= y >> XOR_A;
    y ^= y << XOR_B;
    y ^= y >> XOR_C;
    return y;
}

// Use sources of entropy to generate a random 32-bit word
uint32 nextSeed(uint32 seed)
{
    // New entropy
    seed ^= getBatteryVoltage();
    seed ^= getClockValue();
    
    // Diffuse entropy throughout the word
    return xorshift(seed);
}

void seedColors(uint32 currentColor, uint32* colors)
{   
    // 16 colors per uint32
    uint32 idx = currentColor/16;
    
    if (idx > N_COLOR_WORDS_STORED) {
        return;
    }
    
    uint32 random = nextSeed(colors[idx]);

    // Only change colors that haven't been revealed to the user
    uint32 mask = ~0 << ((idx%16)*2);
    colors[idx] &= ~mask;
    colors[idx] |= (random & mask);
}

Color getColor(uint32 n, const uint32* colors)
{
    // 16 2-bit colors per word
    uint32 colorWord = colors[(n/16) % N_COLOR_WORDS_STORED];
    
    // If n > max stored colors, generate a word pseudorandomly from the existing ones.
    for (uint32 i = n; i > N_COLOR_WORDS_STORED*16; i-=16) {
        colorWord = xorshift(colorWord);
    }
    
    // Return the right color from the word
    uint32 offset = (n%16)*2;
    return (colorWord & (0b11 << offset)) >> offset;
}