#include <Arduino.h>

#define MAX_MENU_DEPTH 10

// Menu IDs
#define MENU_MAIN 0
#define MENU_SETTINGS 1
#define MENU_DISPLAY_SETTINGS 2
#define MENU_AUDIO_SETTINGS 3
#define MENU_BRIGHTNESS 4
#define MENU_CONTRAST 5

// ================= STACK IMPLEMENTATION =================
class MenuStack {
private:
  int stack[MAX_MENU_DEPTH];
  int top; // Index of the top element, -1 means empty

public:
  MenuStack() : top(-1) {}

  // Push a menu ID onto the stack
  void push(int menuId) {
    if (top == MAX_MENU_DEPTH - 1) {
      Serial.println("Stack Overflow!");
      return;
    }

    top++;
    stack[top] = menuId;
  }

  // Pop and return the top menu ID
  int pop() {
    if (isEmpty()) {
      Serial.println("Stack Underflow!");
      return -1;
    }

    int value = stack[top];
    top--;
    return value;
  }

  // Return the top menu ID without removing it
  int peek() {
    if (isEmpty()) {
      return -1;
    }

    return stack[top];
  }

  bool isEmpty() {
    return top == -1;
  }
};

// ================= GLOBAL VARIABLES =================
MenuStack navStack;
int currentMenu = MENU_MAIN;

// ================= MENU DISPLAY FUNCTION =================
void displayMenu(int menuId) {
  Serial.println("\n--- Menu ---");

  switch (menuId) {
    case MENU_MAIN:
      Serial.println("1. Main Menu");
      Serial.println("Enter 's' for Settings");
      break;

    case MENU_SETTINGS:
      Serial.println("2. Settings Menu");
      Serial.println("Enter 'd' for Display Settings");
      Serial.println("Enter 'a' for Audio Settings");
      Serial.println("Enter 'b' to go Back");
      break;

    case MENU_DISPLAY_SETTINGS:
      Serial.println("3. Display Settings Menu");
      Serial.println("Enter 'r' for Brightness");
      Serial.println("Enter 'c' for Contrast");
      Serial.println("Enter 'b' to go Back");
      break;

    case MENU_AUDIO_SETTINGS:
      Serial.println("4. Audio Settings Menu");
      Serial.println("(No sub-menus)");
      Serial.println("Enter 'b' to go Back");
      break;

    case MENU_BRIGHTNESS:
      Serial.println("5. Brightness Menu");
      Serial.println("Enter 'b' to go Back");
      break;

    case MENU_CONTRAST:
      Serial.println("6. Contrast Menu");
      Serial.println("Enter 'b' to go Back");
      break;

    default:
      Serial.println("Unknown Menu");
      break;
  }

  Serial.print("> ");
}

// ================= SETUP =================
void setup() {
  Serial.begin(115200);
  while (!Serial);

  displayMenu(currentMenu);
}

// ================= LOOP =================
void loop() {

  if (Serial.available() > 0) {

    char command = Serial.read();

    // BACK command
    if (command == 'b') {

      if (!navStack.isEmpty()) {
        currentMenu = navStack.pop();
      } else {
        Serial.println("Already at main menu.");
      }

    } else {

      int nextMenu = -1;

      switch (currentMenu) {

        case MENU_MAIN:
          if (command == 's')
            nextMenu = MENU_SETTINGS;
          break;

        case MENU_SETTINGS:
          if (command == 'd')
            nextMenu = MENU_DISPLAY_SETTINGS;
          else if (command == 'a')
            nextMenu = MENU_AUDIO_SETTINGS;
          break;

        case MENU_DISPLAY_SETTINGS:
          if (command == 'r')
            nextMenu = MENU_BRIGHTNESS;
          else if (command == 'c')
            nextMenu = MENU_CONTRAST;
          break;

        case MENU_AUDIO_SETTINGS:
          // No deeper menus
          break;

        case MENU_BRIGHTNESS:
        case MENU_CONTRAST:
          // No deeper menus
          break;
      }

      // Only navigate if valid command
      if (nextMenu != -1) {
        navStack.push(currentMenu);
        currentMenu = nextMenu;
      }
    }

    displayMenu(currentMenu);
  }
}