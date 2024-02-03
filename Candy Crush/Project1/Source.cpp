//It is recommended to go through the Documentation.pdf included although relevant comments have been added in this file

#include <SFML/Graphics.hpp>
#pragma 
#include <fstream>      
#include <iostream>
#include <string>
#include <time.h>
#include <Windows.h>
//-----Function Prototypes-----//

void GameWindow(bool GameExist);   //Renders and manages the actions in the Gamewindow 
void Display(sf::RenderWindow &GWin, sf::RectangleShape Candies[][9], int CandyIDs[][9]); //Displays the candies on the grid after each swapping and special candy behavior
void DispArr(int arr[][9], int &pts); //Displays the numeric values of each candy on console (Mostly used for debugging)
void RowCheck(int arr[][9], int &pts, int &moves); //Checks whether 3 candies of same color are found in a row and takes suitable action
void ColCheck(int arr[][9], int &pts, int &moves); //Checks whether 3 candies of same color are found in a column and takes suitable action
void Row4Check(int arr[][9], int &pts, int &moves); //Checks whether 4 candies of same color are found in a row and takes suitable action
void Col4Check(int arr[][9], int &pts, int &moves); //Checks whether 4 candies of same color are found in a column and takes suitable action
void Row5Check(int arr[][9], int &pts, int &moves); //Checks whether 5 candies of same color are found in a row and takes suitable action
void Col5Check(int arr[][9], int &pts, int &moves); //Checks whether 5 candies of same color are found in a column and takes suitable action
void TCheck(int arr[][9], int &pts, int &moves); //Checks whether candies allign in any orienation of "T" (see documentation for more info) in grid and takes suitable action
void LCheck(int arr[][9], int &pts, int &moves); //Checks whether candies allign in any orienation of "L" (see documentation for more info) in grid and takes suitable action
void DestroyRow(int arr[][9], int RowVal); //Destroys a row, used for when a horizontal striped candy is destroyed.
void DestroyCol(int arr[][9], int RowVal, int ColVal); //Destroys a column, used for when a vertical striped candy is destroyed.
void DestroySqr(int arr[][9], int RowVal, int ColVal); //Destroys a 3x3 area, used for when a packeted candy is destroyed.
void isCrush(int arr[][9], int Row, int Col, sf::RenderWindow &GWindow,bool &Select,int &pts,sf::RectangleShape Candies[][9] ); // Checks whether swapping two candies will result in candies being crushed and updates the grid accordingly
void Swap(int &num1, int &num2); // Swaps two values
void Bomb_Plain(int arr[][9], int BombR, int BombC, int CandyID, int &pts); //Controls the actions that happens when Candy bomb is swapped with a regular candy
void Bomb_Bomb(int arr[][9], int &pts); //Controls the actions that happens when Candy bomb is swapped with another candy bomb
void Bomb_Stripe(int arr[][9], int BombR, int BombC, int CandyID, int &pts); //Controls the actions that happens when Candy bomb is swapped with a striped candy
void Bomb_Pack(int arr[][9], int BombR, int BombC, int CandyID, int &pts); //Controls the actions that happens when Candy bomb is swapped with a packeted candy
void Stripe_Pack(int arr[][9], int Pack_R, int Pack_C, int Stripe_R, int Stripe_C, int &pts); //Controls the actions that happens when Striped candy is swapped with a packeted candy
void Pack_Pack(int arr[][9], int Stripe1_R, int Stripe1_C, int Stripe_R, int Stripe_C, int &pts); //Controls the actions that happens when Packeted candy is swapped with another packeted candy
int PointsCheck(int CandyID); //Returns value of points based on the color of candies crushed


//main function is responsible for creating the main window (see documentation for more info)
int main()
{
	
	//Creating objects like textures,windows,sprites.

		sf::RenderWindow window(sf::VideoMode(800, 600), "My window");
		sf::RectangleShape Rec(sf::Vector2f(800, 600));
		sf::RectangleShape Rec2(sf::Vector2f(160, 51));
		sf::RectangleShape Rec3(sf::Vector2f(160, 51));
		sf::RectangleShape Rec4(sf::Vector2f(160, 51));

		sf::Texture PlayerTexture;
		PlayerTexture.loadFromFile("Artboard1.png");
		Rec.setTexture(&PlayerTexture);

		sf::Texture PlayTexture;
		PlayTexture.loadFromFile("Asset6.png");
		Rec2.setTexture(&PlayTexture);
		Rec2.setPosition(sf::Vector2f(320, 300));

		sf::Texture LoadTexture;
		LoadTexture.loadFromFile("Load.png");
		Rec4.setTexture(&LoadTexture);
		Rec4.setPosition(sf::Vector2f(320, 360));

		sf::Texture ExitTexture;
		ExitTexture.loadFromFile("Asset7.png");
		Rec3.setTexture(&ExitTexture);
		Rec3.setPosition(sf::Vector2f(320, 420));


		sf::Vector2f sizeButton = Rec2.getSize();
		sf::Vector2f sizeExitButton = Rec3.getSize();
		bool FileExist = false; //Check whether the Savefile exists


	//------Object creation end-------
	

	while (window.isOpen()) //Code to run while window is open
	{
		sf::Event event; //Create an event
		

		while (window.pollEvent(event))
		
		//Code to change value of boolean variable and enable/disable Load button depending whether the save file exists. (sec documentation for more info)
		{
			
			std::ifstream fin;
			fin.open("Savefile.txt");
			if (!fin) { 
				FileExist = false;
				Rec4.setFillColor(sf::Color::Color(100, 100, 100));
			}
			else {
				Rec4.setFillColor(sf::Color::Color(255, 255, 255));
				FileExist = true;
			
			}
			fin.close();
			if (event.type == sf::Event::Closed)
				window.close();
			
		
		}
		
		
		//Code to enlarge size of PLAY button if mouse hovers over it
		if (event.mouseMove.x >= 320 && event.mouseMove.x <= 480 && event.mouseMove.y >= 300 && event.mouseMove.y <= 350) {
		
			Rec2.setSize(sf::Vector2f(sizeButton.x*1.2, sizeButton.y*1.2));

			Rec2.setPosition(sf::Vector2f(304, 294.4));
		}
		else 
		{
			Rec2.setSize(sf::Vector2f(sizeButton.x, sizeButton.y));
			Rec2.setPosition(sf::Vector2f(320, 300));
		};


		//Code to enlarge size of LOAD button if mouse hovers over it
		if (event.mouseMove.x >= 320 && event.mouseMove.x <= 480 && event.mouseMove.y >= 360 && event.mouseMove.y <= 410 &&FileExist) {

			Rec4.setSize(sf::Vector2f(sizeExitButton.x*1.2, sizeExitButton.y*1.2));

			Rec4.setPosition(sf::Vector2f(304, 354.9));
		}
		else
		{
			Rec4.setSize(sf::Vector2f(sizeExitButton.x, sizeExitButton.y));
			Rec4.setPosition(sf::Vector2f(320, 360));
		};

		//Code to enlarge size of EXIT button if mouse hovers over it
		if (event.mouseMove.x >= 320 && event.mouseMove.x <= 480 && event.mouseMove.y >= 420 && event.mouseMove.y <= 470) {

			Rec3.setSize(sf::Vector2f(sizeExitButton.x*1.2, sizeExitButton.y*1.2));

			Rec3.setPosition(sf::Vector2f(304, 415.9));
		}
		else
		{
			Rec3.setSize(sf::Vector2f(sizeExitButton.x, sizeExitButton.y));
			Rec3.setPosition(sf::Vector2f(320, 420));
		};

		//------Code to draw all the objects-------
		window.clear(sf::Color::Black);
		window.draw(Rec);
		window.draw(Rec2);
		window.draw(Rec3);
		window.draw(Rec4);
		window.display();

		//------Code to draw all the objects END-------


		//------Code to run when EXIT button is pressed-------
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && (event.mouseMove.x >= 320 && event.mouseMove.x <= 480 && event.mouseMove.y >= 420 && event.mouseMove.y <= 470)) {
			window.close();
		}
	
		//------Code to run when PLAY button is pressed-------
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && event.mouseMove.x >= 320 && event.mouseMove.x <= 480 && event.mouseMove.y >= 300 && event.mouseMove.y <= 350) {
			FileExist = false;
			std::cout << "PLAY PRESSED\n";
			
			GameWindow(FileExist);

			window.setVisible(true);
			std::cout << "CLOSED\n";

		}
	
		//------Code to run when LOAD button is pressed (given save file exists)-------
		if (FileExist && sf::Mouse::isButtonPressed(sf::Mouse::Left) && event.mouseMove.x >= 320 && event.mouseMove.x <= 480 && event.mouseMove.y >= 320 && event.mouseMove.y <= 410) {
			
			std::cout << "PLAY PRESSED\n";
			
			GameWindow(FileExist);

			window.setVisible(true);
			std::cout << "CLOSED\n";

		}}

	return 0;
}

void GameWindow(bool GameExist) {
	//Creating objects for sprites,textures,texts etc and variables for the current points, remaing moves,target points and relevant candyID arrays (see documentation for info)
	int TargetVal = 10000, MovesVal = 25, Points = 0;
	int C = 0, R = 0;
	std::string TV = std::to_string(TargetVal);
	std::string MS = std::to_string(MovesVal);
	std::string CS = std::to_string(Points);
	bool isSelect = false;

	srand(time(NULL));
	bool FirstTimeRun = false;
	sf::RectangleShape LoadingWindow(sf::Vector2f(560, 560));
	sf::RectangleShape Candies[9][9];
	sf::RectangleShape GreenBox(sf::Vector2f(60, 60));
	int CandyIDs[9][9] = { 0 };
	
	sf::Texture BlueTexture;
	sf::Texture RedTexture;
	sf::Texture OrangeTexture;
	sf::Texture GreenTexture;
	sf::Texture YellowTexture;
	sf::Texture BS_HTexture;
	sf::Texture RS_HTexture;
	sf::Texture OS_HTexture;
	sf::Texture GS_HTexture;
	sf::Texture YS_HTexture;
	sf::Texture BS_VTexture;
	sf::Texture RS_VTexture;
	sf::Texture OS_VTexture;
	sf::Texture GS_VTexture;
	sf::Texture YS_VTexture;
	sf::Texture B_PACKTexture;
	sf::Texture R_PACKTexture;
	sf::Texture O_PACKTexture;
	sf::Texture G_PACKTexture;
	sf::Texture Y_PACKTexture;
	sf::Texture BombTexture;
	sf::Texture LoadingTexture;
	sf::Texture GreenBoxTexture;
	BS_VTexture.loadFromFile("BS_V.png");
	GS_VTexture.loadFromFile("GS_V.png");
	RS_VTexture.loadFromFile("RS_V.png");
	OS_VTexture.loadFromFile("OS_V.png");
	YS_VTexture.loadFromFile("YS_V.png");
	BombTexture.loadFromFile("BOMB1.png");
	BS_HTexture.loadFromFile("BS_H.png");
	GS_HTexture.loadFromFile("GS_H.png");
	RS_HTexture.loadFromFile("RS_H.png");
	OS_HTexture.loadFromFile("OS_H.png");
	YS_HTexture.loadFromFile("YS_H.png");
	
	B_PACKTexture.loadFromFile("B_PACK.png");
	G_PACKTexture.loadFromFile("G_PACK.png");
	R_PACKTexture.loadFromFile("R_PACK.png");
	O_PACKTexture.loadFromFile("O_PACK.png");
	Y_PACKTexture.loadFromFile("Y_PACK.png");

	GreenBoxTexture.loadFromFile("Asset3.png");
	LoadingTexture.loadFromFile("Asset2.png");
	BlueTexture.loadFromFile("B.png");
	RedTexture.loadFromFile("R.png");
	OrangeTexture.loadFromFile("O.png");
	GreenTexture.loadFromFile("G.png");
	YellowTexture.loadFromFile("Y.png");

	sf::Font ForteFont;
	ForteFont.loadFromFile("FORTE.TTF");

	sf::Text TargetScore;
	sf::Text CurrentScore;
	sf::Text ScoreTitle;
	sf::Text MovesRemaining;

	TargetScore.setFont(ForteFont);
	CurrentScore.setFont(ForteFont);
	ScoreTitle.setFont(ForteFont);
	MovesRemaining.setFont(ForteFont);


	TargetScore.setString(("Target: " +TV) );
	CurrentScore.setString((CS));
	MovesRemaining.setString(("Moves: "+MS)) ;
	ScoreTitle.setString(("SCORE"));

	TargetScore.setCharacterSize(30); 
	TargetScore.setFillColor(sf::Color::Color(123,65,30));

	CurrentScore.setCharacterSize(45);
	CurrentScore.setFillColor(sf::Color::Color(123, 65, 30));

	ScoreTitle.setCharacterSize(24);
	ScoreTitle.setFillColor(sf::Color::Color(123, 65, 30));

	MovesRemaining.setCharacterSize(32);
	MovesRemaining.setFillColor(sf::Color::Color(123, 65, 30));

	TargetScore.setPosition(sf::Vector2f(25, 70));
	MovesRemaining.setPosition(sf::Vector2f(35, 225));
	ScoreTitle.setPosition(sf::Vector2f(48, 305));
	CurrentScore.setPosition(sf::Vector2f(40, 353));
	
	
	sf::RectangleShape Savebutton(sf::Vector2f(160, 51));
	sf::Texture SaveTexture;
	SaveTexture.loadFromFile("SAVE.png");
	Savebutton.setTexture(&SaveTexture);
	Savebutton.setPosition(sf::Vector2f(35, 455));

	sf::RectangleShape Exitbutton(sf::Vector2f(160, 51));
	sf::Texture ExitTexture;
	ExitTexture.loadFromFile("Asset7.png");
	Exitbutton.setTexture(&ExitTexture);
	Exitbutton.setPosition(sf::Vector2f(35, 515));


	
	GreenBox.setTexture(&GreenBoxTexture);
	sf::RenderWindow Gamewindow(sf::VideoMode(800, 600), "Game window");
	sf::RectangleShape BG(sf::Vector2f(800, 600));
	LoadingWindow.setTexture(&LoadingTexture);
	
	sf::Texture BGTexture;
	BGTexture.loadFromFile("BGTEXT.png");
	BG.setTexture(&BGTexture);


	sf::RectangleShape SelectBox(sf::Vector2f(60, 60));
	sf::Texture BoxTexture;
	BoxTexture.loadFromFile("Asset1.png");
	SelectBox.setTexture(&BoxTexture);
	SelectBox.setPosition(sf::Vector2f(240, 30));


	sf::Vector2f BoxPos = SelectBox.getPosition();
	LoadingWindow.setPosition(sf::Vector2f(235, 25)); 
	
	GreenBox.setFillColor(sf::Color::Color(255, 255, 255, 0));
	if (GameExist == false) { //If file does not exist then generate new random values
		for (int r = 0; r < 9; r++) {
			for (int c = 0; c < 9; c++) {
				CandyIDs[r][c] = (rand() % 5);
			}
		}
	}
	else { //If file exists then take in the relevant CandyID values, remaining moves and current score

		std::ifstream fin;
		fin.open("Savefile.txt");
		for (int r = 0; r < 9; r++) {
			for (int c = 0; c < 9; c++) {
				fin >> CandyIDs[r][c];
			}
		}
		fin >> Points;
		fin >> MovesVal;
		fin.close();
	}


	for (int r = 0; r < 9; r++) {
		for (int c = 0; c < 9; c++) {
			std::cout << CandyIDs[r][c]<<" ";
		}
		std::cout << std::endl;
	}   
	//----Creation of objects END------


	while (Gamewindow.isOpen()) {
		while (Points<TargetVal && MovesVal>0 && Gamewindow.isOpen()){
		sf::Event GameEvent;
		while (Gamewindow.pollEvent(GameEvent)) {

			//convert int values to string 
			std::string TV = std::to_string(TargetVal);
			std::string MS = std::to_string(MovesVal);
			std::string CS = std::to_string(Points);
			
			TargetScore.setString(("Target: " + TV));
			CurrentScore.setString((CS));
			MovesRemaining.setString(("Moves: " + MS));
			ScoreTitle.setString(("SCORE"));

			if (sf::Mouse::isButtonPressed(sf::Mouse::Left) ){

				//Code to run when Exit button pressed
				if(sf::Mouse::getPosition(Gamewindow).x >= 35 && sf::Mouse::getPosition(Gamewindow).x <= 195 && sf::Mouse::getPosition(Gamewindow).y >= 515 && sf::Mouse::getPosition(Gamewindow).y <= 565) {

				Gamewindow.close();

						}

				//Code to run when Save button pressed
				if (sf::Mouse::getPosition(Gamewindow).x >= 35 && sf::Mouse::getPosition(Gamewindow).x <= 195 && sf::Mouse::getPosition(Gamewindow).y >= 405 && sf::Mouse::getPosition(Gamewindow).y <= 505) {
					std::ofstream fout;
					fout.open("Savefile.txt");
					for (int i = 0; i < 9; i++) {
						for (int j = 0; j < 9; j++) {
							fout << CandyIDs[i][j] << "\n";

						}
					}
					fout << Points << "\n";
					fout << MovesVal << "\n";
					fout.close();
				}
			}
			if (GameEvent.type == sf::Event::Closed) {
				Gamewindow.close();
			} //Close window if Close button on top is pressed


			  //Change size of Save button when mouse overs it  
			if (GameEvent.mouseMove.x >= 35 && GameEvent.mouseMove.x <= 195 && GameEvent.mouseMove.y >= 455 && GameEvent.mouseMove.y <= 505) {

				Savebutton.setSize(sf::Vector2f(160*1.2, 51*1.2));
				Savebutton.setPosition(sf::Vector2f(19, 450));
			}

			else
			{
				Savebutton.setSize(sf::Vector2f(160, 51));
				Savebutton.setPosition(sf::Vector2f(35, 455));
			};

			//Change size of Exit button when mouse overs it  
			if (GameEvent.mouseMove.x >= 35 && GameEvent.mouseMove.x <= 195 && GameEvent.mouseMove.y >= 515 && GameEvent.mouseMove.y <= 565) {
				
				Exitbutton.setSize(sf::Vector2f(160 * 1.2, 51 * 1.2));
				Exitbutton.setPosition(sf::Vector2f(19, 510));
				
			}

			else
			{
				Exitbutton.setSize(sf::Vector2f(160, 51));
				Exitbutton.setPosition(sf::Vector2f(35, 515));
			};

			


			if (sf::Mouse::getPosition(Gamewindow).x> 240 && sf::Mouse::getPosition(Gamewindow).x< 780 && sf::Mouse::getPosition(Gamewindow).y > 30 && sf::Mouse::getPosition(Gamewindow).y < 570) {
				SelectBox.setPosition(sf::Vector2f(240 + (60 * ((sf::Mouse::getPosition(Gamewindow).x - 240) / 60)), 30 + (60 * ((sf::Mouse::getPosition(Gamewindow).y - 30) / 60))));
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
					
					if (isSelect == false ) {

						 C = (((sf::Mouse::getPosition(Gamewindow).x) - 240) / 60);
						 R = (((sf::Mouse::getPosition(Gamewindow).y) - 30) / 60);
						 GreenBox.setPosition(sf::Vector2f(240 + (60 * ((sf::Mouse::getPosition(Gamewindow).x - 240) / 60)), 30 + (60 * ((sf::Mouse::getPosition(Gamewindow).y - 30) / 60))));
						  GreenBox.setFillColor(sf::Color::Color(255, 255, 255, 255));
						std::cout << "MAIN" <<std::endl;
					}


					if (isSelect == true ){
						std::cout << "SECOND" << std::endl;
					isCrush(CandyIDs, R, C, Gamewindow,isSelect,Points,Candies);
					MovesVal--;
					isSelect = false;

				GreenBox.setFillColor(sf::Color::Color(255, 255, 255, 0));
					}
					else {
						isSelect = true;
					}
					
				}
			}
		
			
			Gamewindow.clear(sf::Color::Black);
			Gamewindow.draw(BG); 
			Gamewindow.draw(ScoreTitle);
			Gamewindow.draw(MovesRemaining);
			Gamewindow.draw(CurrentScore);
			Gamewindow.draw(TargetScore);
			if (FirstTimeRun == false) { //Show the loading screen when the Gamewindow first loads
				Gamewindow.draw(LoadingWindow);
			}
			else{
			//Check of any combinations possible within the random generated candies
				Row5Check(CandyIDs, Points, MovesVal);
				Col5Check(CandyIDs, Points, MovesVal);
				TCheck(CandyIDs, Points, MovesVal);
				LCheck(CandyIDs, Points, MovesVal);
				Row4Check(CandyIDs, Points, MovesVal);
				Col4Check(CandyIDs, Points, MovesVal);
				RowCheck(CandyIDs, Points,MovesVal);
				ColCheck(CandyIDs, Points, MovesVal);
			//Checking end


				//Draw all the objects on the window
			Gamewindow.draw(SelectBox);
			Gamewindow.draw(Savebutton);
			Gamewindow.draw(Exitbutton);
			Gamewindow.draw(GreenBox);
			Display(Gamewindow, Candies, CandyIDs);
			}
			//Display the objects on the window
			Gamewindow.display();


			if (FirstTimeRun == false) { //Change value of the boolean variable and show the loading screen for 0.8 seconds
				FirstTimeRun = true;
				Sleep(800);

		}

		}	
	
			}

			//When game ends (either target is reached or moves run out)
		if (Gamewindow.isOpen())
{
			std::string TV = std::to_string(TargetVal);
			std::string MS = std::to_string(MovesVal);
			std::string CS = std::to_string(Points);


			//Creating objects for showing the win or lose sprite
			sf::RectangleShape WINLOSE;
			WINLOSE.setSize(sf::Vector2f(550, 550));
			WINLOSE.setPosition(sf::Vector2f(235, 25));
			sf::Texture WinTexture;
			sf::Texture LoseTexture;
			WinTexture.loadFromFile("YOUWIN.PNG");
			LoseTexture.loadFromFile("YOULOSE.PNG");
			WINLOSE.setFillColor(sf::Color::Color(255, 255, 255, 0));
			//Object creation end


			if (TargetVal <= Points) { //If current points are more than Target points (i.e. user wins)
				WINLOSE.setTexture(&WinTexture);
			}
			else {   //If current points are less than Target points (i.e. user loses)
				WINLOSE.setTexture(&LoseTexture);
			}



			for (int i = 0; i <= 255; i++) { //Show the user whether he/she lost or won
				WINLOSE.setFillColor(sf::Color::Color(255, 255, 255, i)); //Produces a fade-in effect
				Gamewindow.clear(sf::Color::Black);
				Gamewindow.draw(BG);
				Gamewindow.draw(ScoreTitle);
				Gamewindow.draw(MovesRemaining);
				Gamewindow.draw(CurrentScore);
				Gamewindow.draw(TargetScore);
				Gamewindow.draw(WINLOSE);
				Gamewindow.display();
				Sleep(10);
			}
			Sleep(2500);//Close the window after 2.5 seconds after  showing the result (win or lose) to user)
			Gamewindow.close();
		}
}
}


void RowCheck(int arr[][9], int &pts,int &moves) {
	
	int rowarr[3][3] = { 0 }; //Create a 3x3 array to store data (row , col and id of candy)
	for (int r = 0; r < 9; r++) {
		for (int c = 0; c <= 6; c++) { //Loop thru the array


			if (arr[r][c] % 5 == arr[r][c + 1] % 5 && arr[r][c + 2] % 5 == arr[r][c + 1] % 5)/* If the 3 candies in a row are same color*/ {
				pts = pts + (3*PointsCheck(arr[r][c])); //Change value of points
				
				for (int i = 0; i < 3; i++) { //Store data of all candies
					rowarr[i][0] = arr[r][c+i];
					rowarr[i][1] = r;
					rowarr[i][2] = c + i;
				}

				for (int i = 0; i < 3; i++) { 
					if (rowarr[i][0] >= 5 && rowarr[i][0] <= 9) /*Check whether any of candies is horizontal striped*/ {
						DestroyRow(arr, rowarr[i][1]);
					} else if (rowarr[i][0] >= 10 && rowarr[i][0] <= 14) /*Check whether any of candies is vertical striped*/ {
						DestroyCol(arr, rowarr[i][1],rowarr[i][2] );
					}else if (rowarr[i][0] >= 15 && rowarr[i][0] <= 19)/*Check whether any of candies is Packet*/  {
					DestroySqr(arr, rowarr[i][1], rowarr[i][2]);
					}
				}
				
				if (r == 0) { /*Check row of candies is in first row*/ 
					arr[0][c] = rand() % 5;
					arr[0][c + 1] = rand() % 5;
					arr[0][c + 2] = rand() % 5;
				
				}else{
				for (int i = r - 1; i >= 0; i--) { //Shift the above candies down
					arr[i + 1][c] = arr[i][c];
					arr[i + 1][c + 1] = arr[i][c + 1];
					arr[i + 1][c + 2] = arr[i][c + 2];
				}

				//Generate new candies in place of the destroyed one
				arr[0][c] = rand() % 5;
				arr[0][c + 1] = rand() % 5;
				arr[0][c + 2] = rand() % 5;
				}
		
				//Print column and rows of destroyed candies on console (debugging purposes)
				std::cout << "[" << r << "," << c << "]\n" ;
				std::cout  << "[" << r << "," << c + 1 << "]\n" ;
				std::cout << "[" << r << "," << c + 2 << "]\n";
				
			}
		}
	}

}

void ColCheck(int arr[][9], int &pts, int &moves) {
	int colarr[3][3] = { 0 };//Create a 3x3 array to store data (row , col and id of candy)

	for (int c = 0; c < 9; c++) {
		for (int r = 0; r <= 6; r++)/*Loop thru the array*/ {

			if (arr[r][c] % 5 == arr[r + 1][c] % 5 && arr[r + 1][c] % 5 == arr[r + 2][c] % 5) /* If 3 candies in column have same color */{
				pts = pts +( 3*PointsCheck(arr[r][c]));

				for (int i = 0; i < 3; i++) { //Store data of candies in array
					colarr[i][0] = arr[r+i][c ];
					colarr[i][1] = r+i;
					colarr[i][2] = c;
				}


				for (int i = 0; i < 3; i++) {
					if (colarr[i][0] >= 5 && colarr[i][0] <= 9)/*Check whether any of candies is horizontal striped*/ {
						DestroyRow(arr, colarr[i][1]);
					}
					else if (colarr[i][0] >= 10 && colarr[i][0] <= 14)/*Check whether any of candies is vertical striped*/ {
						DestroyCol(arr, colarr[i][1], colarr[i][2]);
					}
					else if (colarr[i][0] >= 15 && colarr[i][0] <= 19)/*Check whether any of candies is packet*/ {
						DestroySqr(arr, colarr[i][1], colarr[i][2]);
					}
				}
			for (int i = r - 1; i >= 0; i--) /*Shift candies downwards*/ {
					arr[i + 3][c] = arr[i][c];
				
				}
				for (int i = 0; i < 3; i++) {/*Generate new candies*/
					arr[i][c] = rand() % 5;
				
				}
				/*Print row and column of destroyed candies (debugging purpose)*/
				std::cout << "[" << r << "," << c << "]\n";
				std::cout << "[" << r + 1 << "," << c << "]\n"  ;
				std::cout << "[" << r + 2 << "," << c << "]\n"  ;
				
			}
		}
	}

}

void DispArr(int arr[][9], int &pts) {
	/*Display numeric values of candies on console (debugging purposes)*/
	{
		for (int r = 0; r < 9; r++) {
			for (int c = 0; c < 9; c++) {
				std::cout << arr[r][c] << " ";
			}
			std::cout << std::endl;
		}
		//std::cout << "Points : " << pts << std::endl;

	}
}

void isCrush(int arr[][9], int Row, int Col, sf::RenderWindow &GWindow, bool &Select, int &pts, sf::RectangleShape Candies[][9]) {
	int SecondR = (((sf::Mouse::getPosition(GWindow).y) - 30) / 60); //Row value of second clicked candy
	int SecondC = (((sf::Mouse::getPosition(GWindow).x) - 240) / 60); //Column value of second clicked candy
	int DifR = SecondR - Row; //Difference in row of 1st and 2nd candy
	int DifC = SecondC - Col;//Difference in column of 1st and 2nd candy
	if ((DifR==0 && (DifC==1 || DifC == -1))  || (DifC == 0 && (DifR == 1 || DifR == -1))) /*Check wether the second and first candy lie in a "+" orientation (see documentation for more info) */	{
	
	int tempIDs[9][9] = { 0 }; //a temporary created for comparison
	int temp = 0, count = 0; //temporary variables created, count variable counts the differences.
	bool changed = false; //variable that checks whether array has changes or not
	
	
	for (int i = 0; i < 9; i++) { //copy the value of main array into the temp one
		for (int j = 0; j < 9; j++) {
			tempIDs[i][j] = arr[i][j];
		}
	}
	if ((arr[Row][Col] == 20 && (arr[SecondR][SecondC] >= 0 && arr[SecondR][SecondC] <= 4)) || (arr[SecondR][SecondC] == 20 && (arr[Row][Col] >= 0 && arr[Row][Col] <= 4))) {
		//Code to run when candy bomb and plain candy are swapped
		int BombR = 0, BombC = 0, Candy = 0;
		if (arr[Row][Col] == 20) {
			BombR = Row;
			BombC = Col;
			Candy = arr[SecondR][SecondC];
		}
		else {
			BombR = SecondR;
			BombC = SecondC;
			Candy = arr[Row][Col];
		}
		Bomb_Plain(arr, BombR, BombC, Candy,pts);
	}
	else if (arr[Row][Col] == 20 && arr[SecondR][SecondC] == 20) {
		//Code to run when candy bomb and candy bomb are swapped
		Bomb_Bomb(arr, pts);
	}
	else if ((arr[Row][Col] == 20 && (arr[SecondR][SecondC] >= 5 && arr[SecondR][SecondC] <= 14)) || (arr[SecondR][SecondC] == 20 && (arr[Row][Col] >= 5 && arr[Row][Col] <= 14))) {
		//Code to run when candy bomb and striped candy are swapped
		int BombR = 0, BombC = 0, Candy = 0;
		if (arr[Row][Col] == 20) {
			BombR = Row;
			BombC = Col;
			Candy = arr[SecondR][SecondC];
		}
		else {
			BombR = SecondR;
			BombC = SecondC;
			Candy = arr[Row][Col];
		}
		Bomb_Stripe(arr, BombR, BombC, Candy,pts);
	}
	else if ((arr[Row][Col] == 20 && (arr[SecondR][SecondC] >= 15 && arr[SecondR][SecondC] <= 19)) || (arr[SecondR][SecondC] == 20 && (arr[Row][Col] >= 15 && arr[Row][Col] <= 19))) {
		//Code to run when candy bomb and packeted candy are swapped
		int BombR = 0, BombC = 0, Candy = 0;
		if (arr[Row][Col] == 20) {
			BombR = Row;
			BombC = Col;
			Candy = arr[SecondR][SecondC];
		}
		else {
			BombR = SecondR;
			BombC = SecondC;
			Candy = arr[Row][Col];
		}
		Bomb_Pack(arr, BombR, BombC, Candy, pts);
	}
	else if (((arr[Row][Col] >= 5 && arr[Row][Col] <= 14) && (arr[SecondR][SecondC] >= 15 && arr[SecondR][SecondC] <= 19)) || ((arr[Row][Col] >= 15 && arr[Row][Col] <= 19) && (arr[SecondR][SecondC] >= 5 && arr[SecondR][SecondC] <= 14))) {
		
		//Code to run when Striped and packet candy are swapped
		int Pack_R = 0, Pack_C = 0, Stripe_R = 0, Stripe_C = 0;
		if (arr[Row][Col] >= 5 && arr[Row][Col] <= 14) {
			Stripe_R = Row;
			Stripe_C = Col;
			Pack_R = SecondR;
			Pack_C = SecondC;
		}
		else {
			Stripe_R = SecondR;
			Stripe_C = SecondC;
			Pack_R = Row;
			Pack_C = Col;
		}

		Stripe_Pack(arr, Pack_R, Pack_C, Stripe_R, Stripe_C, pts);
	}
	else if (((arr[Row][Col] >= 15 && arr[Row][Col] <= 19) && (arr[SecondR][SecondC] >= 15 && arr[SecondR][SecondC] <= 19))) {
		//Code to run when bomb and plain candy are swapped

		int Pack1_R = 0, Pack1_C = 0, Pack_R = 0, Pack_C = 0;
		Pack1_R = Row;
		Pack1_C = Col;
		Pack_R = SecondR;
		Pack_C = SecondC;
		Pack_Pack(arr, Pack1_R, Pack1_C, Pack_R, Pack_C, pts);
	}
	else
	{
		//Code to run for any other combination of candies 

		/*Perform checks on the temp array*/
		Swap(tempIDs[Row][Col], tempIDs[SecondR][SecondC]);
		Row5Check(tempIDs, pts, temp);
		Col5Check(tempIDs, pts, temp);
		TCheck(tempIDs, pts, temp);
		LCheck(tempIDs, pts, temp);
		Row4Check(tempIDs, pts, temp);
		Col4Check(tempIDs, pts, temp);
		RowCheck(tempIDs, pts, temp);
		ColCheck(tempIDs, pts, temp);
		/*Temp array checks end*/

		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 9; j++) {
				if (tempIDs[i][j] != arr[i][j])/*Check whether the temp array is different from main array*/ {
					count++;
			
				}
			}
		}

		if (count > 2) /*If count is more than 2 (swapping of candies causes crushing ), copy the temp array into main array*/{
	
			for (int i = 0; i < 9; i++) {

				for (int j = 0; j < 9; j++) {

					arr[i][j] = tempIDs[i][j];

				}

				Display(GWindow, Candies, arr);//Display the swapped candies on the grid 
				Sleep(5);//Minor pause to show the shifting 


			}


		}
	
		std::cout << SecondR << " || " << SecondC << std::endl; //Print rows and column of 2nd click candy
		std::cout << Row << " || " << Col << std::endl; //Print rows and column of 1st click candy

	
		

		DispArr(arr, temp); //show array on console
	}
}

}

void Swap(int &num1, int &num2) {
	int temp = 0;
	temp = num1;
	num1 = num2;
	num2 = temp;
}

void Row4Check(int arr[][9], int &pts, int &moves) {
	
	int rowarr[4][3] = { 0 }; //Create array to store data of candies
	for (int r = 0; r < 9; r++) {
		for (int c = 0; c <= 5; c++) { //Loop thru the array
			if (arr[r][c] % 5 == arr[r][c + 1] % 5 && arr[r][c + 2] % 5 == arr[r][c + 1] % 5 && arr[r][c + 2] % 5 == arr[r][c + 3] % 5) {
				
				pts = pts +(4* PointsCheck(arr[r][c]));


				for (int i = 0; i < 4; i++) { //Store data of candies in array
					rowarr[i][0] = arr[r][c + i];
					rowarr[i][1] = r;
					rowarr[i][2] = c + i;
				}


				for (int i = 0; i < 4; i++) {
					if (rowarr[i][0] >= 5 && rowarr[i][0] <= 9) {
						DestroyRow(arr, rowarr[i][1]); //Check if any horizontal striped candies exist
					}
					else if (rowarr[i][0] >= 10 && rowarr[i][0] <= 14) {
						DestroyCol(arr, rowarr[i][1], rowarr[i][2]);//Check if any vertical striped candies exist
					}
					else if (rowarr[i][0] >= 15 && rowarr[i][0] <= 19) {
						DestroySqr(arr, rowarr[i][1], rowarr[i][2]); //Check if any packet candies exist
					}
				}

				arr[r][c + 3] = arr[r][c] + 5; //spawn a horizontal striped candy

				if (r == 0) { //if first row is destroyed
					arr[0][c + 3] = arr[0][c + 3]+5;
					arr[0][c] = rand() % 5;
					arr[0][c + 1] = rand() % 5;
					arr[0][c + 2] = rand() % 5;
					
				
				}
				else { //shift old candies and spawn new candies
					for (int i = r - 1; i >= 0; i--) {
						arr[i + 1][c] = arr[i][c];
						arr[i + 1][c + 1] = arr[i][c + 1];
						arr[i + 1][c + 2] = arr[i][c + 2];
						arr[0][c] = rand() % 5;
						arr[0][c + 1] = rand() % 5;
						arr[0][c + 2] = rand() % 5;
					
					}
				}
				std::cout << "HOR-STRIPED FORMED";
				std::cout << "[" << r << "," << c << "]\n";

				std::cout << "[" << r << "," << c + 1 << "]\n";

				std::cout << "[" << r << "," << c + 2 << "]\n";
				DispArr(arr, pts);

			}
		}
	}

}

void Col4Check(int arr[][9], int &pts, int &moves){
	int colarr[4][3] = { 0 }; //Create array to store data of candies

	for (int c = 0; c < 9; c++) {
		for (int r = 0; r <= 5; r++) {

			if (arr[r][c] % 5 == arr[r + 1][c] % 5 && arr[r + 1][c] % 5 == arr[r + 2][c] % 5 && arr[r + 3][c] % 5 == arr[r + 2][c] % 5) {
				pts = pts + (4 * PointsCheck(arr[r][c]));
				
				for (int i = 0; i < 4; i++) { //Store data of candies
					colarr[i][0] = arr[r + i][c];
					colarr[i][1] = r + i;
					colarr[i][2] = c;
				}
				for (int i = 0; i < 4; i++) {
					if (colarr[i][0] >= 5 && colarr[i][0] <= 9) {
						DestroyRow(arr, colarr[i][1]); //Check if any horizontal striped candies exist
					}
					else if (colarr[i][0] >= 10 && colarr[i][0] <= 14) {
						DestroyCol(arr, colarr[i][1], colarr[i][2]); //Check if any vertical striped candies exist
					}
					else if (colarr[i][0] >= 15 && colarr[i][0] <= 19) {
						DestroySqr(arr, colarr[i][1], colarr[i][2]); //Check if any packeted candies exist
					}
				}
				arr[r + 3][c] = arr[r + 3][c] +10; //spawn vertical candy
	
				for (int i = r - 1; i >= 0; i--) {//shift old candies down
					arr[i + 3][c] = arr[i][c];
				
				}
				for (int i = 0; i < 3; i++) { //generate new candies
					arr[i][c] = rand() % 5;
					
				}
				std::cout << "[" << r << "," << c << "]\n";

				std::cout << "[" << r + 1 << "," << c << "]\n";

				std::cout << "[" << r + 2 << "," << c << "]\n";
				DispArr(arr, pts);
				std::cout << "VER-STRIPED FORMED";
			}
		}
	}
}

void Row5Check(int arr[][9], int &pts, int &moves) {
	int rowarr[5][3] = { 0 }; //Create new array to store data about candies
	for (int r = 0; r < 9; r++) {
		for (int c = 0; c <= 4; c++) {
			if (arr[r][c] % 5 == arr[r][c + 1] % 5 && arr[r][c + 2] % 5 == arr[r][c + 1] % 5 && arr[r][c + 2] % 5 == arr[r][c + 3] % 5 && arr[r][c + 2] % 5 == arr[r][c + 4] % 5) {
			
				pts = pts + (5 * PointsCheck(arr[r][c]));

				for (int i = 0; i < 5; i++) {//Store data of candies
					rowarr[i][0] = arr[r][c + i];
					rowarr[i][1] = r;
					rowarr[i][2] = c + i;
				}
				for (int i = 0; i < 5; i++) {
					if (rowarr[i][0] >= 5 && rowarr[i][0] <= 9) { //Check if any horizontal striped candies exist
						DestroyRow(arr, rowarr[i][1]);
					}
					else if (rowarr[i][0] >= 10 && rowarr[i][0] <= 14) {
						DestroyCol(arr, rowarr[i][1], rowarr[i][2]);//Check if any vertical striped candies exist
					}
					else if (rowarr[i][0] >= 15 && rowarr[i][0] <= 19) {
						DestroySqr(arr, rowarr[i][1], rowarr[i][2]);//Check if any packeted candies exist
					}
				}
				arr[r][c + 4] = 20; //spawn bomb
				if (r == 0) { //if first row is crushed

					arr[0][c + 4] =20;
					arr[0][c] = rand() % 5;
					arr[0][c + 1] = rand() % 5;
					arr[0][c + 2] = rand() % 5;
					arr[0][c + 3] = rand() % 5;
				
				}
				else {
					for (int i = r - 1; i >= 0; i--) {
						//shift old candies down and generate new candies
						arr[i + 1][c] = arr[i][c];
						arr[i + 1][c + 1] = arr[i][c + 1];
						arr[i + 1][c + 2] = arr[i][c + 2];
						arr[i + 1][c + 3] = arr[i][c + 3];
						arr[0][c] = rand() % 5;
						arr[0][c + 1] = rand() % 5;
						arr[0][c + 2] = rand() % 5;
						arr[0][c + 3] = rand() % 5;
						
					}
				}
				std::cout << "[" << r << "," << c << "]\n";

				std::cout << "[" << r << "," << c + 1 << "]\n";

				std::cout << "[" << r << "," << c + 2 << "]\n";

				std::cout << "[" << r << "," << c + 3 << "]\n";
				DispArr(arr, pts);

			}
		}
	}

}

void Col5Check(int arr[][9], int &pts, int &moves) {
	
	int colarr[5][3] = { 0 }; //Store data of candies
	for (int c = 0; c < 9; c++) {
		for (int r = 0; r <= 4; r++) {
			if (arr[r][c] % 5 == arr[r + 1][c] % 5 && arr[r + 1][c] % 5 == arr[r + 2][c] % 5 && arr[r + 3][c] % 5 == arr[r + 2][c] % 5 && arr[r + 3][c] % 5 == arr[r + 4][c] % 5) {
				pts = pts + (5 * PointsCheck(arr[r][c]));
				for (int i = 0; i < 5; i++) {
					colarr[i][0] = arr[r + i][c];
					colarr[i][1] = r + i;
					colarr[i][2] = c;
				}
				for (int i = 0; i < 5; i++) {
					if (colarr[i][0] >= 5 && colarr[i][0] <= 9) {
						DestroyRow(arr, colarr[i][1]); //Check if any horizontal striped candies exist
					}
					else if (colarr[i][0] >= 10 && colarr[i][0] <= 14) {
						DestroyCol(arr, colarr[i][1], colarr[i][2]);//Check if any vertical striped candies exist
					}
					else if (colarr[i][0] >= 15 && colarr[i][0] <= 19) {
						DestroySqr(arr, colarr[i][1], colarr[i][2]);//Check if any packeted candies exist
					}
				}
				arr[r + 4][c] = 20;//spawn bomb
		
				for (int i = r - 1; i >= 0; i--) {
					arr[i + 4][c] = arr[i][c];//shift old candies down
	
				
				}
				for (int i = 0; i < 4; i++) {
					arr[i][c] = rand() % 5;//generate new candies

					
				}
				std::cout << "[" << r << "," << c << "]\n";

				std::cout << "[" << r + 1 << "," << c << "]\n";

				std::cout << "[" << r + 2 << "," << c << "]\n";
				std::cout << "[" << r + 3 << "," << c << "]\n";

				DispArr(arr, pts);

			}
		}
	}
}

void LCheck(int arr[][9], int &pts, int &moves) {
	/*Code to run if normal L is formed*/
	for (int i = 2; i <= 8; i++) {
		for (int j = 0; j <= 6; j++) {
			if (arr[i][j] % 5 == arr[i][j + 1] % 5 && arr[i][j] % 5 == arr[i][j + 2] % 5 && arr[i][j] % 5 == arr[i - 1][j] % 5 && arr[i][j] % 5 == arr[i - 2][j] % 5) {
				pts = pts + (5 * PointsCheck(arr[i][j]));

				arr[i][j] = arr[i][j] + 15;
				for (int k = i - 3; k >= 0; k--) {
					arr[k + 2][j] = arr[k][j];
				}
				for (int k = 0; k < 2; k++) {
					arr[k][j] = rand() % 5;
				}

				for (int z = i - 1; z >= 0; z--) {
					arr[z + 1][j + 1] = arr[z][j + 1];
					arr[z + 1][j + 2] = arr[z][j + 2];
				}
				arr[0][j + 1] = rand() % 5;
				arr[0][j + 2] = rand() % 5;
				std::cout << "[" << i << "," << j << "]\n";
				DispArr(arr, pts);
			}
		}
	}

	/*Code to run if inverted L (see documentation for more info) is formed*/
	for (int i = 2; i <= 8; i++) {
		for (int j = 2; j <= 8; j++) {
			if (arr[i][j] % 5 == arr[i][j - 1] % 5 && arr[i][j] % 5 == arr[i][j - 2] % 5 && arr[i][j] % 5 == arr[i - 1][j] % 5 && arr[i][j] % 5 == arr[i - 2][j] % 5) {
				pts = pts + (5 * PointsCheck(arr[i][j]));

				arr[i][j] = arr[i][j] + 15;
				for (int k = i - 3; k >= 0; k--) {
					arr[k + 2][j] = arr[k][j];
				}
				for (int k = 0; k < 2; k++) {
					arr[k][j] = rand() % 5;
				}

				for (int z = i - 1; z >= 0; z--) {
					arr[z + 1][j - 1] = arr[z][j - 1];
					arr[z + 1][j - 2] = arr[z][j - 2];
				}
				arr[0][j - 1] = rand() % 5;
				arr[0][j - 2] = rand() % 5;
				std::cout << "[" << i << "," << j << "]\n";
				DispArr(arr, pts);
			}
		}
	}

	/*Code to run if flipped L (see documentation for more info) is formed*/
	for (int i = 0; i <= 6; i++) {
		for (int j = 0; j <= 6; j++) {
			if (arr[i][j] % 5 == arr[i][j + 1] % 5 && arr[i][j] % 5 == arr[i][j + 2] % 5 && arr[i][j] % 5 == arr[i + 1][j] % 5 && arr[i][j] % 5 == arr[i + 2][j] % 5) {
				pts = pts + (5 * PointsCheck(arr[i][j]));
				int temp  = arr[i][j] + 15;
			
				for (int k = i - 1; k >= 0; k--) {
					arr[k + 3][j] = arr[k][j];
				}
				for (int k = 0; k < 3; k++) {
					arr[k][j] = rand() % 5;
				}

				for (int z = i - 1; z >= 0; z--) {
					arr[z + 1][j + 1] = arr[z][j + 1];
					arr[z + 1][j + 2] = arr[z][j + 2];
				}
				arr[i][j] = temp;
				arr[0][j + 1] = rand() % 5;
				arr[0][j + 2] = rand() % 5;
				std::cout << "[" << i << "," << j << "]\n";
				DispArr(arr, pts);
			}
		}
	}

	/*Code to run if inverted and flipped L (see documentation for more info) is formed*/
	for (int i = 0; i <= 6; i++) {
		for (int j = 2; j <= 8; j++) {
			if (arr[i][j] % 5 == arr[i][j - 1] % 5 && arr[i][j] % 5 == arr[i][j - 2] % 5 && arr[i][j] % 5 == arr[i + 1][j] % 5 && arr[i][j] % 5 == arr[i + 2][j] % 5) {
				pts = pts + (5 * PointsCheck(arr[i][j]));
				int temp = arr[i][j] + 15;

				for (int k = i - 1; k >= 0; k--) {
					arr[k + 3][j] = arr[k][j];
				}
				for (int k = 0; k < 3; k++) {
					arr[k][j] = rand() % 5;
				}

				for (int z = i - 1; z >= 0; z--) {
					arr[z + 1][j - 1] = arr[z][j - 1];
					arr[z + 1][j - 2] = arr[z][j - 2];
				}
				arr[i][j] = temp;
				arr[0][j - 1] = rand() % 5;
				arr[0][j - 2] = rand() % 5;
				std::cout << "[" << i << "," << j << "]\n";
				DispArr(arr, pts);
			}
		}
	}

}

void TCheck(int arr[][9], int &pts, int &moves) {
	
	/*Code to run if normal T (see documentation for more info) is formed*/
	for (int r = 0; r <= 6; r++) {
		for (int c = 1; c <= 7; c++) {
			if (arr[r][c]%5 == arr[r][c + 1]%5 && arr[r][c] % 5 == arr[r][c - 1] % 5 && arr[r + 1][c] % 5 == arr[r][c] % 5 && arr[r + 2][c] % 5 == arr[r][c] % 5) {
				arr[r + 2][c] = arr[r + 2][c] + 15;
				pts = pts + (4 * PointsCheck(arr[r][c]));

				for (int i = r - 1; i >= 0; i--) {
					arr[i + 2][c] = arr[i][c];
					arr[i + 1][c + 1] = arr[i][c + 1];
					arr[i + 1][c - 1] = arr[i][c - 1];

				}
				arr[0][c] = rand() % 5;
				arr[1][c] = rand() % 5;
				arr[0][c + 1] = rand() % 5;
				arr[0][c - 1] = rand() % 5;

				std::cout << "[" << r << "," << c << "]\n";
				DispArr(arr, pts);
			}
		}
	}

	/*Code to run if inverted T (see documentation for more info) is formed*/
	for (int r = 2; r <= 8; r++) {
		for (int c = 1; c <= 7; c++) {
			if (arr[r][c] % 5 == arr[r][c + 1] % 5 && arr[r][c] % 5 == arr[r][c - 1] % 5 && arr[r - 1][c] % 5 == arr[r][c] % 5 && arr[r - 2][c] % 5 == arr[r][c] % 5) {
				arr[r][c] = arr[r][c] + 15;
				pts = pts + (4 * PointsCheck(arr[r][c]));

				for (int i = r - 3; i >= 0; i--) {
					arr[i + 2][c] = arr[i][c];
					}
				for (int i = r - 1; i >= 0; i--) {
				
					arr[i + 1][c + 1] = arr[i][c + 1];
					arr[i + 1][c - 1] = arr[i][c - 1];

				}


				arr[0][c] = rand() % 5;
				arr[1][c] = rand() % 5;
				arr[0][c + 1] = rand() % 5;
				arr[0][c - 1] = rand() % 5;

				std::cout << "[" << r << "," << c << "]\n";
				DispArr(arr, pts);
			}
		}
	}


	/*Code to run if flipped T (see documentation for more info) is formed*/
	for (int r = 1; r <= 7; r++) {
		for (int c = 0; c <= 6; c++) {
			if (arr[r][c] % 5 == arr[r][c + 1] % 5 && arr[r][c] % 5 == arr[r][c + 2] % 5 && arr[r + 1][c] % 5 == arr[r][c] % 5 && arr[r - 1][c] % 5 == arr[r][c] % 5) {
				arr[r+1][c]= arr[r][c] + 15;
				pts = pts + (4 * PointsCheck(arr[r][c]));

				for (int i = r - 2; i >= 0; i--) {
					arr[i + 2][c] = arr[i][c];
				}
				for (int i = r - 1; i >= 0; i--) {

					arr[i + 1][c + 1] = arr[i][c + 1];
					arr[i + 1][c + 2] = arr[i][c + 2];

				}


				arr[0][c] = rand() % 5;
				arr[1][c] = rand() % 5;
				arr[0][c + 1] = rand() % 5;
				arr[0][c + 2] = rand() % 5;

				std::cout << "[" << r << "," << c << "]\n";
				DispArr(arr, pts);
			}
		}
	}


	/*Code to run if inverted and flipped T (see documentation for more info) is formed*/
	for (int r = 1; r <= 7; r++) {
		for (int c = 2; c <= 8; c++) {
			if (arr[r][c] % 5 == arr[r][c - 1] % 5 && arr[r][c] % 5 == arr[r][c - 2] % 5 && arr[r + 1][c] % 5 == arr[r][c] % 5 && arr[r - 1][c] % 5 == arr[r][c] % 5) {
				arr[r + 1][c] = arr[r][c] + 15;
				pts = pts + (4 * PointsCheck(arr[r][c]));

				for (int i = r - 2; i >= 0; i--) {
					arr[i + 2][c] = arr[i][c];
				}
				for (int i = r - 1; i >= 0; i--) {

					arr[i + 1][c - 1] = arr[i][c - 1];
					arr[i + 1][c - 2] = arr[i][c - 2];

				}


				arr[0][c] = rand() % 5;
				arr[1][c] = rand() % 5;
				arr[0][c - 1] = rand() % 5;
				arr[0][c - 2] = rand() % 5;

				std::cout << "[" << r << "," << c << "]\n";
				DispArr(arr, pts);
			}
		}
	}

}

void DestroyRow(int arr[][9], int RowVal) {

	/*Code to run to shift above rows down*/
	for (int i = RowVal - 1; i >= 0; i--) {
		for (int j = 0; j < 9; j++) {
			arr[i + 1][j] = arr[i][j];
		}
	}

	/*Code to run to generate new candies in first row*/
	for (int j = 0; j < 9; j++) {
		arr[0][j] =rand()%5;
	}
	std::cout << "ROW " << RowVal << " DESTROYED\n";
}
void DestroyCol(int arr[][9], int RowVal, int ColVal) {

	/*Code to run to generate new candies in destroyed column*/
	for (int i = 0; i < 9; i++) {
		arr[i][ColVal] = rand() % 5;
	}

	std::cout << "COL " << ColVal << " DESTROYED\n";
}
void DestroySqr(int arr[][9], int RowVal, int ColVal) {
	/*Code to generate center position of 3x3 area in case candies are on edge of grid*/
	{
		if (RowVal == 0) {
			RowVal++;
		}
		else if (RowVal == 8) {
			RowVal--;
		}
		if (ColVal == 0) {
			ColVal++;
		}
		else if (ColVal == 8) {
			ColVal--;
		}
}


	int minR = 0, maxR = 0, minC = 0, maxC = 0;
	minR = RowVal - 1;
	minC = ColVal - 1;
	maxR = RowVal + 1;
	maxC = ColVal + 1;

	/*Code to run to shift candies down into the destroyed area*/
	for (int i = minR - 1; i >= 0; i--) {
		for (int j = minC ; j <=maxC; j++) {
			arr[i+3][j] = arr[i][j];

		}
	}
	/*Code to run to generate new candies into the destroyed area*/
	for (int i = 0; i <3; i++) {
		for (int j = minC; j <= maxC; j++) {
			arr[i][j] = rand()%5;

		}
	}
	std::cout << "BOMB EXPLODED\n";
}
void Bomb_Plain(int arr[][9], int BombR, int BombC,int CandyID, int &pts) {


	/*Code to run to shift new candies down where the candy bomb exist*/
	for (int i = BombR - 1; i >= 0; i--) {
		arr[i + 1][BombC] = arr[i][BombC];
	}
	arr[0][BombC] = rand() % 5; /*Code to run to generate new candy into the first row of column where candy bomb existed*/
	
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			if (arr[i][j] == CandyID)/*Code to check if array has any candy matching the swapped one*/	 {
				pts = pts + PointsCheck(CandyID);
				for (int k = i - 1; k >= 0; k--) /*Code to shift candies down where the matching candy existed*/ {
					arr[k+1][j] = arr[k][j];
				}
				arr[0][j] = rand()&5;/*Code to generate new candy in the first row of the column of the matching candy*/
			}
		}
	}
	std::cout << "BOMB + PLAIN\n";
}
void Bomb_Bomb(int arr[][9], int &pts) {
	/*Code generate entirely new random board*/
	pts = pts + (2430);
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			arr[i][j] = rand() & 5;
		}
	}

	std::cout << "BOMB + BOMB\n";
}
void Bomb_Stripe(int arr[][9], int BombR, int BombC, int CandyID, int &pts) {
	
	/*Code to run to shift new candies down where the candy bomb exist*/
	for (int i = BombR-1; i >= 0; i--) {
		arr[i + 1][BombC] = arr[i][BombC];
	}
	arr[0][BombC] = rand()%5;/*Code to run to generate new candy into the first row of column where candy bomb existed*/
	
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			if (arr[i][j] == (CandyID%5)) /*Code to find candy of same color as striped candy*/ {
				if (CandyID >= 5 && CandyID <= 9) /*Code to destroy row if swapped candy was horizontal striped candy*/ {
					DestroyRow(arr, i);
					pts = pts + 270;
				}
				else if (CandyID >= 10 && CandyID <= 14)/*Code to destroy row if swapped candy was vertical striped candy*/  {
					DestroyCol(arr, i,j);
					pts = pts + 270;
				}
			
			}
		}
	}

	std::cout << "BOMB + Stripe\n";
}
void Bomb_Pack(int arr[][9], int BombR, int BombC, int CandyID, int &pts) {

	std::cout << "BOMB + PACK\n";
	for (int i = BombR - 1; i >= 0; i--) /*Code to shift candies down into the place where candy bomb existed*/ {
		arr[i + 1][BombC] = arr[i][BombC];
		
	}
	DestroySqr(arr, 0, BombC);  /*Code to destroy 3x3 square around the pakceted candy*/

	arr[0][BombC] = rand() % 5;  /*Code to generate new candy in first row of column where candy bomb existed*/
	pts = pts + PointsCheck(CandyID);
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			if (arr[i][j] == (CandyID % 5))  /*Code to find candies of same color as packeted candy and replace it with packeted candy*/ {
				arr[i][j] = CandyID;
			}
		}
	}
}
void Stripe_Pack(int arr[][9], int Pack_R, int Pack_C, int Stripe_R, int Stripe_C, int &pts) {
	int Center_Row=0, Center_Col=0;

	std::cout << "STRIPE + PACK\n";

	//Code to find the center column and row from the 3 row and columns to be destroyed
	if (Pack_R == 0 || Stripe_R == 0) {
		Center_Row = 1;
	} else if (Pack_R == 8 || Stripe_R == 8) {
		Center_Row = 7;
	}
	else {
		Center_Row = Pack_R;
	}
	if (Pack_C == 0 || Stripe_C == 0) {
		Center_Col = 1;
	}
	else if (Pack_C == 8 || Stripe_C == 8) {
		Center_Col = 7;
	}
	else {
		Center_Col = Pack_C;
	}
	// Column and Row finding code end 


	for (int i = Center_Row - 1; i <= Center_Row + 1; i++) { //Destroy the 3 rows
		DestroyRow(arr, i);
		pts = pts + (9 * PointsCheck(arr[Pack_R][Pack_C]));
	}
	for (int i = Center_Col - 1; i <= Center_Col + 1; i++) { //Destroy the 3 columns
		DestroyCol(arr, 10,i);
		pts = pts + (9*PointsCheck(arr[Pack_R][Pack_C]));
	}
}
void Pack_Pack(int arr[][9], int Stripe1_R, int Stripe1_C, int Stripe_R, int Stripe_C, int &pts) {
	int Center_Row = 0, Center_Col = 0;

	std::cout << "PACK+ PACK\n";

	//Code to find column and row in case candy swapped existed on edge of grid
	if (Stripe1_C < 2 && Stripe_C < 2) {
		Center_Col = 2;
	} else if (Stripe1_C > 6 && Stripe_C >6) {
		Center_Col = 6;
	}
	else {
		if (Stripe1_C <= Stripe_C)
			Center_Col = Stripe1_C;
		else
			Center_Col = Stripe_C;
	}

	if (Stripe1_R < 2 && Stripe_R < 2) {
		Center_Row = 2;
	}
	else if (Stripe1_R > 6 && Stripe_R > 6) {
		Center_Row = 6;
	}
	else {
		if (Stripe1_R <= Stripe_R)
			Center_Row = Stripe1_R;
		else
			Center_Row = Stripe_R;
	}
	// Column and Row finding code end 


	int minR = 0, maxR = 0, minC = 0, maxC = 0;
	minR = Center_Row - 2;
	minC = Center_Col - 2;
	maxR = Center_Row + 2;
	maxC = Center_Col + 2;



	for (int i = minR - 1; i >= 0; i--) {
		for (int j = minC; j <= maxC; j++) { //Shifting above candies down to the space of destroyed 5x5 area
			arr[i + 5][j] = arr[i][j];

		}
	}
	for (int i = 0; i <5; i++) {
		for (int j = minC; j <= maxC; j++) { //Generating candies in first 5 rows and column of destroyed 5x5 area
			arr[i][j] = rand() % 5;

		}
	}
	pts = pts + (25 * PointsCheck(arr[Center_Row][Center_Col]));
	std::cout << "BIG BOMB EXPLODED\n";
}
void Display(sf::RenderWindow &GWin, sf::RectangleShape Candies[][9],int CandyIDs[][9]) {
	/*Creating sprites,textures, rectanges for candies*/
	sf::Texture BlueTexture;
	sf::Texture RedTexture;
	sf::Texture OrangeTexture;
	sf::Texture GreenTexture;
	sf::Texture YellowTexture;
	sf::Texture BS_HTexture;
	sf::Texture RS_HTexture;
	sf::Texture OS_HTexture;
	sf::Texture GS_HTexture;
	sf::Texture YS_HTexture;
	sf::Texture BS_VTexture;
	sf::Texture RS_VTexture;
	sf::Texture OS_VTexture;
	sf::Texture GS_VTexture;
	sf::Texture YS_VTexture;
	sf::Texture B_PACKTexture;
	sf::Texture R_PACKTexture;
	sf::Texture O_PACKTexture;
	sf::Texture G_PACKTexture;
	sf::Texture Y_PACKTexture;
	sf::Texture BombTexture;
	sf::Texture LoadingTexture;
	sf::Texture GreenBoxTexture;
	BS_VTexture.loadFromFile("BS_V.png");
	GS_VTexture.loadFromFile("GS_V.png");
	RS_VTexture.loadFromFile("RS_V.png");
	OS_VTexture.loadFromFile("OS_V.png");
	YS_VTexture.loadFromFile("YS_V.png");
	BombTexture.loadFromFile("BOMB1.png");
	BS_HTexture.loadFromFile("BS_H.png");
	GS_HTexture.loadFromFile("GS_H.png");
	RS_HTexture.loadFromFile("RS_H.png");
	OS_HTexture.loadFromFile("OS_H.png");
	YS_HTexture.loadFromFile("YS_H.png");

	B_PACKTexture.loadFromFile("B_PACK.png");
	G_PACKTexture.loadFromFile("G_PACK.png");
	R_PACKTexture.loadFromFile("R_PACK.png");
	O_PACKTexture.loadFromFile("O_PACK.png");
	Y_PACKTexture.loadFromFile("Y_PACK.png");

	GreenBoxTexture.loadFromFile("Asset3.png");
	LoadingTexture.loadFromFile("Asset2.png");
	BlueTexture.loadFromFile("B.png");
	RedTexture.loadFromFile("R.png");
	OrangeTexture.loadFromFile("O.png");
	GreenTexture.loadFromFile("G.png");
	YellowTexture.loadFromFile("Y.png");
	/*Creating sprites,textures, rectanges for candies END
	*/

	for (int c = 0; c < 9; c++) {
		for (int r = 0; r < 9; r++) {
			
			Candies[r][c].setSize(sf::Vector2f(60, 60)); //Seting size of each candy
			Candies[c][r].setPosition(sf::Vector2f(240 + (r * 60), 30 + (c * 60))); //Seting location of each candy on grid



			switch (CandyIDs[r][c])/*Setting texture of each candy*/ {
			case 0: {
				Candies[r][c].setTexture(&BlueTexture);
				break;
			}
			case 1: {
				Candies[r][c].setTexture(&RedTexture);
				break;
			}
			case 2: {
				Candies[r][c].setTexture(&OrangeTexture);
				break;

			}
			case 3: {
				Candies[r][c].setTexture(&GreenTexture);
				break;

			}
			case 4: {
				Candies[r][c].setTexture(&YellowTexture);
				break;

			}
			case 5: {
				Candies[r][c].setTexture(&BS_HTexture);
				break;

			}
			case 6: {
				Candies[r][c].setTexture(&RS_HTexture);
				break;

			}case 7: {
				Candies[r][c].setTexture(&OS_HTexture);
				break;

			}case 8: {
				Candies[r][c].setTexture(&GS_HTexture);
				break;

			}case 9: {
				Candies[r][c].setTexture(&YS_HTexture);
				break;

			}case 10: {
				Candies[r][c].setTexture(&BS_VTexture);
				break;

			}case 11: {
				Candies[r][c].setTexture(&RS_VTexture);
				break;

			}case 12: {
				Candies[r][c].setTexture(&OS_VTexture);
				break;

			}case 13: {
				Candies[r][c].setTexture(&GS_VTexture);
				break;

			}case 14: {
				Candies[r][c].setTexture(&YS_VTexture);
				break;

			}case 15: {
				Candies[r][c].setTexture(&B_PACKTexture);
				break;

			}case 16: {
				Candies[r][c].setTexture(&R_PACKTexture);
				break;

			}case 17: {
				Candies[r][c].setTexture(&O_PACKTexture);
				break;

			}case 18: {
				Candies[r][c].setTexture(&G_PACKTexture);
				break;

			}case 19: {
				Candies[r][c].setTexture(&Y_PACKTexture);
				break;

			}case 20: {
				Candies[r][c].setTexture(&BombTexture);
				break;

			}

			}

			GWin.draw(Candies[r][c]);//Draw candies on board

		}

	}
}
int PointsCheck(int CandyID) {
	switch (CandyID % 5)/*Returning score of candy based on its color*/ {
	case 0: {
		return 50;
		break;
	}
	case 1: {
		return  30;
		break;
	}
	case 2: {
		return 60;
		break;
	}
	case 3: {
		return 40;
		break;
	}
	case 4: {
		return 30;
		break; }
	}
}
