
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <curses.h>

#define BLACK_ON_WHITE 1
#define WHITE_ON_CYAN 2
#define DISPLAY_OFFSET_X 2
#define DISPLAY_OFFSET_Y 3

#define true 1
#define false 0
#define PROGMEM

#define DISPLAY_WIDTH 16
#define DISPLAY_HEIGHT 2

#define KEY_CURSOR_LEFT 0
#define KEY_CURSOR_RIGHT 1
#define KEY_CURSOR_UP 2
#define KEY_CURSOR_DOWN 3
#define KEY_CHARACTER_LEFT 4
#define KEY_CHARACTER_RIGHT 5
#define KEY_CHARACTER_UP 6
#define KEY_CHARACTER_DOWN 7
#define KEY_SELECT_OPTION 8
#define KEY_DELETE 9
#define KEY_NEWLINE 10
#define KEY_ESCAPE 11

#define ALLOCATION_PREVIOUS_OFFSET sizeof(int8_t *)
#define ALLOCATION_NEXT_OFFSET (ALLOCATION_PREVIOUS_OFFSET + sizeof(int8_t *))
#define ALLOCATION_SIZE_OFFSET (ALLOCATION_NEXT_OFFSET + 2)
#define ALLOCATION_TYPE_OFFSET (ALLOCATION_SIZE_OFFSET + 1)
#define ALLOCATION_IS_REACHABLE_OFFSET (ALLOCATION_TYPE_OFFSET + 1)
#define ALLOCATION_HEADER_SIZE ALLOCATION_SIZE_OFFSET
#define HEAP_START_ADDRESS (memory + sizeof(memory))

#define ALLOCATION_TYPE_POINTER 1
#define ALLOCATION_TYPE_STRING 2
#define ALLOCATION_TYPE_LIST 4

#define STRING_LENGTH_OFFSET 0
#define STRING_DATA_OFFSET (STRING_LENGTH_OFFSET + 2)

#define LIST_LENGTH_OFFSET 0
#define LIST_DATA_OFFSET (STRING_LENGTH_OFFSET + 2)

#define VALUE_TYPE_NUMBER 1
#define VALUE_TYPE_STRING 2
#define VALUE_TYPE_LIST 3

const int8_t SYMBOL_TEXT_BOOLEAN_AND[] PROGMEM = "&&";
const int8_t SYMBOL_TEXT_BOOLEAN_OR[] PROGMEM = "||";
const int8_t SYMBOL_TEXT_BOOLEAN_XOR[] PROGMEM = "^^";
const int8_t SYMBOL_TEXT_BITSHIFT_LEFT[] PROGMEM = "<<";
const int8_t SYMBOL_TEXT_BITSHIFT_RIGHT[] PROGMEM = ">>";
const int8_t SYMBOL_TEXT_INCREMENT[] PROGMEM = "++";
const int8_t SYMBOL_TEXT_DECREMENT[] PROGMEM = "--";
const int8_t SYMBOL_TEXT_EQUAL[] PROGMEM = "==";
const int8_t SYMBOL_TEXT_NOT_EQUAL[] PROGMEM = "!=";
const int8_t SYMBOL_TEXT_GREATER_OR_EQUAL[] PROGMEM = ">=";
const int8_t SYMBOL_TEXT_LESS_OR_EQUAL[] PROGMEM = "<=";

const int8_t SYMBOL_TEXT_ADD_ASSIGN[] PROGMEM = "+=";
const int8_t SYMBOL_TEXT_SUBTRACT_ASSIGN[] PROGMEM = "-=";
const int8_t SYMBOL_TEXT_MULTIPLY_ASSIGN[] PROGMEM = "*=";
const int8_t SYMBOL_TEXT_DIVIDE_ASSIGN[] PROGMEM = "/=";
const int8_t SYMBOL_TEXT_MODULUS_ASSIGN[] PROGMEM = "%=";
const int8_t SYMBOL_TEXT_BOOLEAN_AND_ASSIGN[] PROGMEM = "&&=";
const int8_t SYMBOL_TEXT_BOOLEAN_OR_ASSIGN[] PROGMEM = "||=";
const int8_t SYMBOL_TEXT_BOOLEAN_XOR_ASSIGN[] PROGMEM = "^^=";
const int8_t SYMBOL_TEXT_BITWISE_AND_ASSIGN[] PROGMEM = "&=";
const int8_t SYMBOL_TEXT_BITWISE_OR_ASSIGN[] PROGMEM = "|=";
const int8_t SYMBOL_TEXT_BITWISE_XOR_ASSIGN[] PROGMEM = "^=";
const int8_t SYMBOL_TEXT_BITSHIFT_LEFT_ASSIGN[] PROGMEM = "<<=";
const int8_t SYMBOL_TEXT_BITSHIFT_RIGHT_ASSIGN[] PROGMEM = ">>=";

const int8_t SYMBOL_TEXT_IF[] PROGMEM = "if:";
const int8_t SYMBOL_TEXT_ELSE_IF[] PROGMEM = "elif:";
const int8_t SYMBOL_TEXT_ELSE[] PROGMEM = "else;";
const int8_t SYMBOL_TEXT_END[] PROGMEM = "end;";
const int8_t SYMBOL_TEXT_WHILE[] PROGMEM = "while:";
const int8_t SYMBOL_TEXT_BREAK[] PROGMEM = "break;";
const int8_t SYMBOL_TEXT_CONTINUE[] PROGMEM = "cont;";
const int8_t SYMBOL_TEXT_FUNCTION[] PROGMEM = "func:";
const int8_t SYMBOL_TEXT_RETURN_WITH_VALUE[] PROGMEM = "ret:";
const int8_t SYMBOL_TEXT_RETURN[] PROGMEM = "ret;";
const int8_t SYMBOL_TEXT_QUIT[] PROGMEM = "quit;";

const int8_t SYMBOL_TEXT_RANDOM[] PROGMEM = "rand;";
const int8_t SYMBOL_TEXT_RANDOM_INTEGER[] PROGMEM = "randInt:";
const int8_t SYMBOL_TEXT_ABSOLUTE_VALUE[] PROGMEM = "abs:";
const int8_t SYMBOL_TEXT_ROUND[] PROGMEM = "round:";
const int8_t SYMBOL_TEXT_FLOOR[] PROGMEM = "floor:";
const int8_t SYMBOL_TEXT_CEILING[] PROGMEM = "ceil:";
const int8_t SYMBOL_TEXT_SINE[] PROGMEM = "sin:";
const int8_t SYMBOL_TEXT_COSINE[] PROGMEM = "cos:";
const int8_t SYMBOL_TEXT_TANGENT[] PROGMEM = "tan:";
const int8_t SYMBOL_TEXT_SQUARE_ROOT[] PROGMEM = "sqrt:";
const int8_t SYMBOL_TEXT_POWER[] PROGMEM = "pow:";
const int8_t SYMBOL_TEXT_LOG[] PROGMEM = "log:";

const int8_t SYMBOL_TEXT_NUMBER[] PROGMEM = "num:";
const int8_t SYMBOL_TEXT_STRING[] PROGMEM = "str:";
const int8_t SYMBOL_TEXT_TYPE[] PROGMEM = "type:";
const int8_t SYMBOL_TEXT_LENGTH[] PROGMEM = "len:";
const int8_t SYMBOL_TEXT_COPY[] PROGMEM = "copy:";
const int8_t SYMBOL_TEXT_INSERT[] PROGMEM = "ins:";
const int8_t SYMBOL_TEXT_REMOVE[] PROGMEM = "rem:";
const int8_t SYMBOL_TEXT_SUBSEQUENCE[] PROGMEM = "sub:";
const int8_t SYMBOL_TEXT_INSERT_SUBSEQUENCE[] PROGMEM = "insSub:";
const int8_t SYMBOL_TEXT_REMOVE_SUBSEQUENCE[] PROGMEM = "remSub:";

const int8_t SYMBOL_TEXT_PRINT[] PROGMEM = "print:";
const int8_t SYMBOL_TEXT_REQUEST_STRING[] PROGMEM = "reqStr;";
const int8_t SYMBOL_TEXT_REQUEST_NUMBER[] PROGMEM = "reqNum;";
const int8_t SYMBOL_TEXT_MENU[] PROGMEM = "menu";
const int8_t SYMBOL_TEXT_FILE_EXISTS[] PROGMEM = "fExists:";
const int8_t SYMBOL_TEXT_FILE_SIZE[] PROGMEM = "fSize:";
const int8_t SYMBOL_TEXT_FILE_CREATE[] PROGMEM = "fCreate:";
const int8_t SYMBOL_TEXT_FILE_DELETE[] PROGMEM = "fDelete:";
const int8_t SYMBOL_TEXT_FILE_READ[] PROGMEM = "fRead:";
const int8_t SYMBOL_TEXT_FILE_WRITE[] PROGMEM = "fWrite:";

const int8_t *SYMBOL_TEXT_LIST[] PROGMEM = {
    SYMBOL_TEXT_BOOLEAN_AND,
    SYMBOL_TEXT_BOOLEAN_OR,
    SYMBOL_TEXT_BOOLEAN_XOR,
    SYMBOL_TEXT_BITSHIFT_LEFT,
    SYMBOL_TEXT_BITSHIFT_RIGHT,
    SYMBOL_TEXT_INCREMENT,
    SYMBOL_TEXT_DECREMENT,
    SYMBOL_TEXT_EQUAL,
    SYMBOL_TEXT_NOT_EQUAL,
    SYMBOL_TEXT_GREATER_OR_EQUAL,
    SYMBOL_TEXT_LESS_OR_EQUAL,
    SYMBOL_TEXT_ADD_ASSIGN,
    SYMBOL_TEXT_SUBTRACT_ASSIGN,
    SYMBOL_TEXT_MULTIPLY_ASSIGN,
    SYMBOL_TEXT_DIVIDE_ASSIGN,
    SYMBOL_TEXT_MODULUS_ASSIGN,
    SYMBOL_TEXT_BOOLEAN_AND_ASSIGN,
    SYMBOL_TEXT_BOOLEAN_OR_ASSIGN,
    SYMBOL_TEXT_BOOLEAN_XOR_ASSIGN,
    SYMBOL_TEXT_BITWISE_AND_ASSIGN,
    SYMBOL_TEXT_BITWISE_OR_ASSIGN,
    SYMBOL_TEXT_BITWISE_XOR_ASSIGN,
    SYMBOL_TEXT_BITSHIFT_LEFT_ASSIGN,
    SYMBOL_TEXT_BITSHIFT_RIGHT_ASSIGN,
    SYMBOL_TEXT_IF,
    SYMBOL_TEXT_ELSE_IF,
    SYMBOL_TEXT_ELSE,
    SYMBOL_TEXT_END,
    SYMBOL_TEXT_WHILE,
    SYMBOL_TEXT_BREAK,
    SYMBOL_TEXT_CONTINUE,
    SYMBOL_TEXT_FUNCTION,
    SYMBOL_TEXT_RETURN_WITH_VALUE,
    SYMBOL_TEXT_RETURN,
    SYMBOL_TEXT_QUIT,
    SYMBOL_TEXT_RANDOM,
    SYMBOL_TEXT_RANDOM_INTEGER,
    SYMBOL_TEXT_ABSOLUTE_VALUE,
    SYMBOL_TEXT_ROUND,
    SYMBOL_TEXT_FLOOR,
    SYMBOL_TEXT_CEILING,
    SYMBOL_TEXT_SINE,
    SYMBOL_TEXT_COSINE,
    SYMBOL_TEXT_TANGENT,
    SYMBOL_TEXT_SQUARE_ROOT,
    SYMBOL_TEXT_POWER,
    SYMBOL_TEXT_LOG,
    SYMBOL_TEXT_NUMBER,
    SYMBOL_TEXT_STRING,
    SYMBOL_TEXT_TYPE,
    SYMBOL_TEXT_LENGTH,
    SYMBOL_TEXT_COPY,
    SYMBOL_TEXT_INSERT,
    SYMBOL_TEXT_REMOVE,
    SYMBOL_TEXT_SUBSEQUENCE,
    SYMBOL_TEXT_INSERT_SUBSEQUENCE,
    SYMBOL_TEXT_REMOVE_SUBSEQUENCE,
    SYMBOL_TEXT_PRINT,
    SYMBOL_TEXT_REQUEST_STRING,
    SYMBOL_TEXT_REQUEST_NUMBER,
    SYMBOL_TEXT_MENU,
    SYMBOL_TEXT_FILE_EXISTS,
    SYMBOL_TEXT_FILE_SIZE,
    SYMBOL_TEXT_FILE_CREATE,
    SYMBOL_TEXT_FILE_DELETE,
    SYMBOL_TEXT_FILE_READ,
    SYMBOL_TEXT_FILE_WRITE
};

#define SYMBOL_BOOLEAN_AND 128
#define SYMBOL_BOOLEAN_OR 129
#define SYMBOL_BOOLEAN_XOR 130
#define SYMBOL_BITSHIFT_LEFT 131
#define SYMBOL_BITSHIFT_RIGHT 132
#define SYMBOL_INCREMENT 133
#define SYMBOL_DECREMENT 134
#define SYMBOL_EQUAL 135
#define SYMBOL_NOT_EQUAL 136
#define SYMBOL_GREATER_OR_EQUAL 137
#define SYMBOL_LESS_OR_EQUAL 138
#define SYMBOL_ADD_ASSIGN 139
#define SYMBOL_SUBTRACT_ASSIGN 140
#define SYMBOL_MULTIPLY_ASSIGN 141
#define SYMBOL_DIVIDE_ASSIGN 142
#define SYMBOL_MODULUS_ASSIGN 143
#define SYMBOL_BOOLEAN_AND_ASSIGN 144
#define SYMBOL_BOOLEAN_OR_ASSIGN 145
#define SYMBOL_BOOLEAN_XOR_ASSIGN 146
#define SYMBOL_BITWISE_AND_ASSIGN 147
#define SYMBOL_BITWISE_OR_ASSIGN 148
#define SYMBOL_BITWISE_XOR_ASSIGN 149
#define SYMBOL_BITSHIFT_LEFT_ASSIGN 150
#define SYMBOL_BITSHIFT_RIGHT_ASSIGN 151
#define SYMBOL_IF 152
#define SYMBOL_ELSE_IF 153
#define SYMBOL_ELSE 154
#define SYMBOL_END 155
#define SYMBOL_WHILE 156
#define SYMBOL_BREAK 157
#define SYMBOL_CONTINUE 158
#define SYMBOL_FUNCTION 159
#define SYMBOL_RETURN_WITH_VALUE 160
#define SYMBOL_RETURN 161
#define SYMBOL_QUIT 162
#define SYMBOL_RANDOM 163
#define SYMBOL_RANDOM_INTEGER 164
#define SYMBOL_ABSOLUTE_VALUE 165
#define SYMBOL_ROUND 166
#define SYMBOL_FLOOR 167
#define SYMBOL_CEILING 168
#define SYMBOL_SINE 169
#define SYMBOL_COSINE 170
#define SYMBOL_TANGENT 171
#define SYMBOL_SQUARE_ROOT 172
#define SYMBOL_POWER 173
#define SYMBOL_LOG 174
#define SYMBOL_NUMBER 175
#define SYMBOL_STRING 176
#define SYMBOL_TYPE 177
#define SYMBOL_LENGTH 178
#define SYMBOL_COPY 179
#define SYMBOL_INSERT 180
#define SYMBOL_REMOVE 181
#define SYMBOL_SUBSEQUENCE 182
#define SYMBOL_INSERT_SUBSEQUENCE 183
#define SYMBOL_REMOVE_SUBSEQUENCE 184
#define SYMBOL_PRINT 185
#define SYMBOL_REQUEST_STRING 186
#define SYMBOL_REQUEST_NUMBER 187
#define SYMBOL_TEXT_MENU 188
#define SYMBOL_FILE_EXISTS 189
#define SYMBOL_FILE_SIZE 190
#define SYMBOL_FILE_CREATE 191
#define SYMBOL_FILE_DELETE 192
#define SYMBOL_FILE_READ 193
#define SYMBOL_FILE_WRITE 194

const int8_t SYMBOL_SET_LETTERS[] PROGMEM = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
    'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
    'U', 'V', 'W', 'X', 'Y', 'Z'
};

const int8_t SYMBOL_SET_NUMBERS[] PROGMEM = {
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
    '.', '-'
};

const uint8_t SYMBOL_SET_PUNCTUATION[] PROGMEM = {
    ',', '\'', '\"', '\\', '[', ']', '(', ')', ' ', '.',
    '?', '_', '@', '#', '{', '}'
};

const uint8_t SYMBOL_SET_OPERATORS[] PROGMEM = {
    '+',
    '-',
    '*',
    '/',
    '%',
    SYMBOL_INCREMENT,
    SYMBOL_DECREMENT,
    SYMBOL_EQUAL,
    SYMBOL_NOT_EQUAL,
    '>',
    '<',
    SYMBOL_GREATER_OR_EQUAL,
    SYMBOL_LESS_OR_EQUAL,
    SYMBOL_BOOLEAN_AND,
    SYMBOL_BOOLEAN_OR,
    SYMBOL_BOOLEAN_XOR,
    '!',
    '[',
    ']',
    '&',
    '|',
    '^',
    '~',
    SYMBOL_BITSHIFT_LEFT,
    SYMBOL_BITSHIFT_RIGHT,
    ':',
    ';'
};

const uint8_t SYMBOL_SET_ASSIGNMENT[] PROGMEM = {
    '=',
    SYMBOL_ADD_ASSIGN,
    SYMBOL_SUBTRACT_ASSIGN,
    SYMBOL_MULTIPLY_ASSIGN,
    SYMBOL_DIVIDE_ASSIGN,
    SYMBOL_MODULUS_ASSIGN,
    SYMBOL_BOOLEAN_AND_ASSIGN,
    SYMBOL_BOOLEAN_OR_ASSIGN,
    SYMBOL_BOOLEAN_XOR_ASSIGN,
    SYMBOL_BITWISE_AND_ASSIGN,
    SYMBOL_BITWISE_OR_ASSIGN,
    SYMBOL_BITWISE_XOR_ASSIGN,
    SYMBOL_BITSHIFT_LEFT_ASSIGN,
    SYMBOL_BITSHIFT_RIGHT_ASSIGN,
};

const uint8_t SYMBOL_SET_CONTROL[] PROGMEM = {
    SYMBOL_IF,
    SYMBOL_ELSE_IF,
    SYMBOL_ELSE,
    SYMBOL_END,
    SYMBOL_WHILE,
    SYMBOL_BREAK,
    SYMBOL_CONTINUE,
    SYMBOL_FUNCTION,
    SYMBOL_RETURN_WITH_VALUE,
    SYMBOL_RETURN,
    SYMBOL_QUIT
};

const uint8_t SYMBOL_SET_MATH[] PROGMEM = {
    SYMBOL_SQUARE_ROOT,
    SYMBOL_SINE,
    SYMBOL_COSINE,
    SYMBOL_TANGENT,
    SYMBOL_POWER,
    SYMBOL_LOG,
    SYMBOL_ABSOLUTE_VALUE,
    SYMBOL_ROUND,
    SYMBOL_FLOOR,
    SYMBOL_CEILING,
    SYMBOL_RANDOM,
    SYMBOL_RANDOM_INTEGER
};

const uint8_t SYMBOL_SET_INPUT_OUTPUT[] PROGMEM = {
    SYMBOL_PRINT,
    SYMBOL_REQUEST_NUMBER,
    SYMBOL_REQUEST_STRING,
    SYMBOL_TEXT_MENU,
    SYMBOL_FILE_EXISTS,
    SYMBOL_FILE_SIZE,
    SYMBOL_FILE_CREATE,
    SYMBOL_FILE_DELETE,
    SYMBOL_FILE_READ,
    SYMBOL_FILE_WRITE,
};

const uint8_t SYMBOL_SET_VALUE[] PROGMEM = {
    SYMBOL_LENGTH,
    SYMBOL_COPY,
    SYMBOL_INSERT,
    SYMBOL_REMOVE,
    SYMBOL_SUBSEQUENCE,
    SYMBOL_INSERT_SUBSEQUENCE,
    SYMBOL_REMOVE_SUBSEQUENCE,
    SYMBOL_NUMBER,
    SYMBOL_STRING,
    SYMBOL_TYPE
};

const int8_t *SYMBOL_SET_LIST[] PROGMEM = {
    SYMBOL_SET_LETTERS,
    SYMBOL_SET_NUMBERS,
    SYMBOL_SET_PUNCTUATION,
    SYMBOL_SET_OPERATORS,
    SYMBOL_SET_ASSIGNMENT,
    SYMBOL_SET_CONTROL,
    SYMBOL_SET_MATH,
    SYMBOL_SET_INPUT_OUTPUT,
    SYMBOL_SET_VALUE
};

const int8_t SYMBOL_SET_NAME_LETTERS[] PROGMEM = "Letter";
const int8_t SYMBOL_SET_NAME_NUMBERS[] PROGMEM = "Number";
const int8_t SYMBOL_SET_NAME_PUNCTUATION[] PROGMEM = "Punc";
const int8_t SYMBOL_SET_NAME_OPERATORS[] PROGMEM = "Operate";
const int8_t SYMBOL_SET_NAME_ASSIGNMENT[] PROGMEM = "Assign";
const int8_t SYMBOL_SET_NAME_CONTROL[] PROGMEM = "Control";
const int8_t SYMBOL_SET_NAME_MATH[] PROGMEM = "Math";
const int8_t SYMBOL_SET_NAME_INPUT_OUTPUT[] PROGMEM = "I/O";
const int8_t SYMBOL_SET_NAME_VALUE[] PROGMEM = "Value";

const int8_t *SYMBOL_SET_NAME_LIST[] PROGMEM = {
    SYMBOL_SET_NAME_LETTERS,
    SYMBOL_SET_NAME_NUMBERS,
    SYMBOL_SET_NAME_PUNCTUATION,
    SYMBOL_SET_NAME_OPERATORS,
    SYMBOL_SET_NAME_ASSIGNMENT,
    SYMBOL_SET_NAME_CONTROL,
    SYMBOL_SET_NAME_MATH,
    SYMBOL_SET_NAME_INPUT_OUTPUT,
    SYMBOL_SET_NAME_VALUE,
};

const int8_t SYMBOL_SET_SIZE_LIST[] PROGMEM = {
    sizeof(SYMBOL_SET_LETTERS),
    sizeof(SYMBOL_SET_NUMBERS),
    sizeof(SYMBOL_SET_PUNCTUATION),
    sizeof(SYMBOL_SET_OPERATORS),
    sizeof(SYMBOL_SET_ASSIGNMENT),
    sizeof(SYMBOL_SET_CONTROL),
    sizeof(SYMBOL_SET_MATH),
    sizeof(SYMBOL_SET_INPUT_OUTPUT),
    sizeof(SYMBOL_SET_VALUE)
};

const int8_t TEST_MESSAGE_1[] PROGMEM = "TITLE";
const int8_t TEST_MESSAGE_2[] PROGMEM = "ONE";
const int8_t TEST_MESSAGE_3[] PROGMEM = "TWO";
const int8_t TEST_MESSAGE_4[] PROGMEM = "THREE";
const int8_t TEST_MESSAGE_5[] PROGMEM = "FOUR";
const int8_t *TEST_MESSAGE_LIST[] PROGMEM = {
    TEST_MESSAGE_2,
    TEST_MESSAGE_3,
    TEST_MESSAGE_4,
    TEST_MESSAGE_5,
};

typedef struct value {
    int8_t type;
    int8_t data[sizeof(int8_t *) > sizeof(float) ? sizeof(int8_t *) : sizeof(float)];
} value_t;

WINDOW *window;
int32_t windowWidth;
int32_t windowHeight;
int8_t displayBuffer[DISPLAY_WIDTH * DISPLAY_HEIGHT];

int8_t memory[1500];
int8_t *firstAllocation = NULL;
int8_t *textEditorText;
int16_t textEditorIndex;

int8_t pgm_read_byte(const int8_t *pointer) {
    return *pointer;
}

int16_t pgm_read_word(const int16_t *pointer) {
    return *pointer;
}

int32_t pgm_read_dword(const int32_t *pointer) {
    return *pointer;
}

const void *pgm_read_ptr(const void **pointer) {
    return *pointer;
}

void displayCharacter(int8_t posX, int8_t posY, int8_t character) {
    if (posX < 0 || posX >= DISPLAY_WIDTH || posY < 0 || posY >= DISPLAY_HEIGHT) {
        return;
    }
    attron(COLOR_PAIR(BLACK_ON_WHITE));
    mvaddch(posY + DISPLAY_OFFSET_Y, posX + DISPLAY_OFFSET_X, character);
    attroff(COLOR_PAIR(BLACK_ON_WHITE));
    displayBuffer[posX + posY * DISPLAY_WIDTH] = character;
}

void handleResize();

int8_t getKey() {
    while (true) {
        int32_t tempKey = getch();
        if (tempKey == KEY_RESIZE) {
            handleResize();
        }
        if (tempKey == KEY_LEFT) {
            return KEY_CURSOR_LEFT;
        }
        if (tempKey == KEY_RIGHT) {
            return KEY_CURSOR_RIGHT;
        }
        if (tempKey == KEY_UP) {
            return KEY_CURSOR_UP;
        }
        if (tempKey == KEY_DOWN) {
            return KEY_CURSOR_DOWN;
        }
        if (tempKey == 'a') {
            return KEY_CHARACTER_LEFT;
        }
        if (tempKey == 'd') {
            return KEY_CHARACTER_RIGHT;
        }
        if (tempKey == 'w') {
            return KEY_CHARACTER_UP;
        }
        if (tempKey == 's') {
            return KEY_CHARACTER_DOWN;
        }
        if (tempKey == ' ') {
            return KEY_SELECT_OPTION;
        }
        if (tempKey == 127 || tempKey == 263) {
            return KEY_DELETE;
        }
        if (tempKey == '\n') {
            return KEY_NEWLINE;
        }
        if (tempKey == 27) {
            return KEY_ESCAPE;
        }
    }
}

void drawDisplayBuffer() {
    attron(COLOR_PAIR(BLACK_ON_WHITE));
    int8_t index = 0;
    int32_t tempPosY = 0;
    while (tempPosY < DISPLAY_HEIGHT) {
        int32_t tempPosX = 0;
        while (tempPosX < DISPLAY_WIDTH) {
            int8_t tempCharacter = displayBuffer[index];
            mvaddch(tempPosY + DISPLAY_OFFSET_Y, tempPosX + DISPLAY_OFFSET_X, tempCharacter);
            index += 1;
            tempPosX += 1;
        }
        tempPosY += 1;
    }
    attroff(COLOR_PAIR(BLACK_ON_WHITE));
}

void drawBackground() {
    int32_t tempPosY = 0;
    while (tempPosY < windowHeight) {
        int32_t tempPosX = 0;
        while (tempPosX < windowWidth) {
            int8_t tempColor;
            if (tempPosX >= DISPLAY_OFFSET_X && tempPosX < DISPLAY_OFFSET_X + DISPLAY_WIDTH
                    && tempPosY >= DISPLAY_OFFSET_Y && tempPosY < DISPLAY_OFFSET_Y + DISPLAY_HEIGHT) {
                tempColor = BLACK_ON_WHITE;
            } else {
                tempColor = WHITE_ON_CYAN;
            }
            attron(COLOR_PAIR(tempColor));
            mvaddch(tempPosY, tempPosX, ' ');
            attroff(COLOR_PAIR(tempColor));
            tempPosX += 1;
        }
        tempPosY += 1;
    }
    attron(COLOR_PAIR(WHITE_ON_CYAN));
    mvprintw(1, 1, "DUO Travel Emulator");
    attroff(COLOR_PAIR(WHITE_ON_CYAN));
    refresh();
}

void handleResize() {
    int32_t tempWidth;
    int32_t tempHeight;
    getmaxyx(window, tempHeight, tempWidth);
    if (tempWidth == windowWidth && tempHeight == windowHeight) {
        return;
    }
    windowWidth = tempWidth;
    windowHeight = tempHeight;
    drawBackground();
    drawDisplayBuffer();
}

static int16_t getProgMemTextLength(const int8_t *text) {
    int16_t index = 0;
    while (true) {
        if (pgm_read_byte(text + index) == 0) {
            return index;
        }
        index += 1;
    }
}

static void readProgMemText(int8_t *destination, const int8_t *text) {
    int16_t index = 0;
    while (true) {
        int8_t tempCharacter = pgm_read_byte(text + index);
        destination[index] = tempCharacter;
        if (tempCharacter == 0) {
            break;
        }
        index += 1;
    }
}

static int8_t *allocate(int16_t size, int8_t type) {
    int8_t *tempPreviousAllocation = NULL;
    int8_t *tempNextAllocation = firstAllocation;
    int8_t *output;
    while (true) {
        int8_t *tempAddress;
        if (tempPreviousAllocation == NULL) {
            tempAddress = HEAP_START_ADDRESS;
        } else {
            tempAddress = tempPreviousAllocation - ALLOCATION_HEADER_SIZE;
        }
        if (tempNextAllocation == NULL) {
            output = tempAddress - size;
            break;
        }
        int16_t tempAllocationSize = *(int16_t *)(tempNextAllocation - ALLOCATION_SIZE_OFFSET);
        int16_t tempGapSize = (tempAddress - tempNextAllocation) - tempAllocationSize;
        if (tempGapSize >= size + ALLOCATION_HEADER_SIZE) {
            output = tempAddress - size;
            break;
        }
        tempPreviousAllocation = tempNextAllocation;
        tempNextAllocation = *(int8_t **)(tempPreviousAllocation - ALLOCATION_NEXT_OFFSET);
    }
    *(int8_t **)(output - ALLOCATION_PREVIOUS_OFFSET) = tempPreviousAllocation;
    *(int8_t **)(output - ALLOCATION_NEXT_OFFSET) = tempNextAllocation;
    *(int16_t *)(output - ALLOCATION_SIZE_OFFSET) = size;
    *(int8_t *)(output - ALLOCATION_TYPE_OFFSET) = type;
    if (tempPreviousAllocation == NULL) {
        firstAllocation = output;
    } else {
        *(int8_t **)(tempPreviousAllocation - ALLOCATION_NEXT_OFFSET) = output;
    }
    if (tempNextAllocation != NULL) {
        *(int8_t **)(tempNextAllocation - ALLOCATION_PREVIOUS_OFFSET) = output;
    }
    return output;
}

static void deallocate(int8_t *allocation) {
    int8_t tempType = *(int8_t *)(allocation - ALLOCATION_TYPE_OFFSET);
    if (tempType == ALLOCATION_TYPE_POINTER) {
        deallocate(*(int8_t **)allocation);
    }
    int8_t *tempPreviousAllocation = *(int8_t **)(allocation - ALLOCATION_PREVIOUS_OFFSET);
    int8_t *tempNextAllocation = *(int8_t **)(allocation - ALLOCATION_NEXT_OFFSET);
    if (tempPreviousAllocation == NULL) {
        firstAllocation = tempNextAllocation;
    } else {
        *(int8_t **)(tempPreviousAllocation - ALLOCATION_NEXT_OFFSET) = tempNextAllocation;
    }
    if (tempNextAllocation != NULL) {
        *(int8_t **)(tempNextAllocation - ALLOCATION_PREVIOUS_OFFSET) = tempPreviousAllocation;
    }
}

static int8_t *createEmptyString(int16_t length) {
    int8_t *output = allocate(sizeof(int8_t *), ALLOCATION_TYPE_POINTER);
    int8_t *tempString = allocate(STRING_DATA_OFFSET + length + 1, ALLOCATION_TYPE_STRING);
    *(int8_t **)output = tempString;
    *(int16_t *)(tempString + STRING_LENGTH_OFFSET) = length;
    *(int8_t *)(tempString + STRING_DATA_OFFSET) = 0;
    return output;
}

static int8_t *createStringFromProgMem(const int8_t *text) {
    int8_t *output = createEmptyString(getProgMemTextLength(text));
    int8_t *tempString = *(int8_t **)output;
    readProgMemText(tempString + STRING_DATA_OFFSET, text);
    return output;
}

static int8_t *createEmptyList(int16_t length) {
    int8_t *output = allocate(sizeof(int8_t *), ALLOCATION_TYPE_POINTER);
    int8_t *tempList = allocate(LIST_DATA_OFFSET + length * sizeof(value_t), ALLOCATION_TYPE_LIST);
    *(int8_t **)output = tempList;
    *(int16_t *)(tempList + LIST_LENGTH_OFFSET) = length;
    int16_t index = 0;
    while (index < length) {
        value_t *tempValue = (value_t *)(tempList + LIST_DATA_OFFSET + index * sizeof(value_t));
        tempValue->type = VALUE_TYPE_NUMBER;
        *(float *)&(tempValue->data) = 0.0;
        index += 1;
    }
    return output;
}

static int8_t getSymbolWidth(uint8_t symbol) {
    if (symbol < 128) {
        return 1;
    }
    int8_t *tempText = (int8_t *)pgm_read_ptr((const void **)(SYMBOL_TEXT_LIST + (symbol - 128)));
    int8_t index = 0;
    while (true) {
        int8_t tempCharacter = pgm_read_byte(tempText + index);
        if (tempCharacter == 0) {
            return index;
        }
        index += 1;
    }
}

static void displaySymbol(int8_t posX, int8_t posY, uint8_t symbol) {
    if (symbol < 128) {
        displayCharacter(posX, posY, symbol);
    } else {
        int8_t *tempText = (int8_t *)pgm_read_ptr((const void **)(SYMBOL_TEXT_LIST + (symbol - 128)));
        int8_t index = 0;
        while (true) {
            int8_t tempCharacter = pgm_read_byte(tempText + index);
            if (tempCharacter == 0) {
                return;
            }
            displayCharacter(posX, posY, tempCharacter);
            index += 1;
            posX += 1;
        }
    }
}

static void displayText(int8_t posX, int8_t posY, int8_t *text) {
    int16_t index = 0;
    while (posY < DISPLAY_HEIGHT) {
        uint8_t tempSymbol = text[index];
        if (tempSymbol == 0) {
            break;
        }
        int8_t tempWidth = getSymbolWidth(tempSymbol);
        if (posX + tempWidth > DISPLAY_WIDTH || tempSymbol == '\n') {
            posX = 0;
            posY += 1;
        }
        if (tempSymbol != '\n') {
            displaySymbol(posX, posY, tempSymbol);
            posX += tempWidth;
        }
        index += 1;
    }
}

static void displayTextFromProgMem(int8_t posX, int8_t posY, const int8_t *text) {
    int8_t tempBuffer[getProgMemTextLength(text) + 1];
    readProgMemText(tempBuffer, text);
    displayText(posX, posY, tempBuffer);
}

static void displayStringAllocation(int8_t posX, int8_t posY, int8_t *string) {
    int8_t *tempString = *(int8_t **)string;
    displayText(posX, posY, tempString + STRING_DATA_OFFSET);
}

static int8_t displayValue(int8_t posX, int8_t posY, value_t *value) {
    if (value->type == VALUE_TYPE_NUMBER) {
        int8_t tempBuffer[20];
        sprintf(tempBuffer, "%f", *(float *)&(value->data));
        displayText(posX, posY, tempBuffer);
        return true;
    } else if (value->type == VALUE_TYPE_STRING) {
        int8_t *tempString = *(int8_t **)&(value->data);
        displayStringAllocation(posX, posY, tempString);
        return true;
    } else {
        return false;
    }
}

static void clearDisplay() {
    int32_t tempPosY = 0;
    while (tempPosY < DISPLAY_HEIGHT) {
        int32_t tempPosX = 0;
        while (tempPosX < DISPLAY_WIDTH) {
            displayCharacter(tempPosX, tempPosY, ' ');
            tempPosX += 1;
        }
        tempPosY += 1;
    }
}

static void clearDisplayRegion(int8_t posX, int8_t posY, int8_t width) {
    int8_t tempOffset = 0;
    while (tempOffset < width) {
        displayCharacter(posX + tempOffset, posY, ' ');
        tempOffset += 1;
    }
}

static int8_t *seekTextRow(int8_t *text, int16_t posY) {
    int8_t *output = text;
    int8_t tempPosX = 0;
    int16_t tempPosY = 0;
    int16_t index = 0;
    while (tempPosY < posY) {
        uint8_t tempSymbol = text[index];
        if (tempSymbol == 0) {
            break;
        }
        int8_t tempWidth = getSymbolWidth(tempSymbol);
        if (tempPosX + tempWidth > DISPLAY_WIDTH || tempSymbol == '\n') {
            tempPosX = 0;
            tempPosY += 1;
            output = text + index;
            if (tempSymbol == '\n') {
                output += 1;
            }
        }
        if (tempSymbol != '\n') {
            tempPosX += tempWidth;
        }
        index += 1;
    }
    return output;
}

static int8_t printText(int8_t *text) {
    int16_t tempPosY = 0;
    int8_t *tempText = text;
    clearDisplay();
    displayText(0, 0, tempText);
    while (true) {
        int8_t tempKey = getKey();
        int16_t tempNextPosY = tempPosY;
        if (tempKey == KEY_CURSOR_UP) {
            tempNextPosY -= 1;
        }
        if (tempKey == KEY_CURSOR_DOWN) {
            tempNextPosY += 1;
        }
        if (tempKey == KEY_SELECT_OPTION) {
            return true;
        }
        if (tempKey == KEY_ESCAPE) {
            return false;
        }
        if (tempNextPosY != tempPosY) {
            int8_t *tempNextText = seekTextRow(text, tempNextPosY);
            if (tempNextText != tempText) {
                tempText = tempNextText;
                tempPosY = tempNextPosY;
                clearDisplay();
                displayText(0, 0, tempText);
            }
        }
    }
}

static int8_t printTextFromProgMem(const int8_t *text) {
    int8_t tempBuffer[getProgMemTextLength(text) + 1];
    readProgMemText(tempBuffer, text);
    return printText(tempBuffer);
}

static int8_t printStringAllocation(int8_t *string) {
    int8_t *tempString = *(int8_t **)string;
    return printText(tempString + STRING_DATA_OFFSET);
}

static int8_t printValue(value_t *value) {
    if (value->type == VALUE_TYPE_NUMBER) {
        int8_t tempBuffer[20];
        sprintf(tempBuffer, "%f", *(float *)&(value->data));
        printText(tempBuffer);
        return true;
    } else if (value->type == VALUE_TYPE_STRING) {
        int8_t *tempString = *(int8_t **)&(value->data);
        printStringAllocation(tempString);
        return true;
    } else {
        return false;
    }
}

static int8_t menu(int8_t *title, int8_t *optionList) {
    int8_t *tempList = *(int8_t **)optionList;
    int16_t tempLength = *(int16_t *)(tempList + LIST_LENGTH_OFFSET);
    clearDisplay();
    displayStringAllocation(0, 0, title);
    displayCharacter(0, 1, '*');
    value_t *tempValue = (value_t *)(tempList + LIST_DATA_OFFSET);
    displayValue(1, 1, tempValue);
    int8_t index = 0;
    while (true) {
        int8_t tempKey = getKey();
        int16_t tempNextIndex = index;
        if (tempKey == KEY_CURSOR_UP) {
            tempNextIndex -= 1;
            if (tempNextIndex < 0) {
                tempNextIndex = tempLength - 1;
            }
        }
        if (tempKey == KEY_CURSOR_DOWN) {
            tempNextIndex += 1;
            if (tempNextIndex >= tempLength) {
                tempNextIndex = 0;
            }
        }
        if (tempKey == KEY_SELECT_OPTION) {
            return index;
        }
        if (tempKey == KEY_ESCAPE) {
            return -1;
        }
        if (tempNextIndex != index) {
            index = tempNextIndex;
            value_t *tempValue = (value_t *)(tempList + LIST_DATA_OFFSET + index * sizeof(value_t));
            clearDisplayRegion(1, 1, DISPLAY_WIDTH - 1);
            displayValue(1, 1, tempValue);
        }
    }
}

static int8_t menuWithOptionsFromProgMem(int8_t *title, const int8_t **optionList, int8_t optionAmount) {
    int8_t *tempList = createEmptyList(optionAmount);
    value_t *tempListContents = (value_t *)(*(int8_t **)tempList + LIST_DATA_OFFSET);
    int8_t index = 0;
    while (index < optionAmount) {
        int8_t *tempString = createStringFromProgMem(pgm_read_ptr((const void **)(optionList + index)));
        value_t *tempValue = tempListContents + index;
        tempValue->type = VALUE_TYPE_STRING;
        *(int8_t **)&(tempValue->data) = tempString;
        index += 1;
    }
    int8_t output = menu(title, tempList);
    index = 0;
    while (index < optionAmount) {
        value_t *tempValue = tempListContents + index;
        int8_t *tempString = *(int8_t **)&(tempValue->data);
        deallocate(tempString);
        index += 1;
    }
    deallocate(tempList);
    return output;
}

static int8_t menuWithTitleFromProgMem(const int8_t *title, int8_t *optionList) {
    int8_t *tempTitle = createStringFromProgMem(title);
    int8_t output = menu(tempTitle, optionList);
    deallocate(tempTitle);
    return output;
}

static int8_t menuFromProgMem(const int8_t *title, const int8_t **optionList, int8_t optionAmount) {
    int8_t *tempTitle = createStringFromProgMem(title);
    int8_t output = menuWithOptionsFromProgMem(tempTitle, optionList, optionAmount);
    deallocate(tempTitle);
    return output;
}

static void displayTextEditorLine() {
    clearDisplayRegion(0, 0, DISPLAY_WIDTH);
    int8_t tempCursorPosX = DISPLAY_WIDTH / 2;
    displayCharacter(tempCursorPosX, 0, '_');
    int8_t tempIndex = textEditorIndex;
    int8_t tempPosX = tempCursorPosX + 1;
    while (true) {
        if (tempPosX >= DISPLAY_WIDTH) {
            break;
        }
        uint8_t tempSymbol = textEditorText[tempIndex];
        if (tempSymbol == '\n' || tempSymbol == 0) {
            break;
        }
        displaySymbol(tempPosX, 0, tempSymbol);
        tempIndex += 1;
        tempPosX += getSymbolWidth(tempSymbol);
    }
    tempIndex = textEditorIndex - 1;
    tempPosX = tempCursorPosX;
    while (true) {
        if (tempIndex < 0) {
            break;
        }
        uint8_t tempSymbol = textEditorText[tempIndex];
        if (tempSymbol == '\n') {
            break;
        }
        tempIndex -= 1;
        tempPosX -= getSymbolWidth(tempSymbol);
        displaySymbol(tempPosX, 0, tempSymbol);
        if (tempPosX <= 0) {
            break;
        }
    }
}

static void runTextEditor() {
    displayTextEditorLine();
    while (true) {
        int8_t tempKey = getKey();
        int8_t shouldDisplayTextLine = false;
        if (tempKey == KEY_CURSOR_LEFT) {
            if (textEditorIndex > 0) {
                uint8_t tempSymbol = textEditorText[textEditorIndex - 1];
                if (tempSymbol != '\n') {
                    textEditorIndex -= 1;
                    shouldDisplayTextLine = true;
                }
            }
        }
        if (tempKey == KEY_CURSOR_RIGHT) {
            uint8_t tempSymbol = textEditorText[textEditorIndex];
            if (tempSymbol != '\n' && tempSymbol != 0) {
                textEditorIndex += 1;
                shouldDisplayTextLine = true;
            }
        }
        if (shouldDisplayTextLine) {
            displayTextEditorLine();
        }
    }
}

static void initializeTextEditor(int8_t *text) {
    textEditorText = text;
    textEditorIndex = 0;
}

int main(int argc, const char *argv[]) {
    
    // TEST CODE.
    /*
    int8_t *tempAllocation1 = allocate(30, 0);
    int8_t *tempAllocation2 = allocate(30, 0);
    int8_t *tempAllocation3 = allocate(30, 0);
    deallocate(tempAllocation2);
    int8_t *tempAllocation4 = allocate(40, 0);
    printf("%ld\n", tempAllocation1 - memory);
    printf("%ld\n", tempAllocation2 - memory);
    printf("%ld\n", tempAllocation3 - memory);
    printf("%ld\n", tempAllocation4 - memory);
    return 0;
    */
    
    int8_t index = 0;
    while (index < DISPLAY_WIDTH * DISPLAY_HEIGHT) {
        displayBuffer[index] = ' ';
        index += 1;
    }
    
    window = initscr();
    noecho();
    curs_set(0);
    keypad(window, true);
    ESCDELAY = 50;
    start_color();
    init_pair(BLACK_ON_WHITE, COLOR_BLACK, COLOR_WHITE);
    init_pair(WHITE_ON_CYAN, COLOR_WHITE, COLOR_CYAN);
    handleResize();
    
    //menuFromProgMem(TEST_MESSAGE_1, TEST_MESSAGE_LIST, sizeof(TEST_MESSAGE_LIST) / sizeof(*TEST_MESSAGE_LIST));
    int8_t tempText[100] = "HELLO\nWORLD";
    initializeTextEditor(tempText);
    runTextEditor();
    
    endwin();
    
}

