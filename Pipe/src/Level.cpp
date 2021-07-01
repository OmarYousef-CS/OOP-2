#include "Level.h"

//-----------------------------------------------
Level::Level() {
	m_height = 0;
	m_width = 0;
}

//-----------------------------------------------
Level::~Level() {
	deleteLevel();
}

//-----------------------------------------------
void Level::deleteLevel() {
	for (int row = 0; row < m_board.size(); row++) {
		for (int col = 0; col < m_board[row].size(); col++) {
			delete m_board[row][col];
		}
	}
}

//-----------------------------------------------
void Level::load_level(int level) {
	deleteLevel();
	m_board.clear();
	std::string str = "C:Level";
	str += std::to_string(level);
	str += ".txt";
	fs::path p = str;
	std::ifstream ifile(fs::absolute(p));
	readFromStream(ifile);
	ifile.close();
}

//-----------------------------------------------
//here the constructor that we use it if we have 
//a level file we open the file and do stream 
//and we read tha inputs from the file and print 
//them on the window the map to
void Level::readFromStream(std::ifstream& stream) {
	int deg;
	sf::Vector2i vec;
	stream >> m_height;
	stream >> m_width;
	//reading the map's chars line by line
	m_receiversPoints.clear();
	m_board.resize(m_height);
	for (int row = 0; row < m_width; row++) {
		//jumping over the new line char
		stream.get();
		for (int col = 0; col < m_width; col++) {
			sf::Vector2f points(col * TEXTURE_SIZE, row * TEXTURE_SIZE);
			vec.x = col;
			vec.y = row;
			switch (char_to_type(stream.get())) {
			case PipeType::Receiver:
				deg = int(stream.get() - '0');
				m_board[row].push_back(new ReceiverPipe(points,deg));
				m_receiversPoints.push_back(vec);
				break;
			case PipeType::Pumper:
				deg = int(stream.get()- '0');
				m_board[row].push_back(new PumperPipe(points,deg));
				m_pumperPoint.x = col;
				m_pumperPoint.y = row;
				break;
			case PipeType::TwoSides:	
				deg = int(stream.get() - '0');
				m_board[row].push_back(new TwoSidesPipe(points,deg));
				break;
			case PipeType::Corner:
				deg = int(stream.get() - '0');
				m_board[row].push_back(new CornerPipe(points,deg));
				break;
			case PipeType::ThreeSides:
				deg = int(stream.get() - '0');
				m_board[row].push_back(new ThreeSidesPipe(points,deg));
				break;
			}
		} 
	}
}

//-----------------------------------------------
void Level::draw(sf::RenderWindow& window) {
	for (int row = 0; row < m_height; row++) {
		for (int col = 0; col < m_width; col++) {
			m_board[row][col]->draw(window);
		}
	}
}

//-----------------------------------------------
int Level::getWidth() const {
	return m_width;
}

//-----------------------------------------------
int Level::getHeight() const {
	return m_height;
}

//-----------------------------------------------
Pipe *Level::getPipe(int row, int col) {
	if (row >= m_height || col >= m_width
		|| row < 0 || col < 0) {
		throw std::exception("Wrong index");
	}
	return (m_board[row][col]);
}

//-----------------------------------------------
bool Level::solved() {
	try {
		bool** arr = new bool* [m_height];
		for (int row = 0; row < m_height; row++) {
			arr[row] = new bool[m_width];
		}
		for (int i = 0; i < m_receiversPoints.size(); i++) {
			fillBoolArr(arr);
			if (!checkIfConnect(m_pumperPoint.y, m_pumperPoint.x, arr, i))
				return false;;
		}
		deleteArr(arr, m_height);
		return true;
	}
	catch (const std::exception& rt) {
		rt.what();
	}
}

//-----------------------------------------------
void Level::resetWaterRec() {
	for (int row = 0; row < m_board.size(); row++) {
		for (int col = 0; col < m_board[row].size(); col++) {
			m_board[row][col]->setWaterRec(false);
		}
	}
	m_board[m_pumperPoint.y][m_pumperPoint.x]->setWaterRec(true);
}

//-----------------------------------------------
void Level::fillBoolArr(bool** arr) {
	for (int row = 0; row < m_height; row++) {
		for (int col = 0; col < m_width; col++) {
			arr[row][col] = true;
		}
	}
}

//-----------------------------------------------
bool Level::pointsOfReceiver(int x, int y, int index) {
	return ((m_receiversPoints[index].x == x && m_receiversPoints[index].y == y));
}

//-----------------------------------------------
bool Level::checkIfConnect(int y, int x, bool** arr, int index) {
	if (!(arr[y][x])) { return false; }
	arr[y][x] = false;
	if (pointsOfReceiver(x, y, index)) { return true; }
	if (x > 0 && m_board[y][x]->canConnect(LEFT)
		&& m_board[y][x - 1]->canConnect(RIGHT) && arr[y][x - 1]) {
		m_board[y][x - 1]->setWaterRec(true);
		if (checkIfConnect(y, x - 1, arr, index)) { return true; }
	}
	if (x < m_width - 1 && m_board[y][x]->canConnect(RIGHT)
		&& m_board[y][x + 1]->canConnect(LEFT) && arr[y][x + 1]) {
		m_board[y][x + 1]->setWaterRec(true);
		if (checkIfConnect(y, x + 1, arr, index)) { return true; }
	}
	if (y > 0 && m_board[y][x]->canConnect(TOP)
		&& m_board[y - 1][x]->canConnect(BOTTOM) && arr[y - 1][x]) {
		m_board[y - 1][x]->setWaterRec(true);
		if (checkIfConnect(y - 1, x, arr, index)) { return true; }
	}
	if (y < m_height - 1 && m_board[y][x]->canConnect(BOTTOM)
		&& m_board[y + 1][x]->canConnect(TOP) && arr[y + 1][x]) {
		m_board[y + 1][x]->setWaterRec(true);
		if (checkIfConnect(y + 1, x, arr, index)) { return true; }
	}
	arr[y][x] = true;
	return false;
}

/*   DONT DELETE IT
//-----------------------------------------------
bool Level::checkIfConnect(int y, int x, bool** arr) {
	if (!(arr[y][x])) { return false; }
	arr[y][x] = false;
	if (y == m_pumperPoint.y && x == m_pumperPoint.x) {
		return true;
	}
	else if (x > 0 && m_board[y][x]->canConnect(LEFT)
			&& m_board[y][x - 1]->canConnect(RIGHT)) {
		m_board[y][x - 1]->setWaterRec(true);
		if (checkIfConnect(y, x - 1, arr)) { return true; }
	}
	else if (x < m_width - 1 && m_board[y][x]->canConnect(RIGHT)
		&& m_board[y][x + 1]->canConnect(LEFT)) {
		m_board[y][x + 1]->setWaterRec(true);
		if (checkIfConnect(y, x + 1, arr)) { return true; }
	}
	else if (y > 0 && m_board[y][x]->canConnect(TOP)
		&& m_board[y - 1][x]->canConnect(BOTTOM)) {
		m_board[y - 1][x]->setWaterRec(true);
		if (checkIfConnect(y - 1, x, arr)) { return true; }
	}
	else if (y < m_height - 1 && m_board[y][x]->canConnect(BOTTOM)
		&& m_board[y + 1][x]->canConnect(TOP)) {
		m_board[y + 1][x]->setWaterRec(true);
		if(checkIfConnect(y + 1, x, arr)){return true;}
	}
	return false;
}
*/

//-----------------------------------------------
void Level::deleteArr(bool** arr, int rowsNum) {
	for (int row = 0; row < rowsNum; row++) {
		delete arr[row];
	}
	delete arr;
}