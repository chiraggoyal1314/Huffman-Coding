#include<iostream>
#include<fstream>
#include<bitset>
#include<queue>
#include<unordered_map>
#include <map>
using namespace std;

unordered_map< char, string> mp;

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
        mp.insert(make_pair(root->data,str));
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


map<char,int> findFrequency(string str)
{
    map<char, int> M;
    for (int i = 0; str[i]; i++)
    {
        if (M.find(str[i]) == M.end())
        {
            M.insert(make_pair(str[i], 1));
        }
        else
        {
            M[str[i]]++;
        }
    }
    // unordered_map<char,int>:: iterator it;
    // for (it = M.begin();it != M.end(); it++ ) {
    //     cout << it->first << ' ' << it->second << '\n';
    // }
    return M;
}



int main(int argc, const char * argv[]){
    string res = "";
    string line;
    ifstream fin;
    fin.open(argv[1]);
    if (fin.is_open()) {

        while(getline(fin, line)){
            res += line;
            res += "\n";
        }
    }
    fin.close();
    // cout << res << endl;
    map<char,int> M = findFrequency(res);

    vector<char> data;
    vector<int> freq;

    map<char,int>:: iterator it;
    for (it = M.begin();it != M.end(); it++ ) {
        cout << it->first << ' ' << it->second << '\n';
        data.push_back(it->first);
        freq.push_back(it->second);
    }

    int size = data.size();
    cout << size << endl;
    HuffmanCodes(data,freq,size);

    unordered_map<char,string>:: iterator it1;
    for (it1 = mp.begin();it1 != mp.end(); it1++ ) {
        cout << it1->first << " : " << it1->second << '\n';
    }
    int n = res.length();
    string s = "";
    int i=0;
    while(i < n){
        s += mp[res[i]];
        i++;
    }
    // cout << s << endl;
    int x = 0;
    if(s.length()%8 != 0){
        x = s.length()%8;
        x = 8-x;
        int a = x;
        while(a--)
            s += '0';
    }

    fstream outFile("outFile.bin", ios::in | ios::out | ios::binary | ios:: trunc);
    if (outFile.is_open()) {

        outFile.put(size);
        for(int i=0;i<size;i++){
            outFile.put(data[i]);
            // outFile.put(':');
            // outFile.put(freq[i]);
            outFile.write((const char*)&freq[i],sizeof(int));
        }

        outFile.put(x);
        for(int i=0;i<s.length();i+=8){
            int y = 0;
            for(int j=i;j<i+8;j++){
                y = y*2 + (s[j]-'0');
            }
            char ascii = y;
            outFile.put(ascii);
        }
        outFile.close();
    }
    else {
        cout << "Error! File cannot be opened!" << endl;
    }

    return 0;
}
