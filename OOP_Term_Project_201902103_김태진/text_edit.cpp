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

//n에 따른 매개변수가 필요.
void showInitTextView(int n) {
	int size = savingSentences.size();
	for (int i = (n-1)*20+1; i < n*20 + 1; i++) {
		/* 이곳에서 만약 사이즈 보다 크다면 빈칸 출력 말고 앞에 출력하는 for문 넣기*/
		if (i < 10) {
			cout << " " << i << "| ";
		}
		else{
			cout << i << "| ";
		}
		
		if (i >= size) {
			cout << "\n";
		}
		if (i < size) {
			cout << savingSentences[i-1];
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
	//int count = 0;

	string str="";

	while (!fin.eof()) {
		string buf;
		fin >> buf;
		//int bufStringSize = buf.length();
		//count += bufStringSize;
		str += buf+" ";

		//if (count > 45) {
			//savingSentences.push_back(str);
			savingSentencesWithString += str;
			str = "";
		//	count = 0;
		//}
	} 

	// 마지막 str은 따로 push 해야 함.
	//savingSentences.push_back(str);

	fin.close();
}

void make_boundary() {
	cout << "-------------------------------------------------------------------------\n";
}

void make_bottom(string result) {
	make_boundary();
	cout << "n:다음페이지, p:이전페이지, i:삽입, d:삭제, c:변경, s:찾기, t:저장후종료\n";
	make_boundary();
	cout << "(콘솔메시지) "<< result <<"\n";
	make_boundary();
	cout << "입력: ";
}

/*
class Strategy {
public:
	virtual void doOperation() {

	}
};

class Insert:Strategy {
public:
	virtual void doOperation() {

	}
};
*/

bool isValid(string str) {
	int size = str.length();
	if (str.at(1) == '(' && str.at(size - 1) == ')') {
		return true;
	}
	return false;
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
	lastPage = (savingSentences.size() / 20)+1;
}

string vectorToString() {
	string returnStr;
	int size = savingSentences.size();
	for (int i = 0; i < size; i++) {
		returnStr += savingSentences[i];
	}
	return returnStr;
}

int main() {

	string instruction="";
	string result="";

	getInitText();

	while (1) {
		stringToVector();
		
		showInitTextView(page);
		make_bottom(result);
		result = "";
		cin >> instruction;
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
			else if (!instruction.compare("i") || !instruction.compare("d") || !instruction.compare("s") || !instruction.compare("c")) {
				result = "you are wrong. please enter completely (your instruction was => "+instruction+") recomended: "+instruction+"(valid parameters)";
				// recomended 띄우기
			}
			else {
				result = "("+instruction+")"+" is not valid instruction";
			}
		}
		else {
			if (instruction.at(0) == 'i') {
				if (isValid(instruction)) {
					string substr = instruction.substr(2, instruction.length()-3);
					vector<string> tokens;
					string buf;
					istringstream iss(substr);

					while (getline(iss,buf,',')) {
						tokens.push_back(buf);
					}

					int row = 0;
					stringstream ssInt(tokens[0]);
					ssInt >> row;

					int col = 0;
					stringstream ssInt2(tokens[1]);
					ssInt2 >> col;

					string InsertingStr;
					InsertingStr = tokens[2];

					string getstrFromSavingSentence = savingSentences[row-1];
	
					int countForInsert = 0;
					string newSentence;
					bool inserted = false;

					int getstrFromSavingSentenceLength = getstrFromSavingSentence.length();

					for (int i = 0; i < getstrFromSavingSentenceLength; i++) {
						if ((getstrFromSavingSentence[i] == ' ')&&!inserted)countForInsert++;
						if ((countForInsert == col) && !inserted) {
							if (col == 0) {
								newSentence += InsertingStr + " ";
								i--;
							}
							else
							{
								newSentence += " "+InsertingStr+" ";
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
			else if (instruction.at(0) == 'd') {
				if (isValid(instruction)) {
					string substr = instruction.substr(2, instruction.length() - 3);
					vector<string> tokens;
					string buf;
					istringstream iss(substr);

					while (getline(iss, buf, ',')) {
						tokens.push_back(buf);
					}

					int row = 0;
					stringstream ssInt(tokens[0]);
					ssInt >> row;

					int col = 0;
					stringstream ssInt2(tokens[1]);
					ssInt2 >> col;

					string sentenceWithDeleteWord = savingSentences[row - 1];
					string newSentence="";

					int sentenceWithDeleteWordLength = sentenceWithDeleteWord.length();

					int count = 0;
					for (int i = 0; i < sentenceWithDeleteWordLength; i++)
					{

						if (sentenceWithDeleteWord[i]==' ') {
							++count;
						}
						if (count == col - 1) {
							continue;
						}
						if (col == 1&&count==col&& sentenceWithDeleteWord[i] == ' ') {
							continue;
						}
						newSentence += sentenceWithDeleteWord[i];

					}
					savingSentences[row - 1] = newSentence;
					savingSentencesWithString = vectorToString();
				}
			}
			else if (instruction.at(0) == 's') {
				if (isValid(instruction)) {
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
							
							if ( deleteLocationX <= i && i <= substrLength + deleteLocationX) {
								continue;
							}
							newSentence += fixed[i];
						}
						savingSentences[deleteLocationY] = newSentence;
					}
					else {
						result += "not found";
					}
					
					savingSentencesWithString = vectorToString();
					savingSentencesWithString = substr + " " + savingSentencesWithString;
				}
			}
			else if (instruction.at(0) == 'c') {
				if (isValid(instruction)) {
					string substr = instruction.substr(2, instruction.length() - 3);
					vector<string> tokens;
					string buf;
					istringstream iss(substr);

					while (getline(iss, buf, ',')) {
						tokens.push_back(buf);
					}

					string originString = tokens[0];
					string replaceString = tokens[1];

					int savingSentencesSize = savingSentences.size();

					for (int i = 0; i < savingSentencesSize; i++) {
						string buf = savingSentences[i];
						int x = buf.find(originString);
						if (x != -1) {
							int size = originString.size();
							buf.replace(x,size,replaceString);
							savingSentences[i] = buf;
						}
					}

					savingSentencesWithString = vectorToString();
				}
			}

		}
		
		make_boundary();
	}
	
	
	return 0;
}
