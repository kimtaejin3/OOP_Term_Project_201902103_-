#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<sstream>

using namespace std;

vector<string> savingSentences;
string savingSentencesWithString;

int page = 1;
int lastPage;
bool isChanged = false;
string result = "";
string instruction = "";


//라인이 20개 이내일 때와 20개 이상일 때를 구분
void showInitTextView20line() {
	int size = savingSentences.size();
	for (int i = 1; i <= 20; i++) {
		if (i < 10) {
			cout << " " << i << "| ";
		}
		else {
			cout << i << "| ";
		}
		if (size > i) {
			cout << savingSentences[i - 1];
		}
		cout << "\n";
	}
}

void showInitTextView(int n) {

	int size = savingSentences.size();
	if (size < 20) {
		showInitTextView20line();
		return;
	}
	int vectorSizeOfFirstLine = (n - 1) * 20 + 1;
	int lackOfLine = -1;
	for (int i = (n - 1) * 20 + 1; i < n * 20 + 1; i++) {

		if (vectorSizeOfFirstLine + 20 > size) {

			lackOfLine = (20 + vectorSizeOfFirstLine) - size - 1;
			cout << i - lackOfLine << "| ";
		}
		else
		{
			if (i < 10) {
				cout << " " << i << "| ";
			}
			else {
				cout << i << "| ";
			}
		}

		if (vectorSizeOfFirstLine + 20 > size) {
			cout << savingSentences[i - 1 - lackOfLine];
			cout << "\n";
		}
		else if (i < size) {
			cout << savingSentences[i - 1];
			cout << "\n";
		}
	}
}

void saveAndExit() {
	ofstream fout("test.txt");
	string saveTextChange = "";
	int size = savingSentences.size();
	for (int i = 0; i < size; i++) {
		saveTextChange += savingSentences[i];
	}
	fout << saveTextChange;
	exit(0);
}

void getInitText() {
	ifstream fin("test.txt");

	if (fin.peek() == ifstream::traits_type::eof()) {
		cout << "디렉토리에서 test.txt를 찾지 못했습니다." << endl;
		exit(0);
	}

	string str = "";

	while (!fin.eof()) {
		string buf;
		fin >> buf;
		str += buf + " ";

		savingSentencesWithString += str;
		str = "";
	}

	fin.close();
}

void make_boundary() {
	cout << "-------------------------------------------------------------------------\n";
}

void make_bottom(string result) {
	make_boundary();
	cout << "n:다음페이지, p:이전페이지, i:삽입, d:삭제, c:변경, s:찾기, t:저장후종료\n";
	make_boundary();
	cout << "(콘솔메시지) " << result << "\n";
	make_boundary();
	cout << "입력: ";
}


void stringToVector() {
	string str;
	int count = 0;

	savingSentences.clear();
	int length = savingSentencesWithString.length();

	for (int i = 0; i < length; i++) {

		str += savingSentencesWithString[i];

		if (count > 50 && savingSentencesWithString[i] == ' ') {
			savingSentences.push_back(str);
			str = "";
			count = 0;
		}

		++count;
	}
	savingSentences.push_back(str);
	lastPage = (savingSentences.size() / 20) + 1;
}

string vectorToString() {
	string returnStr;
	int size = savingSentences.size();
	for (int i = 0; i < size; i++) {
		returnStr += savingSentences[i];
	}
	return returnStr;
}


class Strategy {
public:
	virtual void doOperation() = 0;
};



class ConfirmValidation {
public:
	bool isValid(string str) {
		int size = str.length();
		if (str.at(1) == '(' && str.at(size - 1) == ')') {
			return true;
		}
		result = "출력명세는 명령어(인자, ...) 입니다.";
		return false;
	}

	bool has_only_digits(const string s) {
		return s.find_first_not_of("0123456789") == string::npos;
	}

	bool isOver(int row, int col) {
		if (row == 0) {
			result = "0번째 라인은 존재하지 않습니다.";
			return false;
		}
		int size = savingSentences.size();
		if (page == lastPage) {
			if (row > size) {
				result = "현재 출력창에는 " + to_string(row) + "라인이 존재하지 않습니다.";
				return false;
			}
		}
		else if (row > page * 20) {
			result = "현재 출력창에는 " + to_string(row) + "라인이 존재하지 않습니다.";
			return false;
		}

		int lineLength = savingSentences[row - 1].length();
		int maxCol = 0;
		for (int i = 0; i < lineLength; i++) {
			if (savingSentences[row - 1][i] == ' ') {
				++maxCol;
			}
		}
		if (col > maxCol) {
			result = to_string(col) + "번째 단어가 존재하지 않습니다.";
			return false;
		}
		return true;
	}

	bool InsertInstructionIsWrong(vector<string> vec) {
		if (vec.size() < 3|| vec.size() > 3) {
			result = "인자 세 개를 제대로 입력하세요.";
			return false;
		}

		if (!has_only_digits(vec[0])) {
			result = "숫자(양수)만 입력해 주세요.";
			return false;
		}
		if (!has_only_digits(vec[1])) {
			result = "숫자(양수)만 입력해 주세요.";
			return false;
		}

		if (vec[2].length() > 75) {
			result = "당신이 입력하고 싶은 문자열의 길이가 너무 깁니다.";
			return false;
		}
		return true;
	}

	bool DeleteInstructionIsWrong(vector<string> vec) {
		if (vec.size() < 2 || vec.size()>2) {
			result = "인자 두 개를 제대로 입력하세요.";
			return false;
		}

		if (!has_only_digits(vec[0])) {
			result = "숫자(양수)만 입력해 주세요.";
			return false;
		}
		if (!has_only_digits(vec[1])) {
			result = "숫자(양수)만 입력해 주세요.";
			return false;
		}
		return true;
	}

	bool ChangeInstructionIsWrong(vector<string> vec) {
		if (vec.size() < 2 || vec.size() > 2) {
			result = "인자 두 개를 제대로 입력하세요.";
			return false;
		}
		
		return true;
	}
};

class Insert:public Strategy {
	ConfirmValidation confirmVal;
public:
	void doOperation() {
		if (confirmVal.isValid(instruction)) {
			string substr = instruction.substr(2, instruction.length() - 3);
			vector<string> tokens;
			string buf;
			istringstream iss(substr);

			while (getline(iss, buf, ',')) {
				tokens.push_back(buf);
			}

			if (confirmVal.InsertInstructionIsWrong(tokens)) {
				int row = 0;
				stringstream ssInt(tokens[0]);
				ssInt >> row;
				int col = 0;
				stringstream ssInt2(tokens[1]);
				ssInt2 >> col;
				if (confirmVal.isOver(row, col)) {

					string InsertingStr;
					InsertingStr = tokens[2];

					string getstrFromSavingSentence = savingSentences[row - 1];

					int countForInsert = 0;
					string newSentence;
					bool inserted = false;

					int getstrFromSavingSentenceLength = getstrFromSavingSentence.length();

					for (int i = 0; i < getstrFromSavingSentenceLength; i++) {
						if ((getstrFromSavingSentence[i] == ' ') && !inserted)countForInsert++;
						if ((countForInsert == col) && !inserted) {
							if (col == 0) {
								newSentence += InsertingStr + " ";
								i--;
							}
							else
							{
								newSentence += " " + InsertingStr + " ";
							}
							inserted = true;
							continue;
						}

						newSentence += getstrFromSavingSentence[i];

					}
					savingSentences[row - 1] = newSentence;
					savingSentencesWithString = vectorToString();
				}
			}
		}
	}
};

class Delete :public Strategy {
	ConfirmValidation confirmVal;
public:
	void doOperation() {

		if (confirmVal.isValid(instruction)) {
			string substr = instruction.substr(2, instruction.length() - 3);
			vector<string> tokens;
			string buf;
			istringstream iss(substr);

			while (getline(iss, buf, ',')) {
				tokens.push_back(buf);
			}

			if (confirmVal.DeleteInstructionIsWrong(tokens)) {

				int row = 0;
				stringstream ssInt(tokens[0]);
				ssInt >> row;

				int col = 0;
				stringstream ssInt2(tokens[1]);
				ssInt2 >> col;

				if (confirmVal.isOver(row, col)) {

					string sentenceWithDeleteWord = savingSentences[row - 1];
					string newSentence = "";

					int sentenceWithDeleteWordLength = sentenceWithDeleteWord.length();

					int count = 0;
					for (int i = 0; i < sentenceWithDeleteWordLength; i++)
					{

						if (sentenceWithDeleteWord[i] == ' ') {
							++count;
						}
						if (count == col - 1) {
							continue;
						}
						if (col == 1 && count == col && sentenceWithDeleteWord[i] == ' ') {
							continue;
						}
						newSentence += sentenceWithDeleteWord[i];

					}
					savingSentences[row - 1] = newSentence;
					savingSentencesWithString = vectorToString();
				}
			}

		}
	}
};

class Swap :public Strategy {
	ConfirmValidation confirmVal;
public:
	void doOperation() {
		if (confirmVal.isValid(instruction)) {
			string substr = instruction.substr(2, instruction.length() - 3);
			int deleteLocationX = -1;
			int deleteLocationY = -1;
			int savingSentencesSize = savingSentences.size();

			for (int i = 0; i < savingSentencesSize; i++) {
				string isContain = savingSentences[i];

				deleteLocationX = isContain.find(substr);
				if (deleteLocationX != -1) {
					deleteLocationY = i;
					break;
				}
			}

			if (deleteLocationX != -1 && deleteLocationY != -1) {
				string fixed = savingSentences[deleteLocationY];
				string newSentence;
				int substrLength = substr.length();
				int fixedSize = fixed.size();
				for (int i = 0; i < fixedSize; i++) {

					if (deleteLocationX <= i && i <= substrLength + deleteLocationX) {
						continue;
					}
					newSentence += fixed[i];
				}
				savingSentences[deleteLocationY] = newSentence;
				savingSentencesWithString = vectorToString();
				savingSentencesWithString = substr + " " + savingSentencesWithString;
			}
			else {
				result += "not found";
			}

		}
	}
};

class Change :public Strategy {
	ConfirmValidation confirmVal;
public:
	void doOperation() {
		if (confirmVal.isValid(instruction)) {
			string substr = instruction.substr(2, instruction.length() - 3);
			vector<string> tokens;
			string buf;
			istringstream iss(substr);

			while (getline(iss, buf, ',')) {
				tokens.push_back(buf);
			}

			if (confirmVal.ChangeInstructionIsWrong(tokens)) {
				string originString = tokens[0];
				string replaceString = tokens[1];

				int savingSentencesSize = savingSentences.size();

				for (int i = 0; i < savingSentencesSize; i++) {
					string buf = savingSentences[i];
					int x = buf.find(originString);
					if (x != -1) {
						int size = originString.size();
						buf.replace(x, size, replaceString);
						savingSentences[i] = buf;
					}
				}

				savingSentencesWithString = vectorToString();
			}

		}
	}
};

class Context {
	Strategy* strategy;
public:
	Context(Strategy* s) {
		strategy = s;
	}
	void executionStrategy() {
		strategy->doOperation();
	}
};



int main() {
	Context* context;
	getInitText();

	while (1) {
		stringToVector();

		showInitTextView(page);
		make_bottom(result);
		result = "";
		getline(cin, instruction);
		if (instruction.length() < 2) {
			if (!instruction.compare("n") || !instruction.compare("p") || !instruction.compare("t")) {
				if (instruction.at(0) == 'n') {
					page += 1;
					if (page > lastPage) {
						page -= 1;
						result = "This is the last page!";
					}
				}
				else if (instruction.at(0) == 'p') {
					page -= 1;
					if (page < 1) {
						page += 1;
						result = "This is the first page!";
					}
				}
				else if (instruction.at(0) == 't') {
					saveAndExit();
				}
			}
		}
		else {
			if (instruction.at(0) == 'i') {
				context = new Context(new Insert());
				context->executionStrategy();
			}
			else if (instruction.at(0) == 'd') {
				context = new Context(new Delete());
				context->executionStrategy();
			}
			else if (instruction.at(0) == 's') {
				context = new Context(new Swap());
				context->executionStrategy();
			}
			else if (instruction.at(0) == 'c') {
				context = new Context(new Change());
				context->executionStrategy();
			}
			else {
				result = "(" + instruction + ")" + " is not valid instruction";
			}

		}

		make_boundary();
	}


	return 0;
}
