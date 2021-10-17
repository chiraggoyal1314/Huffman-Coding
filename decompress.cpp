#include <fstream>
#include <iostream>
#include <bitset>
#include <unordered_map>
#include <map>
#include <queue>
#include <vector>
using namespace std;
typedef unsigned char BYTE;

unordered_map< string, char> res;

struct MinHeapNode {
    char data;
    unsigned freq;
    MinHeapNode *left, *right;
 
    MinHeapNode(char data, unsigned freq)
 
    {
 
        left = right = NULL;
        this->data = data;
        this->freq = freq;
    }
};

struct compare {
 
    bool operator()(MinHeapNode* l, MinHeapNode* r)
 
    {
        return (l->freq > r->freq);
    }
};

void mapOfCodes(struct MinHeapNode* root, string str)
{
 
    if (!root)
        return;
 
    if (root->data != '\0'){
        // cout << root->data << ": " << str << "\n";
        res.insert(make_pair(str,root->data));
    }
 
    mapOfCodes(root->left, str + "0");
    mapOfCodes(root->right, str + "1");
}
 

void HuffmanCodes(vector<char> &data, vector<int> &freq, int size)
{
    struct MinHeapNode *left, *right, *top;

    priority_queue<MinHeapNode*, vector<MinHeapNode*>, compare> minHeap;
 
    for (int i = 0; i < size; ++i)
        minHeap.push(new MinHeapNode(data[i], freq[i]));
    while (minHeap.size() != 1) {
        left = minHeap.top();
        minHeap.pop();
 
        right = minHeap.top();
        minHeap.pop();

        top = new MinHeapNode('\0', left->freq + right->freq);
 
        top->left = left;
        top->right = right;
 
        minHeap.push(top);
    }

    mapOfCodes(minHeap.top(), "");
}

vector<BYTE> readFile(const char* filename)
{
    // open the file:
    streampos fileSize;
    ifstream file(filename,ios::binary);

    // get its size:
    file.seekg(0, ios::end);
    fileSize = file.tellg();
    file.seekg(0, ios::beg);

    // read the data:
    vector<BYTE> fileData(fileSize);
    file.read((char*) &fileData[0], fileSize);
    return fileData;
}

int main(int argc, const char * argv[]){
	vector<BYTE> vec= readFile(argv[1]);
	string str = "";
	for(int i=0;i<vec.size();i++){
		string mystr = bitset<8>(vec[i]).to_string<char,string::traits_type,string::allocator_type>();
		str += mystr;
	}
	// cout << str;
	map<char,int> mp;
	int i=0;
	int size = 0;
	for(;i<8;i++){
		size = size*2 + (str[i]-'0');
	}
	int n = str.length();
	for(int x = 0;x < size; x++){
		i = x*40 + 8;
		int char_ascii;
		for(int j=i;j<i+8;j++){
			char_ascii = char_ascii*2 + (str[j]-'0');
		}
		int char_freq = 0;
		for(int j=4;j>=1;j--){
			for(int k=i+ 8*j ; k < i+ 8*(j+1); k++){
				char_freq = char_freq*2 + (str[k]-'0');
			}
		}
		mp.insert(make_pair(char(char_ascii),char_freq));
	}
	vector<char> data;
    vector<int> freq;

    map<char,int>:: iterator it;
    for (it = mp.begin();it != mp.end(); it++ ) {
        cout << it->first << ' ' << it->second << '\n';
        data.push_back(it->first);
        freq.push_back(it->second);
    }
    HuffmanCodes(data,freq,data.size());
    unordered_map<string,char>:: iterator it1;
    for (it1 = res.begin();it1 != res.end(); it1++ ) {
        cout << it1->second << " : " << it1->first << '\n';
    }
	i += 40;
	int padding = 0;
	for(int j=i;j<i+8;j++){
		padding = padding*2 + (str[j]-'0');
	}
	i += 8;

	fstream outFile("decompressed_file.txt", ios::in | ios::out | ios::binary | ios:: trunc);
    if (outFile.is_open()) {

        string s = "";
		while(i <= n-padding){
			s += str[i];
			if(res.find(s) != res.end()){
				outFile.put(res[s]);
				s = "";
			}
			i += 1;
		}
        outFile.close();
    }
    else {
        cout << "Error! File cannot be opened!" << endl;
    }

	return 0;
}