#include <stdio.h>
#include <raylib.h>
#include <unistd.h>
#include <stdlib.h>

#define ROW_SIZE 30
#define WIDTH 600
#define HEIGHT 600

int apples;
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
    return true;
    if(a->x == p->x && a->y == p->y )return false;

}
void positionApple(Apple* a , Player* p){
    //before i set the apple position i should check that the new position of the apple isnt the same position of the snake, in every part of the snake
    do{
        a->x = GetRandomValue(0,WIDTH/ROW_SIZE-1);
        a->y = GetRandomValue(0,HEIGHT/ROW_SIZE-1);
    }while(!isApplePositionAcceptable(a,p));
}
void reset(Player* head,Apple* a , Player** tail){
    head->x = GetRandomValue(0,(HEIGHT/ROW_SIZE)-1);
    head->y = GetRandomValue(0,(HEIGHT/ROW_SIZE)-1);
    printf("starting position (%d,%d)",head->x,head->y);
    head->d = NONE;
    head->next = NULL;
    apples=0;
    *tail = head;
    positionApple(a,head);
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
bool collisionWithBorder(Player* head){
    if(head->x < 0 || head->y < 0 || head->x > (WIDTH/ROW_SIZE)-1 ||head->y > (HEIGHT/ROW_SIZE)-1)return true;
    return false;
}
void checkCollision(Apple* a,  Player* head,Player** tail){
    if(a->x == head->x && head->y == a->y){
        apples++;
        positionApple(a,head);
        *tail = addANode(*tail);
    }
    if(collisionWithItself(head)||collisionWithBorder(head)){
        reset(head,a,tail);
    }
}
void drawSnake(Player* head){
    Player* temp =  head;
    while(temp!=NULL){
        DrawRectangle((*temp).x*ROW_SIZE,(*temp).y*ROW_SIZE,ROW_SIZE,ROW_SIZE,GREEN);
        temp=(Player*)temp->next;
    }
}
void printGame(Player* head,Apple a){
    /*print a grid WIDTH-HEIGHT with squares */
    char appleToString[4];
    for(int y = 0;y<HEIGHT/ROW_SIZE ;y++){
        for(int x = 0; x < WIDTH/ROW_SIZE;x++){
            DrawRectangleLines(x*ROW_SIZE,y*ROW_SIZE ,ROW_SIZE, ROW_SIZE,WHITE);    
        }
    }
    //draw the player
    drawSnake(head);
    //draw the apple
    DrawRectangle(a.x*ROW_SIZE,a.y*ROW_SIZE,ROW_SIZE,ROW_SIZE,RED);
    sprintf(appleToString,"%d",apples);
    //that's not the final ui 
    DrawText(appleToString, (0.9)*WIDTH,20,20,WHITE);
}

int main(void)
{
    InitWindow(WIDTH, HEIGHT, "Snake");
    SetTargetFPS(12);
    Player* head = malloc(sizeof(Player));
    Player* tail = head ;
    Apple a;
    reset(head,&a,&tail);
    while (!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(BLACK);
        printGame(head,a);
        checkDirection(head);
        movePlayer(head);
        checkCollision(&a,head,&tail);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}