#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <fstream>
#include <algorithm>
#include <queue>
#include <stack>

using namespace std;


// structure MagicWord --MagicWord contains the magicword substring,
// the value of the substring and the next array of the substring
struct MagicWord{

	string subStr;
	int value;
	int len;
	vector<int> next;//the next array of the substring used for KMP

	MagicWord(){
		subStr = "";
		value = 0;
		len = 0;
	}

	MagicWord(string w,int v){
		subStr = w;
		value = v;
		len = w.size();
	}

	bool operator < (const MagicWord& right) const {  
	  return value < right.value;  
	}//asscend

	bool operator > (const MagicWord& right) const {  
	  return value > right.value;  
	}//desscend

};

struct Output{
	string outWord;
	int outPlace;
	Output(){
		outPlace = -1;
		outWord = "none";
	}
	Output(string w,int p){
		outWord = w;
		outPlace = p;
	}
};

struct Node{
	string nodeStr;
	vector<Output> vecOut;
	int nodeValue;
	Node(){
		nodeStr = "";
		nodeValue = 0;
	}
	Node(string s, vector<Output> v, int total = 0){
		nodeStr = s;
		vecOut = v;
		nodeValue = total;
	}
	Node(string s, int total = 0){
		nodeStr = s;
		nodeValue = total;
	}
};


// Summary: calculate the next[]
// Parameters:
//      str:the string want to calculate next[].*    
//      next : the int next array to calculate.
//		len: the size of the string.
// Return : none.
void calNext(const char *str, int *next,int len){
	// init next[0]
	next[0] = -1;
	int k = -1;
	for (int q=1;q<=len -1 ;q++){
		while(k > -1 && str[k + 1]!= str[q]){
			k = next[k];
		}
		if(str[k+1]==str[q]){
			k = k+1;
		}
		next[q] = k;
	}
}

// Summary: match the substring by KMP
// Parameters:
//      str:the father string to match. 
//      subStr : the subStr to match.
//		next: the next array of the subStr.
// Return : the first index of the matched substring(-1 if unmatched).
int KMP(string str,string subStr, vector<int> next){
	int slen = str.size();
	int subLen = subStr.size();
	int k = -1;
	for(int i=0;i<slen;i++){

		// unmatched and move to next[k]
		while(k>-1 && subStr[k+1]!=str[i]){
			k = next[k];
		}
		// matched a char and move
		if(subStr [k + 1] == str[i]){
			k = k + 1;
		}
		// matched all
		if(k == subLen-1){
			return i-subLen+1;
		}
	}
	return -1;
}

// Summary: match the substring, remove it and add the value
// Parameters:
//      s:the reference of the father string to match, may change when return. 
//      mw : the class contain the subStr, next array of the subStr.
//		totalValue: the reference of the current value.
// Return : the first index of the matched substring(-1 if unmatched).
int removeSubStr(string &s,MagicWord mw, int &totalValue){
	//remove the substring
	string tmps = s;
	int start = KMP(tmps,mw.subStr,mw.next);
	//cout<<"KMP = "<<start<<endl;
	if(start != -1){
		s = s.substr(0,start)+s.substr(start+mw.len,s.size());
		totalValue += mw.value;
	}
	return start;
}

// Summary: find the best solution by dfs
// Parameters:
//      root:the root Node contains the initial string and value. 
//      vecWord : the vector contains all the MagicWord.
// Return : the Node with the max value.
Node dfsFindMax(Node root,vector<MagicWord> vecWord){
	//init the solution maxNode
    Node maxNode = root;

    //cout<<"s now = "<<root.str<<endl;
    //queue<Node> qNode;

    //stack to cotain the nodes when doing dfs
    stack<Node> stNode;

    //init the stack
    stNode.push(root);
    while(!stNode.empty()){

    	//current node
    	Node tmpN;
    	tmpN = stNode.top();
    	stNode.pop();

    	//update the solution maxNode
    	if(tmpN.nodeValue > maxNode.nodeValue){
    		maxNode = tmpN;
    	}

    	//get all the child neighbor node of current node
    	for(int i = 0;i < vecWord.size();i++){

	    	//cout<<"substr["<<i<<"] = "<<vecWord[i].subStr<<" len = "<<vecWord[i].len<<endl;
	    	// for (int j=0;j<tmpWord[i].next.size();j++){
	    	// 	cout<<"next["<<j<<"] = "<<tmpWord[i].next[j]<<endl;
	    	// }

	    	//temp variables
	    	string nodeStr = tmpN.nodeStr;
	    	int nodeValue = tmpN.nodeValue;
	    	vector<Output> vecOut = tmpN.vecOut;

	    	//the string of current node contains the substring
	    	int outPlace = removeSubStr(nodeStr,vecWord[i],nodeValue);
	    	if( outPlace != -1){
	    		//find a new child neighbor node and put it into stack
	    		Output output(vecWord[i].subStr,outPlace);
	    		vecOut.push_back(output);
	    		Node nghbr(nodeStr,vecOut,nodeValue);
	    		// cout<<"the str now = "<<nghbr.nodeStr<<" value = "<<nghbr.nodeValue<<endl;
	    		// for(int i = 0; i < nghbr.vecOut.size();i++){
			    // 	cout<<"out = "<<nghbr.vecOut[i].outWord<<" "<<nghbr.vecOut[i].outPlace<<endl;
			    // }
	    		stNode.push(nghbr);
	    	}
	    	
	    }

	    // while(!stNode.empty()){
	    // 	Node tmpN;
	    // 	tmpN = stNode.top();
	    // 	stNode.pop();
	    // 	cout<<"nodeStr now = "<<tmpN.nodeStr<<" nodeValue now = "<<tmpN.nodeValue<<endl;
	    // }
    }

    //as it means
    return maxNode;
}

// Summary: read the file
// Parameters:
//      inFile:the path string of the file. 
// Return : none.
int n,k;
string s;
vector<MagicWord> vecWord;
void readFileByStr(string inFile){
	if(inFile.empty()){
    	inFile = "in";
    }

    ifstream inputFile(inFile.c_str());


    inputFile>>n>>k>>s;
    // cout<<n<<endl;
    // cout<<k<<endl;
    // cout<<s<<endl;
    
    

    for(int i=0;i<k;i++){
    	string tmps;
    	double tmpd;

    	inputFile>>tmps>>tmpd;
    	MagicWord mw(tmps,tmpd);
    	
    	//calculate the next array
    	int *nxt = new int [mw.len];
    	calNext(mw.subStr.c_str(),nxt,mw.subStr.size());
    	for(int j = 0;j<mw.len;j++){
    		//cout<<"nxt["<<i<<"] = "<<nxt[i]<<endl;
    		mw.next.push_back(nxt[j]);
    	}

    	//push to the vector
    	vecWord.push_back(mw);
    }
}

// Summary: show the route and value of the Node
// Parameters:
//      maxNode:the best nodes. 
// Return : none.
void showAnswer(Node maxNode){
	// for(int i = 0; i < maxNode.vecOut.size();i++){
	// 	cout<<"out = "<<maxNode.vecOut[i].outPlace<<" "<<maxNode.vecOut[i].outWord<<endl;
	// }
	// cout<<"current string is: "<<maxNode.nodeStr<<" the value is: "<<maxNode.nodeValue<<endl;
	for(int i = 0; i < maxNode.vecOut.size();i++){
		cout<<maxNode.vecOut[i].outPlace<<" "<<maxNode.vecOut[i].outWord<<endl;
	}
	cout<<maxNode.nodeValue<<endl;
}

class Process{
	public:
		int n,k;
		string s;

		Process(){

		}
};

int main(int argc,char* argv[]){
	string inFile;
    string type;  
    // string dbPath;
    // string outFile;  
    // string sep("\t");
    // int col = -1; 

    if(argc > 1){
        type = string(argv[1]);  
    }  
    int ch;  
    opterr = 0; 

    //get the argv from command line
    while ((ch = getopt(argc, argv, "d:i:o:s:c:")) != -1) {  
        switch (ch) {
        	case 'i': 
				inFile = string(optarg); 
				//cout<<"-i is :"<<inFile<<endl;
				break;  
    //         case 'd': 
				// dbPath = string(optarg); 
				// //cout<<"-d is :"<<dbPath<<endl;
				// break;  
            
    //         case 'o':
				// outFile = string(optarg);
				// //cout<<"-o is :"<<outFile<<endl;
				// break;  
    //         case 's': 
				// sep = string(optarg); 
				// //cout<<"-s is :"<<sep<<endl;
				// break;  
    //         case 'c': 
				// col = atoi(optarg); 
				// //cout<<"-c is :"<<col<<endl;
				// break;  
        }
    }

    readFileByStr(inFile);

    sort(vecWord.begin(), vecWord.end(),less<MagicWord>());

    //init the root node
    Node root(s,0);
    Node maxNode = dfsFindMax(root,vecWord);
    showAnswer(maxNode);
    
	return 0;
}

