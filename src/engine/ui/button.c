#include "raylib.h"
#include "button.h"

bool makeButton(Rectangle button, Color rectColor, Color textColor, const char *text, int font) {
/*	Vector2 center = {GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};
	Vector2 textSize = {MeasureText(text, font), font};

	Rectangle centeredRect = {
		center.x + button.x - button.width / 2.0f,
		center.y + button.y - button.height / 2.0f,
		button.width,
		button.height 
	};

	Vector2 textPos = {
		centeredRect.x + (centeredRect.x - textSize.x) / 2.0f,
		centeredRect.y + (centeredRect.y - textSize.y) / 2.0f
	};*/

	
	//DrawText(text, textPos.x, textPos.y, font, textColor);
	
	

	//Vector2 mouse = GetMousePosition();


	Vector2 textCenter = {MeasureText(text, font), font};


	
	

	DrawRectangle(button.x, button.y, button.width, button.height, WHITE);
	DrawText(text, button.x + (button.width - textCenter.x) / 2.0f, button.y + (button.height - textCenter.y) / 2.0f, font, textColor);

	if(CheckCollisionPointRec(GetMousePosition(), button) && 
	   IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
		return true;
	} else {
		return false;
	}
}
