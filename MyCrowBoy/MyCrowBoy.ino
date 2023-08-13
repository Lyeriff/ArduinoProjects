#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64 

#define BUTTON_A 2
#define BUTTON_B 4
#define MAX_OBJ_COUNT 15
#define ASTEROID_COUNT 3
#define BULLET_RADIUS 4


typedef struct {
    bool alive;
    char type;
    char x, y;
} Object;

Object objects[MAX_OBJ_COUNT] = {0, };
Object asteroids[ASTEROID_COUNT] = {0, };


Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
int x_pos = 64, y_pos = 55;
char x_vec = 1, y_vec = 1;
int itr = 1;
int score = 0;


void(* resetFunc) (void) = 0;

void draw_sprite(int x, int y){
  oled.drawRect(x, y, 10, 10, WHITE);
  oled.fillRect(x+2, y+2, 6, 6, WHITE);
}

// void destroy_object(Object *object){
//   object->alive = false;
// }

void draw_objects(){
  Object *ptr;
  for(int i = 0; i < MAX_OBJ_COUNT; i++){
    ptr = &objects[i];
    if(ptr->alive){
      oled.drawCircle(ptr->x, ptr->y, BULLET_RADIUS, WHITE);
    }
  }
  char a, b;

  for(int i = 0; i < ASTEROID_COUNT; i++){
    ptr = &asteroids[i];

    if(ptr->alive){

      a = ptr->x;
      b = ptr->y;

      oled.drawTriangle(a, b, a + 4, b, a + 2, b + 4, WHITE);
    }
  }
}

void create_bullet(){
  Object *ptr;
  for(int i = 0; i < MAX_OBJ_COUNT; i++){
    ptr = &objects[i];
    if(!ptr->alive){
      ptr->x = x_pos;
      ptr->y = y_pos;
      ptr->alive = true;
      break;
    }
  }
}

void create_asteroids(){
  for(int i = 0; i < ASTEROID_COUNT; i++){
    Object *ptr = &asteroids[i];
    if(!ptr->alive){
      randomSeed(analogRead(0));
      ptr->x = (char)random(5, 123);
      ptr->y = 0;
      ptr->alive = true;
      
    }
  }
}

void increment_objects(){
  for(int i = 0; i < MAX_OBJ_COUNT; i++){
    Object *ptr = &objects[i];
    if(ptr->alive){
      ptr->y -= 5;
      if(ptr->y <= 0)
        ptr->alive = false;
    }
  }
  for(int i = 0; i < ASTEROID_COUNT; i++){
    Object *ptr = &asteroids[i];
    if(ptr->alive){
      ptr->y += 5;
      if(ptr->y >= 64)
        ptr->alive = false;
    }
  }
}

void rud_collison_detection(){
  Object *ast, *blt;
  for(int i = 0; i < MAX_OBJ_COUNT; i++){
    for(int j = 0; j < ASTEROID_COUNT; j++){
      ast = &asteroids[j]; blt = &objects[i];
      
      if(ast->alive){
        if(blt->alive){

          // if( 
          //     ((ast->x <= blt->x - 2 <= ast->x + 4) || (ast->x <= blt->x + 2 <= ast->x + 4)
          //   ) 
          //   && (ast->y == blt->y ) 
          // )

          char v1_x = ast->x - blt->x, v1_y = ast->y - blt->y,
               v2_x = ast->x + 4 - blt->x, v2_y = ast->y - blt->y,
               v3_x = ast->x + 2 - blt->x, v3_y = ast->y - blt->y + 4;

          if (
            (sqrt(v1_x*v1_x + v1_y*v1_y) <= BULLET_RADIUS) ||
            (sqrt(v2_x*v2_x + v2_y*v2_y) <= BULLET_RADIUS) ||
            (sqrt(v2_x*v1_x + v2_y*v1_y) <= BULLET_RADIUS)
          )
          {
            Serial.print((int)ast->x);
            Serial.print(", ");
            
            Serial.print((int)ast->y);
            Serial.print(", bullet : ");

            Serial.print((int)blt->x);
            Serial.print(", ");

            
            Serial.print((int)blt->y);
            Serial.print('\n');

            ast->alive = false; blt->alive = false;
            score++;
          }
        }

        if((ast->x == x_pos)&&(ast->y == y_pos)){
          oled.clearDisplay();
          oled.setCursor(37, 32);
          oled.print("GAME OVER");
          oled.setCursor(38, 40);
          oled.print("SCORE: ");
          oled.print(score);
          oled.display();
          delay(1000);
          setup();
          resetFunc();
        }
      }
    }
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(BUTTON_A, INPUT_PULLUP);
  pinMode(BUTTON_B, INPUT_PULLUP);

  
  if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (true);
  }

  delay(1500);

  oled.clearDisplay();
  oled.setTextSize(1);
  oled.setTextColor(WHITE);
  oled.setCursor(37, 32);
  oled.print("MyCrowBoy");
  char x, y, b, h;

  // oled.drawRect(4, 5, 59, 128, WHITE);
  oled.display();
  
  while(true){
    oled.clearDisplay();
    // randomSeed(analogRead(0));
    oled.setCursor(37, 32);
    oled.print("MyCrowBoy");
    oled.drawRect(x, y, b, h, WHITE);
    
    if(!digitalRead(BUTTON_A))
      break;
    
    x +=  4; y += 2;
    
    b = (128 - 2*x); h = (64 - 2*y);

    if((x >= 50) || (y >= 28)){
      x = 0; y = 0;
      h = 64; b = 128;
    }
    oled.display();
  }
}

void loop() {
  oled.clearDisplay();
  
  draw_sprite(x_pos, y_pos); 
  create_asteroids();

  increment_objects();

  draw_objects();
  oled.display();

  delay(50);

  if(x_pos + 2 >= 128)
    x_pos = 0;
  
  else if(x_pos + 2 <= 0)
      x_pos = 128;
  
  
  // x_pos += itr/100 ? x_vec  * (itr/100) + 3 : 3 * x_vec; 
  x_pos += 4 * x_vec;

  if(!digitalRead(BUTTON_A)){
    x_vec *= -1;
    delay(20);
  }

  if(!digitalRead(BUTTON_B)){
    create_bullet();
    delay(20);
  }
  rud_collison_detection();
}