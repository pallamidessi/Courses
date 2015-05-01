#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <vector>
#include "fstream"
#include "iostream"
#include <algorithm>
#include <cctype>
#include <string>
#include <algorithm>

std::map<int, std::vector<int>> willingMap;
std::map<int, std::vector<int>> unwillingMap;

inline std::string trim(const std::string &s)
{
	auto wsfront = std::find_if_not(s.begin(), s.end(), [](int c){return std::isspace(c); });
	auto wsback = std::find_if_not(s.rbegin(), s.rend(), [](int c){return std::isspace(c); }).base();
	return (wsback <= wsfront ? std::string() : std::string(wsfront, wsback));
}


std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(trim(item));
	}
	return elems;
}


std::vector<std::string> split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, elems);
	return elems;
}

bool operator==(Paper const& l, Paper const& r)
{
	return l.idPaper == r.idPaper;
}


void parsePapers(string filename)
{
	ifstream papersFile;
	papersFile.open(filename, ios::in);

	if (papersFile.is_open())
	{
		string line;
		while (getline(papersFile, line))
		{
			std::vector<std::string> lineSplitted = split(line, '|');
			std::vector<std::string> keySetPapers = split(lineSplitted[5], ',');

			int id = 0;

			istringstream(lineSplitted[1]) >> id;

			Paper *p = new Paper(id, lineSplitted[2], lineSplitted[3], lineSplitted[4], keySetPapers);

			auto i = std::find_if(vectorPaper.begin(), vectorPaper.end(), [id](Paper const* n) { return n->idPaper == id; });
			if (i != vectorPaper.end())
			{
				(*i)->authorName += ", " + lineSplitted[3];
			}
			else
			{
				vectorPaper.push_back(p);
			}
		}
	}

	papersFile.close();
}

void parseReviewer(string filename)
{
	ifstream reviewerFile;
	reviewerFile.open(filename, ios::in);

	if (reviewerFile.is_open())
	{
		string line;
		while (getline(reviewerFile, line))
		{
			std::vector<std::string> lineSplitted = split(line, '|');
			std::vector<std::string> keySetPapers = split(lineSplitted[4], ',');

			int id = 0;

			istringstream(lineSplitted[1]) >> id;

			ReviewerInfo *r = new ReviewerInfo(id, lineSplitted[2], lineSplitted[3], keySetPapers, willingMap[id], unwillingMap[id]);

			vectorReviewer.push_back(r);
		}
	}

	reviewerFile.close();
}

void parseWilling(string filename)
{
	ifstream willingFile;
	willingFile.open(filename, ios::in);

	if (willingFile.is_open())
	{
		string line;
		while (getline(willingFile, line))
		{
			std::vector<std::string> lineSplitted = split(line, '|');
			std::vector<std::string> keySetPapers = split(lineSplitted[2], ',');
			std::vector<int> papersWill;

			int id = 0;

			istringstream(lineSplitted[1]) >> id;

			std::transform(keySetPapers.begin(), keySetPapers.end(), std::back_inserter(papersWill),
				[](const std::string& str) { return std::stoi(str); });

			willingMap[id] = papersWill;
		}
	}

	willingFile.close();
}

void parseUnwilling(string filename)
{
	ifstream unwillingFile;
	unwillingFile.open(filename, ios::in);

	if (unwillingFile.is_open())
	{
		string line;
		while (getline(unwillingFile, line))
		{
			std::vector<std::string> lineSplitted = split(line, '|');
			std::vector<std::string> keySetPapers = split(lineSplitted[2], ',');
			std::vector<int> papersUnwill;

			int id = 0;

			istringstream(lineSplitted[1]) >> id;

			std::transform(keySetPapers.begin(), keySetPapers.end(), std::back_inserter(papersUnwill),
				[](const std::string& str) { return std::stoi(str); });

			unwillingMap[id] = papersUnwill;
		}
	}

	unwillingFile.close();
}
