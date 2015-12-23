#include <iostream>
#include <vector>
#include <string>
using namespace std;

char acc_chars[] = "<>.,+-[]/\\!";
vector<unsigned char> program;
vector<unsigned long long> stack;

void fill_ull(unsigned long long i, unsigned long long v) {
	for (int j = 0; j < sizeof(unsigned long long); ++j) {
		program[i + j] = (unsigned char)(v & 0xff);
		v >>= 8;
	}
}

int main() {
	char c;
	while (cin >> c) {
		for (int i = 0; i < sizeof(acc_chars) / sizeof(char); ++i)
			if (c == acc_chars[i])
				program.push_back(c);
		if (c == '[') {
			stack.push_back(program.size() - 1);
			for (int i = 0; i < sizeof(unsigned long long) / sizeof(char); ++i)
				program.push_back(0);
		}
		else if (c == ']') {
			if (!stack.size()) {
				cerr << "ubfc: Unpaired ']' symbol found" << endl;
				return 1;
			}
			unsigned long long v = *stack.rbegin();
			unsigned long long u = program.size() - 1;
			stack.pop_back();
			for (int i = 0; i < sizeof(unsigned long long) / sizeof(char); ++i)
				program.push_back(0);
			fill_ull(v + 1, u);
			fill_ull(u + 1, v);
		}
	}
	if (stack.size()) {
		cerr << "ubfc: end-of-file found, but ']' expected" << endl;
		return 1;
	}
	cout << "unsigned char program[] = {";
	for (int i = 0; i < program.size(); ++i)
	{
		if (i) cout << ", ";
		cout << (unsigned int)program[i];
	}
	cout << "};" << endl;
	return 0;
}
