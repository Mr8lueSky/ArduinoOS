#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
char keys[ROWS][COLS] = {
{'1','2','3', 'A'},
{'4','5','6', 'B'},
{'7','8','9', 'C'},
{'*','0','#', 'D'}
};

byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

byte p20[8]{ //arduino start animation drawing
    B00000,
    B00000,
    B10000,
    B10000,
    B10000,
    B00000,
    B00000,
  };
  byte p40[8]{
    B01000,
    B00000,
    B10000,
    B11000,
    B10000,
    B00000,
    B01000,
  };
  byte p60[8]{
    B01100,
    B00100,
    B10000,
    B11000,
    B10000,
    B00100,
    B01100,
  };
  byte p80[8]{
    B01110,
    B00100,
    B10000,
    B11010,
    B10000,
    B00100,
    B01110,
  };
  byte p100[8]{
    B01110,
    B00100,
    B10001,
    B11011,
    B10001,
    B00100,
    B01110,
  };



  byte o20[8]{ //app start animation drawing
    B00000,
    B00000,
    B00000,
    B10000,
    B00000,
    B00000,
    B00000,
  };
  byte o40[8]{
    B00000,
    B00000,
    B00000,
    B11000,
    B00000,
    B00000,
    B00000,
  };
  byte o60[8]{
    B00000,
    B00100,
    B00000,
    B11100,
    B00000,
    B00100,
    B00000,
  };
  byte o80[8]{
    B00000,
    B00100,
    B00010,
    B11110,
    B00010,
    B00100,
    B00000,
  };
  byte o100[8]{
    B00000,
    B00100,
    B00010,
    B11111,
    B00010,
    B00100,
    B00000,
  };


  byte k20[8]{ //app finish animation drawing
    B00000,
    B00000,
    B00000,
    B10000,
    B00000,
    B00000,
    B00000,
  };
  byte k40[8]{
    B00000,
    B00000,
    B01000,
    B11000,
    B01000,
    B00000,
    B00000,
  };
  byte k60[8]{
    B00000,
    B00100,
    B01000,
    B11100,
    B01000,
    B00100,
    B00000,
  };
  byte k80[8]{
    B00000,
    B00100,
    B01000,
    B11110,
    B01000,
    B00100,
    B00000,
  };
  byte k100[8]{
    B00000,
    B00100,
    B01000,
    B11111,
    B01000,
    B00100,
    B00000,
  };

int q;

void startApp_animation(){
  lcd.clear();
    lcd.createChar(0,o20); //app start animation instruction
    lcd.createChar(1,o40);
    lcd.createChar(2,o60);
    lcd.createChar(3,o80);
    lcd.createChar(4,o100);
    lcd.setCursor(0,1);
    for(int i = 0; i<16; i++){
      for(int j = 0; j<5; j++){  
        lcd.setCursor(i,0);
        lcd.write(j);
        delay(30);
        }
    }
}

void exitApp_animation(){
  lcd.clear();
  lcd.createChar(0,k20); //app finish animation instruction
  lcd.createChar(1,k40);
  lcd.createChar(2,k60);
  lcd.createChar(3,k80);
  lcd.createChar(4,k100);
  lcd.setCursor(15,1);
  for(int i = 0; i<16; i++){
  for(int j = 0; j<5; j++){  
    lcd.setCursor(15-i,1);
    lcd.write(j);
    delay(30);
    }
  }
}

class Hint{
  public:
  char *text;
  int screen = 0, size = 0;
  bool active = false;
  Hint(char *input, int si){
    text = input;
    size = si;
  }
  void display(){
    lcd.clear();
    int line = 0, symbols = 0;
    bool is_sec= false;
    for(int i = 0; i < size; i++){
      if(text[i] == '0'){
        symbols = 0;
          line++;
          continue;
        }
      if(line == screen){
        lcd.print(text[i]);
        }
      else if(line == screen + 1){
        if(!is_sec){
          is_sec = true;
          lcd.setCursor(0, 1);
        }
        lcd.print(text[i]);
      }
      if((symbols+1) % 16 == 0)
        line++;
      symbols++;
    }
  }

  void key_pressed(char key){
    if(key == '2' and screen > 0)
      screen--;
    else if(key == '8' and screen < ceil((double)size / 16))
      screen++;
    else if(key == 'D')
      active = false;
    this->display();
  }

  void start(){
    active = true;
    this->display();
    this->loop();
  }

  void loop(){
    while(active){
      char key = keypad.getKey();
      if (key){
        this->key_pressed(key);
      }
    }
  }
};

class Ask{
  public:
  bool active = false;
  int choice = -1, add;
  bool addition = false;
  char *message, *answers[2];
  Ask(char* text, char* answer1, char* answer2){
    message = text;
    answers[0] = answer1;
    answers[1] = answer2;
  }

  Ask(char* text, char* answer1, char* answer2, int ad){
    message = text;
    add = ad;
    addition = true;
    answers[0] = answer1;
    answers[1] = answer2;
  }

  void display(){
    lcd.clear();
    lcd.print(message);
    if(addition)
      lcd.print(add);
    lcd.setCursor(0, 1);
    if(choice < 0)
      lcd.print("|");
    else
      lcd.print(" ");
    lcd.print(answers[0]);
    
    if(choice > 0)
      lcd.print(" |");
    else
      lcd.print("  ");
    lcd.print(answers[1]);
  }
  
  int show(){
    active = true;
    choice = -1;
    this->display();
    while(active){
      char key = keypad.getKey();
      if (key){
        this->key_pressed(key);
      }
    }
    return choice;
  }

  void key_pressed(char key){
    if(key == '6'){
      choice = 1;
    } else if(key == '4'){
      choice = -1;
    } else if(key == '5' or key == 'A'){
      active = false;
    } else if(key == 'D'){
      choice = 0;
      active = false;
    }
    Serial.println(choice);
    this->display();
  }
};

class Calculator{
  public:
  int argument1;
  int result;
  char symbols = ' ';
  bool active = false;
  bool is_equal = false;
  bool is_multi = true;
  bool is_writen = false;
  Calculator(){
  }

  void clear(){
    argument1 = 0;
    result = 0;
    symbols = ' ';
    }

  void equal(){
     if (symbols == '*'){
        result = argument1*result;
        argument1 = 0;
        symbols = ' ';
        }
      if (symbols == '/'){
        result = result/argument1;
        argument1 = 0;
        symbols = ' ';
        }
      if (symbols == '+'){
        result = result+argument1;
        argument1 = 0;
        symbols = ' ';
        }
      if (symbols == '-'){
        result = result-argument1;
        argument1 = 0;
        symbols = ' ';
        }
        is_equal = true;
        is_multi = true;
        
    }
  
  void start(){
    active = true;
    this->display();
    this->loop();
  }

  void display(){
    if (is_writen == false){
      lcd.print('0');
      }
    if (result != 0){
      lcd.print(result);
      }
    if (symbols != ' '){
      lcd.print(symbols);
      }
    if (argument1 != 0){
      lcd.print(argument1);
      }
  }

  void key_pressed(char key){
    if (argument1 > 3276 and key !='C' and key != '#' and key != 'B' and key != 'A'){
      return;
    }
    if((int)key >=48 and (int)key <= 57){
      if(is_equal == true){
        lcd.clear();
        this->clear();
        is_equal = false;
        }
        
      argument1 *= 10;
      argument1 += (int)key -48;
      is_writen = true;
       }
      
    
    if (key == 'A'){
      this->equal();
      }

    if(key == 'B'){
      if (is_multi == true){
        is_multi = false;
        }
       else{
        is_multi = true;
        }
      }
       
    if(key == 'C'){
      lcd.clear();
      this->clear();
      is_writen = false;
      }
      
    if(key == 'D'){
      active = false;
       }
         
    if(key == '*'){
      if (symbols != ' ' and argument1 != 0 and result != 0){
        this->equal();
        }
      else{
        if (is_multi == true){
        symbols = '*';
        result += argument1;
        argument1 = 0;
        is_equal = false;
        }
        else{
        symbols = '+';
        result += argument1;
        argument1 = 0;
        is_equal = false;
        }
        }  
       }
       
    if(key == '#'){
      if (symbols != ' ' and argument1 != 0 and result != 0){
        this->equal();
        }
      else{
        if (is_multi == true){
          symbols = '/';
          result += argument1;
          argument1 = 0;
          is_equal = false;
          }
        if (is_multi == false){
          symbols = '-';
          result += argument1;
          argument1 = 0;
          is_equal = false;
          }
      }
    }
           
    lcd.clear();
    this->display();
  }

  void loop(){
    while(active){
    char key = keypad.getKey();
  if (key)
    this->key_pressed(key);
  }
  }
};

class Item{
  public:
  char *name;
  bool active = true;
  Item(){
  }

  virtual void start(){}

  void loop(){
    while(active){
      char key = keypad.getKey();
      if (key)
        this->key_pressed(key);
    }
  }
  
  virtual void key_pressed(char key){
   
    }
};

class Measurements:public Item{
  public:
  bool full = false, type;
  int pin = 0, step = 0;
  Measurements(){
  }
  
  virtual void start(){
    active = true;
    pin = 0; step = 0;
    this->display();
    this->loop();
  }

  void display(){
    if(step == 0){
      lcd.clear();
      lcd.print("Digital pin: ");
      lcd.print(pin);
    }
    else if (step == 1){
      Ask ask("Type?", "input", "output");
      int out = ask.show();
      if(out < 0)
        type = false;
      else if(out > 0)
        type = true;
      else{
        active = false;
        return;
      }
      if(!type){
        pinMode(pin, INPUT); 
      } else {
        pinMode(pin, OUTPUT);
      }
      step = 2;
      this->display();
    } else if (step == 2){
      if(!type){
        Ask ask("Input:", "HIGH", "LOW");
        int ans = ask.show();
        if(ans < 0){
          digitalWrite(pin, HIGH);
        } else if(ans > 0)
          digitalWrite(pin, LOW);
        else
          active = false;
      } else{
        int val;
        val = digitalRead(pin);
        while(active){
          Ask ask("Output: ", "refresh", "exit", val);
          if(ask.show() >= 0)
            active = false;
          val = digitalRead(pin);
          Serial.println(val);
        }
      }
    }
  }

  void loop(){
    while(active){
      char key = keypad.getKey();
      if (key){
        this->key_pressed(key);
      }
    }
  }

  virtual void key_pressed(char key){
    if(step == 0){
      if((int)key >= 48 and (int)key <= 57){
        int number = (int)key - 48;
        if(pin * 10 + number <= 13){
          pin *= 10;
          pin += number;
        } else {
          pin = 0;
        }
      } else if(key == 'A'){
        step = 1;
      }
    } 
    this->display();
    }
};

class MainMenu{
  public:
  char *list[16];
  char description[3][100] = { 
    {"Simple calculator.0A - =0B - change signsC - clear all0D - exit0*, # - + or *, - or /"},
    {"Use your modules rigth now, without programming!0A or 5 - ok0D - exit"},
    {"Simple race like on tetris.02 - up08 - down0D - exit"}
  };
  int desc[3] = {86, 69, 52};
  int count = 0;
  int current = 0;
  int screen = 0;
  
  MainMenu(){
    this->add_item("Calculator");
    this->add_item("Measurements");
    this->add_item("Race");
    }

  void add_item(char *title){
    list[count] = title;
    count++;
  }

  void display(){
    lcd.clear();
    for(int i = screen; i <= screen + 1; i++){
      lcd.setCursor(0,i - screen);
      if(i == current)
        lcd.print("|");
      else
        lcd.print(" ");
      lcd.setCursor(1, i - screen);
      lcd.print(list[i]);
    }
  }

  void key_pressed(char key){
    if(key == '2' ){ // and current > 0
      this->move_up();
    } else if(key == '8'){ // and current < count - 1
      this->move_down();
    } else if(key == '5' or key == 'A'){ // and current < count - 1
      this->app_start();
    } else if(key == 'B'){ // and current < count - 1
      Hint hint(description[current], desc[current]);
      hint.start();
    }
    this->display();
  }

  void start(){
    lcd.setCursor(0,1); //arduino start animation code
    for(int i = 0; i<16; i++){
      for(int j = 0; j<5; j++){
        if ((int) i % 2 != 0){
          q = 0;
        }
      else{
        q = 1;
        }  
      lcd.setCursor(i,q);
      lcd.write(j);
      delay(25);
      }
    }
    lcd.clear();
    lcd.setCursor(2,0);
    lcd.print("WELCOME TO :");
    delay(1000);
    lcd.clear();
    lcd.setCursor(5,0);
    lcd.print("IYT_OS");
    lcd.setCursor(9,1);
    lcd.print("by Kids");
    delay(1000);
    this->display();
    while(true){
      this->loop();
    }
  }

  void app_start(){
    startApp_animation();
    if (current == 1){
      Measurements app;
      app.start();
    } else if (current == 0){
      Calculator calc;
      lcd.clear();
      calc.clear();
      calc.start();
    }
    exitApp_animation();
    this->display();
  }

  void loop(){
    char key = keypad.getKey();
    if (key){
      this->key_pressed(key);
    } 
  }

  void move_up(){
    if(current > 0)
      current--;
    if(current < screen)
      screen--;
  }

  void move_down(){
    if(current < count - 1)
      current++;
    if(current > screen + 1)
      screen++;
  }
};

MainMenu menu;

void setup()
{
  Serial.begin(9600);
  lcd.init();                      // initialize the lcd 
  lcd.backlight();
  lcd.createChar(0,p20); //arduino start animation instruction
  lcd.createChar(1,p40);
  lcd.createChar(2,p60);
  lcd.createChar(3,p80);
  lcd.createChar(4,p100);
}

void loop()
{
  menu.start(); 
}
