// #include <conio.h>
// #include <bitset>
// #include <deque>

// const unsigned short MINMOUSE = 0x01;
// const unsigned short MAXMOUSE = 0x08;
// const unsigned short MINKEY = 0x08;
// const unsigned short MAXKEY = 0xFF;
// bool CanSend[MAXKEY] = {0}, hold[MAXKEY] = {0};

// bool NotKey(unsigned short &i) {
//     if (i == 0x03)
//         return 1;
//     if (i >= 0x05 && i <= 0x07)
//         return 1;
//     if (i >= 0x0A && i <= 0x0B)
//         return 1;
//     if (i >= 0x10 && i <= 0x13)
//         return 1;
//     if (i == 0x5E)
//         return 1;
//     if (i >= 0x88 && i <= 0x8F)
//         return 1;
//     if (i >= 0xB8 && i <= 0xB9)
//         return 1;
//     if (i >= 0xC1 && i <= 0xDA)
//         return 1;
//     if (i == 0xE0)
//         return 1;
//     if (i == 0xFC)
//         return 1;
//     return 0;
// }