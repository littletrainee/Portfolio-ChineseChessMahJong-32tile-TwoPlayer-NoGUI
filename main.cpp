#include <algorithm>
#include <ctime>
#include <iostream>
#include <iterator>
#include <map>
#include <string>
#include <tuple>
#include <vector>

using namespace std;

void AppendToWallHand(map< string, string> tiles,
		vector<string> &    wallhand) {
	int loopint;
	for (map<string, string> :: const_iterator tile = tiles.begin();
			tile != tiles.end(); tile++) {
		if (tile->first == "b1" || tile->first == "r1")
			loopint = 1;
		else if (tile->first == "b7" || tile->first == "r7")
			loopint = 5;
		else
			loopint = 2;
		for (int i = 0; i < loopint; ++i)
			wallhand.push_back(tile->first);
	}
}

void  PrintHand(string              targetname, 
		vector<string>      targethand,
		map<string, string> tile, 
		bool                shownumber) {
	cout << targetname << ':';
	if (shownumber) 
		for (int i = 0; i < targethand.size(); i++)
			cout << i + 1 << "." << tile[targethand[i]] << "  ";
	else {
		for (vector<string> :: iterator target = targethand.begin();
				target != targethand.end(); target++)
			cout << tile[*target] << ' ';
	}
	cout << endl << endl;
}

void  PrintMeldHand(string                 targetname, 
										vector<vector<string>> targetmeldhand,
										map<string, string>    tile) {
	vector<string> targetmelds;
	cout << targetname << ':';
	for (vector<vector<string>> :: iterator targetmeld = targetmeldhand.begin();
			targetmeld != targetmeldhand.end(); targetmeld++) {
		cout << "{ ";
		targetmelds = *targetmeld;
		for (vector<string> :: iterator target = targetmelds.begin();
				target != targetmelds.end(); target++) {
			cout << tile[*target] << ' ';
		}
		cout << '}';
	}
	cout << endl << endl;
}

void PrintAllHand(string                 wallname, 
									vector<string>         wallhand,
									string                 player1handname,
									vector<string>         player1hand,
									string                 player1meldhandname,
									vector<vector<string>> player1meldhand,
									string         				 rivername,
									vector<string> 				 riverhand,
									string         				 player2meldhandname,
									vector<vector<string>> player2meldhand,
									string 								 player2handname,
									vector<string> 				 player2hand,
									map<string,string>     tile){
	PrintHand(wallname, wallhand, tile, false);
	PrintHand(player1handname, player1hand, tile, false);
	PrintMeldHand(player1meldhandname, player1meldhand, tile);
	PrintHand(rivername, riverhand, tile, false);
	PrintMeldHand(player2meldhandname, player2meldhand, tile);
	PrintHand(player2handname, player2hand, tile, false);
}

void DrawCard(vector<string> & wallhand,
		vector<string> & playerhand) {
	playerhand.push_back(wallhand.front());
	vector<string> :: iterator it = wallhand.begin();
	wallhand.erase(it);
}

void Sort(map<string, string> tiles, 
		vector<string> &    playerhand) {
	vector<string> :: iterator it;
	vector<string>           temphand;
	int                      i;
	for (map<string, string> :: const_iterator tile = tiles.begin();
			tile != tiles.end(); tile++) {
		it = find(playerhand.begin(), playerhand.end(), tile->first);
		if (it != playerhand.end()) {
			i = count(playerhand.begin(), playerhand.end(), *it);
			for (int j = 0; j < i; j++)
				temphand.push_back(*it);
		}
	}
	playerhand = temphand;
}

vector<string> ProbablyEyes(vector<string>      playerhand,
		map<string, string> tiles) {
	int                      i;
	vector<string>           temphand;
	vector<string> :: iterator it;
	for (map<string, string> :: const_iterator tile = tiles.begin();
			tile != tiles.end(); tile++) {
		it = find(playerhand.begin(), playerhand.end(), tile->first);
		if (it != playerhand.end()) {
			i = count(playerhand.begin(), playerhand.end(), *it);
			if (i >= 2)
				temphand.push_back(*it);
		}
	}
	if (find(playerhand.begin(), playerhand.end(), "b1") != playerhand.end() 
			&& find(playerhand.begin(), playerhand.end(), "r1") != playerhand.end()) {
		it = find(playerhand.begin(), playerhand.end(), "b1"); 
		temphand.push_back(*it);
		it = find(playerhand.begin(), playerhand.end(), "r1"); 
		temphand.push_back(*it);
	}
	return temphand;
}

bool CheckMeld(vector<string> targethand) {
	bool returnbool = false;
	if (targethand[0][0] == targethand[1][0] &&
			targethand[1][0] == targethand[2][0]) // check kind
		// check sequence or triple
		if ((int)targethand[0][1] + 1 - 48 == (int)targethand[1][1] - 48 &&
				(int)targethand[1][1] + 1 - 48 == (int)targethand[2][1] - 48 ||
				targethand[0] == targethand[1] && targethand[1] == targethand[2])
			returnbool = !returnbool;
	return returnbool;
}

bool CheckToWin(vector<string>         playerhand, 
		map<string, string>    tiles,
		vector<vector<string>> playermeldhand,
		vector<string>         riverhand) {
	vector<string>           temphand;
	vector<string>           eyes;
	vector<string> :: iterator it;
	bool                     returnbool = true;
	if (!playermeldhand.empty())
		for (vector<vector<string>> :: iterator i = playermeldhand.begin();
				i != playermeldhand.end(); i++) {
			temphand = *i;
			playerhand.insert(playerhand.end(), temphand.begin(), temphand.end());
		}
	if (playerhand.size() != 5)
		playerhand.push_back(riverhand.back());
	Sort(tiles, playerhand);
	temphand = playerhand;
	eyes = ProbablyEyes(playerhand, tiles);
	if (!eyes.empty()) {
		if (find(playerhand.begin(), playerhand.end(), "r1") != playerhand.end() &&
				find(playerhand.begin(), playerhand.end(), "b1") != playerhand.end()) {
			it = find(playerhand.begin(), playerhand.end(), "b1");
			playerhand.erase(it);
			it = find(playerhand.begin(), playerhand.end(), "r1");
			playerhand.erase(it);
			returnbool = CheckMeld(playerhand) ? false : true;
		} else {
			for (vector<string> :: iterator eye = eyes.begin(); eye != eyes.end();
					eye++) {
				playerhand = temphand;
				for (int j = 0; j < 2; j++) {
					it = find(playerhand.begin(), playerhand.end(), *eye);
					playerhand.erase(it);
				}
				returnbool = CheckMeld(playerhand) ? false : true;
				if (!returnbool)
					break;
			}
		}
	} else
		returnbool = true; // if true is gameon(go to loop)
	return returnbool;
}

void  DisCard(vector<string> &playerhand,
		vector<string> &riverhand){
	char                     keyin;
	int                      target;
	vector<string> :: iterator it;
	cout << "請選擇您要打出牌的編號(1-" <<playerhand.size() <<"):";      
	cin >> keyin;
	while (isdigit(keyin) == false && (int)keyin != 1 && (int)keyin != 2 &&
			(int)keyin != 3 && (int)keyin != 4 && (int)keyin != 5) {
		cout << "請重新輸入:";
		cin >> keyin;
	}
	target = (int)keyin - 48; // set select number
	it = playerhand.begin() + target - 1;
	riverhand.push_back(playerhand[target - 1]);
	playerhand.erase(it);
}

void AutoDisCard(vector<string> & playerhand,
		vector<string> & riverhand){
	riverhand.push_back(playerhand.back());
	playerhand.pop_back();	
}

int  MeldSelect(vector<vector<string>> checktomeld, 
		map<string, string>    tile) {
	pair<int, int> range;
	char           key;
	vector<string> temphand;
	int            returnchoose = 0;
	if (!checktomeld.empty()) { // isn't empty
MeldChoice:
		cout << "要做成組合嗎(y/n)?";
		cin >> key;
		while (key != 'y' && key != 'n') {
			cout << "請重新輸入選擇:";
			cin >> key;
			cout << endl;
		}
		if (key == 'y') {
			cout << "請選擇要做成組合的搭子或返回(r)組合選擇: "
				<< endl;
			for (vector<vector<string>> :: iterator i = checktomeld.begin();
					i != checktomeld.end(); i++) {
				cout << i - checktomeld.begin() + 1 << '.';
				temphand = *i;
				for (vector<string> :: iterator j = temphand.begin(); j != temphand.end();
						j++)
					cout << tile[*j] << ' ';
				cout << ' ';
			}
			cout << endl << "選擇是:";
			cin >> key;
			if (key == 'r')
				goto MeldChoice;
			switch (checktomeld.size()) {
				case 1:
					range = {49, 49};
					break;
				case 2:
					range = {49, 50};
					break;
				case 3:
					range = {49, 51};
					break;
			}
			while ((int)key < range.first || (int)key > range.second) {
				cout << "Please Renter Answer:";
				cin >> key;
			}
			returnchoose = (int)key - 48;
		} else if (key == 'n')
			returnchoose = 0;
	}
	return returnchoose;
}

vector<vector<string>>  CheckToMeld(vector<string> playerhand,
		vector<string> riverhand) {
	string                   comparetarget = riverhand.back();
	char                     kind = comparetarget[0];
	vector<string>           tempmeldhand;
	vector<vector<string>>   returnvector;
	vector<string> :: iterator comparetarget1;
	vector<string> :: iterator comparetarget2;
	vector<string> :: iterator comparetarget3;
	vector<string> :: iterator comparetarget4;
	comparetarget1 = find(playerhand.begin(), playerhand.end(),
			kind + to_string((int)comparetarget[1] - 48 - 2));
	comparetarget2 = find(playerhand.begin(), playerhand.end(),
			kind + to_string((int)comparetarget[1] - 48 - 1));
	comparetarget3 = find(playerhand.begin(), playerhand.end(),
			kind + to_string((int)comparetarget[1] - 48 + 1));
	comparetarget4 = find(playerhand.begin(), playerhand.end(),
			kind + to_string((int)comparetarget[1] - 48 + 2));
	if (comparetarget1 != playerhand.end() &&
			comparetarget2 != playerhand.end()) { //-2-1
		tempmeldhand.clear();
		tempmeldhand.push_back(*comparetarget1);
		tempmeldhand.push_back(*comparetarget2);
		returnvector.push_back(tempmeldhand);
	}
	if (comparetarget2 != playerhand.end() &&
			comparetarget3 != playerhand.end()) { //-1+1
		tempmeldhand.clear();
		tempmeldhand.push_back(*comparetarget2);
		tempmeldhand.push_back(*comparetarget3);
		returnvector.push_back(tempmeldhand);
	}
	if (comparetarget3 != playerhand.end() &&
			comparetarget4 != playerhand.end()) { //+1+2
		tempmeldhand.clear();
		tempmeldhand.push_back(*comparetarget3);
		tempmeldhand.push_back(*comparetarget4);
		returnvector.push_back(tempmeldhand);
	}
	return returnvector;
}

void MakeMeldFromPlayerHandToMeldHand(vector<string> &      		playerhand,
		vector<vector<string>> &  playermeldhand,
		vector<string> &          riverhand,
		vector<vector<string>>    checktomeld,
		int                       removeby) {
	vector<string>           temphand = checktomeld[removeby - 1];
	vector<string>           appendtomeldhand;
	vector<string> :: iterator removetarget;
	// temphand[0] to meldhand[1]
	appendtomeldhand.push_back(temphand[0]); // append temphand[0] to meldhand[1]
	removetarget =
		find(playerhand.begin(), playerhand.end(),
				temphand[0]); // find temphand[0] value's position in playerhand
	playerhand.erase(removetarget); // remove value from playerhand
	// riverhand last to meldhand[1]
	appendtomeldhand.push_back(
			riverhand.back()); // append riverhand last element to meldhand[1]
	riverhand.pop_back();  // remove riverhand's last
	// temphand[1]] to meldhand[1]
	appendtomeldhand.push_back(temphand[1]); // append temphand[1] to meldhand[1]
	removetarget =
		find(playerhand.begin(), playerhand.end(),
				temphand[1]); // find temphand[1] value's postition in playerhand
	playerhand.erase(removetarget); // remove value from playerhand
	// meldhand[1] to playermeldhand
	playermeldhand.push_back(appendtomeldhand);
}

void MakeCalculateScoreHand(vector<string> &       calculatehand,
		vector<string>         playerhand,
		vector<vector<string>> playermeldhand,
		string                 riverhandlastone) {
	vector<string> temphand;
	calculatehand =	playerhand;
	if (!playermeldhand.empty()) {
		for (vector<vector<string>> :: iterator i = playermeldhand.begin();
				i != playermeldhand.end(); i++) {
			temphand = *i;
			calculatehand.insert(calculatehand.end(), temphand.begin(),
					temphand.end());
		}
	}
	if (calculatehand.size() != 5)
		calculatehand.push_back(riverhandlastone);
}

vector<string>  TenPaiCheck(vector<string>         playerhand, 
		map<string,string>     tiles, 
		vector<vector<string>> playermeldhand){
	vector<string> 					 probablywintile;
	vector<string>           checkhand;
	vector<string>           temphand4tile;
	vector<string>           temphand5tile;
	vector<string>           eyes;
	vector<string> :: iterator it;
	// set the original hand
	if (!playermeldhand.empty())
		for(vector<vector<string>> :: iterator meld = playermeldhand.begin(); meld !=playermeldhand.end(); meld++){
			temphand4tile = *meld;
			playerhand.insert(playerhand.end(), temphand4tile.begin(), temphand4tile.end());
		}
	for(int i = 0;i<5;i++){ // pop one by one from checkhand and append one by one from tile to check win
		checkhand = playerhand; // from playerhand[5] to checkhand[5]
		checkhand.erase(checkhand.begin()+i); // delete from checkhand[5] to checkhand[4]
		temphand4tile = checkhand; // copy checkhand[4] to temphand[4]
		for (map<string,string> :: iterator tile = tiles.begin(); tile != tiles.end(); tile++){
			checkhand = temphand4tile; // reset checkhand[4] from temphand[4] 
			checkhand.push_back(tile -> first); // append tile from checkhand[4] to checkhnad[5]
			Sort(tiles, checkhand); // sort checkhand[5]
			temphand5tile = checkhand; // copy to temphand5tile from checkhand[5]
			eyes = ProbablyEyes(checkhand,tiles); // check the eye between checkhand[5] by tiles
			if(!eyes.empty()){ // check eyes is not empty
				if (find(checkhand.begin(), checkhand.end(), "r1") != checkhand.end() &&
						find(checkhand.begin(), checkhand.end(), "b1") != checkhand.end()) { // if the pair = b1,r1
					it = find(checkhand.begin(), checkhand.end(), "b1"); // find b1 in checkmeld[5] position
					checkhand.erase(it); // delete b1 from checkmeld[5]
					it = find(checkhand.begin(), checkhand.end(), "r1"); // find r1 in checkmeld[5] position
					checkhand.erase(it); // delete r1 from checkmeld[5]
					if (CheckMeld(checkhand)) // check the (checkmeld[5] without b1 and r1) is meld
						probablywintile.push_back(tile -> first); // if it is meld then append to porpablywintile
				} else { // pair = two the same
					for (vector<string> :: iterator eye = eyes.begin(); eye != eyes.end(); eye++) {
						checkhand = temphand5tile; // reset checkhand to checkhand[5]
						for (int j = 0; j < 2; j++) { // delete pair
							it = find(checkhand.begin(), checkhand.end(), *eye);
							checkhand.erase(it);
						}
						if (CheckMeld(checkhand)) // check the meld
							probablywintile.push_back(tile -> first);
					}
				}
			}
		} 
	}
	return probablywintile;
}

void DeclareTenPai(vector<string> probablywintile ,
		bool &         tenpai){
	char key;
	if (!probablywintile.empty()){
		cout <<  "要宣告聽牌嗎(y/n)? ";
		cin >> key;
		cout << endl;
		while(key != 'y' && key != 'n')
			cout  <<  "請重新輸入選擇: ";
		tenpai = key=='y' ? true : false;
	}
}

class TileType {
	private:
		vector<string> calculatehand;
		int            gameturn;
		int 					 gameround;
		bool 					 lastwalltile;
		bool 					 tsumo;
		bool 					 tenpai;
		int 					 totaltai = 0;
		// 6 tai
		void TenHou();
		// 5 tai
		void FiveSoldier();
		// 2 tai
		void DifferentGeneralBeenPair(); 
		// 2 or 1 tai
		void TsumoOrRon();
		// 1 tai 
		void SameKind();
		void WinOnLastTile();
		void TenPai();
	public:
		void SetValue(vector<string> calculatehand, int gameturn, int gamround, bool lastwalltile, bool tsumo, bool tenpai);
		void PrintTileType();
};

void TileType :: TenHou(){
	if (this -> gameround == 1 && this -> gameturn == 1){
		this -> totaltai += 6;
		cout  <<  "天胡        6台" << endl;
	}
}

void TileType :: FiveSoldier(){
	if (count(this -> calculatehand.begin(), this -> calculatehand.end(), "b7") == 5){
		this -> totaltai += 5;
		cout << "五卒連橫    5台"  <<  endl;
	}
	else if(count(this -> calculatehand.begin(), this -> calculatehand.end(),"r7") == 5){
		this -> totaltai  += 5;	
		cout << "五兵合縱    5台" <<  endl;
	}
}

void TileType :: DifferentGeneralBeenPair(){
	if(find(this -> calculatehand.begin(), this->calculatehand.end(), "b1") != this -> calculatehand.end() 
			&& find(this -> calculatehand.begin(), this->calculatehand.end(), "r1") != this -> calculatehand.end()){
		this -> totaltai += 2;
		cout << "將帥聽令    2台" <<  endl;
	}
} 

void TileType :: TsumoOrRon(){
	if(this -> tsumo){
		this -> totaltai +=  2;
		cout << "自摸        2台" <<  endl;
	}
	else{
		this -> totaltai += 1;
		cout << "胡牌        1台" <<  endl;
	}
} 

void TileType :: SameKind(){
	if(this -> calculatehand[0][0] == this -> calculatehand[1][0] 
			&& this -> calculatehand[1][0] == this -> calculatehand[2][0]
			&& this -> calculatehand[2][0] == this -> calculatehand[3][0]
			&& this -> calculatehand[3][0] == this -> calculatehand[4][0]){
		this -> totaltai += 1;
		cout << "清一色      1台" <<  endl;
	}
}

void TileType :: WinOnLastTile(){ 
	if (this -> lastwalltile){
		this -> totaltai += 1;
		cout << "海底撈月    1台" <<  endl;
	}
}

void TileType :: TenPai(){
	if (this -> tenpai){
		this -> totaltai += 1;
		cout << "聽牌        1台" <<  endl;
	}
}

void TileType :: SetValue(vector<string> calculatehand, int gameturn, int gameround, bool lastwalltile, bool tsumo, bool tenpai){
	this -> calculatehand = calculatehand;
	this -> gameturn = gameturn;
	this -> gameround = gameround;
	this -> lastwalltile = lastwalltile;
	this -> tsumo = tsumo;
	this -> tenpai = tenpai;
} 

void TileType :: PrintTileType(){
	TenHou();
	TenPai();
	WinOnLastTile();
	TsumoOrRon();
	SameKind();
	DifferentGeneralBeenPair();
	FiveSoldier();
	cout <<  "===============" << endl;  
	cout <<  "共：        " <<  this -> totaltai <<  "台" <<  endl;
} 

int main() {
	// set random seed
	srand(unsigned(time(0)));
	// declare bool
	bool gameswitch = true;
	bool tsumo = false;
	bool lastwalltile = false;
	bool player1tenpai = false;
	bool player2tenpai = false;
	// declare int
	int gameturn = 0; // #0 = player1, #1 = player2
	int gameround = 1;
	int meldselect;
	// declare string
	string player1handname = "玩家1手牌";
	string player2handname = "玩家2手牌";
	string player1meldhandname = "玩家1組合的手牌";
	string player2meldhandname = "玩家2組合的手牌";
	string wallname = "牌山";
	string rivername = "牌河";
	// declare map
	map<string, string> tile = {{"b1", "黑將"}, {"b2", "黑士"}, {"b3", "黑象"},
		{"b4", "黑車"}, {"b5", "黑馬"}, {"b6", "黑包"},
		{"b7", "黑卒"}, {"r1", "紅帥"}, {"r2", "紅仕"},
		{"r3", "紅相"}, {"r4", "紅俥"}, {"r5", "紅傌"},
		{"r6", "紅炮"}, {"r7", "紅兵"}};
	// declare vector<string>
	vector<string> wallhand;
	vector<string> player1hand;
	vector<string> player2hand;
	vector<string> riverhand;
	vector<string> temphand;
	vector<string> calculatehand;
	vector<string> player1probablywintile;
	vector<string> player2probablywintile;
	// declare vector<vector<string>>
	vector<vector<string>> player1meldhand;
	vector<vector<string>> player2meldhand;
	vector<vector<string>> checktomeld;
	// declare class
	TileType tt;
	// append tile to wallhand and shuffle
	AppendToWallHand(tile, wallhand);
	random_shuffle(wallhand.begin(), wallhand.end());
	// playerdrawcard 2 tile 2 time (4tile)
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++)
			DrawCard(wallhand, player1hand);
		for (int j = 0; j < 2; j++)
			DrawCard(wallhand, player2hand);
	}
	// player1 draw the 5th card
	DrawCard(wallhand, player1hand);
	// sort all playerhand
	Sort(tile, player1hand);
	Sort(tile, player2hand);
	// print round
	cout << "第 "  << gameround << " 巡" << endl;     
	// print all hand
	PrintAllHand(wallname, wallhand, 
							 player1handname, player1hand,
							 player1meldhandname, player1meldhand,
							 rivername, riverhand,
							 player2meldhandname, player2meldhand,
							 player2handname, player2hand,
							 tile);
	gameswitch = CheckToWin(player1hand, tile, player1meldhand, riverhand);
	// if player1 isn't winning
	while (gameswitch) { // win by tsu-mo
		// change to next player
		gameturn != 1 ? gameturn += 1 : gameturn -= 1;
		lastwalltile = wallhand.size() <= 1?  true : false;
		// player1's round
		if (gameturn == 0) {
			if (player2tenpai)
				AutoDisCard(player2hand,riverhand);
			else{
				PrintHand(player2handname, player2hand, tile, true);
				player2probablywintile = TenPaiCheck(player2hand,tile,player2meldhand);
				DeclareTenPai(player2probablywintile,player2tenpai);
				DisCard(player2hand, riverhand);
			}
			// print all hand
			Sort(tile, player2hand);
			PrintAllHand(wallname, wallhand, 
									 player1handname, player1hand,
									 player1meldhandname, player1meldhand,
									 rivername, riverhand,
									 player2meldhandname, player2meldhand,
									 player2handname, player2hand,
									 tile);
			// chcek player1 is winning(ron)
			gameswitch = CheckToWin(player1hand, tile, player1meldhand, riverhand);
			// if player2 isn't winning
			if (gameswitch) {
				if(!player1tenpai){
					// check the riverhand's last card will be the meld with player1hand's card
					checktomeld = CheckToMeld(player1hand, riverhand);
					meldselect = MeldSelect(checktomeld, tile);
					if (meldselect != 0)  // if player1 have meld
						MakeMeldFromPlayerHandToMeldHand(player1hand, player1meldhand, 
								riverhand, checktomeld, meldselect);
					else
						DrawCard(wallhand, player1hand);
				}else
					DrawCard(wallhand, player1hand);
			}
			gameround += 1;
			cout << "第 "  << gameround << " 巡" << endl;     
			PrintAllHand(wallname, wallhand, 
									 player1handname, player1hand,
									 player1meldhandname, player1meldhand,
									 rivername, riverhand,
									 player2meldhandname, player2meldhand,
									 player2handname, player2hand,
									 tile);
			gameswitch =
				CheckToWin(player1hand, tile, player1meldhand, riverhand);
			tsumo = gameswitch? false:true;
		}
		else if (gameturn == 1) { // player2's round
			if (player1tenpai)
				AutoDisCard(player1hand, riverhand); 
			else{
				PrintHand(player1handname, player1hand, tile, true);
				player1probablywintile = TenPaiCheck(player1hand,tile,player1meldhand); 
				DeclareTenPai(player1probablywintile,player1tenpai);	
				DisCard(player1hand, riverhand);
			}
			// print all hand
			Sort(tile, player1hand);
			PrintAllHand(wallname, wallhand, 
									 player1handname, player1hand,
									 player1meldhandname, player1meldhand,
									 rivername, riverhand,
									 player2meldhandname, player2meldhand,
									 player2handname, player2hand,
									 tile);
			// check player2 is winning(ron)
			gameswitch = CheckToWin(player2hand, tile, player2meldhand,
				riverhand); // player2 win by ron
			// if player2 isn't winning
			if (gameswitch) {
				if(!player2tenpai){ // check the riverhand's last card will be the meld with player2hand's card
					checktomeld = CheckToMeld(player2hand, riverhand);
					meldselect = MeldSelect(checktomeld, tile);
					if (meldselect != 0)  // if player2 have meld
						MakeMeldFromPlayerHandToMeldHand(player2hand, player2meldhand,
							riverhand, checktomeld,	meldselect);
					else
						DrawCard(wallhand, player2hand);
				}else
					DrawCard(wallhand, player2hand);
				PrintAllHand(wallname, wallhand, 
										 player1handname, player1hand,
										 player1meldhandname, player1meldhand,
										 rivername, riverhand,
										 player2meldhandname, player2meldhand,
										 player2handname, player2hand,
										 tile);
				gameswitch =
					CheckToWin(player2hand, tile, player2meldhand, riverhand);
				tsumo = gameswitch? false:true;
			}
		}
	}
	if (gameturn == 0) {
		MakeCalculateScoreHand(calculatehand, player1hand, player1meldhand,
			riverhand.back());
		cout <<  "玩家1獲勝"  <<  endl;
		tt.SetValue(calculatehand, gameturn, gameround, lastwalltile, tsumo, player1tenpai);
	} else if (gameturn == 1) {
		MakeCalculateScoreHand(calculatehand, player2hand, player2meldhand,
			riverhand.back());
		cout <<  "玩家2獲勝" <<  endl;
		tt.SetValue(calculatehand, gameturn, gameround, lastwalltile, tsumo, player2tenpai);
	}
	Sort(tile, calculatehand);
	PrintHand("獲勝的手牌是", calculatehand, tile, false);
	tt.PrintTileType();
	return 0;
}
