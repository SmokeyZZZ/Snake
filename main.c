#include <stdio.h>
#include <raylib.h>
#include <unistd.h>
#include <stdlib.h>

#define ROW_SIZE 30
#define WIDTH 700
#define HEIGHT 500

int apples =0 ;
char appleToString[15];
int bestScore=0;
char bestScoreToString[15];
typedef struct{
    int height;
    int width;
    int x;
    int y;
}Game;
typedef enum {
    LEFT,
    RIGHT,
    UP,
    DOWN,
    NONE
} Direction;

typedef struct{
    int x;
    int y;
}Apple;

typedef struct {
    int x;
    int y;
    Direction d;
    struct Player* next;
} Player;

//if there's a change of direction it changes the property of the player, otherwise it continues the movement as it was doing before the check
// firstly i need to check the direction
void checkDirection(Player* p){
    if(IsKeyPressed(KEY_D)||IsKeyPressed(KEY_RIGHT)){
        p->d=RIGHT;
    }
    if(IsKeyPressed(KEY_A)||IsKeyPressed(KEY_LEFT)){
        p->d=LEFT;
    }
    if (IsKeyPressed(KEY_S)||IsKeyPressed(KEY_DOWN)){
        p->d=DOWN;
    }
    if(IsKeyPressed(KEY_W)|| IsKeyPressed(KEY_UP)){
        p->d=UP;
    }
}
bool isApplePositionAcceptable(Apple* a ,Player* p){
    //at the moment i only check if the head of the snake and the apple dont collide
    Player* temp = p;
    bool isAcceptable= true;
    while(temp!=NULL){
        if(temp->x == a->x &&temp->y == a->y)isAcceptable = false;
    };
    return isAcceptable;

}
void positionApple(Apple* a , Player* p,Game g){
    //before i set the apple position i should check that the new position of the apple isnt the same position of the snake, in every part of the snake
    do{
        a->x = GetRandomValue(0,g.width/ROW_SIZE-1);
        a->y = GetRandomValue(0,g.height/ROW_SIZE-1);
    }while(!isApplePositionAcceptable(a,p));
}
void reset(Player* head,Apple* a , Player** tail, Game g){
    head->x = GetRandomValue(0,(g.width/ROW_SIZE)-1);
    head->y = GetRandomValue(0,(g.width/ROW_SIZE)-1);
    printf("starting position (%d,%d)",head->x,head->y);
    head->d = NONE;
    head->next = NULL;
    if(apples>bestScore){
        bestScore = apples;
    }
    apples=0;
    *tail = head;
    positionApple(a,head, g);
}
Player* addANode(Player* tail){
    Player* newNode = malloc(sizeof(Player));
    switch(tail->d){
        case LEFT:
            newNode->x = tail->x +1;
            newNode->y = tail->y;
            break;
        case RIGHT:
            newNode->x = tail->x -1;
            newNode->y = tail->y;
            break;
        case UP:
            newNode->x = tail->x ;
            newNode->y = tail->y+1;
        break;

        case DOWN:
            newNode->x = tail->x;
            newNode->y = tail->y-1;
    }
    newNode->d = tail->d;
    newNode->next = NULL;
    tail->next = newNode;
    return newNode;
}

//critica
void moveSinglePiece(Player* p){
    switch(p->d){
        case LEFT:
            p->x -=1;
            break;
        case RIGHT:
            p->x+=1;
            break;
        case UP:
            p->y-=1;
            break;
        case DOWN:
            p->y+=1;
            break;
        default:
            printf("no direction");
    }
    
}
//critica
void movePlayer(Player* head){
    Player* temp =  head;
    Direction previousDirection = temp->d;
    Direction tmpDir;
    while(temp!=NULL){
        moveSinglePiece(temp);
        tmpDir = temp->d;
        temp->d =previousDirection;
        previousDirection =tmpDir;

        temp=(Player*)temp->next;
    }
    
}
bool collisionWithItself(Player* head){
    Player* temp = head->next;
    while(temp!= NULL){
        if(head->x == temp->x && temp->y==head->y){
            return true;
        }
        temp= temp->next;
    }
    return false;
}
bool collisionWithBorder(Player* head,Game g){
    if(head->x > (g.width/ROW_SIZE )- 1||head->y > (g.height/ROW_SIZE )-1 ||head->x <0 ||head->y<0)return true;
    printf("Head at (%d, %d)\n", head->x, head->y);
    return false;
}
void checkCollision(Apple* a,  Player* head,Player** tail,Game g){
    if(a->x == head->x && head->y == a->y){
        apples++;
        positionApple(a,head,g);
        *tail = addANode(*tail);
    }
    if(collisionWithItself(head)||collisionWithBorder(head,g)){
        reset(head,a,tail,g);
    }
}
void drawSnake(Player* head,Game g){
    Player* temp =  head;
    while(temp!=NULL){
        DrawRectangle((*temp).x*ROW_SIZE +g.x,(*temp).y*ROW_SIZE +g.y,ROW_SIZE,ROW_SIZE,GREEN);
        temp=(Player*)temp->next;
    }
}
void printGame(Player* head,Apple a,Game g){
    /*print a grid WIDTH-HEIGHT with squares */
    

    for(int y = 0;y<(g.height/ROW_SIZE);y++){
        for(int x = 0; x < (g.width /ROW_SIZE);x++){
            DrawRectangleLines(x*ROW_SIZE+g.x
                ,y*ROW_SIZE+g.y 
                ,ROW_SIZE, ROW_SIZE,WHITE);    
        }
    }
    //draw the player
    drawSnake(head,g);
    //draw the apple
    DrawRectangle(a.x*ROW_SIZE+g.x,a.y*ROW_SIZE+g.y,ROW_SIZE,ROW_SIZE,RED);
    
    sprintf(appleToString,"ACTUAL: %d",apples);
    sprintf(bestScoreToString,"BEST SCORE: %d",bestScore); 
    DrawText(appleToString, g.x+g.width+40,20,20,WHITE);
    DrawText(bestScoreToString, g.x+g.width+40,50,20,WHITE);
}
void setUpGame(Game* g){
    g->x = 15;
    g->y=15;
    g->width=400;
    g->height=400;
}
int main(void)
{
    InitWindow(WIDTH, HEIGHT, "Snake");
    SetTargetFPS(12);
    Game g;
    setUpGame(&g);
    Player* head = malloc(sizeof(Player));
    Player* tail = head ;
    Apple a;
    reset(head,&a,&tail,g);
    while (!WindowShouldClose()){
        
        BeginDrawing();       
        ClearBackground(BLACK);
        printGame(head,a,g);
        checkDirection(head);
        movePlayer(head);
        checkCollision(&a,head,&tail,g);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}