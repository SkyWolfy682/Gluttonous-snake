//��ʾȦȦ�ͷ���
//�������Ҽ��ƶ�
//relationship between mapsize and sleep time
//+˫��
//+��ɫ
//To do s maked by repeated '/'
////debugһ���޵�ģʽ
//�������'��'�������ַ�ʱ��'bug'

/*In the original version(1.0)*/
//food num is not limited

/*Change log*/
/*keywords are marked by '*'*/
//version 1.1.0: the *"world" is updated to be *soft, thus
//               the world size can be defined by users.
//version 1.1.1: use macro to malloc and check.
//version 1.1.2: change "BODY" and so on to "DEFAULT_BODYSTL" and so on.
//               use *struct to describe *settings(int column, row;)
//               (char body, land, food_style, border, head_style;)
//version 1.1.9: macro to controll cursor.
//               find that the user_defined two_dim array's
//               invoking macro is slightly wrong and then repaired.
//version 1.2.0: no longer use array "WORLD", just MOVE CURSOR to
//               erase and add char to the console directly.
//version ? ? ?: optimize the interface(color and so on).

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <windows.h>
#include <ctype.h>

/*default settings*/
#define DEFAULT_COLUMN 23 //alternative aptions: 46 49
#define DEFAULT_ROW 46    //alternative aptions: 92 94
//Deleted by version 1.1.2:
//        int column, row;
#define DEFAULT_BODYSTL '~'
#define DEFAULT_LANDSTL ' '   //alternative aption: '*'
#define DEFAULT_FOODSTL '`'   //alternative aption:'��'
#define DEFAULT_BORDERSTL '.' //�����޷����'��'�������......///////////////////////�պ���//////////
#define DEFAULT_HEADSTL '/'
/*end default settings*/

//Deleted by version 1.1.2:
//        char body, land, food_style, border, head_style;

#define BLANK 4
//In case that the snake may be genarated too close to the border

#define LAST (snake->capacity - 1)
#define LENGTH (snake->capacity)
#define FOOD_NUM (food->capacity)
#define LATEST_FOOD (food->capacity - 1)

/*version 1.1.2*/

typedef struct _style_setting
{
    char bodyStl;
    char landStl;
    char foodStl;
    char borderStl;
    char headStl;
} style_setting;

typedef struct _custom_setting
{
    COORD size;
    style_setting stl;
    int invinciblility; /////////////////////////////
} custom_setting;

custom_setting setting;
/*end version 1.1.2*/

typedef struct _soft_array
{
    int capacity;
    COORD array[];
} soft_array;

/*version 1.1.0
typedef struct _soft_map
{
    COORD size;
    char *map;
} soft_map;

#define WORLD(m, n) \
    (*(world.map + (m)*world.size.Y + (n)))
end version 1.1.0*/

COORD head /*coordinate of head of snake*/, temp_coord /*a multifunctional variety*/;
soft_array *snake, *food;
char stored_command, command;

#define EXIT_CHECK(pointer, sentence)                \
    if (pointer == NULL)                             \
    {                                                \
        puts(sentence "!!!");                        \
        puts("Programe will exit after 3 seconds."); \
        Sleep(3000);                                 \
        exit(EXIT_FAILURE);                          \
    }

#define PROMPT_AND_CHECK(prompt, default_value, format, var)          \
    printf(" Please enter " prompt ": " format "\n ", default_value); \
    while (scanf(format, &var) != 1)                                  \
    {                                                                 \
        puts(" \aWRONG INPUT!!!");                                    \
        printf(" Re_enter please.\n ");                               \
        setbuf(stdin, NULL);                                          \
    }                                                                 \
    setbuf(stdin, NULL);

#define CONFINE(var, operator, limit)              \
    if (!(var operator limit))                     \
    {                                              \
        puts("THE VALUE OF" #var                   \
             "HAS GONE BEYONGED GIVEN LIMITS!!!"); \
        var = limit;                               \
        puts("Now" #var "is adjusted to" #limit);  \
    }

/*In case that these "functions" followed are invoked 
only once, I decided to use macro. */

#if 0
#define PAVE()                                   \
    for (int i = 0; i < setting.size.X; i++)     \
    {                                            \
        for (int j = 0; j < setting.size.Y; j++) \
        {                                        \
            WORLD(i, j) = setting.stl.landStl;   \
        }                                        \
    }
#endif

#define INIT_EMERGE()                                \
    {                                                \
        for (int i = 0; i < setting.size.X + 2; i++) \
        {                                            \
            printf("%2c", setting.stl.borderStl);    \
        }                                            \
        printf("\n");                                \
        for (int i = 0; i < setting.size.Y; i++)     \
        {                                            \
            printf("%2c", setting.stl.borderStl);    \
            for (int j = 0; j < setting.size.X; j++) \
            {                                        \
                printf("%2c", setting.stl.landStl);  \
            }                                        \
            printf("%2c\n", setting.stl.borderStl);  \
        }                                            \
        for (int i = 0; i < setting.size.X + 2; i++) \
        {                                            \
            printf("%2c", setting.stl.borderStl);    \
        }                                            \
        puts("\n");                                  \
    }

#define GENERATE_SNAKE()                                           \
    snake = (soft_array *)malloc(sizeof(int) + 2 * sizeof(COORD)); \
    EXIT_CHECK(snake, "ALLOC FAILED")                              \
    LENGTH = 2;                                                    \
    COORD ini_tail;                                                \
    ini_tail.X = BLANK + rand() % (setting.size.X - 2 * BLANK);    \
    ini_tail.Y = BLANK + rand() % (setting.size.Y - 2 * BLANK);    \
    MOVE_CURSOR(COORD_TRANSFORME(ini_tail))                        \
    putchar(setting.stl.bodyStl);                                  \
    snake->array[0] = ini_tail;                                    \
    ini_tail.Y++;                                                  \
    MOVE_CURSOR(COORD_TRANSFORME(ini_tail));                       \
    putchar(setting.stl.headStl);                                  \
    head = snake->array[1] = ini_tail;

//����д����Ҳ���ԱȽ�stored_cammand��command�����ô˷���ע���ʼ����//////////////////////////////////////
#define HEAD_CRAWL()                                                                                                       \
    head = move(head, command);                                                                                            \
    if (command == '\0')                                                                                                   \
    {                                                                                                                      \
        head = move(head, stored_command); /*δ����W��A��S��D֮һʱ,����һ�ֵ�ָ���ƶ�*/                 \
    }                                                                                                                      \
    else                                                                                                                   \
    {                                                                                                                      \
        if (!memcmp(&head, &(snake->array[LAST - 1]), sizeof(COORD)))                                                      \
        {                                                                                                                  \
            head = move(head, stored_command);                                                                             \
            head = move(head, stored_command); /*If moved back, move twice by the last command to invalidate the operate*/ \
        }                                                                                                                  \
        else                                                                                                               \
        {                                                                                                                  \
            stored_command = command; /*������ָ����Ч����֮�洢����*/                                    \
        }                                                                                                                  \
    }

//set the position of cursor
#define MOVE_CURSOR(parameter_coordinate) \
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), (parameter_coordinate));

//hide/show cursor
#define VIEW_CURSOR(visibility)                          \
    CONSOLE_CURSOR_INFO cursor_info = {1, (visibility)}; \
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);

#define COORD_TRANSFORME(original_coord) \
    (temp_coord.X = 1 + 2 * (original_coord.X) + 2, temp_coord.Y = (original_coord.Y) + 1, temp_coord)

#if 0
COORD symmetric_coord_of(COORD original_coord)
{
    COORD symmetrically_transformed;
    symmetrically_transformed.X = original_coord.Y;
    symmetrically_transformed.Y = original_coord.X;
    return symmetrically_transformed;
}
#endif

int overlap(COORD *point, COORD *label, int former_adjustor, int later_adjustor)
{

    for (int i = former_adjustor; i < later_adjustor; i++)
    {
        if (memcmp(point, label + i, sizeof(COORD)) == 0)
        {
            return 1; //point������label��
        }
    }
    return 0; //point������label��
}

void generate_food(int adjustor)
{
    if (adjustor == 0)
    {
        food = (soft_array *)malloc(sizeof(int) + sizeof(COORD));
        FOOD_NUM = 0;
    }
    else
    {
        if (adjustor == 1)
        {
            food = (soft_array *)realloc(food, sizeof(int) + (FOOD_NUM + 1) * sizeof(COORD));
        }
        else
        {
            EXIT_CHECK(NULL, "WRONG ADJUSTOR")
        }
    }
    EXIT_CHECK(food, "ALLOC FAILED")

    FOOD_NUM++;

    do
    {
        food->array[LATEST_FOOD].X = rand() % setting.size.X;
        food->array[LATEST_FOOD].Y = rand() % setting.size.Y;
    } while (overlap(&(food->array[LATEST_FOOD]), snake->array, 0, LENGTH) || (overlap(&(food->array[LATEST_FOOD]), food->array, 0, FOOD_NUM - 1)));

    MOVE_CURSOR(COORD_TRANSFORME(food->array[LATEST_FOOD]))
    putchar(setting.stl.foodStl);
}

COORD move(COORD point, char instruct)
{
    switch (instruct)
    {
    case 'W':
        point.Y--;
        break;
    case 'A':
        point.X--;
        break;
    case 'S':
        point.Y++;
        break;
    case 'D':
        point.X++;
        break;
    default:
        command = '\0';
        break;
    }
    return point;
}

void crawl(void)
{

    MOVE_CURSOR(COORD_TRANSFORME(snake->array[LAST]))
    putchar(setting.stl.bodyStl);

    if (overlap(&head, food->array, 0, FOOD_NUM))
    {
        //�Ե�ʳ��ʱ
        FOOD_NUM--;
        snake = (soft_array *)realloc(snake, sizeof(int) + (LENGTH + 1) * sizeof(COORD));
        EXIT_CHECK(snake, "ALLOC_FAILED")
        LENGTH++;
    }
    else
    {
        //û�Ե�ʳ��ʱ
        MOVE_CURSOR(COORD_TRANSFORME(snake->array[0]))
        putchar(setting.stl.landStl);

        for (int i = 0; i < LAST; i++) //memmove/???////////////////////////////////////////////////////
        {
            snake->array[i] = snake->array[i + 1];
        }
    }

    snake->array[LAST] = head;

    MOVE_CURSOR(COORD_TRANSFORME(head))
    putchar(setting.stl.headStl);
}

int reach_border(void)
{
    if ((head.X >= 0) && (head.X < setting.size.X) && (head.Y >= 0) && (head.Y < setting.size.Y))
    {
        return 0; //safe
    }
    else
    {
        return 1; //OVERSTEP THE BOUNDARY
    }
}

int game_over(void) //�ظ���->����//////////////////////////
{
    if (overlap(&head, snake->array, 1, LENGTH - 3))
    {

        puts(" GAME OVER!\a");
        puts(" THE SNAKE CRASHED INTO ITSELF!!!");
        return 1;
    }
    if (reach_border())
    {
        puts(" GAME OVER!\a");
        puts(" THE SNAKE CRASHED AGAINST THE BORDER OF THE WORLD!!!");
        return 1;
    }
    return 0;
}

//rename & macro
void color(int c)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), c); //������ɫ
}

int main(void)
{
    int invincible_decider, replay, default_settings_decider;

    setting.stl.bodyStl = DEFAULT_BODYSTL;
    setting.stl.borderStl = DEFAULT_BORDERSTL;
    setting.stl.foodStl = DEFAULT_FOODSTL;
    setting.stl.headStl = DEFAULT_HEADSTL;
    setting.stl.landStl = DEFAULT_LANDSTL;

    setting.size.Y = DEFAULT_COLUMN;
    setting.size.X = DEFAULT_ROW;
start:
    PROMPT_AND_CHECK("even number(0) to SET parameters BY YOURSELF or odd number(1) to adapt DEFAULT SETTINGS.\n DEFAULT", 1, "%d", default_settings_decider)
    default_settings_decider = default_settings_decider % 2;
    if (default_settings_decider)
    {
        ;
    }
    else
    {
        PROMPT_AND_CHECK("the num of columns(9 <= column <= 46).\n DEFAULT COLUMN", DEFAULT_COLUMN, "%d", setting.size.Y)
        PROMPT_AND_CHECK("the num of rows(9 <= row <= 92).\n DEFAULT ROW", DEFAULT_ROW, "%d", setting.size.X)
        CONFINE(setting.size.Y, >=, 9)
        CONFINE(setting.size.Y, <=, 46)
        CONFINE(setting.size.X, >=, 9)
        CONFINE(setting.size.X, <=, 92)
        PROMPT_AND_CHECK("a character to represent head_style.\n DEFAULT HEAD", DEFAULT_HEADSTL, "%c", setting.stl.headStl)
        PROMPT_AND_CHECK("a character to represent body.\n DEFAULT BODY", DEFAULT_BODYSTL, "%c", setting.stl.bodyStl)
        PROMPT_AND_CHECK("a character to represent food_style.\n DEFAULT FOOD", DEFAULT_FOODSTL, "%c", setting.stl.foodStl)
        PROMPT_AND_CHECK("a character to represent land.\n DEFAULT LAND", DEFAULT_LANDSTL, "%c", setting.stl.landStl)
        PROMPT_AND_CHECK("a character to represent border.\n DEFAULT BORDER", DEFAULT_BORDERSTL, "%c", setting.stl.borderStl)
    }
    PROMPT_AND_CHECK("even number(0) for INVINCIBLE MODE or odd number(1) for NORMAL.\n DEFAULT", 1, "%d", invincible_decider)
    invincible_decider = invincible_decider % 2;
#if 0
    soft_map world; //���ܷ�ǰ�棬column��row��ֵ�󷽿ɶ���world


    world.size.X = setting.size.X;
    world.size.Y = setting.size.Y;
    world.map = (char *)malloc(world.size.X * world.size.Y * sizeof(char));
    EXIT_CHECK(world.map, "ALLOC FAILED")
#endif

    stored_command = 'D';

    srand((unsigned int)time(NULL));

    system("CLS");
    INIT_EMERGE();
    GENERATE_SNAKE()
    generate_food(0);

    do
    {
        if ((FOOD_NUM == 0) || (!(rand() % 3)))
        {
            generate_food(1);
        }

        VIEW_CURSOR((BOOL)0)

        if (_kbhit())
        {
            command = toupper(_getch());
        }
        setbuf(stdin, NULL);

        HEAD_CRAWL()
        crawl();

        if (invincible_decider && game_over())
        {
            break;
        }

        // printf(" The length is %d now.", LENGTH);

#if 1
        Sleep(200); //��ʹ��������
#endif
    } while (1);

    temp_coord.Y = setting.size.Y + 1;
    temp_coord.X = 1;
    MOVE_CURSOR(temp_coord)

    printf("\n The final length of the snake is %d.\n", LENGTH);
    free(snake);
    printf(" ARE YOU GOING TO REPLAY OR EXIT THE GAME?\n ");
    VIEW_CURSOR((BOOL)1)
    PROMPT_AND_CHECK("odd number(1) to REPLAY or even number(0) to EXIT.\n DEFAULT", 1, "%d", replay)
    replay = replay % 2;
    if (replay)
    {
        goto start;
    }
    else
    {
        puts(" GAME WILL EXIT AFTER 3 SECONDS");
        Sleep(3000);
        exit(0);
    }
}