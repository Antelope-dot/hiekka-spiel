#include "raylib.h"
#include "raymath.h"

#define ROWS 200
#define COLS ROWS

#define screenWidth 800
#define screenHeight screenWidth

const int CELL_WIDTH = screenWidth / COLS;
const int CELL_HEIGHT = screenHeight / ROWS;

int currentTool = 1;

float dT = 0;

typedef struct Cell
{
	int id;
	int row;
	int col;
} Cell;

Cell grid[COLS][ROWS];

void InitGrid();
void CellDraw(cell);
bool PosIsValid(x, y, rand);
void UpdateCell(col, row);
void UpdateWater(col, row);
void UpdateSand(col, row);

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
	// Initialization
	//--------------------------------------------------------------------------------------
	InitWindow(screenWidth, screenHeight, "Sandspiel");

	SetTargetFPS(60);
	InitGrid();

	// Main game loop
	//--------------------------------------------------------------------------------------
	while (!WindowShouldClose())    // Detect window close button or ESC key
	{
		// Update
		//----------------------------------------------------------------------------------
		dT += GetFrameTime();
		if (IsKeyPressed(KEY_ONE)) {
			currentTool = 1;
		}
		if (IsKeyPressed(KEY_TWO)) {
			currentTool = 2;
		}

		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
		{
			Vector2 mPos = GetMousePosition();
			int posX = (int)mPos.x / CELL_WIDTH;
			int posY = (int)mPos.y / CELL_HEIGHT;
			int rand = GetRandomValue(0, 9);
			if (PosIsValid(posX, posY, rand)) {
				grid[posX + rand][posY + rand].id = currentTool;
			}
		}

		for (int c = 0; c < COLS; c++)
		{
			for (int r = ROWS-1; r >= 0; r--)
			{
				UpdateCell(c, r);
			}
		}

		if (dT > .05) {
			dT = 0.;
		}
		// Draw
		//----------------------------------------------------------------------------------
		BeginDrawing();

		ClearBackground(RAYWHITE);

		for (int c = 0; c < COLS; c++)
		{
			for (int r = 0; r < ROWS; r++)
			{
				CellDraw(grid[c][r]);
			}
		}

		EndDrawing();
		//----------------------------------------------------------------------------------
	}

	// De-Initialization
	//--------------------------------------------------------------------------------------
	CloseWindow();        // Close window and OpenGL context
	//--------------------------------------------------------------------------------------

	return 0;
}

void CellDraw(Cell cell) {
	if (cell.id == 1) {
		DrawRectangle(cell.col * CELL_WIDTH, cell.row * CELL_HEIGHT, CELL_WIDTH, CELL_HEIGHT, ORANGE);
	}
	else if (cell.id == 2) {
		DrawRectangle(cell.col * CELL_WIDTH, cell.row * CELL_HEIGHT, CELL_WIDTH, CELL_HEIGHT, BLUE);
	}
	//DrawRectangleLines(cell.col * CELL_WIDTH, cell.row * CELL_HEIGHT, CELL_WIDTH, CELL_HEIGHT, GRAY);

}

bool PosIsValid(int x, int y, int rand)   {
	return x + rand < ROWS && x + rand >= 0 && y < COLS && y + rand >= 0 && grid[x+rand][y+rand].id == 0;
}

void InitGrid() {
	for (int c = 0; c < COLS; c++)
	{
		for (int r = 0; r < ROWS; r++)
		{
			grid[c][r] = (Cell){
				.col = c,
				.row = r,
				.id = 0
			};
		}
	}
}

void UpdateCell(int col, int row) {
	//SAND
	if (grid[col][row].id == 1) {
		UpdateSand(col, row);
	//WATER
	} else if (grid[col][row].id == 2) {
		UpdateWater(col, row);
	}
}

void UpdateSand(int col, int row) {
	
	if (row != ROWS-1) {
		//Check bottom
		if (row <= ROWS - 1 && grid[col][row + 1].id != 1) {
			grid[col][row].id = 0;
			grid[col][row + 1].id = 1;
		}
		//Check left
		else if (row <= ROWS - 1 && col >= 0 && grid[col - 1][row + 1].id != 1) {
			grid[col][row].id = 0;
			grid[col - 1][row + 1].id = 1;
		}
		//Check right
		else if (row <= ROWS - 1 && col <= COLS - 1 && grid[col + 1][row + 1].id != 1) {
			grid[col][row].id = 0;
			grid[col + 1][row + 1].id = 1;
		}
	}
}

void UpdateWater(int col, int row) {
	int randValue = GetRandomValue(0,1);
	if (row != ROWS-1) {
		//Check bottom
		if (row <= ROWS - 1 && grid[col][row + 1].id == 0) {
			grid[col][row].id = 0;
			grid[col][row + 1].id = 2;
		}
		//Check down left
		else if (row <= ROWS - 1 && col >= 0 && grid[col - 1][row + 1].id == 0 && randValue == 0) {
			grid[col][row].id = 0;
			grid[col - 1][row + 1].id = 2;
		}
		//Check down right
		else if (row <= ROWS - 1 && col <= COLS - 1 && grid[col + 1][row + 1].id == 0 && randValue == 1) {
			grid[col][row].id = 0;
			grid[col + 1][row + 1].id = 2;
		}
		// Check left
		else if (row <= ROWS - 1 && col > 0 && grid[col - 1][row].id == 0 && randValue == 0) {
			grid[col][row].id = 0;
			grid[col - 1][row].id = 2;
		}
		// Check right
		else if (row <= ROWS - 1 && col <= COLS - 1 && grid[col + 1][row].id == 0 && randValue == 1) {
			grid[col][row].id = 0;
			grid[col + 1][row].id = 2;

		}
	}
}
