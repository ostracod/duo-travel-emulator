
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <curses.h>

#define BLACK_ON_WHITE 1
#define WHITE_ON_CYAN 2
#define DISPLAY_OFFSET_X 2
#define DISPLAY_OFFSET_Y 3

#define true 1
#define false 0
#define PROGMEM

#define IS_EMULATOR

#define DISPLAY_WIDTH 16
#define DISPLAY_HEIGHT 2
#define SYMBOL_SET_NAME_WIDTH 5

#define STORAGE_SIZE ((int32_t)256 * (int32_t)128)

#define KEY_CURSOR_LEFT 0
#define KEY_CURSOR_RIGHT 1
#define KEY_CURSOR_UP 2
#define KEY_CURSOR_DOWN 3
#define KEY_SYMBOL_LEFT 4
#define KEY_SYMBOL_RIGHT 5
#define KEY_SYMBOL_UP 6
#define KEY_SYMBOL_DOWN 7
#define KEY_SELECT_OPTION 8
#define KEY_DELETE 9
#define KEY_NEWLINE 10
#define KEY_ESCAPE 11
#define KEY_FINISH 12
#define KEY_CLEAR_LINE 13

#define ALLOCATION_PREVIOUS_OFFSET sizeof(int8_t *)
#define ALLOCATION_NEXT_OFFSET (ALLOCATION_PREVIOUS_OFFSET + sizeof(int8_t *))
#define ALLOCATION_SIZE_OFFSET (ALLOCATION_NEXT_OFFSET + 2)
#define ALLOCATION_TYPE_OFFSET (ALLOCATION_SIZE_OFFSET + 1)
#define ALLOCATION_IS_REACHABLE_OFFSET (ALLOCATION_TYPE_OFFSET + 1)
#define ALLOCATION_HEADER_SIZE ALLOCATION_IS_REACHABLE_OFFSET
#define HEAP_START_ADDRESS (memory + sizeof(memory))

#define ALLOCATION_TYPE_POINTER 1
#define ALLOCATION_TYPE_STRING 2
#define ALLOCATION_TYPE_LIST 3

#define STRING_LENGTH_OFFSET 0
#define STRING_DATA_OFFSET (STRING_LENGTH_OFFSET + 2)

#define LIST_LENGTH_OFFSET 0
#define LIST_DATA_OFFSET (STRING_LENGTH_OFFSET + 2)

#define VALUE_TYPE_MISSING 0
#define VALUE_TYPE_NUMBER 1
#define VALUE_TYPE_STRING 2
#define VALUE_TYPE_LIST 3
#define VALUE_TYPE_FUNCTION 4

#define FILE_NAME_MAXIMUM_LENGTH 15
#define FILE_MAXIMUM_SIZE 1000
#define FILE_EXISTS_TRUE 1
#define FILE_EXISTS_FALSE 255

#define FILE_ENTRY_SIZE 1024
#define FILE_EXISTS_OFFSET 0
#define FILE_NAME_OFFSET (FILE_EXISTS_OFFSET + 1)
#define FILE_SIZE_OFFSET (FILE_NAME_OFFSET + FILE_NAME_MAXIMUM_LENGTH + 1)
#define FILE_DATA_OFFSET (FILE_SIZE_OFFSET + 2)

#define DESTINATION_TYPE_VALUE 0
#define DESTINATION_TYPE_SYMBOL 1

#define EVALUATION_STATUS_NORMAL 0
#define EVALUATION_STATUS_QUIT 1
#define EVALUATION_STATUS_RETURN 2

#define SCOPE_SIZE_OFFSET 0
#define SCOPE_VARIABLE_OFFSET (SCOPE_SIZE_OFFSET + 2)
#define SCOPE_BRANCH_OFFSET (SCOPE_VARIABLE_OFFSET + sizeof(int8_t *))
#define SCOPE_DATA_OFFSET (SCOPE_BRANCH_OFFSET + sizeof(int8_t *))

#define NUMBER_LITERAL_MAXIMUM_LENGTH 20
#define VARIABLE_NAME_MAXIMUM_LENGTH 15

#define VARIABLE_NEXT_OFFSET 0
#define VARIABLE_VALUE_OFFSET (VARIABLE_NEXT_OFFSET + sizeof(int8_t *))
#define VARIABLE_NAME_OFFSET (VARIABLE_VALUE_OFFSET + sizeof(value_t))

#define BRANCH_ACTION_RUN 0
#define BRANCH_ACTION_IGNORE_SOFT 1
#define BRANCH_ACTION_IGNORE_HARD 2
#define BRANCH_ACTION_LOOP 3

#define TREASURE_TYPE_VALUE 0
#define TREASURE_TYPE_VALUE_ARRAY 1
#define TREASURE_TYPE_SCOPE 2

#define REQUEST_STRING_MAXIMUM_LENGTH 100
#define REQUEST_NUMBER_MAXIMUM_LENGTH 20

#define MENU_RESULT_ESCAPE -1
#define MENU_RESULT_ERROR -2

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
const int8_t SYMBOL_TEXT_EQUAL_REFERENCE[] PROGMEM = "equRef:";

const int8_t SYMBOL_TEXT_PRINT[] PROGMEM = "print:";
const int8_t SYMBOL_TEXT_REQUEST_STRING[] PROGMEM = "reqStr;";
const int8_t SYMBOL_TEXT_REQUEST_NUMBER[] PROGMEM = "reqNum;";
const int8_t SYMBOL_TEXT_MENU[] PROGMEM = "menu:";
const int8_t SYMBOL_TEXT_FILE_EXISTS[] PROGMEM = "fExists:";
const int8_t SYMBOL_TEXT_FILE_SIZE[] PROGMEM = "fSize:";
const int8_t SYMBOL_TEXT_FILE_CREATE[] PROGMEM = "fCreate:";
const int8_t SYMBOL_TEXT_FILE_DELETE[] PROGMEM = "fDelete:";
const int8_t SYMBOL_TEXT_FILE_SET_NAME[] PROGMEM = "fSetName:";
const int8_t SYMBOL_TEXT_FILE_READ[] PROGMEM = "fRead:";
const int8_t SYMBOL_TEXT_FILE_WRITE[] PROGMEM = "fWrite:";
const int8_t SYMBOL_TEXT_FILE_IMPORT[] PROGMEM = "fImport:";

const int8_t * const SYMBOL_TEXT_LIST[] PROGMEM = {
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
    SYMBOL_TEXT_EQUAL_REFERENCE,
    SYMBOL_TEXT_PRINT,
    SYMBOL_TEXT_REQUEST_STRING,
    SYMBOL_TEXT_REQUEST_NUMBER,
    SYMBOL_TEXT_MENU,
    SYMBOL_TEXT_FILE_EXISTS,
    SYMBOL_TEXT_FILE_SIZE,
    SYMBOL_TEXT_FILE_CREATE,
    SYMBOL_TEXT_FILE_DELETE,
    SYMBOL_TEXT_FILE_SET_NAME,
    SYMBOL_TEXT_FILE_READ,
    SYMBOL_TEXT_FILE_WRITE,
    SYMBOL_TEXT_FILE_IMPORT
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
#define SYMBOL_EQUAL_REFERENCE 185
#define SYMBOL_PRINT 186
#define SYMBOL_REQUEST_STRING 187
#define SYMBOL_REQUEST_NUMBER 188
#define SYMBOL_MENU 189
#define SYMBOL_FILE_EXISTS 190
#define SYMBOL_FILE_SIZE 191
#define SYMBOL_FILE_CREATE 192
#define SYMBOL_FILE_DELETE 193
#define SYMBOL_FILE_SET_NAME 194
#define SYMBOL_FILE_READ 195
#define SYMBOL_FILE_WRITE 196
#define SYMBOL_FILE_IMPORT 197

const int8_t UNARY_OPERATOR_LIST[] PROGMEM = {
    '-',
    SYMBOL_INCREMENT,
    SYMBOL_DECREMENT,
    '!',
    '~'
};

const int8_t BINARY_OPERATOR_LIST[] PROGMEM = {
    '*',
    '/',
    '%',
    '+',
    '-',
    SYMBOL_BITSHIFT_LEFT,
    SYMBOL_BITSHIFT_RIGHT,
    '>',
    '<',
    SYMBOL_GREATER_OR_EQUAL,
    SYMBOL_LESS_OR_EQUAL,
    SYMBOL_EQUAL,
    SYMBOL_NOT_EQUAL,
    '&',
    '^',
    '|',
    SYMBOL_BOOLEAN_AND,
    SYMBOL_BOOLEAN_XOR,
    SYMBOL_BOOLEAN_OR,
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

// All unary operators have a precedence of 0.

const int8_t BINARY_OPERATOR_PRECEDENCE_LIST[] PROGMEM = {
    1, // '*'
    1, // '/'
    1, // '%'
    2, // '+'
    2, // '-'
    3, // SYMBOL_BITSHIFT_LEFT
    3, // SYMBOL_BITSHIFT_RIGHT
    4, // '>'
    4, // '<'
    4, // SYMBOL_GREATER_OR_EQUAL
    4, // SYMBOL_LESS_OR_EQUAL
    5, // SYMBOL_EQUAL
    5, // SYMBOL_NOT_EQUAL
    6, // '&'
    7, // '^'
    8, // '|'
    9, // SYMBOL_BOOLEAN_AND
    10, // SYMBOL_BOOLEAN_XOR
    11, // SYMBOL_BOOLEAN_OR
    12, // '='
    12, // SYMBOL_ADD_ASSIGN
    12, // SYMBOL_SUBTRACT_ASSIGN
    12, // SYMBOL_MULTIPLY_ASSIGN
    12, // SYMBOL_DIVIDE_ASSIGN
    12, // SYMBOL_MODULUS_ASSIGN
    12, // SYMBOL_BOOLEAN_AND_ASSIGN
    12, // SYMBOL_BOOLEAN_OR_ASSIGN
    12, // SYMBOL_BOOLEAN_XOR_ASSIGN
    12, // SYMBOL_BITWISE_AND_ASSIGN
    12, // SYMBOL_BITWISE_OR_ASSIGN
    12, // SYMBOL_BITWISE_XOR_ASSIGN
    12, // SYMBOL_BITSHIFT_LEFT_ASSIGN
    12 // SYMBOL_BITSHIFT_RIGHT_ASSIGN
};

#define FIRST_FUNCTION_SYMBOL SYMBOL_IF
#define LAST_FUNCTION_SYMBOL SYMBOL_FILE_IMPORT

const int8_t FUNCTION_ARGUMENT_AMOUNT_LIST[] PROGMEM = {
    1, // SYMBOL_IF
    1, // SYMBOL_ELSE_IF
    0, // SYMBOL_ELSE
    0, // SYMBOL_END
    1, // SYMBOL_WHILE
    0, // SYMBOL_BREAK
    0, // SYMBOL_CONTINUE
    -1, // SYMBOL_FUNCTION
    1, // SYMBOL_RETURN_WITH_VALUE
    0, // SYMBOL_RETURN
    0, // SYMBOL_QUIT
    0, // SYMBOL_RANDOM
    2, // SYMBOL_RANDOM_INTEGER
    1, // SYMBOL_ABSOLUTE_VALUE
    1, // SYMBOL_ROUND
    1, // SYMBOL_FLOOR
    1, // SYMBOL_CEILING
    1, // SYMBOL_SINE
    1, // SYMBOL_COSINE
    1, // SYMBOL_TANGENT
    1, // SYMBOL_SQUARE_ROOT
    2, // SYMBOL_POWER
    2, // SYMBOL_LOG
    1, // SYMBOL_NUMBER
    1, // SYMBOL_STRING
    1, // SYMBOL_TYPE
    1, // SYMBOL_LENGTH
    1, // SYMBOL_COPY
    3, // SYMBOL_INSERT
    2, // SYMBOL_REMOVE
    3, // SYMBOL_SUBSEQUENCE
    3, // SYMBOL_INSERT_SUBSEQUENCE
    3, // SYMBOL_REMOVE_SUBSEQUENCE
    2, // SYMBOL_EQUAL_REFERENCE
    1, // SYMBOL_PRINT
    0, // SYMBOL_REQUEST_STRING
    0, // SYMBOL_REQUEST_NUMBER
    2, // SYMBOL_MENU
    1, // SYMBOL_FILE_EXISTS
    1, // SYMBOL_FILE_SIZE
    1, // SYMBOL_FILE_CREATE
    1, // SYMBOL_FILE_DELETE
    2, // SYMBOL_FILE_SET_NAME
    3, // SYMBOL_FILE_READ
    2, // SYMBOL_FILE_WRITE
    1 // SYMBOL_FILE_IMPORT
};

const uint8_t SYMBOL_SET_LETTERS[] PROGMEM = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
    'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
    'U', 'V', 'W', 'X', 'Y', 'Z'
};

const uint8_t SYMBOL_SET_NUMBERS[] PROGMEM = {
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
    '.', '-', 'e'
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
    SYMBOL_MENU,
    SYMBOL_FILE_EXISTS,
    SYMBOL_FILE_SIZE,
    SYMBOL_FILE_CREATE,
    SYMBOL_FILE_DELETE,
    SYMBOL_FILE_SET_NAME,
    SYMBOL_FILE_READ,
    SYMBOL_FILE_WRITE,
    SYMBOL_FILE_IMPORT
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
    SYMBOL_TYPE,
    SYMBOL_EQUAL_REFERENCE
};

const int8_t * const SYMBOL_SET_LIST[] PROGMEM = {
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

const int8_t SYMBOL_SET_NAME_LETTERS[] PROGMEM = "Lett";
const int8_t SYMBOL_SET_NAME_NUMBERS[] PROGMEM = "Num";
const int8_t SYMBOL_SET_NAME_PUNCTUATION[] PROGMEM = "Punc";
const int8_t SYMBOL_SET_NAME_OPERATORS[] PROGMEM = "Oper";
const int8_t SYMBOL_SET_NAME_ASSIGNMENT[] PROGMEM = "Asgn";
const int8_t SYMBOL_SET_NAME_CONTROL[] PROGMEM = "Ctrl";
const int8_t SYMBOL_SET_NAME_MATH[] PROGMEM = "Math";
const int8_t SYMBOL_SET_NAME_INPUT_OUTPUT[] PROGMEM = "I/O";
const int8_t SYMBOL_SET_NAME_VALUE[] PROGMEM = "Val";

const int8_t * const SYMBOL_SET_NAME_LIST[] PROGMEM = {
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

#define SYMBOL_SET_AMOUNT (sizeof(SYMBOL_SET_LIST) / sizeof(*SYMBOL_SET_LIST))

const int8_t SYMBOL_REPRESENTATION_SPACE[] PROGMEM = "(Space)";

const int8_t MENU_TITLE_FILE_LIST[] PROGMEM = "File List";
const int8_t MENU_TITLE_TEXT_EDITOR[] PROGMEM = "Text Editor";
const int8_t MENU_TITLE_FILE_DELETE[] PROGMEM = "Are you sure?";

const int8_t MENU_OPTION_CREATE_FILE[] PROGMEM = "Create file";
const int8_t MENU_OPTION_RUN[] PROGMEM = "Run";
const int8_t MENU_OPTION_EDIT[] PROGMEM = "Edit";
const int8_t MENU_OPTION_RENAME[] PROGMEM = "Rename";
const int8_t MENU_OPTION_DELETE[] PROGMEM = "Delete";
const int8_t MENU_OPTION_SAVE[] PROGMEM = "Save";
const int8_t MENU_OPTION_QUIT[] PROGMEM = "Quit";
const int8_t MENU_OPTION_YES_DELETE[] PROGMEM = "Yes delete";
const int8_t MENU_OPTION_NO_DELETE[] PROGMEM = "Do not delete";

const int8_t * const MENU_FILE[] PROGMEM = {
    MENU_OPTION_RUN,
    MENU_OPTION_EDIT,
    MENU_OPTION_RENAME,
    MENU_OPTION_DELETE
};

const int8_t * const MENU_TEXT_EDITOR[] PROGMEM = {
    MENU_OPTION_SAVE,
    MENU_OPTION_QUIT
};

const int8_t * const MENU_FILE_DELETE[] PROGMEM = {
    MENU_OPTION_NO_DELETE,
    MENU_OPTION_YES_DELETE
};

const int8_t MESSAGE_ENTER_NAME[] PROGMEM = "Enter file name.";
const int8_t MESSAGE_FILE_CREATED[] PROGMEM = "Created file.";
const int8_t MESSAGE_ENTER_NEW_NAME[] PROGMEM = "Enter new name.";
const int8_t MESSAGE_FILE_RENAMED[] PROGMEM = "Renamed file.";
const int8_t MESSAGE_FILE_DELETED[] PROGMEM = "Deleted file.";
const int8_t MESSAGE_SAVING[] PROGMEM = "Saving...";
const int8_t MESSAGE_FILE_SAVED[] PROGMEM = "Saved file.";
const int8_t MESSAGE_RUNNING[] PROGMEM = "Running...";
const int8_t MESSAGE_WELCOME[] PROGMEM = "DUO Travel OS\nVersion 1.0";

const int8_t ERROR_MESSAGE_BAD_START_OF_EXPRESSION[] PROGMEM = "ERROR: Bad\nstart of\nexpression.";
const int8_t ERROR_MESSAGE_BAD_END_STATEMENT[] PROGMEM = "ERROR: Bad\nend statement.";
const int8_t ERROR_MESSAGE_BAD_CONTINUE_STATEMENT[] PROGMEM = "ERROR: Bad\ncontinue\nstatement.";
const int8_t ERROR_MESSAGE_BAD_BREAK_STATEMENT[] PROGMEM = "ERROR: Bad\nbreak statement.";
const int8_t ERROR_MESSAGE_BAD_ARGUMENT_TYPE[] PROGMEM = "ERROR: Bad\nargument type.";
const int8_t ERROR_MESSAGE_MISSING_APOSTROPHE[] PROGMEM = "ERROR: Missing\napostrophe.";
const int8_t ERROR_MESSAGE_MISSING_QUOTATION_MARK[] PROGMEM = "ERROR: Missing\nquotation mark.";
const int8_t ERROR_MESSAGE_MISSING_BRACKET[] PROGMEM = "ERROR: Missing\nbracket.";
const int8_t ERROR_MESSAGE_MISSING_PARENTHESIS[] PROGMEM = "ERROR: Missing\nparenthesis.";
const int8_t ERROR_MESSAGE_MISSING_COMMA[] PROGMEM = "ERROR: Missing\ncomma.";
const int8_t ERROR_MESSAGE_STACK_HEAP_COLLISION[] PROGMEM = "ERROR: Stack-\nheap collision.";
const int8_t ERROR_MESSAGE_BAD_INDEX[] PROGMEM = "ERROR: Bad\nindex.";
const int8_t ERROR_MESSAGE_MISSING_FILE[] PROGMEM = "ERROR: Missing\nfile.";
const int8_t ERROR_MESSAGE_NAME_IS_TOO_LONG[] PROGMEM = "ERROR: Name is\ntoo long.";
const int8_t ERROR_MESSAGE_STORAGE_IS_FULL[] PROGMEM = "ERROR: Storage\nis full.";
const int8_t ERROR_MESSAGE_BAD_AMOUNT[] PROGMEM = "ERROR: Bad\namount.";
const int8_t ERROR_MESSAGE_FILE_EXISTS[] PROGMEM = "ERROR: File\nexists.";
const int8_t ERROR_MESSAGE_BAD_DESTINATION[] PROGMEM = "ERROR: Bad\ndestination.";
const int8_t ERROR_MESSAGE_BAD_OPERAND_TYPE[] PROGMEM = "ERROR: Bad\noperand type.";
const int8_t ERROR_MESSAGE_DIVIDE_BY_ZERO[] PROGMEM = "ERROR: Divide\nby zero.";
const int8_t ERROR_MESSAGE_BAD_VALUE[] PROGMEM = "ERROR: Bad\nvalue.";
const int8_t ERROR_MESSAGE_NOT_TOP_LEVEL[] PROGMEM = "ERROR: Not\ntop level.";
const int8_t ERROR_MESSAGE_MISSING_VALUE[] PROGMEM = "ERROR: Missing\nvalue.";
const int8_t ERROR_MESSAGE_NUMBER_LITERAL_TOO_LONG[] PROGMEM = "ERROR: Number\nliteral too\nlong.";
const int8_t ERROR_MESSAGE_VARIABLE_NAME_TOO_LONG[] PROGMEM = "ERROR: Variable\nname too long.";
const int8_t ERROR_MESSAGE_EXPRESSION_TOO_COMPLEX[] PROGMEM = "ERROR:\nExpression too\ncomplex.";

typedef struct value {
    int8_t type;
    int8_t data[sizeof(int8_t *) > sizeof(float) ? sizeof(int8_t *) : sizeof(float)];
} value_t;

typedef struct expressionResult {
    int8_t status;
    int8_t destinationType;
    int8_t *destination;
    value_t value;
    int32_t nextCode;
} expressionResult_t;

typedef struct branch branch_t;

typedef struct branch {
    branch_t *previous;
    branch_t *next;
    int8_t action;
    int32_t address;
} branch_t;

typedef struct treasureTracker treasureTracker_t;

// "Treasure" is a term I made up.
// It is the opposite of garbage.
// In other words, treasure is reachable, active memory.

typedef struct treasureTracker {
    treasureTracker_t *next;
    int8_t type;
    int8_t amount;
    void *treasure;
} treasureTracker_t;

WINDOW *window;
int32_t windowWidth;
int32_t windowHeight;
int8_t displayBuffer[DISPLAY_WIDTH * DISPLAY_HEIGHT];
int8_t storageFilePath[] = "./storage.dat";
FILE *storageFile;
int8_t isTesting = false;
uint8_t testStorage[STORAGE_SIZE];
int8_t *testOutput = NULL;
int8_t testIsFinished = false;
int8_t testSuiteIsFinished = false;
int8_t testSuiteFilePath[] = "./testSuite.txt";
FILE *testSuiteFile;
int32_t testProgramFile;
int8_t testHasFailed;
int8_t testSuiteHasFailed;
int32_t testCount;
int32_t testAssertionCount;

int8_t memory[1200];
int8_t *firstAllocation = NULL;
int8_t *lastAllocation = NULL;
treasureTracker_t *firstTreasureTracker = NULL;
int8_t *textEditorText;
int16_t textEditorIndex;
int16_t textEditorLength;
int16_t textEditorMaximumLength;
int8_t textEditorSymbolSetIndex;
int8_t textEditorSymbolIndex[SYMBOL_SET_AMOUNT];
int8_t textEditorNumberOnly;
int8_t *globalScope = NULL;
int8_t *localScope = NULL;
int16_t commandsSinceMarkAndSweep = 0;
int16_t allocationsSinceMarkAndSweep = 0;
const int8_t *errorMessage = NULL;
int32_t errorCode;

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

int8_t nativeStackHasCollision(int16_t offset) {
    return false;
}

void displayCharacter(int8_t posX, int8_t posY, int8_t character) {
    if (isTesting) {
        return;
    }
    if (posX < 0 || posX >= DISPLAY_WIDTH || posY < 0 || posY >= DISPLAY_HEIGHT) {
        return;
    }
    attron(COLOR_PAIR(BLACK_ON_WHITE));
    mvaddch(posY + DISPLAY_OFFSET_Y, posX + DISPLAY_OFFSET_X, character);
    attroff(COLOR_PAIR(BLACK_ON_WHITE));
    displayBuffer[posX + posY * DISPLAY_WIDTH] = character;
}

void handleResize();
int8_t getTestKey();

int8_t getKey() {
    if (isTesting) {
        return getTestKey();
    }
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
            return KEY_SYMBOL_LEFT;
        }
        if (tempKey == 'd') {
            return KEY_SYMBOL_RIGHT;
        }
        if (tempKey == 'w') {
            return KEY_SYMBOL_UP;
        }
        if (tempKey == 's') {
            return KEY_SYMBOL_DOWN;
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
        if (tempKey == 'f') {
            return KEY_FINISH;
        }
        if (tempKey == 'c') {
            return KEY_CLEAR_LINE;
        }
    }
}

void drawDisplayBuffer() {
    if (isTesting) {
        return;
    }
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
    if (isTesting) {
        return;
    }
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
    if (isTesting) {
        return;
    }
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

void readStorage(void *destination, int32_t address, int32_t amount) {
    if (isTesting) {
        memcpy(destination, testStorage + address, amount);
        return;
    }
    fseek(storageFile, address, SEEK_SET);
    fread(destination, 1, amount, storageFile);
}

void writeStorage(int32_t address, void *source, int32_t amount) {
    if (isTesting) {
        memcpy(testStorage + address, source, amount);
        return;
    }
    fseek(storageFile, address, SEEK_SET);
    fwrite(source, 1, amount, storageFile);
}

int8_t readStorageInt8(int32_t address) {
    int8_t output;
    readStorage(&output, address, 1);
    return output;
}

void dumpMemory() {
    FILE *tempFile = fopen("./memoryDump.dat", "w");
    fwrite(memory, 1, sizeof(memory), tempFile);
    fclose(tempFile);
}

void scrambleMemory() {
    int8_t garbage[20000];
    int16_t index = 0;
    while (index < sizeof(garbage)) {
        garbage[index] = rand() % 100;
        index += 1;
    }
    index = 0;
    while (index < sizeof(garbage)) {
        mvprintw(0, 0, "%d", garbage[index]);
        index += 1;
    }
}

static void convertFloatToText(int8_t *destination, float number) {
    float tempNumber;
    if (number < 0.0) {
        tempNumber = -number;
    } else {
        tempNumber = number;
    }
    if ((tempNumber > 10000000.0 || tempNumber < 0.001) && tempNumber != 0.0) {
        int8_t tempExponent = 0;
        while (tempNumber >= 10.0) {
            tempNumber /= 10.0;
            tempExponent += 1;
        }
        while (tempNumber < 1.0) {
            tempNumber *= 10.0;
            tempExponent -= 1;
        }
        if (number < 0.0) {
            tempNumber = -tempNumber;
        }
        sprintf(destination, "%.4fe%d", tempNumber, tempExponent);
    } else {
        sprintf(destination, "%.4f", number);
        int8_t tempHasDecimalPoint = false;
        int8_t index = 0;
        while (true) {
            int8_t tempCharacter = destination[index];
            if (tempCharacter == '.') {
                tempHasDecimalPoint = true;
            } else if (!((tempCharacter >= '0' && tempCharacter <= '9') || tempCharacter == '-')) {
                break;
            }
            index += 1;
        }
        if (!tempHasDecimalPoint) {
            return;
        }
        while (index > 0) {
            int8_t tempCharacter = destination[index - 1];
            if (tempCharacter >= '1' && tempCharacter <= '9') {
                break;
            }
            index -= 1;
            if (tempCharacter == '.') {
                break;
            }
        }
        destination[index] = 0;
    }
}

static float convertTextToFloat(int8_t *text) {
    return atof(text);
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

static int8_t *getStackBoundary() {
    if (localScope == NULL) {
        return memory;
    }
    int16_t tempSize = *(int16_t *)(localScope + SCOPE_SIZE_OFFSET);
    return localScope + SCOPE_DATA_OFFSET + tempSize;
}

static int8_t *getHeapBoundary() {
    if (lastAllocation == NULL) {
        return memory + sizeof(memory);
    }
    return lastAllocation - ALLOCATION_HEADER_SIZE;
}

static void resetHeap() {
    firstAllocation = NULL;
    lastAllocation = NULL;
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
        int16_t tempGapSize = tempAddress - (tempNextAllocation + tempAllocationSize);
        if (tempGapSize >= size + ALLOCATION_HEADER_SIZE) {
            output = tempAddress - size;
            break;
        }
        tempPreviousAllocation = tempNextAllocation;
        tempNextAllocation = *(int8_t **)(tempPreviousAllocation - ALLOCATION_NEXT_OFFSET);
    }
    if (getStackBoundary() > output - ALLOCATION_HEADER_SIZE) {
        return NULL;
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
    if (tempNextAllocation == NULL) {
        lastAllocation = output;
    } else {
        *(int8_t **)(tempNextAllocation - ALLOCATION_PREVIOUS_OFFSET) = output;
    }
    allocationsSinceMarkAndSweep += 1;
    return output;
}

static void deallocate(int8_t *allocation) {
    int8_t *tempPreviousAllocation = *(int8_t **)(allocation - ALLOCATION_PREVIOUS_OFFSET);
    int8_t *tempNextAllocation = *(int8_t **)(allocation - ALLOCATION_NEXT_OFFSET);
    if (tempPreviousAllocation == NULL) {
        firstAllocation = tempNextAllocation;
    } else {
        *(int8_t **)(tempPreviousAllocation - ALLOCATION_NEXT_OFFSET) = tempNextAllocation;
    }
    if (tempNextAllocation == NULL) {
        lastAllocation = tempPreviousAllocation;
    } else {
        *(int8_t **)(tempNextAllocation - ALLOCATION_PREVIOUS_OFFSET) = tempPreviousAllocation;
    }
}

static void deallocatePointer(int8_t *allocation) {
    deallocate(*(int8_t **)allocation);
    deallocate(allocation);
}

static int8_t *resizeAllocation(int8_t *allocation, int16_t size) {
    int16_t tempSize = *(int16_t *)(allocation - ALLOCATION_SIZE_OFFSET);
    int8_t tempType = *(int8_t *)(allocation - ALLOCATION_TYPE_OFFSET);
    int8_t *output;
    if (size > tempSize) {
        output = allocate(size, tempType);
        if (output == NULL) {
            return NULL;
        }
        memcpy(output, allocation, tempSize);
        deallocate(allocation);
    } else {
        output = allocation;
        *(int16_t *)(allocation - ALLOCATION_SIZE_OFFSET) = size;
    }
    return output;
}

static int8_t *createEmptyString(int16_t length) {
    int8_t *output = allocate(sizeof(int8_t *), ALLOCATION_TYPE_POINTER);
    if (output == NULL) {
        return NULL;
    }
    int8_t *tempString = allocate(STRING_DATA_OFFSET + length + 1, ALLOCATION_TYPE_STRING);
    if (tempString == NULL) {
        return NULL;
    }
    *(int8_t **)output = tempString;
    *(int16_t *)(tempString + STRING_LENGTH_OFFSET) = length;
    *(int8_t *)(tempString + STRING_DATA_OFFSET) = 0;
    return output;
}

static int8_t *createString(int8_t *text) {
    int8_t *output = createEmptyString(strlen(text));
    if (output == NULL) {
        return NULL;
    }
    int8_t *tempString = *(int8_t **)output;
    strcpy(tempString + STRING_DATA_OFFSET, text);
    return output;
}

static int8_t *createStringFromProgMem(const int8_t *text) {
    int8_t *output = createEmptyString(getProgMemTextLength(text));
    if (output == NULL) {
        return NULL;
    }
    int8_t *tempString = *(int8_t **)output;
    readProgMemText(tempString + STRING_DATA_OFFSET, text);
    return output;
}

static int8_t *resizeString(int8_t *string, int16_t length) {
    int8_t *tempString = *(int8_t **)string;
    int16_t tempSize = *(int16_t *)(tempString - ALLOCATION_SIZE_OFFSET);
    int16_t tempMinimumSize = STRING_DATA_OFFSET + length + 1;
    if (tempMinimumSize > tempSize || tempMinimumSize < tempSize / 4) {
        tempString = resizeAllocation(tempString, STRING_DATA_OFFSET + (length + 1) * 2);
        if (tempString == NULL) {
            return NULL;
        }
        *(int8_t **)string = tempString;
    }
    *(int16_t *)(tempString + STRING_LENGTH_OFFSET) = length;
    return tempString;
}

static int8_t *createEmptyList(int16_t length) {
    int8_t *output = allocate(sizeof(int8_t *), ALLOCATION_TYPE_POINTER);
    if (output == NULL) {
        return NULL;
    }
    int8_t *tempList = allocate(LIST_DATA_OFFSET + length * sizeof(value_t), ALLOCATION_TYPE_LIST);
    if (tempList == NULL) {
        return NULL;
    }
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

static int8_t *resizeList(int8_t *list, int16_t length) {
    int8_t *tempList = *(int8_t **)list;
    int16_t tempSize = *(int16_t *)(tempList - ALLOCATION_SIZE_OFFSET);
    int16_t tempMinimumSize = LIST_DATA_OFFSET + length * sizeof(value_t);
    if (tempMinimumSize > tempSize || tempMinimumSize < tempSize / 4) {
        tempList = resizeAllocation(tempList, LIST_DATA_OFFSET + length * sizeof(value_t) * 2);
        if (tempList == NULL) {
            return NULL;
        }
        *(int8_t **)list = tempList;
    }
    *(int16_t *)(tempList + LIST_LENGTH_OFFSET) = length;
    return tempList;
}

static int8_t insertListValue(int8_t *list, int16_t index, value_t *value) {
    int8_t *tempList = *(int8_t **)list;
    int16_t tempLength = *(int16_t *)(tempList + LIST_LENGTH_OFFSET);
    if (index < 0 || index > tempLength) {
        errorMessage = ERROR_MESSAGE_BAD_INDEX;
        return false;
    }
    tempList = resizeList(list, tempLength + 1);
    if (tempList == NULL) {
        errorMessage = ERROR_MESSAGE_STACK_HEAP_COLLISION;
        return false;
    }
    value_t *tempValue = (value_t *)(tempList + LIST_DATA_OFFSET + index * sizeof(value_t));
    memmove(tempValue + 1, tempValue, (tempLength - index) * sizeof(value_t));
    *tempValue = *value;
    return true;
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
        int8_t tempBuffer[NUMBER_LITERAL_MAXIMUM_LENGTH + 1];
        convertFloatToText(tempBuffer, *(float *)&(value->data));
        displayText(posX, posY, tempBuffer);
        return true;
    } else if (value->type == VALUE_TYPE_STRING) {
        int8_t *tempString = *(int8_t **)&(value->data);
        displayStringAllocation(posX, posY, tempString);
        return true;
    } else {
        errorMessage = ERROR_MESSAGE_BAD_ARGUMENT_TYPE;
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
    #ifdef IS_EMULATOR
        testOutput = text;
    #endif
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
        if (tempKey == KEY_SELECT_OPTION || tempKey == KEY_FINISH) {
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
        int8_t tempBuffer[NUMBER_LITERAL_MAXIMUM_LENGTH + 1];
        convertFloatToText(tempBuffer, *(float *)&(value->data));
        int8_t tempResult = printText(tempBuffer);
        return tempResult;
    } else if (value->type == VALUE_TYPE_STRING) {
        int8_t *tempString = *(int8_t **)&(value->data);
        int8_t tempResult = printStringAllocation(tempString);
        return tempResult;
    } else {
        errorMessage = ERROR_MESSAGE_BAD_ARGUMENT_TYPE;
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
    int8_t tempSuccess = displayValue(1, 1, tempValue);
    if (!tempSuccess) {
        return MENU_RESULT_ERROR;
    }
    int8_t index = 0;
    while (true) {
        int8_t tempKey = getKey();
        int16_t tempNextIndex = index;
        if (tempKey == KEY_CURSOR_UP || tempKey == KEY_CURSOR_LEFT) {
            tempNextIndex -= 1;
            if (tempNextIndex < 0) {
                tempNextIndex = tempLength - 1;
            }
        }
        if (tempKey == KEY_CURSOR_DOWN || tempKey == KEY_CURSOR_RIGHT) {
            tempNextIndex += 1;
            if (tempNextIndex >= tempLength) {
                tempNextIndex = 0;
            }
        }
        if (tempKey == KEY_SELECT_OPTION || tempKey == KEY_FINISH) {
            return index;
        }
        if (tempKey == KEY_ESCAPE) {
            return MENU_RESULT_ESCAPE;
        }
        if (tempNextIndex != index) {
            index = tempNextIndex;
            value_t *tempValue = (value_t *)(tempList + LIST_DATA_OFFSET + index * sizeof(value_t));
            clearDisplayRegion(1, 1, DISPLAY_WIDTH - 1);
            int8_t tempSuccess = displayValue(1, 1, tempValue);
            if (!tempSuccess) {
                return MENU_RESULT_ERROR;
            }
        }
    }
}

static int8_t menuWithOptionsFromProgMem(int8_t *title, const int8_t * const *optionList, int8_t optionAmount) {
    int8_t *tempList = createEmptyList(optionAmount);
    if (tempList == NULL) {
        return MENU_RESULT_ERROR;
    }
    value_t *tempListContents = (value_t *)(*(int8_t **)tempList + LIST_DATA_OFFSET);
    int8_t index = 0;
    while (index < optionAmount) {
        int8_t *tempString = createStringFromProgMem(pgm_read_ptr((const void **)(optionList + index)));
        if (tempString == NULL) {
            return MENU_RESULT_ERROR;
        }
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
        deallocatePointer(tempString);
        index += 1;
    }
    deallocatePointer(tempList);
    return output;
}

static int8_t menuWithTitleFromProgMem(const int8_t *title, int8_t *optionList) {
    int8_t *tempTitle = createStringFromProgMem(title);
    if (tempTitle == NULL) {
        return MENU_RESULT_ERROR;
    }
    int8_t output = menu(tempTitle, optionList);
    deallocatePointer(tempTitle);
    return output;
}

static int8_t menuFromProgMem(const int8_t *title, const int8_t * const *optionList, int8_t optionAmount) {
    int8_t *tempTitle = createStringFromProgMem(title);
    if (tempTitle == NULL) {
        return MENU_RESULT_ERROR;
    }
    int8_t output = menuWithOptionsFromProgMem(tempTitle, optionList, optionAmount);
    deallocatePointer(tempTitle);
    return output;
}

static void displayTextEditorLine() {
    clearDisplayRegion(0, 0, DISPLAY_WIDTH);
    int8_t tempCursorPosX = DISPLAY_WIDTH / 2;
    displayCharacter(tempCursorPosX, 0, '_');
    int16_t tempIndex = textEditorIndex;
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

static void displayTextEditorSymbolSet() {
    clearDisplayRegion(0, 1, SYMBOL_SET_NAME_WIDTH);
    displayTextFromProgMem(0, 1, pgm_read_ptr((const void **)(SYMBOL_SET_NAME_LIST + textEditorSymbolSetIndex)));
}

static void displayTextEditorSymbol() {
    clearDisplayRegion(SYMBOL_SET_NAME_WIDTH, 1, DISPLAY_WIDTH - SYMBOL_SET_NAME_WIDTH);
    const uint8_t *tempSymbolSet = pgm_read_ptr((const void **)(SYMBOL_SET_LIST + textEditorSymbolSetIndex));
    uint8_t tempSymbol = pgm_read_byte(tempSymbolSet + textEditorSymbolIndex[textEditorSymbolSetIndex]);
    if (tempSymbol == ' ') {
        displayTextFromProgMem(SYMBOL_SET_NAME_WIDTH, 1, SYMBOL_REPRESENTATION_SPACE);
    } else {
        displaySymbol(SYMBOL_SET_NAME_WIDTH, 1, tempSymbol);
    }
}

static int8_t insertTextEditorSymbol(uint8_t symbol) {
    if (textEditorLength >= textEditorMaximumLength) {
        return false;
    }
    int16_t index = textEditorIndex;
    uint8_t tempLastSymbol = textEditorText[index];
    while (true) {
        index += 1;
        int8_t tempNextSymbol = textEditorText[index];
        textEditorText[index] = tempLastSymbol;
        if (tempLastSymbol == 0) {
            break;
        }
        tempLastSymbol = tempNextSymbol;
    }
    textEditorText[textEditorIndex] = symbol;
    textEditorIndex += 1;
    textEditorLength += 1;
    return true;
}

static void clearTextEditorLine() {
    int16_t tempStartIndex = textEditorIndex;
    int16_t tempEndIndex = textEditorIndex;
    while (tempStartIndex > 0) {
        uint8_t tempSymbol = textEditorText[tempStartIndex - 1];
        if (tempSymbol == '\n') {
            break;
        }
        tempStartIndex -= 1;
    }
    while (true) {
        uint8_t tempSymbol = textEditorText[tempEndIndex];
        if (tempSymbol == '\n' || tempSymbol == 0) {
            break;
        }
        tempEndIndex += 1;
    }
    int16_t tempLength = tempEndIndex - tempStartIndex;
    int16_t index = tempEndIndex;
    while (true) {
        uint8_t tempSymbol = textEditorText[index];
        textEditorText[index - tempLength] = tempSymbol;
        if (tempSymbol == 0) {
            break;
        }
        index += 1;
    }
    textEditorIndex = tempStartIndex;
    textEditorLength -= tempLength;
}

static int8_t runTextEditor() {
    displayTextEditorLine();
    displayTextEditorSymbolSet();
    displayTextEditorSymbol();
    textEditorLength = strlen(textEditorText);
    while (true) {
        int8_t tempKey = getKey();
        int8_t shouldDisplayTextLine = false;
        int8_t shouldDisplaySymbol = false;
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
        if (tempKey == KEY_CURSOR_UP) {
            while (textEditorIndex > 0) {
                textEditorIndex -= 1;
                uint8_t tempSymbol = textEditorText[textEditorIndex];
                if (tempSymbol == '\n') {
                    break;
                }
            }
            shouldDisplayTextLine = true;
        }
        if (tempKey == KEY_CURSOR_DOWN) {
            while (true) {
                uint8_t tempSymbol = textEditorText[textEditorIndex];
                if (tempSymbol == 0) {
                    break;
                }
                textEditorIndex += 1;
                if (tempSymbol == '\n') {
                    break;
                }
            }
            while (true) {
                uint8_t tempSymbol = textEditorText[textEditorIndex];
                if (tempSymbol == '\n' || tempSymbol == 0) {
                    break;
                }
                textEditorIndex += 1;
            }
            shouldDisplayTextLine = true;
        }
        if (tempKey == KEY_SYMBOL_UP) {
            if (!textEditorNumberOnly) {
                textEditorSymbolSetIndex -= 1;
                if (textEditorSymbolSetIndex < 0) {
                    textEditorSymbolSetIndex = SYMBOL_SET_AMOUNT - 1;
                }
                displayTextEditorSymbolSet();
                shouldDisplaySymbol = true;
            }
        }
        if (tempKey == KEY_SYMBOL_DOWN) {
            if (!textEditorNumberOnly) {
                textEditorSymbolSetIndex += 1;
                if (textEditorSymbolSetIndex >= SYMBOL_SET_AMOUNT) {
                    textEditorSymbolSetIndex = 0;
                }
                displayTextEditorSymbolSet();
                shouldDisplaySymbol = true;
            }
        }
        if (tempKey == KEY_SYMBOL_LEFT) {
            textEditorSymbolIndex[textEditorSymbolSetIndex] -= 1;
            if (textEditorSymbolIndex[textEditorSymbolSetIndex] < 0) {
               textEditorSymbolIndex[textEditorSymbolSetIndex] = pgm_read_byte((SYMBOL_SET_SIZE_LIST + textEditorSymbolSetIndex)) - 1;
            }
            shouldDisplaySymbol = true;
        }
        if (tempKey == KEY_SYMBOL_RIGHT) {
            textEditorSymbolIndex[textEditorSymbolSetIndex] += 1;
            if (textEditorSymbolIndex[textEditorSymbolSetIndex] >= pgm_read_byte((SYMBOL_SET_SIZE_LIST + textEditorSymbolSetIndex))) {
               textEditorSymbolIndex[textEditorSymbolSetIndex] = 0;
            }
            shouldDisplaySymbol = true;
        }
        if (tempKey == KEY_SELECT_OPTION) {
            const uint8_t *tempSymbolSet = pgm_read_ptr((const void **)(SYMBOL_SET_LIST + textEditorSymbolSetIndex));
            uint8_t tempSymbol = pgm_read_byte(tempSymbolSet + textEditorSymbolIndex[textEditorSymbolSetIndex]);
            int8_t tempResult = insertTextEditorSymbol(tempSymbol);
            if (tempResult) {
                shouldDisplayTextLine = true;
            }
        }
        if (tempKey == KEY_NEWLINE) {
            int8_t tempResult = insertTextEditorSymbol('\n');
            if (tempResult) {
                shouldDisplayTextLine = true;
            }
        }
        if (tempKey == KEY_DELETE) {
            if (textEditorIndex > 0) {
                int16_t index = textEditorIndex;
                while (true) {
                    int8_t tempSymbol = textEditorText[index];
                    textEditorText[index - 1] = tempSymbol;
                    if (tempSymbol == 0) {
                        break;
                    }
                    index += 1;
                }
                textEditorIndex -= 1;
                textEditorLength -= 1;
                shouldDisplayTextLine = true;
            }
        }
        if (tempKey == KEY_CLEAR_LINE) {
            clearTextEditorLine();
            shouldDisplayTextLine = true;
        }
        if (tempKey == KEY_FINISH) {
            return true;
        }
        if (tempKey == KEY_ESCAPE) {
            return false;
        }
        if (shouldDisplayTextLine) {
            displayTextEditorLine();
        }
        if (shouldDisplaySymbol) {
            displayTextEditorSymbol();
        }
    }
}

static void initializeTextEditor(int8_t *text, int16_t maximumLength, int8_t numbersOnly) {
    textEditorText = text;
    textEditorIndex = 0;
    textEditorMaximumLength = maximumLength;
    textEditorNumberOnly = numbersOnly;
    if (numbersOnly) {
        textEditorSymbolSetIndex = 1;
    } else {
        textEditorSymbolSetIndex = 0;
    }
    int8_t index = 0;
    while (index < SYMBOL_SET_AMOUNT) {
        textEditorSymbolIndex[index] = 0;
        index += 1;
    }
}

static int32_t fileFindByName(int8_t *name) {
    int32_t tempAddress = 0;
    while (tempAddress < STORAGE_SIZE) {
        uint8_t tempExists;
        readStorage(&tempExists, tempAddress + FILE_EXISTS_OFFSET, 1);
        if (tempExists == FILE_EXISTS_TRUE) {
            int8_t tempName[FILE_NAME_MAXIMUM_LENGTH + 1];
            readStorage(tempName, tempAddress + FILE_NAME_OFFSET, sizeof(tempName));
            if (strcmp(tempName, name) == 0) {
                return tempAddress;
            }
        }
        tempAddress += FILE_ENTRY_SIZE;
    }
    return -1;
}

static int32_t fileCreate(int8_t *name) {
    if (strlen(name) > FILE_NAME_MAXIMUM_LENGTH) {
        errorMessage = ERROR_MESSAGE_NAME_IS_TOO_LONG;
        return -1;
    }
    int32_t tempFile = fileFindByName(name);
    if (tempFile >= 0) {
        errorMessage = ERROR_MESSAGE_FILE_EXISTS;
        return -1;
    }
    int32_t tempAddress = 0;
    while (tempAddress < STORAGE_SIZE) {
        uint8_t tempExists;
        readStorage(&tempExists, tempAddress + FILE_EXISTS_OFFSET, 1);
        if (tempExists == FILE_EXISTS_FALSE) {
            tempExists = FILE_EXISTS_TRUE;
            writeStorage(tempAddress + FILE_EXISTS_OFFSET, &tempExists, 1);
            writeStorage(tempAddress + FILE_NAME_OFFSET, name, strlen(name) + 1);
            int16_t tempSize = 0;
            int8_t tempData = 0;
            writeStorage(tempAddress + FILE_SIZE_OFFSET, &tempSize, 2);
            writeStorage(tempAddress + FILE_DATA_OFFSET, &tempData, 1);
            return tempAddress;
        }
        tempAddress += FILE_ENTRY_SIZE;
    }
    errorMessage = ERROR_MESSAGE_STORAGE_IS_FULL;
    return -1;
}

static int8_t fileSetName(int32_t address, int8_t *newName) {
    int16_t tempLength = strlen(newName);
    if (tempLength > FILE_NAME_MAXIMUM_LENGTH) {
        errorMessage = ERROR_MESSAGE_NAME_IS_TOO_LONG;
        return false;
    }
    int32_t tempFile = fileFindByName(newName);
    if (tempFile >= 0) {
        errorMessage = ERROR_MESSAGE_FILE_EXISTS;
        return false;
    }
    writeStorage(address + FILE_NAME_OFFSET, newName, tempLength + 1);
    return true;
}

static int8_t *fileRead(int32_t address, int16_t index, int16_t amount) {
    int16_t tempSize;
    readStorage(&tempSize, address + FILE_SIZE_OFFSET, 2);
    if (index < 0 || index >= tempSize) {
        errorMessage = ERROR_MESSAGE_BAD_INDEX;
        return NULL;
    }
    if (index + amount > tempSize) {
        errorMessage = ERROR_MESSAGE_BAD_AMOUNT;
        return NULL;
    }
    int8_t *output = createEmptyString(amount);
    if (output == NULL) {
        errorMessage = ERROR_MESSAGE_STACK_HEAP_COLLISION;
        return NULL;
    }
    int8_t *tempString = *(int8_t **)output;
    readStorage(tempString + STRING_DATA_OFFSET, address + FILE_DATA_OFFSET + index, amount);
    *(tempString + STRING_DATA_OFFSET + amount) = 0;
    return output;
}

static int8_t fileWrite(int32_t address, int8_t *text) {
    int16_t tempSize = strlen(text);
    if (tempSize > FILE_MAXIMUM_SIZE) {
        errorMessage = ERROR_MESSAGE_BAD_AMOUNT;
        return false;
    }
    writeStorage(address + FILE_SIZE_OFFSET, &tempSize, 2);
    writeStorage(address + FILE_DATA_OFFSET, text, tempSize + 1);
    return true;
}

static void promptCreateFile() {
    int8_t tempResult;
    tempResult = printTextFromProgMem(MESSAGE_ENTER_NAME);
    if (!tempResult) {
        return;
    }
    int8_t tempName[FILE_NAME_MAXIMUM_LENGTH + 1];
    tempName[0] = 0;
    initializeTextEditor(tempName, FILE_NAME_MAXIMUM_LENGTH, false);
    tempResult = runTextEditor();
    if (!tempResult) {
        return;
    }
    int32_t tempFile = fileCreate(tempName);
    if (tempFile < 0) {
        printTextFromProgMem(errorMessage);
    } else {
        printTextFromProgMem(MESSAGE_FILE_CREATED);
    }
}

static void promptRenameFile(int32_t address) {
    int8_t tempResult;
    tempResult = printTextFromProgMem(MESSAGE_ENTER_NEW_NAME);
    if (!tempResult) {
        return;
    }
    int8_t tempName[FILE_NAME_MAXIMUM_LENGTH + 1];
    tempName[0] = 0;
    initializeTextEditor(tempName, FILE_NAME_MAXIMUM_LENGTH, false);
    tempResult = runTextEditor();
    if (!tempResult) {
        return;
    }
    int8_t tempSuccess = fileSetName(address, tempName);
    if (tempSuccess) {
        printTextFromProgMem(MESSAGE_FILE_RENAMED);
    } else {
        printTextFromProgMem(errorMessage);
    }
}

static int8_t promptDeleteFile(int32_t address) {
    int8_t tempResult = menuFromProgMem(MENU_TITLE_FILE_DELETE, MENU_FILE_DELETE, sizeof(MENU_FILE_DELETE) / sizeof(*MENU_FILE_DELETE));
    if (tempResult == 1) {
        uint8_t tempExists = FILE_EXISTS_FALSE;
        writeStorage(address + FILE_EXISTS_OFFSET, &tempExists, 1);
        printTextFromProgMem(MESSAGE_FILE_DELETED);
        return true;
    }
    return false;
}

static int8_t isUnaryOperator(uint8_t symbol) {
    int8_t index = 0;
    while (index < sizeof(UNARY_OPERATOR_LIST)) {
        uint8_t tempSymbol = pgm_read_byte(UNARY_OPERATOR_LIST + index);
        if (symbol == tempSymbol) {
            return true;
        }
        index += 1;
    }
    return false;
}

static int32_t readStorageVariableName(uint8_t *destination, int32_t address) {
    int8_t index = 0;
    while (true) {
        if (index >= VARIABLE_NAME_MAXIMUM_LENGTH) {
            errorMessage = ERROR_MESSAGE_VARIABLE_NAME_TOO_LONG;
            return -1;
        }
        uint8_t tempSymbol = readStorageInt8(address);
        if (index == 0) {
            if (!((tempSymbol >= 'A' && tempSymbol <= 'Z') || tempSymbol == '_')) {
                break;
            }
        } else {
            if (!((tempSymbol >= 'A' && tempSymbol <= 'Z') || (tempSymbol >= '0' && tempSymbol <= '9') || tempSymbol == '_')) {
                break;
            }
        }
        destination[index] = tempSymbol;
        address += 1;
        index += 1;
    }
    if (index == 0) {
        errorMessage = ERROR_MESSAGE_BAD_DESTINATION;
        return -1;
    }
    destination[index] = 0;
    return address;
}

static value_t *findVariableValueByNameInScope(uint8_t *name, int8_t *scope) {
    int8_t *tempVariable = scope + SCOPE_VARIABLE_OFFSET;
    while (tempVariable != NULL) {
        if (strcmp(name, tempVariable + VARIABLE_NAME_OFFSET) == 0) {
            return (value_t *)(tempVariable + VARIABLE_VALUE_OFFSET);
        }
        tempVariable = *(int8_t **)(tempVariable + VARIABLE_NEXT_OFFSET);
    }
    return NULL;
}

static value_t *findVariableValueByName(uint8_t *name) {
    value_t *output = findVariableValueByNameInScope(name, localScope);
    if (output != NULL) {
        return output;
    }
    return findVariableValueByNameInScope(name, globalScope);
}

static value_t *createVariable(uint8_t *name) {
    int8_t tempLength = strlen(name);
    int16_t tempSize = *(int16_t *)(localScope + SCOPE_SIZE_OFFSET);
    int8_t *tempVariable = localScope + SCOPE_DATA_OFFSET + tempSize;
    tempSize += VARIABLE_NAME_OFFSET + tempLength + 1;
    if (localScope + SCOPE_DATA_OFFSET + tempSize > getHeapBoundary()) {
        return NULL;
    }
    *(int16_t *)(localScope + SCOPE_SIZE_OFFSET) = tempSize;
    *(int8_t **)(tempVariable + VARIABLE_NEXT_OFFSET) = *(int8_t **)(localScope + SCOPE_VARIABLE_OFFSET);
    *(int8_t **)(localScope + SCOPE_VARIABLE_OFFSET) = tempVariable;
    strcpy(tempVariable + VARIABLE_NAME_OFFSET, name);
    return (value_t *)(tempVariable + VARIABLE_VALUE_OFFSET);
}

static branch_t *createBranch() {
    int16_t tempSize = *(int16_t *)(localScope + SCOPE_SIZE_OFFSET);
    branch_t *output = (branch_t *)(localScope + SCOPE_DATA_OFFSET + tempSize);
    tempSize += sizeof(branch_t);
    if (localScope + SCOPE_DATA_OFFSET + tempSize > getHeapBoundary()) {
        return NULL;
    }
    *(int16_t *)(localScope + SCOPE_SIZE_OFFSET) = tempSize;
    branch_t *tempPreviousBranch = *(branch_t **)(localScope + SCOPE_BRANCH_OFFSET);
    if (tempPreviousBranch != NULL) {
        tempPreviousBranch->next = output;
    }
    output->previous = tempPreviousBranch;
    output->next = NULL;
    return output;
}

static int8_t pushBranch(int8_t action, int32_t address) {
    branch_t *tempBranch = *(branch_t **)(localScope + SCOPE_BRANCH_OFFSET);
    branch_t *tempNextBranch;
    if (tempBranch == NULL) {
        tempNextBranch = createBranch();
    } else {
        tempNextBranch = tempBranch->next;
        if (tempNextBranch == NULL) {
            tempNextBranch = createBranch();
        }
    }
    if (tempNextBranch == NULL) {
        errorMessage = ERROR_MESSAGE_STACK_HEAP_COLLISION;
        return false;
    }
    tempNextBranch->action = action;
    tempNextBranch->address = address;
    *(branch_t **)(localScope + SCOPE_BRANCH_OFFSET) = tempNextBranch;
    return true;
}

static int8_t popBranch() {
    branch_t *tempBranch = *(branch_t **)(localScope + SCOPE_BRANCH_OFFSET);
    branch_t *tempPreviousBranch = tempBranch->previous;
    if (tempPreviousBranch == NULL) {
        return false;
    }
    *(branch_t **)(localScope + SCOPE_BRANCH_OFFSET) = tempPreviousBranch;
    return true;
}

static int32_t skipStorageLine(int32_t address) {
    while (true) {
        uint8_t tempSymbol = readStorageInt8(address);
        if (tempSymbol == '\n' || tempSymbol == 0) {
            break;
        }
        address += 1;
    }
    return address;
}

static int8_t getCustomFunctionArgumentAmount(int32_t code) {
    int8_t output = 0;
    code += 1;
    while (true) {
        uint8_t tempSymbol = readStorageInt8(code);
        if (tempSymbol == ',') {
            output += 1;
        }
        if (tempSymbol == '\n' || tempSymbol == 0) {
            break;
        }
        code += 1;
    }
    return output;
}

static int16_t getStringLiteralLength(int32_t code) {
    int16_t output = 0;
    int8_t tempIsEscaped = false;
    while (true) {
        uint8_t tempSymbol = readStorageInt8(code);
        if (tempSymbol == 0) {
            break;
        }
        if (tempIsEscaped) {
            output += 1;
            tempIsEscaped = false;
        } else {
            if (tempSymbol == '\\') {
                tempIsEscaped = true;
            } else if (tempSymbol == '"') {
                break;
            } else {
                output += 1;
            }
        }
        code += 1;
    }
    return output;
}

static void initializeTreasureTracker(treasureTracker_t *treasureTracker, int8_t type, void *treasure) {
    treasureTracker->next = firstTreasureTracker;
    treasureTracker->type = type;
    treasureTracker->amount = 0;
    treasureTracker->treasure = treasure;
    firstTreasureTracker = treasureTracker;
}

static void markValueAsReachable(value_t *value) {
    if (value->type == VALUE_TYPE_STRING) {
        int8_t *tempPointer = *(int8_t **)(value->data);
        int8_t *tempString = *(int8_t **)tempPointer;
        *(int8_t *)(tempPointer - ALLOCATION_IS_REACHABLE_OFFSET) = true;
        *(int8_t *)(tempString - ALLOCATION_IS_REACHABLE_OFFSET) = true;
    }
    if (value->type == VALUE_TYPE_LIST) {
        int8_t *tempPointer = *(int8_t **)(value->data);
        int8_t *tempList = *(int8_t **)tempPointer;
        // Dont stay in a cycle forever.
        if (!*(int8_t *)(tempPointer - ALLOCATION_IS_REACHABLE_OFFSET)) {
            *(int8_t *)(tempPointer - ALLOCATION_IS_REACHABLE_OFFSET) = true;
            *(int8_t *)(tempList - ALLOCATION_IS_REACHABLE_OFFSET) = true;
            int16_t tempLength = *(int16_t *)(tempList + LIST_LENGTH_OFFSET);
            value_t *tempValue = (value_t *)(tempList + LIST_DATA_OFFSET);
            int16_t index = 0;
            while (index < tempLength) {
                markValueAsReachable(tempValue + index);
                index += 1;
            }
        }
    }
}

static void markAndSweep() {
    int8_t *tempAllocation = firstAllocation;
    while (tempAllocation != NULL) {
        *(int8_t *)(tempAllocation - ALLOCATION_IS_REACHABLE_OFFSET) = false;
        tempAllocation = *(int8_t **)(tempAllocation - ALLOCATION_NEXT_OFFSET);
    }
    treasureTracker_t *tempTreasureTracker = firstTreasureTracker;
    while (tempTreasureTracker != NULL) {
        if (tempTreasureTracker->type == TREASURE_TYPE_VALUE) {
            markValueAsReachable((value_t *)(tempTreasureTracker->treasure));
        }
        if (tempTreasureTracker->type == TREASURE_TYPE_VALUE_ARRAY) {
            int8_t index = 0;
            while (index < tempTreasureTracker->amount) {
                markValueAsReachable((value_t *)(tempTreasureTracker->treasure) + index);
                index += 1;
            }
        }
        if (tempTreasureTracker->type == TREASURE_TYPE_SCOPE) {
            int8_t *tempScope = (int8_t *)(tempTreasureTracker->treasure);
            int8_t *tempVariable = *(int8_t **)(tempScope + SCOPE_VARIABLE_OFFSET);
            while (tempVariable != NULL) {
                markValueAsReachable((value_t *)(tempVariable + VARIABLE_VALUE_OFFSET));
                tempVariable = *(int8_t **)(tempVariable + VARIABLE_NEXT_OFFSET);
            }
        }
        tempTreasureTracker = tempTreasureTracker->next;
    }
    tempAllocation = firstAllocation;
    while (tempAllocation != NULL) {
        int8_t *tempNextAllocation = *(int8_t **)(tempAllocation - ALLOCATION_NEXT_OFFSET);
        if (!*(int8_t *)(tempAllocation - ALLOCATION_IS_REACHABLE_OFFSET)) {
            deallocate(tempAllocation);
        }
        tempAllocation = tempNextAllocation;
    }
    commandsSinceMarkAndSweep = 0;
    allocationsSinceMarkAndSweep = 0;
}

static int8_t insertValueIntoSequence(value_t *sequence, int16_t index, value_t *value) {
    if (sequence->type == VALUE_TYPE_STRING) {
        if (value->type != VALUE_TYPE_NUMBER) {
            errorMessage = ERROR_MESSAGE_BAD_ARGUMENT_TYPE;
            return false;
        }
        int8_t *tempPointer = *(int8_t **)(sequence->data);
        int8_t *tempString = *(int8_t **)tempPointer;
        int16_t tempLength = *(int16_t *)(tempString + STRING_LENGTH_OFFSET);
        if (index < 0 || index > tempLength) {
            errorMessage = ERROR_MESSAGE_BAD_INDEX;
            return false;
        }        uint8_t tempSymbol = *(float *)(value->data);
        tempString = resizeString(tempPointer, tempLength + 1);
        if (tempString == NULL) {
            errorMessage = ERROR_MESSAGE_STACK_HEAP_COLLISION;
            return false;
        }
        memmove(tempString + STRING_DATA_OFFSET + index + 1, tempString + STRING_DATA_OFFSET + index, tempLength - index + 1);
        *(tempString + STRING_DATA_OFFSET + index) = tempSymbol;
        return true;
    } else if (sequence->type == VALUE_TYPE_LIST) {
        return insertListValue(*(int8_t **)(sequence->data), index, value);
    } else {
        errorMessage = ERROR_MESSAGE_BAD_ARGUMENT_TYPE;
        return false;
    }
}

static int8_t removeSubsequenceFromSequence(value_t *sequence, int16_t startIndex, int16_t endIndex) {
    if (sequence->type == VALUE_TYPE_STRING) {
        int8_t *tempPointer = *(int8_t **)(sequence->data);
        int8_t *tempString = *(int8_t **)tempPointer;
        int16_t tempLength1 = *(int16_t *)(tempString + STRING_LENGTH_OFFSET);
        int16_t tempLength2 = endIndex - startIndex;
        if (startIndex < 0 || startIndex >= tempLength1 || endIndex < 0 || endIndex > tempLength1 || startIndex > endIndex) {
            errorMessage = ERROR_MESSAGE_BAD_INDEX;
            return false;
        }
        memmove(tempString + STRING_DATA_OFFSET + startIndex, tempString + STRING_DATA_OFFSET + endIndex, tempLength1 - endIndex + 1);
        tempString = resizeString(tempPointer, tempLength1 - tempLength2);
        if (tempString == NULL) {
            errorMessage = ERROR_MESSAGE_STACK_HEAP_COLLISION;
            return false;
        }
        return true;
    } else if (sequence->type == VALUE_TYPE_LIST) {
        int8_t *tempPointer = *(int8_t **)(sequence->data);
        int8_t *tempList = *(int8_t **)tempPointer;
        int16_t tempLength1 = *(int16_t *)(tempList + LIST_LENGTH_OFFSET);
        int16_t tempLength2 = endIndex - startIndex;
        if (startIndex < 0 || startIndex >= tempLength1 || endIndex < 0 || endIndex > tempLength1 || startIndex > endIndex) {
            errorMessage = ERROR_MESSAGE_BAD_INDEX;
            return false;
        }
        memmove(tempList + LIST_DATA_OFFSET + startIndex * sizeof(value_t), tempList + LIST_DATA_OFFSET + endIndex * sizeof(value_t), (tempLength1 - endIndex) * sizeof(value_t));
        tempList = resizeList(tempPointer, tempLength1 - tempLength2);
        if (tempList == NULL) {
            errorMessage = ERROR_MESSAGE_STACK_HEAP_COLLISION;
            return false;
        }
        return true;
    } else {
        errorMessage = ERROR_MESSAGE_BAD_ARGUMENT_TYPE;
        return false;
    }
}

static int8_t removeValueFromSequence(value_t *sequence, int16_t index) {
    return removeSubsequenceFromSequence(sequence, index, index + 1);
}

static value_t getSubsequenceFromSequence(value_t *sequence, int16_t startIndex, int16_t endIndex) {
    value_t output;
    output.type = VALUE_TYPE_MISSING;
    if (sequence->type == VALUE_TYPE_STRING) {
        int8_t *tempPointer1 = *(int8_t **)(sequence->data);
        int8_t *tempString1 = *(int8_t **)tempPointer1;
        int16_t tempLength1 = *(int16_t *)(tempString1 + STRING_LENGTH_OFFSET);
        int16_t tempLength2 = endIndex - startIndex;
        if (startIndex < 0 || startIndex >= tempLength1 || endIndex < 0 || endIndex > tempLength1 || startIndex > endIndex) {
            errorMessage = ERROR_MESSAGE_BAD_INDEX;
            return output;
        }
        int8_t *tempPointer2 = createEmptyString(tempLength2);
        if (tempPointer2 == NULL) {
            errorMessage = ERROR_MESSAGE_STACK_HEAP_COLLISION;
            return output;
        }
        int8_t *tempString2 = *(int8_t **)tempPointer2;
        memcpy(tempString2 + STRING_DATA_OFFSET, tempString1 + STRING_DATA_OFFSET + startIndex, tempLength2);
        *(tempString2 + STRING_DATA_OFFSET + tempLength2) = 0;
        output.type = VALUE_TYPE_STRING;
        *(int8_t **)(output.data) = tempPointer2;
    } else if (sequence->type == VALUE_TYPE_LIST) {
        int8_t *tempPointer1 = *(int8_t **)(sequence->data);
        int8_t *tempList1 = *(int8_t **)tempPointer1;
        int16_t tempLength1 = *(int16_t *)(tempList1 + LIST_LENGTH_OFFSET);
        int16_t tempLength2 = endIndex - startIndex;
        if (startIndex < 0 || startIndex >= tempLength1 || endIndex < 0 || endIndex > tempLength1 || startIndex > endIndex) {
            errorMessage = ERROR_MESSAGE_BAD_INDEX;
            return output;
        }
        int8_t *tempPointer2 = createEmptyList(tempLength2);
        if (tempPointer2 == NULL) {
            errorMessage = ERROR_MESSAGE_STACK_HEAP_COLLISION;
            return output;
        }
        int8_t *tempList2 = *(int8_t **)tempPointer2;
        memcpy(tempList2 + LIST_DATA_OFFSET, tempList1 + LIST_DATA_OFFSET + startIndex * sizeof(value_t), tempLength2 * sizeof(value_t));
        output.type = VALUE_TYPE_LIST;
        *(int8_t **)(output.data) = tempPointer2;
    } else {
        errorMessage = ERROR_MESSAGE_BAD_ARGUMENT_TYPE;
    }
    return output;
}

static int8_t insertSubsequenceIntoSequence(value_t *sequence, int16_t index, value_t *subsequence) {
    if (sequence->type == VALUE_TYPE_STRING) {
        if (subsequence->type != VALUE_TYPE_STRING) {
            errorMessage = ERROR_MESSAGE_BAD_ARGUMENT_TYPE;
            return false;
        }
        int8_t *tempPointer1 = *(int8_t **)(sequence->data);
        int8_t *tempPointer2 = *(int8_t **)(subsequence->data);
        int8_t *tempString1 = *(int8_t **)tempPointer1;
        int8_t *tempString2 = *(int8_t **)tempPointer2;
        int16_t tempLength1 = *(int16_t *)(tempString1 + STRING_LENGTH_OFFSET);
        int16_t tempLength2 = *(int16_t *)(tempString2 + STRING_LENGTH_OFFSET);
        if (index < 0 || index > tempLength1) {
            errorMessage = ERROR_MESSAGE_BAD_INDEX;
            return false;
        }
        tempString1 = resizeString(tempPointer1, tempLength1 + tempLength2);
        if (tempString1 == NULL) {
            errorMessage = ERROR_MESSAGE_STACK_HEAP_COLLISION;
            return false;
        }
        memmove(tempString1 + STRING_DATA_OFFSET + index + tempLength2, tempString1 + STRING_DATA_OFFSET + index, tempLength1 - index + 1);
        memcpy(tempString1 + STRING_DATA_OFFSET + index, tempString2 + STRING_DATA_OFFSET, tempLength2);
        return true;
    } else if (sequence->type == VALUE_TYPE_LIST) {
        if (subsequence->type != VALUE_TYPE_LIST) {
            errorMessage = ERROR_MESSAGE_BAD_ARGUMENT_TYPE;
            return false;
        }
        int8_t *tempPointer1 = *(int8_t **)(sequence->data);
        int8_t *tempPointer2 = *(int8_t **)(subsequence->data);
        int8_t *tempList1 = *(int8_t **)tempPointer1;
        int8_t *tempList2 = *(int8_t **)tempPointer2;
        int16_t tempLength1 = *(int16_t *)(tempList1 + LIST_LENGTH_OFFSET);
        int16_t tempLength2 = *(int16_t *)(tempList2 + LIST_LENGTH_OFFSET);
        if (index < 0 || index > tempLength1) {
            errorMessage = ERROR_MESSAGE_BAD_INDEX;
            return false;
        }
        tempList1 = resizeList(tempPointer1, tempLength1 + tempLength2);
        if (tempList1 == NULL) {
            errorMessage = ERROR_MESSAGE_STACK_HEAP_COLLISION;
            return false;
        }
        memmove(tempList1 + LIST_DATA_OFFSET + (index + tempLength2) * sizeof(value_t), tempList1 + LIST_DATA_OFFSET + index * sizeof(value_t), (tempLength1 - index) * sizeof(value_t));
        memcpy(tempList1 + LIST_DATA_OFFSET + index * sizeof(value_t), tempList2 + LIST_DATA_OFFSET, tempLength2 * sizeof(value_t));
        return true;
    } else {
        errorMessage = ERROR_MESSAGE_BAD_ARGUMENT_TYPE;
        return false;
    }
}

static int8_t stringsAreEqual(int8_t *string1, int8_t *string2) {
    int8_t *tempString1 = *(int8_t **)string1;
    int8_t *tempString2 = *(int8_t **)string2;
    int16_t tempLength1 = *(int16_t *)(tempString1 + STRING_LENGTH_OFFSET);
    int16_t tempLength2 = *(int16_t *)(tempString2 + STRING_LENGTH_OFFSET);
    if (tempLength1 != tempLength2) {
        return false;
    }
    int16_t index = 0;
    while (index < tempLength1) {
        uint8_t tempSymbol1 = *(tempString1 + STRING_DATA_OFFSET + index);
        uint8_t tempSymbol2 = *(tempString2 + STRING_DATA_OFFSET + index);
        if (tempSymbol1 != tempSymbol2) {
            return false;
        }
        index += 1;
    }
    return true;
}

static void reportError(const int8_t *message, int32_t code) {
    errorMessage = message;
    errorCode = code;
}

static expressionResult_t runCode(int32_t address);

static void debugPrint(int16_t value) {
    clearDisplay();
    if (value < 0) {
        value = -value;
    }
    int8_t index = 0;
    int16_t tempValue = 10000;
    while (tempValue >= 1) {
        displayCharacter(index, 0, '0' + value / tempValue);
        value %= tempValue;
        index += 1;
        tempValue /= 10;
    }
    getKey();
}

static expressionResult_t evaluateExpression(int32_t code, int8_t precedence, int8_t isTopLevel) {
    branch_t *tempBranch = *(branch_t **)(localScope + SCOPE_BRANCH_OFFSET);
    //mvprintw(0, 0, "%d %d      ", code, tempBranch->action);
    //getch();
    int32_t tempStartCode = code;
    uint8_t tempSymbol = readStorageInt8(code);
    expressionResult_t tempResult;
    //debugPrint(tempSymbol);
    //debugPrint((int16_t)&tempResult);
    tempResult.status = EVALUATION_STATUS_NORMAL;
    tempResult.destination = NULL;
    tempResult.value.type = VALUE_TYPE_MISSING;
    if (nativeStackHasCollision(100)) {
        reportError(ERROR_MESSAGE_EXPRESSION_TOO_COMPLEX, tempStartCode);
        tempResult.status = EVALUATION_STATUS_QUIT;
        return tempResult;
    }
    treasureTracker_t tempTreasureTracker;
    initializeTreasureTracker(&tempTreasureTracker, TREASURE_TYPE_VALUE, &(tempResult.value));
    if (tempBranch->action == BRANCH_ACTION_IGNORE_SOFT || tempBranch->action == BRANCH_ACTION_IGNORE_HARD) {
        if (tempSymbol == SYMBOL_IF || tempSymbol == SYMBOL_WHILE || tempSymbol == SYMBOL_FUNCTION) {
            int8_t tempSuccess = pushBranch(BRANCH_ACTION_IGNORE_HARD, 0);
            if (!tempSuccess) {
                errorCode = tempStartCode;
                tempResult.status = EVALUATION_STATUS_QUIT;
                return tempResult;
            }
        } else if (tempSymbol == SYMBOL_ELSE_IF) {
            if (tempBranch->action == BRANCH_ACTION_IGNORE_SOFT) {
                tempBranch->action = BRANCH_ACTION_RUN;
                expressionResult_t tempResult2 = evaluateExpression(code + 1, 99, false);
                tempBranch->action = BRANCH_ACTION_IGNORE_SOFT;
                firstTreasureTracker = &tempTreasureTracker;
                treasureTracker_t tempTreasureTracker2;
                initializeTreasureTracker(&tempTreasureTracker2, TREASURE_TYPE_VALUE, &(tempResult2.value));
                if (tempResult2.status != EVALUATION_STATUS_NORMAL) {
                    tempResult.status = tempResult2.status;
                    return tempResult;
                }
                code = tempResult2.nextCode;
                if (tempResult2.value.type == VALUE_TYPE_MISSING) {
                    reportError(ERROR_MESSAGE_MISSING_VALUE, tempStartCode);
                    tempResult.status = EVALUATION_STATUS_QUIT;
                    return tempResult;
                }
                if (tempResult2.value.type != VALUE_TYPE_NUMBER) {
                    reportError(ERROR_MESSAGE_BAD_ARGUMENT_TYPE, tempStartCode);
                    tempResult.status = EVALUATION_STATUS_QUIT;
                    return tempResult;
                }
                if (*(float *)(tempResult2.value.data) != 0.0) {
                    tempBranch->action = BRANCH_ACTION_RUN;
                }
            }
        } else if (tempSymbol == SYMBOL_ELSE) {
            if (tempBranch->action == BRANCH_ACTION_IGNORE_SOFT) {
                tempBranch->action = BRANCH_ACTION_RUN;
            }
        } else if (tempSymbol == SYMBOL_END) {
            int8_t tempSuccess = popBranch();
            if (!tempSuccess) {
                reportError(ERROR_MESSAGE_BAD_END_STATEMENT, tempStartCode);
                tempResult.status = EVALUATION_STATUS_QUIT;
                return tempResult;
            }
        }
        code = skipStorageLine(code);
    } else {
        if ((tempSymbol >= '0' && tempSymbol <= '9') || tempSymbol == '.') {
            volatile int16_t tempCheatSize = NUMBER_LITERAL_MAXIMUM_LENGTH + 1;
            uint8_t tempBuffer[tempCheatSize];
            tempBuffer[0] = tempSymbol;
            code += 1;
            uint8_t tempLastSymbol = 0;
            int8_t index = 1;
            while (true) {
                if (index >= NUMBER_LITERAL_MAXIMUM_LENGTH) {
                    reportError(ERROR_MESSAGE_NUMBER_LITERAL_TOO_LONG, tempStartCode);
                    tempResult.status = EVALUATION_STATUS_QUIT;
                    return tempResult;
                }
                tempSymbol = readStorageInt8(code);
                if (!((tempSymbol >= '0' && tempSymbol <= '9') || tempSymbol == '.' || tempSymbol == 'e'
                        || (tempSymbol == '-' && tempLastSymbol == 'e'))) {
                    tempBuffer[index] = 0;
                    break;
                }
                tempBuffer[index] = tempSymbol;
                tempLastSymbol = tempSymbol;
                code += 1;
                index += 1;
            }
            tempResult.value.type = VALUE_TYPE_NUMBER;
            float tempNumber = convertTextToFloat(tempBuffer);
            *(float *)(tempResult.value.data) = tempNumber;
        } else if ((tempSymbol >= 'A' && tempSymbol <= 'Z') || tempSymbol == '_') {
            volatile int16_t tempCheatSize = VARIABLE_NAME_MAXIMUM_LENGTH + 1;
            uint8_t tempBuffer[tempCheatSize];
            code = readStorageVariableName(tempBuffer, code);
            if (code < 0) {
                errorCode = tempStartCode;
                tempResult.status = EVALUATION_STATUS_QUIT;
                return tempResult;
            }
            tempResult.destinationType = DESTINATION_TYPE_VALUE;
            tempResult.destination = (int8_t *)findVariableValueByName(tempBuffer);
            if (tempResult.destination != NULL) {
                tempResult.value = *(value_t *)(tempResult.destination);
            }
        } else if (tempSymbol == '\'') {
            code += 1;
            uint8_t tempSymbol = readStorageInt8(code);
            code += 1;
            if (tempSymbol == '\\') {
                tempSymbol = readStorageInt8(code);
                code += 1;
                if (tempSymbol == 'N') {
                    tempSymbol = '\n';
                }
            }
            int8_t tempSymbol2 = readStorageInt8(code);
            if (tempSymbol2 != '\'') {
                reportError(ERROR_MESSAGE_MISSING_APOSTROPHE, tempStartCode);
                tempResult.status = EVALUATION_STATUS_QUIT;
                return tempResult;
            }
            code += 1;
            tempResult.value.type = VALUE_TYPE_NUMBER;
            *(float *)&(tempResult.value.data) = tempSymbol;
        } else if (tempSymbol == '"') {
            code += 1;
            int16_t tempLength = getStringLiteralLength(code);
            int8_t *tempString = createEmptyString(tempLength);
            if (tempString == NULL) {
                reportError(ERROR_MESSAGE_STACK_HEAP_COLLISION, tempStartCode);
                tempResult.status = EVALUATION_STATUS_QUIT;
                return tempResult;
            }
            int8_t *tempStringContents = *(int8_t **)tempString;
            int16_t index = 0;
            int8_t tempIsEscaped = false;
            while (true) {
                uint8_t tempSymbol = readStorageInt8(code);
                if (tempSymbol == 0) {
                    reportError(ERROR_MESSAGE_MISSING_QUOTATION_MARK, tempStartCode);
                    tempResult.status = EVALUATION_STATUS_QUIT;
                    return tempResult;
                }
                if (tempIsEscaped) {
                    if (tempSymbol == 'N') {
                        *(tempStringContents + STRING_DATA_OFFSET + index) = '\n';
                    } else {
                        *(tempStringContents + STRING_DATA_OFFSET + index) = tempSymbol;
                    }
                    index += 1;
                    tempIsEscaped = false;
                } else {
                    if (tempSymbol == '\\') {
                        tempIsEscaped = true;
                    } else if (tempSymbol == '"') {
                        code += 1;
                        break;
                    } else {
                        *(tempStringContents + STRING_DATA_OFFSET + index) = tempSymbol;
                        index += 1;
                    }
                }
                code += 1;
            }
            *(tempStringContents + STRING_DATA_OFFSET + index) = 0;
            tempResult.value.type = VALUE_TYPE_STRING;
            *(int8_t **)(tempResult.value.data) = tempString;
        } else if (tempSymbol == '[') {
            code += 1;
            int8_t *tempList = createEmptyList(0);
            if (tempList == NULL) {
                reportError(ERROR_MESSAGE_STACK_HEAP_COLLISION, tempStartCode);
                tempResult.status = EVALUATION_STATUS_QUIT;
                return tempResult;
            }
            int16_t index = 0;
            tempResult.value.type = VALUE_TYPE_LIST;
            *(int8_t **)(tempResult.value.data) = tempList;
            while (true) {
                uint8_t tempSymbol = readStorageInt8(code);
                if (tempSymbol == '\n' || tempSymbol == 0) {
                    reportError(ERROR_MESSAGE_MISSING_BRACKET, tempStartCode);
                    tempResult.status = EVALUATION_STATUS_QUIT;
                    return tempResult;
                }
                if (tempSymbol == ']') {
                    code += 1;
                    break;
                }
                if (tempSymbol == ',') {
                    code += 1;
                }
                expressionResult_t tempResult2 = evaluateExpression(code, 99, false);
                // Treasure does not need to be tracked because it will
                // be immediately inserted into the tracked list.
                if (tempResult2.status != EVALUATION_STATUS_NORMAL) {
                    tempResult.status = tempResult2.status;
                    return tempResult;
                }
                if (tempResult2.value.type == VALUE_TYPE_MISSING) {
                    reportError(ERROR_MESSAGE_MISSING_VALUE, tempStartCode);
                    tempResult.status = EVALUATION_STATUS_QUIT;
                    return tempResult;
                }
                code = tempResult2.nextCode;
                int8_t tempSuccess = insertListValue(tempList, index, &(tempResult2.value));
                if (!tempSuccess) {
                    errorCode = tempStartCode;
                    tempResult.status = EVALUATION_STATUS_QUIT;
                    return tempResult;
                }
                index += 1;
            }
        } else if (tempSymbol == '(') {
            code += 1;
            expressionResult_t tempResult2 = evaluateExpression(code, 99, false);
            firstTreasureTracker = &tempTreasureTracker;
            treasureTracker_t tempTreasureTracker2;
            initializeTreasureTracker(&tempTreasureTracker2, TREASURE_TYPE_VALUE, &(tempResult2.value));
            if (tempResult2.status != EVALUATION_STATUS_NORMAL) {
                tempResult.status = tempResult2.status;
                return tempResult;
            }
            code = tempResult2.nextCode;
            tempResult.destinationType = tempResult2.destinationType;
            tempResult.destination = tempResult2.destination;
            tempResult.value = tempResult2.value;
            tempSymbol = readStorageInt8(code);
            if (tempSymbol != ')') {
                reportError(ERROR_MESSAGE_MISSING_PARENTHESIS, tempStartCode);
                tempResult.status = EVALUATION_STATUS_QUIT;
                return tempResult;
            }
            code += 1;
        } else if (tempSymbol >= FIRST_FUNCTION_SYMBOL && tempSymbol <= LAST_FUNCTION_SYMBOL) {
            uint8_t tempFunction = tempSymbol;
            code += 1;
            int8_t tempArgumentAmount = pgm_read_byte(FUNCTION_ARGUMENT_AMOUNT_LIST + (tempSymbol - FIRST_FUNCTION_SYMBOL));
            if (tempArgumentAmount < 0) {
                tempArgumentAmount = getCustomFunctionArgumentAmount(tempStartCode) + 1;
            }
            value_t tempArgumentList[tempArgumentAmount];
            int32_t tempExpressionList[tempArgumentAmount];
            if (nativeStackHasCollision(100)) {
                reportError(ERROR_MESSAGE_EXPRESSION_TOO_COMPLEX, tempStartCode);
                tempResult.status = EVALUATION_STATUS_QUIT;
                return tempResult;
            }
            treasureTracker_t tempTreasureTracker2;
            initializeTreasureTracker(&tempTreasureTracker2, TREASURE_TYPE_VALUE_ARRAY, tempArgumentList);
            int8_t index = 0;
            while (index < tempArgumentAmount) {
                tempExpressionList[index] = code;
                expressionResult_t tempResult2 = evaluateExpression(code, 99, false);
                firstTreasureTracker = &tempTreasureTracker2;
                if (tempResult2.status != EVALUATION_STATUS_NORMAL) {
                    tempResult.status = tempResult2.status;
                    return tempResult;
                }
                if (tempResult2.value.type == VALUE_TYPE_MISSING && tempFunction != SYMBOL_FUNCTION) {
                    reportError(ERROR_MESSAGE_MISSING_VALUE, tempStartCode);
                    tempResult.status = EVALUATION_STATUS_QUIT;
                    return tempResult;
                }
                tempArgumentList[index] = tempResult2.value;
                tempTreasureTracker2.amount += 1;
                code = tempResult2.nextCode;
                if (index < tempArgumentAmount - 1) {
                    int8_t tempSymbol = readStorageInt8(code);
                    if (tempSymbol != ',') {
                        reportError(ERROR_MESSAGE_MISSING_COMMA, tempStartCode);
                        tempResult.status = EVALUATION_STATUS_QUIT;
                        return tempResult;
                    }
                    code += 1;
                }
                index += 1;
            }
            int8_t tempShouldDisplayRunning = false;
            // Control functions.
            if (tempFunction >= SYMBOL_IF && tempFunction <= SYMBOL_QUIT) {
                if (!isTopLevel) {
                    reportError(ERROR_MESSAGE_NOT_TOP_LEVEL, tempStartCode);
                    tempResult.status = EVALUATION_STATUS_QUIT;
                    return tempResult;
                }
                if (tempFunction == SYMBOL_RETURN) {
                    tempResult.status = EVALUATION_STATUS_RETURN;
                }
                if (tempFunction == SYMBOL_RETURN_WITH_VALUE) {
                    tempResult.status = EVALUATION_STATUS_RETURN;
                    tempResult.value = tempArgumentList[0];
                }
                if (tempFunction == SYMBOL_IF) {
                    if ((tempArgumentList + 0)->type != VALUE_TYPE_NUMBER) {
                        reportError(ERROR_MESSAGE_BAD_ARGUMENT_TYPE, tempStartCode);
                        tempResult.status = EVALUATION_STATUS_QUIT;
                        return tempResult;
                    }
                    int8_t tempSuccess;
                    if (*(float *)((tempArgumentList + 0)->data) == 0.0) {
                        tempSuccess = pushBranch(BRANCH_ACTION_IGNORE_SOFT, 0);
                    } else {
                        tempSuccess = pushBranch(BRANCH_ACTION_RUN, 0);
                    }
                    if (!tempSuccess) {
                        errorCode = tempStartCode;
                        tempResult.status = EVALUATION_STATUS_QUIT;
                        return tempResult;
                    }
                }
                if (tempFunction == SYMBOL_ELSE_IF) {
                    tempBranch->action = BRANCH_ACTION_IGNORE_HARD;
                }
                if (tempFunction == SYMBOL_ELSE) {
                    tempBranch->action = BRANCH_ACTION_IGNORE_HARD;
                }
                if (tempFunction == SYMBOL_WHILE) {
                    if ((tempArgumentList + 0)->type != VALUE_TYPE_NUMBER) {
                        reportError(ERROR_MESSAGE_BAD_ARGUMENT_TYPE, tempStartCode);
                        tempResult.status = EVALUATION_STATUS_QUIT;
                        return tempResult;
                    }
                    int8_t tempSuccess;
                    if (*(float *)((tempArgumentList + 0)->data) == 0.0) {
                        tempSuccess = pushBranch(BRANCH_ACTION_IGNORE_HARD, 0);
                    } else {
                        tempSuccess = pushBranch(BRANCH_ACTION_LOOP, tempStartCode);
                    }
                    if (!tempSuccess) {
                        errorCode = tempStartCode;
                        tempResult.status = EVALUATION_STATUS_QUIT;
                        return tempResult;
                    }
                }
                if (tempFunction == SYMBOL_BREAK) {
                    branch_t *tempBranch2 = tempBranch;
                    while (true) {
                        int8_t tempAction = tempBranch2->action;
                        tempBranch2->action = BRANCH_ACTION_IGNORE_HARD;
                        if (tempAction == BRANCH_ACTION_LOOP) {
                            break;
                        }
                        tempBranch2 = tempBranch2->previous;
                        if (tempBranch2 == NULL) {
                            reportError(ERROR_MESSAGE_BAD_BREAK_STATEMENT, tempStartCode);
                            tempResult.status = EVALUATION_STATUS_QUIT;
                            return tempResult;
                        }
                    }
                }
                if (tempFunction == SYMBOL_CONTINUE) {
                    while (true) {
                        if (tempBranch->action == BRANCH_ACTION_LOOP) {
                            code = tempBranch->address;
                            break;
                        }
                        int8_t tempSuccess = popBranch();
                        if (!tempSuccess) {
                            reportError(ERROR_MESSAGE_BAD_CONTINUE_STATEMENT, tempStartCode);
                            tempResult.status = EVALUATION_STATUS_QUIT;
                            return tempResult;
                        }
                        tempBranch = tempBranch->previous;
                    }
                }
                if (tempFunction == SYMBOL_QUIT) {
                    tempResult.status = EVALUATION_STATUS_QUIT;
                }
                if (tempFunction == SYMBOL_END) {
                    int8_t tempAction = tempBranch->action;
                    int32_t tempAddress = tempBranch->address;
                    int8_t tempSuccess = popBranch();
                    if (!tempSuccess) {
                        if (localScope != globalScope) {
                            tempResult.status = EVALUATION_STATUS_RETURN;
                        } else {
                            reportError(ERROR_MESSAGE_BAD_END_STATEMENT, tempStartCode);
                            tempResult.status = EVALUATION_STATUS_QUIT;
                            return tempResult;
                        }
                    }
                    if (tempAction == BRANCH_ACTION_LOOP) {
                        code = tempAddress;
                    }
                }
                if (tempFunction == SYMBOL_FUNCTION) {
                    volatile int16_t tempCheatSize = VARIABLE_NAME_MAXIMUM_LENGTH + 1;
                    uint8_t tempBuffer[tempCheatSize];
                    int32_t tempCode = readStorageVariableName(tempBuffer, tempExpressionList[0]);
                    if (tempCode < 0) {
                        errorCode = tempStartCode;
                        tempResult.status = EVALUATION_STATUS_QUIT;
                        return tempResult;
                    }
                    value_t *tempValue = findVariableValueByName(tempBuffer);
                    if (tempValue == NULL) {
                        tempValue = createVariable(tempBuffer);
                        if (tempValue == NULL) {
                            reportError(ERROR_MESSAGE_STACK_HEAP_COLLISION, tempStartCode);
                            tempResult.status = EVALUATION_STATUS_QUIT;
                            return tempResult;
                        }
                    }
                    tempValue->type = VALUE_TYPE_FUNCTION;
                    *(int32_t *)(tempValue->data) = tempStartCode;
                    int8_t tempSuccess = pushBranch(BRANCH_ACTION_IGNORE_HARD, 0);
                    if (!tempSuccess) {
                        errorCode = tempStartCode;
                        tempResult.status = EVALUATION_STATUS_QUIT;
                        return tempResult;
                    }
                }
            }
            // Math functions.
            if (tempFunction >= SYMBOL_RANDOM && tempFunction <= SYMBOL_LOG) {
                if (tempFunction == SYMBOL_RANDOM) {
                    tempResult.value.type = VALUE_TYPE_NUMBER;
                    *(float *)(tempResult.value.data) = (float)(rand() % 10000) / 10000.0;
                } else {
                    if ((tempArgumentList + 0)->type != VALUE_TYPE_NUMBER) {
                        reportError(ERROR_MESSAGE_BAD_ARGUMENT_TYPE, tempStartCode);
                        tempResult.status = EVALUATION_STATUS_QUIT;
                        return tempResult;
                    }
                }
                if (tempFunction == SYMBOL_RANDOM_INTEGER) {
                    if ((tempArgumentList + 1)->type != VALUE_TYPE_NUMBER) {
                        reportError(ERROR_MESSAGE_BAD_ARGUMENT_TYPE, tempStartCode);
                        tempResult.status = EVALUATION_STATUS_QUIT;
                        return tempResult;
                    }
                    int32_t tempMinimum = *(float *)((tempArgumentList + 0)->data);
                    int32_t tempMaximum = *(float *)((tempArgumentList + 1)->data);
                    tempResult.value.type = VALUE_TYPE_NUMBER;
                    *(float *)(tempResult.value.data) = tempMinimum + (rand() % (tempMaximum - tempMinimum + 1));
                }
                if (tempFunction == SYMBOL_ABSOLUTE_VALUE) {
                    float tempNumber = *(float *)((tempArgumentList + 0)->data);
                    tempResult.value.type = VALUE_TYPE_NUMBER;
                    *(float *)(tempResult.value.data) = fabs(tempNumber);
                }
                if (tempFunction == SYMBOL_ROUND) {
                    float tempNumber = *(float *)((tempArgumentList + 0)->data);
                    tempResult.value.type = VALUE_TYPE_NUMBER;
                    *(float *)(tempResult.value.data) = floor(tempNumber + 0.5);
                }
                if (tempFunction == SYMBOL_FLOOR) {
                    float tempNumber = *(float *)((tempArgumentList + 0)->data);
                    tempResult.value.type = VALUE_TYPE_NUMBER;
                    *(float *)(tempResult.value.data) = floor(tempNumber);
                }
                if (tempFunction == SYMBOL_CEILING) {
                    float tempNumber = *(float *)((tempArgumentList + 0)->data);
                    tempResult.value.type = VALUE_TYPE_NUMBER;
                    *(float *)(tempResult.value.data) = ceil(tempNumber);
                }
                if (tempFunction == SYMBOL_SINE) {
                    float tempNumber = *(float *)((tempArgumentList + 0)->data);
                    tempResult.value.type = VALUE_TYPE_NUMBER;
                    *(float *)(tempResult.value.data) = sin(tempNumber);
                }
                if (tempFunction == SYMBOL_COSINE) {
                    float tempNumber = *(float *)((tempArgumentList + 0)->data);
                    tempResult.value.type = VALUE_TYPE_NUMBER;
                    *(float *)(tempResult.value.data) = cos(tempNumber);
                }
                if (tempFunction == SYMBOL_TANGENT) {
                    float tempNumber = *(float *)((tempArgumentList + 0)->data);
                    tempResult.value.type = VALUE_TYPE_NUMBER;
                    *(float *)(tempResult.value.data) = tan(tempNumber);
                }
                if (tempFunction == SYMBOL_SQUARE_ROOT) {
                    float tempNumber = *(float *)((tempArgumentList + 0)->data);
                    if (tempNumber < 0.0) {
                        reportError(ERROR_MESSAGE_BAD_VALUE, tempStartCode);
                        tempResult.status = EVALUATION_STATUS_QUIT;
                        return tempResult;
                    }
                    tempResult.value.type = VALUE_TYPE_NUMBER;
                    *(float *)(tempResult.value.data) = sqrt(tempNumber);
                }
                if (tempFunction == SYMBOL_POWER) {
                    if ((tempArgumentList + 1)->type != VALUE_TYPE_NUMBER) {
                        reportError(ERROR_MESSAGE_BAD_ARGUMENT_TYPE, tempStartCode);
                        tempResult.status = EVALUATION_STATUS_QUIT;
                        return tempResult;
                    }
                    float tempNumber1 = *(float *)((tempArgumentList + 0)->data);
                    float tempNumber2 = *(float *)((tempArgumentList + 1)->data);
                    if (tempNumber1 < 0.0 && floor(tempNumber2) != tempNumber2) {
                        reportError(ERROR_MESSAGE_BAD_VALUE, tempStartCode);
                        tempResult.status = EVALUATION_STATUS_QUIT;
                        return tempResult;
                    }
                    tempResult.value.type = VALUE_TYPE_NUMBER;
                    *(float *)(tempResult.value.data) = pow(tempNumber1, tempNumber2);
                }
                if (tempFunction == SYMBOL_LOG) {
                    if ((tempArgumentList + 1)->type != VALUE_TYPE_NUMBER) {
                        reportError(ERROR_MESSAGE_BAD_ARGUMENT_TYPE, tempStartCode);
                        tempResult.status = EVALUATION_STATUS_QUIT;
                        return tempResult;
                    }
                    float tempNumber1 = *(float *)((tempArgumentList + 0)->data);
                    float tempNumber2 = *(float *)((tempArgumentList + 1)->data);
                    if (tempNumber1 <= 0.0 || tempNumber2 <= 0.0) {
                        reportError(ERROR_MESSAGE_BAD_VALUE, tempStartCode);
                        tempResult.status = EVALUATION_STATUS_QUIT;
                        return tempResult;
                    }
                    tempResult.value.type = VALUE_TYPE_NUMBER;
                    *(float *)(tempResult.value.data) = log(tempNumber1) / log(tempNumber2);
                }
            }
            if (tempFunction >= SYMBOL_PRINT && tempFunction <= SYMBOL_FILE_IMPORT) {
                if (tempFunction == SYMBOL_PRINT) {
                    int8_t tempResult2 = printValue(tempArgumentList + 0);
                    if (!tempResult2) {
                        if (errorMessage != NULL) {
                            errorCode = tempStartCode;
                        }
                        tempResult.status = EVALUATION_STATUS_QUIT;
                        return tempResult;
                    } else {
                        tempShouldDisplayRunning = true;
                    }
                }
                if (tempFunction == SYMBOL_REQUEST_STRING) {
                    volatile int16_t tempCheatSize = REQUEST_STRING_MAXIMUM_LENGTH + 1;
                    uint8_t tempText[tempCheatSize];
                    if (nativeStackHasCollision(100)) {
                        reportError(ERROR_MESSAGE_EXPRESSION_TOO_COMPLEX, tempStartCode);
                        tempResult.status = EVALUATION_STATUS_QUIT;
                        return tempResult;
                    }
                    tempText[0] = 0;
                    initializeTextEditor(tempText, REQUEST_STRING_MAXIMUM_LENGTH, false);
                    int8_t tempResult2 = runTextEditor();
                    if (!tempResult2) {
                        tempResult.status = EVALUATION_STATUS_QUIT;
                    } else {
                        tempResult.value.type = VALUE_TYPE_STRING;
                        int8_t *tempString = createString(tempText);
                        if (tempString == NULL) {
                            reportError(ERROR_MESSAGE_STACK_HEAP_COLLISION, tempStartCode);
                            tempResult.status = EVALUATION_STATUS_QUIT;
                            return tempResult;
                        }
                        *(int8_t **)(tempResult.value.data) = tempString;
                        tempShouldDisplayRunning = true;
                    }
                }
                if (tempFunction == SYMBOL_REQUEST_NUMBER) {
                    volatile int16_t tempCheatSize = REQUEST_NUMBER_MAXIMUM_LENGTH + 1;
                    uint8_t tempText[tempCheatSize];
                    tempText[0] = 0;
                    initializeTextEditor(tempText, REQUEST_NUMBER_MAXIMUM_LENGTH, true);
                    int8_t tempResult2 = runTextEditor();
                    if (!tempResult2) {
                        tempResult.status = EVALUATION_STATUS_QUIT;
                    } else {
                        tempResult.value.type = VALUE_TYPE_NUMBER;
                        *(float *)(tempResult.value.data) = convertTextToFloat(tempText);
                        tempShouldDisplayRunning = true;
                    }
                }
                if (tempFunction == SYMBOL_MENU) {
                    if ((tempArgumentList + 0)->type != VALUE_TYPE_STRING || (tempArgumentList + 1)->type != VALUE_TYPE_LIST) {
                        reportError(ERROR_MESSAGE_BAD_ARGUMENT_TYPE, tempStartCode);
                        tempResult.status = EVALUATION_STATUS_QUIT;
                        return tempResult;
                    }
                    int8_t *tempTitle = *(int8_t **)((tempArgumentList + 0)->data);
                    int8_t *tempList = *(int8_t **)((tempArgumentList + 1)->data);
                    int8_t tempResult2 = menu(tempTitle, tempList);
                    if (tempResult2 == MENU_RESULT_ERROR) {
                        errorCode = tempStartCode;
                        tempResult.status = EVALUATION_STATUS_QUIT;
                        return tempResult;
                    } else if (tempResult2 < MENU_RESULT_ESCAPE) {
                        tempResult.status = EVALUATION_STATUS_QUIT;
                        return tempResult;
                    } else {
                        tempResult.value.type = VALUE_TYPE_NUMBER;
                        *(float *)(tempResult.value.data) = tempResult2;
                        tempShouldDisplayRunning = true;
                    }
                }
                if (tempFunction == SYMBOL_FILE_EXISTS) {
                    if ((tempArgumentList + 0)->type != VALUE_TYPE_STRING) {
                        reportError(ERROR_MESSAGE_BAD_ARGUMENT_TYPE, tempStartCode);
                        tempResult.status = EVALUATION_STATUS_QUIT;
                        return tempResult;
                    }
                    int8_t *tempPointer = *(int8_t **)((tempArgumentList + 0)->data);
                    int8_t *tempString = *(int8_t **)tempPointer;
                    int32_t tempFile = fileFindByName(tempString + STRING_DATA_OFFSET);
                    tempResult.value.type = VALUE_TYPE_NUMBER;
                    *(float *)(tempResult.value.data) = (tempFile >= 0);
                }
                if (tempFunction == SYMBOL_FILE_SIZE) {
                    if ((tempArgumentList + 0)->type != VALUE_TYPE_STRING) {
                        reportError(ERROR_MESSAGE_BAD_ARGUMENT_TYPE, tempStartCode);
                        tempResult.status = EVALUATION_STATUS_QUIT;
                        return tempResult;
                    }
                    int8_t *tempPointer = *(int8_t **)((tempArgumentList + 0)->data);
                    int8_t *tempString = *(int8_t **)tempPointer;
                    int32_t tempFile = fileFindByName(tempString + STRING_DATA_OFFSET);
                    if (tempFile < 0) {
                        reportError(ERROR_MESSAGE_MISSING_FILE, tempStartCode);
                        tempResult.status = EVALUATION_STATUS_QUIT;
                        return tempResult;
                    }
                    int16_t tempSize;
                    readStorage(&tempSize, tempFile + FILE_SIZE_OFFSET, 2);
                    tempResult.value.type = VALUE_TYPE_NUMBER;
                    *(float *)(tempResult.value.data) = tempSize;
                }
                if (tempFunction == SYMBOL_FILE_CREATE) {
                    if ((tempArgumentList + 0)->type != VALUE_TYPE_STRING) {
                        reportError(ERROR_MESSAGE_BAD_ARGUMENT_TYPE, tempStartCode);
                        tempResult.status = EVALUATION_STATUS_QUIT;
                        return tempResult;
                    }
                    int8_t *tempPointer = *(int8_t **)((tempArgumentList + 0)->data);
                    int8_t *tempString = *(int8_t **)tempPointer;
                    int32_t tempFile = fileCreate(tempString + STRING_DATA_OFFSET);
                    if (tempFile < 0) {
                        errorCode = tempStartCode;
                        tempResult.status = EVALUATION_STATUS_QUIT;
                        return tempResult;
                    }
                }
                if (tempFunction == SYMBOL_FILE_DELETE) {
                    if ((tempArgumentList + 0)->type != VALUE_TYPE_STRING) {
                        reportError(ERROR_MESSAGE_BAD_ARGUMENT_TYPE, tempStartCode);
                        tempResult.status = EVALUATION_STATUS_QUIT;
                        return tempResult;
                    }
                    int8_t *tempPointer = *(int8_t **)((tempArgumentList + 0)->data);
                    int8_t *tempString = *(int8_t **)tempPointer;
                    int32_t tempFile = fileFindByName(tempString + STRING_DATA_OFFSET);
                    if (tempFile < 0) {
                        reportError(ERROR_MESSAGE_MISSING_FILE, tempStartCode);
                        tempResult.status = EVALUATION_STATUS_QUIT;
                        return tempResult;
                    }
                    uint8_t tempStatus = FILE_EXISTS_FALSE;
                    writeStorage(tempFile + FILE_EXISTS_OFFSET, &tempStatus, 1);
                }
                if (tempFunction == SYMBOL_FILE_SET_NAME) {
                    if ((tempArgumentList + 0)->type != VALUE_TYPE_STRING || (tempArgumentList + 1)->type != VALUE_TYPE_STRING) {
                        reportError(ERROR_MESSAGE_BAD_ARGUMENT_TYPE, tempStartCode);
                        tempResult.status = EVALUATION_STATUS_QUIT;
                        return tempResult;
                    }
                    int8_t *tempPointer1 = *(int8_t **)((tempArgumentList + 0)->data);
                    int8_t *tempPointer2 = *(int8_t **)((tempArgumentList + 1)->data);
                    int8_t *tempString1 = *(int8_t **)tempPointer1;
                    int8_t *tempString2 = *(int8_t **)tempPointer2;
                    int32_t tempFile = fileFindByName(tempString1 + STRING_DATA_OFFSET);
                    if (tempFile < 0) {
                        reportError(ERROR_MESSAGE_MISSING_FILE, tempStartCode);
                        tempResult.status = EVALUATION_STATUS_QUIT;
                        return tempResult;
                    }
                    int8_t tempSuccess = fileSetName(tempFile, tempString2 + STRING_DATA_OFFSET);
                    if (!tempSuccess) {
                        errorCode = tempStartCode;
                        tempResult.status = EVALUATION_STATUS_QUIT;
                        return tempResult;
                    }
                }
                if (tempFunction == SYMBOL_FILE_READ) {
                    if ((tempArgumentList + 0)->type != VALUE_TYPE_STRING || (tempArgumentList + 1)->type != VALUE_TYPE_NUMBER
                            || (tempArgumentList + 2)->type != VALUE_TYPE_NUMBER) {
                        reportError(ERROR_MESSAGE_BAD_ARGUMENT_TYPE, tempStartCode);
                        tempResult.status = EVALUATION_STATUS_QUIT;
                        return tempResult;
                    }
                    int8_t *tempPointer = *(int8_t **)((tempArgumentList + 0)->data);
                    float tempIndex = *(float *)((tempArgumentList + 1)->data);
                    float tempAmount = *(float *)((tempArgumentList + 2)->data);
                    int8_t *tempString = *(int8_t **)tempPointer;
                    int32_t tempFile = fileFindByName(tempString + STRING_DATA_OFFSET);
                    if (tempFile < 0) {
                        reportError(ERROR_MESSAGE_MISSING_FILE, tempStartCode);
                        tempResult.status = EVALUATION_STATUS_QUIT;
                        return tempResult;
                    }
                    int8_t *tempResult2 = fileRead(tempFile, tempIndex, tempAmount);
                    if (tempResult2 == NULL) {
                        errorCode = tempStartCode;
                        tempResult.status = EVALUATION_STATUS_QUIT;
                        return tempResult;
                    }
                    tempResult.value.type = VALUE_TYPE_STRING;
                    *(int8_t **)(tempResult.value.data) = tempResult2;
                }
                if (tempFunction == SYMBOL_FILE_WRITE) {
                    if ((tempArgumentList + 0)->type != VALUE_TYPE_STRING || (tempArgumentList + 1)->type != VALUE_TYPE_STRING) {
                        reportError(ERROR_MESSAGE_BAD_ARGUMENT_TYPE, tempStartCode);
                        tempResult.status = EVALUATION_STATUS_QUIT;
                        return tempResult;
                    }
                    int8_t *tempPointer1 = *(int8_t **)((tempArgumentList + 0)->data);
                    int8_t *tempPointer2 = *(int8_t **)((tempArgumentList + 1)->data);
                    int8_t *tempString1 = *(int8_t **)tempPointer1;
                    int8_t *tempString2 = *(int8_t **)tempPointer2;
                    int32_t tempFile = fileFindByName(tempString1 + STRING_DATA_OFFSET);
                    if (tempFile < 0) {
                        reportError(ERROR_MESSAGE_MISSING_FILE, tempStartCode);
                        tempResult.status = EVALUATION_STATUS_QUIT;
                        return tempResult;
                    }
                    int8_t tempSuccess = fileWrite(tempFile, tempString2 + STRING_DATA_OFFSET);
                    if (!tempSuccess) {
                        errorCode = tempStartCode;
                        tempResult.status = EVALUATION_STATUS_QUIT;
                        return tempResult;
                    }
                }
                if (tempFunction == SYMBOL_FILE_IMPORT) {
                    if ((tempArgumentList + 0)->type != VALUE_TYPE_STRING) {
                        reportError(ERROR_MESSAGE_BAD_ARGUMENT_TYPE, tempStartCode);
                        tempResult.status = EVALUATION_STATUS_QUIT;
                        return tempResult;
                    }
                    int8_t *tempPointer = *(int8_t **)((tempArgumentList + 0)->data);
                    int8_t *tempString = *(int8_t **)tempPointer;
                    int32_t tempFile = fileFindByName(tempString + STRING_DATA_OFFSET);
                    if (tempFile < 0) {
                        reportError(ERROR_MESSAGE_MISSING_FILE, tempStartCode);
                        tempResult.status = EVALUATION_STATUS_QUIT;
                        return tempResult;
                    }
                    expressionResult_t tempResult2 = runCode(tempFile + FILE_DATA_OFFSET);
                    if (tempResult2.status != EVALUATION_STATUS_NORMAL) {
                        tempResult.status = tempResult2.status;
                        return tempResult;
                    }
                }
            }
            if (tempFunction >= SYMBOL_NUMBER && tempFunction <= SYMBOL_EQUAL_REFERENCE) {
                if (tempFunction == SYMBOL_NUMBER) {
                    int8_t tempType = (tempArgumentList + 0)->type;
                    if (tempType == VALUE_TYPE_NUMBER) {
                        tempResult.value = tempArgumentList[0];
                    } else if (tempType == VALUE_TYPE_STRING) {
                        int8_t *tempPointer = *(int8_t **)((tempArgumentList + 0)->data);
                        int8_t *tempString = *(int8_t **)tempPointer;
                        tempResult.value.type = VALUE_TYPE_NUMBER;
                        *(float *)(tempResult.value.data) = convertTextToFloat(tempString + STRING_DATA_OFFSET);
                    } else {
                        reportError(ERROR_MESSAGE_BAD_ARGUMENT_TYPE, tempStartCode);
                        tempResult.status = EVALUATION_STATUS_QUIT;
                        return tempResult;
                    }
                }
                if (tempFunction == SYMBOL_STRING) {
                    int8_t tempType = (tempArgumentList + 0)->type;
                    if (tempType == VALUE_TYPE_NUMBER) {
                        volatile int16_t tempCheatSize = NUMBER_LITERAL_MAXIMUM_LENGTH + 1;
                        uint8_t tempBuffer[tempCheatSize];
                        convertFloatToText(tempBuffer, *(float *)((tempArgumentList + 0)->data));
                        tempResult.value.type = VALUE_TYPE_STRING;
                        int8_t *tempString = createString(tempBuffer);
                        if (tempString == NULL) {
                            reportError(ERROR_MESSAGE_STACK_HEAP_COLLISION, tempStartCode);
                            tempResult.status = EVALUATION_STATUS_QUIT;
                            return tempResult;
                        }
                        *(int8_t **)(tempResult.value.data) = tempString;
                    } else if (tempType == VALUE_TYPE_STRING) {
                        tempResult.value = tempArgumentList[0];
                    } else {
                        reportError(ERROR_MESSAGE_BAD_ARGUMENT_TYPE, tempStartCode);
                        tempResult.status = EVALUATION_STATUS_QUIT;
                        return tempResult;
                    }
                }
                if (tempFunction == SYMBOL_TYPE) {
                    tempResult.value.type = VALUE_TYPE_NUMBER;
                    *(float *)(tempResult.value.data) = (tempArgumentList + 0)->type;
                }
                if (tempFunction == SYMBOL_LENGTH) {
                    int8_t tempType = (tempArgumentList + 0)->type;
                    if (tempType == VALUE_TYPE_STRING) {
                        int8_t *tempPointer = *(int8_t **)((tempArgumentList + 0)->data);
                        int8_t *tempString = *(int8_t **)tempPointer;
                        tempResult.value.type = VALUE_TYPE_NUMBER;
                        *(float *)(tempResult.value.data) = *(int16_t *)(tempString + STRING_LENGTH_OFFSET);
                    } else if (tempType == VALUE_TYPE_LIST) {
                        int8_t *tempPointer = *(int8_t **)((tempArgumentList + 0)->data);
                        int8_t *tempList = *(int8_t **)tempPointer;
                        tempResult.value.type = VALUE_TYPE_NUMBER;
                        *(float *)(tempResult.value.data) = *(int16_t *)(tempList + LIST_LENGTH_OFFSET);
                    } else {
                        reportError(ERROR_MESSAGE_BAD_ARGUMENT_TYPE, tempStartCode);
                        tempResult.status = EVALUATION_STATUS_QUIT;
                        return tempResult;
                    }
                }
                if (tempFunction == SYMBOL_COPY) {
                    int8_t tempType = (tempArgumentList + 0)->type;
                    if (tempType == VALUE_TYPE_STRING) {
                        int8_t *tempPointer1 = *(int8_t **)((tempArgumentList + 0)->data);
                        int8_t *tempString1 = *(int8_t **)tempPointer1;
                        int16_t tempLength = *(int16_t *)(tempString1 + STRING_LENGTH_OFFSET);
                        int8_t *tempPointer2 = createEmptyString(tempLength);
                        if (tempPointer2 == NULL) {
                            reportError(ERROR_MESSAGE_STACK_HEAP_COLLISION, tempStartCode);
                            tempResult.status = EVALUATION_STATUS_QUIT;
                            return tempResult;
                        }
                        int8_t *tempString2 = *(int8_t **)tempPointer2;
                        memcpy(tempString2 + STRING_DATA_OFFSET, tempString1 + STRING_DATA_OFFSET, tempLength + 1);
                        tempResult.value.type = VALUE_TYPE_STRING;
                        *(int8_t **)(tempResult.value.data) = tempPointer2;
                    } else if (tempType == VALUE_TYPE_LIST) {
                        int8_t *tempPointer1 = *(int8_t **)((tempArgumentList + 0)->data);
                        int8_t *tempList1 = *(int8_t **)tempPointer1;
                        int16_t tempLength = *(int16_t *)(tempList1 + LIST_LENGTH_OFFSET);
                        int8_t *tempPointer2 = createEmptyList(tempLength);
                        if (tempPointer2 == NULL) {
                            reportError(ERROR_MESSAGE_STACK_HEAP_COLLISION, tempStartCode);
                            tempResult.status = EVALUATION_STATUS_QUIT;
                            return tempResult;
                        }
                        int8_t *tempList2 = *(int8_t **)tempPointer2;
                        memcpy(tempList2 + LIST_DATA_OFFSET, tempList1 + LIST_DATA_OFFSET, tempLength * sizeof(value_t));
                        tempResult.value.type = VALUE_TYPE_LIST;
                        *(int8_t **)(tempResult.value.data) = tempPointer2;
                    } else {
                        reportError(ERROR_MESSAGE_BAD_ARGUMENT_TYPE, tempStartCode);
                        tempResult.status = EVALUATION_STATUS_QUIT;
                        return tempResult;
                    }
                }
                if (tempFunction == SYMBOL_INSERT) {
                    if ((tempArgumentList + 1)->type != VALUE_TYPE_NUMBER) {
                        reportError(ERROR_MESSAGE_BAD_ARGUMENT_TYPE, tempStartCode);
                        tempResult.status = EVALUATION_STATUS_QUIT;
                        return tempResult;
                    }
                    int16_t index = *(float *)((tempArgumentList + 1)->data);
                    int8_t tempResult2 = insertValueIntoSequence(tempArgumentList + 0, index, tempArgumentList + 2);
                    if (!tempResult2) {
                        errorCode = tempStartCode;
                        tempResult.status = EVALUATION_STATUS_QUIT;
                        return tempResult;
                    }
                }
                if (tempFunction == SYMBOL_REMOVE) {
                    if ((tempArgumentList + 1)->type != VALUE_TYPE_NUMBER) {
                        reportError(ERROR_MESSAGE_BAD_ARGUMENT_TYPE, tempStartCode);
                        tempResult.status = EVALUATION_STATUS_QUIT;
                        return tempResult;
                    }
                    int16_t index = *(float *)((tempArgumentList + 1)->data);
                    int8_t tempResult2 = removeValueFromSequence(tempArgumentList + 0, index);
                    if (!tempResult2) {
                        errorCode = tempStartCode;
                        tempResult.status = EVALUATION_STATUS_QUIT;
                        return tempResult;
                    }
                }
                if (tempFunction == SYMBOL_SUBSEQUENCE) {
                    if ((tempArgumentList + 1)->type != VALUE_TYPE_NUMBER || (tempArgumentList + 2)->type != VALUE_TYPE_NUMBER) {
                        reportError(ERROR_MESSAGE_BAD_ARGUMENT_TYPE, tempStartCode);
                        tempResult.status = EVALUATION_STATUS_QUIT;
                        return tempResult;
                    }
                    int16_t tempStartIndex = *(float *)((tempArgumentList + 1)->data);
                    int16_t tempEndIndex = *(float *)((tempArgumentList + 2)->data);
                    tempResult.value = getSubsequenceFromSequence(tempArgumentList + 0, tempStartIndex, tempEndIndex);
                    if (tempResult.value.type == VALUE_TYPE_MISSING) {
                        errorCode = tempStartCode;
                        tempResult.status = EVALUATION_STATUS_QUIT;
                        return tempResult;
                    }
                }
                if (tempFunction == SYMBOL_INSERT_SUBSEQUENCE) {
                    if ((tempArgumentList + 1)->type != VALUE_TYPE_NUMBER) {
                        reportError(ERROR_MESSAGE_BAD_ARGUMENT_TYPE, tempStartCode);
                        tempResult.status = EVALUATION_STATUS_QUIT;
                        return tempResult;
                    }
                    int16_t index = *(float *)((tempArgumentList + 1)->data);
                    int8_t tempResult2 = insertSubsequenceIntoSequence(tempArgumentList + 0, index, tempArgumentList + 2);
                    if (!tempResult2) {
                        errorCode = tempStartCode;
                        tempResult.status = EVALUATION_STATUS_QUIT;
                        return tempResult;
                    }
                }
                if (tempFunction == SYMBOL_REMOVE_SUBSEQUENCE) {
                    if ((tempArgumentList + 1)->type != VALUE_TYPE_NUMBER || (tempArgumentList + 2)->type != VALUE_TYPE_NUMBER) {
                        reportError(ERROR_MESSAGE_BAD_ARGUMENT_TYPE, tempStartCode);
                        tempResult.status = EVALUATION_STATUS_QUIT;
                        return tempResult;
                    }
                    int16_t tempStartIndex = *(float *)((tempArgumentList + 1)->data);
                    int16_t tempEndIndex = *(float *)((tempArgumentList + 2)->data);
                    int8_t tempResult2 = removeSubsequenceFromSequence(tempArgumentList + 0, tempStartIndex, tempEndIndex);
                    if (!tempResult2) {
                        errorCode = tempStartCode;
                        tempResult.status = EVALUATION_STATUS_QUIT;
                        return tempResult;
                    }
                }
                if (tempFunction == SYMBOL_EQUAL_REFERENCE) {
                    int8_t tempType1 = (tempArgumentList + 0)->type;
                    int8_t tempType2 = (tempArgumentList + 1)->type;
                    if ((tempType1 != VALUE_TYPE_STRING && tempType1 != VALUE_TYPE_LIST) || tempType1 != tempType2) {
                        reportError(ERROR_MESSAGE_BAD_ARGUMENT_TYPE, tempStartCode);
                        tempResult.status = EVALUATION_STATUS_QUIT;
                        return tempResult;
                    }
                    tempResult.value.type = VALUE_TYPE_NUMBER;
                    *(float *)(tempResult.value.data) = (*(int8_t **)(tempArgumentList + 0)->data == *(int8_t **)((tempArgumentList + 1)->data));
                }
            }
            if (tempShouldDisplayRunning) {
                clearDisplay();
                displayTextFromProgMem(0, 0, MESSAGE_RUNNING);
            }
        } else if (isUnaryOperator(tempSymbol)) {
            code += 1;
            expressionResult_t tempResult2 = evaluateExpression(code, 0, false);
            firstTreasureTracker = &tempTreasureTracker;
            treasureTracker_t tempTreasureTracker2;
            initializeTreasureTracker(&tempTreasureTracker2, TREASURE_TYPE_VALUE, &(tempResult2.value));
            if (tempResult2.status != EVALUATION_STATUS_NORMAL) {
                tempResult.status = tempResult2.status;
                return tempResult;
            }
            if (tempResult2.value.type == VALUE_TYPE_MISSING) {
                reportError(ERROR_MESSAGE_MISSING_VALUE, tempStartCode);
                tempResult.status = EVALUATION_STATUS_QUIT;
                return tempResult;
            }
            code = tempResult2.nextCode;
            if (tempResult2.value.type != VALUE_TYPE_NUMBER) {
                reportError(ERROR_MESSAGE_BAD_OPERAND_TYPE, tempStartCode);
                tempResult.status = EVALUATION_STATUS_QUIT;
                return tempResult;
            }
            if (tempSymbol == '-') {
                tempResult.value.type = VALUE_TYPE_NUMBER;
                *(float *)&(tempResult.value.data) = -(*(float *)&(tempResult2.value.data));
            }
            if (tempSymbol == '!') {
                tempResult.value.type = VALUE_TYPE_NUMBER;
                *(float *)&(tempResult.value.data) = (*(float *)&(tempResult2.value.data) == 0.0);
            }
            if (tempSymbol == '~') {
                tempResult.value.type = VALUE_TYPE_NUMBER;
                *(float *)&(tempResult.value.data) = ~(int32_t)*(float *)&(tempResult2.value.data);
            }
            if (tempSymbol == SYMBOL_INCREMENT) {
                if (tempResult2.destination == NULL) {
                    reportError(ERROR_MESSAGE_BAD_DESTINATION, tempStartCode);
                    tempResult.status = EVALUATION_STATUS_QUIT;
                    return tempResult;
                }
                tempResult.destinationType = tempResult2.destinationType;
                tempResult.destination = tempResult2.destination;
                if (tempResult2.destinationType == DESTINATION_TYPE_VALUE) {
                    *(float *)(((value_t *)(tempResult.destination))->data) += 1.0;
                    tempResult.value.type = VALUE_TYPE_NUMBER;
                    tempResult.value = *(value_t *)(tempResult.destination);
                }
                if (tempResult2.destinationType == DESTINATION_TYPE_SYMBOL) {
                    *(uint8_t *)(tempResult.destination) += 1; 
                    tempResult.value.type = VALUE_TYPE_NUMBER;
                    *(float *)(tempResult.value.data) = *(uint8_t *)(tempResult.destination);
                }
            }
            if (tempSymbol == SYMBOL_DECREMENT) {
                if (tempResult2.destination == NULL) {
                    reportError(ERROR_MESSAGE_BAD_DESTINATION, tempStartCode);
                    tempResult.status = EVALUATION_STATUS_QUIT;
                    return tempResult;
                }
                tempResult.destinationType = tempResult2.destinationType;
                tempResult.destination = tempResult2.destination;
                if (tempResult2.destinationType == DESTINATION_TYPE_VALUE) {
                    *(float *)(((value_t *)(tempResult.destination))->data) -= 1.0;
                    tempResult.value.type = VALUE_TYPE_NUMBER;
                    tempResult.value = *(value_t *)(tempResult.destination);
                }
                if (tempResult2.destinationType == DESTINATION_TYPE_SYMBOL) {
                    *(uint8_t *)(tempResult.destination) -= 1; 
                    tempResult.value.type = VALUE_TYPE_NUMBER;
                    *(float *)(tempResult.value.data) = *(uint8_t *)(tempResult.destination);
                }
            }
        } else {
            reportError(ERROR_MESSAGE_BAD_START_OF_EXPRESSION, tempStartCode);
            tempResult.status = EVALUATION_STATUS_QUIT;
            return tempResult;
        }
        while (true) {
            uint8_t tempSymbol = readStorageInt8(code);
            if (tempSymbol == SYMBOL_INCREMENT) {
                code += 1;
                if (tempResult.value.type != VALUE_TYPE_NUMBER) {
                    reportError(ERROR_MESSAGE_BAD_OPERAND_TYPE, tempStartCode);
                    tempResult.status = EVALUATION_STATUS_QUIT;
                    return tempResult;
                }
                if (tempResult.destination == NULL) {
                    reportError(ERROR_MESSAGE_BAD_DESTINATION, tempStartCode);
                    tempResult.status = EVALUATION_STATUS_QUIT;
                    return tempResult;
                }
                if (tempResult.destinationType == DESTINATION_TYPE_VALUE) {
                    *(float *)(((value_t *)(tempResult.destination))->data) += 1.0;
                }
                if (tempResult.destinationType == DESTINATION_TYPE_SYMBOL) {
                    *(uint8_t *)(tempResult.destination) += 1;
                }
            } else if (tempSymbol == SYMBOL_DECREMENT) {
                code += 1;
                if (tempResult.value.type != VALUE_TYPE_NUMBER) {
                    reportError(ERROR_MESSAGE_BAD_OPERAND_TYPE, tempStartCode);
                    tempResult.status = EVALUATION_STATUS_QUIT;
                    return tempResult;
                }
                if (tempResult.destination == NULL) {
                    reportError(ERROR_MESSAGE_BAD_DESTINATION, tempStartCode);
                    tempResult.status = EVALUATION_STATUS_QUIT;
                    return tempResult;
                }
                if (tempResult.destinationType == DESTINATION_TYPE_VALUE) {
                    *(float *)(((value_t *)(tempResult.destination))->data) -= 1.0;
                }
                if (tempResult.destinationType == DESTINATION_TYPE_SYMBOL) {
                    *(uint8_t *)(tempResult.destination) -= 1;
                }
            } else if (tempSymbol == '[') {
                code += 1;
                if (tempResult.value.type == VALUE_TYPE_MISSING) {
                    reportError(ERROR_MESSAGE_MISSING_VALUE, tempStartCode);
                    tempResult.status = EVALUATION_STATUS_QUIT;
                    return tempResult;
                }
                expressionResult_t tempResult2 = evaluateExpression(code, 99, false);
                firstTreasureTracker = &tempTreasureTracker;
                treasureTracker_t tempTreasureTracker2;
                initializeTreasureTracker(&tempTreasureTracker2, TREASURE_TYPE_VALUE, &(tempResult2.value));
                if (tempResult2.status != EVALUATION_STATUS_NORMAL) {
                    tempResult.status = tempResult2.status;
                    return tempResult;
                }
                if (tempResult2.value.type == VALUE_TYPE_MISSING) {
                    reportError(ERROR_MESSAGE_MISSING_VALUE, tempStartCode);
                    tempResult.status = EVALUATION_STATUS_QUIT;
                    return tempResult;
                }
                code = tempResult2.nextCode;
                int8_t tempSymbol = readStorageInt8(code);
                if (tempSymbol != ']') {
                    reportError(ERROR_MESSAGE_MISSING_BRACKET, tempStartCode);
                    tempResult.status = EVALUATION_STATUS_QUIT;
                    return tempResult;
                }
                code += 1;
                if (tempResult2.value.type != VALUE_TYPE_NUMBER) {
                    reportError(ERROR_MESSAGE_BAD_OPERAND_TYPE, tempStartCode);
                    tempResult.status = EVALUATION_STATUS_QUIT;
                    return tempResult;
                }
                int16_t index = *(float *)(tempResult2.value.data);
                if (tempResult.value.type == VALUE_TYPE_LIST) {
                    int8_t *tempPointer = *(int8_t **)(tempResult.value.data);
                    int8_t *tempList = *(int8_t **)tempPointer;
                    int16_t tempLength = *(int16_t *)(tempList + LIST_LENGTH_OFFSET);
                    if (index < 0 || index >= tempLength) {
                        reportError(ERROR_MESSAGE_BAD_INDEX, tempStartCode);
                        tempResult.status = EVALUATION_STATUS_QUIT;
                        return tempResult;
                    }
                    value_t *tempValue = (value_t *)(tempList + LIST_DATA_OFFSET + index * sizeof(value_t));
                    tempResult.destinationType = DESTINATION_TYPE_VALUE;
                    *(value_t **)&(tempResult.destination) = tempValue;
                    tempResult.value = *tempValue;
                } else if (tempResult.value.type == VALUE_TYPE_STRING) {
                    int8_t *tempPointer = *(int8_t **)(tempResult.value.data);
                    int8_t *tempString = *(int8_t **)tempPointer;
                    int16_t tempLength = *(int16_t *)(tempString + STRING_LENGTH_OFFSET);
                    if (index < 0 || index >= tempLength) {
                        reportError(ERROR_MESSAGE_BAD_INDEX, tempStartCode);
                        tempResult.status = EVALUATION_STATUS_QUIT;
                        return tempResult;
                    }
                    uint8_t *tempSymbol = tempString + STRING_DATA_OFFSET + index;
                    tempResult.destinationType = DESTINATION_TYPE_SYMBOL;
                    tempResult.destination = tempSymbol;
                    tempResult.value.type = VALUE_TYPE_NUMBER;
                    *(float *)(tempResult.value.data) = *tempSymbol;
                } else {
                    reportError(ERROR_MESSAGE_BAD_OPERAND_TYPE, tempStartCode);
                    tempResult.status = EVALUATION_STATUS_QUIT;
                    return tempResult;
                }
            } else if (tempSymbol == ':' || tempSymbol == ';') {
                code += 1;
                if (tempResult.value.type == VALUE_TYPE_MISSING) {
                    reportError(ERROR_MESSAGE_MISSING_VALUE, tempStartCode);
                    tempResult.status = EVALUATION_STATUS_QUIT;
                    return tempResult;
                }
                if (tempResult.value.type != VALUE_TYPE_FUNCTION) {
                    reportError(ERROR_MESSAGE_BAD_OPERAND_TYPE, tempStartCode);
                    tempResult.status = EVALUATION_STATUS_QUIT;
                    return tempResult;
                }
                int32_t tempCode = *(int32_t *)(tempResult.value.data);
                treasureTracker_t tempTreasureTracker3;
                int8_t tempArgumentAmount = getCustomFunctionArgumentAmount(tempCode);
                int8_t *tempPreviousScope = localScope;
                {
                    value_t tempArgumentList[tempArgumentAmount];
                    if (nativeStackHasCollision(100)) {
                        reportError(ERROR_MESSAGE_EXPRESSION_TOO_COMPLEX, tempStartCode);
                        tempResult.status = EVALUATION_STATUS_QUIT;
                        return tempResult;
                    }
                    treasureTracker_t tempTreasureTracker2;
                    initializeTreasureTracker(&tempTreasureTracker2, TREASURE_TYPE_VALUE_ARRAY, tempArgumentList);
                    int8_t index = 0;
                    while (index < tempArgumentAmount) {
                        expressionResult_t tempResult2 = evaluateExpression(code, 99, false);
                        firstTreasureTracker = &tempTreasureTracker2;
                        if (tempResult2.status != EVALUATION_STATUS_NORMAL) {
                            tempResult.status = tempResult2.status;
                            return tempResult;
                        }
                        if (tempResult2.value.type == VALUE_TYPE_MISSING) {
                            reportError(ERROR_MESSAGE_MISSING_VALUE, tempStartCode);
                            tempResult.status = EVALUATION_STATUS_QUIT;
                            return tempResult;
                        }
                        tempArgumentList[index] = tempResult2.value;
                        tempTreasureTracker2.amount += 1;
                        code = tempResult2.nextCode;
                        uint8_t tempSymbol = readStorageInt8(code);
                        if (tempSymbol == ',') {
                            code += 1;
                        }
                        index += 1;
                    }
                    int16_t tempSize = *(int16_t *)(localScope + SCOPE_SIZE_OFFSET);
                    localScope += SCOPE_DATA_OFFSET + tempSize;
                    *(int16_t *)(localScope + SCOPE_SIZE_OFFSET) = 0;
                    *(int8_t **)(localScope + SCOPE_VARIABLE_OFFSET) = NULL;
                    *(branch_t **)(localScope + SCOPE_BRANCH_OFFSET) = NULL;
                    int8_t tempSuccess = pushBranch(BRANCH_ACTION_RUN, 0);
                    if (!tempSuccess) {
                        errorCode = tempStartCode;
                        tempResult.status = EVALUATION_STATUS_QUIT;
                        return tempResult;
                    }
                    firstTreasureTracker = &tempTreasureTracker;
                    initializeTreasureTracker(&tempTreasureTracker3, TREASURE_TYPE_SCOPE, localScope);
                    tempCode += 1;
                    index = 0;
                    while (index < tempArgumentAmount + 1) {
                        volatile int16_t tempCheatSize = VARIABLE_NAME_MAXIMUM_LENGTH + 1;
                        uint8_t tempBuffer[tempCheatSize];
                        int8_t tempIndex = 0;
                        while (true) {
                            uint8_t tempSymbol = readStorageInt8(tempCode);
                            if (tempSymbol == ',') {
                                tempCode += 1;
                                break;
                            }
                            if (tempSymbol == '\n' || tempSymbol == 0) {
                                break;
                            }
                            tempBuffer[tempIndex] = tempSymbol;
                            tempCode += 1;
                            tempIndex += 1;
                        }
                        tempBuffer[tempIndex] = 0;
                        if (index > 0) {
                            value_t *tempValue = createVariable(tempBuffer);
                            if (tempValue == NULL) {
                                reportError(ERROR_MESSAGE_STACK_HEAP_COLLISION, tempStartCode);
                                tempResult.status = EVALUATION_STATUS_QUIT;
                                return tempResult;
                            }
                            *tempValue = tempArgumentList[index - 1];
                        }
                        index += 1;
                    }
                }
                expressionResult_t tempResult2 = runCode(tempCode);
                firstTreasureTracker = &tempTreasureTracker3;
                if (tempResult2.status != EVALUATION_STATUS_NORMAL) {
                    tempResult.status = tempResult2.status;
                    return tempResult;
                }
                localScope = tempPreviousScope;
                tempResult.value = tempResult2.value;
            } else {
                int8_t tempHasFoundOperator = false;
                int8_t index = 0;
                while (index < sizeof(BINARY_OPERATOR_LIST)) {
                    uint8_t tempSymbol2 = pgm_read_byte(BINARY_OPERATOR_LIST + index);
                    if (tempSymbol == tempSymbol2) {
                        tempHasFoundOperator = true;
                        break;
                    }
                    index += 1;
                }
                if (tempHasFoundOperator) {
                    int8_t tempPrecedence = pgm_read_byte(BINARY_OPERATOR_PRECEDENCE_LIST + index);
                    if (tempPrecedence >= precedence) {
                        break;
                    }
                    code += 1;
                    expressionResult_t tempResult2 = evaluateExpression(code, tempPrecedence, false);
                    firstTreasureTracker = &tempTreasureTracker;
                    treasureTracker_t tempTreasureTracker2;
                    initializeTreasureTracker(&tempTreasureTracker2, TREASURE_TYPE_VALUE, &(tempResult2.value));
                    if (tempResult2.status != EVALUATION_STATUS_NORMAL) {
                        tempResult.status = tempResult2.status;
                        return tempResult;
                    }
                    if (tempResult2.value.type == VALUE_TYPE_MISSING) {
                        reportError(ERROR_MESSAGE_MISSING_VALUE, tempStartCode);
                        tempResult.status = EVALUATION_STATUS_QUIT;
                        return tempResult;
                    }
                    code = tempResult2.nextCode;
                    float tempOperand1Float = *(float *)&(tempResult.value.data);
                    float tempOperand2Float = *(float *)&(tempResult2.value.data);
                    int32_t tempOperand1Int = (int32_t)tempOperand1Float;
                    int32_t tempOperand2Int = (int32_t)tempOperand2Float;
                    if (tempSymbol >= SYMBOL_ADD_ASSIGN && tempSymbol <= SYMBOL_BITSHIFT_RIGHT_ASSIGN) {
                        if (tempResult.destination == NULL) {
                            reportError(ERROR_MESSAGE_BAD_DESTINATION, tempStartCode);
                            tempResult.status = EVALUATION_STATUS_QUIT;
                            return tempResult;
                        } else {
                            int8_t tempType;
                            float tempNumber;
                            if (tempResult.destinationType == DESTINATION_TYPE_VALUE) {
                                tempType = ((value_t *)(tempResult.destination))->type;
                                if (tempType == VALUE_TYPE_NUMBER) {
                                    tempNumber = *(float *)(((value_t *)(tempResult.destination))->data);
                                }
                            }
                            if (tempResult.destinationType == DESTINATION_TYPE_SYMBOL) {
                                tempType = VALUE_TYPE_NUMBER;
                                tempNumber = *(uint8_t *)(tempResult.destination);
                            }
                            if (tempSymbol == SYMBOL_ADD_ASSIGN) {
                                if (tempType == VALUE_TYPE_NUMBER) {
                                    if (tempResult2.value.type != VALUE_TYPE_NUMBER) {
                                        reportError(ERROR_MESSAGE_BAD_OPERAND_TYPE, tempStartCode);
                                        tempResult.status = EVALUATION_STATUS_QUIT;
                                        return tempResult;
                                    }
                                    tempNumber += tempOperand2Float;
                                } else if (tempType == VALUE_TYPE_STRING) {
                                    if (tempResult2.value.type != VALUE_TYPE_STRING) {
                                        reportError(ERROR_MESSAGE_BAD_OPERAND_TYPE, tempStartCode);
                                        tempResult.status = EVALUATION_STATUS_QUIT;
                                        return tempResult;
                                    }
                                    int8_t *tempPointer = *(int8_t **)(((value_t *)(tempResult.destination))->data);
                                    int8_t *tempString = *(int8_t **)tempPointer;
                                    int16_t tempLength = *(int16_t *)(tempString + STRING_LENGTH_OFFSET);
                                    int8_t tempResult3 = insertSubsequenceIntoSequence((value_t *)(tempResult.destination), tempLength, &(tempResult2.value));
                                    if (!tempResult3) {
                                        errorCode = tempStartCode;
                                        tempResult.status = EVALUATION_STATUS_QUIT;
                                        return tempResult;
                                    }
                                } else {
                                    reportError(ERROR_MESSAGE_BAD_OPERAND_TYPE, tempStartCode);
                                    tempResult.status = EVALUATION_STATUS_QUIT;
                                    return tempResult;
                                }
                            } else if (tempType != VALUE_TYPE_NUMBER || tempResult2.value.type != VALUE_TYPE_NUMBER) {
                                reportError(ERROR_MESSAGE_BAD_OPERAND_TYPE, tempStartCode);
                                tempResult.status = EVALUATION_STATUS_QUIT;
                                return tempResult;
                            }
                            if (tempSymbol == SYMBOL_SUBTRACT_ASSIGN) {
                                tempNumber -= tempOperand2Float;
                            }
                            if (tempSymbol == SYMBOL_MULTIPLY_ASSIGN) {
                                tempNumber *= tempOperand2Float;
                            }
                            if (tempSymbol == SYMBOL_DIVIDE_ASSIGN) {
                                if (tempOperand2Float == 0.0) {
                                    reportError(ERROR_MESSAGE_DIVIDE_BY_ZERO, tempStartCode);
                                    tempResult.status = EVALUATION_STATUS_QUIT;
                                    return tempResult;
                                }
                                tempNumber /= tempOperand2Float;
                            }
                            if (tempSymbol == SYMBOL_MODULUS_ASSIGN) {
                                if (tempOperand2Float == 0.0) {
                                    reportError(ERROR_MESSAGE_DIVIDE_BY_ZERO, tempStartCode);
                                    tempResult.status = EVALUATION_STATUS_QUIT;
                                    return tempResult;
                                }
                                tempNumber = tempOperand1Int % tempOperand2Int;
                            }
                            if (tempSymbol == SYMBOL_BOOLEAN_AND_ASSIGN) {
                                tempNumber = ((tempOperand1Float != 0.0) & (tempOperand2Float != 0.0));
                            }
                            if (tempSymbol == SYMBOL_BOOLEAN_OR_ASSIGN) {
                                tempNumber = ((tempOperand1Float != 0.0) | (tempOperand2Float != 0.0));
                            }
                            if (tempSymbol == SYMBOL_BOOLEAN_XOR_ASSIGN) {
                                tempNumber = ((tempOperand1Float != 0.0) ^ (tempOperand2Float != 0.0));
                            }
                            if (tempSymbol == SYMBOL_BITWISE_AND_ASSIGN) {
                                tempNumber = (tempOperand1Int & tempOperand2Int);
                            }
                            if (tempSymbol == SYMBOL_BITWISE_OR_ASSIGN) {
                                tempNumber = (tempOperand1Int | tempOperand2Int);
                            }
                            if (tempSymbol == SYMBOL_BITWISE_XOR_ASSIGN) {
                                tempNumber = (tempOperand1Int ^ tempOperand2Int);
                            }
                            if (tempSymbol == SYMBOL_BITSHIFT_LEFT_ASSIGN) {
                                tempNumber = (tempOperand1Int << tempOperand2Int);
                            }
                            if (tempSymbol == SYMBOL_BITSHIFT_RIGHT_ASSIGN) {
                                tempNumber = (tempOperand1Int >> tempOperand2Int);
                            }
                            if (tempType == VALUE_TYPE_NUMBER) {
                                if (tempResult.destinationType == DESTINATION_TYPE_VALUE) {
                                    *(float *)(((value_t *)(tempResult.destination))->data) = tempNumber;
                                }
                                if (tempResult.destinationType == DESTINATION_TYPE_SYMBOL) {
                                    *(uint8_t *)(tempResult.destination) = tempNumber;
                                }
                            }
                        }
                    } else {
                        if (tempSymbol != '=') {
                            if (tempResult.value.type == VALUE_TYPE_MISSING) {
                                reportError(ERROR_MESSAGE_MISSING_VALUE, tempStartCode);
                                tempResult.status = EVALUATION_STATUS_QUIT;
                                return tempResult;
                            }
                        }
                        if (tempSymbol == '+') {
                            if (tempResult.value.type == VALUE_TYPE_NUMBER) {
                                if (tempResult2.value.type != VALUE_TYPE_NUMBER) {
                                    reportError(ERROR_MESSAGE_BAD_OPERAND_TYPE, tempStartCode);
                                    tempResult.status = EVALUATION_STATUS_QUIT;
                                    return tempResult;
                                }
                                *(float *)&(tempResult.value.data) += tempOperand2Float;
                            } else if (tempResult.value.type == VALUE_TYPE_STRING) {
                                if (tempResult2.value.type != VALUE_TYPE_STRING) {
                                    reportError(ERROR_MESSAGE_BAD_OPERAND_TYPE, tempStartCode);
                                    tempResult.status = EVALUATION_STATUS_QUIT;
                                    return tempResult;
                                }
                                int8_t *tempPointer1 = *(int8_t **)(tempResult.value.data);
                                int8_t *tempPointer2 = *(int8_t **)(tempResult2.value.data);
                                int8_t *tempString1 = *(int8_t **)tempPointer1;
                                int8_t *tempString2 = *(int8_t **)tempPointer2;
                                int16_t tempLength1 = *(int16_t *)(tempString1 + STRING_LENGTH_OFFSET);
                                int16_t tempLength2 = *(int16_t *)(tempString2 + STRING_LENGTH_OFFSET);
                                int8_t *tempPointer3 = createEmptyString(tempLength1 + tempLength2);
                                if (tempPointer3 == NULL) {
                                    reportError(ERROR_MESSAGE_STACK_HEAP_COLLISION, tempStartCode);
                                    tempResult.status = EVALUATION_STATUS_QUIT;
                                    return tempResult;
                                }
                                int8_t *tempString3 = *(int8_t **)tempPointer3;
                                memcpy(tempString3 + STRING_DATA_OFFSET, tempString1 + STRING_DATA_OFFSET, tempLength1);
                                memcpy(tempString3 + STRING_DATA_OFFSET + tempLength1, tempString2 + STRING_DATA_OFFSET, tempLength2 + 1);
                                *(int8_t **)&(tempResult.value.data) = tempPointer3;
                            } else {
                                reportError(ERROR_MESSAGE_BAD_OPERAND_TYPE, tempStartCode);
                                tempResult.status = EVALUATION_STATUS_QUIT;
                                return tempResult;
                            }
                        } else if (tempSymbol == SYMBOL_EQUAL) {
                            if (tempResult.value.type == VALUE_TYPE_NUMBER) {
                                if (tempResult2.value.type != VALUE_TYPE_NUMBER) {
                                    reportError(ERROR_MESSAGE_BAD_OPERAND_TYPE, tempStartCode);
                                    tempResult.status = EVALUATION_STATUS_QUIT;
                                    return tempResult;
                                }
                                *(float *)&(tempResult.value.data) = (tempOperand1Float == tempOperand2Float);
                            } else if (tempResult.value.type == VALUE_TYPE_STRING) {
                                if (tempResult2.value.type != VALUE_TYPE_STRING) {
                                    reportError(ERROR_MESSAGE_BAD_OPERAND_TYPE, tempStartCode);
                                    tempResult.status = EVALUATION_STATUS_QUIT;
                                    return tempResult;
                                }
                                int8_t *tempPointer1 = *(int8_t **)(tempResult.value.data);
                                int8_t *tempPointer2 = *(int8_t **)(tempResult2.value.data);
                                tempResult.value.type = VALUE_TYPE_NUMBER;
                                *(float *)&(tempResult.value.data) = stringsAreEqual(tempPointer1, tempPointer2);
                            } else {
                                reportError(ERROR_MESSAGE_BAD_OPERAND_TYPE, tempStartCode);
                                tempResult.status = EVALUATION_STATUS_QUIT;
                                return tempResult;
                            }
                        } else if (tempSymbol == SYMBOL_NOT_EQUAL) {
                            if (tempResult.value.type == VALUE_TYPE_NUMBER) {
                                if (tempResult2.value.type != VALUE_TYPE_NUMBER) {
                                    reportError(ERROR_MESSAGE_BAD_OPERAND_TYPE, tempStartCode);
                                    tempResult.status = EVALUATION_STATUS_QUIT;
                                    return tempResult;
                                }
                                *(float *)&(tempResult.value.data) = (tempOperand1Float != tempOperand2Float);
                            } else if (tempResult.value.type == VALUE_TYPE_STRING) {
                                if (tempResult2.value.type != VALUE_TYPE_STRING) {
                                    reportError(ERROR_MESSAGE_BAD_OPERAND_TYPE, tempStartCode);
                                    tempResult.status = EVALUATION_STATUS_QUIT;
                                    return tempResult;
                                }
                                int8_t *tempPointer1 = *(int8_t **)(tempResult.value.data);
                                int8_t *tempPointer2 = *(int8_t **)(tempResult2.value.data);
                                tempResult.value.type = VALUE_TYPE_NUMBER;
                                *(float *)&(tempResult.value.data) = !stringsAreEqual(tempPointer1, tempPointer2);
                            } else {
                                reportError(ERROR_MESSAGE_BAD_OPERAND_TYPE, tempStartCode);
                                tempResult.status = EVALUATION_STATUS_QUIT;
                                return tempResult;
                            }
                        } else if (tempSymbol == '=') {
                            if (tempResult.destination == NULL) {
                                volatile int16_t tempCheatSize = VARIABLE_NAME_MAXIMUM_LENGTH + 1;
                                uint8_t tempBuffer[tempCheatSize];
                                int32_t tempCode = readStorageVariableName(tempBuffer, tempStartCode);
                                if (tempCode < 0) {
                                    errorCode = tempStartCode;
                                    tempResult.status = EVALUATION_STATUS_QUIT;
                                    return tempResult;
                                }
                                tempResult.destinationType = DESTINATION_TYPE_VALUE;
                                value_t *tempValue = createVariable(tempBuffer);
                                if (tempValue == NULL) {
                                    reportError(ERROR_MESSAGE_STACK_HEAP_COLLISION, tempStartCode);
                                    tempResult.status = EVALUATION_STATUS_QUIT;
                                    return tempResult;
                                }
                                *(value_t **)&(tempResult.destination) = tempValue;
                            }
                            if (tempResult.destinationType == DESTINATION_TYPE_VALUE) {
                                *(value_t *)(tempResult.destination) = tempResult2.value;
                            }
                            if (tempResult.destinationType == DESTINATION_TYPE_SYMBOL) {
                                if (tempResult2.value.type != VALUE_TYPE_NUMBER) {
                                    reportError(ERROR_MESSAGE_BAD_OPERAND_TYPE, tempStartCode);
                                    tempResult.status = EVALUATION_STATUS_QUIT;
                                    return tempResult;
                                }
                                *(uint8_t *)(tempResult.destination) = *(float *)(tempResult2.value.data);
                            }
                        } else if (tempResult.value.type != VALUE_TYPE_NUMBER || tempResult2.value.type != VALUE_TYPE_NUMBER) {
                            reportError(ERROR_MESSAGE_BAD_OPERAND_TYPE, tempStartCode);
                            tempResult.status = EVALUATION_STATUS_QUIT;
                            return tempResult;
                        }
                        if (tempSymbol == '-') {
                            *(float *)&(tempResult.value.data) -= tempOperand2Float;
                        }
                        if (tempSymbol == '*') {
                            *(float *)&(tempResult.value.data) *= tempOperand2Float;
                        }
                        if (tempSymbol == '/') {
                            if (tempOperand2Float == 0.0) {
                                reportError(ERROR_MESSAGE_DIVIDE_BY_ZERO, tempStartCode);
                                tempResult.status = EVALUATION_STATUS_QUIT;
                                return tempResult;
                            }
                            *(float *)&(tempResult.value.data) /= tempOperand2Float;
                        }
                        if (tempSymbol == '%') {
                            if (tempOperand2Int == 0) {
                                reportError(ERROR_MESSAGE_DIVIDE_BY_ZERO, tempStartCode);
                                tempResult.status = EVALUATION_STATUS_QUIT;
                                return tempResult;
                            }
                            *(float *)&(tempResult.value.data) = tempOperand1Int % tempOperand2Int;
                        }
                        if (tempSymbol == SYMBOL_BOOLEAN_AND) {
                            *(float *)&(tempResult.value.data) = ((tempOperand1Float != 0.0) & (tempOperand2Float != 0.0));
                        }
                        if (tempSymbol == SYMBOL_BOOLEAN_OR) {
                            *(float *)&(tempResult.value.data) = ((tempOperand1Float != 0.0) | (tempOperand2Float != 0.0));
                        }
                        if (tempSymbol == SYMBOL_BOOLEAN_XOR) {
                            *(float *)&(tempResult.value.data) = ((tempOperand1Float != 0.0) ^ (tempOperand2Float != 0.0));
                        }
                        if (tempSymbol == '&') {
                            *(float *)&(tempResult.value.data) = (tempOperand1Int & tempOperand2Int);
                        }
                        if (tempSymbol == '|') {
                            *(float *)&(tempResult.value.data) = (tempOperand1Int | tempOperand2Int);
                        }
                        if (tempSymbol == '^') {
                            *(float *)&(tempResult.value.data) = (tempOperand1Int ^ tempOperand2Int);
                        }
                        if (tempSymbol == SYMBOL_BITSHIFT_LEFT) {
                            *(float *)&(tempResult.value.data) = (tempOperand1Int << tempOperand2Int);
                        }
                        if (tempSymbol == SYMBOL_BITSHIFT_RIGHT) {
                            *(float *)&(tempResult.value.data) = (tempOperand1Int >> tempOperand2Int);
                        }
                        if (tempSymbol == '>') {
                            *(float *)&(tempResult.value.data) = tempOperand1Float > tempOperand2Float;
                        }
                        if (tempSymbol == '<') {
                            *(float *)&(tempResult.value.data) = tempOperand1Float < tempOperand2Float;
                        }
                        if (tempSymbol == SYMBOL_GREATER_OR_EQUAL) {
                            *(float *)&(tempResult.value.data) = tempOperand1Float >= tempOperand2Float;
                        }
                        if (tempSymbol == SYMBOL_LESS_OR_EQUAL) {
                            *(float *)&(tempResult.value.data) = tempOperand1Float <= tempOperand2Float;
                        }
                    }
                } else {
                    break;
                }
            }
            firstTreasureTracker = &tempTreasureTracker;
        }
    }
    tempResult.nextCode = code;
    return tempResult;
}

static expressionResult_t runCode(int32_t address) {
    treasureTracker_t *tempTreasureTracker = firstTreasureTracker;
    while (true) {
        uint8_t tempSymbol = readStorageInt8(address);
        if (tempSymbol == '\n') {
            address += 1;
        } else if (tempSymbol == 0) {
            expressionResult_t tempResult;
            tempResult.status = EVALUATION_STATUS_NORMAL;
            tempResult.destination = NULL;
            tempResult.value.type = VALUE_TYPE_MISSING;
            return tempResult;
        } else {
            expressionResult_t tempResult = evaluateExpression(address, 99, true);
            firstTreasureTracker = tempTreasureTracker;
            if (tempResult.status == EVALUATION_STATUS_QUIT) {
                return tempResult;
            }
            if (tempResult.status == EVALUATION_STATUS_RETURN) {
                tempResult.status = EVALUATION_STATUS_NORMAL;
                return tempResult;
            }
            commandsSinceMarkAndSweep += 1;
            address = tempResult.nextCode;
        }
        if (allocationsSinceMarkAndSweep > 5 || commandsSinceMarkAndSweep > 30) {
            markAndSweep();
        }
    }
}

static void runFile(int32_t address) {
    errorMessage = NULL;
    resetHeap();
    firstTreasureTracker = NULL;
    commandsSinceMarkAndSweep = 0;
    allocationsSinceMarkAndSweep = 0;
    int32_t tempCode = address + FILE_DATA_OFFSET;
    clearDisplay();
    displayTextFromProgMem(0, 0, MESSAGE_RUNNING);
    globalScope = memory;
    localScope = memory;
    *(int16_t *)(globalScope + SCOPE_SIZE_OFFSET) = 0;
    *(int8_t **)(globalScope + SCOPE_VARIABLE_OFFSET) = NULL;
    *(branch_t **)(globalScope + SCOPE_BRANCH_OFFSET) = NULL;
    int8_t tempSuccess = pushBranch(BRANCH_ACTION_RUN, 0);
    if (!tempSuccess) {
        return;
    }
    treasureTracker_t tempTreasureTracker;
    initializeTreasureTracker(&tempTreasureTracker, TREASURE_TYPE_SCOPE, localScope);
    runCode(tempCode);
    resetHeap();
    localScope = NULL;
    if (errorMessage) {
        printTextFromProgMem(errorMessage);
        int32_t tempFileDataStartAddress = errorCode - errorCode % FILE_ENTRY_SIZE + FILE_DATA_OFFSET;
        int32_t tempStartAddress = errorCode;
        int32_t tempEndAddress = errorCode;
        while (tempStartAddress > tempFileDataStartAddress) {
            uint8_t tempSymbol = readStorageInt8(tempStartAddress - 1);
            if (tempSymbol == '\n') {
                break;
            }
            tempStartAddress -= 1;
        }
        while (true) {
            uint8_t tempSymbol = readStorageInt8(tempEndAddress);
            if (tempSymbol == '\n' || tempSymbol == 0) {
                break;
            }
            tempEndAddress += 1;
        }
        int16_t tempLength = tempEndAddress - tempStartAddress;
        uint8_t tempBuffer[tempLength + 1];
        readStorage(tempBuffer, tempStartAddress, tempLength);
        tempBuffer[tempLength] = 0;
        printText(tempBuffer);
    }
}

static void editFile(int32_t address) {
    int16_t tempSize;
    readStorage(&tempSize, address + FILE_SIZE_OFFSET, 2);
    readStorage(memory, address + FILE_DATA_OFFSET, tempSize + 1);
    initializeTextEditor(memory, FILE_MAXIMUM_SIZE, false);
    while (true) {
        runTextEditor();
        int8_t tempShouldQuitEditor = false;
        while (true) {
            int8_t tempResult = menuFromProgMem(MENU_TITLE_TEXT_EDITOR, MENU_TEXT_EDITOR, sizeof(MENU_TEXT_EDITOR) / sizeof(*MENU_TEXT_EDITOR));
            if (tempResult < 0) {
                break;
            }
            if (tempResult == 0) {
                clearDisplay();
                displayTextFromProgMem(0, 0, MESSAGE_SAVING);
                tempSize = strlen(memory);
                writeStorage(address + FILE_SIZE_OFFSET, &tempSize, 2);
                writeStorage(address + FILE_DATA_OFFSET, memory, tempSize + 1);
                printTextFromProgMem(MESSAGE_FILE_SAVED);
            }
            if (tempResult == 1) {
                tempShouldQuitEditor = true;
                break;
            }
        }
        if (tempShouldQuitEditor) {
            break;
        }
    }
}

static void promptFileAction(int32_t address) {
    while (true) {
        int8_t tempResult;
        {
            volatile int16_t tempCheatSize = FILE_NAME_MAXIMUM_LENGTH + 1;
            int8_t tempName[tempCheatSize];
            readStorage(tempName, address + FILE_NAME_OFFSET, FILE_NAME_MAXIMUM_LENGTH + 1);
            int8_t *tempTitle = createString(tempName);
            tempResult = menuWithOptionsFromProgMem(tempTitle, MENU_FILE, sizeof(MENU_FILE) / sizeof(*MENU_FILE));
            deallocatePointer(tempTitle);
        }
        if (tempResult < 0) {
            break;
        }
        if (tempResult == 0) {
            runFile(address);
        }
        if (tempResult == 1) {
            editFile(address);
        }
        if (tempResult == 2) {
            promptRenameFile(address);
        }
        if (tempResult == 3) {
            int8_t tempResult = promptDeleteFile(address);
            if (tempResult) {
                break;
            }
        }
    }
}

static void mainMenu() {
    while (true) {
        int8_t tempFileAmount = 0;
        int32_t tempAddress = 0;
        while (tempAddress < STORAGE_SIZE) {
            uint8_t tempExists;
            readStorage(&tempExists, tempAddress + FILE_EXISTS_OFFSET, 1);
            if (tempExists == FILE_EXISTS_TRUE) {
                tempFileAmount += 1;
            }
            tempAddress += FILE_ENTRY_SIZE;
        }
        int8_t tempListLength = tempFileAmount + 1;
        int8_t *tempList = createEmptyList(tempListLength);
        value_t *tempListContents = (value_t *)(*(int8_t **)tempList + LIST_DATA_OFFSET);
        int8_t *tempString = createStringFromProgMem(MENU_OPTION_CREATE_FILE);
        value_t *tempValue = tempListContents + 0;
        tempValue->type = VALUE_TYPE_STRING;
        *(int8_t **)&(tempValue->data) = tempString;
        int8_t index = 1;
        tempAddress = 0;
        while (tempAddress < STORAGE_SIZE) {
            uint8_t tempExists;
            readStorage(&tempExists, tempAddress + FILE_EXISTS_OFFSET, 1);
            if (tempExists == FILE_EXISTS_TRUE) {
                volatile int16_t tempCheatSize = FILE_NAME_MAXIMUM_LENGTH + 1;
                int8_t tempName[tempCheatSize];
                readStorage(tempName, tempAddress + FILE_NAME_OFFSET, FILE_NAME_MAXIMUM_LENGTH + 1);
                int8_t *tempString = createString(tempName);
                value_t *tempValue = tempListContents + index;
                tempValue->type = VALUE_TYPE_STRING;
                *(int8_t **)&(tempValue->data) = tempString;
                index += 1;
            }
            tempAddress += FILE_ENTRY_SIZE;
        }
        int8_t tempResult = menuWithTitleFromProgMem(MENU_TITLE_FILE_LIST, tempList);
        index = 0;
        while (index < tempListLength) {
            value_t *tempValue = tempListContents + index;
            int8_t *tempString = *(int8_t **)&(tempValue->data);
            deallocatePointer(tempString);
            index += 1;
        }
        deallocatePointer(tempList);
        if (tempResult == 0) {
            promptCreateFile();
        } else if (tempResult > 0) {
            int8_t index = 1;
            tempAddress = 0;
            while (tempAddress < STORAGE_SIZE) {
                uint8_t tempExists;
                readStorage(&tempExists, tempAddress + FILE_EXISTS_OFFSET, 1);
                if (tempExists == FILE_EXISTS_TRUE) {
                    if (index == tempResult) {
                        break;
                    }
                    index += 1;
                }
                tempAddress += FILE_ENTRY_SIZE;
            }
            promptFileAction(tempAddress);
        }
    }
}

void appendSymbolToTestProgramFile(uint8_t symbol) {
    int16_t tempSize;
    readStorage(&tempSize, testProgramFile + FILE_SIZE_OFFSET, 2);
    writeStorage(testProgramFile + FILE_DATA_OFFSET + tempSize, &symbol, 1);
    tempSize += 1;
    int8_t tempData = 0;
    writeStorage(testProgramFile + FILE_DATA_OFFSET + tempSize, &tempData, 1);
    writeStorage(testProgramFile + FILE_SIZE_OFFSET, &tempSize, 2);
}

void loadTestExpression(int8_t *text) {
    int32_t index = 0;
    while (true) {
        uint8_t tempSymbol;
        int8_t tempHasFoundSymbol = false;
        int8_t tempSelectedSymbolLength = 0;
        int16_t tempIndex = 0;
        while (tempIndex < sizeof(SYMBOL_TEXT_LIST) / sizeof(*SYMBOL_TEXT_LIST)) {
            const int8_t *tempSymbolText = SYMBOL_TEXT_LIST[tempIndex];
            int8_t tempLength = strlen(tempSymbolText);
            if (tempLength > tempSelectedSymbolLength) {
                if (memcmp(text + index, tempSymbolText, tempLength) == 0) {
                    tempSymbol = 128 + tempIndex;
                    tempHasFoundSymbol = true;
                    tempSelectedSymbolLength = tempLength;
                }
            }
            tempIndex += 1;
        }
        if (tempHasFoundSymbol) {
            index += tempSelectedSymbolLength;
        } else {
            tempSymbol = text[index];
            if (tempSymbol == 0) {
                break;
            }
            index += 1;
        }
        appendSymbolToTestProgramFile(tempSymbol);
    }
    appendSymbolToTestProgramFile('\n');
}

const int8_t *convertTestErrorToErrorMessage(int8_t *name) {
    if (strcmp(name, "BAD_START_OF_EXPRESSION") == 0) {
        return ERROR_MESSAGE_BAD_START_OF_EXPRESSION;
    }
    if (strcmp(name, "BAD_END_STATEMENT") == 0) {
        return ERROR_MESSAGE_BAD_END_STATEMENT;
    }
    if (strcmp(name, "BAD_CONTINUE_STATEMENT") == 0) {
        return ERROR_MESSAGE_BAD_CONTINUE_STATEMENT;
    }
    if (strcmp(name, "BAD_BREAK_STATEMENT") == 0) {
        return ERROR_MESSAGE_BAD_BREAK_STATEMENT;
    }
    if (strcmp(name, "BAD_ARGUMENT_TYPE") == 0) {
        return ERROR_MESSAGE_BAD_ARGUMENT_TYPE;
    }
    if (strcmp(name, "MISSING_APOSTROPHE") == 0) {
        return ERROR_MESSAGE_MISSING_APOSTROPHE;
    }
    if (strcmp(name, "MISSING_QUOTATION_MARK") == 0) {
        return ERROR_MESSAGE_MISSING_QUOTATION_MARK;
    }
    if (strcmp(name, "MISSING_BRACKET") == 0) {
        return ERROR_MESSAGE_MISSING_BRACKET;
    }
    if (strcmp(name, "MISSING_PARENTHESIS") == 0) {
        return ERROR_MESSAGE_MISSING_PARENTHESIS;
    }
    if (strcmp(name, "MISSING_COMMA") == 0) {
        return ERROR_MESSAGE_MISSING_COMMA;
    }
    if (strcmp(name, "STACK_HEAP_COLLISION") == 0) {
        return ERROR_MESSAGE_STACK_HEAP_COLLISION;
    }
    if (strcmp(name, "BAD_INDEX") == 0) {
        return ERROR_MESSAGE_BAD_INDEX;
    }
    if (strcmp(name, "MISSING_FILE") == 0) {
        return ERROR_MESSAGE_MISSING_FILE;
    }
    if (strcmp(name, "NAME_IS_TOO_LONG") == 0) {
        return ERROR_MESSAGE_NAME_IS_TOO_LONG;
    }
    if (strcmp(name, "STORAGE_IS_FULL") == 0) {
        return ERROR_MESSAGE_STORAGE_IS_FULL;
    }
    if (strcmp(name, "BAD_AMOUNT") == 0) {
        return ERROR_MESSAGE_BAD_AMOUNT;
    }
    if (strcmp(name, "FILE_EXISTS") == 0) {
        return ERROR_MESSAGE_FILE_EXISTS;
    }
    if (strcmp(name, "BAD_DESTINATION") == 0) {
        return ERROR_MESSAGE_BAD_DESTINATION;
    }
    if (strcmp(name, "BAD_OPERAND_TYPE") == 0) {
        return ERROR_MESSAGE_BAD_OPERAND_TYPE;
    }
    if (strcmp(name, "DIVIDE_BY_ZERO") == 0) {
        return ERROR_MESSAGE_DIVIDE_BY_ZERO;
    }
    if (strcmp(name, "BAD_VALUE") == 0) {
        return ERROR_MESSAGE_BAD_VALUE;
    }
    if (strcmp(name, "NOT_TOP_LEVEL") == 0) {
        return ERROR_MESSAGE_NOT_TOP_LEVEL;
    }
    if (strcmp(name, "MISSING_VALUE") == 0) {
        return ERROR_MESSAGE_MISSING_VALUE;
    }
    if (strcmp(name, "NUMBER_LITERAL_TOO_LONG") == 0) {
        return ERROR_MESSAGE_NUMBER_LITERAL_TOO_LONG;
    }
    if (strcmp(name, "VARIABLE_NAME_TOO_LONG") == 0) {
        return ERROR_MESSAGE_VARIABLE_NAME_TOO_LONG;
    }
    return NULL;
}

int8_t convertTestKeyToKey(int8_t *name) {
    if (strcmp(name, "CURSOR_LEFT") == 0) {
        return KEY_CURSOR_LEFT;
    }
    if (strcmp(name, "CURSOR_RIGHT") == 0) {
        return KEY_CURSOR_RIGHT;
    }
    if (strcmp(name, "CURSOR_UP") == 0) {
        return KEY_CURSOR_UP;
    }
    if (strcmp(name, "CURSOR_DOWN") == 0) {
        return KEY_CURSOR_DOWN;
    }
    if (strcmp(name, "SYMBOL_LEFT") == 0) {
        return KEY_SYMBOL_LEFT;
    }
    if (strcmp(name, "SYMBOL_RIGHT") == 0) {
        return KEY_SYMBOL_RIGHT;
    }
    if (strcmp(name, "SYMBOL_UP") == 0) {
        return KEY_SYMBOL_UP;
    }
    if (strcmp(name, "SYMBOL_DOWN") == 0) {
        return KEY_SYMBOL_DOWN;
    }
    if (strcmp(name, "SELECT_OPTION") == 0) {
        return KEY_SELECT_OPTION;
    }
    if (strcmp(name, "DELETE") == 0) {
        return KEY_DELETE;
    }
    if (strcmp(name, "NEWLINE") == 0) {
        return KEY_NEWLINE;
    }
    if (strcmp(name, "ESCAPE") == 0) {
        return KEY_ESCAPE;
    }
    if (strcmp(name, "FINISH") == 0) {
        return KEY_FINISH;
    }
    if (strcmp(name, "CLEAR_LINE") == 0) {
        return KEY_CLEAR_LINE;
    }
    return -1;
}

int8_t evaluateNextTestCommand() {
    int8_t output = -1;
    int32_t tempLength;
    size_t tempSize;
    int8_t *tempLine = NULL;
    tempLength = getline((char **)&tempLine, &tempSize, testSuiteFile);
    if (tempLength < 0) {
        testIsFinished = true;
        testSuiteIsFinished = true;
        return -1;
    }
    int8_t *tempCommand = tempLine;
    int8_t *tempArgument;
    int8_t tempHasArgument = true;
    int32_t index = 0;
    while (true) {
        int8_t tempCharacter = tempLine[index];
        if (tempCharacter == ' ') {
            tempLine[index] = 0;
            break;
        }
        if (tempCharacter == '\n') {
            tempLine[index] = 0;
            tempHasArgument = false;
            break;
        }
        if (tempCharacter == 0) {
            tempHasArgument = false;
            break;
        }
        index += 1;
    }
    if (tempHasArgument) {
        index += 1;
        tempArgument = tempLine + index;
        while (true) {
            int8_t tempCharacter = tempLine[index];
            if (tempCharacter == '\n') {
                tempLine[index] = 0;
                break;
            }
            if (tempCharacter == 0) {
                break;
            }
            index += 1;
        }
    }
    if (strcmp(tempCommand, "START_TEST") == 0) {
        printf("----------Running test: %s\n", tempArgument);
        testHasFailed = false;
        testIsFinished = false;
        testCount += 1;
        int32_t index = 0;
        while (index < STORAGE_SIZE) {
            testStorage[index] = 0xFF;
            index += 1;
        }
        testProgramFile = fileCreate("TEST");
        testOutput = NULL;
    }
    if (strcmp(tempCommand, "LOAD") == 0) {
        loadTestExpression(tempArgument);
    }
    if (strcmp(tempCommand, "RUN") == 0) {
        runFile(testProgramFile);
    }
    if (strcmp(tempCommand, "PRESS_KEY") == 0) {
        output = convertTestKeyToKey(tempArgument);
    }
    if (strcmp(tempCommand, "EXPECT_OUTPUT") == 0) {
        if (testOutput == NULL) {
            printf("Missing output\n");
            testHasFailed = true;
            testSuiteHasFailed = true;
        } else {
            if (strcmp(tempArgument, testOutput) != 0) {
                printf("Expected output: %s\n", tempArgument);
                printf("Found: %s\n", testOutput);
                testHasFailed = true;
                testSuiteHasFailed = true;
            }
            output = KEY_SELECT_OPTION;
        }
        testAssertionCount += 1;
    }
    if (strcmp(tempCommand, "EXPECT_ERROR") == 0) {
        const int8_t *tempErrorMessage = convertTestErrorToErrorMessage(tempArgument);
        if (tempErrorMessage == NULL) {
            printf("Could not find error with name \"%s\".\n", tempArgument);
            testHasFailed = true;
            testSuiteHasFailed = true;
            testIsFinished = true;
            testSuiteIsFinished = true;
        } else if (tempErrorMessage != errorMessage) {
            printf("Expected error: %s\n", tempErrorMessage);
            if (errorMessage == NULL) {
                printf("Found: NULL\n");
            } else {
                printf("Found: %s\n", errorMessage);
            }
            testHasFailed = true;
            testSuiteHasFailed = true;
        }
        testAssertionCount += 1;
    }
    if (strcmp(tempCommand, "STOP_TEST") == 0) {
        if (testHasFailed) {
            printf("FAIL\n");
        } else {
            printf("PASS\n");
        }
        testIsFinished = true;
    }
    free(tempLine);
    return output;
}

int8_t getTestKey() {
    while (!testIsFinished) {
        int8_t tempKey = evaluateNextTestCommand();
        if (tempKey >= 0) {
            return tempKey;
        }
    }
    return KEY_ESCAPE;
}

void runTestSuite() {
    isTesting = true;
    testCount = 0;
    testAssertionCount = 0;
    testSuiteFile = fopen(testSuiteFilePath, "r");
    while (!testSuiteIsFinished) {
        evaluateNextTestCommand();
    }
    fclose(testSuiteFile);
    printf("========================================\n");
    printf("Test suite result:\n");
    if (testSuiteHasFailed) {
        printf("> > > FAIL < < <\n");
    } else {
        printf("> > > PASS < < <\n");
    }
    printf("Test count: %d\n", testCount);
    printf("Assertion count: %d\n", testAssertionCount);
}

int main(int argc, const char *argv[]) {
    
    srand(time(NULL));
    
    if (argc == 2) {
        if (strcmp(argv[1], "test") == 0) {
            runTestSuite();
            return 0;
        }
    }
    
    storageFile = fopen(storageFilePath, "r+");
    if (storageFile == NULL) {
        storageFile = fopen(storageFilePath, "w");
        int8_t tempData = 0xFF;
        int32_t tempCount = 0;
        while (tempCount < STORAGE_SIZE) {
            fwrite(&tempData, 1, 1, storageFile);
            tempCount += 1;
        }
        fclose(storageFile);
        storageFile = fopen(storageFilePath, "r+");
    }
    
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
    
    displayTextFromProgMem(0, 0, MESSAGE_WELCOME);
    // Seed would be generated here.
    getKey();
    
    mainMenu();
    
    endwin();
    
}

