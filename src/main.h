#include <stdint.h>
void SetupPurpleAndGreenPalette();
void SetupTotallyRandomPalette();
void SetupBlackAndWhiteStripedPalette();

void ChangePaletteOnTouch(uint8_t secondHand);
void ChangePalettePeriodically(uint8_t secondHand);

void FillLEDsFromPaletteColors(uint8_t colorIndex);
