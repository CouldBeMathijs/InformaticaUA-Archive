# Generated from src/parser/grammars/Grammar.g4 by ANTLR 4.13.1
# encoding: utf-8
from antlr4 import *
from io import StringIO
import sys
if sys.version_info[1] > 5:
	from typing import TextIO
else:
	from typing.io import TextIO

def serializedATN():
    return [
        4,1,66,531,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,2,6,7,
        6,2,7,7,7,2,8,7,8,2,9,7,9,2,10,7,10,2,11,7,11,2,12,7,12,2,13,7,13,
        2,14,7,14,2,15,7,15,2,16,7,16,2,17,7,17,2,18,7,18,2,19,7,19,2,20,
        7,20,2,21,7,21,2,22,7,22,2,23,7,23,2,24,7,24,2,25,7,25,2,26,7,26,
        2,27,7,27,2,28,7,28,2,29,7,29,2,30,7,30,2,31,7,31,2,32,7,32,2,33,
        7,33,2,34,7,34,2,35,7,35,1,0,5,0,74,8,0,10,0,12,0,77,9,0,1,0,1,0,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,3,1,91,8,1,1,2,1,2,1,2,1,
        2,1,2,1,2,3,2,99,8,2,1,3,1,3,3,3,103,8,3,1,3,1,3,5,3,107,8,3,10,
        3,12,3,110,9,3,1,3,1,3,1,3,1,3,1,3,3,3,117,8,3,1,4,1,4,1,4,1,4,1,
        5,1,5,1,5,5,5,126,8,5,10,5,12,5,129,9,5,1,6,1,6,3,6,133,8,6,1,7,
        1,7,3,7,137,8,7,1,7,1,7,1,7,1,7,1,7,1,8,1,8,1,8,1,9,1,9,1,9,5,9,
        150,8,9,10,9,12,9,153,9,9,1,9,3,9,156,8,9,1,10,1,10,1,10,3,10,161,
        8,10,1,11,1,11,1,11,1,12,1,12,1,13,1,13,1,13,1,13,3,13,172,8,13,
        1,13,1,13,1,13,1,13,5,13,178,8,13,10,13,12,13,181,9,13,1,13,3,13,
        184,8,13,1,13,1,13,1,13,3,13,189,8,13,1,13,1,13,1,13,1,13,5,13,195,
        8,13,10,13,12,13,198,9,13,1,13,3,13,201,8,13,3,13,203,8,13,1,14,
        1,14,1,14,5,14,208,8,14,10,14,12,14,211,9,14,1,15,1,15,1,15,3,15,
        216,8,15,1,16,1,16,3,16,220,8,16,1,16,1,16,1,16,1,16,1,16,1,16,1,
        16,1,16,5,16,230,8,16,10,16,12,16,233,9,16,1,16,1,16,1,16,1,16,1,
        16,1,16,1,16,1,16,1,16,1,16,1,16,3,16,246,8,16,1,16,1,16,3,16,250,
        8,16,1,17,1,17,1,17,1,17,1,17,1,17,1,17,3,17,259,8,17,1,18,1,18,
        1,18,1,18,1,18,1,18,1,19,1,19,3,19,269,8,19,1,20,1,20,1,20,3,20,
        274,8,20,1,20,1,20,3,20,278,8,20,1,20,1,20,3,20,282,8,20,1,20,1,
        20,1,20,1,21,1,21,1,21,1,21,1,21,1,21,5,21,293,8,21,10,21,12,21,
        296,9,21,1,21,1,21,1,22,1,22,1,22,1,22,5,22,304,8,22,10,22,12,22,
        307,9,22,1,22,1,22,1,22,5,22,312,8,22,10,22,12,22,315,9,22,3,22,
        317,8,22,1,23,1,23,1,23,1,23,1,23,1,23,1,23,1,23,1,23,1,23,1,23,
        1,23,1,23,1,23,1,23,1,23,1,23,1,23,1,23,1,23,1,23,1,23,1,23,1,23,
        1,23,3,23,344,8,23,1,23,1,23,1,23,1,23,1,23,1,23,1,23,1,23,1,23,
        1,23,1,23,1,23,1,23,1,23,1,23,1,23,1,23,1,23,1,23,1,23,1,23,1,23,
        1,23,1,23,1,23,1,23,1,23,1,23,1,23,1,23,1,23,1,23,1,23,1,23,1,23,
        1,23,3,23,382,8,23,1,23,1,23,1,23,1,23,1,23,1,23,1,23,1,23,1,23,
        1,23,1,23,1,23,1,23,1,23,5,23,398,8,23,10,23,12,23,401,9,23,1,24,
        1,24,1,24,5,24,406,8,24,10,24,12,24,409,9,24,1,25,1,25,1,26,1,26,
        1,26,1,26,5,26,417,8,26,10,26,12,26,420,9,26,1,27,1,27,3,27,424,
        8,27,1,27,1,27,1,27,3,27,429,8,27,3,27,431,8,27,1,28,1,28,1,28,1,
        28,5,28,437,8,28,10,28,12,28,440,9,28,3,28,442,8,28,1,28,3,28,445,
        8,28,1,28,1,28,1,29,1,29,3,29,451,8,29,1,30,1,30,1,30,1,30,1,30,
        1,30,5,30,459,8,30,10,30,12,30,462,9,30,1,30,1,30,1,30,1,30,4,30,
        468,8,30,11,30,12,30,469,3,30,472,8,30,1,31,5,31,475,8,31,10,31,
        12,31,478,9,31,1,31,1,31,5,31,482,8,31,10,31,12,31,485,9,31,1,31,
        5,31,488,8,31,10,31,12,31,491,9,31,1,31,4,31,494,8,31,11,31,12,31,
        495,1,31,5,31,499,8,31,10,31,12,31,502,9,31,3,31,504,8,31,1,32,5,
        32,507,8,32,10,32,12,32,510,9,32,1,32,1,32,5,32,514,8,32,10,32,12,
        32,517,9,32,1,33,1,33,1,34,1,34,1,34,1,34,1,34,1,34,3,34,527,8,34,
        1,35,1,35,1,35,0,1,46,36,0,2,4,6,8,10,12,14,16,18,20,22,24,26,28,
        30,32,34,36,38,40,42,44,46,48,50,52,54,56,58,60,62,64,66,68,70,0,
        11,1,0,4,5,1,0,63,64,1,0,30,37,2,0,36,36,38,39,1,0,32,33,1,0,40,
        41,1,0,42,45,1,0,46,47,1,0,30,31,1,0,57,60,1,0,53,56,592,0,75,1,
        0,0,0,2,90,1,0,0,0,4,98,1,0,0,0,6,116,1,0,0,0,8,118,1,0,0,0,10,122,
        1,0,0,0,12,130,1,0,0,0,14,134,1,0,0,0,16,143,1,0,0,0,18,146,1,0,
        0,0,20,157,1,0,0,0,22,162,1,0,0,0,24,165,1,0,0,0,26,202,1,0,0,0,
        28,204,1,0,0,0,30,212,1,0,0,0,32,249,1,0,0,0,34,251,1,0,0,0,36,260,
        1,0,0,0,38,268,1,0,0,0,40,270,1,0,0,0,42,286,1,0,0,0,44,316,1,0,
        0,0,46,343,1,0,0,0,48,402,1,0,0,0,50,410,1,0,0,0,52,412,1,0,0,0,
        54,421,1,0,0,0,56,432,1,0,0,0,58,450,1,0,0,0,60,471,1,0,0,0,62,503,
        1,0,0,0,64,508,1,0,0,0,66,518,1,0,0,0,68,526,1,0,0,0,70,528,1,0,
        0,0,72,74,3,2,1,0,73,72,1,0,0,0,74,77,1,0,0,0,75,73,1,0,0,0,75,76,
        1,0,0,0,76,78,1,0,0,0,77,75,1,0,0,0,78,79,5,0,0,1,79,1,1,0,0,0,80,
        91,3,22,11,0,81,91,3,14,7,0,82,91,3,26,13,0,83,91,3,16,8,0,84,91,
        3,6,3,0,85,91,3,4,2,0,86,87,3,46,23,0,87,88,5,1,0,0,88,91,1,0,0,
        0,89,91,5,1,0,0,90,80,1,0,0,0,90,81,1,0,0,0,90,82,1,0,0,0,90,83,
        1,0,0,0,90,84,1,0,0,0,90,85,1,0,0,0,90,86,1,0,0,0,90,89,1,0,0,0,
        91,3,1,0,0,0,92,93,5,2,0,0,93,94,3,62,31,0,94,95,5,61,0,0,95,96,
        5,1,0,0,96,99,1,0,0,0,97,99,5,3,0,0,98,92,1,0,0,0,98,97,1,0,0,0,
        99,5,1,0,0,0,100,102,7,0,0,0,101,103,5,61,0,0,102,101,1,0,0,0,102,
        103,1,0,0,0,103,104,1,0,0,0,104,108,5,6,0,0,105,107,3,8,4,0,106,
        105,1,0,0,0,107,110,1,0,0,0,108,106,1,0,0,0,108,109,1,0,0,0,109,
        111,1,0,0,0,110,108,1,0,0,0,111,112,5,7,0,0,112,117,5,1,0,0,113,
        114,7,0,0,0,114,115,5,61,0,0,115,117,5,1,0,0,116,100,1,0,0,0,116,
        113,1,0,0,0,117,7,1,0,0,0,118,119,3,62,31,0,119,120,3,10,5,0,120,
        121,5,1,0,0,121,9,1,0,0,0,122,127,3,12,6,0,123,124,5,8,0,0,124,126,
        3,12,6,0,125,123,1,0,0,0,126,129,1,0,0,0,127,125,1,0,0,0,127,128,
        1,0,0,0,128,11,1,0,0,0,129,127,1,0,0,0,130,132,5,61,0,0,131,133,
        3,60,30,0,132,131,1,0,0,0,132,133,1,0,0,0,133,13,1,0,0,0,134,136,
        5,9,0,0,135,137,5,61,0,0,136,135,1,0,0,0,136,137,1,0,0,0,137,138,
        1,0,0,0,138,139,5,6,0,0,139,140,3,18,9,0,140,141,5,7,0,0,141,142,
        5,1,0,0,142,15,1,0,0,0,143,144,3,52,26,0,144,145,5,1,0,0,145,17,
        1,0,0,0,146,151,3,20,10,0,147,148,5,8,0,0,148,150,3,20,10,0,149,
        147,1,0,0,0,150,153,1,0,0,0,151,149,1,0,0,0,151,152,1,0,0,0,152,
        155,1,0,0,0,153,151,1,0,0,0,154,156,5,8,0,0,155,154,1,0,0,0,155,
        156,1,0,0,0,156,19,1,0,0,0,157,160,5,61,0,0,158,159,5,10,0,0,159,
        161,3,46,23,0,160,158,1,0,0,0,160,161,1,0,0,0,161,21,1,0,0,0,162,
        163,5,11,0,0,163,164,3,24,12,0,164,23,1,0,0,0,165,166,7,1,0,0,166,
        25,1,0,0,0,167,168,3,62,31,0,168,169,5,61,0,0,169,171,5,12,0,0,170,
        172,3,28,14,0,171,170,1,0,0,0,171,172,1,0,0,0,172,173,1,0,0,0,173,
        183,5,13,0,0,174,184,5,1,0,0,175,179,5,6,0,0,176,178,3,32,16,0,177,
        176,1,0,0,0,178,181,1,0,0,0,179,177,1,0,0,0,179,180,1,0,0,0,180,
        182,1,0,0,0,181,179,1,0,0,0,182,184,5,7,0,0,183,174,1,0,0,0,183,
        175,1,0,0,0,184,203,1,0,0,0,185,186,5,61,0,0,186,188,5,12,0,0,187,
        189,3,28,14,0,188,187,1,0,0,0,188,189,1,0,0,0,189,190,1,0,0,0,190,
        200,5,13,0,0,191,201,5,1,0,0,192,196,5,6,0,0,193,195,3,32,16,0,194,
        193,1,0,0,0,195,198,1,0,0,0,196,194,1,0,0,0,196,197,1,0,0,0,197,
        199,1,0,0,0,198,196,1,0,0,0,199,201,5,7,0,0,200,191,1,0,0,0,200,
        192,1,0,0,0,201,203,1,0,0,0,202,167,1,0,0,0,202,185,1,0,0,0,203,
        27,1,0,0,0,204,209,3,30,15,0,205,206,5,8,0,0,206,208,3,30,15,0,207,
        205,1,0,0,0,208,211,1,0,0,0,209,207,1,0,0,0,209,210,1,0,0,0,210,
        29,1,0,0,0,211,209,1,0,0,0,212,213,3,62,31,0,213,215,5,61,0,0,214,
        216,3,60,30,0,215,214,1,0,0,0,215,216,1,0,0,0,216,31,1,0,0,0,217,
        220,3,52,26,0,218,220,3,46,23,0,219,217,1,0,0,0,219,218,1,0,0,0,
        220,221,1,0,0,0,221,222,5,1,0,0,222,250,1,0,0,0,223,250,3,26,13,
        0,224,250,3,14,7,0,225,250,3,6,3,0,226,250,3,4,2,0,227,231,5,6,0,
        0,228,230,3,32,16,0,229,228,1,0,0,0,230,233,1,0,0,0,231,229,1,0,
        0,0,231,232,1,0,0,0,232,234,1,0,0,0,233,231,1,0,0,0,234,250,5,7,
        0,0,235,250,3,34,17,0,236,250,3,36,18,0,237,250,3,40,20,0,238,250,
        3,42,21,0,239,240,5,14,0,0,240,250,5,1,0,0,241,242,5,15,0,0,242,
        250,5,1,0,0,243,245,5,16,0,0,244,246,3,46,23,0,245,244,1,0,0,0,245,
        246,1,0,0,0,246,247,1,0,0,0,247,250,5,1,0,0,248,250,5,1,0,0,249,
        219,1,0,0,0,249,223,1,0,0,0,249,224,1,0,0,0,249,225,1,0,0,0,249,
        226,1,0,0,0,249,227,1,0,0,0,249,235,1,0,0,0,249,236,1,0,0,0,249,
        237,1,0,0,0,249,238,1,0,0,0,249,239,1,0,0,0,249,241,1,0,0,0,249,
        243,1,0,0,0,249,248,1,0,0,0,250,33,1,0,0,0,251,252,5,17,0,0,252,
        253,5,12,0,0,253,254,3,46,23,0,254,255,5,13,0,0,255,258,3,32,16,
        0,256,257,5,18,0,0,257,259,3,32,16,0,258,256,1,0,0,0,258,259,1,0,
        0,0,259,35,1,0,0,0,260,261,5,19,0,0,261,262,5,12,0,0,262,263,3,46,
        23,0,263,264,5,13,0,0,264,265,3,32,16,0,265,37,1,0,0,0,266,269,3,
        52,26,0,267,269,3,46,23,0,268,266,1,0,0,0,268,267,1,0,0,0,269,39,
        1,0,0,0,270,271,5,20,0,0,271,273,5,12,0,0,272,274,3,38,19,0,273,
        272,1,0,0,0,273,274,1,0,0,0,274,275,1,0,0,0,275,277,5,1,0,0,276,
        278,3,46,23,0,277,276,1,0,0,0,277,278,1,0,0,0,278,279,1,0,0,0,279,
        281,5,1,0,0,280,282,3,46,23,0,281,280,1,0,0,0,281,282,1,0,0,0,282,
        283,1,0,0,0,283,284,5,13,0,0,284,285,3,32,16,0,285,41,1,0,0,0,286,
        287,5,21,0,0,287,288,5,12,0,0,288,289,3,46,23,0,289,290,5,13,0,0,
        290,294,5,6,0,0,291,293,3,44,22,0,292,291,1,0,0,0,293,296,1,0,0,
        0,294,292,1,0,0,0,294,295,1,0,0,0,295,297,1,0,0,0,296,294,1,0,0,
        0,297,298,5,7,0,0,298,43,1,0,0,0,299,300,5,22,0,0,300,301,3,46,23,
        0,301,305,5,23,0,0,302,304,3,32,16,0,303,302,1,0,0,0,304,307,1,0,
        0,0,305,303,1,0,0,0,305,306,1,0,0,0,306,317,1,0,0,0,307,305,1,0,
        0,0,308,309,5,24,0,0,309,313,5,23,0,0,310,312,3,32,16,0,311,310,
        1,0,0,0,312,315,1,0,0,0,313,311,1,0,0,0,313,314,1,0,0,0,314,317,
        1,0,0,0,315,313,1,0,0,0,316,299,1,0,0,0,316,308,1,0,0,0,317,45,1,
        0,0,0,318,319,6,23,-1,0,319,320,5,12,0,0,320,321,3,46,23,0,321,322,
        5,13,0,0,322,344,1,0,0,0,323,344,5,61,0,0,324,344,3,50,25,0,325,
        326,5,25,0,0,326,327,5,12,0,0,327,328,3,46,23,0,328,329,5,13,0,0,
        329,344,1,0,0,0,330,331,5,25,0,0,331,332,5,12,0,0,332,333,3,62,31,
        0,333,334,5,13,0,0,334,344,1,0,0,0,335,336,5,12,0,0,336,337,3,62,
        31,0,337,338,5,13,0,0,338,339,1,0,0,0,339,340,3,46,23,13,340,344,
        1,0,0,0,341,342,7,2,0,0,342,344,3,46,23,12,343,318,1,0,0,0,343,323,
        1,0,0,0,343,324,1,0,0,0,343,325,1,0,0,0,343,330,1,0,0,0,343,335,
        1,0,0,0,343,341,1,0,0,0,344,399,1,0,0,0,345,346,10,11,0,0,346,347,
        7,3,0,0,347,398,3,46,23,12,348,349,10,10,0,0,349,350,7,4,0,0,350,
        398,3,46,23,11,351,352,10,9,0,0,352,353,7,5,0,0,353,398,3,46,23,
        10,354,355,10,8,0,0,355,356,7,6,0,0,356,398,3,46,23,9,357,358,10,
        7,0,0,358,359,7,7,0,0,359,398,3,46,23,8,360,361,10,6,0,0,361,362,
        5,37,0,0,362,398,3,46,23,7,363,364,10,5,0,0,364,365,5,48,0,0,365,
        398,3,46,23,6,366,367,10,4,0,0,367,368,5,49,0,0,368,398,3,46,23,
        5,369,370,10,3,0,0,370,371,5,50,0,0,371,398,3,46,23,4,372,373,10,
        2,0,0,373,374,5,51,0,0,374,398,3,46,23,3,375,376,10,1,0,0,376,377,
        5,10,0,0,377,398,3,46,23,1,378,379,10,18,0,0,379,381,5,12,0,0,380,
        382,3,48,24,0,381,380,1,0,0,0,381,382,1,0,0,0,382,383,1,0,0,0,383,
        398,5,13,0,0,384,385,10,17,0,0,385,386,5,26,0,0,386,387,3,46,23,
        0,387,388,5,27,0,0,388,398,1,0,0,0,389,390,10,16,0,0,390,391,5,28,
        0,0,391,398,5,61,0,0,392,393,10,15,0,0,393,394,5,29,0,0,394,398,
        5,61,0,0,395,396,10,14,0,0,396,398,7,8,0,0,397,345,1,0,0,0,397,348,
        1,0,0,0,397,351,1,0,0,0,397,354,1,0,0,0,397,357,1,0,0,0,397,360,
        1,0,0,0,397,363,1,0,0,0,397,366,1,0,0,0,397,369,1,0,0,0,397,372,
        1,0,0,0,397,375,1,0,0,0,397,378,1,0,0,0,397,384,1,0,0,0,397,389,
        1,0,0,0,397,392,1,0,0,0,397,395,1,0,0,0,398,401,1,0,0,0,399,397,
        1,0,0,0,399,400,1,0,0,0,400,47,1,0,0,0,401,399,1,0,0,0,402,407,3,
        46,23,0,403,404,5,8,0,0,404,406,3,46,23,0,405,403,1,0,0,0,406,409,
        1,0,0,0,407,405,1,0,0,0,407,408,1,0,0,0,408,49,1,0,0,0,409,407,1,
        0,0,0,410,411,7,9,0,0,411,51,1,0,0,0,412,413,3,62,31,0,413,418,3,
        54,27,0,414,415,5,8,0,0,415,417,3,54,27,0,416,414,1,0,0,0,417,420,
        1,0,0,0,418,416,1,0,0,0,418,419,1,0,0,0,419,53,1,0,0,0,420,418,1,
        0,0,0,421,423,5,61,0,0,422,424,3,60,30,0,423,422,1,0,0,0,423,424,
        1,0,0,0,424,430,1,0,0,0,425,428,5,10,0,0,426,429,3,46,23,0,427,429,
        3,56,28,0,428,426,1,0,0,0,428,427,1,0,0,0,429,431,1,0,0,0,430,425,
        1,0,0,0,430,431,1,0,0,0,431,55,1,0,0,0,432,441,5,6,0,0,433,438,3,
        58,29,0,434,435,5,8,0,0,435,437,3,58,29,0,436,434,1,0,0,0,437,440,
        1,0,0,0,438,436,1,0,0,0,438,439,1,0,0,0,439,442,1,0,0,0,440,438,
        1,0,0,0,441,433,1,0,0,0,441,442,1,0,0,0,442,444,1,0,0,0,443,445,
        5,8,0,0,444,443,1,0,0,0,444,445,1,0,0,0,445,446,1,0,0,0,446,447,
        5,7,0,0,447,57,1,0,0,0,448,451,3,46,23,0,449,451,3,56,28,0,450,448,
        1,0,0,0,450,449,1,0,0,0,451,59,1,0,0,0,452,453,5,26,0,0,453,460,
        5,27,0,0,454,455,5,26,0,0,455,456,3,46,23,0,456,457,5,27,0,0,457,
        459,1,0,0,0,458,454,1,0,0,0,459,462,1,0,0,0,460,458,1,0,0,0,460,
        461,1,0,0,0,461,472,1,0,0,0,462,460,1,0,0,0,463,464,5,26,0,0,464,
        465,3,46,23,0,465,466,5,27,0,0,466,468,1,0,0,0,467,463,1,0,0,0,468,
        469,1,0,0,0,469,467,1,0,0,0,469,470,1,0,0,0,470,472,1,0,0,0,471,
        452,1,0,0,0,471,467,1,0,0,0,472,61,1,0,0,0,473,475,3,66,33,0,474,
        473,1,0,0,0,475,478,1,0,0,0,476,474,1,0,0,0,476,477,1,0,0,0,477,
        479,1,0,0,0,478,476,1,0,0,0,479,483,3,68,34,0,480,482,3,66,33,0,
        481,480,1,0,0,0,482,485,1,0,0,0,483,481,1,0,0,0,483,484,1,0,0,0,
        484,489,1,0,0,0,485,483,1,0,0,0,486,488,3,64,32,0,487,486,1,0,0,
        0,488,491,1,0,0,0,489,487,1,0,0,0,489,490,1,0,0,0,490,504,1,0,0,
        0,491,489,1,0,0,0,492,494,3,66,33,0,493,492,1,0,0,0,494,495,1,0,
        0,0,495,493,1,0,0,0,495,496,1,0,0,0,496,500,1,0,0,0,497,499,3,64,
        32,0,498,497,1,0,0,0,499,502,1,0,0,0,500,498,1,0,0,0,500,501,1,0,
        0,0,501,504,1,0,0,0,502,500,1,0,0,0,503,476,1,0,0,0,503,493,1,0,
        0,0,504,63,1,0,0,0,505,507,3,66,33,0,506,505,1,0,0,0,507,510,1,0,
        0,0,508,506,1,0,0,0,508,509,1,0,0,0,509,511,1,0,0,0,510,508,1,0,
        0,0,511,515,5,36,0,0,512,514,3,66,33,0,513,512,1,0,0,0,514,517,1,
        0,0,0,515,513,1,0,0,0,515,516,1,0,0,0,516,65,1,0,0,0,517,515,1,0,
        0,0,518,519,5,52,0,0,519,67,1,0,0,0,520,527,3,70,35,0,521,522,5,
        9,0,0,522,527,5,61,0,0,523,524,7,0,0,0,524,527,5,61,0,0,525,527,
        5,61,0,0,526,520,1,0,0,0,526,521,1,0,0,0,526,523,1,0,0,0,526,525,
        1,0,0,0,527,69,1,0,0,0,528,529,7,10,0,0,529,71,1,0,0,0,59,75,90,
        98,102,108,116,127,132,136,151,155,160,171,179,183,188,196,200,202,
        209,215,219,231,245,249,258,268,273,277,281,294,305,313,316,343,
        381,397,399,407,418,423,428,430,438,441,444,450,460,469,471,476,
        483,489,495,500,503,508,515,526
    ]

class GrammarParser ( Parser ):

    grammarFileName = "Grammar.g4"

    atn = ATNDeserializer().deserialize(serializedATN())

    decisionsToDFA = [ DFA(ds, i) for i, ds in enumerate(atn.decisionToState) ]

    sharedContextCache = PredictionContextCache()

    literalNames = [ "<INVALID>", "';'", "'typedef'", "'typedef;'", "'struct'", 
                     "'union'", "'{'", "'}'", "','", "'enum'", "'='", "'#include'", 
                     "'('", "')'", "'break'", "'continue'", "'return'", 
                     "'if'", "'else'", "'while'", "'for'", "'switch'", "'case'", 
                     "':'", "'default'", "'sizeof'", "'['", "']'", "'.'", 
                     "'->'", "'++'", "'--'", "'+'", "'-'", "'!'", "'~'", 
                     "'*'", "'&'", "'/'", "'%'", "'<<'", "'>>'", "'<'", 
                     "'>'", "'<='", "'>='", "'=='", "'!='", "'^'", "'|'", 
                     "'&&'", "'||'", "'const'", "'float'", "'int'", "'char'", 
                     "'void'" ]

    symbolicNames = [ "<INVALID>", "<INVALID>", "<INVALID>", "<INVALID>", 
                      "<INVALID>", "<INVALID>", "<INVALID>", "<INVALID>", 
                      "<INVALID>", "<INVALID>", "<INVALID>", "<INVALID>", 
                      "<INVALID>", "<INVALID>", "<INVALID>", "<INVALID>", 
                      "<INVALID>", "<INVALID>", "<INVALID>", "<INVALID>", 
                      "<INVALID>", "<INVALID>", "<INVALID>", "<INVALID>", 
                      "<INVALID>", "<INVALID>", "<INVALID>", "<INVALID>", 
                      "<INVALID>", "<INVALID>", "<INVALID>", "<INVALID>", 
                      "<INVALID>", "<INVALID>", "<INVALID>", "<INVALID>", 
                      "<INVALID>", "<INVALID>", "<INVALID>", "<INVALID>", 
                      "<INVALID>", "<INVALID>", "<INVALID>", "<INVALID>", 
                      "<INVALID>", "<INVALID>", "<INVALID>", "<INVALID>", 
                      "<INVALID>", "<INVALID>", "<INVALID>", "<INVALID>", 
                      "<INVALID>", "<INVALID>", "<INVALID>", "<INVALID>", 
                      "<INVALID>", "CHAR", "STRING", "INT", "REAL", "ID", 
                      "WS", "HEADER_LOCAL", "HEADER_SYSTEM", "LINE_COMMENT", 
                      "BLOCK_COMMENT" ]

    RULE_root = 0
    RULE_headerElement = 1
    RULE_typedefDecl = 2
    RULE_structDecl = 3
    RULE_structFieldDecl = 4
    RULE_structFieldList = 5
    RULE_structField = 6
    RULE_enum = 7
    RULE_globalDeclaration = 8
    RULE_labels = 9
    RULE_label = 10
    RULE_include = 11
    RULE_header = 12
    RULE_function = 13
    RULE_parameters = 14
    RULE_parameter = 15
    RULE_statement = 16
    RULE_ifStatement = 17
    RULE_whileLoop = 18
    RULE_forInit = 19
    RULE_forLoop = 20
    RULE_switchStatement = 21
    RULE_switchCase = 22
    RULE_expr = 23
    RULE_arg_list = 24
    RULE_literal = 25
    RULE_declaration = 26
    RULE_init_declarator = 27
    RULE_initializer_list = 28
    RULE_initializer_element = 29
    RULE_array_sizes = 30
    RULE_type = 31
    RULE_pointerQualifier = 32
    RULE_typeQualifier = 33
    RULE_typeSpecifier = 34
    RULE_keyword = 35

    ruleNames =  [ "root", "headerElement", "typedefDecl", "structDecl", 
                   "structFieldDecl", "structFieldList", "structField", 
                   "enum", "globalDeclaration", "labels", "label", "include", 
                   "header", "function", "parameters", "parameter", "statement", 
                   "ifStatement", "whileLoop", "forInit", "forLoop", "switchStatement", 
                   "switchCase", "expr", "arg_list", "literal", "declaration", 
                   "init_declarator", "initializer_list", "initializer_element", 
                   "array_sizes", "type", "pointerQualifier", "typeQualifier", 
                   "typeSpecifier", "keyword" ]

    EOF = Token.EOF
    T__0=1
    T__1=2
    T__2=3
    T__3=4
    T__4=5
    T__5=6
    T__6=7
    T__7=8
    T__8=9
    T__9=10
    T__10=11
    T__11=12
    T__12=13
    T__13=14
    T__14=15
    T__15=16
    T__16=17
    T__17=18
    T__18=19
    T__19=20
    T__20=21
    T__21=22
    T__22=23
    T__23=24
    T__24=25
    T__25=26
    T__26=27
    T__27=28
    T__28=29
    T__29=30
    T__30=31
    T__31=32
    T__32=33
    T__33=34
    T__34=35
    T__35=36
    T__36=37
    T__37=38
    T__38=39
    T__39=40
    T__40=41
    T__41=42
    T__42=43
    T__43=44
    T__44=45
    T__45=46
    T__46=47
    T__47=48
    T__48=49
    T__49=50
    T__50=51
    T__51=52
    T__52=53
    T__53=54
    T__54=55
    T__55=56
    CHAR=57
    STRING=58
    INT=59
    REAL=60
    ID=61
    WS=62
    HEADER_LOCAL=63
    HEADER_SYSTEM=64
    LINE_COMMENT=65
    BLOCK_COMMENT=66

    def __init__(self, input:TokenStream, output:TextIO = sys.stdout):
        super().__init__(input, output)
        self.checkVersion("4.13.1")
        self._interp = ParserATNSimulator(self, self.atn, self.decisionsToDFA, self.sharedContextCache)
        self._predicates = None




    class RootContext(ParserRuleContext):
        __slots__ = 'parser'

        def __init__(self, parser, parent:ParserRuleContext=None, invokingState:int=-1):
            super().__init__(parent, invokingState)
            self.parser = parser

        def EOF(self):
            return self.getToken(GrammarParser.EOF, 0)

        def headerElement(self, i:int=None):
            if i is None:
                return self.getTypedRuleContexts(GrammarParser.HeaderElementContext)
            else:
                return self.getTypedRuleContext(GrammarParser.HeaderElementContext,i)


        def getRuleIndex(self):
            return GrammarParser.RULE_root

        def accept(self, visitor:ParseTreeVisitor):
            if hasattr( visitor, "visitRoot" ):
                return visitor.visitRoot(self)
            else:
                return visitor.visitChildren(self)




    def root(self):

        localctx = GrammarParser.RootContext(self, self._ctx, self.state)
        self.enterRule(localctx, 0, self.RULE_root)
        self._la = 0 # Token type
        try:
            self.enterOuterAlt(localctx, 1)
            self.state = 75
            self._errHandler.sync(self)
            _la = self._input.LA(1)
            while (((_la) & ~0x3f) == 0 and ((1 << _la) & 4607182692637743678) != 0):
                self.state = 72
                self.headerElement()
                self.state = 77
                self._errHandler.sync(self)
                _la = self._input.LA(1)

            self.state = 78
            self.match(GrammarParser.EOF)
        except RecognitionException as re:
            localctx.exception = re
            self._errHandler.reportError(self, re)
            self._errHandler.recover(self, re)
        finally:
            self.exitRule()
        return localctx


    class HeaderElementContext(ParserRuleContext):
        __slots__ = 'parser'

        def __init__(self, parser, parent:ParserRuleContext=None, invokingState:int=-1):
            super().__init__(parent, invokingState)
            self.parser = parser

        def include(self):
            return self.getTypedRuleContext(GrammarParser.IncludeContext,0)


        def enum(self):
            return self.getTypedRuleContext(GrammarParser.EnumContext,0)


        def function(self):
            return self.getTypedRuleContext(GrammarParser.FunctionContext,0)


        def globalDeclaration(self):
            return self.getTypedRuleContext(GrammarParser.GlobalDeclarationContext,0)


        def structDecl(self):
            return self.getTypedRuleContext(GrammarParser.StructDeclContext,0)


        def typedefDecl(self):
            return self.getTypedRuleContext(GrammarParser.TypedefDeclContext,0)


        def expr(self):
            return self.getTypedRuleContext(GrammarParser.ExprContext,0)


        def getRuleIndex(self):
            return GrammarParser.RULE_headerElement

        def accept(self, visitor:ParseTreeVisitor):
            if hasattr( visitor, "visitHeaderElement" ):
                return visitor.visitHeaderElement(self)
            else:
                return visitor.visitChildren(self)




    def headerElement(self):

        localctx = GrammarParser.HeaderElementContext(self, self._ctx, self.state)
        self.enterRule(localctx, 2, self.RULE_headerElement)
        try:
            self.state = 90
            self._errHandler.sync(self)
            la_ = self._interp.adaptivePredict(self._input,1,self._ctx)
            if la_ == 1:
                self.enterOuterAlt(localctx, 1)
                self.state = 80
                self.include()
                pass

            elif la_ == 2:
                self.enterOuterAlt(localctx, 2)
                self.state = 81
                self.enum()
                pass

            elif la_ == 3:
                self.enterOuterAlt(localctx, 3)
                self.state = 82
                self.function()
                pass

            elif la_ == 4:
                self.enterOuterAlt(localctx, 4)
                self.state = 83
                self.globalDeclaration()
                pass

            elif la_ == 5:
                self.enterOuterAlt(localctx, 5)
                self.state = 84
                self.structDecl()
                pass

            elif la_ == 6:
                self.enterOuterAlt(localctx, 6)
                self.state = 85
                self.typedefDecl()
                pass

            elif la_ == 7:
                self.enterOuterAlt(localctx, 7)
                self.state = 86
                self.expr(0)
                self.state = 87
                self.match(GrammarParser.T__0)
                pass

            elif la_ == 8:
                self.enterOuterAlt(localctx, 8)
                self.state = 89
                self.match(GrammarParser.T__0)
                pass


        except RecognitionException as re:
            localctx.exception = re
            self._errHandler.reportError(self, re)
            self._errHandler.recover(self, re)
        finally:
            self.exitRule()
        return localctx


    class TypedefDeclContext(ParserRuleContext):
        __slots__ = 'parser'

        def __init__(self, parser, parent:ParserRuleContext=None, invokingState:int=-1):
            super().__init__(parent, invokingState)
            self.parser = parser

        def type_(self):
            return self.getTypedRuleContext(GrammarParser.TypeContext,0)


        def ID(self):
            return self.getToken(GrammarParser.ID, 0)

        def getRuleIndex(self):
            return GrammarParser.RULE_typedefDecl

        def accept(self, visitor:ParseTreeVisitor):
            if hasattr( visitor, "visitTypedefDecl" ):
                return visitor.visitTypedefDecl(self)
            else:
                return visitor.visitChildren(self)




    def typedefDecl(self):

        localctx = GrammarParser.TypedefDeclContext(self, self._ctx, self.state)
        self.enterRule(localctx, 4, self.RULE_typedefDecl)
        try:
            self.state = 98
            self._errHandler.sync(self)
            token = self._input.LA(1)
            if token in [2]:
                self.enterOuterAlt(localctx, 1)
                self.state = 92
                self.match(GrammarParser.T__1)
                self.state = 93
                self.type_()
                self.state = 94
                self.match(GrammarParser.ID)
                self.state = 95
                self.match(GrammarParser.T__0)
                pass
            elif token in [3]:
                self.enterOuterAlt(localctx, 2)
                self.state = 97
                self.match(GrammarParser.T__2)
                pass
            else:
                raise NoViableAltException(self)

        except RecognitionException as re:
            localctx.exception = re
            self._errHandler.reportError(self, re)
            self._errHandler.recover(self, re)
        finally:
            self.exitRule()
        return localctx


    class StructDeclContext(ParserRuleContext):
        __slots__ = 'parser'

        def __init__(self, parser, parent:ParserRuleContext=None, invokingState:int=-1):
            super().__init__(parent, invokingState)
            self.parser = parser

        def ID(self):
            return self.getToken(GrammarParser.ID, 0)

        def structFieldDecl(self, i:int=None):
            if i is None:
                return self.getTypedRuleContexts(GrammarParser.StructFieldDeclContext)
            else:
                return self.getTypedRuleContext(GrammarParser.StructFieldDeclContext,i)


        def getRuleIndex(self):
            return GrammarParser.RULE_structDecl

        def accept(self, visitor:ParseTreeVisitor):
            if hasattr( visitor, "visitStructDecl" ):
                return visitor.visitStructDecl(self)
            else:
                return visitor.visitChildren(self)




    def structDecl(self):

        localctx = GrammarParser.StructDeclContext(self, self._ctx, self.state)
        self.enterRule(localctx, 6, self.RULE_structDecl)
        self._la = 0 # Token type
        try:
            self.state = 116
            self._errHandler.sync(self)
            la_ = self._interp.adaptivePredict(self._input,5,self._ctx)
            if la_ == 1:
                self.enterOuterAlt(localctx, 1)
                self.state = 100
                _la = self._input.LA(1)
                if not(_la==4 or _la==5):
                    self._errHandler.recoverInline(self)
                else:
                    self._errHandler.reportMatch(self)
                    self.consume()
                self.state = 102
                self._errHandler.sync(self)
                _la = self._input.LA(1)
                if _la==61:
                    self.state = 101
                    self.match(GrammarParser.ID)


                self.state = 104
                self.match(GrammarParser.T__5)
                self.state = 108
                self._errHandler.sync(self)
                _la = self._input.LA(1)
                while (((_la) & ~0x3f) == 0 and ((1 << _la) & 2445454597662179888) != 0):
                    self.state = 105
                    self.structFieldDecl()
                    self.state = 110
                    self._errHandler.sync(self)
                    _la = self._input.LA(1)

                self.state = 111
                self.match(GrammarParser.T__6)
                self.state = 112
                self.match(GrammarParser.T__0)
                pass

            elif la_ == 2:
                self.enterOuterAlt(localctx, 2)
                self.state = 113
                _la = self._input.LA(1)
                if not(_la==4 or _la==5):
                    self._errHandler.recoverInline(self)
                else:
                    self._errHandler.reportMatch(self)
                    self.consume()
                self.state = 114
                self.match(GrammarParser.ID)
                self.state = 115
                self.match(GrammarParser.T__0)
                pass


        except RecognitionException as re:
            localctx.exception = re
            self._errHandler.reportError(self, re)
            self._errHandler.recover(self, re)
        finally:
            self.exitRule()
        return localctx


    class StructFieldDeclContext(ParserRuleContext):
        __slots__ = 'parser'

        def __init__(self, parser, parent:ParserRuleContext=None, invokingState:int=-1):
            super().__init__(parent, invokingState)
            self.parser = parser

        def type_(self):
            return self.getTypedRuleContext(GrammarParser.TypeContext,0)


        def structFieldList(self):
            return self.getTypedRuleContext(GrammarParser.StructFieldListContext,0)


        def getRuleIndex(self):
            return GrammarParser.RULE_structFieldDecl

        def accept(self, visitor:ParseTreeVisitor):
            if hasattr( visitor, "visitStructFieldDecl" ):
                return visitor.visitStructFieldDecl(self)
            else:
                return visitor.visitChildren(self)




    def structFieldDecl(self):

        localctx = GrammarParser.StructFieldDeclContext(self, self._ctx, self.state)
        self.enterRule(localctx, 8, self.RULE_structFieldDecl)
        try:
            self.enterOuterAlt(localctx, 1)
            self.state = 118
            self.type_()
            self.state = 119
            self.structFieldList()
            self.state = 120
            self.match(GrammarParser.T__0)
        except RecognitionException as re:
            localctx.exception = re
            self._errHandler.reportError(self, re)
            self._errHandler.recover(self, re)
        finally:
            self.exitRule()
        return localctx


    class StructFieldListContext(ParserRuleContext):
        __slots__ = 'parser'

        def __init__(self, parser, parent:ParserRuleContext=None, invokingState:int=-1):
            super().__init__(parent, invokingState)
            self.parser = parser

        def structField(self, i:int=None):
            if i is None:
                return self.getTypedRuleContexts(GrammarParser.StructFieldContext)
            else:
                return self.getTypedRuleContext(GrammarParser.StructFieldContext,i)


        def getRuleIndex(self):
            return GrammarParser.RULE_structFieldList

        def accept(self, visitor:ParseTreeVisitor):
            if hasattr( visitor, "visitStructFieldList" ):
                return visitor.visitStructFieldList(self)
            else:
                return visitor.visitChildren(self)




    def structFieldList(self):

        localctx = GrammarParser.StructFieldListContext(self, self._ctx, self.state)
        self.enterRule(localctx, 10, self.RULE_structFieldList)
        self._la = 0 # Token type
        try:
            self.enterOuterAlt(localctx, 1)
            self.state = 122
            self.structField()
            self.state = 127
            self._errHandler.sync(self)
            _la = self._input.LA(1)
            while _la==8:
                self.state = 123
                self.match(GrammarParser.T__7)
                self.state = 124
                self.structField()
                self.state = 129
                self._errHandler.sync(self)
                _la = self._input.LA(1)

        except RecognitionException as re:
            localctx.exception = re
            self._errHandler.reportError(self, re)
            self._errHandler.recover(self, re)
        finally:
            self.exitRule()
        return localctx


    class StructFieldContext(ParserRuleContext):
        __slots__ = 'parser'

        def __init__(self, parser, parent:ParserRuleContext=None, invokingState:int=-1):
            super().__init__(parent, invokingState)
            self.parser = parser

        def ID(self):
            return self.getToken(GrammarParser.ID, 0)

        def array_sizes(self):
            return self.getTypedRuleContext(GrammarParser.Array_sizesContext,0)


        def getRuleIndex(self):
            return GrammarParser.RULE_structField

        def accept(self, visitor:ParseTreeVisitor):
            if hasattr( visitor, "visitStructField" ):
                return visitor.visitStructField(self)
            else:
                return visitor.visitChildren(self)




    def structField(self):

        localctx = GrammarParser.StructFieldContext(self, self._ctx, self.state)
        self.enterRule(localctx, 12, self.RULE_structField)
        self._la = 0 # Token type
        try:
            self.enterOuterAlt(localctx, 1)
            self.state = 130
            self.match(GrammarParser.ID)
            self.state = 132
            self._errHandler.sync(self)
            _la = self._input.LA(1)
            if _la==26:
                self.state = 131
                self.array_sizes()


        except RecognitionException as re:
            localctx.exception = re
            self._errHandler.reportError(self, re)
            self._errHandler.recover(self, re)
        finally:
            self.exitRule()
        return localctx


    class EnumContext(ParserRuleContext):
        __slots__ = 'parser'

        def __init__(self, parser, parent:ParserRuleContext=None, invokingState:int=-1):
            super().__init__(parent, invokingState)
            self.parser = parser

        def labels(self):
            return self.getTypedRuleContext(GrammarParser.LabelsContext,0)


        def ID(self):
            return self.getToken(GrammarParser.ID, 0)

        def getRuleIndex(self):
            return GrammarParser.RULE_enum

        def accept(self, visitor:ParseTreeVisitor):
            if hasattr( visitor, "visitEnum" ):
                return visitor.visitEnum(self)
            else:
                return visitor.visitChildren(self)




    def enum(self):

        localctx = GrammarParser.EnumContext(self, self._ctx, self.state)
        self.enterRule(localctx, 14, self.RULE_enum)
        self._la = 0 # Token type
        try:
            self.enterOuterAlt(localctx, 1)
            self.state = 134
            self.match(GrammarParser.T__8)
            self.state = 136
            self._errHandler.sync(self)
            _la = self._input.LA(1)
            if _la==61:
                self.state = 135
                self.match(GrammarParser.ID)


            self.state = 138
            self.match(GrammarParser.T__5)
            self.state = 139
            self.labels()
            self.state = 140
            self.match(GrammarParser.T__6)
            self.state = 141
            self.match(GrammarParser.T__0)
        except RecognitionException as re:
            localctx.exception = re
            self._errHandler.reportError(self, re)
            self._errHandler.recover(self, re)
        finally:
            self.exitRule()
        return localctx


    class GlobalDeclarationContext(ParserRuleContext):
        __slots__ = 'parser'

        def __init__(self, parser, parent:ParserRuleContext=None, invokingState:int=-1):
            super().__init__(parent, invokingState)
            self.parser = parser

        def declaration(self):
            return self.getTypedRuleContext(GrammarParser.DeclarationContext,0)


        def getRuleIndex(self):
            return GrammarParser.RULE_globalDeclaration

        def accept(self, visitor:ParseTreeVisitor):
            if hasattr( visitor, "visitGlobalDeclaration" ):
                return visitor.visitGlobalDeclaration(self)
            else:
                return visitor.visitChildren(self)




    def globalDeclaration(self):

        localctx = GrammarParser.GlobalDeclarationContext(self, self._ctx, self.state)
        self.enterRule(localctx, 16, self.RULE_globalDeclaration)
        try:
            self.enterOuterAlt(localctx, 1)
            self.state = 143
            self.declaration()
            self.state = 144
            self.match(GrammarParser.T__0)
        except RecognitionException as re:
            localctx.exception = re
            self._errHandler.reportError(self, re)
            self._errHandler.recover(self, re)
        finally:
            self.exitRule()
        return localctx


    class LabelsContext(ParserRuleContext):
        __slots__ = 'parser'

        def __init__(self, parser, parent:ParserRuleContext=None, invokingState:int=-1):
            super().__init__(parent, invokingState)
            self.parser = parser

        def label(self, i:int=None):
            if i is None:
                return self.getTypedRuleContexts(GrammarParser.LabelContext)
            else:
                return self.getTypedRuleContext(GrammarParser.LabelContext,i)


        def getRuleIndex(self):
            return GrammarParser.RULE_labels

        def accept(self, visitor:ParseTreeVisitor):
            if hasattr( visitor, "visitLabels" ):
                return visitor.visitLabels(self)
            else:
                return visitor.visitChildren(self)




    def labels(self):

        localctx = GrammarParser.LabelsContext(self, self._ctx, self.state)
        self.enterRule(localctx, 18, self.RULE_labels)
        self._la = 0 # Token type
        try:
            self.enterOuterAlt(localctx, 1)
            self.state = 146
            self.label()
            self.state = 151
            self._errHandler.sync(self)
            _alt = self._interp.adaptivePredict(self._input,9,self._ctx)
            while _alt!=2 and _alt!=ATN.INVALID_ALT_NUMBER:
                if _alt==1:
                    self.state = 147
                    self.match(GrammarParser.T__7)
                    self.state = 148
                    self.label() 
                self.state = 153
                self._errHandler.sync(self)
                _alt = self._interp.adaptivePredict(self._input,9,self._ctx)

            self.state = 155
            self._errHandler.sync(self)
            _la = self._input.LA(1)
            if _la==8:
                self.state = 154
                self.match(GrammarParser.T__7)


        except RecognitionException as re:
            localctx.exception = re
            self._errHandler.reportError(self, re)
            self._errHandler.recover(self, re)
        finally:
            self.exitRule()
        return localctx


    class LabelContext(ParserRuleContext):
        __slots__ = 'parser'

        def __init__(self, parser, parent:ParserRuleContext=None, invokingState:int=-1):
            super().__init__(parent, invokingState)
            self.parser = parser

        def ID(self):
            return self.getToken(GrammarParser.ID, 0)

        def expr(self):
            return self.getTypedRuleContext(GrammarParser.ExprContext,0)


        def getRuleIndex(self):
            return GrammarParser.RULE_label

        def accept(self, visitor:ParseTreeVisitor):
            if hasattr( visitor, "visitLabel" ):
                return visitor.visitLabel(self)
            else:
                return visitor.visitChildren(self)




    def label(self):

        localctx = GrammarParser.LabelContext(self, self._ctx, self.state)
        self.enterRule(localctx, 20, self.RULE_label)
        self._la = 0 # Token type
        try:
            self.enterOuterAlt(localctx, 1)
            self.state = 157
            self.match(GrammarParser.ID)
            self.state = 160
            self._errHandler.sync(self)
            _la = self._input.LA(1)
            if _la==10:
                self.state = 158
                self.match(GrammarParser.T__9)
                self.state = 159
                self.expr(0)


        except RecognitionException as re:
            localctx.exception = re
            self._errHandler.reportError(self, re)
            self._errHandler.recover(self, re)
        finally:
            self.exitRule()
        return localctx


    class IncludeContext(ParserRuleContext):
        __slots__ = 'parser'

        def __init__(self, parser, parent:ParserRuleContext=None, invokingState:int=-1):
            super().__init__(parent, invokingState)
            self.parser = parser

        def header(self):
            return self.getTypedRuleContext(GrammarParser.HeaderContext,0)


        def getRuleIndex(self):
            return GrammarParser.RULE_include

        def accept(self, visitor:ParseTreeVisitor):
            if hasattr( visitor, "visitInclude" ):
                return visitor.visitInclude(self)
            else:
                return visitor.visitChildren(self)




    def include(self):

        localctx = GrammarParser.IncludeContext(self, self._ctx, self.state)
        self.enterRule(localctx, 22, self.RULE_include)
        try:
            self.enterOuterAlt(localctx, 1)
            self.state = 162
            self.match(GrammarParser.T__10)
            self.state = 163
            self.header()
        except RecognitionException as re:
            localctx.exception = re
            self._errHandler.reportError(self, re)
            self._errHandler.recover(self, re)
        finally:
            self.exitRule()
        return localctx


    class HeaderContext(ParserRuleContext):
        __slots__ = 'parser'

        def __init__(self, parser, parent:ParserRuleContext=None, invokingState:int=-1):
            super().__init__(parent, invokingState)
            self.parser = parser

        def HEADER_SYSTEM(self):
            return self.getToken(GrammarParser.HEADER_SYSTEM, 0)

        def HEADER_LOCAL(self):
            return self.getToken(GrammarParser.HEADER_LOCAL, 0)

        def getRuleIndex(self):
            return GrammarParser.RULE_header

        def accept(self, visitor:ParseTreeVisitor):
            if hasattr( visitor, "visitHeader" ):
                return visitor.visitHeader(self)
            else:
                return visitor.visitChildren(self)




    def header(self):

        localctx = GrammarParser.HeaderContext(self, self._ctx, self.state)
        self.enterRule(localctx, 24, self.RULE_header)
        self._la = 0 # Token type
        try:
            self.enterOuterAlt(localctx, 1)
            self.state = 165
            _la = self._input.LA(1)
            if not(_la==63 or _la==64):
                self._errHandler.recoverInline(self)
            else:
                self._errHandler.reportMatch(self)
                self.consume()
        except RecognitionException as re:
            localctx.exception = re
            self._errHandler.reportError(self, re)
            self._errHandler.recover(self, re)
        finally:
            self.exitRule()
        return localctx


    class FunctionContext(ParserRuleContext):
        __slots__ = 'parser'

        def __init__(self, parser, parent:ParserRuleContext=None, invokingState:int=-1):
            super().__init__(parent, invokingState)
            self.parser = parser

        def type_(self):
            return self.getTypedRuleContext(GrammarParser.TypeContext,0)


        def ID(self):
            return self.getToken(GrammarParser.ID, 0)

        def parameters(self):
            return self.getTypedRuleContext(GrammarParser.ParametersContext,0)


        def statement(self, i:int=None):
            if i is None:
                return self.getTypedRuleContexts(GrammarParser.StatementContext)
            else:
                return self.getTypedRuleContext(GrammarParser.StatementContext,i)


        def getRuleIndex(self):
            return GrammarParser.RULE_function

        def accept(self, visitor:ParseTreeVisitor):
            if hasattr( visitor, "visitFunction" ):
                return visitor.visitFunction(self)
            else:
                return visitor.visitChildren(self)




    def function(self):

        localctx = GrammarParser.FunctionContext(self, self._ctx, self.state)
        self.enterRule(localctx, 26, self.RULE_function)
        self._la = 0 # Token type
        try:
            self.state = 202
            self._errHandler.sync(self)
            la_ = self._interp.adaptivePredict(self._input,18,self._ctx)
            if la_ == 1:
                self.enterOuterAlt(localctx, 1)
                self.state = 167
                self.type_()
                self.state = 168
                self.match(GrammarParser.ID)
                self.state = 169
                self.match(GrammarParser.T__11)
                self.state = 171
                self._errHandler.sync(self)
                _la = self._input.LA(1)
                if (((_la) & ~0x3f) == 0 and ((1 << _la) & 2445454597662179888) != 0):
                    self.state = 170
                    self.parameters()


                self.state = 173
                self.match(GrammarParser.T__12)
                self.state = 183
                self._errHandler.sync(self)
                token = self._input.LA(1)
                if token in [1]:
                    self.state = 174
                    self.match(GrammarParser.T__0)
                    pass
                elif token in [6]:
                    self.state = 175
                    self.match(GrammarParser.T__5)
                    self.state = 179
                    self._errHandler.sync(self)
                    _la = self._input.LA(1)
                    while (((_la) & ~0x3f) == 0 and ((1 << _la) & 4607182692641657470) != 0):
                        self.state = 176
                        self.statement()
                        self.state = 181
                        self._errHandler.sync(self)
                        _la = self._input.LA(1)

                    self.state = 182
                    self.match(GrammarParser.T__6)
                    pass
                else:
                    raise NoViableAltException(self)

                pass

            elif la_ == 2:
                self.enterOuterAlt(localctx, 2)
                self.state = 185
                self.match(GrammarParser.ID)
                self.state = 186
                self.match(GrammarParser.T__11)
                self.state = 188
                self._errHandler.sync(self)
                _la = self._input.LA(1)
                if (((_la) & ~0x3f) == 0 and ((1 << _la) & 2445454597662179888) != 0):
                    self.state = 187
                    self.parameters()


                self.state = 190
                self.match(GrammarParser.T__12)
                self.state = 200
                self._errHandler.sync(self)
                token = self._input.LA(1)
                if token in [1]:
                    self.state = 191
                    self.match(GrammarParser.T__0)
                    pass
                elif token in [6]:
                    self.state = 192
                    self.match(GrammarParser.T__5)
                    self.state = 196
                    self._errHandler.sync(self)
                    _la = self._input.LA(1)
                    while (((_la) & ~0x3f) == 0 and ((1 << _la) & 4607182692641657470) != 0):
                        self.state = 193
                        self.statement()
                        self.state = 198
                        self._errHandler.sync(self)
                        _la = self._input.LA(1)

                    self.state = 199
                    self.match(GrammarParser.T__6)
                    pass
                else:
                    raise NoViableAltException(self)

                pass


        except RecognitionException as re:
            localctx.exception = re
            self._errHandler.reportError(self, re)
            self._errHandler.recover(self, re)
        finally:
            self.exitRule()
        return localctx


    class ParametersContext(ParserRuleContext):
        __slots__ = 'parser'

        def __init__(self, parser, parent:ParserRuleContext=None, invokingState:int=-1):
            super().__init__(parent, invokingState)
            self.parser = parser

        def parameter(self, i:int=None):
            if i is None:
                return self.getTypedRuleContexts(GrammarParser.ParameterContext)
            else:
                return self.getTypedRuleContext(GrammarParser.ParameterContext,i)


        def getRuleIndex(self):
            return GrammarParser.RULE_parameters

        def accept(self, visitor:ParseTreeVisitor):
            if hasattr( visitor, "visitParameters" ):
                return visitor.visitParameters(self)
            else:
                return visitor.visitChildren(self)




    def parameters(self):

        localctx = GrammarParser.ParametersContext(self, self._ctx, self.state)
        self.enterRule(localctx, 28, self.RULE_parameters)
        self._la = 0 # Token type
        try:
            self.enterOuterAlt(localctx, 1)
            self.state = 204
            self.parameter()
            self.state = 209
            self._errHandler.sync(self)
            _la = self._input.LA(1)
            while _la==8:
                self.state = 205
                self.match(GrammarParser.T__7)
                self.state = 206
                self.parameter()
                self.state = 211
                self._errHandler.sync(self)
                _la = self._input.LA(1)

        except RecognitionException as re:
            localctx.exception = re
            self._errHandler.reportError(self, re)
            self._errHandler.recover(self, re)
        finally:
            self.exitRule()
        return localctx


    class ParameterContext(ParserRuleContext):
        __slots__ = 'parser'

        def __init__(self, parser, parent:ParserRuleContext=None, invokingState:int=-1):
            super().__init__(parent, invokingState)
            self.parser = parser

        def type_(self):
            return self.getTypedRuleContext(GrammarParser.TypeContext,0)


        def ID(self):
            return self.getToken(GrammarParser.ID, 0)

        def array_sizes(self):
            return self.getTypedRuleContext(GrammarParser.Array_sizesContext,0)


        def getRuleIndex(self):
            return GrammarParser.RULE_parameter

        def accept(self, visitor:ParseTreeVisitor):
            if hasattr( visitor, "visitParameter" ):
                return visitor.visitParameter(self)
            else:
                return visitor.visitChildren(self)




    def parameter(self):

        localctx = GrammarParser.ParameterContext(self, self._ctx, self.state)
        self.enterRule(localctx, 30, self.RULE_parameter)
        self._la = 0 # Token type
        try:
            self.enterOuterAlt(localctx, 1)
            self.state = 212
            self.type_()
            self.state = 213
            self.match(GrammarParser.ID)
            self.state = 215
            self._errHandler.sync(self)
            _la = self._input.LA(1)
            if _la==26:
                self.state = 214
                self.array_sizes()


        except RecognitionException as re:
            localctx.exception = re
            self._errHandler.reportError(self, re)
            self._errHandler.recover(self, re)
        finally:
            self.exitRule()
        return localctx


    class StatementContext(ParserRuleContext):
        __slots__ = 'parser'

        def __init__(self, parser, parent:ParserRuleContext=None, invokingState:int=-1):
            super().__init__(parent, invokingState)
            self.parser = parser


        def getRuleIndex(self):
            return GrammarParser.RULE_statement

     
        def copyFrom(self, ctx:ParserRuleContext):
            super().copyFrom(ctx)



    class ForStmtContext(StatementContext):

        def __init__(self, parser, ctx:ParserRuleContext): # actually a GrammarParser.StatementContext
            super().__init__(parser)
            self.copyFrom(ctx)

        def forLoop(self):
            return self.getTypedRuleContext(GrammarParser.ForLoopContext,0)


        def accept(self, visitor:ParseTreeVisitor):
            if hasattr( visitor, "visitForStmt" ):
                return visitor.visitForStmt(self)
            else:
                return visitor.visitChildren(self)


    class WhileStmtContext(StatementContext):

        def __init__(self, parser, ctx:ParserRuleContext): # actually a GrammarParser.StatementContext
            super().__init__(parser)
            self.copyFrom(ctx)

        def whileLoop(self):
            return self.getTypedRuleContext(GrammarParser.WhileLoopContext,0)


        def accept(self, visitor:ParseTreeVisitor):
            if hasattr( visitor, "visitWhileStmt" ):
                return visitor.visitWhileStmt(self)
            else:
                return visitor.visitChildren(self)


    class StructStmtContext(StatementContext):

        def __init__(self, parser, ctx:ParserRuleContext): # actually a GrammarParser.StatementContext
            super().__init__(parser)
            self.copyFrom(ctx)

        def structDecl(self):
            return self.getTypedRuleContext(GrammarParser.StructDeclContext,0)


        def accept(self, visitor:ParseTreeVisitor):
            if hasattr( visitor, "visitStructStmt" ):
                return visitor.visitStructStmt(self)
            else:
                return visitor.visitChildren(self)


    class FunctionStmtContext(StatementContext):

        def __init__(self, parser, ctx:ParserRuleContext): # actually a GrammarParser.StatementContext
            super().__init__(parser)
            self.copyFrom(ctx)

        def function(self):
            return self.getTypedRuleContext(GrammarParser.FunctionContext,0)


        def accept(self, visitor:ParseTreeVisitor):
            if hasattr( visitor, "visitFunctionStmt" ):
                return visitor.visitFunctionStmt(self)
            else:
                return visitor.visitChildren(self)


    class EmptyStmtContext(StatementContext):

        def __init__(self, parser, ctx:ParserRuleContext): # actually a GrammarParser.StatementContext
            super().__init__(parser)
            self.copyFrom(ctx)


        def accept(self, visitor:ParseTreeVisitor):
            if hasattr( visitor, "visitEmptyStmt" ):
                return visitor.visitEmptyStmt(self)
            else:
                return visitor.visitChildren(self)


    class ReturnStmtContext(StatementContext):

        def __init__(self, parser, ctx:ParserRuleContext): # actually a GrammarParser.StatementContext
            super().__init__(parser)
            self.copyFrom(ctx)

        def expr(self):
            return self.getTypedRuleContext(GrammarParser.ExprContext,0)


        def accept(self, visitor:ParseTreeVisitor):
            if hasattr( visitor, "visitReturnStmt" ):
                return visitor.visitReturnStmt(self)
            else:
                return visitor.visitChildren(self)


    class SwitchStmtContext(StatementContext):

        def __init__(self, parser, ctx:ParserRuleContext): # actually a GrammarParser.StatementContext
            super().__init__(parser)
            self.copyFrom(ctx)

        def switchStatement(self):
            return self.getTypedRuleContext(GrammarParser.SwitchStatementContext,0)


        def accept(self, visitor:ParseTreeVisitor):
            if hasattr( visitor, "visitSwitchStmt" ):
                return visitor.visitSwitchStmt(self)
            else:
                return visitor.visitChildren(self)


    class SimpleStmtContext(StatementContext):

        def __init__(self, parser, ctx:ParserRuleContext): # actually a GrammarParser.StatementContext
            super().__init__(parser)
            self.copyFrom(ctx)

        def declaration(self):
            return self.getTypedRuleContext(GrammarParser.DeclarationContext,0)

        def expr(self):
            return self.getTypedRuleContext(GrammarParser.ExprContext,0)


        def accept(self, visitor:ParseTreeVisitor):
            if hasattr( visitor, "visitSimpleStmt" ):
                return visitor.visitSimpleStmt(self)
            else:
                return visitor.visitChildren(self)


    class AnonymousScopeContext(StatementContext):

        def __init__(self, parser, ctx:ParserRuleContext): # actually a GrammarParser.StatementContext
            super().__init__(parser)
            self.copyFrom(ctx)

        def statement(self, i:int=None):
            if i is None:
                return self.getTypedRuleContexts(GrammarParser.StatementContext)
            else:
                return self.getTypedRuleContext(GrammarParser.StatementContext,i)


        def accept(self, visitor:ParseTreeVisitor):
            if hasattr( visitor, "visitAnonymousScope" ):
                return visitor.visitAnonymousScope(self)
            else:
                return visitor.visitChildren(self)


    class IfStmtContext(StatementContext):

        def __init__(self, parser, ctx:ParserRuleContext): # actually a GrammarParser.StatementContext
            super().__init__(parser)
            self.copyFrom(ctx)

        def ifStatement(self):
            return self.getTypedRuleContext(GrammarParser.IfStatementContext,0)


        def accept(self, visitor:ParseTreeVisitor):
            if hasattr( visitor, "visitIfStmt" ):
                return visitor.visitIfStmt(self)
            else:
                return visitor.visitChildren(self)


    class BreakStmtContext(StatementContext):

        def __init__(self, parser, ctx:ParserRuleContext): # actually a GrammarParser.StatementContext
            super().__init__(parser)
            self.copyFrom(ctx)


        def accept(self, visitor:ParseTreeVisitor):
            if hasattr( visitor, "visitBreakStmt" ):
                return visitor.visitBreakStmt(self)
            else:
                return visitor.visitChildren(self)


    class EnumStmtContext(StatementContext):

        def __init__(self, parser, ctx:ParserRuleContext): # actually a GrammarParser.StatementContext
            super().__init__(parser)
            self.copyFrom(ctx)

        def enum(self):
            return self.getTypedRuleContext(GrammarParser.EnumContext,0)


        def accept(self, visitor:ParseTreeVisitor):
            if hasattr( visitor, "visitEnumStmt" ):
                return visitor.visitEnumStmt(self)
            else:
                return visitor.visitChildren(self)


    class ContinueStmtContext(StatementContext):

        def __init__(self, parser, ctx:ParserRuleContext): # actually a GrammarParser.StatementContext
            super().__init__(parser)
            self.copyFrom(ctx)


        def accept(self, visitor:ParseTreeVisitor):
            if hasattr( visitor, "visitContinueStmt" ):
                return visitor.visitContinueStmt(self)
            else:
                return visitor.visitChildren(self)


    class TypedefStmtContext(StatementContext):

        def __init__(self, parser, ctx:ParserRuleContext): # actually a GrammarParser.StatementContext
            super().__init__(parser)
            self.copyFrom(ctx)

        def typedefDecl(self):
            return self.getTypedRuleContext(GrammarParser.TypedefDeclContext,0)


        def accept(self, visitor:ParseTreeVisitor):
            if hasattr( visitor, "visitTypedefStmt" ):
                return visitor.visitTypedefStmt(self)
            else:
                return visitor.visitChildren(self)



    def statement(self):

        localctx = GrammarParser.StatementContext(self, self._ctx, self.state)
        self.enterRule(localctx, 32, self.RULE_statement)
        self._la = 0 # Token type
        try:
            self.state = 249
            self._errHandler.sync(self)
            la_ = self._interp.adaptivePredict(self._input,24,self._ctx)
            if la_ == 1:
                localctx = GrammarParser.SimpleStmtContext(self, localctx)
                self.enterOuterAlt(localctx, 1)
                self.state = 219
                self._errHandler.sync(self)
                la_ = self._interp.adaptivePredict(self._input,21,self._ctx)
                if la_ == 1:
                    self.state = 217
                    self.declaration()
                    pass

                elif la_ == 2:
                    self.state = 218
                    self.expr(0)
                    pass


                self.state = 221
                self.match(GrammarParser.T__0)
                pass

            elif la_ == 2:
                localctx = GrammarParser.FunctionStmtContext(self, localctx)
                self.enterOuterAlt(localctx, 2)
                self.state = 223
                self.function()
                pass

            elif la_ == 3:
                localctx = GrammarParser.EnumStmtContext(self, localctx)
                self.enterOuterAlt(localctx, 3)
                self.state = 224
                self.enum()
                pass

            elif la_ == 4:
                localctx = GrammarParser.StructStmtContext(self, localctx)
                self.enterOuterAlt(localctx, 4)
                self.state = 225
                self.structDecl()
                pass

            elif la_ == 5:
                localctx = GrammarParser.TypedefStmtContext(self, localctx)
                self.enterOuterAlt(localctx, 5)
                self.state = 226
                self.typedefDecl()
                pass

            elif la_ == 6:
                localctx = GrammarParser.AnonymousScopeContext(self, localctx)
                self.enterOuterAlt(localctx, 6)
                self.state = 227
                self.match(GrammarParser.T__5)
                self.state = 231
                self._errHandler.sync(self)
                _la = self._input.LA(1)
                while (((_la) & ~0x3f) == 0 and ((1 << _la) & 4607182692641657470) != 0):
                    self.state = 228
                    self.statement()
                    self.state = 233
                    self._errHandler.sync(self)
                    _la = self._input.LA(1)

                self.state = 234
                self.match(GrammarParser.T__6)
                pass

            elif la_ == 7:
                localctx = GrammarParser.IfStmtContext(self, localctx)
                self.enterOuterAlt(localctx, 7)
                self.state = 235
                self.ifStatement()
                pass

            elif la_ == 8:
                localctx = GrammarParser.WhileStmtContext(self, localctx)
                self.enterOuterAlt(localctx, 8)
                self.state = 236
                self.whileLoop()
                pass

            elif la_ == 9:
                localctx = GrammarParser.ForStmtContext(self, localctx)
                self.enterOuterAlt(localctx, 9)
                self.state = 237
                self.forLoop()
                pass

            elif la_ == 10:
                localctx = GrammarParser.SwitchStmtContext(self, localctx)
                self.enterOuterAlt(localctx, 10)
                self.state = 238
                self.switchStatement()
                pass

            elif la_ == 11:
                localctx = GrammarParser.BreakStmtContext(self, localctx)
                self.enterOuterAlt(localctx, 11)
                self.state = 239
                self.match(GrammarParser.T__13)
                self.state = 240
                self.match(GrammarParser.T__0)
                pass

            elif la_ == 12:
                localctx = GrammarParser.ContinueStmtContext(self, localctx)
                self.enterOuterAlt(localctx, 12)
                self.state = 241
                self.match(GrammarParser.T__14)
                self.state = 242
                self.match(GrammarParser.T__0)
                pass

            elif la_ == 13:
                localctx = GrammarParser.ReturnStmtContext(self, localctx)
                self.enterOuterAlt(localctx, 13)
                self.state = 243
                self.match(GrammarParser.T__15)
                self.state = 245
                self._errHandler.sync(self)
                _la = self._input.LA(1)
                if (((_la) & ~0x3f) == 0 and ((1 << _la) & 4467571104189255680) != 0):
                    self.state = 244
                    self.expr(0)


                self.state = 247
                self.match(GrammarParser.T__0)
                pass

            elif la_ == 14:
                localctx = GrammarParser.EmptyStmtContext(self, localctx)
                self.enterOuterAlt(localctx, 14)
                self.state = 248
                self.match(GrammarParser.T__0)
                pass


        except RecognitionException as re:
            localctx.exception = re
            self._errHandler.reportError(self, re)
            self._errHandler.recover(self, re)
        finally:
            self.exitRule()
        return localctx


    class IfStatementContext(ParserRuleContext):
        __slots__ = 'parser'

        def __init__(self, parser, parent:ParserRuleContext=None, invokingState:int=-1):
            super().__init__(parent, invokingState)
            self.parser = parser

        def expr(self):
            return self.getTypedRuleContext(GrammarParser.ExprContext,0)


        def statement(self, i:int=None):
            if i is None:
                return self.getTypedRuleContexts(GrammarParser.StatementContext)
            else:
                return self.getTypedRuleContext(GrammarParser.StatementContext,i)


        def getRuleIndex(self):
            return GrammarParser.RULE_ifStatement

        def accept(self, visitor:ParseTreeVisitor):
            if hasattr( visitor, "visitIfStatement" ):
                return visitor.visitIfStatement(self)
            else:
                return visitor.visitChildren(self)




    def ifStatement(self):

        localctx = GrammarParser.IfStatementContext(self, self._ctx, self.state)
        self.enterRule(localctx, 34, self.RULE_ifStatement)
        try:
            self.enterOuterAlt(localctx, 1)
            self.state = 251
            self.match(GrammarParser.T__16)
            self.state = 252
            self.match(GrammarParser.T__11)
            self.state = 253
            self.expr(0)
            self.state = 254
            self.match(GrammarParser.T__12)
            self.state = 255
            self.statement()
            self.state = 258
            self._errHandler.sync(self)
            la_ = self._interp.adaptivePredict(self._input,25,self._ctx)
            if la_ == 1:
                self.state = 256
                self.match(GrammarParser.T__17)
                self.state = 257
                self.statement()


        except RecognitionException as re:
            localctx.exception = re
            self._errHandler.reportError(self, re)
            self._errHandler.recover(self, re)
        finally:
            self.exitRule()
        return localctx


    class WhileLoopContext(ParserRuleContext):
        __slots__ = 'parser'

        def __init__(self, parser, parent:ParserRuleContext=None, invokingState:int=-1):
            super().__init__(parent, invokingState)
            self.parser = parser

        def expr(self):
            return self.getTypedRuleContext(GrammarParser.ExprContext,0)


        def statement(self):
            return self.getTypedRuleContext(GrammarParser.StatementContext,0)


        def getRuleIndex(self):
            return GrammarParser.RULE_whileLoop

        def accept(self, visitor:ParseTreeVisitor):
            if hasattr( visitor, "visitWhileLoop" ):
                return visitor.visitWhileLoop(self)
            else:
                return visitor.visitChildren(self)




    def whileLoop(self):

        localctx = GrammarParser.WhileLoopContext(self, self._ctx, self.state)
        self.enterRule(localctx, 36, self.RULE_whileLoop)
        try:
            self.enterOuterAlt(localctx, 1)
            self.state = 260
            self.match(GrammarParser.T__18)
            self.state = 261
            self.match(GrammarParser.T__11)
            self.state = 262
            self.expr(0)
            self.state = 263
            self.match(GrammarParser.T__12)
            self.state = 264
            self.statement()
        except RecognitionException as re:
            localctx.exception = re
            self._errHandler.reportError(self, re)
            self._errHandler.recover(self, re)
        finally:
            self.exitRule()
        return localctx


    class ForInitContext(ParserRuleContext):
        __slots__ = 'parser'

        def __init__(self, parser, parent:ParserRuleContext=None, invokingState:int=-1):
            super().__init__(parent, invokingState)
            self.parser = parser

        def declaration(self):
            return self.getTypedRuleContext(GrammarParser.DeclarationContext,0)


        def expr(self):
            return self.getTypedRuleContext(GrammarParser.ExprContext,0)


        def getRuleIndex(self):
            return GrammarParser.RULE_forInit

        def accept(self, visitor:ParseTreeVisitor):
            if hasattr( visitor, "visitForInit" ):
                return visitor.visitForInit(self)
            else:
                return visitor.visitChildren(self)




    def forInit(self):

        localctx = GrammarParser.ForInitContext(self, self._ctx, self.state)
        self.enterRule(localctx, 38, self.RULE_forInit)
        try:
            self.state = 268
            self._errHandler.sync(self)
            la_ = self._interp.adaptivePredict(self._input,26,self._ctx)
            if la_ == 1:
                self.enterOuterAlt(localctx, 1)
                self.state = 266
                self.declaration()
                pass

            elif la_ == 2:
                self.enterOuterAlt(localctx, 2)
                self.state = 267
                self.expr(0)
                pass


        except RecognitionException as re:
            localctx.exception = re
            self._errHandler.reportError(self, re)
            self._errHandler.recover(self, re)
        finally:
            self.exitRule()
        return localctx


    class ForLoopContext(ParserRuleContext):
        __slots__ = 'parser'

        def __init__(self, parser, parent:ParserRuleContext=None, invokingState:int=-1):
            super().__init__(parent, invokingState)
            self.parser = parser
            self.init = None # ForInitContext
            self.cond = None # ExprContext
            self.update = None # ExprContext
            self.body = None # StatementContext

        def statement(self):
            return self.getTypedRuleContext(GrammarParser.StatementContext,0)


        def forInit(self):
            return self.getTypedRuleContext(GrammarParser.ForInitContext,0)


        def expr(self, i:int=None):
            if i is None:
                return self.getTypedRuleContexts(GrammarParser.ExprContext)
            else:
                return self.getTypedRuleContext(GrammarParser.ExprContext,i)


        def getRuleIndex(self):
            return GrammarParser.RULE_forLoop

        def accept(self, visitor:ParseTreeVisitor):
            if hasattr( visitor, "visitForLoop" ):
                return visitor.visitForLoop(self)
            else:
                return visitor.visitChildren(self)




    def forLoop(self):

        localctx = GrammarParser.ForLoopContext(self, self._ctx, self.state)
        self.enterRule(localctx, 40, self.RULE_forLoop)
        self._la = 0 # Token type
        try:
            self.enterOuterAlt(localctx, 1)
            self.state = 270
            self.match(GrammarParser.T__19)
            self.state = 271
            self.match(GrammarParser.T__11)
            self.state = 273
            self._errHandler.sync(self)
            _la = self._input.LA(1)
            if (((_la) & ~0x3f) == 0 and ((1 << _la) & 4607182692637741616) != 0):
                self.state = 272
                localctx.init = self.forInit()


            self.state = 275
            self.match(GrammarParser.T__0)
            self.state = 277
            self._errHandler.sync(self)
            _la = self._input.LA(1)
            if (((_la) & ~0x3f) == 0 and ((1 << _la) & 4467571104189255680) != 0):
                self.state = 276
                localctx.cond = self.expr(0)


            self.state = 279
            self.match(GrammarParser.T__0)
            self.state = 281
            self._errHandler.sync(self)
            _la = self._input.LA(1)
            if (((_la) & ~0x3f) == 0 and ((1 << _la) & 4467571104189255680) != 0):
                self.state = 280
                localctx.update = self.expr(0)


            self.state = 283
            self.match(GrammarParser.T__12)
            self.state = 284
            localctx.body = self.statement()
        except RecognitionException as re:
            localctx.exception = re
            self._errHandler.reportError(self, re)
            self._errHandler.recover(self, re)
        finally:
            self.exitRule()
        return localctx


    class SwitchStatementContext(ParserRuleContext):
        __slots__ = 'parser'

        def __init__(self, parser, parent:ParserRuleContext=None, invokingState:int=-1):
            super().__init__(parent, invokingState)
            self.parser = parser

        def expr(self):
            return self.getTypedRuleContext(GrammarParser.ExprContext,0)


        def switchCase(self, i:int=None):
            if i is None:
                return self.getTypedRuleContexts(GrammarParser.SwitchCaseContext)
            else:
                return self.getTypedRuleContext(GrammarParser.SwitchCaseContext,i)


        def getRuleIndex(self):
            return GrammarParser.RULE_switchStatement

        def accept(self, visitor:ParseTreeVisitor):
            if hasattr( visitor, "visitSwitchStatement" ):
                return visitor.visitSwitchStatement(self)
            else:
                return visitor.visitChildren(self)




    def switchStatement(self):

        localctx = GrammarParser.SwitchStatementContext(self, self._ctx, self.state)
        self.enterRule(localctx, 42, self.RULE_switchStatement)
        self._la = 0 # Token type
        try:
            self.enterOuterAlt(localctx, 1)
            self.state = 286
            self.match(GrammarParser.T__20)
            self.state = 287
            self.match(GrammarParser.T__11)
            self.state = 288
            self.expr(0)
            self.state = 289
            self.match(GrammarParser.T__12)
            self.state = 290
            self.match(GrammarParser.T__5)
            self.state = 294
            self._errHandler.sync(self)
            _la = self._input.LA(1)
            while _la==22 or _la==24:
                self.state = 291
                self.switchCase()
                self.state = 296
                self._errHandler.sync(self)
                _la = self._input.LA(1)

            self.state = 297
            self.match(GrammarParser.T__6)
        except RecognitionException as re:
            localctx.exception = re
            self._errHandler.reportError(self, re)
            self._errHandler.recover(self, re)
        finally:
            self.exitRule()
        return localctx


    class SwitchCaseContext(ParserRuleContext):
        __slots__ = 'parser'

        def __init__(self, parser, parent:ParserRuleContext=None, invokingState:int=-1):
            super().__init__(parent, invokingState)
            self.parser = parser

        def expr(self):
            return self.getTypedRuleContext(GrammarParser.ExprContext,0)


        def statement(self, i:int=None):
            if i is None:
                return self.getTypedRuleContexts(GrammarParser.StatementContext)
            else:
                return self.getTypedRuleContext(GrammarParser.StatementContext,i)


        def getRuleIndex(self):
            return GrammarParser.RULE_switchCase

        def accept(self, visitor:ParseTreeVisitor):
            if hasattr( visitor, "visitSwitchCase" ):
                return visitor.visitSwitchCase(self)
            else:
                return visitor.visitChildren(self)




    def switchCase(self):

        localctx = GrammarParser.SwitchCaseContext(self, self._ctx, self.state)
        self.enterRule(localctx, 44, self.RULE_switchCase)
        self._la = 0 # Token type
        try:
            self.state = 316
            self._errHandler.sync(self)
            token = self._input.LA(1)
            if token in [22]:
                self.enterOuterAlt(localctx, 1)
                self.state = 299
                self.match(GrammarParser.T__21)
                self.state = 300
                self.expr(0)
                self.state = 301
                self.match(GrammarParser.T__22)
                self.state = 305
                self._errHandler.sync(self)
                _la = self._input.LA(1)
                while (((_la) & ~0x3f) == 0 and ((1 << _la) & 4607182692641657470) != 0):
                    self.state = 302
                    self.statement()
                    self.state = 307
                    self._errHandler.sync(self)
                    _la = self._input.LA(1)

                pass
            elif token in [24]:
                self.enterOuterAlt(localctx, 2)
                self.state = 308
                self.match(GrammarParser.T__23)
                self.state = 309
                self.match(GrammarParser.T__22)
                self.state = 313
                self._errHandler.sync(self)
                _la = self._input.LA(1)
                while (((_la) & ~0x3f) == 0 and ((1 << _la) & 4607182692641657470) != 0):
                    self.state = 310
                    self.statement()
                    self.state = 315
                    self._errHandler.sync(self)
                    _la = self._input.LA(1)

                pass
            else:
                raise NoViableAltException(self)

        except RecognitionException as re:
            localctx.exception = re
            self._errHandler.reportError(self, re)
            self._errHandler.recover(self, re)
        finally:
            self.exitRule()
        return localctx


    class ExprContext(ParserRuleContext):
        __slots__ = 'parser'

        def __init__(self, parser, parent:ParserRuleContext=None, invokingState:int=-1):
            super().__init__(parent, invokingState)
            self.parser = parser


        def getRuleIndex(self):
            return GrammarParser.RULE_expr

     
        def copyFrom(self, ctx:ParserRuleContext):
            super().copyFrom(ctx)


    class Sizeof_typeContext(ExprContext):

        def __init__(self, parser, ctx:ParserRuleContext): # actually a GrammarParser.ExprContext
            super().__init__(parser)
            self.copyFrom(ctx)

        def type_(self):
            return self.getTypedRuleContext(GrammarParser.TypeContext,0)


        def accept(self, visitor:ParseTreeVisitor):
            if hasattr( visitor, "visitSizeof_type" ):
                return visitor.visitSizeof_type(self)
            else:
                return visitor.visitChildren(self)


    class Function_callContext(ExprContext):

        def __init__(self, parser, ctx:ParserRuleContext): # actually a GrammarParser.ExprContext
            super().__init__(parser)
            self.copyFrom(ctx)

        def expr(self):
            return self.getTypedRuleContext(GrammarParser.ExprContext,0)

        def arg_list(self):
            return self.getTypedRuleContext(GrammarParser.Arg_listContext,0)


        def accept(self, visitor:ParseTreeVisitor):
            if hasattr( visitor, "visitFunction_call" ):
                return visitor.visitFunction_call(self)
            else:
                return visitor.visitChildren(self)


    class Array_accessContext(ExprContext):

        def __init__(self, parser, ctx:ParserRuleContext): # actually a GrammarParser.ExprContext
            super().__init__(parser)
            self.copyFrom(ctx)

        def expr(self, i:int=None):
            if i is None:
                return self.getTypedRuleContexts(GrammarParser.ExprContext)
            else:
                return self.getTypedRuleContext(GrammarParser.ExprContext,i)


        def accept(self, visitor:ParseTreeVisitor):
            if hasattr( visitor, "visitArray_access" ):
                return visitor.visitArray_access(self)
            else:
                return visitor.visitChildren(self)


    class AssignmentContext(ExprContext):

        def __init__(self, parser, ctx:ParserRuleContext): # actually a GrammarParser.ExprContext
            super().__init__(parser)
            self.copyFrom(ctx)

        def expr(self, i:int=None):
            if i is None:
                return self.getTypedRuleContexts(GrammarParser.ExprContext)
            else:
                return self.getTypedRuleContext(GrammarParser.ExprContext,i)


        def accept(self, visitor:ParseTreeVisitor):
            if hasattr( visitor, "visitAssignment" ):
                return visitor.visitAssignment(self)
            else:
                return visitor.visitChildren(self)


    class Sizeof_exprContext(ExprContext):

        def __init__(self, parser, ctx:ParserRuleContext): # actually a GrammarParser.ExprContext
            super().__init__(parser)
            self.copyFrom(ctx)

        def expr(self):
            return self.getTypedRuleContext(GrammarParser.ExprContext,0)


        def accept(self, visitor:ParseTreeVisitor):
            if hasattr( visitor, "visitSizeof_expr" ):
                return visitor.visitSizeof_expr(self)
            else:
                return visitor.visitChildren(self)


    class Member_access_ptrContext(ExprContext):

        def __init__(self, parser, ctx:ParserRuleContext): # actually a GrammarParser.ExprContext
            super().__init__(parser)
            self.copyFrom(ctx)

        def expr(self):
            return self.getTypedRuleContext(GrammarParser.ExprContext,0)

        def ID(self):
            return self.getToken(GrammarParser.ID, 0)

        def accept(self, visitor:ParseTreeVisitor):
            if hasattr( visitor, "visitMember_access_ptr" ):
                return visitor.visitMember_access_ptr(self)
            else:
                return visitor.visitChildren(self)


    class Unary_postfixContext(ExprContext):

        def __init__(self, parser, ctx:ParserRuleContext): # actually a GrammarParser.ExprContext
            super().__init__(parser)
            self.copyFrom(ctx)

        def expr(self):
            return self.getTypedRuleContext(GrammarParser.ExprContext,0)


        def accept(self, visitor:ParseTreeVisitor):
            if hasattr( visitor, "visitUnary_postfix" ):
                return visitor.visitUnary_postfix(self)
            else:
                return visitor.visitChildren(self)


    class Member_accessContext(ExprContext):

        def __init__(self, parser, ctx:ParserRuleContext): # actually a GrammarParser.ExprContext
            super().__init__(parser)
            self.copyFrom(ctx)

        def expr(self):
            return self.getTypedRuleContext(GrammarParser.ExprContext,0)

        def ID(self):
            return self.getToken(GrammarParser.ID, 0)

        def accept(self, visitor:ParseTreeVisitor):
            if hasattr( visitor, "visitMember_access" ):
                return visitor.visitMember_access(self)
            else:
                return visitor.visitChildren(self)


    class Primary_litContext(ExprContext):

        def __init__(self, parser, ctx:ParserRuleContext): # actually a GrammarParser.ExprContext
            super().__init__(parser)
            self.copyFrom(ctx)

        def literal(self):
            return self.getTypedRuleContext(GrammarParser.LiteralContext,0)


        def accept(self, visitor:ParseTreeVisitor):
            if hasattr( visitor, "visitPrimary_lit" ):
                return visitor.visitPrimary_lit(self)
            else:
                return visitor.visitChildren(self)


    class Primary_parenContext(ExprContext):

        def __init__(self, parser, ctx:ParserRuleContext): # actually a GrammarParser.ExprContext
            super().__init__(parser)
            self.copyFrom(ctx)

        def expr(self):
            return self.getTypedRuleContext(GrammarParser.ExprContext,0)


        def accept(self, visitor:ParseTreeVisitor):
            if hasattr( visitor, "visitPrimary_paren" ):
                return visitor.visitPrimary_paren(self)
            else:
                return visitor.visitChildren(self)


    class Unary_prefixContext(ExprContext):

        def __init__(self, parser, ctx:ParserRuleContext): # actually a GrammarParser.ExprContext
            super().__init__(parser)
            self.copyFrom(ctx)

        def expr(self):
            return self.getTypedRuleContext(GrammarParser.ExprContext,0)


        def accept(self, visitor:ParseTreeVisitor):
            if hasattr( visitor, "visitUnary_prefix" ):
                return visitor.visitUnary_prefix(self)
            else:
                return visitor.visitChildren(self)


    class BinaryContext(ExprContext):

        def __init__(self, parser, ctx:ParserRuleContext): # actually a GrammarParser.ExprContext
            super().__init__(parser)
            self.copyFrom(ctx)

        def expr(self, i:int=None):
            if i is None:
                return self.getTypedRuleContexts(GrammarParser.ExprContext)
            else:
                return self.getTypedRuleContext(GrammarParser.ExprContext,i)


        def accept(self, visitor:ParseTreeVisitor):
            if hasattr( visitor, "visitBinary" ):
                return visitor.visitBinary(self)
            else:
                return visitor.visitChildren(self)


    class Primary_idContext(ExprContext):

        def __init__(self, parser, ctx:ParserRuleContext): # actually a GrammarParser.ExprContext
            super().__init__(parser)
            self.copyFrom(ctx)

        def ID(self):
            return self.getToken(GrammarParser.ID, 0)

        def accept(self, visitor:ParseTreeVisitor):
            if hasattr( visitor, "visitPrimary_id" ):
                return visitor.visitPrimary_id(self)
            else:
                return visitor.visitChildren(self)


    class Unary_castContext(ExprContext):

        def __init__(self, parser, ctx:ParserRuleContext): # actually a GrammarParser.ExprContext
            super().__init__(parser)
            self.copyFrom(ctx)

        def expr(self):
            return self.getTypedRuleContext(GrammarParser.ExprContext,0)

        def type_(self):
            return self.getTypedRuleContext(GrammarParser.TypeContext,0)


        def accept(self, visitor:ParseTreeVisitor):
            if hasattr( visitor, "visitUnary_cast" ):
                return visitor.visitUnary_cast(self)
            else:
                return visitor.visitChildren(self)



    def expr(self, _p:int=0):
        _parentctx = self._ctx
        _parentState = self.state
        localctx = GrammarParser.ExprContext(self, self._ctx, _parentState)
        _prevctx = localctx
        _startState = 46
        self.enterRecursionRule(localctx, 46, self.RULE_expr, _p)
        self._la = 0 # Token type
        try:
            self.enterOuterAlt(localctx, 1)
            self.state = 343
            self._errHandler.sync(self)
            la_ = self._interp.adaptivePredict(self._input,34,self._ctx)
            if la_ == 1:
                localctx = GrammarParser.Primary_parenContext(self, localctx)
                self._ctx = localctx
                _prevctx = localctx

                self.state = 319
                self.match(GrammarParser.T__11)
                self.state = 320
                self.expr(0)
                self.state = 321
                self.match(GrammarParser.T__12)
                pass

            elif la_ == 2:
                localctx = GrammarParser.Primary_idContext(self, localctx)
                self._ctx = localctx
                _prevctx = localctx
                self.state = 323
                self.match(GrammarParser.ID)
                pass

            elif la_ == 3:
                localctx = GrammarParser.Primary_litContext(self, localctx)
                self._ctx = localctx
                _prevctx = localctx
                self.state = 324
                self.literal()
                pass

            elif la_ == 4:
                localctx = GrammarParser.Sizeof_exprContext(self, localctx)
                self._ctx = localctx
                _prevctx = localctx
                self.state = 325
                self.match(GrammarParser.T__24)
                self.state = 326
                self.match(GrammarParser.T__11)
                self.state = 327
                self.expr(0)
                self.state = 328
                self.match(GrammarParser.T__12)
                pass

            elif la_ == 5:
                localctx = GrammarParser.Sizeof_typeContext(self, localctx)
                self._ctx = localctx
                _prevctx = localctx
                self.state = 330
                self.match(GrammarParser.T__24)
                self.state = 331
                self.match(GrammarParser.T__11)
                self.state = 332
                self.type_()
                self.state = 333
                self.match(GrammarParser.T__12)
                pass

            elif la_ == 6:
                localctx = GrammarParser.Unary_castContext(self, localctx)
                self._ctx = localctx
                _prevctx = localctx

                self.state = 335
                self.match(GrammarParser.T__11)
                self.state = 336
                self.type_()
                self.state = 337
                self.match(GrammarParser.T__12)
                self.state = 339
                self.expr(13)
                pass

            elif la_ == 7:
                localctx = GrammarParser.Unary_prefixContext(self, localctx)
                self._ctx = localctx
                _prevctx = localctx
                self.state = 341
                _la = self._input.LA(1)
                if not((((_la) & ~0x3f) == 0 and ((1 << _la) & 273804165120) != 0)):
                    self._errHandler.recoverInline(self)
                else:
                    self._errHandler.reportMatch(self)
                    self.consume()
                self.state = 342
                self.expr(12)
                pass


            self._ctx.stop = self._input.LT(-1)
            self.state = 399
            self._errHandler.sync(self)
            _alt = self._interp.adaptivePredict(self._input,37,self._ctx)
            while _alt!=2 and _alt!=ATN.INVALID_ALT_NUMBER:
                if _alt==1:
                    if self._parseListeners is not None:
                        self.triggerExitRuleEvent()
                    _prevctx = localctx
                    self.state = 397
                    self._errHandler.sync(self)
                    la_ = self._interp.adaptivePredict(self._input,36,self._ctx)
                    if la_ == 1:
                        localctx = GrammarParser.BinaryContext(self, GrammarParser.ExprContext(self, _parentctx, _parentState))
                        self.pushNewRecursionContext(localctx, _startState, self.RULE_expr)
                        self.state = 345
                        if not self.precpred(self._ctx, 11):
                            from antlr4.error.Errors import FailedPredicateException
                            raise FailedPredicateException(self, "self.precpred(self._ctx, 11)")
                        self.state = 346
                        _la = self._input.LA(1)
                        if not((((_la) & ~0x3f) == 0 and ((1 << _la) & 893353197568) != 0)):
                            self._errHandler.recoverInline(self)
                        else:
                            self._errHandler.reportMatch(self)
                            self.consume()
                        self.state = 347
                        self.expr(12)
                        pass

                    elif la_ == 2:
                        localctx = GrammarParser.BinaryContext(self, GrammarParser.ExprContext(self, _parentctx, _parentState))
                        self.pushNewRecursionContext(localctx, _startState, self.RULE_expr)
                        self.state = 348
                        if not self.precpred(self._ctx, 10):
                            from antlr4.error.Errors import FailedPredicateException
                            raise FailedPredicateException(self, "self.precpred(self._ctx, 10)")
                        self.state = 349
                        _la = self._input.LA(1)
                        if not(_la==32 or _la==33):
                            self._errHandler.recoverInline(self)
                        else:
                            self._errHandler.reportMatch(self)
                            self.consume()
                        self.state = 350
                        self.expr(11)
                        pass

                    elif la_ == 3:
                        localctx = GrammarParser.BinaryContext(self, GrammarParser.ExprContext(self, _parentctx, _parentState))
                        self.pushNewRecursionContext(localctx, _startState, self.RULE_expr)
                        self.state = 351
                        if not self.precpred(self._ctx, 9):
                            from antlr4.error.Errors import FailedPredicateException
                            raise FailedPredicateException(self, "self.precpred(self._ctx, 9)")
                        self.state = 352
                        _la = self._input.LA(1)
                        if not(_la==40 or _la==41):
                            self._errHandler.recoverInline(self)
                        else:
                            self._errHandler.reportMatch(self)
                            self.consume()
                        self.state = 353
                        self.expr(10)
                        pass

                    elif la_ == 4:
                        localctx = GrammarParser.BinaryContext(self, GrammarParser.ExprContext(self, _parentctx, _parentState))
                        self.pushNewRecursionContext(localctx, _startState, self.RULE_expr)
                        self.state = 354
                        if not self.precpred(self._ctx, 8):
                            from antlr4.error.Errors import FailedPredicateException
                            raise FailedPredicateException(self, "self.precpred(self._ctx, 8)")
                        self.state = 355
                        _la = self._input.LA(1)
                        if not((((_la) & ~0x3f) == 0 and ((1 << _la) & 65970697666560) != 0)):
                            self._errHandler.recoverInline(self)
                        else:
                            self._errHandler.reportMatch(self)
                            self.consume()
                        self.state = 356
                        self.expr(9)
                        pass

                    elif la_ == 5:
                        localctx = GrammarParser.BinaryContext(self, GrammarParser.ExprContext(self, _parentctx, _parentState))
                        self.pushNewRecursionContext(localctx, _startState, self.RULE_expr)
                        self.state = 357
                        if not self.precpred(self._ctx, 7):
                            from antlr4.error.Errors import FailedPredicateException
                            raise FailedPredicateException(self, "self.precpred(self._ctx, 7)")
                        self.state = 358
                        _la = self._input.LA(1)
                        if not(_la==46 or _la==47):
                            self._errHandler.recoverInline(self)
                        else:
                            self._errHandler.reportMatch(self)
                            self.consume()
                        self.state = 359
                        self.expr(8)
                        pass

                    elif la_ == 6:
                        localctx = GrammarParser.BinaryContext(self, GrammarParser.ExprContext(self, _parentctx, _parentState))
                        self.pushNewRecursionContext(localctx, _startState, self.RULE_expr)
                        self.state = 360
                        if not self.precpred(self._ctx, 6):
                            from antlr4.error.Errors import FailedPredicateException
                            raise FailedPredicateException(self, "self.precpred(self._ctx, 6)")
                        self.state = 361
                        self.match(GrammarParser.T__36)
                        self.state = 362
                        self.expr(7)
                        pass

                    elif la_ == 7:
                        localctx = GrammarParser.BinaryContext(self, GrammarParser.ExprContext(self, _parentctx, _parentState))
                        self.pushNewRecursionContext(localctx, _startState, self.RULE_expr)
                        self.state = 363
                        if not self.precpred(self._ctx, 5):
                            from antlr4.error.Errors import FailedPredicateException
                            raise FailedPredicateException(self, "self.precpred(self._ctx, 5)")
                        self.state = 364
                        self.match(GrammarParser.T__47)
                        self.state = 365
                        self.expr(6)
                        pass

                    elif la_ == 8:
                        localctx = GrammarParser.BinaryContext(self, GrammarParser.ExprContext(self, _parentctx, _parentState))
                        self.pushNewRecursionContext(localctx, _startState, self.RULE_expr)
                        self.state = 366
                        if not self.precpred(self._ctx, 4):
                            from antlr4.error.Errors import FailedPredicateException
                            raise FailedPredicateException(self, "self.precpred(self._ctx, 4)")
                        self.state = 367
                        self.match(GrammarParser.T__48)
                        self.state = 368
                        self.expr(5)
                        pass

                    elif la_ == 9:
                        localctx = GrammarParser.BinaryContext(self, GrammarParser.ExprContext(self, _parentctx, _parentState))
                        self.pushNewRecursionContext(localctx, _startState, self.RULE_expr)
                        self.state = 369
                        if not self.precpred(self._ctx, 3):
                            from antlr4.error.Errors import FailedPredicateException
                            raise FailedPredicateException(self, "self.precpred(self._ctx, 3)")
                        self.state = 370
                        self.match(GrammarParser.T__49)
                        self.state = 371
                        self.expr(4)
                        pass

                    elif la_ == 10:
                        localctx = GrammarParser.BinaryContext(self, GrammarParser.ExprContext(self, _parentctx, _parentState))
                        self.pushNewRecursionContext(localctx, _startState, self.RULE_expr)
                        self.state = 372
                        if not self.precpred(self._ctx, 2):
                            from antlr4.error.Errors import FailedPredicateException
                            raise FailedPredicateException(self, "self.precpred(self._ctx, 2)")
                        self.state = 373
                        self.match(GrammarParser.T__50)
                        self.state = 374
                        self.expr(3)
                        pass

                    elif la_ == 11:
                        localctx = GrammarParser.AssignmentContext(self, GrammarParser.ExprContext(self, _parentctx, _parentState))
                        self.pushNewRecursionContext(localctx, _startState, self.RULE_expr)
                        self.state = 375
                        if not self.precpred(self._ctx, 1):
                            from antlr4.error.Errors import FailedPredicateException
                            raise FailedPredicateException(self, "self.precpred(self._ctx, 1)")
                        self.state = 376
                        self.match(GrammarParser.T__9)
                        self.state = 377
                        self.expr(1)
                        pass

                    elif la_ == 12:
                        localctx = GrammarParser.Function_callContext(self, GrammarParser.ExprContext(self, _parentctx, _parentState))
                        self.pushNewRecursionContext(localctx, _startState, self.RULE_expr)
                        self.state = 378
                        if not self.precpred(self._ctx, 18):
                            from antlr4.error.Errors import FailedPredicateException
                            raise FailedPredicateException(self, "self.precpred(self._ctx, 18)")
                        self.state = 379
                        self.match(GrammarParser.T__11)
                        self.state = 381
                        self._errHandler.sync(self)
                        _la = self._input.LA(1)
                        if (((_la) & ~0x3f) == 0 and ((1 << _la) & 4467571104189255680) != 0):
                            self.state = 380
                            self.arg_list()


                        self.state = 383
                        self.match(GrammarParser.T__12)
                        pass

                    elif la_ == 13:
                        localctx = GrammarParser.Array_accessContext(self, GrammarParser.ExprContext(self, _parentctx, _parentState))
                        self.pushNewRecursionContext(localctx, _startState, self.RULE_expr)
                        self.state = 384
                        if not self.precpred(self._ctx, 17):
                            from antlr4.error.Errors import FailedPredicateException
                            raise FailedPredicateException(self, "self.precpred(self._ctx, 17)")
                        self.state = 385
                        self.match(GrammarParser.T__25)
                        self.state = 386
                        self.expr(0)
                        self.state = 387
                        self.match(GrammarParser.T__26)
                        pass

                    elif la_ == 14:
                        localctx = GrammarParser.Member_accessContext(self, GrammarParser.ExprContext(self, _parentctx, _parentState))
                        self.pushNewRecursionContext(localctx, _startState, self.RULE_expr)
                        self.state = 389
                        if not self.precpred(self._ctx, 16):
                            from antlr4.error.Errors import FailedPredicateException
                            raise FailedPredicateException(self, "self.precpred(self._ctx, 16)")
                        self.state = 390
                        self.match(GrammarParser.T__27)
                        self.state = 391
                        self.match(GrammarParser.ID)
                        pass

                    elif la_ == 15:
                        localctx = GrammarParser.Member_access_ptrContext(self, GrammarParser.ExprContext(self, _parentctx, _parentState))
                        self.pushNewRecursionContext(localctx, _startState, self.RULE_expr)
                        self.state = 392
                        if not self.precpred(self._ctx, 15):
                            from antlr4.error.Errors import FailedPredicateException
                            raise FailedPredicateException(self, "self.precpred(self._ctx, 15)")
                        self.state = 393
                        self.match(GrammarParser.T__28)
                        self.state = 394
                        self.match(GrammarParser.ID)
                        pass

                    elif la_ == 16:
                        localctx = GrammarParser.Unary_postfixContext(self, GrammarParser.ExprContext(self, _parentctx, _parentState))
                        self.pushNewRecursionContext(localctx, _startState, self.RULE_expr)
                        self.state = 395
                        if not self.precpred(self._ctx, 14):
                            from antlr4.error.Errors import FailedPredicateException
                            raise FailedPredicateException(self, "self.precpred(self._ctx, 14)")
                        self.state = 396
                        _la = self._input.LA(1)
                        if not(_la==30 or _la==31):
                            self._errHandler.recoverInline(self)
                        else:
                            self._errHandler.reportMatch(self)
                            self.consume()
                        pass

             
                self.state = 401
                self._errHandler.sync(self)
                _alt = self._interp.adaptivePredict(self._input,37,self._ctx)

        except RecognitionException as re:
            localctx.exception = re
            self._errHandler.reportError(self, re)
            self._errHandler.recover(self, re)
        finally:
            self.unrollRecursionContexts(_parentctx)
        return localctx


    class Arg_listContext(ParserRuleContext):
        __slots__ = 'parser'

        def __init__(self, parser, parent:ParserRuleContext=None, invokingState:int=-1):
            super().__init__(parent, invokingState)
            self.parser = parser

        def expr(self, i:int=None):
            if i is None:
                return self.getTypedRuleContexts(GrammarParser.ExprContext)
            else:
                return self.getTypedRuleContext(GrammarParser.ExprContext,i)


        def getRuleIndex(self):
            return GrammarParser.RULE_arg_list

        def accept(self, visitor:ParseTreeVisitor):
            if hasattr( visitor, "visitArg_list" ):
                return visitor.visitArg_list(self)
            else:
                return visitor.visitChildren(self)




    def arg_list(self):

        localctx = GrammarParser.Arg_listContext(self, self._ctx, self.state)
        self.enterRule(localctx, 48, self.RULE_arg_list)
        self._la = 0 # Token type
        try:
            self.enterOuterAlt(localctx, 1)
            self.state = 402
            self.expr(0)
            self.state = 407
            self._errHandler.sync(self)
            _la = self._input.LA(1)
            while _la==8:
                self.state = 403
                self.match(GrammarParser.T__7)
                self.state = 404
                self.expr(0)
                self.state = 409
                self._errHandler.sync(self)
                _la = self._input.LA(1)

        except RecognitionException as re:
            localctx.exception = re
            self._errHandler.reportError(self, re)
            self._errHandler.recover(self, re)
        finally:
            self.exitRule()
        return localctx


    class LiteralContext(ParserRuleContext):
        __slots__ = 'parser'

        def __init__(self, parser, parent:ParserRuleContext=None, invokingState:int=-1):
            super().__init__(parent, invokingState)
            self.parser = parser

        def INT(self):
            return self.getToken(GrammarParser.INT, 0)

        def REAL(self):
            return self.getToken(GrammarParser.REAL, 0)

        def CHAR(self):
            return self.getToken(GrammarParser.CHAR, 0)

        def STRING(self):
            return self.getToken(GrammarParser.STRING, 0)

        def getRuleIndex(self):
            return GrammarParser.RULE_literal

        def accept(self, visitor:ParseTreeVisitor):
            if hasattr( visitor, "visitLiteral" ):
                return visitor.visitLiteral(self)
            else:
                return visitor.visitChildren(self)




    def literal(self):

        localctx = GrammarParser.LiteralContext(self, self._ctx, self.state)
        self.enterRule(localctx, 50, self.RULE_literal)
        self._la = 0 # Token type
        try:
            self.enterOuterAlt(localctx, 1)
            self.state = 410
            _la = self._input.LA(1)
            if not((((_la) & ~0x3f) == 0 and ((1 << _la) & 2161727821137838080) != 0)):
                self._errHandler.recoverInline(self)
            else:
                self._errHandler.reportMatch(self)
                self.consume()
        except RecognitionException as re:
            localctx.exception = re
            self._errHandler.reportError(self, re)
            self._errHandler.recover(self, re)
        finally:
            self.exitRule()
        return localctx


    class DeclarationContext(ParserRuleContext):
        __slots__ = 'parser'

        def __init__(self, parser, parent:ParserRuleContext=None, invokingState:int=-1):
            super().__init__(parent, invokingState)
            self.parser = parser

        def type_(self):
            return self.getTypedRuleContext(GrammarParser.TypeContext,0)


        def init_declarator(self, i:int=None):
            if i is None:
                return self.getTypedRuleContexts(GrammarParser.Init_declaratorContext)
            else:
                return self.getTypedRuleContext(GrammarParser.Init_declaratorContext,i)


        def getRuleIndex(self):
            return GrammarParser.RULE_declaration

        def accept(self, visitor:ParseTreeVisitor):
            if hasattr( visitor, "visitDeclaration" ):
                return visitor.visitDeclaration(self)
            else:
                return visitor.visitChildren(self)




    def declaration(self):

        localctx = GrammarParser.DeclarationContext(self, self._ctx, self.state)
        self.enterRule(localctx, 52, self.RULE_declaration)
        self._la = 0 # Token type
        try:
            self.enterOuterAlt(localctx, 1)
            self.state = 412
            self.type_()
            self.state = 413
            self.init_declarator()
            self.state = 418
            self._errHandler.sync(self)
            _la = self._input.LA(1)
            while _la==8:
                self.state = 414
                self.match(GrammarParser.T__7)
                self.state = 415
                self.init_declarator()
                self.state = 420
                self._errHandler.sync(self)
                _la = self._input.LA(1)

        except RecognitionException as re:
            localctx.exception = re
            self._errHandler.reportError(self, re)
            self._errHandler.recover(self, re)
        finally:
            self.exitRule()
        return localctx


    class Init_declaratorContext(ParserRuleContext):
        __slots__ = 'parser'

        def __init__(self, parser, parent:ParserRuleContext=None, invokingState:int=-1):
            super().__init__(parent, invokingState)
            self.parser = parser

        def ID(self):
            return self.getToken(GrammarParser.ID, 0)

        def array_sizes(self):
            return self.getTypedRuleContext(GrammarParser.Array_sizesContext,0)


        def expr(self):
            return self.getTypedRuleContext(GrammarParser.ExprContext,0)


        def initializer_list(self):
            return self.getTypedRuleContext(GrammarParser.Initializer_listContext,0)


        def getRuleIndex(self):
            return GrammarParser.RULE_init_declarator

        def accept(self, visitor:ParseTreeVisitor):
            if hasattr( visitor, "visitInit_declarator" ):
                return visitor.visitInit_declarator(self)
            else:
                return visitor.visitChildren(self)




    def init_declarator(self):

        localctx = GrammarParser.Init_declaratorContext(self, self._ctx, self.state)
        self.enterRule(localctx, 54, self.RULE_init_declarator)
        self._la = 0 # Token type
        try:
            self.enterOuterAlt(localctx, 1)
            self.state = 421
            self.match(GrammarParser.ID)
            self.state = 423
            self._errHandler.sync(self)
            _la = self._input.LA(1)
            if _la==26:
                self.state = 422
                self.array_sizes()


            self.state = 430
            self._errHandler.sync(self)
            _la = self._input.LA(1)
            if _la==10:
                self.state = 425
                self.match(GrammarParser.T__9)
                self.state = 428
                self._errHandler.sync(self)
                token = self._input.LA(1)
                if token in [12, 25, 30, 31, 32, 33, 34, 35, 36, 37, 57, 58, 59, 60, 61]:
                    self.state = 426
                    self.expr(0)
                    pass
                elif token in [6]:
                    self.state = 427
                    self.initializer_list()
                    pass
                else:
                    raise NoViableAltException(self)



        except RecognitionException as re:
            localctx.exception = re
            self._errHandler.reportError(self, re)
            self._errHandler.recover(self, re)
        finally:
            self.exitRule()
        return localctx


    class Initializer_listContext(ParserRuleContext):
        __slots__ = 'parser'

        def __init__(self, parser, parent:ParserRuleContext=None, invokingState:int=-1):
            super().__init__(parent, invokingState)
            self.parser = parser

        def initializer_element(self, i:int=None):
            if i is None:
                return self.getTypedRuleContexts(GrammarParser.Initializer_elementContext)
            else:
                return self.getTypedRuleContext(GrammarParser.Initializer_elementContext,i)


        def getRuleIndex(self):
            return GrammarParser.RULE_initializer_list

        def accept(self, visitor:ParseTreeVisitor):
            if hasattr( visitor, "visitInitializer_list" ):
                return visitor.visitInitializer_list(self)
            else:
                return visitor.visitChildren(self)




    def initializer_list(self):

        localctx = GrammarParser.Initializer_listContext(self, self._ctx, self.state)
        self.enterRule(localctx, 56, self.RULE_initializer_list)
        self._la = 0 # Token type
        try:
            self.enterOuterAlt(localctx, 1)
            self.state = 432
            self.match(GrammarParser.T__5)
            self.state = 441
            self._errHandler.sync(self)
            _la = self._input.LA(1)
            if (((_la) & ~0x3f) == 0 and ((1 << _la) & 4467571104189255744) != 0):
                self.state = 433
                self.initializer_element()
                self.state = 438
                self._errHandler.sync(self)
                _alt = self._interp.adaptivePredict(self._input,43,self._ctx)
                while _alt!=2 and _alt!=ATN.INVALID_ALT_NUMBER:
                    if _alt==1:
                        self.state = 434
                        self.match(GrammarParser.T__7)
                        self.state = 435
                        self.initializer_element() 
                    self.state = 440
                    self._errHandler.sync(self)
                    _alt = self._interp.adaptivePredict(self._input,43,self._ctx)



            self.state = 444
            self._errHandler.sync(self)
            _la = self._input.LA(1)
            if _la==8:
                self.state = 443
                self.match(GrammarParser.T__7)


            self.state = 446
            self.match(GrammarParser.T__6)
        except RecognitionException as re:
            localctx.exception = re
            self._errHandler.reportError(self, re)
            self._errHandler.recover(self, re)
        finally:
            self.exitRule()
        return localctx


    class Initializer_elementContext(ParserRuleContext):
        __slots__ = 'parser'

        def __init__(self, parser, parent:ParserRuleContext=None, invokingState:int=-1):
            super().__init__(parent, invokingState)
            self.parser = parser

        def expr(self):
            return self.getTypedRuleContext(GrammarParser.ExprContext,0)


        def initializer_list(self):
            return self.getTypedRuleContext(GrammarParser.Initializer_listContext,0)


        def getRuleIndex(self):
            return GrammarParser.RULE_initializer_element

        def accept(self, visitor:ParseTreeVisitor):
            if hasattr( visitor, "visitInitializer_element" ):
                return visitor.visitInitializer_element(self)
            else:
                return visitor.visitChildren(self)




    def initializer_element(self):

        localctx = GrammarParser.Initializer_elementContext(self, self._ctx, self.state)
        self.enterRule(localctx, 58, self.RULE_initializer_element)
        try:
            self.state = 450
            self._errHandler.sync(self)
            token = self._input.LA(1)
            if token in [12, 25, 30, 31, 32, 33, 34, 35, 36, 37, 57, 58, 59, 60, 61]:
                self.enterOuterAlt(localctx, 1)
                self.state = 448
                self.expr(0)
                pass
            elif token in [6]:
                self.enterOuterAlt(localctx, 2)
                self.state = 449
                self.initializer_list()
                pass
            else:
                raise NoViableAltException(self)

        except RecognitionException as re:
            localctx.exception = re
            self._errHandler.reportError(self, re)
            self._errHandler.recover(self, re)
        finally:
            self.exitRule()
        return localctx


    class Array_sizesContext(ParserRuleContext):
        __slots__ = 'parser'

        def __init__(self, parser, parent:ParserRuleContext=None, invokingState:int=-1):
            super().__init__(parent, invokingState)
            self.parser = parser

        def expr(self, i:int=None):
            if i is None:
                return self.getTypedRuleContexts(GrammarParser.ExprContext)
            else:
                return self.getTypedRuleContext(GrammarParser.ExprContext,i)


        def getRuleIndex(self):
            return GrammarParser.RULE_array_sizes

        def accept(self, visitor:ParseTreeVisitor):
            if hasattr( visitor, "visitArray_sizes" ):
                return visitor.visitArray_sizes(self)
            else:
                return visitor.visitChildren(self)




    def array_sizes(self):

        localctx = GrammarParser.Array_sizesContext(self, self._ctx, self.state)
        self.enterRule(localctx, 60, self.RULE_array_sizes)
        self._la = 0 # Token type
        try:
            self.state = 471
            self._errHandler.sync(self)
            la_ = self._interp.adaptivePredict(self._input,49,self._ctx)
            if la_ == 1:
                self.enterOuterAlt(localctx, 1)
                self.state = 452
                self.match(GrammarParser.T__25)
                self.state = 453
                self.match(GrammarParser.T__26)
                self.state = 460
                self._errHandler.sync(self)
                _la = self._input.LA(1)
                while _la==26:
                    self.state = 454
                    self.match(GrammarParser.T__25)
                    self.state = 455
                    self.expr(0)
                    self.state = 456
                    self.match(GrammarParser.T__26)
                    self.state = 462
                    self._errHandler.sync(self)
                    _la = self._input.LA(1)

                pass

            elif la_ == 2:
                self.enterOuterAlt(localctx, 2)
                self.state = 467 
                self._errHandler.sync(self)
                _la = self._input.LA(1)
                while True:
                    self.state = 463
                    self.match(GrammarParser.T__25)
                    self.state = 464
                    self.expr(0)
                    self.state = 465
                    self.match(GrammarParser.T__26)
                    self.state = 469 
                    self._errHandler.sync(self)
                    _la = self._input.LA(1)
                    if not (_la==26):
                        break

                pass


        except RecognitionException as re:
            localctx.exception = re
            self._errHandler.reportError(self, re)
            self._errHandler.recover(self, re)
        finally:
            self.exitRule()
        return localctx


    class TypeContext(ParserRuleContext):
        __slots__ = 'parser'

        def __init__(self, parser, parent:ParserRuleContext=None, invokingState:int=-1):
            super().__init__(parent, invokingState)
            self.parser = parser

        def typeSpecifier(self):
            return self.getTypedRuleContext(GrammarParser.TypeSpecifierContext,0)


        def typeQualifier(self, i:int=None):
            if i is None:
                return self.getTypedRuleContexts(GrammarParser.TypeQualifierContext)
            else:
                return self.getTypedRuleContext(GrammarParser.TypeQualifierContext,i)


        def pointerQualifier(self, i:int=None):
            if i is None:
                return self.getTypedRuleContexts(GrammarParser.PointerQualifierContext)
            else:
                return self.getTypedRuleContext(GrammarParser.PointerQualifierContext,i)


        def getRuleIndex(self):
            return GrammarParser.RULE_type

        def accept(self, visitor:ParseTreeVisitor):
            if hasattr( visitor, "visitType" ):
                return visitor.visitType(self)
            else:
                return visitor.visitChildren(self)




    def type_(self):

        localctx = GrammarParser.TypeContext(self, self._ctx, self.state)
        self.enterRule(localctx, 62, self.RULE_type)
        self._la = 0 # Token type
        try:
            self.state = 503
            self._errHandler.sync(self)
            la_ = self._interp.adaptivePredict(self._input,55,self._ctx)
            if la_ == 1:
                self.enterOuterAlt(localctx, 1)
                self.state = 476
                self._errHandler.sync(self)
                _la = self._input.LA(1)
                while _la==52:
                    self.state = 473
                    self.typeQualifier()
                    self.state = 478
                    self._errHandler.sync(self)
                    _la = self._input.LA(1)

                self.state = 479
                self.typeSpecifier()
                self.state = 483
                self._errHandler.sync(self)
                _alt = self._interp.adaptivePredict(self._input,51,self._ctx)
                while _alt!=2 and _alt!=ATN.INVALID_ALT_NUMBER:
                    if _alt==1:
                        self.state = 480
                        self.typeQualifier() 
                    self.state = 485
                    self._errHandler.sync(self)
                    _alt = self._interp.adaptivePredict(self._input,51,self._ctx)

                self.state = 489
                self._errHandler.sync(self)
                _la = self._input.LA(1)
                while _la==36 or _la==52:
                    self.state = 486
                    self.pointerQualifier()
                    self.state = 491
                    self._errHandler.sync(self)
                    _la = self._input.LA(1)

                pass

            elif la_ == 2:
                self.enterOuterAlt(localctx, 2)
                self.state = 493 
                self._errHandler.sync(self)
                _alt = 1
                while _alt!=2 and _alt!=ATN.INVALID_ALT_NUMBER:
                    if _alt == 1:
                        self.state = 492
                        self.typeQualifier()

                    else:
                        raise NoViableAltException(self)
                    self.state = 495 
                    self._errHandler.sync(self)
                    _alt = self._interp.adaptivePredict(self._input,53,self._ctx)

                self.state = 500
                self._errHandler.sync(self)
                _la = self._input.LA(1)
                while _la==36 or _la==52:
                    self.state = 497
                    self.pointerQualifier()
                    self.state = 502
                    self._errHandler.sync(self)
                    _la = self._input.LA(1)

                pass


        except RecognitionException as re:
            localctx.exception = re
            self._errHandler.reportError(self, re)
            self._errHandler.recover(self, re)
        finally:
            self.exitRule()
        return localctx


    class PointerQualifierContext(ParserRuleContext):
        __slots__ = 'parser'

        def __init__(self, parser, parent:ParserRuleContext=None, invokingState:int=-1):
            super().__init__(parent, invokingState)
            self.parser = parser

        def typeQualifier(self, i:int=None):
            if i is None:
                return self.getTypedRuleContexts(GrammarParser.TypeQualifierContext)
            else:
                return self.getTypedRuleContext(GrammarParser.TypeQualifierContext,i)


        def getRuleIndex(self):
            return GrammarParser.RULE_pointerQualifier

        def accept(self, visitor:ParseTreeVisitor):
            if hasattr( visitor, "visitPointerQualifier" ):
                return visitor.visitPointerQualifier(self)
            else:
                return visitor.visitChildren(self)




    def pointerQualifier(self):

        localctx = GrammarParser.PointerQualifierContext(self, self._ctx, self.state)
        self.enterRule(localctx, 64, self.RULE_pointerQualifier)
        self._la = 0 # Token type
        try:
            self.enterOuterAlt(localctx, 1)
            self.state = 508
            self._errHandler.sync(self)
            _la = self._input.LA(1)
            while _la==52:
                self.state = 505
                self.typeQualifier()
                self.state = 510
                self._errHandler.sync(self)
                _la = self._input.LA(1)

            self.state = 511
            self.match(GrammarParser.T__35)
            self.state = 515
            self._errHandler.sync(self)
            _alt = self._interp.adaptivePredict(self._input,57,self._ctx)
            while _alt!=2 and _alt!=ATN.INVALID_ALT_NUMBER:
                if _alt==1:
                    self.state = 512
                    self.typeQualifier() 
                self.state = 517
                self._errHandler.sync(self)
                _alt = self._interp.adaptivePredict(self._input,57,self._ctx)

        except RecognitionException as re:
            localctx.exception = re
            self._errHandler.reportError(self, re)
            self._errHandler.recover(self, re)
        finally:
            self.exitRule()
        return localctx


    class TypeQualifierContext(ParserRuleContext):
        __slots__ = 'parser'

        def __init__(self, parser, parent:ParserRuleContext=None, invokingState:int=-1):
            super().__init__(parent, invokingState)
            self.parser = parser


        def getRuleIndex(self):
            return GrammarParser.RULE_typeQualifier

        def accept(self, visitor:ParseTreeVisitor):
            if hasattr( visitor, "visitTypeQualifier" ):
                return visitor.visitTypeQualifier(self)
            else:
                return visitor.visitChildren(self)




    def typeQualifier(self):

        localctx = GrammarParser.TypeQualifierContext(self, self._ctx, self.state)
        self.enterRule(localctx, 66, self.RULE_typeQualifier)
        try:
            self.enterOuterAlt(localctx, 1)
            self.state = 518
            self.match(GrammarParser.T__51)
        except RecognitionException as re:
            localctx.exception = re
            self._errHandler.reportError(self, re)
            self._errHandler.recover(self, re)
        finally:
            self.exitRule()
        return localctx


    class TypeSpecifierContext(ParserRuleContext):
        __slots__ = 'parser'

        def __init__(self, parser, parent:ParserRuleContext=None, invokingState:int=-1):
            super().__init__(parent, invokingState)
            self.parser = parser


        def getRuleIndex(self):
            return GrammarParser.RULE_typeSpecifier

     
        def copyFrom(self, ctx:ParserRuleContext):
            super().copyFrom(ctx)



    class BaseSpecifierContext(TypeSpecifierContext):

        def __init__(self, parser, ctx:ParserRuleContext): # actually a GrammarParser.TypeSpecifierContext
            super().__init__(parser)
            self.copyFrom(ctx)

        def keyword(self):
            return self.getTypedRuleContext(GrammarParser.KeywordContext,0)


        def accept(self, visitor:ParseTreeVisitor):
            if hasattr( visitor, "visitBaseSpecifier" ):
                return visitor.visitBaseSpecifier(self)
            else:
                return visitor.visitChildren(self)


    class TypedefSpecifierContext(TypeSpecifierContext):

        def __init__(self, parser, ctx:ParserRuleContext): # actually a GrammarParser.TypeSpecifierContext
            super().__init__(parser)
            self.copyFrom(ctx)

        def ID(self):
            return self.getToken(GrammarParser.ID, 0)

        def accept(self, visitor:ParseTreeVisitor):
            if hasattr( visitor, "visitTypedefSpecifier" ):
                return visitor.visitTypedefSpecifier(self)
            else:
                return visitor.visitChildren(self)


    class StructSpecifierContext(TypeSpecifierContext):

        def __init__(self, parser, ctx:ParserRuleContext): # actually a GrammarParser.TypeSpecifierContext
            super().__init__(parser)
            self.copyFrom(ctx)

        def ID(self):
            return self.getToken(GrammarParser.ID, 0)

        def accept(self, visitor:ParseTreeVisitor):
            if hasattr( visitor, "visitStructSpecifier" ):
                return visitor.visitStructSpecifier(self)
            else:
                return visitor.visitChildren(self)


    class EnumSpecifierContext(TypeSpecifierContext):

        def __init__(self, parser, ctx:ParserRuleContext): # actually a GrammarParser.TypeSpecifierContext
            super().__init__(parser)
            self.copyFrom(ctx)

        def ID(self):
            return self.getToken(GrammarParser.ID, 0)

        def accept(self, visitor:ParseTreeVisitor):
            if hasattr( visitor, "visitEnumSpecifier" ):
                return visitor.visitEnumSpecifier(self)
            else:
                return visitor.visitChildren(self)



    def typeSpecifier(self):

        localctx = GrammarParser.TypeSpecifierContext(self, self._ctx, self.state)
        self.enterRule(localctx, 68, self.RULE_typeSpecifier)
        self._la = 0 # Token type
        try:
            self.state = 526
            self._errHandler.sync(self)
            token = self._input.LA(1)
            if token in [53, 54, 55, 56]:
                localctx = GrammarParser.BaseSpecifierContext(self, localctx)
                self.enterOuterAlt(localctx, 1)
                self.state = 520
                self.keyword()
                pass
            elif token in [9]:
                localctx = GrammarParser.EnumSpecifierContext(self, localctx)
                self.enterOuterAlt(localctx, 2)
                self.state = 521
                self.match(GrammarParser.T__8)
                self.state = 522
                self.match(GrammarParser.ID)
                pass
            elif token in [4, 5]:
                localctx = GrammarParser.StructSpecifierContext(self, localctx)
                self.enterOuterAlt(localctx, 3)
                self.state = 523
                _la = self._input.LA(1)
                if not(_la==4 or _la==5):
                    self._errHandler.recoverInline(self)
                else:
                    self._errHandler.reportMatch(self)
                    self.consume()
                self.state = 524
                self.match(GrammarParser.ID)
                pass
            elif token in [61]:
                localctx = GrammarParser.TypedefSpecifierContext(self, localctx)
                self.enterOuterAlt(localctx, 4)
                self.state = 525
                self.match(GrammarParser.ID)
                pass
            else:
                raise NoViableAltException(self)

        except RecognitionException as re:
            localctx.exception = re
            self._errHandler.reportError(self, re)
            self._errHandler.recover(self, re)
        finally:
            self.exitRule()
        return localctx


    class KeywordContext(ParserRuleContext):
        __slots__ = 'parser'

        def __init__(self, parser, parent:ParserRuleContext=None, invokingState:int=-1):
            super().__init__(parent, invokingState)
            self.parser = parser


        def getRuleIndex(self):
            return GrammarParser.RULE_keyword

        def accept(self, visitor:ParseTreeVisitor):
            if hasattr( visitor, "visitKeyword" ):
                return visitor.visitKeyword(self)
            else:
                return visitor.visitChildren(self)




    def keyword(self):

        localctx = GrammarParser.KeywordContext(self, self._ctx, self.state)
        self.enterRule(localctx, 70, self.RULE_keyword)
        self._la = 0 # Token type
        try:
            self.enterOuterAlt(localctx, 1)
            self.state = 528
            _la = self._input.LA(1)
            if not((((_la) & ~0x3f) == 0 and ((1 << _la) & 135107988821114880) != 0)):
                self._errHandler.recoverInline(self)
            else:
                self._errHandler.reportMatch(self)
                self.consume()
        except RecognitionException as re:
            localctx.exception = re
            self._errHandler.reportError(self, re)
            self._errHandler.recover(self, re)
        finally:
            self.exitRule()
        return localctx



    def sempred(self, localctx:RuleContext, ruleIndex:int, predIndex:int):
        if self._predicates == None:
            self._predicates = dict()
        self._predicates[23] = self.expr_sempred
        pred = self._predicates.get(ruleIndex, None)
        if pred is None:
            raise Exception("No predicate with index:" + str(ruleIndex))
        else:
            return pred(localctx, predIndex)

    def expr_sempred(self, localctx:ExprContext, predIndex:int):
            if predIndex == 0:
                return self.precpred(self._ctx, 11)
         

            if predIndex == 1:
                return self.precpred(self._ctx, 10)
         

            if predIndex == 2:
                return self.precpred(self._ctx, 9)
         

            if predIndex == 3:
                return self.precpred(self._ctx, 8)
         

            if predIndex == 4:
                return self.precpred(self._ctx, 7)
         

            if predIndex == 5:
                return self.precpred(self._ctx, 6)
         

            if predIndex == 6:
                return self.precpred(self._ctx, 5)
         

            if predIndex == 7:
                return self.precpred(self._ctx, 4)
         

            if predIndex == 8:
                return self.precpred(self._ctx, 3)
         

            if predIndex == 9:
                return self.precpred(self._ctx, 2)
         

            if predIndex == 10:
                return self.precpred(self._ctx, 1)
         

            if predIndex == 11:
                return self.precpred(self._ctx, 18)
         

            if predIndex == 12:
                return self.precpred(self._ctx, 17)
         

            if predIndex == 13:
                return self.precpred(self._ctx, 16)
         

            if predIndex == 14:
                return self.precpred(self._ctx, 15)
         

            if predIndex == 15:
                return self.precpred(self._ctx, 14)
         




