/******************************************************************************
Copyright (c) 2018 Advanced Micro Devices, Inc. All rights reserved.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*******************************************************************************/

#include "simple_convolution/simple_convolution.h"

#include <assert.h>
#include <string.h>
#include <iostream>

#include "util/helper_funcs.h"
#include "util/test_assert.h"

const uint32_t SimpleConvolution::input_data_[]{
    15,  201, 51,  89,  92,  34,  96,  66,  11,  225, 161, 96,  81,  211, 108, 124, 202, 244, 182,
    90,  215, 92,  98,  20,  44,  225, 55,  247, 202, 0,   45,  218, 202, 97,  51,  39,  131, 147,
    105, 143, 116, 11,  239, 198, 222, 92,  67,  169, 81,  250, 3,   40,  86,  101, 60,  131, 70,
    116, 123, 17,  117, 168, 236, 64,  10,  31,  103, 142, 179, 209, 29,  40,  220, 13,  239, 187,
    105, 50,  100, 186, 44,  104, 227, 131, 205, 32,  6,   20,  149, 130, 38,  10,  43,  18,  75,
    53,  50,  178, 195, 230, 132, 225, 14,  96,  238, 253, 27,  88,  48,  128, 18,  92,  232, 246,
    224, 182, 23,  231, 203, 172, 105, 241, 183, 148, 4,   2,   202, 55,  181, 142, 29,  57,  111,
    43,  153, 93,  41,  181, 181, 89,  54,  200, 182, 31,  190, 150, 213, 213, 126, 160, 130, 232,
    146, 57,  125, 151, 59,  71,  206, 240, 213, 236, 42,  68,  24,  195, 162, 65,  121, 87,  155,
    175, 31,  81,  207, 222, 232, 164, 180, 102, 69,  55,  79,  216, 112, 204, 112, 171, 19,  63,
    156, 233, 43,  198, 46,  67,  138, 208, 132, 4,   39,  32,  180, 71,  113, 131, 38,  90,  40,
    219, 193, 109, 18,  16,  70,  131, 220, 182, 46,  240, 245, 203, 217, 32,  146, 7,   100, 28,
    216, 233, 32,  255, 9,   213, 71,  123, 88,  110, 213, 128, 74,  150, 238, 93,  166, 52,  224,
    131, 234, 15,  115, 224, 218, 76,  1,   108, 84,  101, 137, 44,  79,  170, 44,  88,  127, 116,
    211, 216, 226, 168, 88,  45,  63,  70,  138, 230, 123, 107, 105, 101, 122, 220, 70,  84,  41,
    71,  193, 125, 173, 75,  169, 252, 245, 213, 84,  117, 73,  40,  77,  44,  209, 166, 90,  16,
    237, 229, 246, 104, 80,  95,  206, 202, 60,  20,  31,  101, 92,  225, 226, 9,   44,  140, 5,
    34,  97,  89,  151, 171, 129, 229, 216, 82,  139, 51,  99,  120, 24,  89,  225, 104, 185, 175,
    50,  246, 196, 82,  91,  32,  51,  62,  42,  96,  202, 47,  130, 44,  137, 26,  215, 10,  255,
    176, 93,  138, 227, 193, 3,   251, 27,  229, 100, 212, 149, 151, 202, 89,  233, 38,  122, 29,
    100, 164, 125, 46,  212, 0,   90,  93,  26,  50,  103, 25,  226, 197, 164, 198, 135, 168, 194,
    162, 141, 38,  119, 34,  190, 66,  124, 167, 104, 247, 197, 204, 156, 67,  251, 112, 67,  85,
    205, 93,  135, 53,  119, 106, 251, 28,  49,  130, 196, 243, 36,  82,  26,  155, 117, 216, 221,
    241, 128, 70,  233, 70,  18,  133, 137, 14,  245, 204, 99,  195, 42,  235, 248, 161, 86,  243,
    190, 135, 118, 130, 123, 154, 213, 150, 54,  74,  111, 20,  60,  240, 90,  37,  54,  109, 171,
    191, 123, 161, 140, 222, 100, 182, 202, 93,  88,  32,  80,  23,  168, 198, 153, 36,  97,  111,
    187, 151, 185, 43,  172, 245, 27,  6,   27,  82,  115, 199, 18,  239, 104, 158, 206, 205, 85,
    152, 42,  174, 185, 123, 197, 98,  65,  95,  135, 163, 206, 66,  59,  136, 109, 231, 125, 137,
    237, 153, 219, 97,  96,  237, 81,  201, 140, 31,  150, 226, 183, 192, 144, 113, 59,  86,  212,
    125, 182, 91,  33,  132, 158, 92,  12,  12,  68,  138, 149, 50,  36,  113, 147, 133, 95,  229,
    78,  235, 4,   228, 206, 188, 165, 95,  45,  225, 181, 1,   94,  107, 93,  128, 240, 251, 220,
    252, 7,   32,  135, 156, 83,  171, 14,  230, 48,  109, 203, 126, 89,  208, 99,  39,  140, 9,
    134, 185, 234, 60,  187, 73,  167, 24,  201, 152, 20,  166, 148, 27,  199, 28,  184, 26,  199,
    198, 0,   248, 52,  204, 119, 141, 157, 218, 181, 41,  227, 59,  227, 206, 119, 159, 23,  31,
    184, 224, 183, 204, 134, 76,  231, 77,  105, 160, 103, 48,  103, 104, 41,  155, 53,  160, 41,
    210, 123, 222, 252, 95,  26,  223, 45,  146, 126, 68,  177, 54,  37,  105, 3,   171, 182, 235,
    249, 31,  139, 97,  80,  243, 202, 121, 143, 0,   26,  184, 210, 149, 151, 207, 244, 177, 174,
    34,  67,  45,  102, 245, 100, 140, 95,  104, 55,  21,  83,  49,  53,  223, 147, 134, 210, 93,
    0,   97,  93,  26,  26,  48,  175, 178, 255, 164, 99,  174, 198, 167, 220, 45,  156, 64,  185,
    252, 168, 241, 18,  252, 35,  71,  219, 182, 205, 173, 19,  206, 15,  113, 232, 42,  161, 152,
    220, 160, 60,  64,  79,  3,   231, 43,  49,  132, 108, 235, 128, 21,  220, 146, 17,  255, 218,
    236, 182, 168, 154, 201, 118, 170, 58,  94,  212, 220, 246, 177, 125, 51,  241, 204, 55,  216,
    248, 104, 92,  100, 83,  221, 121, 48,  111, 138, 47,  73,  119, 230, 241, 17,  175, 103, 187,
    234, 198, 144, 199, 188, 65,  68,  240, 51,  17,  39,  11,  9,   143, 104, 109, 227, 70,  231,
    19,  181, 113, 66,  255, 233, 41,  241, 250, 217, 89,  182, 196, 31,  71,  139, 220, 137, 208,
    204, 188, 225, 243, 200, 234, 131, 48,  88,  102, 119, 63,  121, 44,  177, 188, 44,  154, 229,
    29,  149, 190, 118, 76,  130, 150, 147, 14,  114, 28,  222, 62,  217, 191, 50,  161, 170, 181,
    210, 2,   28,  73,  66,  149, 117, 243, 81,  162, 141, 55,  191, 35,  245, 54,  111, 120, 204,
    2,   134, 62,  31,  100, 125, 248, 36,  175, 153, 206, 101, 107, 209, 129, 181, 19,  22,  43,
    7,   104, 205, 149, 159, 140, 184, 149, 195, 39,  14,  143, 42,  148, 205, 73,  249, 74,  66,
    30,  250, 219, 237, 96,  71,  190, 225, 253, 210, 248, 40,  218, 96,  245, 111, 0,   130, 39,
    150, 69,  79,  165, 212, 122, 57,  162, 195, 51,  237, 6,   82,  231, 225, 63,  71,  41,  253,
    41,  38,  208, 33,  78,  170, 130, 68,  26,  131, 198, 66,  26,  12,  145, 191, 224, 11,  249,
    130, 207, 44,  112, 213, 126, 88,  183, 190, 160, 225, 187, 201, 8,   140, 235, 87,  55,  109,
    155, 81,  241, 98,  147, 11,  110, 37,  202, 79,  49,  195, 210, 0,   240, 66,  214, 110, 154,
    142, 44,  58,  111, 232, 4,   119, 117, 239, 207, 172, 93,  106, 254, 78,  205, 145, 89,  59,
    183, 35,  138, 232, 230, 92,  233, 214, 159, 191, 69,  58,  78,  114, 116, 189, 91,  121, 53,
    208, 104, 4,   125, 198, 111, 123, 20,  60,  13,  109, 120, 196, 145, 3,   172, 119, 95,  150,
    78,  255, 85,  147, 57,  163, 6,   174, 97,  97,  39,  151, 50,  144, 155, 175, 86,  11,  43,
    107, 71,  56,  216, 191, 253, 105, 194, 170, 225, 34,  64,  47,  34,  150, 195, 91,  58,  201,
    10,  155, 43,  49,  50,  93,  194, 206, 13,  25,  217, 56,  132, 33,  112, 92,  225, 109, 198,
    164, 23,  167, 199, 88,  215, 234, 238, 155, 69,  40,  100, 80,  196, 144, 129, 246, 237, 68,
    197, 250, 93,  159, 51,  225, 193, 163, 62,  163, 17,  4,   71,  41,  172, 15,  130, 132, 249,
    112, 31,  63,  152, 132, 143, 92,  20,  17,  83,  1,   86,  25,  252, 179, 185, 47,  149, 122,
    211, 211, 29,  229, 216, 101, 15,  133, 117, 145, 9,   111, 1,   40,  175, 154, 173, 62,  247,
    193, 80,  75,  194, 166, 100, 191, 90,  29,  239, 239, 152, 194, 195, 182, 168, 156, 27,  183,
    33,  145, 73,  43,  0,   75,  83,  175, 229, 0,   238, 221, 194, 63,  40,  133, 230, 140, 68,
    64,  170, 51,  48,  66,  246, 243, 248, 159, 144, 20,  87,  177, 165, 160, 220, 166, 235, 48,
    86,  209, 49,  68,  174, 243, 132, 214, 120, 106, 99,  189, 170, 13,  241, 219, 80,  232, 207,
    72,  135, 95,  92,  223, 16,  2,   127, 237, 169, 107, 29,  255, 61,  79,  68,  236, 67,  200,
    194, 188, 50,  38,  121, 221, 52,  107, 184, 132, 84,  136, 204, 219, 231, 41,  186, 248, 44,
    58,  229, 213, 166, 3,   212, 227, 82,  25,  207, 150, 225, 146, 82,  20,  185, 204, 242, 237,
    55,  170, 113, 139, 50,  62,  103, 26,  103, 34,  18,  148, 93,  247, 105, 3,   251, 62,  231,
    77,  87,  182, 227, 57,  73,  54,  77,  2,   2,   63,  239, 57,  234, 97,  197, 29,  159, 44,
    55,  7,   79,  74,  155, 172, 66,  5,   175, 61,  67,  150, 139, 155, 77,  111, 212, 151, 165,
    34,  153, 167, 98,  137, 225, 77,  234, 166, 107, 138, 211, 163, 145, 34,  237, 45,  206, 47,
    50,  126, 108, 117, 21,  248, 17,  98,  103, 230, 249, 12,  9,   147, 179, 107, 29,  149, 185,
    7,   59,  37,  146, 14,  200, 35,  49,  182, 80,  0,   230, 130, 126, 83,  248, 148, 75,  9,
    247, 178, 240, 240, 190, 249, 132, 114, 101, 161, 7,   30,  169, 67,  68,  59,  82,  12,  95,
    131, 195, 176, 131, 169, 51,  2,   252, 44,  150, 72,  54,  141, 250, 38,  126, 185, 31,  3,
    44,  132, 165, 52,  163, 78,  120, 231, 138, 202, 244, 234, 77,  183, 155, 209, 97,  207, 212,
    94,  251, 107, 166, 49,  249, 161, 88,  120, 91,  120, 123, 135, 253, 33,  188, 160, 112, 52,
    136, 250, 254, 125, 229, 76,  53,  128, 30,  150, 79,  243, 244, 75,  95,  155, 125, 88,  60,
    213, 209, 152, 78,  77,  32,  75,  110, 220, 236, 222, 17,  117, 217, 15,  242, 190, 92,  39,
    63,  123, 190, 143, 111, 178, 219, 206, 78,  88,  38,  138, 46,  247, 34,  124, 69,  66,  199,
    179, 31,  179, 145, 48,  41,  106, 64,  27,  41,  157, 67,  105, 24,  1,   249, 135, 179, 212,
    86,  1,   44,  124, 140, 91,  116, 175, 215, 185, 242, 159, 108, 17,  83,  254, 66,  124, 105,
    131, 151, 146, 32,  218, 252, 57,  219, 245, 193, 143, 201, 23,  145, 246, 148, 30,  82,  8,
    206, 41,  194, 192, 201, 47,  210, 28,  46,  20,  152, 151, 151, 48,  42,  184, 11,  38,  241,
    231, 28,  179, 119, 230, 202, 8,   220, 94,  39,  46,  103, 245, 88,  42,  181, 33,  90,  136,
    62,  136, 156, 214, 31,  52,  7,   74,  237, 19,  113, 223, 250, 141, 146, 113, 115, 92,  122,
    80,  187, 161, 126, 35,  150, 215, 78,  76,  249, 168, 212, 55,  48,  113, 14,  80,  166, 21,
    154, 147, 40,  12,  114, 35,  153, 5,   148, 12,  98,  15,  92,  29,  176, 219, 65,  71,  179,
    143, 147, 172, 56,  104, 227, 104, 218, 241, 185, 128, 7,   84,  20,  47,  96,  135, 82,  249,
    140, 231, 6,   238, 246, 99,  12,  167, 63,  77,  238, 242, 221, 130, 158, 21,  235, 129, 126,
    197, 114, 56,  69,  121, 140, 90,  169, 237, 225, 252, 231, 109, 228, 237, 91,  219, 81,  104,
    130, 144, 181, 113, 130, 147, 244, 32,  169, 223, 162, 39,  164, 21,  95,  234, 143, 236, 68,
    57,  217, 37,  53,  192, 147, 25,  174, 239, 245, 0,   87,  119, 144, 13,  232, 19,  160, 220,
    51,  73,  188, 214, 113, 96,  235, 209, 75,  122, 190, 144, 179, 151, 181, 233, 88,  73,  3,
    7,   56,  248, 7,   143, 112, 152, 156, 89,  171, 61,  53,  223, 135, 242, 181, 248, 83,  161,
    202, 158, 28,  136, 46,  208, 32,  228, 186, 121, 45,  189, 128, 102, 182, 136, 246, 38,  32,
    147, 127, 204, 208, 181, 171, 87,  167, 97,  80,  250, 2,   26,  153, 31,  163, 200, 239, 195,
    172, 169, 60,  218, 103, 188, 65,  30,  69,  55,  68,  102, 202, 196, 50,  154, 121, 221, 242,
    33,  63,  67,  28,  66,  93,  181, 97,  0,   126, 81,  196, 43,  251, 0,   5,   98,  189, 70,
    128, 3,   126, 197, 105, 72,  137, 155, 227, 3,   121, 214, 36,  184, 25,  65,  250, 118, 247,
    91,  119, 117, 173, 60,  160, 168, 60,  166, 10,  250, 237, 139, 253, 107, 80,  102, 180, 217,
    2,   151, 221, 123, 109, 1,   52,  134, 66,  46,  253, 57,  138, 117, 175, 55,  178, 79,  223,
    239, 245, 234, 233, 226, 117, 231, 78,  198, 78,  2,   159, 80,  154, 124, 204, 7,   126, 0,
    142, 193, 47,  140, 251, 185, 2,   170, 241, 180, 249, 208, 163, 239, 186, 141, 210, 48,  116,
    32,  246, 195, 34,  150, 19,  188, 19,  224, 196, 146, 224, 83,  83,  15,  224, 78,  201, 226,
    249, 186, 151, 243, 139, 58,  226, 70,  199, 181, 118, 60,  213, 109, 255, 248, 3,   19,  181,
    23,  243, 122, 169, 212, 205, 252, 228, 173, 75,  173, 144, 68,  104, 39,  55,  243, 98,  26,
    57,  41,  207, 175, 102, 165, 29,  102, 158, 32,  121, 83,  56,  109, 205, 225, 66,  155, 222,
    38,  73,  42,  212, 218, 110, 60,  1,   166, 48,  99,  193, 105, 141, 145, 25,  244, 54,  54,
    90,  213, 87,  212, 40,  143, 66,  246, 112, 132, 146, 79,  171, 220, 121, 128, 182, 232, 189,
    184, 143, 237, 27,  80,  86,  169, 226, 112, 158, 25,  166, 248, 238, 253, 204, 23,  141, 15,
    13,  254, 147, 160, 77,  63,  124, 199, 191, 50,  175, 124, 234, 62,  105, 6,   143, 192, 176,
    113, 48,  78,  139, 215, 71,  121, 213, 20,  144, 98,  35,  158, 96,  183, 62,  174, 246, 187,
    117, 182, 237, 37,  50,  216, 99,  156, 223, 243, 93,  143, 101, 142, 222, 240, 101, 37,  106,
    58,  57,  250, 157, 93,  153, 254, 20,  216, 172, 10,  147, 34,  192, 129, 71,  243, 90,  171,
    144, 57,  159, 238, 201, 4,   124, 167, 244, 225, 205, 95,  28,  7,   89,  185, 100, 243, 184,
    121, 203, 100, 131, 95,  135, 68,  224, 207, 56,  58,  122, 201, 115, 25,  183, 61,  30,  51,
    229, 18,  21,  178, 113, 49,  186, 203, 235, 31,  191, 163, 152, 138, 8,   28,  233, 143, 97,
    202, 95,  153, 4,   217, 98,  120, 243, 26,  182, 17,  77,  155, 36,  99,  78,  150, 149, 8,
    98,  128, 39,  33,  36,  192, 172, 45,  220, 149, 189, 61,  96,  28,  215, 100, 246, 58,  221,
    233, 84,  147, 251, 162, 47,  31,  5,   125, 181, 154, 134, 23,  27,  174, 57,  64,  110, 229,
    109, 75,  123, 43,  136, 219, 71,  95,  64,  61,  154, 29,  39,  238, 177, 34,  145, 225, 65,
    150, 94,  247, 49,  229, 15,  77,  147, 72,  141, 2,   45,  251, 77,  169, 38,  213, 132, 110,
    53,  196, 172, 207, 226, 212, 190, 148, 246, 79,  117, 56,  230, 212, 48,  23,  185, 63,  100,
    76,  136, 242, 78,  181, 237, 156, 95,  20,  113, 227, 131, 167, 168, 47,  119, 139, 3,   53,
    31,  250, 133, 149, 50,  107, 105, 99,  130, 34,  162, 231, 111, 42,  217, 190, 224, 199, 90,
    63,  220, 204, 35,  95,  115, 203, 143, 234, 86,  147, 32,  118, 141, 165, 11,  192, 16,  117,
    35,  147, 152, 198, 123, 7,   240, 84,  198, 209, 28,  33,  17,  248, 237, 52,  88,  97,  255,
    231, 76,  86,  122, 109, 204, 8,   18,  216, 201, 35,  77,  237, 183, 229, 179, 50,  237, 164,
    135, 179, 118, 164, 213, 135, 157, 195, 187, 245, 36,  187, 220, 113, 18,  87,  222, 222, 96,
    241, 183, 42,  21,  4,   23,  205, 233, 203, 0,   214, 112, 136, 138, 230, 44,  95,  110, 201,
    34,  41,  191, 71,  229, 155, 185, 247, 243, 151, 214, 84,  137, 141, 126, 159, 146, 149, 108,
    124, 97,  109, 82,  209, 245, 221, 183, 34,  60,  37,  236, 95,  79,  171, 167, 53,  71,  96,
    45,  58,  248, 3,   142, 129, 145, 12,  33,  36,  162, 142, 160, 3,   251, 243, 213, 240, 208,
    141, 19,  13,  178, 255, 109, 2,   170, 20,  55,  241, 116, 101, 44,  108, 105, 186, 238, 251,
    199, 15,  31,  106, 157, 191, 110, 152, 178, 67,  137, 131, 208, 156, 144, 131, 155, 253, 134,
    70,  18,  190, 55,  134, 35,  99,  243, 140, 30,  225, 135, 230, 240, 166, 81,  142, 102, 191,
    39,  25,  3,   177, 156, 211, 77,  45,  87,  233, 43,  221, 48,  61,  155, 103, 195, 191, 203,
    182, 75,  233, 152, 211, 208, 136, 121, 33,  23,  224, 224, 62,  249, 227, 239, 149, 183, 61,
    195, 15,  39,  238, 236, 87,  43,  136, 191, 239, 71,  138, 166, 147, 116, 62,  102, 68,  199,
    224, 101, 223, 193, 70,  29,  186, 42,  13,  80,  225, 75,  19,  241, 115, 1,   221, 202, 45,
    102, 137, 29,  174, 20,  195, 66,  136, 2,   168, 205, 201, 137, 50,  168, 74,  121, 198, 4,
    163, 212, 85,  133, 31,  105, 118, 146, 106, 84,  93,  152, 187, 231, 181, 105, 251, 121, 171,
    132, 123, 84,  81,  69,  221, 132, 238, 40,  253, 181, 45,  161, 137, 130, 39,  169, 235, 158,
    59,  86,  242, 153, 239, 173, 128, 165, 23,  123, 30,  195, 0,   154, 23,  81,  224, 245, 214,
    206, 30,  212, 131, 75,  117, 12,  206, 157, 181, 186, 59,  241, 17,  45,  138, 0,   219, 11,
    165, 243, 135, 196, 182, 135, 95,  205, 217, 63,  195, 175, 14,  225, 131, 145, 45,  249, 158,
    251, 150, 84,  182, 209, 70,  199, 255, 209, 199, 219, 220, 109, 206, 99,  50,  132, 234, 146,
    82,  195, 209, 22,  114, 223, 247, 246, 113, 37,  239, 16,  33,  134, 100, 215, 88,  170, 158,
    87,  123, 102, 50,  88,  211, 1,   187, 6,   134, 165, 152, 216, 105, 106, 239, 220, 74,  231,
    210, 187, 12,  194, 204, 45,  72,  49,  4,   160, 219, 162, 248, 87,  8,   43,  176, 220, 44,
    107, 227, 178, 17,  124, 139, 122, 230, 122, 87,  48,  97,  42,  236, 110, 236, 185, 155, 53,
    234, 159, 214, 198, 66,  206, 30,  75,  249, 206, 40,  38,  57,  11,  217, 74,  136, 100, 197,
    110, 223, 29,  159, 65,  71,  140, 175, 51,  69,  74,  105, 48,  234, 63,  246, 45,  13,  20,
    121, 7,   226, 161, 46,  28,  173, 7,   103, 53,  108, 45,  164, 76,  74,  68,  141, 145, 208,
    61,  197, 22,  136, 46,  70,  115, 110, 60,  161, 124, 81,  26,  132, 51,  188, 178, 79,  106,
    186, 183, 160, 39,  228, 68,  115, 46,  136, 1,   192, 89,  62,  133, 112, 198, 180, 182, 58,
    34,  243, 219, 158, 69,  245, 34,  120, 178, 213, 200, 28,  143, 128, 188, 182, 100, 1,   41,
    146, 137, 43,  82,  227, 105, 216, 83,  48,  140, 10,  106, 175, 254, 70,  77,  67,  59,  112,
    188, 237, 69,  133, 10,  212, 5,   198, 138, 105, 199, 180, 252, 81,  223, 79,  53,  73,  39,
    137, 121, 180, 148, 228, 99,  146, 42,  177, 214, 102, 33,  147, 84,  102, 25,  94,  59,  31,
    37,  197, 137, 237, 122, 133, 63,  90,  213, 116, 163, 253, 253, 29,  177, 145, 2,   21,  36,
    45,  198, 251, 147, 231, 143, 232, 78,  168, 71,  137, 199, 108, 79,  80,  90,  201, 214, 153,
    35,  172, 13,  199, 169, 11,  228, 91,  157, 231, 112, 193, 20,  54,  189, 167, 30,  77,  144,
    108, 245, 215, 246, 189, 68,  69,  14,  158, 14,  228, 55,  50,  145, 69,  249, 58,  80,  222,
    149, 237, 198, 5,   175, 218, 60,  109, 130, 91,  186, 18,  200, 175, 234, 190, 109, 46,  3,
    123, 204, 18,  96,  4,   68,  241, 73,  62,  44,  154, 29,  193, 136, 227, 199, 55,  189, 4,
    164, 64,  95,  95,  82,  39,  15,  60,  230, 124, 107, 233, 248, 55,  251, 89,  60,  63,  75,
    134, 126, 119, 32,  156, 57,  168, 127, 0,   224, 61,  5,   133, 125, 100, 228, 208, 140, 243,
    12,  114, 111, 119, 92,  104, 175, 87,  193, 236, 151, 13,  114, 21,  132, 146, 177, 189, 59,
    49,  190, 27,  110, 195, 160, 236, 40,  132, 188, 181, 120, 201, 40,  232, 65,  132, 80,  241,
    220, 18,  221, 115, 31,  79,  137, 164, 226, 58,  98,  29,  108, 32,  57,  219, 228, 218, 199,
    13,  95,  132, 195, 215, 77,  235, 191, 143, 112, 16,  128, 76,  35,  93,  191, 66,  173, 73,
    231, 143, 132, 73,  173, 240, 106, 231, 203, 78,  193, 147, 92,  33,  23,  31,  248, 100, 11,
    184, 243, 123, 201, 115, 200, 236, 209, 135, 47,  126, 209, 22,  14,  85,  95,  188, 69,  202,
    163, 17,  24,  101, 164, 117, 134, 187, 148, 127, 31,  159, 55,  19,  27,  1,   135, 227, 237,
    89,  107, 28,  216, 60,  51,  230, 145, 147, 163, 215, 93,  70,  232, 118, 172, 140, 235, 50,
    71,  128, 177, 103, 32,  233, 123, 60,  234, 2,   31,  216, 91,  139, 244, 52,  200, 40,  26,
    90,  188, 189, 49,  25,  4,   25,  144, 176, 166, 124, 227, 237, 252, 148, 85,  29,  125, 208,
    89,  104, 210, 121, 64,  46,  4,   53,  99,  204, 93,  125, 38,  25,  59,  88,  51,  64,  113,
    195, 241, 23,  64,  212, 5,   60,  104, 90,  90,  230, 42,  179, 78,  253, 44,  143, 44,  49,
    196, 143, 254, 34,  13,  36,  60,  73,  125, 112, 137, 239, 52,  122, 7,   116, 79,  12,  177,
    183, 103, 11,  158, 146, 190, 237, 143, 235, 124, 188, 28,  65,  76,  26,  100, 89,  63,  160,
    163, 188, 17,  44,  172, 69,  167, 179, 185, 246, 191, 107, 174, 38,  118, 76,  184, 53,  58,
    72,  32,  182, 5,   61,  248, 81,  88,  92,  170, 152, 253, 77,  84,  14,  122, 1,   83,  34,
    180, 13,  25,  115, 120, 199, 154, 238, 20,  83,  36,  79,  155, 68,  5,   160, 130, 254, 242,
    218, 90,  156, 114, 87,  234, 199, 101, 101, 200, 185, 135, 124, 198, 160, 240, 62,  104, 138,
    45,  125, 222, 81,  204, 122, 150, 210, 26,  24,  208, 12,  242, 42,  169, 101, 130, 148, 44,
    232, 249, 245, 161, 128, 113, 103, 33,  98,  166, 137, 236, 212, 7,   202, 38,  211, 69,  188,
    165, 95,  212, 118, 108, 199, 161, 22,  45,  35,  170, 90,  11,  163, 79,  173, 36,  193, 20,
    69,  35,  187, 207, 16,  144, 214, 219, 182, 170, 32,  114, 79,  128, 71,  198, 237, 15,  103,
    4,   60,  139, 175, 150, 151, 82,  230, 68,  119, 168, 89,  188, 204, 20,  140, 220, 165, 98,
    184, 91,  12,  217, 205, 92,  90,  20,  35,  71,  36,  138, 76,  96,  22,  251, 247, 173, 78,
    222, 241, 197, 134, 75,  130, 83,  96,  14,  47,  5,   113, 232, 96,  126, 193, 45,  218, 28,
    66,  253, 99,  103, 136, 176, 200, 158, 171, 191, 76,  249, 158, 62,  190, 37,  137, 65,  120,
    233, 80,  168, 238, 193, 145, 79,  63,  82,  125, 26,  111, 191, 24,  210, 39,  161, 131, 239,
    64,  46,  175, 140, 39,  77,  202, 230, 115, 84,  40,  235, 62,  120, 148, 45,  57,  37,  124,
    121, 120, 249, 148, 231, 185, 172, 186, 224, 77,  61,  207, 141, 107, 126, 26,  147, 204, 229,
    121, 63,  58,  161, 43,  120, 25,  191, 165, 83,  228, 34,  205, 92,  27,  97,  67,  213, 13,
    253, 182, 91,  59,  133, 233, 166, 4,   4,   57,  209, 233, 179, 16,  35,  85,  59,  155, 111,
    250, 65,  194, 223, 99,  144, 59,  127, 241, 127, 85,  255, 125, 11,  90,  184, 145, 68,  95,
    150, 72,  153, 103, 49,  76,  120, 85,  161, 179, 241, 16,  174, 51,  211, 142, 150, 99,  201,
    22,  85,  73,  108, 84,  199, 120, 175, 128, 9,   243, 223, 160, 59,  120, 8,   109, 197, 128,
    194, 103, 52,  180, 119, 227, 231, 75,  113, 126, 175, 59,  148, 4,   132, 1,   89,  75,  121,
    8,   204, 131, 251, 171, 36,  55,  36,  44,  165, 233, 172, 103, 80,  224, 28,  200, 195, 3,
    20,  53,  129, 195, 112, 22,  200, 244, 23,  34,  64,  145, 42,  12,  20,  38,  184, 56,  94,
    220, 101, 3,   198, 17,  107, 22,  242, 135, 222, 182, 138, 243, 235, 11,  182, 91,  34,  127,
    80,  58,  161, 145, 203, 204, 158, 224, 242, 86,  24,  81,  51,  126, 84,  249, 143, 191, 15,
    130, 70,  238, 57,  209, 225, 36,  221, 152, 128, 255, 24,  208, 57,  186, 97,  4,   134, 255,
    229, 121, 86,  254, 202, 137, 124, 31,  130, 12,  222, 146, 142, 37,  129, 199, 247, 98,  236,
    212, 251, 108, 211, 20,  60,  13,  206, 158, 18,  84};

SimpleConvolution::SimpleConvolution() {
  width_ = 64;
  height_ = 64;
  mask_width_ = 3;
  mask_height_ = mask_width_;
  randomize_seed_ = 0;

  if (!IsPowerOf2(width_)) {
    width_ = RoundToPowerOf2(width_);
  }

  if (!IsPowerOf2(height_)) {
    height_ = RoundToPowerOf2(height_);
  }

  if (!(mask_width_ % 2)) {
    mask_width_++;
  }

  if (!(mask_height_ % 2)) {
    mask_height_++;
  }

  if (width_ * height_ < 256) {
    width_ = 64;
    height_ = 64;
  }

  const uint32_t input_size_bytes = width_ * height_ * sizeof(uint32_t);
  const uint32_t mask_size_bytes = mask_width_ * mask_height_ * sizeof(float);

  SetInDescr(KERNARG_BUF_ID, KERNARG_DES_ID, sizeof(kernel_args_t));
  SetInDescr(INPUT_BUF_ID, SYS_DES_ID, input_size_bytes);
  SetInDescr(MASK_BUF_ID, SYS_DES_ID, mask_size_bytes);
  SetOutDescr(LOCAL_BUF_ID, LOCAL_DES_ID, input_size_bytes);
  SetHostDescr(REFOUT_BUF_ID, REFOUT_DES_ID, input_size_bytes);

  if (!randomize_seed_) TEST_ASSERT(sizeof(input_data_) <= input_size_bytes);
}

void SimpleConvolution::Init() {
  std::clog << "SimpleConvolution::init :" << std::endl;

  mem_descr_t kernarg_des = GetDescr(KERNARG_BUF_ID);
  mem_descr_t input_des = GetDescr(INPUT_BUF_ID);
  mem_descr_t mask_des = GetDescr(MASK_BUF_ID);
  mem_descr_t output_des = GetDescr(LOCAL_BUF_ID);
#if 0
  printf("kernarg_des %p 0x%x\n", kernarg_des.ptr, kernarg_des.size);
  printf("input_des %p 0x%x\n", input_des.ptr, input_des.size);
  printf("mask_des %p 0x%x\n", mask_des.ptr, mask_des.size);
  printf("output_des %p 0x%x\n", output_des.ptr, output_des.size);
#endif
  uint32_t* input = reinterpret_cast<uint32_t*>(input_des.ptr);
  uint32_t* output_local = reinterpret_cast<uint32_t*>(output_des.ptr);
  float* mask = reinterpret_cast<float*>(mask_des.ptr);
  kernel_args_t* kernel_args = reinterpret_cast<kernel_args_t*>(kernarg_des.ptr);

  if (randomize_seed_) {
    // random initialisation of input
    FillRandom<uint32_t>(input, width_, height_, 0, 255, randomize_seed_);
  } else {
    // initialization with preset values
    memcpy(input, input_data_, width_ * height_ * sizeof(uint32_t));
  }

  // Fill a blurr filter or some other filter of your choice
  const float val = 1.0f / (mask_width_ * 2.0f - 1.0f);
  for (uint32_t i = 0; i < (mask_width_ * mask_height_); i++) {
    mask[i] = 0;
  }
  for (uint32_t i = 0; i < mask_width_; i++) {
    uint32_t y = mask_height_ / 2;
    mask[y * mask_width_ + i] = val;
  }
  for (uint32_t i = 0; i < mask_height_; i++) {
    uint32_t x = mask_width_ / 2;
    mask[i * mask_width_ + x] = val;
  }

  // Print the INPUT array.
  std::clog << std::dec;
  PrintArray<uint32_t>("> Input[0]", input, width_, 1);
  PrintArray<float>("> Mask", mask, mask_width_, mask_height_);

  // Fill the kernel args
  kernel_args->arg1 = output_local;
  kernel_args->arg2 = input;
  kernel_args->arg3 = mask;
  kernel_args->arg4 = width_;
  kernel_args->arg41 = height_;
  kernel_args->arg5 = mask_width_;
  kernel_args->arg51 = mask_height_;

  // Calculate the reference output
  ReferenceImplementation(reinterpret_cast<uint32_t*>(GetRefOut()), input, mask, width_, height_,
                          mask_width_, mask_height_);
}

void SimpleConvolution::PrintOutput(const void* ptr) const {
  PrintArray<uint32_t>("> Output[0]", reinterpret_cast<const uint32_t*>(ptr), width_, 1);
}

bool SimpleConvolution::ReferenceImplementation(uint32_t* output, const uint32_t* input,
                                                const float* mask, const uint32_t width,
                                                const uint32_t height, const uint32_t mask_width,
                                                const uint32_t mask_height) {
  const uint32_t vstep = (mask_width - 1) / 2;
  const uint32_t hstep = (mask_height - 1) / 2;

  // for each pixel in the input
  for (uint32_t x = 0; x < width; x++) {
    for (uint32_t y = 0; y < height; y++) {
      // find the left, right, top and bottom indices such that
      // the indices do not go beyond image boundaires
      const uint32_t left = (x < vstep) ? 0 : (x - vstep);
      const uint32_t right = ((x + vstep) >= width) ? width - 1 : (x + vstep);
      const uint32_t top = (y < hstep) ? 0 : (y - hstep);
      const uint32_t bottom = ((y + hstep) >= height) ? height - 1 : (y + hstep);

      // initializing wighted sum value
      float sum_fx = 0;
      for (uint32_t i = left; i <= right; ++i) {
        for (uint32_t j = top; j <= bottom; ++j) {
          // performing wighted sum within the mask boundaries
          uint32_t mask_idx = (j - (y - hstep)) * mask_width + (i - (x - vstep));
          uint32_t index = j * width + i;

          // to round to the nearest integer
          sum_fx += ((float)input[index] * mask[mask_idx]);
        }
      }
      sum_fx += 0.5f;
      output[y * width + x] = uint32_t(sum_fx);
    }
  }

  return true;
}
