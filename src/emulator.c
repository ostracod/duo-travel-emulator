
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
#define ALLOCATION_TYPE_LIST 4

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

#define EVALUATION_STATUS_NORMAL 0
#define EVALUATION_STATUS_QUIT 1
#define EVALUATION_STATUS_RETURN 2

#define SCOPE_SIZE_OFFSET 0
#define SCOPE_VARIABLE_OFFSET (SCOPE_SIZE_OFFSET + 2)
#define SCOPE_BRANCH_OFFSET (SCOPE_VARIABLE_OFFSET + sizeof(int8_t *))
#define SCOPE_DATA_OFFSET (SCOPE_BRANCH_OFFSET + sizeof(int8_t *))

#define VARIABLE_NAME_MAXIMUM_LENGTH 15

#define VARIABLE_NEXT_OFFSET 0
#define VARIABLE_VALUE_OFFSET (VARIABLE_NEXT_OFFSET + sizeof(int8_t *))
#define VARIABLE_NAME_OFFSET (VARIABLE_VALUE_OFFSET + sizeof(value_t))

#define BRANCH_ACTION_RUN 0
#define BRANCH_ACTION_IGNORE_SOFT 1
#define BRANCH_ACTION_IGNORE_HARD 2
#define BRANCH_ACTION_LOOP 3

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
const int8_t SYMBOL_TEXT_FILE_SET_NAME[] PROGMEM = "fSetName:";
const int8_t SYMBOL_TEXT_FILE_READ[] PROGMEM = "fRead:";
const int8_t SYMBOL_TEXT_FILE_WRITE[] PROGMEM = "fWrite:";
const int8_t SYMBOL_TEXT_FILE_IMPORT[] PROGMEM = "fImport:";

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
#define SYMBOL_PRINT 185
#define SYMBOL_REQUEST_STRING 186
#define SYMBOL_REQUEST_NUMBER 187
#define SYMBOL_TEXT_MENU 188
#define SYMBOL_FILE_EXISTS 189
#define SYMBOL_FILE_SIZE 190
#define SYMBOL_FILE_CREATE 191
#define SYMBOL_FILE_DELETE 192
#define SYMBOL_FILE_SET_NAME 193
#define SYMBOL_FILE_READ 194
#define SYMBOL_FILE_WRITE 195
#define SYMBOL_FILE_IMPORT 196

const int8_t UNARY_OPERATOR_LIST[] = {
    '-',
    SYMBOL_INCREMENT,
    SYMBOL_DECREMENT,
    '!',
    '~'
};

const int8_t BINARY_OPERATOR_LIST[] = {
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

const int8_t BINARY_OPERATOR_PRECEDENCE_LIST[] = {
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

const int8_t FUNCTION_ARGUMENT_AMOUNT_LIST[] = {
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
    1, // SYMBOL_PRINT
    0, // SYMBOL_REQUEST_STRING
    0, // SYMBOL_REQUEST_NUMBER
    2, // SYMBOL_TEXT_MENU
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

const int8_t SYMBOL_SET_NAME_LETTERS[] PROGMEM = "Lett";
const int8_t SYMBOL_SET_NAME_NUMBERS[] PROGMEM = "Num";
const int8_t SYMBOL_SET_NAME_PUNCTUATION[] PROGMEM = "Punc";
const int8_t SYMBOL_SET_NAME_OPERATORS[] PROGMEM = "Oper";
const int8_t SYMBOL_SET_NAME_ASSIGNMENT[] PROGMEM = "Asgn";
const int8_t SYMBOL_SET_NAME_CONTROL[] PROGMEM = "Ctrl";
const int8_t SYMBOL_SET_NAME_MATH[] PROGMEM = "Math";
const int8_t SYMBOL_SET_NAME_INPUT_OUTPUT[] PROGMEM = "I/O";
const int8_t SYMBOL_SET_NAME_VALUE[] PROGMEM = "Val";

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

const int8_t *MENU_FILE[] PROGMEM = {
    MENU_OPTION_RUN,
    MENU_OPTION_EDIT,
    MENU_OPTION_RENAME,
    MENU_OPTION_DELETE
};

const int8_t *MENU_TEXT_EDITOR[] PROGMEM = {
    MENU_OPTION_SAVE,
    MENU_OPTION_QUIT
};

const int8_t *MENU_FILE_DELETE[] PROGMEM = {
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

typedef struct value {
    int8_t type;
    int8_t data[sizeof(int8_t *) > sizeof(float) ? sizeof(int8_t *) : sizeof(float)];
} value_t;

typedef struct expressionResult {
    int8_t status;
    value_t *destination;
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

WINDOW *window;
int32_t windowWidth;
int32_t windowHeight;
int8_t displayBuffer[DISPLAY_WIDTH * DISPLAY_HEIGHT];
int8_t storageFilePath[] = "./storage.dat";
FILE *storageFile;

int8_t memory[1200];
int8_t *firstAllocation = NULL;
int8_t *textEditorText;
int16_t textEditorIndex;
int16_t textEditorLength;
int16_t textEditorMaximumLength;
int8_t textEditorSymbolSetIndex;
int8_t textEditorSymbolIndex[SYMBOL_SET_AMOUNT];
int8_t textEditorNumberOnly;
int8_t *globalScope;
int8_t *localScope;

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

void convertFloatToText(int8_t *destination, float number) {
    sprintf(destination, "%f", number);
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
    int8_t tempStartIndex = index;
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
    if (index != tempStartIndex) {
        strcpy(destination + index, destination + tempStartIndex);
    }
}

float convertTextToFloat(int8_t *text) {
    return atof(text);
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

void readStorage(void *destination, int32_t address, int32_t amount) {
    fseek(storageFile, address, SEEK_SET);
    fread(destination, 1, amount, storageFile);
}

void writeStorage(int32_t address, void *source, int32_t amount) {
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

static void resetHeap() {
    firstAllocation = NULL;
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

static int8_t *createString(int8_t *text) {
    int8_t *output = createEmptyString(strlen(text));
    int8_t *tempString = *(int8_t **)output;
    strcpy(tempString + STRING_DATA_OFFSET, text);
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
        convertFloatToText(tempBuffer, *(float *)&(value->data));
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
        int8_t tempBuffer[20];
        convertFloatToText(tempBuffer, *(float *)&(value->data));
        int8_t tempResult = printText(tempBuffer);
        return tempResult;
    } else if (value->type == VALUE_TYPE_STRING) {
        int8_t *tempString = *(int8_t **)&(value->data);
        int8_t tempResult = printStringAllocation(tempString);
        return tempResult;
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
        if (tempKey == KEY_SELECT_OPTION || tempKey == KEY_FINISH) {
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
                int8_t index = textEditorIndex;
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
    return -1;
}

static int8_t *fileRead(int32_t address, int16_t index, int16_t amount) {
    int16_t tempSize;
    readStorage(&tempSize, address + FILE_SIZE_OFFSET, 2);
    if (index < 0 || index + amount > tempSize) {
        return NULL;
    }
    int8_t *output = createEmptyString(amount);
    int8_t *tempString = *(int8_t **)output;
    readStorage(tempString + STRING_DATA_OFFSET, address + FILE_DATA_OFFSET + index, amount);
    return output;
}

static void fileWrite(int32_t address, int8_t *text) {
    int16_t tempSize = strlen(text);
    writeStorage(address + FILE_SIZE_OFFSET, &tempSize, 2);
    writeStorage(address + FILE_DATA_OFFSET, text, tempSize + 1);
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
    fileCreate(tempName);
    printTextFromProgMem(MESSAGE_FILE_CREATED);
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
    writeStorage(address + FILE_NAME_OFFSET, tempName, FILE_NAME_MAXIMUM_LENGTH + 1);
    printTextFromProgMem(MESSAGE_FILE_RENAMED);
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
    while (index < VARIABLE_NAME_MAXIMUM_LENGTH) {
        uint8_t tempSymbol = readStorageInt8(address);
        if (!((tempSymbol >= 'A' && tempSymbol <= 'Z') || (tempSymbol >= '0' && tempSymbol <= '9') || tempSymbol == '_')) {
            break;
        }
        destination[index] = tempSymbol;
        address += 1;
        index += 1;
    }
    destination[index] = 0;
    return address;
}

static value_t *findVariableValueByName(uint8_t *name) {
    int8_t *tempVariable = localScope + SCOPE_VARIABLE_OFFSET;
    while (tempVariable != NULL) {
        if (strcmp(name, tempVariable + VARIABLE_NAME_OFFSET) == 0) {
            return (value_t *)(tempVariable + VARIABLE_VALUE_OFFSET);
        }
        tempVariable = *(int8_t **)(tempVariable + VARIABLE_NEXT_OFFSET);
    }
    return NULL;
}

static value_t *createVariable(uint8_t *name) {
    int8_t tempLength = strlen(name);
    int16_t tempSize = *(int16_t *)(localScope + SCOPE_SIZE_OFFSET);
    int8_t *tempVariable = localScope + SCOPE_DATA_OFFSET + tempSize;
    tempSize += VARIABLE_NAME_OFFSET + tempLength + 1;
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
    *(int16_t *)(localScope + SCOPE_SIZE_OFFSET) = tempSize;
    branch_t *tempPreviousBranch = *(branch_t **)(localScope + SCOPE_BRANCH_OFFSET);
    if (tempPreviousBranch != NULL) {
        tempPreviousBranch->next = output;
    }
    output->previous = tempPreviousBranch;
    output->next = NULL;
    return output;
}

static void pushBranch(int8_t action, int32_t address) {
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
    tempNextBranch->action = action;
    tempNextBranch->address = address;
    *(branch_t **)(localScope + SCOPE_BRANCH_OFFSET) = tempNextBranch;
}

static void popBranch() {
    branch_t *tempBranch = *(branch_t **)(localScope + SCOPE_BRANCH_OFFSET);
    branch_t *tempPreviousBranch = tempBranch->previous;
    // TODO: Handle error if tempPreviousBranch is NULL.
    *(branch_t **)(localScope + SCOPE_BRANCH_OFFSET) = tempPreviousBranch;
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

static expressionResult_t runCode(int32_t address);

static expressionResult_t evaluateExpression(int32_t code, int8_t precedence, int8_t isTopLevel) {
    branch_t *tempBranch = *(branch_t **)(localScope + SCOPE_BRANCH_OFFSET);
    //mvprintw(0, 0, "%d %d      ", code, tempBranch->action);
    //getch();
    int32_t tempStartCode = code;
    uint8_t tempSymbol = readStorageInt8(code);
    expressionResult_t tempResult;
    tempResult.status = EVALUATION_STATUS_NORMAL;
    tempResult.destination = NULL;
    tempResult.value.type = VALUE_TYPE_MISSING;
    int8_t tempShouldRun = true;
    if (tempBranch->action == BRANCH_ACTION_IGNORE_SOFT || tempBranch->action == BRANCH_ACTION_IGNORE_HARD) {
        tempShouldRun = false;
        if (tempSymbol == SYMBOL_IF || tempSymbol == SYMBOL_WHILE || tempSymbol == SYMBOL_FUNCTION) {
            pushBranch(BRANCH_ACTION_IGNORE_HARD, 0);
            code = skipStorageLine(code);
        } else if (tempSymbol == SYMBOL_END) {
            popBranch();
            code = skipStorageLine(code);
        } else {
            code = skipStorageLine(code);
        }
    }
    if (tempShouldRun) {
        if ((tempSymbol >= '0' && tempSymbol <= '9') || tempSymbol == '.') {
            uint8_t tempBuffer[20];
            tempBuffer[0] = tempSymbol;
            code += 1;
            int8_t index = 1;
            while (true) {
                tempSymbol = readStorageInt8(code);
                if (!((tempSymbol >= '0' && tempSymbol <= '9') || tempSymbol == '.')) {
                    tempBuffer[index] = 0;
                    break;
                }
                tempBuffer[index] = tempSymbol;
                code += 1;
                index += 1;
            }
            tempResult.value.type = VALUE_TYPE_NUMBER;
            *(float *)&(tempResult.value.data) = convertTextToFloat(tempBuffer);
        } else if ((tempSymbol >= 'A' && tempSymbol <= 'Z') || tempSymbol == '_') {
            uint8_t tempBuffer[VARIABLE_NAME_MAXIMUM_LENGTH + 1];
            code = readStorageVariableName(tempBuffer, code);
            tempResult.destination = findVariableValueByName(tempBuffer);
            if (tempResult.destination != NULL) {
                tempResult.value = *(tempResult.destination);
            }
        } else if (tempSymbol >= FIRST_FUNCTION_SYMBOL && tempSymbol <= LAST_FUNCTION_SYMBOL) {
            uint8_t tempFunction = tempSymbol;
            code += 1;
            int8_t tempArgumentAmount = pgm_read_byte(FUNCTION_ARGUMENT_AMOUNT_LIST + (tempSymbol - FIRST_FUNCTION_SYMBOL));
            if (tempArgumentAmount < 0) {
                tempArgumentAmount = getCustomFunctionArgumentAmount(tempStartCode);
            }
            value_t tempArgumentList[tempArgumentAmount];
            int32_t tempExpressionList[tempArgumentAmount];
            int8_t index = 0;
            while (index < tempArgumentAmount) {
                tempExpressionList[index] = code;
                expressionResult_t tempResult2 = evaluateExpression(code, 99, false);
                if (tempResult2.status != EVALUATION_STATUS_NORMAL && tempResult2.status != EVALUATION_STATUS_RETURN) {
                    tempResult.status = tempResult2.status;
                    return tempResult;
                }
                tempArgumentList[index] = tempResult2.value;
                code = tempResult2.nextCode;
                if (index < tempArgumentAmount - 1) {
                    // TODO: Check for comma.
                    code += 1;
                }
                index += 1;
            }
            int8_t tempShouldDisplayRunning = false;
            if (tempFunction == SYMBOL_PRINT) {
                int8_t tempResult2 = printValue(tempArgumentList + 0);
                if (!tempResult2) {
                    tempResult.status = EVALUATION_STATUS_QUIT;
                } else {
                    tempShouldDisplayRunning = true;
                }
            }
            if (tempFunction == SYMBOL_RETURN) {
                tempResult.status = EVALUATION_STATUS_RETURN;
            }
            if (tempFunction == SYMBOL_RETURN_WITH_VALUE) {
                tempResult.status = EVALUATION_STATUS_RETURN;
                tempResult.value = tempArgumentList[0];
            }
            if (tempFunction == SYMBOL_IF) {
                if (*(float *)((tempArgumentList + 0)->data) == 0.0) {
                    pushBranch(BRANCH_ACTION_IGNORE_SOFT, 0);
                } else {
                    pushBranch(BRANCH_ACTION_RUN, 0);
                }
            }
            if (tempFunction == SYMBOL_WHILE) {
                if (*(float *)((tempArgumentList + 0)->data) == 0.0) {
                    pushBranch(BRANCH_ACTION_IGNORE_HARD, 0);
                } else {
                    pushBranch(BRANCH_ACTION_LOOP, tempStartCode);
                }
            }
            if (tempFunction == SYMBOL_END) {
                int8_t tempAction = tempBranch->action;
                int32_t tempAddress = tempBranch->address;
                popBranch();
                if (tempAction == BRANCH_ACTION_LOOP) {
                    code = tempAddress;
                }
            }
            if (tempFunction == SYMBOL_FUNCTION) {
                uint8_t tempBuffer[VARIABLE_NAME_MAXIMUM_LENGTH + 1];
                readStorageVariableName(tempBuffer, tempExpressionList[0]);
                value_t *tempValue = createVariable(tempBuffer);
                tempValue->type = VALUE_TYPE_FUNCTION;
                *(int32_t *)(tempValue->data) = tempStartCode;
                pushBranch(BRANCH_ACTION_IGNORE_HARD, 0);
            }
            if (tempShouldDisplayRunning) {
                clearDisplay();
                displayTextFromProgMem(0, 0, MESSAGE_RUNNING);
            }
        } else if (isUnaryOperator(tempSymbol)) {
            code += 1;
            expressionResult_t tempResult2 = evaluateExpression(code, 0, false);
            code = tempResult2.nextCode;
            if (tempSymbol == '!') {
                tempResult.value.type = VALUE_TYPE_NUMBER;
                if (*(float *)&(tempResult2.value.data) == 0.0) {
                    *(float *)&(tempResult.value.data) = 1.0;
                } else {
                    *(float *)&(tempResult.value.data) = 0.0;
                }
            }
        }
        while (true) {
            uint8_t tempSymbol = readStorageInt8(code);
            if (tempSymbol == SYMBOL_INCREMENT) {
                code += 1;
                *(float *)(tempResult.destination->data) += 1;
            } else if (tempSymbol == ':' || tempSymbol == ';') {
                code += 1;
                int32_t tempCode = *(int32_t *)(tempResult.value.data);
                int8_t tempArgumentAmount = getCustomFunctionArgumentAmount(tempCode);
                int8_t *tempPreviousScope = localScope;
                {
                    value_t tempArgumentList[tempArgumentAmount];
                    int8_t index = 0;
                    while (index < tempArgumentAmount) {
                        expressionResult_t tempResult2 = evaluateExpression(code, 99, false);
                        if (tempResult2.status != EVALUATION_STATUS_NORMAL && tempResult2.status != EVALUATION_STATUS_RETURN) {
                            tempResult.status = tempResult2.status;
                            return tempResult;
                        }
                        tempArgumentList[index] = tempResult2.value;
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
                    pushBranch(BRANCH_ACTION_RUN, 0);
                    tempCode += 1;
                    index = 0;
                    while (index < tempArgumentAmount + 1) {
                        uint8_t tempBuffer[VARIABLE_NAME_MAXIMUM_LENGTH + 1];
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
                            *tempValue = tempArgumentList[index - 1];
                        }
                        index += 1;
                    }
                }
                expressionResult_t tempResult2 = runCode(tempCode);
                if (tempResult2.status != EVALUATION_STATUS_NORMAL && tempResult2.status != EVALUATION_STATUS_RETURN) {
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
                    expressionResult_t tempResult2 = evaluateExpression(code + 1, tempPrecedence, false);
                    if (tempResult2.status != EVALUATION_STATUS_NORMAL && tempResult2.status != EVALUATION_STATUS_RETURN) {
                        tempResult.status = tempResult2.status;
                        return tempResult;
                    }
                    if (tempSymbol == '+') {
                        *(float *)&(tempResult.value.data) += *(float *)&(tempResult2.value.data);
                    }
                    if (tempSymbol == '*') {
                        *(float *)&(tempResult.value.data) *= *(float *)&(tempResult2.value.data);
                    }
                    if (tempSymbol == '>') {
                        *(float *)&(tempResult.value.data) = *(float *)&(tempResult.value.data) > *(float *)&(tempResult2.value.data);
                    }
                    if (tempSymbol == '<') {
                        *(float *)&(tempResult.value.data) = *(float *)&(tempResult.value.data) < *(float *)&(tempResult2.value.data);
                    }
                    if (tempSymbol == '=') {
                        if (tempResult.destination == NULL) {
                            // TODO: Make sure this is actually a variable.
                            uint8_t tempBuffer[VARIABLE_NAME_MAXIMUM_LENGTH + 1];
                            readStorageVariableName(tempBuffer, tempStartCode);
                            tempResult.destination = createVariable(tempBuffer);
                        }
                        *(tempResult.destination) = tempResult2.value;
                    }
                    code = tempResult2.nextCode;
                } else {
                    break;
                }
            }
        }
    }
    tempResult.nextCode = code;
    return tempResult;
}

static expressionResult_t runCode(int32_t address) {
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
            if (tempResult.status == EVALUATION_STATUS_QUIT || tempResult.status == EVALUATION_STATUS_RETURN) {
                return tempResult;
            }
            address = tempResult.nextCode;
        }
    }
}

static void runFile(int32_t address) {
    resetHeap();
    int32_t tempCode = address + FILE_DATA_OFFSET;
    clearDisplay();
    displayTextFromProgMem(0, 0, MESSAGE_RUNNING);
    globalScope = memory;
    localScope = memory;
    *(int16_t *)(globalScope + SCOPE_SIZE_OFFSET) = 0;
    *(int8_t **)(globalScope + SCOPE_VARIABLE_OFFSET) = NULL;
    *(branch_t **)(globalScope + SCOPE_BRANCH_OFFSET) = NULL;
    pushBranch(BRANCH_ACTION_RUN, 0);
    runCode(tempCode);
    resetHeap();
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
            int8_t tempName[FILE_NAME_MAXIMUM_LENGTH + 1];
            readStorage(tempName, address + FILE_NAME_OFFSET, FILE_NAME_MAXIMUM_LENGTH + 1);
            int8_t *tempTitle = createString(tempName);
            tempResult = menuWithOptionsFromProgMem(tempTitle, MENU_FILE, sizeof(MENU_FILE) / sizeof(*MENU_FILE));
            deallocate(tempTitle);
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
                int8_t tempName[FILE_NAME_MAXIMUM_LENGTH + 1];
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
            deallocate(tempString);
            index += 1;
        }
        deallocate(tempList);
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
    
    mainMenu();
    
    endwin();
    
}

