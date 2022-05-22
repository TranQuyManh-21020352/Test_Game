#include "Functions.h"

int randomObstacles()
{
	return rand() % MaxLane;
}

void setRect() {
	
	pic1 = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT - speed * i };
	pic2 = { 0, SCREEN_HEIGHT - speed * i, SCREEN_WIDTH, speed * i };
	pic3 = { 0, speed * i, SCREEN_WIDTH, SCREEN_HEIGHT - speed * i };
	pic4 = { 0, 0, SCREEN_WIDTH, speed * i };
}

void RunBackground(BaseObject& background, SDL_Renderer* screen,bool explosion)
{
	setRect();
	if (SCREEN_HEIGHT - i * speed < 0)
	{
		i = 0;
	}
	background.renderBackground(screen, &pic1, &pic3);
	background.renderBackground(screen, &pic2, &pic4);
	i++;
	if (explosion) {
		i--;
	}
}

void setObstacles(std::vector<Obstacles>& Obstacle, SDL_Renderer* screen, int& currentObstacles,int& timeAppearObstacles, int& timeMaxAppear)
{
	if (timeAppearObstacles < timeMaxAppear)
	{
		timeAppearObstacles++;
	}
	if (currentObstacles < MaxObstacles) {
		if (timeAppearObstacles == timeMaxAppear)
		{
			int index = randomObstacles();
			Obstacles current;
			if (index == 0)
			{
				current.setPosition(110, POSITIONY);
			}
			else if (index == 1)
			{
				current.setPosition(110 + 75, POSITIONY);
			}
			else if (index == 2)
			{
				current.setPosition(110 + 75 * 2, POSITIONY);
			}
			else if (index == 3)
			{
				current.setPosition(110 + 75 * 3, POSITIONY);
			}
			Obstacle.push_back(current);
			for (int obs = 0; obs < Obstacle.size(); obs++)
			{
					Obstacle[obs].loadImageObstacles("image//obstacles.png", screen);
			}
			timeAppearObstacles = 0;
			currentObstacles++;
		}
	}
	//std::cout << timeMaxAppear << std::endl;
}

void renderObs(std::vector<Obstacles>& Obstacle,SDL_Renderer* screen)
{
	for (int obs = 0; obs < Obstacle.size(); obs++)
	{
		if (Obstacle[obs].GetPosYObstacles() > SCREEN_HEIGHT + 60)
		{
			int random = rand() % 2;
			if (random == 0) {
				Obstacle[obs].loadImageObstacles("image//obstacles.png", screen);
			}
			else {
				Obstacle[obs].loadImageObstacles("image//yelowCar_reverse.png", screen);
			}
			int index = randomObstacles();
			if (index == 0)
			{
				Obstacle[obs].setPosition(110, POSITIONY);
			}
			else if (index == 1)
			{
				Obstacle[obs].setPosition(110 + 75, POSITIONY);
			}
			else if (index == 2)
			{
				Obstacle[obs].setPosition(110 + 75 * 2, POSITIONY);
			}
			else if (index == 3)
			{
				Obstacle[obs].setPosition(110 + 75 * 3, POSITIONY);
			}
		}
		Obstacle[obs].moveObstacles();
		Obstacle[obs].renderObstacles(screen);
	}
	
}

void setVelObstacles(std::vector<Obstacles>&obstacle, int velObs) {
	for (int obs = 0; obs < obstacle.size(); obs++)
	{
		obstacle[obs].setVelocity(velObs);
	}
}

bool checkCollision(std::vector<SDL_Rect>&A, std::vector<SDL_Rect>&B)
{
	int Top_A, Top_B;
	int Bottom_A, Bottom_B;
	int Left_A, Left_B;
	int Right_A, Right_B;
	for (int Apix = 0; Apix < A.size(); Apix++)
	{
		Top_A = A[Apix].y;
		Bottom_A = A[Apix].y + A[Apix].h;
		Left_A = A[Apix].x;
		Right_A = A[Apix].x + A[Apix].w;
		for (int Bpix = 0; Bpix < B.size(); Bpix++)
		{
			Top_B = B[Bpix].y;
			Bottom_B = B[Bpix].y + B[Bpix].h;
			Left_B = B[Bpix].x;
			Right_B = B[Bpix].x + B[Bpix].w;
			if (((Top_A > Bottom_B) || (Bottom_A < Top_B) || (Right_A < Left_B) || (Left_A > Right_B)) == false)
			{
				std::cout << "car: " << Apix << " obs: " << Bpix << std::endl;
				return true;
			}
		}
	}
	return false;
}

void renderCollision(CarObject& Car, Obstacles& Obs)
{
	int VelxCar = Car.GetVelxCar();
	int VelyCar = Car.GetVelyCar();
	int PosXCar = Car.GetPosXCar();
	int PosYCar = Car.GetPosYCar();
	int PosXObs = Obs.GetPosXObstacles();
	int PosYObs = Obs.GetPosYObstacles();
	if (VelyCar == 0)
	{
		if (VelxCar < 0)
		{
			if (PosYCar < PosYObs + HEIGHTCAR - 5 && PosYCar + HEIGHTCAR > PosYObs + 5)
			{
				Car.setPosX(PosXObs + WIDTHCAR);
			}
			std::cout << "TH1" << std::endl;
		}
		if (VelxCar > 0)
		{
			if (PosYCar < PosYObs + HEIGHTCAR - 5 && PosYCar + HEIGHTCAR > PosYObs + 5)
			{
				Car.setPosX(PosXObs - WIDTHCAR);
			}
			std::cout << "TH2" << std::endl;
		}
	}
	else if (VelyCar < 0)
	{
		if (VelxCar == 0)
		{
			if ((PosXCar + 7 >= PosXObs + 7 && PosXCar  <= PosXObs + 26) || (PosXCar + 26 >= PosXObs && PosXCar <= PosXObs ))
			{
				Car.setPosY(PosYObs + HEIGHTCAR);
			}
			else if ((PosXCar >= PosXObs + WIDTHCAR - 7 && PosXCar <= PosXObs + WIDTHCAR && PosYCar > PosYObs + HEIGHTCAR) || (PosXCar + WIDTHCAR <= PosXObs + 7 && PosXCar + WIDTHCAR >= PosXObs && PosYCar > PosYObs + HEIGHTCAR))
			{
				Car.setPosY(PosYObs + HEIGHTCAR - 5);
			}
			else if (PosXCar == 365 )
			{
				Car.setPosY(PosYObs + HEIGHTCAR - 5);
			}
			std::cout << "TH3" << std::endl;
		}
		else if (VelxCar < 0)
		{
			if (PosXCar == 85)
			{
				Car.setPosX(85);
				Car.setPosY(PosYObs + HEIGHTCAR - 2);
			}
			else {
				if (PosYCar - VelyCar < PosYObs + HEIGHTCAR - 5 && PosYCar -VelyCar + HEIGHTCAR > PosYObs + 5)
				{
					Car.setPosX(PosXObs + WIDTHCAR);
					
				}
				else if (PosYCar - VelyCar >= PosYObs + HEIGHTCAR  || PosYCar - VelyCar + HEIGHTCAR <= PosYObs )
				{
					Car.setPosY(PosYObs + HEIGHTCAR);
				}
				else {
					Car.setPosX(PosXCar - VelxCar);
					Car.setPosY(PosYCar - VelyCar);
				}
			}
			std::cout << "TH4" << std::endl;
		}
		else {
			if (PosXCar == 365)
			{
				Car.setPosX(365);
				Car.setPosY(PosYObs + HEIGHTCAR - 5);
			}
			else {
				if (PosYCar - VelyCar < PosYObs + HEIGHTCAR - 5 && PosYCar - VelyCar + HEIGHTCAR > PosYObs + 5 )
				{
					Car.setPosX(PosXObs - WIDTHCAR);
				}
				else if (PosYCar - VelyCar >= PosYObs + HEIGHTCAR || PosYCar - VelyCar + HEIGHTCAR <= PosYObs)
				{
					Car.setPosY(PosYObs + HEIGHTCAR);
				}
				else {
					Car.setPosX(PosXCar - VelxCar);
					Car.setPosY(PosYCar - VelyCar);
				}
			}
			std::cout << "TH5" << std::endl;
		}
	}
	else {
		if (VelxCar == 0)
		{
			if (2 * abs(PosXCar - PosXObs + 26) <= 52)
			{
				Car.setPosY(PosYObs - HEIGHTCAR);
			}
			std::cout << "TH6" << std::endl;
		}
		else if (VelxCar < 0)
		{

			if (PosXCar == 85)
			{
				Car.setPosX(85);
				Car.setPosY(PosYObs - HEIGHTCAR + 2);
			}
			else {
				if (PosYCar < PosYObs + HEIGHTCAR - 5 && PosYCar + HEIGHTCAR > PosYObs + 5)
				{
					Car.setPosX(PosXObs + WIDTHCAR);
				}
			}
			std::cout << "TH7" << std::endl;
		}
		else {
			if (PosXCar == 365)
			{
				Car.setPosX(365);
				Car.setPosY(PosYObs - HEIGHTCAR + 2);
			}
			else {
				if (PosYCar < PosYObs + HEIGHTCAR - 5 && PosYCar + HEIGHTCAR > PosYObs + 5)
				{
					Car.setPosX(PosXObs - WIDTHCAR);
				}
			}
			std::cout << "TH8" << std::endl;
		}
	}
}
// setcountdown
void SetCountdown(SDL_Renderer* screen, TTF_Font* font, Mix_Chunk* Coundown_, BaseObject& background, CarObject& car,TextObject& text, TextObject& textcoundown_)
{
	
	    background.renderBackground(screen, &pic1, &pic3);
	    background.renderBackground(screen, &pic2, &pic4);
		car.CarRender(screen);
		RenderRectangle(screen, FillColor, FillRect);
		RenderOutLine(screen, OutlineColor, OutlineRect);
		text.renderText((SCREEN_WIDTH - text.getWidthText()) / 2, 1, screen);
		if (!textcoundown_.loadFromRenderedText(font,screen))
		{
			success = false;
		}
		textcoundown_.renderText((SCREEN_WIDTH - textcoundown_.getWidthText()) / 2, SCREEN_HEIGHT / 4, screen);
		SDL_Delay(500);
		SDL_RenderPresent(screen);
		
}

bool checkFocusButton(const int& x, const int& y, SDL_Rect& check) {
	if (x >= check.x && x <= check.x + check.w && y >= check.y && y <= check.y + check.h)
	{
		return true;
	}
	return false;
}

void sort(std::vector<int>& hightscore_)
{
	for (int k = 0; k < hightscore_.size(); k++)
	{
		for (int j = k + 1; j < hightscore_.size(); j++)
		{
			if (hightscore_[j] > hightscore_[k])
			{
				int temp = hightscore_[k];
				hightscore_[k] = hightscore_[j];
				hightscore_[j] = temp;
			}
		}
	}
}